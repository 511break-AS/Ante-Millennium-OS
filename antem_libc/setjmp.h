// Ante-Millennium Operating System - antem_setjmp
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


// Gestione dei salti non locali (Exception handling in C)

#ifndef _ANTEM_SETJMP_H
#define _ANTEM_SETJMP_H

// In x86 a 32-bit, dobbiamo salvare lo stato di 6 registri della CPU.
// Creiamo un array di 6 interi (24 byte in totale) per conservarli.
typedef int jmp_buf[6];

// Sfruttiamo le funzioni "segrete" di GCC per teletrasportare la CPU
// senza dover scrivere mezza riga di Assembly a mano!
#define setjmp(env)       __builtin_setjmp(env)
#define longjmp(env, val) __builtin_longjmp(env, val)

#endif