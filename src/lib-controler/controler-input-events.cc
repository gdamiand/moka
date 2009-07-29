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
#include "controler.hh"
#include "view-precompile.hh"
//******************************************************************************
TMode CControler::getMode() const
{ return FCurrentMode; }
//******************************************************************************
void CControler::setMode(TMode AMode)
{
  if (AMode == FCurrentMode)
    return;

  // Sortie du mode courant :
  exitMode();

  // Entrée dans le nouveau mode :
  FCurrentMode = AMode;

  // Fonction à exécuter pour entrer dans le mode.
  onEnterMode();
}
//******************************************************************************
void CControler::toggleMode(TMode AMode)
{
  if (AMode == FCurrentMode) setMode(MODE_DEFAULT);
  else                       setMode(AMode);
}
//******************************************************************************
void CControler::exitMode()
{
  // Sortie de l'opération courante
  operationModeStop(0,0);

  // Fonction à exécuter pour sortir du mode.
  onExitMode();

  // Revenir au mode par defaut
  FCurrentMode = MODE_DEFAULT;
}
//******************************************************************************
void CControler::onEnterMode()
{
  switch (FCurrentMode)
    {
    case MODE_SELECTION: modeSelectionBegin(); break;
    }
}
//------------------------------------------------------------------------------
void CControler::onExitMode()
{
  switch (FCurrentMode)
    {
    case MODE_SELECTION: modeSelectionEnd(); break;
    }
}
//******************************************************************************
void CControler::onOperationModeStart()
{
  switch (FCurrentMode)
    {
    case MODE_SELECTION:
      FCurrentModeOperation = MODE_OPERATION_SELECTION;
      modeSelectionOperationStart();
      break;
    }
}
//------------------------------------------------------------------------------
void CControler::onOperationModeMove()
{
  switch (FCurrentModeOperation)
    {
    case MODE_OPERATION_SELECTION        : modeSelectionOperationMove       ();
      break;
    case MODE_OPERATION_SCENE_TRANSLATION: modeSceneTranslationOperationMove();
      break;
    case MODE_OPERATION_SCENE_ROTATION   : modeSceneRotationOperationMove   ();
      break;
    case MODE_OPERATION_SCENE_SCALE      : modeSceneScaleOperationMove      ();
      break;
    }
}
//------------------------------------------------------------------------------
void CControler::onOperationModeStop()
{
  switch (FCurrentModeOperation)
    {
    case MODE_OPERATION_SELECTION        : modeSelectionOperationStop(); break;
    case MODE_OPERATION_SCENE_TRANSLATION:
    case MODE_OPERATION_SCENE_ROTATION   :
    case MODE_OPERATION_SCENE_SCALE      : modeSceneTransformationOperationStop();
      break;
    }
}
//******************************************************************************
void CControler::operationModeStart(TViewId AViewId, int Ax, int Ay)
{
  if (FCurrentModeOperation!=MODE_OPERATION_NONE)
    operationModeStop(FLastX, FLastY);

  assert(FViews[AViewId] != NULL);

  FCurrentViewId = AViewId;
  FFirstX = FLastX = Ax; FDeltaX = 0;
  FFirstY = FLastY = Ay; FDeltaY = 0;

  // Fonction à exécuter quand on commence une opération d'un mode.
  onOperationModeStart();
}
//------------------------------------------------------------------------------
void CControler::operationModeMove(int Ax, int Ay)
{
  if (FCurrentModeOperation==MODE_OPERATION_NONE) return;

  FLastX = Ax; FDeltaX = FLastX - FFirstX;
  FLastY = Ay; FDeltaY = FLastY - FFirstY;

  // Fonction à exécuter au milieu d'une opération d'un mode.
  onOperationModeMove();
}
//------------------------------------------------------------------------------
void CControler::operationModeStop(int Ax, int Ay)
{
  if (FCurrentModeOperation==MODE_OPERATION_NONE) return;

  FLastX = Ax; FDeltaX = FLastX - FFirstX;
  FLastY = Ay; FDeltaY = FLastY - FFirstY;

  // Fonction à exécuter quand on termine une opération d'un mode.
  onOperationModeStop();


  // On arête l'opération.
  FCurrentModeOperation = MODE_OPERATION_NONE;
}
//******************************************************************************
void CControler::operationModeIndependentStart( TModeOperation AOperation,
						TViewId AViewId,
						int Ax, int Ay )
{
  if ( FCurrentModeOperation!=MODE_OPERATION_NONE ||
       ( AOperation!=MODE_OPERATION_SCENE_TRANSLATION &&
	 AOperation!=MODE_OPERATION_SCENE_ROTATION &&
	 AOperation!=MODE_OPERATION_SCENE_SCALE ))
    return;

  FCurrentModeOperation = AOperation;

  FCurrentViewId = AViewId;

  FFirstX = FLastX = Ax; FDeltaX = 0;
  FFirstY = FLastY = Ay; FDeltaY = 0;

  CViewPrecompile* view = FViews[FCurrentViewId];
  assert(view != NULL);

  FMemoEyePosition = view->getEyePosition();
  FMemoAimedPosition = view->getAimedPosition();

  view->unproject(FFirstX, FFirstY, FFirst3D);
}
//******************************************************************************
