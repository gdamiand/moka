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
//******************************************************************************
bool CControlerGMap::thicken()
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_THICKEN)))
    {
      undoRedoPreSave();

      float coef = getParameterOperations()->getExtrusionCoef();

      int nb = FMap->thickenMarkedDarts(getSelectionMark(), coef);

      if (nb==0)
	{
	  setMessage("No object to thicken");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();
	  setModelChanged();
	  setMessage(nb, nb==1 ? " dart thicken" : " darts thicken");
	  res = true;
	}
    }
  
  return res;
}
//******************************************************************************
bool CControlerGMap::extrudeByNormal(int ADim)
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_EXTRUDE,
				   SUB_OPERATION_EXTRUDE_BY_NORMAL, ADim)))
    {
      undoRedoPreSave();

      float coef = getParameterOperations()->getExtrusionCoef();

      int nb =
	FMap->extrudeByNormalMarkedCells(getSelectionMark(), ADim,
					 getParameterOperations()
					 ->getExtrusionAdjacentSews(),
					 coef);
      
      if (nb==0)
	{
	  setMessage("No extrusion possible");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();

	  switch (ADim)
	    {
	    case 0:
	      setMessage(nb, nb==1? " vertex extruded": " vertices extruded");
	      break;
	    case 1:
	      setMessage(nb, nb==1? " edge extruded" : " edges extruded");
	      break;
	    case 2:
	      setMessage(nb, nb==1? " face extruded"  : " faces extruded" );
	      break;
	    }

	  setModelChanged();
	  res = true;
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::intuitiveExtrudeByNormal()
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_EXTRUDE,
				   SUB_OPERATION_INTUITIVE_EXTRUDE_BY_NORMAL, -1)))
    {
      undoRedoPreSave();

      float coef = getParameterOperations()->getExtrusionCoef();

      int nb =
	FMap->intuitiveExtrudeByNormalMarkedCells(getSelectionMark(),
						  getParameterOperations()
						  ->getExtrusionAdjacentSews(),
						  coef);
      
      if (nb==0)
	{
	  setMessage("No extrusion possible");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();

	  setModelChanged();
	  setMessage(nb, nb==1 ? " cell extruded"  : " cells extruded" );
	  res = true;
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::extrudeByPath(int ADim)
{
  bool res = false;

  if (canApplyOperation(COperation(OPERATION_EXTRUDE,
				   SUB_OPERATION_EXTRUDE_BY_PATH, ADim)))
    {
      CDart * pathStart = getNextLastSelectedDart(1);
      CDart * pathPonderation = NULL;

      bool ok = true;

      if (getParameterOperations()->getExtrusionPonderateSection())
	{
	  CDart * last2 = getNextLastSelectedDart(2);

	  int n = FMap->getMarkedCells(ORBIT_SELF, getNextSelectionMark(2),
				       last2, &pathPonderation);

	  if (last2==NULL && n>1)
	    {
	      ok = false;
	      setMessage("Extrusion impossible: "
			 "ambiguity on the ponderation path");
	    }

	  if (ok && pathPonderation!=NULL &&
	      FMap->isClosedPolyline(pathPonderation))
	    {
	      ok = false;
	      setMessage("Extrusion impossible: "
			 "the ponderation path is closed");
	    }
	}

      if (ok)
	{
	  if (pathStart==NULL &&
	      FMap->getMarkedCells(ORBIT_SELF, getNextSelectionMark(1),
				   NULL, &pathStart) != 1)
	    setMessage("Extrusion impossible: no path selected");
	  else
	    {
	      undoRedoPreSave();

	      int nb =
		FMap->extrudeByPathMarkedCells(getSelectionMark(), ADim, pathStart,
					       getParameterOperations()->
					       getExtrusionAdjacentSews(),
					       getParameterOperations()->
					       getExtrusionInitialPositionMode(),
					       getParameterOperations()->
					       getExtrusionInitialDirectionMode(),
					       getParameterOperations()->
					       getExtrusionDirection(),
					       getParameterOperations()->
					       getExtrusionScale(),
					       pathPonderation);

	      if (nb==0)
		{
		  setMessage("No extrusion done");
		  undoRedoPostSaveFailed();
		}
	      else
		{
		  undoRedoPostSaveOk();

		  switch (ADim)
		    {
		    case 1:
		      setMessage(nb, nb==1 ? " edge extruded" :
				 " edges extruded");
		      break;
		    case 2:
		      setMessage(nb, nb==1 ? " face extruded" :
				 " faces extruded" );
		      break;
		    }

		  setModelChanged();
		  res = true;
		}
	    }
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::extrudeByRevolution(int ADim)
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_EXTRUDE,
				   SUB_OPERATION_EXTRUDE_BY_NORMAL, ADim)))
    {
      bool ok = true;

      CDart * d1, * d2; // axe
      CDart * pathPonderation = NULL;

      int found = FMap->getMarkedCells(ORBIT_VERTEX, getNextSelectionMark(1),
				       getNextLastSelectedDart(1), &d2,&d1);

      if (found!=2 && (found!=1 || FMap->isFree0(d2)))
	{
	  setMessage("Selection of the axis not correct: we need two darts");
	  ok = false;
	}

      if (ok && d1==NULL)
	{
	  assert(d2!=NULL);
	  assert(!FMap->isFree0(d2));
	  d1 = FMap->alpha0(d2);
	}

      CVertex axeVertex, axeDirection;

      if (ok)
	{
	  axeVertex    = * FMap->findVertex(d1);
	  axeDirection = * FMap->findVertex(d2) - axeVertex;

	  if (axeDirection.isNull())
	    {
	      setMessage("Extrusion impossible: axis of length zero");
	      ok = false;
	    }
	}

      if (ok && getParameterOperations()->getExtrusionPonderateSection())
	{
	  CDart * last2 = getNextLastSelectedDart(2);

	  int n = FMap->getMarkedCells(ORBIT_SELF, getNextSelectionMark(2),
				       last2, &pathPonderation);

	  if (last2==NULL && n>1)
	    {
	      ok = false;
	      setMessage("Extrusion impossible: "
			 "ambiguity on the ponderation path");
	    }

	  if (ok && pathPonderation!=NULL &&
	      FMap->isClosedPolyline(pathPonderation))
	    {
	      ok = false;
	      setMessage("Extrusion impossible: "
			 "the ponderation path is closed");
	    }
	}

      if (ok)
	{
	  undoRedoPreSave();

	  float angle =
	    getParameterOperations()->getExtrusionRevolutionAngle();

	  int nb =
	    FMap->extrudeByRevolutionMarkedCells(getSelectionMark(),
						 ADim, axeVertex, axeDirection,
						 angle,
						 getParameterOperations()->
						 getExtrusionRevolutionNbEdges(),
						 getParameterOperations()->
						 getExtrusionAdjacentSews(),
						 getParameterOperations()->
						 getExtrusionInitialPositionMode(),
						 getParameterOperations()->
						 getExtrusionInitialDirectionMode(),
						 getParameterOperations()->
						 getExtrusionDirection(),
						 getParameterOperations()->
						 getExtrusionScale(),
						 pathPonderation);

	  if (nb==0)
	    {
	      setMessage("No extrusion done");
	      undoRedoPostSaveFailed();
	    }
	  else
	    {
	      undoRedoPostSaveOk();
	
	      switch (ADim)
		{
		case 1:
		  setMessage(nb, nb==1
			     ? " edge extruded" : " edges extruded");
		  break;
		case 2:
		  setMessage(nb, nb==1
			     ? " face extruded"  : " faces extruded" );
		  break;
		}
	
	      setModelChanged();
	      res = true;
	    }
	}
    }

  return res;
}
//******************************************************************************
