// Ante-Millennium OS - TCC Wrapper
// A fork of the Tiny C Compiler (TCC), originally created by Fabrice Bellard.

// COME COMPILARLO


// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/stdio.c -o stdio.o
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/stdlib.c -o stdlib.o
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -I./antem_libc -I./am-tcc -c am-tcc/antem_tcc.c -o tcc.o
// i686-elf-ld -T app_linker.ld tcc.o stdio.o stdlib.o string.o -o tcc.bin
// cat header.bin tcc.bin > tcc.edxi
// e2cp tcc.edxi disk.img:/tcc.edxi

#include "../antem_libc/stdio.h"
#include "../antem_libc/stdlib.h"
#include "../antem_libc/string.h"
#include "../antem_libc/time.h"
#include "../antem_libc/sys/ucontext.h" // Per i tipi di ucontext e sigaction
#include "../antem_libc/sys/time.h"

#define ONE_SOURCE 1
#define TCC_TARGET_I386 1
#define CONFIG_TCCDIR "/system/lib/tcc"
#define CONFIG_TCC_SYSINCLUDEPATHS "/system/include"
#define RTLD_DEFAULT ((void *)0)

// ---------------------------------------------------------
// LA SUPER-FABBRICA DEI MOCK: Facciamo fessi il compilatore!
// ---------------------------------------------------------

// --- I Flussi della console (stdin, stdout, stderr) ---
// Creiamo 3 "file" finti.
FILE dummy_stdout, dummy_stderr, dummy_stdin;
FILE *stdout = &dummy_stdout;
FILE *stderr = &dummy_stderr;
FILE *stdin = &dummy_stdin;



// --- Le Variadiche e I/O Formattato (BULLETPROOF!) ---
int vsnprintf(char *str, size_t size, const char *format, __builtin_va_list args) {
    int buf_idx = 0;
    if (!str || size == 0) return 0;
    for (int i = 0; format[i] != '\0' && buf_idx < size - 1; i++) {
        if (format[i] == '%') {
            i++;
            // Salta i modificatori avanzati ignorandoli (es: 02, l, h, ., *, -)
            while (format[i] && (format[i] == '0' || format[i] == '1' || format[i] == '2' || format[i] == '3' ||
                   format[i] == '4' || format[i] == '5' || format[i] == '6' || format[i] == '7' ||
                   format[i] == '8' || format[i] == '9' || format[i] == '.' || format[i] == '-' ||
                   format[i] == '+' || format[i] == ' ' || format[i] == 'l' || format[i] == 'L' ||
                   format[i] == 'h' || format[i] == 'z' || format[i] == 't' || format[i] == 'j' || format[i] == '*')) {
                if (format[i] == '*') __builtin_va_arg(args, int); // Consuma l'argomento extra per l'asterisco
                i++;
            }

            if (format[i] == 'd' || format[i] == 'i' || format[i] == 'u') {
                int num = __builtin_va_arg(args, int);
                char num_str[32]; itoa(num, num_str, 10);
                for (int j = 0; num_str[j] != '\0' && buf_idx < size - 1; j++) str[buf_idx++] = num_str[j];
            } else if (format[i] == 's') {
                char* s = __builtin_va_arg(args, char*);
                if (!s) s = "(null)";
                for (int j = 0; s[j] != '\0' && buf_idx < size - 1; j++) str[buf_idx++] = s[j];
            } else if (format[i] == 'c') {
                str[buf_idx++] = (char)__builtin_va_arg(args, int);
            } else if (format[i] == 'x' || format[i] == 'X' || format[i] == 'p') {
                int num = __builtin_va_arg(args, int);
                char num_str[32]; itoa(num, num_str, 16);
                for (int j = 0; num_str[j] != '\0' && buf_idx < size - 1; j++) str[buf_idx++] = num_str[j];
            } else if (format[i] == '%') {
                str[buf_idx++] = '%';
            } else {
                // Se c'è un flag non riconosciuto, stampiamo '?' e salviamo il resto del messaggio!
                str[buf_idx++] = '?';
            }
        } else {
            str[buf_idx++] = format[i];
        }
    }
    str[buf_idx] = '\0';
    return buf_idx;
}

int sprintf(char *str, const char *format, ...) {
    __builtin_va_list args; __builtin_va_start(args, format);
    int res = vsnprintf(str, 999999, format, args);
    __builtin_va_end(args); return res;
}

int snprintf(char *str, size_t size, const char *format, ...) {
    __builtin_va_list args; __builtin_va_start(args, format);
    int res = vsnprintf(str, size, format, args);
    __builtin_va_end(args); return res;
}



