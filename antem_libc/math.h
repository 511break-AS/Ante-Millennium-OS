// Ante-Millennium Operating System - antem_math
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




// Stub della libreria matematica per il compilatore ospite

#ifndef _ANTEM_MATH_H
#define _ANTEM_MATH_H


double ldexp(double x, int exp);
// Per ora lasciamo il file vuoto. 
// Serve esclusivamente per non far arrabbiare il preprocessore di GCC
// quando TCC fa "#include <math.h>".

#endif