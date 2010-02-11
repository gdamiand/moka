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
virtual void viewUpdate(TViewId AViewId);

void viewEnablePreviewPrecompile (TViewId AViewId);
void viewDisablePreviewPrecompile(TViewId AViewId);

void viewEnableObjectTransformationPrecompile (TViewId AViewId);
void viewDisableObjectTransformationPrecompile(TViewId AViewId);

/// Méthodes pour récupérer et modifier les coéficients d'éclatement de la carte.
TCoordinate getMapBurstCoef(int ADim) const;
void        setMapBurstCoef(int ADim, TCoordinate ACoef);
void        setMapBurstCoefs(TCoordinate ACoef0, TCoordinate ACoef1,
			     TCoordinate ACoef2,TCoordinate ACoef3);

/// Méthodes pour gérer le type de calcul des coordonnées éclatées.
void         setBurstMethod(TBurstMethod AMethod);
TBurstMethod getBurstMethod() const;

/// Méthodes pour bloquer le recalcul des coordonnées éclatées.
bool isModelBlocked    () const;
void setModelBlocked   ();
void unsetModelBlocked ();
void toggleModelBlocked();

/// Méthodes pour basculer en mode simplification
bool isModeSimplification    () const;
void setModeSimplification   ();
void unsetModeSimplification ();
void toggleModeSimplification();

//******************************************************************************
