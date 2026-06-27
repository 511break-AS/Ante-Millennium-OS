// Ante-Millennium Operating System - Anchor Test App
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

// COMPILAZIONE + CONVERSIONE + INSTALLAZIONE

// NUOVI COMANDI
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/stdio.c -o stdio.o
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/stdlib.c -o stdlib.o
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/string.c -o string.o

// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -I./antem_libc -c test_anchors.c -o test_anchors.o

// i686-elf-ld -T app_linker.ld test_anchors.o stdio.o stdlib.o string.o -o test_anchors.bin

// cat header_gui.bin test_anchors.bin > test_anchors.edxi 
// e2cp test_anchors.edxi disk.img:/test_anchors.edxi


// Programma di collaudo per la griglia a 9 punti

#include "antem_libc/stdio.h"
#include "antem_libc/stdlib.h"

int main_win = 0; 
uint32_t bg_color = 0xFF386DA4; // Sfondo Blu

// Callback condivisa per i pulsanti (cambia il colore di sfondo per dare feedback visivo)
void cmd_click() {
    if (bg_color == 0xFF386DA4) bg_color = 0xFF008080;
    else bg_color = 0xFF386DA4;
}

void main() {
    // Dimensione bottoni
    int w = 60; // Larghezza bottoni
    int h = 30; // Altezza bottoni

    while(1) {
        gui_poll_events();
        gui_set_context(main_win);
        gui_clear();

        // 1. Sfondo globale
        gui_panel(ANCHOR_TL, 2, 2, SIZE_FILL, SIZE_FILL, bg_color);
        
        // 2. Testi Centrali Informativi
        gui_text(ANCHOR_CC, 17, -25, "Test Griglia 9 Punti", 0xFFFFFFFF);
        gui_text(ANCHOR_CC, 22, -10, "Ridimensiona la finestra!", 0xFFC0C0C0);

        // 3. I NOVE BOTTONI CON I NOVE ANCORAGGI
        
        // RIGA ALTA (Top)
        gui_action_button(ANCHOR_TL, 10, 10, w, h, 0xFFC0C0C0, 0xFF000000, "TL", cmd_click);
        gui_action_button(ANCHOR_TC, 0,  10, w, h, 0xFFC0C0C0, 0xFF000000, "TC", cmd_click);
        gui_action_button(ANCHOR_TR, 10, 10, w, h, 0xFFC0C0C0, 0xFF000000, "TR", cmd_click);

        // RIGA CENTRALE (Center)
        gui_action_button(ANCHOR_LC, 10, 0, w, h, 0xFFC0C0C0, 0xFF000000, "LC", cmd_click);
        gui_action_button(ANCHOR_CC, 0,  30, w, h, 0xFFC0C0C0, 0xFF000000, "CC", cmd_click); // Abbassato un po' per non coprire il testo
        gui_action_button(ANCHOR_RC, 10, 0, w, h, 0xFFC0C0C0, 0xFF000000, "RC", cmd_click);

        // RIGA BASSA (Bottom)
        gui_action_button(ANCHOR_BL, 10, 10, w, h, 0xFFC0C0C0, 0xFF000000, "BL", cmd_click);
        gui_action_button(ANCHOR_BC, 0,  10, w, h, 0xFFC0C0C0, 0xFF000000, "BC", cmd_click);
        gui_action_button(ANCHOR_BR, 10, 10, w, h, 0xFFC0C0C0, 0xFF000000, "BR", cmd_click);

        gui_yield();
    }
}