// --- I/O COLLEGATO AL TERMINALE E AI FILE (ANTE-M OS) ---

int vfprintf(FILE *stream, const char *format, __builtin_va_list arg) {
    char buf[512]; 
    int len = vsnprintf(buf, 512, format, arg);
    
    // Se lo stream è il terminale, stampa a schermo
    if (stream == stdout || stream == stderr) {
        print(buf);
    } 
    // Se lo stream è un vero file aperto in scrittura (mode == 2)
    else if (stream && stream->mode == 2) {
        for (int i = 0; i < len && stream->cursor < 300000; i++) {
            stream->buffer[stream->cursor++] = buf[i];
        }
        // Estendiamo la dimensione totale del file se abbiamo superato il vecchio limite
        if (stream->cursor > stream->size) {
            stream->size = stream->cursor;
        }
    }
    return len;
}

int fprintf(FILE *stream, const char *format, ...) {
    __builtin_va_list args; 
    __builtin_va_start(args, format);
    int len = vfprintf(stream, format, args);
    __builtin_va_end(args); 
    return len;
}

// --- Operazioni su File ---

int fputs(const char *str, FILE *stream) { 
    if (stream == stdout || stream == stderr) {
        print(str);
    } 
    else if (stream && stream->mode == 2) {
        int i = 0;
        // Scrive la stringa byte per byte finché c'è spazio nel buffer di 300KB
        while (str[i] != '\0' && stream->cursor < 300000) {
            stream->buffer[stream->cursor++] = str[i++];
        }
        if (stream->cursor > stream->size) {
            stream->size = stream->cursor;
        }
    }
    return 0; 
}

int fputc(int chara, FILE *stream) { 
    if (stream == stdout || stream == stderr) {
        char buf[2] = {(char)chara, '\0'}; 
        print(buf); 
    } 
    else if (stream && stream->mode == 2 && stream->cursor < 300000) {
        // Accoda un singolo byte binario/carattere
        stream->buffer[stream->cursor++] = (char)chara;
        if (stream->cursor > stream->size) {
            stream->size = stream->cursor;
        }
    }
    return chara; 
}

int fflush(FILE *stream) { return 0; }
int sscanf(const char *str, const char *format, ...) { return 0; }
int remove(const char *pathname) { return 0; }
int unlink(const char *pathname) { return 0; }
void *fdopen(int fd, const char *mode);
char *getcwd(char *buf, size_t size) { 
    if (buf && size > 1) { buf[0] = '/'; buf[1] = '\0'; return buf; }
    return 0;
}
int execvp(const char *file, char *const argv[]) { return -1; }



// --- Il Ponte Magico POSIX <-> Ante-M OS (STATIC BUFFER) ---
typedef struct {
    int used;
    int mode; // 1 = Lettura, 2 = Scrittura
    int size;
    int cursor;
    char buffer[200000]; // 200 KB fissa in RAM per bypassare l'Heap frammentato!
    char filename[64];
} TCC_FILE_BUFFER;

TCC_FILE_BUFFER tcc_fds[5] = {0}; 

int open(const char *pathname, int flags, ...) { 
    int fd = -1;
    for(int i = 0; i < 5; i++) { if(tcc_fds[i].used == 0) { fd = i; break; } }
    if (fd == -1) return -1; // Troppi file aperti
    
    tcc_fds[fd].used = 1;
    tcc_fds[fd].cursor = 0;
    tcc_fds[fd].size = 0;
    
    int n = 0; 
    while(pathname[n] && n < 63) { tcc_fds[fd].filename[n] = pathname[n]; n++; }
    tcc_fds[fd].filename[n] = '\0';
    
    // Se i flag indicano scrittura
    if ((flags & 3) != 0 || (flags & 0x40) || (flags & 0x200)) {
        tcc_fds[fd].mode = 2; 
        return fd + 3; // +3 per non confondere TCC con i canali standard (0, 1, 2)
    } 
    // Lettura
    else {
        tcc_fds[fd].mode = 1; 
        uint32_t actual_size = 0;
        load_file(pathname, tcc_fds[fd].buffer, &actual_size);
        
        if (actual_size == 0) {
            tcc_fds[fd].used = 0;
            return -1; // File non trovato o vuoto
        }
        tcc_fds[fd].size = actual_size;
        return fd + 3;
    }
}

