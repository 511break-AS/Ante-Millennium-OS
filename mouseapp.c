// ==========================================
// 1. DICHIARAZIONI (Le "Promesse")
// ==========================================
void print(const char* str);
void clear_screen();

// La nostra struttura replicata nell'App!
typedef struct {
    int x;
    int y;
    int clicked;
} mouse_state_t;

void get_mouse(mouse_state_t* state);
void itoa(unsigned int num, char* str);
void yield_cpu();
// ==========================================
// 2. ENTRY POINT (Il primo codice fisico!)
// ==========================================
void main() {
    clear_screen();
    print("Test API Mouse (Syscall 5) Avviato!\n");
    print("Muovi il mouse e clicca...\n");

    mouse_state_t m;
    char out_buf[64];
    char num_buf[16];
    
    int prev_click = 0;

    // Un loop infinito velocissimo che ascolta il mouse
    while(1) {
        // Chiediamo al Kernel dove si trova il mouse in questo esatto millisecondo!
        get_mouse(&m);

        // Se l'utente ha appena cliccato DENTRO la finestra...
        if (m.clicked == 1 && prev_click == 0 && m.x != -1) {
            // Assembliamo la stringa "Click a X: 100, Y: 50"
            out_buf[0] = 'C'; out_buf[1] = 'l'; out_buf[2] = 'i'; out_buf[3] = 'c'; 
            out_buf[4] = 'k'; out_buf[5] = ' '; out_buf[6] = 'a'; out_buf[7] = ' ';
            out_buf[8] = 'X'; out_buf[9] = ':'; out_buf[10] = ' '; out_buf[11] = '\0';
            
            itoa((unsigned int)m.x, num_buf);
            int ob_idx = 11; int nb_idx = 0;
            while(num_buf[nb_idx] != '\0') { out_buf[ob_idx++] = num_buf[nb_idx++]; }
            
            out_buf[ob_idx++] = ','; out_buf[ob_idx++] = ' '; out_buf[ob_idx++] = 'Y'; 
            out_buf[ob_idx++] = ':'; out_buf[ob_idx++] = ' '; out_buf[ob_idx] = '\0';
            
            itoa((unsigned int)m.y, num_buf);
            nb_idx = 0;
            while(num_buf[nb_idx] != '\0') { out_buf[ob_idx++] = num_buf[nb_idx++]; }
            
            out_buf[ob_idx++] = '\n'; out_buf[ob_idx] = '\0';
            
            print(out_buf); // Stampiamo il risultato!
        }
        
        prev_click = m.clicked;
        
        // Mettiamo in pausa l'App per un millisecondo per non bruciare la CPU
        yield_cpu();
    }
}

// ==========================================
// 3. IMPLEMENTAZIONI DELLE API
// ==========================================
void print(const char* str) {
    asm volatile ("int $0x80" : : "a"(1), "b"((unsigned int)str));
}
void clear_screen() {
    asm volatile ("int $0x80" : : "a"(2));
}

// LA NUOVA SYSCALL!
void get_mouse(mouse_state_t* state) {
    asm volatile (
        "int $0x80"
        : 
        : "a"(5), "b"((unsigned int)state)
    );
}

// Funzione matematica di utilità inclusa nell'app
void itoa(unsigned int num, char* str) {
    if (num == 0) { str[0] = '0'; str[1] = '\0'; return; }
    int i = 0; unsigned int temp = num;
    while (temp > 0) { temp /= 10; i++; }
    str[i] = '\0';
    while (num > 0) { str[--i] = (num % 10) + '0'; num /= 10; }
}

void yield_cpu() { asm volatile ("int $0x80" : : "a"(6)); }