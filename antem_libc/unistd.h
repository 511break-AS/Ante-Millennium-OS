// Ante-Millennium Operating System - antem_unistd
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


// API POSIX standard (stub)

#ifndef _ANTEM_UNISTD_H
#define _ANTEM_UNISTD_H

#include "stddef.h" // Ci serve sempre size_t e NULL

// ssize_t è la versione "signed" (con segno) di size_t.
// Serve a TCC per capire se una lettura ha restituito un errore (-1)
// o il numero di byte letti. In un OS a 32-bit è un semplice int.
typedef int ssize_t;


ssize_t read(int fd, void *buf, size_t count);
int unlink(const char *pathname);
int open(const char *pathname, int flags, ...);
void *fdopen(int fd, const char *mode);
int lseek(int fd, int offset, int whence);
char *getcwd(char *buf, size_t size);

#endif