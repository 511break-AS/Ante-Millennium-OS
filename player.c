// Ante-Millennium Operating System - Media Player
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
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/stdio.c -o stdio.o 
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -c antem_libc/stdlib.c -o stdlib.o
// i686-elf-gcc -m32 -ffreestanding -fno-pie -fno-pic -nostdlib -O0 -I./antem_libc -c player.c -o player.o
// i686-elf-ld -T app_linker.ld player.o stdio.o stdlib.o -o player.bin
// cat header_gui.bin player.bin > player.edxi 
// e2cp player.edxi disk.img:/player.edxi 

#include "antem_libc/stdio.h"
#include "antem_libc/stdlib.h"

// Variabili globali dell'App
uint32_t tema_sfondo = 0xFFC0C0C0; 
int main_win = 0; 
int about_win = -1; 
char audio_path[64] = "/system/media/startup.wav";
int is_playing = 0;

// ==========================================
// LE FUNZIONI (CALLBACKS) CHE REAGISCONO AI CLICK
// ==========================================
void cmd_play() {
    is_playing = 1;
    audio_play(audio_path);
}

void cmd_stop() {
    is_playing = 0;
    audio_stop(); 
}

void cmd_about() {
    if (about_win == -1) about_win = gui_spawn_window("Informazioni", 250, 180);
}

void cmd_theme_light() { tema_sfondo = 0xFFE0E0E0; }
void cmd_theme_orange()  { tema_sfondo = 0xFFFF9900; } // TEMA ARANCIONE


// ==========================================
// IL PROGRAMMA PRINCIPALE
// ==========================================
void main() {
    int primo_frame = 1; 

    while(1) {
        // 1. Legge globalmente mouse e tastiera!
        gui_poll_events();
        char tasto = get_key();
        
        gui_set_context(main_win);
        
        char temp_path[64];
        gui_get_text(2, temp_path); 

        if (primo_frame == 1) {
            primo_frame = 0; 
        } else {
            int i = 0;
            while(temp_path[i] != '\0' && i < 63) { audio_path[i] = temp_path[i]; i++; }
            audio_path[i] = '\0';
        }

        if (about_win != -1 && gui_is_window_open(about_win) == 0) {
            about_win = -1; 
        }

        // =======================================================
        // DISEGNO MAIN WINDOW
        // =======================================================
        gui_set_context(main_win);
        gui_clear();

        gui_panel(ANCHOR_TL, 2, 2, SIZE_FILL, SIZE_FILL, tema_sfondo);
        gui_text(ANCHOR_TL, 10, 15, "Lettore Audio AC'97", 0xFF000080);
        gui_textbox(ANCHOR_TL, 10, 45, 250, 24, 0xFFFFFFFF, 60, audio_path);
        
        // Verde scuro se in riproduzione, altrimenti verde acceso
        uint32_t play_color = is_playing ? 0xFF008000 : 0xFF00FF00; 
        
        // Verde per il Play, Rosso vivo (0xFFFF0000) per lo Stop
        gui_action_button(ANCHOR_BL, 10, 10, 80, 30, play_color, 0xFF000000, "PLAY", cmd_play);
        gui_action_button(ANCHOR_BL, 100, 10, 80, 30, 0xFFFF0000, 0xFFFFFFFF, "STOP", cmd_stop);
        gui_action_button(ANCHOR_BR, 10, 10, 60, 30, 0xFFC0C0C0, 0xFF000000, "About", cmd_about);


        // =======================================================
        // DISEGNO ABOUT WINDOW
        // =======================================================
        if (about_win != -1) {
            gui_set_context(about_win);
            gui_clear();
            gui_text(ANCHOR_TL, 20, 20, "Ante-M Media Player v1.0", 0xFF000080);
            gui_text(ANCHOR_TL, 20, 40, "Di Alberto Sanfelice", 0xFF000000);
            
            gui_action_button(ANCHOR_BL, 20, 45, 150, 25, 0xFFC0C0C0, 0xFF000000, "Usa Tema Chiaro", cmd_theme_light);
            // Sostituito Scuro con Arancione
            gui_action_button(ANCHOR_BL, 20, 10, 150, 25, 0xFFC0C0C0, 0xFF000000, "Usa Tema Arancione", cmd_theme_orange);
        }

        // Usiamo la nuova funzione dell'SDK invece della vecchia Syscall!
        gui_yield();
    }
}