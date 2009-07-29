/*
 * lib-controler : Un contrôleur générique de scène 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler
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
// Ce fichier est une partie de "controler.hh".
//******************************************************************************
public:
//@{
TMode getMode() const;
void  setMode(TMode AMode);
void  toggleMode(TMode AMode);

void operationModeStart(TViewId AViewId, int Ax, int Ay);
void operationModeMove (int Ax, int Ay);
void operationModeStop (int Ax, int Ay);
//@}

/// Méthode pour gerer les opérations qui ne dépendent pas d'un mode.
///  Pour le moment uniquement translation, rotation et homothetie de la scene.
//@{

void operationModeIndependentStart( TModeOperation AOperation,
				    TViewId AViewId,
				    int Ax, int Ay );
//@}

// Positionne le point de vue au coordonnees (x,y) de la souris.

void setClickAimedPosition(TViewId AViewId, int x, int y);

//******************************************************************************
protected:

virtual void onEnterMode();
virtual void onExitMode ();

virtual void onOperationModeStart();
virtual void onOperationModeMove ();
virtual void onOperationModeStop ();

void exitMode();
//@}

//******************************************************************************
protected:
//@{

TMode          FCurrentMode;
TModeOperation FCurrentModeOperation;

int FFirstX, FLastX, FDeltaX;
int FFirstY, FLastY, FDeltaY;
int FCurrentViewId;

/// Coordonnées du point 3D correspondant à FFirst{X,Y}
GLfloat FFirst3D[3];

CParameterEyePosition   FMemoEyePosition;
CParameterAimedPosition FMemoAimedPosition;

//@}

//******************************************************************************
