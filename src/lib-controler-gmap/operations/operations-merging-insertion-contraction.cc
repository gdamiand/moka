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
bool CControlerGMap::merge(int ADimension)
{
   assert(1 <= ADimension && ADimension <= 3);

   bool res = false;

   if (canApplyOperation(COperation(OPERATION_MERGE,
                                    SUB_OPERATION_TOPO, ADimension)))
   {
      undoRedoPreSave();

      int nb = FMap->mergeMarkedCells(getSelectionMark(), ADimension, true);

      if (nb == 0)
      {
         setMessage(ADimension, "-fusion impossible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, ADimension, (nb == 1 ? "-fusion effectuée" :
                                     "-fusions effectuées"));
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
         setMessage("Fusion intuitive impossible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ? " fusion effectuée" : 
               " fusions effectuées"));
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
         setMessage("Fusion d'arêtes alignées impossible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ?
                         " fusion d'arêtes alignées effectuée" :
                         " fusions d'arêtes alignées effectuées"));
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
         setMessage("Fusion de faces coplanaires impossible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ?
                         " fusion de faces coplanaires effectuée" :
                         " fusions de faces coplanaires effectuées"));
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
         setMessage("Pas d'arête de longueur nulle");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ?
                    " arête de longueur nulle supprimée" :
                    " arêtes de longueur nulle supprimées");
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
         setMessage("Pas de face composée uniquement de deux arêtes");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ?
                    " face composée uniquement de deux arêtes supprimée" :
                    " face composée uniquement de deux arêtes supprimées");
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
         setMessage("Pas de volume composé uniquement de deux faces");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ?
                    " volume composé uniquement de deux faces supprimée" :
                    " volume composé uniquement de deux faces supprimées");
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
         setMessage("Aucune 1-suppression possible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ? " 1-suppression effectuée" :
                         " 1-suppressions effectuées"));
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
         setMessage("Aucune 2-suppression possible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ? " 2-suppression effectuée" :
                         " 2-suppressions effectuées"));
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
      setMessage("Dernier brin sélectionné introuvable");
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
      setMessage(nb, (nb == 1 ? " arête décallée." :
                      " arêtes décallées."));
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
         setMessage("Aucune arête pendante supprimée");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, (nb == 1 ? " arête pendante supprimée" :
                         " arêtes pendantes supprimées"));
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

   if (canApplyOperation(COperation(OPERATION_MERGE,
                                    SUB_OPERATION_TOPO, ADimension)))
   {
      undoRedoPreSave();

      int nb = FMap->contractMarkedCells(getSelectionMark(), ADimension, true);

      if (nb == 0)
      {
         setMessage(ADimension, "-contraction impossible");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         unsetLastSelectedDart();
         setModelChanged();
         setMessage(nb, ADimension, (nb == 1 ? "-contraction effectuée" :
                                     "-contractions effectuées"));
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
         default: setMessage("Orbite de sélection incohérente"); return false;
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
         setMessage(nb, dim, (nb == 1 ? "-contraction effectuée" :
                              "-contractions effectuées"));
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
         setMessage("Aucun sommet inséré");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ? " sommet inséré" : " sommets insérés");
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
         setMessage("Aucune arête insérée");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ? " arête insérée" : " arêtes insérées");
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
         setMessage("Aucune face insérée");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ? " face insérée" : " faces insérées");
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
         setMessage("Aucun bord fermé");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, nb == 1 ? " bord fermé" : " bords fermés");
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
         setMessage("Aucun ", ADimension, "-bord bouché");
         undoRedoPostSaveFailed();
      }
      else
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(nb, ADimension, nb == 1 ? "-bord bouché" : 
               "-bords bouchés");
         res = true;
      }
   }

   return res;
}
//******************************************************************************

