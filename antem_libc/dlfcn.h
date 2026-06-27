// Ante-Millennium Operating System - antem_dlfcn
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

// Stub per il caricamento dinamico delle librerie (Dynamic Linking)

#ifndef _ANTEM_DLFCN_H
#define _ANTEM_DLFCN_H

#include "stddef.h"

// Flag classici richiesti dallo standard POSIX per dlopen
#define RTLD_LAZY   1
#define RTLD_NOW    2
#define RTLD_GLOBAL 4

// Prototipi delle funzioni fittizie. 
// Se TCC proverà a chiamarle davvero, il Linker ci avviserà!
void* dlopen(const char* filename, int flag);
const char* dlerror(void);
void* dlsym(void* handle, const char* symbol);
int dlclose(void* handle);

#endif