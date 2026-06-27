// Ante-Millennium Operating System - app
// Copyright (C) 2026  Alberto Sanfelice

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2
// of the License.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, see
// <https://www.gnu.org/licenses/>.

// app.c - La primissima Calcolatrice per Ante-M OS
#include "antem_libc/stdio.h"
#include "antem_libc/stdlib.h"

void main() {
    clear();
    
    print("========================================");
    print("        CALCOLATRICE DI ANTE-M OS       ");
    print("========================================");
    
    char buf1[32];
    char buf2[32];
    
    print("Inserisci il primo numero intero:");
    read_string(buf1, 32); // La nostra nuova funzione bloccante!
    
    print("Inserisci il secondo numero intero:");
    read_string(buf2, 32); 
    
    // Convertiamo le stringhe in numeri (grazie al nuovo atoi!)
    int n1 = atoi(buf1);
    int n2 = atoi(buf2);
    int sum = n1 + n2;
    
    print("----------------------------------------");
    
    // Sfruttiamo la magia della tua printf per iniettare il numero nel testo!
    printf("IL RISULTATO DELLA SOMMA E': %d", sum);
    
    print("========================================");
}