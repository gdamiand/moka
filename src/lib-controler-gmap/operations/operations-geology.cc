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
#ifdef MODULE_GEOLOGY

#include "g-map-vertex.hh"
#include "controler-gmap.hh"
#include "operations.hh"
#include "parameter-selection.hh"
#include "view-precompile.hh"

#include "geology-api.hh"
#include "geo-tools.hh"
#include "corefinement-api.hh"
#include "corefine-3d.hh"

using namespace GMap3d;

//******************************************************************************

bool CControlerGMap::createUncertainZone()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_UNCERTAIN_ZONE, -1))) {
    CGeologyAPI geo(FMap);
    CDart *d1, *d2;
    
    if (FMap->getMarkedCells(ORBIT_CC, getSelectionMark(), NULL, &d1) != 1) {
      setMessage("Sélection maillage 1 incohérente");
      return false;
    }
    
    if (FMap->getMarkedCells(ORBIT_EDGE, getNextSelectionMark(1),
			     NULL, &d2) != 1) {
      setMessage("Sélection de distance incohérente");
      return false;
    }

    CVertex v = *FMap->findVertex(FMap->alpha0(d2)) - *FMap->findVertex(d2);

    undoRedoPreSave();
    geo.createUncertainZone(d1, v.norm(), 5);
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Création d'une zone d'incertitude effectuée");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::createNearestIntersections()
{
  if (canApplyOperation(COperation(OPERATION_COREFINE,
				   SUB_OPERATION_COREFINE3D, -1)))
    {
      CDart *d1, *d2;

      if (FMap->getMarkedCells(ORBIT_CC, getSelectionMark(), NULL, &d1) != 1)
	{
	  setMessage("Sélection maillage 1 incohérente");
	  return false;
	}

      if (FMap->getMarkedCells(ORBIT_CC, getNextSelectionMark(1),
			       NULL, &d2) != 1)
	{
	  setMessage("Sélection maillage 2 incohérente");
	  return false;
	}

      undoRedoPreSave();
     
      CCorefine3d *coref = new CCorefine3d(FMap);

      coref->computeOnlyFirstIntersection(true);
      coref->splitAndMergeMeshes(d1, d2);

      delete coref;

      undoRedoPostSaveOk();

      setModelChanged();
      setMessage("Co-raffinement 3D effectué");
      return true;
    }

  return false;
}

//******************************************************************************

bool CControlerGMap::extendSelectedBorders()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_EXTEND, -1))) {
    CGeologyAPI geo(FMap);
    CDart *d;
    
    if (FMap->getMarkedCells(ORBIT_EDGE, getNextSelectionMark(1),
			     NULL, &d) != 1) {
      setMessage("Sélection de distance incohérente");
      return false;
    }

    CVertex v = *FMap->findVertex(FMap->alpha0(d)) - *FMap->findVertex(d);

    undoRedoPreSave();
    geo.extendSelectedBorders(getSelectionMark(), v.norm());
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Extensions des bords effectuées");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::extendSelectedBordersToSurface()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_EXTEND_TO, -1))) {
    CGeologyAPI geo(FMap);
    CDart *d;
    
    if (FMap->getMarkedCells(ORBIT_CC, getNextSelectionMark(1),
			     NULL, &d) != 1) {
      setMessage("Sélection de la surface incohérente");
      return false;
    }

    undoRedoPreSave();
    geo.extendSelectedBordersToSurface(getSelectionMark(), d);
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Extension des bords effectué");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::smoothSelection()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_SMOOTH, -1))) {
    CGeologyAPI geo(FMap);
    
    undoRedoPreSave();
    geo.smoothSelection(getSelectionMark());
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Lissage effectué");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::relaxSelection()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_RELAX, -1))) {
    CGeologyAPI geo(FMap, 1E-2);
    
    undoRedoPreSave();
    geo.relaxSelection(getSelectionMark());
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Relachement effectué");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::simplifyMesh()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_SIMPLIFY, -1))) {
    CGeologyAPI geo(FMap);
//     CDart *d;
    
//     if (FMap->getMarkedCells(ORBIT_CC, getSelectionMark(), NULL, &d) != 1) {
//       setMessage("Sélection maillage 1 incohérente");
//       return false;
//     }
    
    undoRedoPreSave();
    geo.optimizeSelection(getSelectionMark(), 5);
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Maillage simplifié");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::centerSelection()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_CENTER, -1))) {
    CGeoTools tools(FMap);
    
    undoRedoPreSave();
    tools.centerSelectedDarts(getSelectionMark());
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Centrage effectué");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::plateVerticesOnFaces()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_PLATE_ON_FACES, -1))) {
    CGeologyAPI geo(FMap);
    CDart *d;
    CVertex v;

    int nb = FMap->getMarkedCells(ORBIT_EDGE, getNextSelectionMark(2),
				  NULL, &d);

    if (nb > 1) {
      setMessage("Sélection de direction incohérente");
      return false;
    }

    if (nb == 0) v = OZ;
    else v = *FMap->findVertex(FMap->alpha0(d)) - *FMap->findVertex(d);

    undoRedoPreSave();
    geo.plateVerticesOnFaces(getSelectionMark(), getNextSelectionMark(1), v);
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Plaquage effectué");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::plateVerticesOnEdges()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_PLATE_ON_EDGES, -1))) {
    CGeologyAPI geo(FMap);
    CDart *d;
    CVertex v;

    int nb = FMap->getMarkedCells(ORBIT_EDGE, getNextSelectionMark(2),
				  NULL, &d);

    if (nb != 1) {
      setMessage("Sélection de direction incohérente");
      return false;
    }

    v = *FMap->findVertex(FMap->alpha0(d)) - *FMap->findVertex(d);

    undoRedoPreSave();
    geo.plateVerticesOnEdges(getSelectionMark(), getNextSelectionMark(1), v);
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Plaquage effectué");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::plateVerticesOnVertices()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_PLATE_ON_VERTICES, -1))) {
    CGeologyAPI geo(FMap);

    undoRedoPreSave();
    geo.plateVerticesOnVertices(getSelectionMark(), getNextSelectionMark(1));
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Plaquage effectué");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::selectMacroVertices()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_SELECT_VERTICES, -1))) {
    CGeologyAPI geo(FMap);
    
    geo.selectMacroVertices(getSelectionMark());

    setMessage("Selection effectuée");
    setSelectionChanged();
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::selectAllMacroEdges()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_SELECT_ALL_EDGES, -1))) {
    CGeologyAPI geo(FMap);
    
    geo.selectAllMacroEdges(getSelectionMark());
    
    setMessage("Selection effectuée");
    setSelectionChanged();
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::selectMacroEdges()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_SELECT_EDGES, -1))) {
    CGeologyAPI geo(FMap);
    
    geo.selectMacroEdges(getSelectionMark(), getNextSelectionMark(1));
    
    setMessage("Selection effectuée");
    setSelectionChanged();
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::selectMacroFaces()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_SELECT_FACES, -1))) {
    CGeologyAPI geo(FMap);
    
    geo.selectMacroFaces(getSelectionMark(), getNextSelectionMark(1));
    
    setMessage("Selection effectuée");
    setSelectionChanged();
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::selectBordersBetweenSelectedDarts()
{
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_SELECT_BORDERS, -1))) {
    CGeoTools tools(FMap);
    
    tools.selectBordersBetweenSelectedDarts(getSelectionMark());
    
    setMessage("Selection des bords effectuée");
    setSelectionChanged();
    return true;
  }
  
  return false;
}

