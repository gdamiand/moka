/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//******************************************************************************
// Ce fichier fait partie de "controler-gmap-operations.hh".
//******************************************************************************

public:

bool goForward1        ();
bool goForward0        ();
bool goBackward1       ();
bool goBackward0       ();
bool goLeft1           ();
bool goLeft0           ();
bool goRight1          ();
bool goRight0          ();

bool goForward1Rep     ();
bool goForward0Rep     ();
bool goBackward1Rep    ();
bool goBackward0Rep    ();
bool goLeft1Rep        ();
bool goLeft0Rep        ();
bool goRight1Rep       ();
bool goRight0Rep       ();

bool goAlphai          ( unsigned int ADim );
bool goAlpha0          ();
bool goAlpha1          ();
bool goAlpha2          ();
bool goAlpha3          ();

bool canExtendSelection();

//******************************************************************************
