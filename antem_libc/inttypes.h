// Ante-Millennium Operating System - antem_inttypes
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


// Tipi interi a grandezza fissa (Standard C99)

#ifndef _ANTEM_INTTYPES_H
#define _ANTEM_INTTYPES_H

// --- Interi esatti a 8-bit ---
typedef signed char int8_t;
typedef unsigned char uint8_t;

// --- Interi esatti a 16-bit ---
typedef short int16_t;
typedef unsigned short uint16_t;

// --- Interi esatti a 32-bit ---
typedef int int32_t;
// Nota: Abbiamo già definito uint32_t in stdio.h in passato. 
// Per evitare conflitti se includiamo entrambi, GCC ci perdona 
// la doppia definizione solo se è identica.
typedef unsigned int uint32_t; 

// --- Interi esatti a 64-bit ---
typedef long long int64_t;
typedef unsigned long long uint64_t;

// --- Interi grandi quanto un puntatore in memoria (32-bit per noi) ---
typedef int intptr_t;
typedef unsigned int uintptr_t;

#endif