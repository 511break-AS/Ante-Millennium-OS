// Ante-Millennium Operating System - antem_sys_time
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

// Strutture POSIX per il tempo ad alta precisione

#ifndef _ANTEM_SYS_TIME_H
#define _ANTEM_SYS_TIME_H

// Struttura usata da gettimeofday() per precisione al microsecondo
struct timeval {
    long tv_sec;       // Secondi
    long tv_usec;      // Microsecondi
};

// Struttura per il fuso orario (spesso ignorata ma richiesta dalla firma)
struct timezone {
    int tz_minuteswest; // Minuti a ovest di Greenwich
    int tz_dsttime;     // Tipo di correzione ora legale
};

// Dichiariamo la funzione (più avanti potremo farla puntare al tuo RTC)
int gettimeofday(struct timeval *tv, struct timezone *tz);

#endif