// Ante-Millennium Operating System - antem_stddef
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


// Definizioni standard indipendenti dal compilatore ospite

#ifndef _ANTEM_STDDEF_H
#define _ANTEM_STDDEF_H

// In un OS a 32-bit, la dimensione degli oggetti in memoria è un intero senza segno
typedef unsigned int size_t;

// Il puntatore nullo non è altro che uno zero travestito da puntatore
#define NULL ((void*)0)

#endif