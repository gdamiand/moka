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
#include <sstream>

using namespace GMap3d;
//******************************************************************************
void CControlerGMap::setSelectionLevel(int ALevel)
{
   assert(0 <= ALevel && ALevel < getNbSelectionLevels());

   if (ALevel != getSelectionLevel() &&
         canApplyOperation(COperation(OPERATION_SELECT,
                                      SUB_OPERATION_SET_SELECTION_LEVEL, -1)))
   {
      FParameterSelection->setSelectionLevel(ALevel);

      std::stringstream s;
      s<<"Selection level number: "<<ALevel + 1;
      setSelectionChanged();
      setMessage(s.str());
   }
}
//******************************************************************************
void CControlerGMap::setNextSelectionLevel(int AIndex)
{
   setSelectionLevel(getNextSelectionLevel(AIndex));
}
//******************************************************************************
bool CControlerGMap::unmarkAllCurrentSelectionLevel()
{
   if (canApplyOperation(COperation(OPERATION_SELECT,
                                    SUB_OPERATION_REINIT_SELECTION, -1)))
   {
      FMap->unmarkAll(getSelectionMark());
      unsetLastSelectedDart();
      setSelectionChanged();
      setMessage("Darts unmarked");
      return true;
   }

   return false;
}
//******************************************************************************
bool CControlerGMap::negateAllCurrentSelectionLevel()
{
   if (canApplyOperation(COperation(OPERATION_SELECT,
                                    SUB_OPERATION_NEGATE_SELECTION, -1)))
   {
      int mark = getSelectionMark();

      FMap->negateMaskMark(mark);

      CDart* last = getLastSelectedDart();
      if (last != NULL)
         unsetLastSelectedDart();

      setSelectionChanged();
      setMessage("Marked darts negated");
      return true;
   }

   return false;
}
//******************************************************************************
bool CControlerGMap::swapSelectionLevels(int ALevel1, int ALevel2)
{
   if (canApplyOperation(COperation(OPERATION_SELECT,
                                    SUB_OPERATION_SWAP_SELECTIONS, -1)))
   {
      // TEMPORAIRE POUR NE PAS AVOIR A SAUVER LES NUMEROS DES MARQUES
      // CE QU'IL FAUDRAIT FAIRE ENSUITE POUR LE UNDO
      int mark1 = getSelectionMark(ALevel1);
      int mark2 = getSelectionMark(ALevel2);
      CDynamicCoverageAll it(FMap);
      while (it.cont())
      {
         bool t = FMap->isMarked(*it, mark1);
         FMap->setMarkTo(*it, mark1, FMap->isMarked(*it, mark2));
         FMap->setMarkTo(*it, mark2, t);
         ++it;
      }

      // FIN TEMPO
      FParameterSelection->swapLevels(ALevel1, ALevel2);
      setSelectionChanged();

      std::stringstream s;
      s<<"Swap selection levels "<<ALevel1<<" and "<<ALevel2;
      setMessage(s.str());
      return true;
   }

   return false;
}
//******************************************************************************
bool CControlerGMap::swapCurrentSelectionLevelWithNext()
{
   return swapSelectionLevels(getSelectionLevel(), getNextSelectionLevel(1));
}
//******************************************************************************
bool CControlerGMap::selectIncidents()
{
   if (canApplyOperation(COperation(OPERATION_SELECT,
                                    SUB_OPERATION_SELECT_INCIDENTS, -1)))
   {
      FMap->markIncidentCells(getSelectionOrbit(), getSelectionMark());
      setMessage("Incident cells marked");
      setSelectionChanged();
      return true;
   }

   return false;
}
//******************************************************************************
bool CControlerGMap::selectBorders(int ADimension)
{
   assert(0 <= ADimension && ADimension <= 3);

   if (canApplyOperation(COperation(OPERATION_SELECT,
                                    SUB_OPERATION_BORDERS, ADimension)))
   {
      if (FMap->markBorders(getSelectionMark(), ADimension))
      {
         setSelectionChanged();
         setMessage(ADimension, 
                    "-borders incident to selected darts marked");
         return true;
      }
      else
      {
         setMessage("No border marked");
         return false;
      }
   }

   return false;
}
//******************************************************************************
bool CControlerGMap::intuitiveSelectBorders()
{
   if (canApplyOperation(COperation(OPERATION_SELECT,
                                    SUB_OPERATION_BORDERS, -1)))
   {
      if (FMap->markBorders(getSelectionMark()))
      {
         setSelectionChanged();
         setMessage("Borders incident to selected darts marked");
         return true;
      }
      else
      {
         setMessage("No border marked");
         return false;
      }
   }

   return false;
}
//******************************************************************************
bool CControlerGMap::selectNonPlanarFaces()
{
   if (canApplyOperation(COperation(OPERATION_SELECT,
                                    SUB_OPERATION_SELECT_NON_PLANAR_FACES, 
                                    -1)))
   {
      if (FMap->markNonPlanarPolylines(getSelectionMark()))
      {
         setMessage("Non-planar faces marked");
         setSelectionChanged();
         return true;
      }
      else
      {
         setMessage("There is no non-planar face");
         return false;
      }
   }

   return false;
}
//******************************************************************************
bool CControlerGMap::markInterior(int ADimension)
{
   if (canApplyOperation(COperation(OPERATION_SELECT,
                                    SUB_OPERATION_SELECT_INTERIOR, 
                                    ADimension)))
   {
      if (getLastSelectedDart() == NULL)
      {
         setMessage("A 'last' dart must be selected");
         return false;
      }

      FMap->markInterior(getLastSelectedDart(),
                         ADimension, getSelectionMark());

      setSelectionChanged();
      setMessage("Interior marked");
      return true;
   }

   return false;
}
//******************************************************************************
bool CControlerGMap::markIntersection(int AInitialDim, int AFinalDim)
{
   if (canApplyOperation(COperation(OPERATION_SELECT,
                                    SUB_OPERATION_SELECT_INTERSECTION, -1)))
   {
      if (!((1 <= AInitialDim && AInitialDim <= 3) &&
            (0 <= AFinalDim   && AFinalDim   <= 2) &&
            (AInitialDim != AFinalDim)))
      {
         setMessage("Selection of the intersection not possible");
         return false;
      }

      int mark1 = getSelectionMark();
      int mark2 = getNextSelectionMark(1);
      int markR = FMap->getNewMark();

      TOrbit orbit1 = ORBIT_CELL[AInitialDim];
      TOrbit orbit2 = ORBIT_CELL[AFinalDim  ];

      undoRedoPreSave();
      FMap->markIntersection(mark1, mark2, orbit1, orbit2, markR, true);
      FMap->markCopy(markR, mark1);
      undoRedoPostSaveOk();

      setSelectionChanged();
      setMessage("Intersection selected");
      return true;
   }

   return false;
}
//******************************************************************************
bool CControlerGMap::findMotifOff(const char* AFilename)
{
   if (canApplyOperation(COperation(OPERATION_FIND_MOTIF_OFF)))
   {
      CGMapVertex motif;
      if (!motif.importOff(AFilename))
      {
         setMessage("Error to import the Off file");
         return false;
      }
      undoRedoPreSave();
      int index = motif.findMotif(FMap);
      undoRedoPostSaveOk();

      if (index == -1)
         setMessage("Motif not founded in the current map");
      else
      {
         setMessage("Motif founded");
         for (CDynamicCoverageAll it(&motif); it.cont(); ++it)
         {
            assert(motif.getDirectInfo(*it, index) != NULL);
            FMap->setMark(motif.getDirectInfoAsDart(*it, index),
                          getSelectionMark());
         }
         motif.freeDirectInfo(index);
         setSelectionChanged();
      }
      return true;
   }
   return false;
}
//******************************************************************************
bool CControlerGMap::findMotifMoka(const char* AFilename)
{
   if (canApplyOperation(COperation(OPERATION_FIND_MOTIF_MOKA)))
   {
      CGMapVertex motif;
      TSaveFormat format = CGMapVertex::getFileFormat(AFilename);
      if (motif.load(AFilename, format)==NULL)
      {
         setMessage("Error to load the file");
         return false;
      }
      undoRedoPreSave();
      int index = motif.findMotif(FMap);
      undoRedoPostSaveOk();

      if (index == -1)
         setMessage("Motif not founded in the current map");
      else
      {
         setMessage("Motif founded");
         for (CDynamicCoverageAll it(&motif); it.cont(); ++it)
         {
            assert(motif.getDirectInfo(*it, index) != NULL);
            FMap->setMark(motif.getDirectInfoAsDart(*it, index),
                          getSelectionMark());
         }
         motif.freeDirectInfo(index);
         setSelectionChanged();
      }
      return true;
   }
   return false;
}
//******************************************************************************
bool CControlerGMap::countNumberOfMotifsMoka(const char* AFilename)
{
   if (canApplyOperation(COperation(OPERATION_FIND_MOTIF_MOKA)))
   {
      CGMapVertex motif;
      TSaveFormat format = CGMapVertex::getFileFormat(AFilename);
      if (motif.load(AFilename, format)==NULL)
      {
         setMessage("Error to load the file");
         return false;
      }
      undoRedoPreSave();
      unsigned int res = motif.countNumberOfMotifs(FMap);
      undoRedoPostSaveOk();

      if (res == 0)
         setMessage("Motif not founded in the current map");
      else
      {
	setMessage("Motif founded ", res, (res>1?"times":"time"));
      }
      return true;
   }
   return false;
}
//******************************************************************************
bool CControlerGMap::countNumberOfMotifsOff(const char* AFilename)
{
   if (canApplyOperation(COperation(OPERATION_FIND_MOTIF_OFF)))
   {
      CGMapVertex motif;
      if (!motif.importOff(AFilename))
      {
         setMessage("Error to import the Off file");
         return false;
      }
      undoRedoPreSave();
      unsigned int res = motif.countNumberOfMotifs(FMap);
      undoRedoPostSaveOk();

      if (res == 0)
         setMessage("Motif not founded in the current map");
      else
      {
         setMessage("Motif founded ", res, (res>1?"times":"time"));
      }
      return true;
   }
   return false;
}
//******************************************************************************
