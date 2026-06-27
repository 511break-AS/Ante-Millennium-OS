/* Ante-Millennium Operating System */
/* Copyright (C) 2026  Alberto Sanfelice */

/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License */
/* as published by the Free Software Foundation; version 2 */
/* of the License. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License */
/*  along with this program; if not, see */
/* <https://www.gnu.org/licenses/>. */


/* Costanti Multiboot */
.set ALIGN,    1<<0             /* Allinea i moduli caricati ai confini di pagina */
.set MEMINFO,  1<<1             /* Fornisci la mappa della memoria */
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002       /* Il numero magico che identifica il kernel */
.set CHECKSUM, -(MAGIC + FLAGS) /* Verifica di integrità */

/* L'intestazione che GRUB o QEMU andranno a cercare */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Prepariamo lo Stack (16 Kilobyte) */
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/* Il punto di ingresso vero e proprio */
.section .text
.global _start
.type _start, @function
_start:
    /* Impostiamo il puntatore dello stack */
    mov $stack_top, %esp

    /* Chiamiamo la nostra funzione in C */
    call kernel_main

    /* Se il kernel in C finisce, blocchiamo la CPU per sicurezza */
    cli
1:  hlt
    jmp 1b
