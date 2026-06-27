// Ante-Millennium Operating System - antem_time
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


// Definizioni standard per la gestione del tempo

#ifndef _ANTEM_TIME_H
#define _ANTEM_TIME_H

#include "stddef.h" // Mettiamo sempre stddef per sicurezza (NULL, size_t)

// Tipo base per rappresentare i secondi trascorsi (di solito dal 1970)
typedef unsigned int time_t;

// La classica struttura C che contiene la data scomposta
struct tm {
    int tm_sec;   // Secondi (0-59)
    int tm_min;   // Minuti (0-59)
    int tm_hour;  // Ore (0-23)
    int tm_mday;  // Giorno del mese (1-31)
    int tm_mon;   // Mese (0-11)
    int tm_year;  // Anno (dal 1900)
    int tm_wday;  // Giorno della settimana
    int tm_yday;  // Giorno dell'anno
    int tm_isdst; // Ora legale
};

// Dichiariamo che esiste una funzione time() (la implementeremo se TCC la esige)
time_t time(time_t *tloc);

struct tm *localtime(const time_t *timer);

#endif