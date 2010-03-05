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
#include "g-map-vertex.hh"
#include "parameter-selection.hh"
#include "view-precompile.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
static string textOrbit[16] =
{
   "Dart", "0", "1", "01",
   "2", "02", "12", "Volume",
   "3", "03", "13", "Face",
   "23", "Edge", "Vertex", "Connected component"
};
//******************************************************************************
int CControlerGMap::getNbSelectionLevels() const
{ return FParameterSelection->getNbSelectionLevels(); }
//******************************************************************************
int CControlerGMap::getSelectionLevel() const
{ return FParameterSelection->getSelectionLevel(); }
//------------------------------------------------------------------------------
int CControlerGMap::getNextSelectionLevel(int AIndex) const
{ return FParameterSelection->getNextSelectionLevel(AIndex); }
//******************************************************************************
TOrbit CControlerGMap::getSelectionOrbit() const
{ return FParameterSelection->getSelectionOrbit(); }
//------------------------------------------------------------------------------
void CControlerGMap::setSelectionOrbit(TOrbit AOrbit)
{
   FParameterSelection->setSelectionOrbit(AOrbit);

   string message("Selected orbit: orbit ");
   setMessage(message + textOrbit[getSelectionOrbit()]);
}
//------------------------------------------------------------------------------
void CControlerGMap::setNextSelectionOrbit()
{
   FParameterSelection->setNextSelectionOrbit();

   string message("Selected orbit: orbit ");
   setMessage(message + textOrbit[getSelectionOrbit()]);
}
//------------------------------------------------------------------------------
bool CControlerGMap::getHalfSelectionOrbit() const
{ return FParameterSelection->getHalfSelection(); }
//------------------------------------------------------------------------------
void CControlerGMap::setHalfSelectionOrbit(bool ABool)
{ FParameterSelection->setHalfSelection(ABool); }
//******************************************************************************
int CControlerGMap::getSelectionMark() const
{ return FParameterSelection->getSelectionMark(); }
//------------------------------------------------------------------------------
int CControlerGMap::getSelectionMark(int ALevel) const
{ return FParameterSelection->getSelectionMark(ALevel); }
//------------------------------------------------------------------------------
int CControlerGMap::getNextSelectionMark(int AIndex) const
{ return FParameterSelection->getNextSelectionMark(AIndex); }
//******************************************************************************
CDart* CControlerGMap::getLastSelectedDart() const
{ return FParameterSelection->getLastSelectedDart(); }
//------------------------------------------------------------------------------
CDart* CControlerGMap::getLastSelectedDart(int ALevel) const
{ return FParameterSelection->getLastSelectedDart(ALevel); }
//------------------------------------------------------------------------------
CDart* CControlerGMap::getNextLastSelectedDart(int AIndex) const
{ return FParameterSelection->getNextLastSelectedDart(AIndex); }
//******************************************************************************
void CControlerGMap::unsetLastSelectedDart()
{ FParameterSelection->unsetLastSelectedDart(); }
//------------------------------------------------------------------------------
void CControlerGMap::unsetLastSelectedDart(int ALevel)
{ FParameterSelection->unsetLastSelectedDart(ALevel); }
//------------------------------------------------------------------------------
void CControlerGMap::unsetNextLastSelectedDart(int AIndex)
{ FParameterSelection->unsetNextLastSelectedDart(AIndex); }
//------------------------------------------------------------------------------
void CControlerGMap::unsetAllLastSelectedDarts()
{ FParameterSelection->unsetAllLastSelectedDarts(); }
//******************************************************************************
bool CControlerGMap::isDartSelected(CDart* ADart) const
{ return FMap->isMarked(ADart, getSelectionMark()); }
//------------------------------------------------------------------------------
bool CControlerGMap::isDartSelected(CDart* ADart, int ALevel) const
{ return FMap->isMarked(ADart, getSelectionMark(ALevel)); }
//------------------------------------------------------------------------------
bool CControlerGMap::isDartNextSelected(CDart* ADart, int AIndex) const
{ return FMap->isMarked(ADart, getNextSelectionMark(AIndex)); }
//******************************************************************************
void CControlerGMap::selectDart(CDart* ADart)
{ selectDart(ADart, getSelectionLevel()); }
//------------------------------------------------------------------------------
void CControlerGMap::selectDart(CDart* ADart, int ALevel)
{
   assert(0 <= ALevel && ALevel < getNbSelectionLevels());
   assert(ADart != NULL);

   if (!isDartSelected(ADart, ALevel))
   {
      FMap->setMark(ADart, getSelectionMark(ALevel));
      setSelectionChanged();
   }

   FParameterSelection->setLastSelectedDart(ALevel, ADart);
}
//------------------------------------------------------------------------------
void CControlerGMap::selectNextDart(CDart* ADart, int AIndex)
{ selectDart(ADart, getNextSelectionLevel(AIndex)); }
//******************************************************************************
void CControlerGMap::deselectDart(CDart* ADart)
{ deselectDart(ADart, getSelectionLevel()); }
//------------------------------------------------------------------------------
void CControlerGMap::deselectDart(CDart* ADart, int ALevel)
{
   assert(0 <= ALevel && ALevel < getNbSelectionLevels());
   assert(ADart != NULL);

   if (isDartSelected(ADart, ALevel))
   {
      FMap->unsetMark(ADart, getSelectionMark(ALevel));
      setSelectionChanged();

      if (ADart == getLastSelectedDart(ALevel))
         FParameterSelection->unsetLastSelectedDart(ALevel);
   }
}
//------------------------------------------------------------------------------
void CControlerGMap::deselectNextDart(CDart* ADart, int AIndex)
{ deselectDart(ADart, getNextSelectionLevel(AIndex)); }
//******************************************************************************
void CControlerGMap::toggleDartSelection(CDart* ADart)
{ toggleDartSelection(ADart, getSelectionLevel()); }
//------------------------------------------------------------------------------
void CControlerGMap::toggleDartSelection(CDart* ADart, int ALevel)
{
   assert(0 <= ALevel && ALevel < getNbSelectionLevels());
   assert(ADart != NULL);

   if (isDartSelected(ADart, ALevel)) deselectDart(ADart, ALevel);
   else                                 selectDart(ADart, ALevel);
}
//------------------------------------------------------------------------------
void CControlerGMap::toggleDartNextSelection(CDart* ADart, int AIndex)
{ toggleDartSelection(ADart, getNextSelectionLevel(AIndex)); }
//******************************************************************************
void CControlerGMap::selectOrbit(CDart* ADart)
{  selectOrbit(ADart, getSelectionLevel()); }
//------------------------------------------------------------------------------
void CControlerGMap::selectOrbit(CDart* ADart, int ALevel)
{
   assert(0 <= ALevel && ALevel < getNbSelectionLevels());
   assert(ADart != NULL);

   if (FParameterSelection->getHalfSelection() &&
         FMap->isOrientable(ADart, getSelectionOrbit()))
      FMap->halfMarkOrbit(ADart,getSelectionOrbit(),getSelectionMark(ALevel));
   else
      FMap->markOrbit(ADart, getSelectionOrbit(), getSelectionMark(ALevel));

   FParameterSelection->setLastSelectedDart(ALevel, ADart);
   setSelectionChanged();
}
//------------------------------------------------------------------------------
void CControlerGMap::selectNextOrbit(CDart* ADart, int AIndex)
{ selectOrbit(ADart, getNextSelectionLevel(AIndex)); }
//******************************************************************************
void CControlerGMap::deselectOrbit(CDart* ADart)
{ deselectOrbit(ADart, getSelectionLevel()); }
//------------------------------------------------------------------------------
void CControlerGMap::deselectOrbit(CDart* ADart, int ALevel)
{
   assert(0 <= ALevel && ALevel < getNbSelectionLevels());
   assert(ADart != NULL);

   if (FParameterSelection->getHalfSelection() &&
         FMap->isOrientable(ADart, getSelectionOrbit()))
      FMap->halfUnmarkOrbit(ADart, getSelectionOrbit(),
                            getSelectionMark(ALevel));
   else
      FMap->unmarkOrbit(ADart, getSelectionOrbit(), getSelectionMark(ALevel));

   if (getLastSelectedDart(ALevel) != NULL &&
         !isDartSelected(getLastSelectedDart(ALevel), ALevel))
      FParameterSelection->unsetLastSelectedDart(ALevel);

   setSelectionChanged();
}
//------------------------------------------------------------------------------
void CControlerGMap::deselectNextOrbit(CDart* ADart, int AIndex)
{ deselectOrbit(ADart, getNextSelectionLevel(AIndex)); }
//******************************************************************************
void CControlerGMap::toggleOrbitSelection(CDart* ADart)
{ toggleOrbitSelection(ADart, getSelectionLevel()); }
//------------------------------------------------------------------------------
void CControlerGMap::toggleOrbitSelection(CDart* ADart, int ALevel)
{
   assert(0 <= ALevel && ALevel < getNbSelectionLevels());
   assert(ADart != NULL);

   if (isDartSelected(ADart, ALevel)) deselectOrbit(ADart, ALevel);
   else                                 selectOrbit(ADart, ALevel);
}
//------------------------------------------------------------------------------
void CControlerGMap::toggleOrbitNextSelection(CDart* ADart, int AIndex)
{  toggleOrbitSelection(ADart, getNextSelectionLevel(AIndex)); }
//******************************************************************************
bool CControlerGMap::takeCenter(CVertex & AResult)
{
   int    sel  = getNextSelectionMark(1);
   CDart* last = getNextLastSelectedDart(1);

   if (last == NULL)
      if (FMap->getMarkedCells(ORBIT_VERTEX, sel, NULL, &last) != 1)
         return false;

   AResult = * FMap->findVertex(last);
   return true;
}
//------------------------------------------------------------------------------
bool CControlerGMap::takeVector(CVertex & AResult)
{
   int    sel  = getNextSelectionMark(1);
   CDart* last = getNextLastSelectedDart(1);

   if (last == NULL)
      return false;

   CDart* d1;
   CDart* d2;

   int nb = FMap->getMarkedCells(ORBIT_VERTEX, sel, last, &d1, &d2);

   if (nb != 1 && nb != 2)
      return false;

   if (nb == 1)
   {
      if (FMap->isFree1(last))
         return false;

      d2 = FMap->alpha0(d1);
   }

   // Axe definition
   AResult = * FMap->findVertex(d1) - * FMap->findVertex(d2);
   return true;
}
//******************************************************************************
void CControlerGMap::setModeDeselectionAtStop()
{ FDeselectMode = true; }
//------------------------------------------------------------------------------
bool CControlerGMap::getModeDeselectionAtStop() const
{ return FDeselectMode; }
//******************************************************************************
void CControlerGMap::modeSelectionOperationStop()
{
   CViewPrecompile* view = FViews[FCurrentViewId];

   if (view != NULL  &&  FFirstX != FLastX  &&  FFirstY != FLastY)
   {
      CDart * dart;
      float screen1[3], screen2[3];
      int xmin, ymin, xmax, ymax;
      CVertex v1, v2;

      if (FFirstX < FLastX){ xmin = FFirstX; xmax = FLastX; }
      else { xmin = FLastX; xmax = FFirstX; }
      if (FFirstY < FLastY){ ymin = FFirstY; ymax = FLastY; }
      else { ymin = FLastY; ymax = FFirstY; }

      // Pointeur vers fonction membre pour éviter de faire le
      // meme test plein de fois dans la boucle.
      void (CControlerGMap::*f)(CDart*);
      if (FDeselectMode) f = &CControlerGMap::deselectDart;
      else f = &CControlerGMap::selectDart;

      // Parcours des brins   
      for (CDynamicCoverageAll coverage(FMap); coverage.cont(); ++coverage)
      {
         dart = *coverage;

         v1 = FMap -> getBurstVertex(dart);
         view -> project(v1.getX(), v1.getY(), v1.getZ(), screen1);

         if (screen1[0] >= xmin  &&  screen1[1] >= ymin  &&
               screen1[0] <= xmax  &&  screen1[1] <= ymax)
            (this->*f)(dart);
         else
         {
            v2 = FMap -> computeBurstExtremity(dart);
            view -> project(v2.getX(), v2.getY(), v2.getZ(), screen2);

            if (screen2[0] >= xmin  &&  screen2[1] >= ymin &&
                  screen2[0] <= xmax  &&  screen2[1] <= ymax)
               (this->*f)(dart);
         }
      }
   }
   FDeselectMode = false;
}
//******************************************************************************
