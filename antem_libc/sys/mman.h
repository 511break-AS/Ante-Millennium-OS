#ifndef _ANTEM_MMAN_H
#define _ANTEM_MMAN_H
#include "../stddef.h"

// Le flag che TCC vuole vedere
#define PROT_READ  1
#define PROT_WRITE 2
#define PROT_EXEC  4

// Mock della funzione di protezione (il nostro OS lascia l'heap sempre eseguibile)
int mprotect(void *addr, size_t len, int prot);

#endif