//******************************************************************************
bool CControlerGMap::importGr2d(const char * AFileName)
{
  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_IMPORT_GR2D, -1))) {
    CGeologyAPI geo(FMap);
    
    undoRedoPreSave();
    
    if (geo.importFile(AFileName, GR2D_Format)) {
      undoRedoPostSaveOk();
      setModelChanged();
      setMessage("Chargement effectué");
      res = true;
    }
    else {
      undoRedoPostSaveFailed();
      setMessage("Chargement impossible");
    }
    
    assert(isMapOk());
  }
  
  return res;
}

//******************************************************************************

bool CControlerGMap::importXyz(const char * AFileName)
{
  bool res = false;

  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_IMPORT_XYZ, -1))) {
    CGeologyAPI geo(FMap);
    
    undoRedoPreSave();
    
    if (geo.importFile(AFileName, XYZ_Format)) {
      undoRedoPostSaveOk();
      setModelChanged();
      setMessage("Chargement effectué");
      res = true;
    }
    else {
      undoRedoPostSaveFailed();
      setMessage("Chargement impossible");
    }
    
    assert(isMapOk());
  }
  
  return res;
}

//******************************************************************************

bool CControlerGMap::importTs(const char * AFileName)
{
  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_IMPORT_TS, -1))) {
    CGeologyAPI geo(FMap);
    
    undoRedoPreSave();
    
    if (geo.importFile(AFileName, TS_Format)) {
      undoRedoPostSaveOk();
      setModelChanged();
      setMessage("Chargement effectué");
      res = true;
    }
    else {
      undoRedoPostSaveFailed();
      setMessage("Chargement impossible");
    }
    
    assert(isMapOk());
  }
  
  return res;
}

//******************************************************************************

bool CControlerGMap::importCloud(const char * AFileName)
{
  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_GEOLOGY,
				   SUB_OPERATION_GEOLOGY_IMPORT_CLOUD, -1))) {
    CGeologyAPI geo(FMap);
    
    undoRedoPreSave();
    
    if (geo.importFile(AFileName, CLOUD_Format)) {
      undoRedoPostSaveOk();
      setModelChanged();
      setMessage("Chargement effectué");
      res = true;
    }
    else {
      undoRedoPostSaveFailed();
      setMessage("Chargement impossible");
    }
    
    assert(isMapOk());
  }
  
  return res;
}
//******************************************************************************
#endif // MODULE_GEOLOGY
//******************************************************************************