typedef int ssize_t;
ssize_t read(int fd, void *buf, size_t count) { 
    int idx = fd - 3;
    if (idx >= 0 && idx < 5 && tcc_fds[idx].used && tcc_fds[idx].mode == 1) {
        int bytes_to_read = count;
        if (tcc_fds[idx].cursor + bytes_to_read > tcc_fds[idx].size) {
            bytes_to_read = tcc_fds[idx].size - tcc_fds[idx].cursor;
        }
        if (bytes_to_read <= 0) return 0;
        
        char* dest = (char*)buf;
        for(int i=0; i<bytes_to_read; i++) {
            dest[i] = tcc_fds[idx].buffer[tcc_fds[idx].cursor++];
        }
        return bytes_to_read;
    }
    return 0; 
}

 
ssize_t write(int fd, const void *buf, size_t count) { 
    int idx = fd - 3;
    if (idx >= 0 && idx < 5 && tcc_fds[idx].used && tcc_fds[idx].mode == 2) {
        int bytes_to_write = count;
        if (tcc_fds[idx].cursor + bytes_to_write > 200000) {
            bytes_to_write = 200000 - tcc_fds[idx].cursor;
        }
        if (bytes_to_write <= 0) return 0;
        
        const char* src = (const char*)buf;
        for(int i=0; i<bytes_to_write; i++) {
            tcc_fds[idx].buffer[tcc_fds[idx].cursor++] = src[i];
        }
        if (tcc_fds[idx].cursor > tcc_fds[idx].size) {
            tcc_fds[idx].size = tcc_fds[idx].cursor;
        }
        return bytes_to_write;
    }
    return 0; 
}

int close(int fd) { 
    int idx = fd - 3;
    if (idx >= 0 && idx < 5 && tcc_fds[idx].used) {
        if (tcc_fds[idx].mode == 2) {
            // SALVA DIRETTAMENTE SU DISCO ALLA CHIUSURA!
            save_file(tcc_fds[idx].filename, tcc_fds[idx].buffer, tcc_fds[idx].size);
        }
        tcc_fds[idx].used = 0;
        return 0;
    }
    return -1; 
}

int lseek(int fd, int offset, int whence) { 
    int idx = fd - 3;
    if (idx >= 0 && idx < 5 && tcc_fds[idx].used) {
        int new_pos = 0;
        if (whence == 0) new_pos = offset; 
        else if (whence == 1) new_pos = tcc_fds[idx].cursor + offset;
        else if (whence == 2) new_pos = tcc_fds[idx].size + offset; 
        
        if (new_pos < 0) new_pos = 0;
        if (new_pos > 200000) new_pos = 200000;
        
        tcc_fds[idx].cursor = new_pos;
        return new_pos;
    }
    return -1; 
}

// --- INIZIO FIX FDOPEN E FCLOSE ---

void *fdopen(int fd, const char *mode) { 
    int idx = fd - 3;
    if (idx >= 0 && idx < 5 && tcc_fds[idx].used) {
        
        FILE* f = (FILE*)malloc(sizeof(FILE));
        if (!f) return 0;
        
        // IL TRUCCO: Invece di fare malloc(300000) rischiando l'Out-Of-Memory,
        // ricicliamo la RAM statica del Ponte Magico!
        f->buffer = tcc_fds[idx].buffer;
        f->cursor = tcc_fds[idx].cursor;
        f->size = tcc_fds[idx].size;
        f->mode = tcc_fds[idx].mode; 
        
        int n = 0;
        while (tcc_fds[idx].filename[n] != '\0' && n < 63) {
            f->filename[n] = tcc_fds[idx].filename[n];
            n++;
        }
        f->filename[n] = '\0';
        
        // Lasciamo tcc_fds[idx].used a 1 in modo che se TCC fa stranezze 
        // e chiama close(fd) invece di fclose(f), il file si salva comunque!
        return f;
    }
    return 0; 
}

// Sovrascriviamo la fclose di stdio.c per proteggere il nostro buffer statico!
// Rinominiamo la funzione per evitare conflitti con stdio.o
void antem_fclose(FILE *stream) {
    if (!stream) return;
    if (stream == stdout || stream == stderr || stream == stdin) return;
    
    // Se stavamo scrivendo un file, è il momento di salvarlo su disco!
    if (stream->mode == 2) {
        save_file(stream->filename, stream->buffer, stream->size);
    }
    
    // Sicurezza: Controlliamo se il buffer appartiene al Ponte Magico
    int is_static = 0;
    for (int i = 0; i < 5; i++) {
        if (stream->buffer == tcc_fds[i].buffer) {
            is_static = 1;
            tcc_fds[i].used = 0; // Liberiamo lo slot del file descriptor
            break;
        }
    }
    
    // IMPORTANTISSIMO: Se il buffer era il nostro array statico, NON fare free()!
    if (!is_static) {
        free(stream->buffer);
    }
    
    // Liberiamo solo la piccola struct FILE (~80 bytes)
    free(stream);
}

