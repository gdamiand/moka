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
void CControlerGMap::updateDartAfterRemovals(unsigned int ADim)
{
  if ( !getParameterGMapVertex()->getModeSimplification() )
    return;
  
  int deleted = getParameterGMapVertex()->getDrawingMap()->getNewMark();
  getParameterGMapVertex()->getDrawingMap()->negateMaskMark(deleted);

  //1) we unmark all surviving darts
  for (CDynamicCoverageAll it(FMap); it.cont(); ++it)
    {
      getParameterGMapVertex()->getDrawingMap()->
	unsetMark(getParameterGMapVertex()->getDartWithEmbedding(*it),deleted);
    }
  
  // Now we run through all darts of the drawing map. Each dart marked with
  // deleted and not yet marked with a removed mark was just being removed.
  for (CDynamicCoverageAll it(getParameterGMapVertex()->getDrawingMap());
       it.cont(); ++it)
    {
      if ( getParameterGMapVertex()->getDrawingMap()->isMarked(*it,deleted) &&
	   ! getParameterGMapVertex()->isMarkedDeleted(*it) )
	getParameterGMapVertex()->getDrawingMap()->
	  setMark(*it,getParameterGMapVertex()->getMarkRemoved(ADim));
      
      getParameterGMapVertex()->getDrawingMap()->unsetMark(*it, deleted);
    }
  
  getParameterGMapVertex()->getDrawingMap()->freeMark(deleted);
}
//******************************************************************************
bool CControlerGMap::merge(int ADim)
{
   assert(1 <= ADim && ADim <= 3);

   bool res = false;

   if (canApplyOperation(COperation(OPERATION_MERGE,
                                    SUB_OPERATION_TOPO, ADim)))
   {
      undoRedoPreSave();

      int nb = FMap->mergeMarkedCells(getSelectionMark(), ADim, true);

      if (nb == 0)
      {
         setMessage(ADim, "-merge impossible");
         undoRedoPostSaveFailed();
      }
      else
      {
	updateDartAfterRemovals(ADim-1);
	
	undoRedoPostSaveOk();
	unsetLastSelectedDart();
	setModelChanged();
	setMessage(nb, ADim, (nb == 1 ? "-merge done" :
                                     "-merges done"));
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::intuitiveMerge()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_MERGE,
                                    SUB_OPERATION_INTUITIVE_TOPO, -1)))
   {
      undoRedoPreSave();

      int nb = FMap->intuitiveMergeMarkedCells(getSelectionMark(), true);

      if (nb == 0)
      {
         setMessage("Intuitive merge impossible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ? " merge done" : 
               " merges done"));
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::mergeColinearEdges()
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_MERGE_COLINEAR, 1)))
   {
      undoRedoPreSave();

      int nb = FMap->mergeMarkedColinearEdges(getSelectionMark(), true);

      if (nb == 0)
      {
         setMessage("No colinear edges exist");
         undoRedoPostSaveFailed();
      }
      else
      {
	updateDartAfterRemovals(0);

	undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ? " edge merged" :
                         " edges merged"));
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::mergeCoplanarFaces()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_MERGE_COLINEAR, 2)))
   {
      undoRedoPreSave();

      int nb = FMap->mergeMarkedCoplanarFaces(getSelectionMark(), true);

      if (nb == 0)
      {
         setMessage("Coplanar face merging impossible");
         undoRedoPostSaveFailed();
      }
      else
      {
	updateDartAfterRemovals(1);

	undoRedoPostSaveOk();
	unsetLastSelectedDart();
	setModelChanged();
	setMessage(nb, (nb == 1 ?
			" edge removed between coplanar faces" :
			" edges removed between coplanar faces"));
	res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::delNullEdges()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_DEL_NULL_EDGE)))
   {
      undoRedoPreSave();

      int nb = FMap->deleteNullLengthEdges();

      if (nb == 0)
      {
         setMessage("No null length edge");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ?
                    " null length edge removed" :
                    " null length edges removed");
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::delFlatFaces()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_DEL_FLAT_FACES)))
   {
      undoRedoPreSave();

      int nb = FMap->deleteFlatFaces();

      if (nb == 0)
      {
         setMessage("No face made of only two edges");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ?
                    " face made of two edges removed" :
                    " faces made of two edges removed");
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::delFlatVolumes()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_DEL_FLAT_VOLUMES)))
   {
      undoRedoPreSave();

      int nb = FMap->deleteFlatVolumes();

      if (nb == 0)
      {
         setMessage("No volume made of only two faces");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ?
                    " volume made of two faces removed" :
                    " volumes made of two faces removed");
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::removeMarkedEdgesWithoutDisconnection()
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_MERGE_FACE_NODISCONNECTION,
                                    1)))
   {
      undoRedoPreSave();

      int nb = FMap->removeMarkedEdgesWithoutDisconnection(getSelectionMark());

      if (nb == 0)
      {
         setMessage("No 1-removal possible");
         undoRedoPostSaveFailed();
      }
      else
      {
	updateDartAfterRemovals(1);

	undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ? " 1-removal done" : " 1-removals done"));
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::removeMarkedFacesButKeepBalls()
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_REMOVE_FACES_KEEP_BALLS,
                                    1)))
   {
      undoRedoPreSave();

      int nb = FMap->removeMarkedFacesButKeepBalls(getSelectionMark());

      if (nb == 0)
      {
         setMessage("No 2-removal possible");
         undoRedoPostSaveFailed();
      }
      else
      {
	updateDartAfterRemovals(2);

        undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ? " 2-removal done" : " 2-removals done"));
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::shiftAllEdgesIncidentToVertex()
{
   bool res = false;

   if (getLastSelectedDart() == NULL)
   {
      setMessage("No last selected dart");
      return false;
   }

   if (canApplyOperation(COperation(OPERATION_SHIFT_EDGES_INCIDENT_TO_VERTEX)))
   {
      undoRedoPreSave();

      unsigned int nb =
         FMap->shiftAllEdgesIncidentToVertex(getLastSelectedDart());

      deselectDart(getLastSelectedDart());

      undoRedoPostSaveOk();

      setModelChanged();
      setMessage(nb, (nb == 1 ? " edge shifted." : " edges shifted."));
      res = true;
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::removeDanglingEdges()
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_REMOVE_DANGLING_EDGES)))
   {
      undoRedoPreSave();

      int nb = FMap->removeDanglingEdges();

      if (nb == 0)
      {
         setMessage("No dangling edge removed");
         undoRedoPostSaveFailed();
      }
      else
      {
	updateDartAfterRemovals(1);

	undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ? " dangling edge removed" :
                         " dangling edges removed"));
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::contract(int ADimension)
{
   assert(1 <= ADimension && ADimension <= 3);

   bool res = false;

   if (canApplyOperation(COperation(OPERATION_CONTRACT,
                                    SUB_OPERATION_TOPO, ADimension)))
   {
      undoRedoPreSave();

      int nb = FMap->contractMarkedCells(getSelectionMark(), ADimension, true);

      if (nb == 0)
      {
         setMessage(ADimension, "-contraction not possible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, ADimension, (nb == 1 ? "-contraction done" :
                                     "-contractions done"));
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::contextContract()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_CONTRACT,
                                    SUB_OPERATION_INTUITIVE_TOPO, -1)))
   {
      undoRedoPreSave();

      int dim = 0;
      switch (getSelectionOrbit())
      {
         case ORBIT_EDGE   : dim = 1; break;
         case ORBIT_FACE   : dim = 2; break;
         case ORBIT_VOLUME : dim = 3; break;
         default: setMessage("Selected orbit not correct"); return false;
      }

      int nb = FMap->contractMarkedCells(getSelectionMark(), dim, true);

      if (nb == 0)
      {
         setMessage(dim, "-contraction impossible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, dim, (nb == 1 ? "-contraction done" :
                              "-contractions done"));
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::insertVertex()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_INSERT, 0)))
   {
      undoRedoPreSave();

      int nb = FMap->insertVertexOnMarkedEdges(getSelectionMark());

      if (nb == 0)
      {
         setMessage("No vertex inserted");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ? " vertex inserted" : " vertices inserted");
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::insertEdge()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_INSERT, 1)))
   {
      undoRedoPreSave();

      CDart* dart1;
      CDart* dart2;

      int nb = 0;

      if (FMap->getMarkedCells(ORBIT_SELF, getSelectionMark(),
                               NULL, &dart1, &dart2) == 2)
      {
         if (!FMap->isSameOrbit(dart1, dart2, ORBIT_VERTEX))
         {
            nb = 1;
            FMap->insertEdge(dart1, dart2);
         }
      }
      else
         nb = FMap->insertEdgeOnMarkedFaces(getSelectionMark(), true, true);

      if (nb == 0)
      {
         setMessage("No edge inserted");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ? " edge inserted" : " edges inserted");
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::insertFace()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_INSERT, 2)))
   {
      undoRedoPreSave();

      int nb =
         FMap->insertFaceOnMarkedVolumes(getSelectionMark(), true, true, true);

      if (nb == 0)
      {
         setMessage("No face inserted");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ? " face inserted" : " faces inserted");
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::intuitiveStopUp()
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_STOP_UP,
                                    SUB_OPERATION_INTUITIVE_TOPO, -1)))
   {
      undoRedoPreSave();

      int nb = FMap->intuitiveStopUpMarkedBorders(getSelectionMark());

      if (nb == 0)
      {
         setMessage("No border closed");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ? " border closed" : " borders closed");
         res = true;
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::stopUp(int ADimension)
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_STOP_UP, ADimension)))
   {
      undoRedoPreSave();

      int nb = FMap->stopUpMarkedBorders(getSelectionMark(), ADimension);

      if (nb == 0)
      {
         setMessage("No ", ADimension, "-border closed");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, ADimension, nb == 1 ? "-border closed" : 
               "-borders closed");
         res = true;
      }
   }

   return res;
}
//******************************************************************************

