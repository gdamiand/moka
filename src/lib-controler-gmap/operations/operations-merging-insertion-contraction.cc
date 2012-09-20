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

#ifndef _WINDOWS
#include "chrono.hh"
#endif

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

  if ( ADim==2 ) // TODO better ;)
  {
    getParameterGMapVertex()->getDrawingMap()->
        mergeMarkedCells(deleted, 3, true);
    return;
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
void CControlerGMap::updateDartAfterRemovals(int AMark0, int AMark1, int AMark2)
{
  if ( !getParameterGMapVertex()->getModeSimplification() )
    return;

  CDart* current = NULL;

  if ( AMark2!=-1 )
  {
    int deleted = getParameterGMapVertex()->getDrawingMap()->getNewMark();

    //1) we mark all 2-removed darts
    for (CDynamicCoverageAll it(FMap); it.cont(); )
    {
      current = it++;
      if ( FMap->isMarked(current,AMark2) )
      {
        getParameterGMapVertex()->getDrawingMap()->
            setMark(getParameterGMapVertex()->
                    getDartWithEmbedding(current),deleted);
        FMap->delMapDart(current);
      }
    }
    getParameterGMapVertex()->getDrawingMap()->
        mergeMarkedCells(deleted, 3, true);

    getParameterGMapVertex()->getDrawingMap()->freeMark(deleted);
  }

  for (CDynamicCoverageAll it(FMap); it.cont(); )
  {
    current = it++;

    if ( AMark0!=-1 && FMap->isMarked(current,AMark0) )
    {
      getParameterGMapVertex()->getDrawingMap()->
          setMark(getParameterGMapVertex()->getDartWithEmbedding(current),
                  getParameterGMapVertex()->getMarkRemoved(0));
      FMap->delMapDart(current);
    }
    else if ( AMark1!=-1 && FMap->isMarked(current,AMark1) )
    {
      getParameterGMapVertex()->getDrawingMap()->
          setMark(getParameterGMapVertex()->getDartWithEmbedding(current),
                  getParameterGMapVertex()->getMarkRemoved(1));
      FMap->delMapDart(current);
    }
    //       else if ( AMark2!=-1 && FMap->isMarked(current,AMark2) )
    // 	{
    // 	  getParameterGMapVertex()->getDrawingMap()->
    // 	    setMark(getParameterGMapVertex()->getDartWithEmbedding(current),
    // 		    getParameterGMapVertex()->getMarkRemoved(2));
    // 	  FMap->delMapDart(current);
    // 	}
  }
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
bool CControlerGMap::simplify2DObject()
{
  bool res = false;

  if (canApplyOperation(COperation(OPERATION_SIMPLIFY_2D_OBJECT)))
  {
    undoRedoPreSave();

    int nbdarts, nbvertices, nbedges, nbfaces, nbvolumes, nbcc;
    getMapGlobalCharacteristics(&nbdarts,&nbvertices,&nbedges,
                                &nbfaces,&nbvolumes,&nbcc,
                                NULL,NULL,NULL,NULL);
    std::cout<<"Map before simplification: darts="<<nbdarts
            <<", vertices="<<nbvertices
           <<", edges="<<nbedges
          <<", faces="<<nbfaces
         <<", volumes="<<nbvolumes
        <<", cc="<<nbcc<<std::endl;

    int m0=-1;
    int m1=-1;

    if ( getParameterGMapVertex()->getModeSimplification() )
    {
      m0=FMap->getNewMark();
      m1=FMap->getNewMark();
    }

#ifndef _WINDOWS
    CChrono c;
    c.start();
#endif

    int nb = FMap->simplify2DObject(m0,m1);

#ifndef _WINDOWS
    c.stop();
    c.display("Time for simplification");
#endif
    assert(isMapOk());

    if ( nb==0 )
    {
      setMessage("Nothing was simplified, the map was already in "
                 "its minimal form.");
      undoRedoPostSaveFailed();
    }
    else
    {
      if ( getParameterGMapVertex()->getModeSimplification() )
      {
        updateDartAfterRemovals(m0,m1,-1);
        FMap->freeMark(m0);
        FMap->freeMark(m1);
      }

      undoRedoPostSaveOk();
      unsetLastSelectedDart();
      setModelChanged();
      setMessage(nb, (nb == 1 ? " dart removed during the simplification." :
                                " darts removed during the simplification."));
      res = true;

      getMapGlobalCharacteristics(&nbdarts,&nbvertices,&nbedges,
                                  &nbfaces,&nbvolumes,&nbcc,
                                  NULL,NULL,NULL,NULL);
      std::cout<<"Map after simplification: darts="<<nbdarts
              <<", vertices="<<nbvertices
             <<", edges="<<nbedges
            <<", faces="<<nbfaces
           <<", volumes="<<nbvolumes
          <<", cc="<<nbcc<<std::endl;
    }
  }

  return res;
}
//******************************************************************************
bool CControlerGMap::simplify3DObject()
{
  bool res = false;

  if (canApplyOperation(COperation(OPERATION_SIMPLIFY_3D_OBJECT)))
  {
    undoRedoPreSave();

    int nbdarts, nbvertices, nbedges, nbfaces, nbvolumes, nbcc;
    getMapGlobalCharacteristics(&nbdarts,&nbvertices,&nbedges,
                                &nbfaces,&nbvolumes,&nbcc,
                                NULL,NULL,NULL,NULL);
    std::cout<<"Map before simplification: darts="<<nbdarts
            <<", vertices="<<nbvertices
           <<", edges="<<nbedges
          <<", faces="<<nbfaces
         <<", volumes="<<nbvolumes
        <<", cc="<<nbcc<<std::endl;

    int m0=-1;
    int m1=-1;
    int m2=-1;

    if ( getParameterGMapVertex()->getModeSimplification() )
    {
      m0=FMap->getNewMark();
      m1=FMap->getNewMark();
      m2=FMap->getNewMark(); // Problem with connected darts
    }

#ifndef _WINDOWS
    CChrono c;
    c.start();
#endif

    int nb;
    if ( getParameterGMapVertex()->getModeSimplification() )
      nb = FMap->simplify3DObject(m0, m1, m2);
    else
      nb = FMap->simplify3DObject();

#ifndef _WINDOWS
    c.stop();
    c.display("Time for simplification");
#endif
    assert(isMapOk());

    if ( nb==0 )
    {
      setMessage("Nothing was simplified, the map was already in "
                 "its minimal form.");
      undoRedoPostSaveFailed();
    }
    else
    {
      if ( getParameterGMapVertex()->getModeSimplification() )
      {
        updateDartAfterRemovals(m0,m1,m2);
        FMap->freeMark(m0);
        FMap->freeMark(m1);
        FMap->freeMark(m2);
      }

      undoRedoPostSaveOk();
      unsetLastSelectedDart();
      setModelChanged();
      setMessage(nb, (nb == 1 ? " dart removed during the simplification." :
                                " darts removed during the simplification."));
      res = true;

      getMapGlobalCharacteristics(&nbdarts,&nbvertices,&nbedges,
                                  &nbfaces,&nbvolumes,&nbcc,
                                  NULL,NULL,NULL,NULL);
      std::cout<<"Map after simplification: darts="<<nbdarts
              <<", vertices="<<nbvertices
             <<", edges="<<nbedges
            <<", faces="<<nbfaces
           <<", volumes="<<nbvolumes
          <<", cc="<<nbcc<<std::endl;
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

