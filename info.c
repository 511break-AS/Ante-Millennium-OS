// Ante-Millennium Operating System - System Info App
// Copyright (C) 2026  Alberto Sanfelice

// COMPILAZIONE + CONVERSIONE + INSTALLAZIONE
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/stdio.c -o stdio.o
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/stdlib.c -o stdlib.o
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/string.c -o string.o
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -I./antem_libc -c info.c -o info.o


// i686-elf-ld -T app_linker.ld info.o stdio.o stdlib.o string.o -o info.bin
// cat header_gui.bin info.bin > info.edxi 
// e2cp info.edxi disk.img:/system/sys_apps/info.edxi 


#include "antem_libc/stdio.h"
#include "antem_libc/stdlib.h"

int main_win = 0; 


// Callback per chiudere l'applicazione
void cmd_close() {
    exit(0); 
}

void main() {
    // Fissa istantaneamente le dimensioni base al centro dello schermo
    gui_set_window_size(250, 180);
    
    while(1) {
        gui_poll_events();
        gui_set_context(main_win);
        gui_clear();

        // Testi Ancorati in Alto al Centro in GRASSETTO
        gui_text_bold(ANCHOR_TC, 0, 20, "Ante-Millennium", 0xFF000000); 
        gui_text_bold(ANCHOR_TC, 0, 40, "Alpha", 0xFF000000);           
        
        gui_text_bold(ANCHOR_TC, 0, 80, "Developed by Alberto Sanfelice", 0xFF000000); 

        // Pulsante di Chiusura
        gui_action_button(ANCHOR_BC, 0, 10, 150, 25, 0xFFC0C0C0, 0xFF000000, "Close", cmd_close);

        gui_yield();
    }
}