// Dirottiamo magicamente tutte le chiamate 'fclose' di TCC verso la nostra funzione!
#define fclose antem_fclose

// --- FINE FIX ---


// --- Strumenti Matematici (Ora supportano l'Esadecimale!) ---
double ldexp(double x, int exp) { return x; }

unsigned long int strtoul(const char *nptr, char **endptr, int base) { 
    unsigned long int res = 0;
    if (nptr[0] == '0' && (nptr[1] == 'x' || nptr[1] == 'X')) {
        nptr += 2; // Salta il prefisso 0x
        while ((*nptr >= '0' && *nptr <= '9') || (*nptr >= 'a' && *nptr <= 'f') || (*nptr >= 'A' && *nptr <= 'F')) {
            int val = (*nptr >= 'a') ? *nptr - 'a' + 10 : (*nptr >= 'A') ? *nptr - 'A' + 10 : *nptr - '0';
            res = res * 16 + val;
            nptr++;
        }
    } else {
        while(*nptr >= '0' && *nptr <= '9') { res = res * 10 + (*nptr - '0'); nptr++; }
    }
    if(endptr) *endptr = (char*)nptr;
    return res; 
}
double strtod(const char *nptr, char **endptr) { if(endptr) *endptr = (char*)(nptr + 1); return 0.0; }
float strtof(const char *nptr, char **endptr) { if(endptr) *endptr = (char*)(nptr + 1); return 0.0f; }
long double strtold(const char *nptr, char **endptr) { if(endptr) *endptr = (char*)(nptr + 1); return 0.0; }
unsigned long long int strtoull(const char *nptr, char **endptr, int base) { return (unsigned long long)strtoul(nptr, endptr, base); }
long long int strtoll(const char *nptr, char **endptr, int base) { return (long long)strtoul(nptr, endptr, base); }
long int strtol(const char *nptr, char **endptr, int base) { return (long int)strtoul(nptr, endptr, base); }




// --- Memoria e Array ---
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {}
int mprotect(void *addr, size_t len, int prot) { return 0; }



// Inganniamo TCC rinominando "errno" in una variabile che controlliamo noi
int antem_dummy_errno = 0;
#define errno antem_dummy_errno


// --- Variabili d'ambiente e Dynamic Linking ---
char *getenv(const char *name) { return ""; }
void* dlopen(const char* filename, int flag) { return 0; }
void* dlsym(void* handle, const char* symbol) { return 0; }
int dlclose(void* handle) { return 0; }

// --- Gestione Tempo e Segnali ---
time_t time(time_t *tloc) { return 0; }
struct tm dummy_time = {0};
struct tm *localtime(const time_t *timer) { return &dummy_time; }
int gettimeofday(struct timeval *tv, struct timezone *tz) { return 0; }
int sigemptyset(void *set) { return 0; }
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) { return 0; }

// --- Funzioni Segrete di GCC per i 64-bit ---
// GCC quando compila codice a 32-bit su un host a 64-bit, a volte inserisce di nascosto queste funzioni per calcolare divisioni enormi.
unsigned long long __udivdi3(unsigned long long a, unsigned long long b) { return a / b; }
unsigned long long __umoddi3(unsigned long long a, unsigned long long b) { return a % b; }

// ---------------------------------------------------------
// FINE DEI MOCK
// ---------------------------------------------------------



// 1. Rinominiamo il main originale di TCC per non farlo scontrare col nostro!
#define main tcc_main 

// 2. Includiamo tutto il bestione
#include "tcc.c"

// 3. ANNULLIAMO la macro!
#undef main

// Dichiariamo a GCC che tcc_main esiste
int tcc_main(int argc, char **argv);

// 4. Creiamo il NOSTRO main per Ante-M OS!
void main() {
    clear();
    print("========================================");
    print("             Tiny C Compiler            ");
    print("         for Ante-Millennium OS         ");
    print("========================================");

    // Diamo gli ordini a TCC
    char a0[] = "tcc";
    char a1[] = "-c";          // Ordine: Genera Object File (.o)
    char a2[] = "-nostdinc";   
    char a3[] = "-nostdlib";   
    char a4[] = "-o";          // NUOVO: Vogliamo decidere noi il nome!
    char a5[] = "/hello.o";    // NUOVO: Percorso assoluto con lo slash
    char a6[] = "/hello.c";    // Il codice sorgente
    char* fake_argv[] = { a0, a1, a2, a3, a4, a5, a6, NULL };
    
    // Chiamiamo il VERO main di TCC con i 7 argomenti!
    tcc_main(7, fake_argv);

    print("\n[TCC ha terminato il lavoro]");
}