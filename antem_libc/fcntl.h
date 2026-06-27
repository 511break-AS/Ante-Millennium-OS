// Ante-Millennium Operating System - antem_fcntl
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


// Costanti POSIX per il controllo dei file

#ifndef _ANTEM_FCNTL_H
#define _ANTEM_FCNTL_H

// --- Modalita' di accesso base ---
#define O_RDONLY    0x0000  // Solo lettura
#define O_WRONLY    0x0001  // Solo scrittura
#define O_RDWR      0x0002  // Lettura e scrittura

// --- Flag di creazione e comportamento ---
#define O_CREAT     0x0040  // Crea il file se non esiste
#define O_EXCL      0x0080  // Errore se il file esiste gia'
#define O_TRUNC     0x0200  // Svuota il file prima di scriverci
#define O_APPEND    0x0400  // Scrivi sempre alla fine del file

// Flag obsoleto di Windows/DOS che TCC a volte cerca per sicurezza
#define O_BINARY    0x0000  // (Su Linux/Ante-M i file sono sempre binari)

#endif
