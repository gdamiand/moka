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
// Ce fichier est une partie de "controler-gmap.hh".
//******************************************************************************

public:
bool isMapOk() const;

bool isModelChanged   () const;
void setModelChanged  ();
void unsetModelChanged();

bool isSelectionChanged     () const;
void setSelectionChanged    ();
void unsetSelectionChanged  ();


protected:

virtual CPrecompile* newPrecompile(TViewId AViewId,
				   TPrecompile APrecompileType);

bool isPartialModelChanged   () const;
void setPartialModelChanged  ();
void unsetPartialModelChanged();

void updateModelIfNeeded();
void updateSelectionIfNeeded();

bool getDartsForSew(CDart** ADart1, CDart** ADart2);

private:

int FNbDarts;
int FNbSelectedDarts;
int FNbVertices;

//******************************************************************************
