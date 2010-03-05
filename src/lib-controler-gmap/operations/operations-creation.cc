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
#include "g-map-vertex.hh"
#include "controler-gmap.hh"
#include <cassert>

using namespace GMap3d;
using namespace std;
//******************************************************************************
bool CControlerGMap::createObject()
{
  switch (getMode())
    {
    case MODE_CREATION_POLYLINE: return createPolyline (); break;
    case MODE_CREATION_POLYGON : return createPolygon  (); break;
    case MODE_CREATION_MESH    : return createMesh     (); break;
    case MODE_CREATION_SPHERE  : return createSphere   (); break;
    case MODE_CREATION_CYLINDER: return createCylinder (); break;
    case MODE_CREATION_PYRAMID : return createPyramid  (); break;
    case MODE_CREATION_TORUS   : return createTorus    (); break;
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::createPolyline()
{
  if (!canApplyOperation(COperation(OPERATION_CREATE,
				    SUB_OPERATION_CREATE_POLYLINE, -1)) ||
      FParameterPolylinePosition->getNbVertices()==0 )
    return false;

  undoRedoPreSave();
  FMap->createPolyline(FParameterPolylinePosition->getVertices());
  undoRedoPostSaveOk();
  
  setModelChanged();
  setMessage("Polyline created");
  return true;
}
//******************************************************************************
bool CControlerGMap::createPolygon()
{
  if (!canApplyOperation(COperation(OPERATION_CREATE,
				    SUB_OPERATION_CREATE_POLYGON, -1)))
    return false;

  CTransformationMatrix matrix =
    FParameterObjectPosition[OBJECT_POLYGON]->getTransformationMatrix();

  int n = getParameterCreation()->getPolygonNbEdges();

  undoRedoPreSave();
  FMap->applyMatrix(matrix, FMap->createRegularPolygon(n), ORBIT_CC);
  undoRedoPostSaveOk();

  setModelChanged();
  setMessage("Polygone created");
  return true;
}
//******************************************************************************
bool CControlerGMap::createMesh()
{
  if (!canApplyOperation(COperation(OPERATION_CREATE,
				    SUB_OPERATION_CREATE_MESH, -1)))
    return false;

  CTransformationMatrix matrix =
    FParameterObjectPosition[OBJECT_MESH]->getTransformationMatrix();

  int SX = getParameterCreation()->getMeshNbSubdivisionsX();
  int SY = getParameterCreation()->getMeshNbSubdivisionsY();
  int SZ = getParameterCreation()->getMeshNbSubdivisionsZ();

  int SD = getParameterCreation()->getMeshSubdivisionDimension();

  CDart * handle1, * handle2;

  undoRedoPreSave();

  switch (getParameterCreation()->getMeshDimension())
    {
    case 1:
      handle1 = FMap->createMesh1(SD==0 ? 1 : SX);
      FMap->applyMatrix(matrix, handle1, ORBIT_CC);

      switch (SD)
	{
	case 0: setMessage("Edge created"); break;
	case 1: setMessage("Edge meshed created"); break;
	default: return false;
	}

      break;

    case 2:
      handle1 = FMap->createSquareIMeshed(SX, SY, SD);
      FMap->applyMatrix(matrix, handle1, ORBIT_CC);

      switch (SD)
	{
	case 0: setMessage("Square created"); break;
	case 1: setMessage("Square 1-meshed created"); break;
	case 2: setMessage("Square 2-meshed created"); break;
	default: return false;
	}

      break;

    case 3:
      if (SD==3)
	{
	  handle1 = FMap->createMesh3(SX, SY, SZ);
	  handle2 = NULL;
	  setMessage("Cube 3-meshed created");
	}
      else
	{
	  unsigned char F = getParameterCreation()->getMeshCreatedFaces();

	  bool created[3][2] =
	  {
	    { F & CUBE_X1, F & CUBE_X2 },
	    { F & CUBE_Y1, F & CUBE_Y2 },
	    { F & CUBE_Z1, F & CUBE_Z2 }
	  };

	  FMap->createCubeIMeshed(SX,SY,SZ, SD, created, & handle1, & handle2);

	  switch (SD)
	    {
	    case 0: setMessage("Cube created"); break;
	    case 1: setMessage("Cube 1-meshed created"); break;
	    case 2: setMessage("Cube 2-meshed created"); break;
	    default: return false;
	    }
	}

      FMap->applyMatrix(matrix, handle1, ORBIT_CC);

      if (handle2 != NULL)
	FMap->applyMatrix(matrix, handle2, ORBIT_CC);

      break;

    default: return false;
    }

  undoRedoPostSaveOk();
  setModelChanged();
  return true;
}
//******************************************************************************
bool CControlerGMap::createCylinder()
{
  if (!canApplyOperation(COperation(OPERATION_CREATE,
				    SUB_OPERATION_CREATE_CYLINDER, -1)))
    return false;

  CTransformationMatrix matrix =
    FParameterObjectPosition[OBJECT_CYLINDER]->getTransformationMatrix();

  int M = getParameterCreation()->getCylinderNbMeridians();
  int P = getParameterCreation()->getCylinderNbParallels();
  bool C1 = getParameterCreation()->getCylinderClosedUp();
  bool C2 = getParameterCreation()->getCylinderClosedDown();

  undoRedoPreSave();
  FMap->applyMatrix(matrix, FMap->createCylinder(M, P, C1, C2), ORBIT_CC);
  undoRedoPostSaveOk();

  setModelChanged();
  setMessage("Cylindre created");
  return true;
}
//******************************************************************************
bool CControlerGMap::createPyramid()
{
  if (!canApplyOperation(COperation(OPERATION_CREATE,
				    SUB_OPERATION_CREATE_PYRAMID, -1)))
    return false;

  CTransformationMatrix matrix =
    FParameterObjectPosition[OBJECT_PYRAMID]->getTransformationMatrix();

  int M = getParameterCreation()->getPyramidNbMeridians();
  int P = getParameterCreation()->getPyramidNbParallels();
  bool C = getParameterCreation()->getPyramidClosed();

  undoRedoPreSave();
  FMap->applyMatrix(matrix, FMap->createPyramid(M, P, C), ORBIT_CC);
  undoRedoPostSaveOk();

  setModelChanged();
  setMessage("Pyramide created");
  return true;
}
//******************************************************************************
bool CControlerGMap::createSphere()
{
  if (!canApplyOperation(COperation(OPERATION_CREATE,
				    SUB_OPERATION_CREATE_SPHERE, -1)))
    return false;

  CTransformationMatrix matrix =
    FParameterObjectPosition[OBJECT_SPHERE]->getTransformationMatrix();

  int M = getParameterCreation()->getSphereNbMeridians();
  int P = getParameterCreation()->getSphereNbParallels();

  undoRedoPreSave();
  FMap->applyMatrix(matrix, FMap->createSphere(M, P), ORBIT_CC);
  undoRedoPostSaveOk();

  setModelChanged();
  setMessage("Sphere created");
  return true;
}
//******************************************************************************
bool CControlerGMap::createTorus()
{
  if (!canApplyOperation(COperation(OPERATION_CREATE,
				    SUB_OPERATION_CREATE_TORUS, -1)))
    return false;

  CTransformationMatrix matrix =
    FParameterObjectPosition[OBJECT_TORUS]->getTransformationMatrix();

  int M   = getParameterCreation()->getTorusNbMeridians();
  int P   = getParameterCreation()->getTorusNbParallels();
  float R = getParameterCreation()->getTorusRadiusRapport();

  undoRedoPreSave();
  FMap->applyMatrix(matrix, FMap->createTorus(M, P, R), ORBIT_CC);
  undoRedoPostSaveOk();

  setModelChanged();
  setMessage("Torus created");
  return true;
}
//******************************************************************************
float CControlerGMap::getPolygonEdgesLength() const
{
  return FParameterObjectPosition[OBJECT_POLYGON]->getScale()
    *dSin(180.0/FParameterCreation->getPolygonNbEdges());
}
//------------------------------------------------------------------------------
void CControlerGMap::setPolygonEdgesLength(float AValue)
{
  // getEdgesNumber()>3 => dSin != 0
  CParameterObjectPosition* param = FParameterObjectPosition[OBJECT_POLYGON];
  
  TCoordinate s = AValue/dSin(180.0/FParameterCreation->getPolygonNbEdges()); 
  param->setScale(s);
  param->setDimensions(2*s*param->getProportions());
}
//------------------------------------------------------------------------------
void CControlerGMap::setPolygonEdgesNumber(int AValue)
{
  FParameterCreation->setPolygonNbEdges(AValue);
  setPolygonEdgesLength(FParameterObjectPosition[OBJECT_POLYGON]->getScale()*
			dSin(180.0/AValue));
}
//******************************************************************************
bool CControlerGMap::randomizeDarts()
{  
  FMap->randomizeDarts();
  setMessage("Order of darts was randomized");
  return true;
}
//******************************************************************************
