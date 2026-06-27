// Ante-Millennium Operating System - antem_ucontext
// Stub per i contesti di esecuzione utente

#ifndef _ANTEM_UCONTEXT_H
#define _ANTEM_UCONTEXT_H

// Costanti dei Segnali e delle flag
#define SIGFPE  8
#define SIGILL  4
#define SIGSEGV 11
#define SIGBUS  10
#define SIGABRT 6
#define SA_SIGINFO   4
#define SA_RESETHAND 0x80000000

// Tipi di errore per la divisione per zero
#define FPE_INTDIV 1
#define FPE_FLTDIV 2

// Nomi dei Registri
#define EIP 14
#define EBP 10

// La struttura siginfo_t ora ha il membro richiesto da TCC
typedef struct { 
    int si_code; 
} siginfo_t;

// Creiamo ESATTAMENTE la struttura chiamata "struct sigaction"
struct sigaction { 
    int sa_flags; 
    void* sa_mask; 
    void (*sa_sigaction)(int, siginfo_t*, void*); 
};

// Mock della struttura ucontext che calma TCC
typedef struct {
    struct {
        int gregs[20];
    } uc_mcontext;
} ucontext_t;

// Mock per sigemptyset e sigaction
int sigemptyset(void *set);
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

#endif