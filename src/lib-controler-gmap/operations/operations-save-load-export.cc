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
#include <iostream>
#include <cassert>

#ifdef MODULE_EXTRACTION_IMAGE
#include "extraction-images.hh"
#endif // MODULE_EXTRACTION_IMAGE

using namespace GMap3d;
using namespace std;
//******************************************************************************
bool CControlerGMap::empty()
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_DELETE_DARTS)))
   {
      undoRedoPreSave();

      FMap->removeAllDarts();

      unsetLastSelectedDart();

      undoRedoPostSaveOk();
      setModelChanged();
      setMessage("Empty the map");
      res = true;
   }
   return res;
}
//******************************************************************************
bool CControlerGMap::addMap(const char * AFilename)
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_LOAD, SUB_OPERATION_ADD_MAP, -1)))
   {
      undoRedoPreSave();

      if (FMap->load(AFilename)!=NULL)
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(string("Add the map from ") + AFilename + " done");
         res = true;
      }
      else
      {
         undoRedoPostSaveFailed();
         setMessage(string("Error for loading ")+AFilename);
      }

      assert(isMapOk());
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::loadMap(const char * AFilename)
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_LOAD, SUB_OPERATION_LOAD_MAP, -1)))
   {
      undoRedoPreSave();

      FMap->removeAllDarts();
      unsetAllLastSelectedDarts();

      if (FMap->load(AFilename)!=NULL)
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(string("Load the map from ")+AFilename+" done");
         res = true;
      }
      else
      {
         undoRedoPostSaveFailed();
         setMessage(string("Error for loading ")+AFilename);
      }

      assert(isMapOk());
      std::cout<<"isMapOk ? "<<isMapOk()<<std::endl;
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::saveMapAscii(const char* AFilename)
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_SAVE)))
   {
      res = FMap->save(AFilename, AsciiFormat);
      if (res)
	setMessage(string("Save in ")+AFilename+" done");
      else
	setMessage(string("Error during saving ")+AFilename);
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::saveMapBinary(const char * AFilename)
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_SAVE)))
   {
      res = FMap->save(AFilename, BinaryFormat);
      if (res)
	setMessage(string("Binary save in ")+AFilename+ " done");
      else
         setMessage(string("Error during binary saving ")+AFilename);
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::exportXfig(const char * AFilename, TViewId AViewId,
                                bool AArrow, bool AFace, bool ASews,
                                bool AGrid, bool AVertices)
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_EXPORT)))
   {
      // updateBurstView();
      ofstream fout(AFilename, ios::trunc);
      sauveCarteXfig(AViewId, fout, AArrow, AFace, ASews, AGrid, AVertices);
      fout.close();
      setMessage(string("Export Xfig ")+AFilename+" done");
      res = true;
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::exportPov(const char * AFilename)
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_EXPORT)))
   {
      // updateBurstView();
      ofstream fout(AFilename, ios::trunc);
      bool ok = FMap->exportToPov(fout);
      fout.close();
      setMessage(ok ? string("Export PoV ")+AFilename+" done" :
		 string("Error during export PoV ")+AFilename);
      res = true;
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::exportXfigAsIntervoxel(const char * AFilename,
      TViewId AViewId)
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_EXPORT)))
   {
      //      updateBurstView();
      ofstream fout(AFilename, ios::trunc);
      sauveCarteIntervoxelXfig(AViewId, fout);
      fout.close();
      setMessage(string("Export in intervoxel ") + AFilename + " done");
      res = true;
   }

   return res;
}
//******************************************************************************
#ifdef MODULE_EXTRACTION_IMAGE
//******************************************************************************
#ifndef MODULE_EXTRACTION_WITHOUT_MAGICK
//******************************************************************************
bool CControlerGMap::extractImage2D(const string & AFilename)
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_EXTRACT_IMAGE, 2)))
   {
      undoRedoPreSave();

      CExtractionImage extractor(FMap);

      if (extractor.extract2dImage(AFilename))
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage("Extraction de l'image 2d effectuée");
         res = true;
      }
      else
      {
         setMessage("Erreur durant l'extraction de l'image 2d");
         undoRedoPostSaveFailed();
      }
   }

   return res;
}
//******************************************************************************
#endif // MODULE_EXTRACTION_WITHOUT_MAGICK
//******************************************************************************
bool CControlerGMap::extractImage3D(const string & AFilename, int AFirstPlane,
                                    int ANbPlane, int ALevel, int NbChiffres,
                                    bool ADestroyBorder, bool AKeepFictiveEdges,
                                    bool AShiftFictiveEdges, bool AWithPrecodes,
                                    bool ACEAFormat)
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_EXTRACT_IMAGE, 3)))
   {
      undoRedoPreSave();

      CExtractionImage extractor(FMap);

      if (ACEAFormat)
      {
         if (AWithPrecodes)
            res =
               extractor.extract3dImageCEAWithPrecodes(AFilename, AFirstPlane,
                                                       ANbPlane, ALevel,
                                                       ADestroyBorder,
                                                       AKeepFictiveEdges,
                                                       AShiftFictiveEdges);
         else
            res = extractor.extract3dImageCEA(AFilename, AFirstPlane, ANbPlane,
                                              ALevel, ADestroyBorder,
                                              AKeepFictiveEdges,
                                              AShiftFictiveEdges);
      }
      else
      {
         if (AWithPrecodes)
            res =
               extractor.extract3dImageWithPrecodes(AFilename, AFirstPlane,
                                                    ANbPlane, ALevel,
                                                    NbChiffres, ADestroyBorder,
                                                    AKeepFictiveEdges,
                                                    AShiftFictiveEdges);
         else
            res = extractor.extract3dImage(AFilename, AFirstPlane, ANbPlane,
                                           ALevel, NbChiffres, ADestroyBorder,
                                           AKeepFictiveEdges,
                                           AShiftFictiveEdges);
      }

      if (res)
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage("Extraction de l'image 3d effectuée");
      }
      else
      {
         setMessage("Erreur durant l'extraction de l'image 3d");
         undoRedoPostSaveFailed();
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::extractOneRegionVoxels(const std::string & AFilename,
      int AFirstPlane, int ANbPlane,
      int NbChiffres,
      int ARed, int AGreen, int ABlue,
      int AAlpha)
{
   bool res = false;
   if (canApplyOperation(COperation(OPERATION_EXTRACT_IMAGE, 3)))
   {
      undoRedoPreSave();

      CExtractionImage extractor(FMap);

      res =
         extractor.extractOneRegionVoxels(AFilename, AFirstPlane,
                                          ANbPlane, NbChiffres,
                                          ARed, AGreen, ABlue, AAlpha);

      if (res)
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage("Extraction des voxels d'une région effectuée");
      }
      else
      {
         setMessage("Erreur durant l'extraction des voxels "
                    "d'une région effectuée");
         undoRedoPostSaveFailed();
      }
   }

   return res;
}
//******************************************************************************
#endif // MODULE_EXTRACTION_IMAGE
//******************************************************************************
bool CControlerGMap::importOff(const char* AFilename)
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_LOAD, SUB_OPERATION_ADD_MAP, -1)))
   {
      undoRedoPreSave();

      if (FMap->importOff(AFilename)!=NULL)
      {
         undoRedoPostSaveOk();
         setModelChanged();
         setMessage(string("Import OFF ") + AFilename + " done");
         res = true;
      }
      else
      {
         undoRedoPostSaveFailed();
         setMessage(string("Error during import OFF ")+AFilename);
      }

      assert(isMapOk());
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::exportOff(const char* AFilename)
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_EXPORT)))
   {
      if (FMap->exportOff(AFilename))
      {
	setMessage(string("Export OFF ") + AFilename + " done");
         res = true;
      }
      else
      {
	setMessage(string("Error during export OFF ")+AFilename);
      }
   }

   return res;
}
//******************************************************************************
bool CControlerGMap::exportOff3D(const char* AFilename)
{
   bool res = false;

   if (canApplyOperation(COperation(OPERATION_EXPORT)))
   {
      if (FMap->exportOff3D(AFilename))
      {
	setMessage(string("Export OFF ")+AFilename+" done");
         res = true;
      }
      else
      {
	setMessage(string("Error during export OFF")+AFilename);
      }
   }

   return res;
}
//******************************************************************************
