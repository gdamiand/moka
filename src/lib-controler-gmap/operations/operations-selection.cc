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
void CControlerGMap::setSelectionLevel(int ALevel)
{
   assert(0 <= ALevel && ALevel < getNbSelectionLevels());

   if (ALevel != getSelectionLevel() &&
         canApplyOperation(COperation(OPERATION_SELECT,
                                      SUB_OPERATION_SET_SELECTION_LEVEL, -1)))
   {
      FParameterSelection->setSelectionLevel(ALevel);
      char message[64];
      sprintf(message, "Niveau de sélection : %d", ALevel + 1);
      setSelectionChanged();
      setMessage(message);
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
      setMessage("Brins démarqués");
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
      setMessage("Marque inversée");
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

      char message[64];
      sprintf(message, "Échange des niveaux de sélection %d et %d",
              ALevel1, ALevel2);
      setMessage(message);
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
      setMessage("Cellules incidentes marquées");
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
                    "-bords incidents aux brins sélectionnés marqués");
         return true;
      }
      else
      {
         setMessage("Aucun bord marqué");
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
         setMessage("Bords incidents aux brins sélectionnés marqués");
         return true;
      }
      else
      {
         setMessage("Aucun bord marqué");
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
         setMessage("Faces non planes ajoutées à la sélection");
         setSelectionChanged();
         return true;
      }
      else
      {
         setMessage("Pas de face non plane");
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
         setMessage("Un brin 'last' doit être sélectionné");
         return false;
      }

      FMap->markInterior(getLastSelectedDart(),
                         ADimension, getSelectionMark());

      setSelectionChanged();
      setMessage("Intérieur sélectionné");
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
         setMessage("Sélection de l'intersection impossible");
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
      setMessage("Intersection sélectionnée");
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
         setMessage("Erreur de chargement du fichier Off.");
         return false;
      }
      undoRedoPreSave();
      int index = motif.findMotif(FMap);
      undoRedoPostSaveOk();

      if (index == -1)
         setMessage("Motif non trouvé dans la carte courante.");
      else
      {
         setMessage("Motif trouvé.");
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
         setMessage("Erreur de chargement du fichier.");
         return false;
      }
      undoRedoPreSave();
      int index = motif.findMotif(FMap);
      undoRedoPostSaveOk();

      if (index == -1)
         setMessage("Motif non trouvé dans la carte courante.");
      else
      {
         setMessage("Motif trouvé.");
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
         setMessage("Erreur de chargement du fichier.");
         return false;
      }
      undoRedoPreSave();
      unsigned int res = motif.countNumberOfMotifs(FMap);
      undoRedoPostSaveOk();

      if (res == 0)
         setMessage("Motif non trouvé dans la carte courante.");
      else
      {
         setMessage("Motif trouvé ", res, " fois.");
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
         setMessage("Erreur de chargement du fichier Off.");
         return false;
      }
      undoRedoPreSave();
      unsigned int res = motif.countNumberOfMotifs(FMap);
      undoRedoPostSaveOk();

      if (res == 0)
         setMessage("Motif non trouvé dans la carte courante.");
      else
      {
         setMessage("Motif trouvé ", res, " fois.");
      }
      return true;
   }
   return false;
}
//******************************************************************************
