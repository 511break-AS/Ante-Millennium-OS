// Ante-Millennium Operating System - antem_errno
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



// Gestione dei codici di errore standard

#ifndef _ANTEM_ERRNO_H
#define _ANTEM_ERRNO_H

// La variabile globale che TCC userà per segnalare gli errori
extern int errno;

// I codici di errore base che un compilatore si aspetta di trovare
#define ENOENT   2  // Nessun file o directory
#define EBADF    9  // Descrittore file errato
#define ENOMEM  12  // Memoria esaurita
#define EACCES  13  // Permesso negato
#define EINVAL  22  // Argomento non valido

#endif