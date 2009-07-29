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
#include "controler-gmap.hh"
#include "view-precompile.hh"
using namespace GMap3d;
//******************************************************************************
void CControlerGMap::onEnterMode()
{
  switch (FCurrentMode)
    {
    case MODE_CREATION_POLYGON : FCreationType = OBJECT_POLYGON ; break;
    case MODE_CREATION_MESH    : FCreationType = OBJECT_MESH    ; break;
    case MODE_CREATION_SPHERE  : FCreationType = OBJECT_SPHERE  ; break;
    case MODE_CREATION_CYLINDER: FCreationType = OBJECT_CYLINDER; break;
    case MODE_CREATION_PYRAMID : FCreationType = OBJECT_PYRAMID ; break;
    case MODE_CREATION_TORUS   : FCreationType = OBJECT_TORUS   ; break;
    }
  
  switch (FCurrentMode)
    {
    case MODE_CREATION_POLYLINE:
    case MODE_CREATION_POLYGON :
    case MODE_CREATION_MESH    :
    case MODE_CREATION_SPHERE  :
    case MODE_CREATION_CYLINDER:
    case MODE_CREATION_PYRAMID :
    case MODE_CREATION_TORUS   : modeCreationBegin   (); break;
    case MODE_TRANSLATION      : modeTranslationBegin(); break;
    case MODE_ROTATION         : modeRotationBegin   (); break;
    case MODE_SCALE            : modeScaleBegin      (); break;
#ifdef MODULE_ROUNDING
    case MODE_ROUNDING         : modeRoundingBegin   (); break;
#endif // MODULE_ROUNDING
      
    default: CControler::onEnterMode(); break;
    }
}
//******************************************************************************
void CControlerGMap::onExitMode()
{
  switch (FCurrentMode)
    {
    case MODE_CREATION_POLYLINE:
    case MODE_CREATION_POLYGON :
    case MODE_CREATION_MESH    :
    case MODE_CREATION_SPHERE  :
    case MODE_CREATION_CYLINDER:
    case MODE_CREATION_PYRAMID :
    case MODE_CREATION_TORUS   : modeCreationEnd(); break;
    case MODE_TRANSLATION      : modeTranslationEnd(); break;
    case MODE_ROTATION         : modeRotationEnd(); break;
    case MODE_SCALE            : modeScaleEnd(); break;
#ifdef MODULE_ROUNDING
    case MODE_ROUNDING         : modeRoundingEnd(); break;
#endif // MODULE_ROUNDING

    default: CControler::onExitMode(); break;
    }
}
//******************************************************************************
bool CControlerGMap::isInCreationMode() const
{
  return
    FCurrentMode>=MODE_CREATION_POLYLINE &&
    FCurrentMode<=MODE_CREATION_TORUS;
}
//******************************************************************************
void CControlerGMap::setNextCreationMode()
{
  if (!isInCreationMode())
    return;

  TMode currentMode = FCurrentMode+1;

  if (currentMode>MODE_CREATION_TORUS)
    currentMode=MODE_CREATION_POLYLINE;

  setMode(currentMode);
}
//******************************************************************************
void CControlerGMap::onOperationModeStart()
{
  switch (FCurrentMode)
    {
    case MODE_CREATION_POLYLINE:
      FCurrentModeOperation = MODE_OPERATION_CREATION_POLYLINE; break;
    case MODE_CREATION_POLYGON :
      FCurrentModeOperation = MODE_OPERATION_CREATION_POLYGON;  break;
    case MODE_CREATION_MESH    :
      FCurrentModeOperation = MODE_OPERATION_CREATION_MESH; 	break;
    case MODE_CREATION_SPHERE  :
      FCurrentModeOperation = MODE_OPERATION_CREATION_SPHERE;   break;
    case MODE_CREATION_CYLINDER:
      FCurrentModeOperation = MODE_OPERATION_CREATION_CYLINDER; break;
    case MODE_CREATION_PYRAMID :
      FCurrentModeOperation = MODE_OPERATION_CREATION_PYRAMID;  break;
    case MODE_CREATION_TORUS   :
      FCurrentModeOperation = MODE_OPERATION_CREATION_TORUS;    break;
    case MODE_TRANSLATION      :
      FCurrentModeOperation = MODE_OPERATION_TRANSLATION; 	break;
    case MODE_ROTATION         :
      FCurrentModeOperation = MODE_OPERATION_ROTATION; 	        break;
    case MODE_SCALE            :
      FCurrentModeOperation = MODE_OPERATION_SCALE; 		break;
    case MODE_ROUNDING         :
      FCurrentModeOperation = MODE_OPERATION_ROUNDING; 		break;
    }

  switch (FCurrentMode)
    {
    case MODE_CREATION_POLYLINE: modeCreationPolylineOperationStart(); break;
    case MODE_CREATION_POLYGON :
    case MODE_CREATION_MESH    :
    case MODE_CREATION_SPHERE  :
    case MODE_CREATION_CYLINDER:
    case MODE_CREATION_PYRAMID :
    case MODE_CREATION_TORUS   : modeCreationOperationStart(); 	       break;
    case MODE_TRANSLATION      : modeTranslationOperationStart();      break;
    case MODE_ROTATION         : modeRotationOperationStart(); 	       break;
    case MODE_SCALE            : modeScaleOperationStart(); 	       break;
#ifdef MODULE_ROUNDING
    case MODE_ROUNDING         : modeRoundingOperationStart();         break;
#endif // MODULE_ROUNDING

    default: CControler::onOperationModeStart();
    }
}
//------------------------------------------------------------------------------
void CControlerGMap::onOperationModeMove()
{
  switch (FCurrentModeOperation)
    {
    case MODE_OPERATION_CREATION_POLYLINE:
      modeCreationPolylineOperationMove(); break;
    case MODE_OPERATION_CREATION_POLYGON :
    case MODE_OPERATION_CREATION_MESH    :
    case MODE_OPERATION_CREATION_SPHERE  :
    case MODE_OPERATION_CREATION_CYLINDER:
    case MODE_OPERATION_CREATION_PYRAMID :
    case MODE_OPERATION_CREATION_TORUS   :
      modeCreationOperationMove        (); break;
    case MODE_OPERATION_TRANSLATION      :
      modeTranslationOperationMove     (); break;
    case MODE_OPERATION_ROTATION         :
      modeRotationOperationMove        (); break;
    case MODE_OPERATION_SCALE            :
      modeScaleOperationMove           (); break;
#ifdef MODULE_ROUNDING
    case MODE_OPERATION_ROUNDING         :
      modeRoundingOperationMove        (); break;
#endif // MODULE_ROUNDING

    default: CControler::onOperationModeMove();
    }
}
//------------------------------------------------------------------------------
void CControlerGMap::onOperationModeStop()
{
  switch (FCurrentModeOperation)
    {
    case MODE_OPERATION_CREATION_POLYLINE:
      modeCreationPolylineOperationStop   (); break;
    case MODE_OPERATION_CREATION_POLYGON :
    case MODE_OPERATION_CREATION_MESH    :
    case MODE_OPERATION_CREATION_SPHERE  :
    case MODE_OPERATION_CREATION_CYLINDER:
    case MODE_OPERATION_CREATION_PYRAMID :
    case MODE_OPERATION_CREATION_TORUS   :
      modeCreationOperationStop           (); break;
    case MODE_OPERATION_TRANSLATION      :
    case MODE_OPERATION_ROTATION         :
    case MODE_OPERATION_SCALE            :
      modeTransformationOperationStop     (); break;
#ifdef MODULE_ROUNDING
    case MODE_OPERATION_ROUNDING         :
      modeRoundingOperationStop           (); break;
#endif // MODULE_ROUNDING

    default:  CControler::onOperationModeStop();
    }
}
//******************************************************************************
