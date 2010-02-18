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
#include "controler-gmap-types.hh"
#include "view.hh"
#include "precompile-dart.hh"
#include "dart-vertex.hh"
#include "parameter-gmap-vertex.hh"
#include "parameter-dart.hh"
#include "parameter-selection.hh"
#include <cassert>
#include <climits>
using namespace GMap3d;
//******************************************************************************
CPrecompileDart::CPrecompileDart(CParameterGMapVertex * AParameterGMapV,
                                 CParameterDart       * AParameterDart,
                                 CParameterSelection  * AParameterSelection) :
      FParameterGMapV(AParameterGMapV),
      FParameterDart(AParameterDart),
      FParameterSelection(AParameterSelection)
{
   assert(FParameterGMapV != NULL && FParameterDart != NULL);

   FParameterGMapV->addPrecompileToUpdate(this);
   FParameterDart->addPrecompileToUpdate(this);

   if (FParameterSelection != NULL)
      FParameterSelection->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileDart::CPrecompileDart(const CPrecompileDart & APrecompile) :
      CPrecompile(APrecompile),
      FParameterGMapV(APrecompile.FParameterGMapV),
      FParameterDart(static_cast<CParameterDart*>
                     (APrecompile.FParameterDart->copy())),
      FParameterSelection(APrecompile.FParameterSelection)
{
   assert(FParameterGMapV != NULL && FParameterDart != NULL);

   FParameterGMapV->addPrecompileToUpdate(this);
   FParameterDart->addPrecompileToUpdate(this);

   if (FParameterSelection != NULL)
      FParameterSelection->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileDart::~CPrecompileDart()
{
   FParameterGMapV->removePrecompileToUpdate(this);
   FParameterDart->removePrecompileToUpdate(this);

   if (FParameterSelection != NULL)
      FParameterSelection->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompileDart::copy() const
{ return new CPrecompileDart(*this); }
//******************************************************************************
void CPrecompileDart::setParameter(CParameter* AParameter)
{
   switch (AParameter->getType())
   {
      case PARAMETER_GMAP_VERTEX:
         setGMapVertex(static_cast<CParameterGMapVertex *>(AParameter));
         break;
      case PARAMETER_DART:
         setDart(static_cast<CParameterDart *>(AParameter));
         break;
      case PARAMETER_SELECTION:
         setSelection(static_cast<CParameterSelection *>(AParameter));
         break;
   }
}
//******************************************************************************
CParameter* CPrecompileDart::getParameter() const
{ return FParameterDart; }
//******************************************************************************
void CPrecompileDart::setGMapVertex(CParameterGMapVertex* AParameterGMapV)
{
   assert(AParameterGMapV != NULL);
   AParameterGMapV->addPrecompileToUpdate(this);
   FParameterGMapV->removePrecompileToUpdate(this);
   FParameterGMapV = AParameterGMapV;
   setToUpdate();
}
//******************************************************************************
void CPrecompileDart::setDart(CParameterDart* ADart)
{
   assert(ADart != NULL);
   ADart->addPrecompileToUpdate(this);
   FParameterDart->removePrecompileToUpdate(this);
   FParameterDart = ADart;
   setToUpdate();
}
//******************************************************************************
void CPrecompileDart::setSelection(CParameterSelection* ASelection)
{
   if (ASelection != NULL)
      ASelection->addPrecompileToUpdate(this);

   if (FParameterSelection != NULL)
      FParameterSelection->removePrecompileToUpdate(this);

   FParameterSelection = ASelection;
   setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileDart::getType() const
{ return PRECOMPILE_DART; }
//******************************************************************************
CDart* CPrecompileDart::nextDartAlpha(CDart* ADart, unsigned int ADim)
{
  assert(ADim<=3);
  
  if ( !FParameterGMapV->getModeSimplification() || ADim==3 )
    return ADart->getAlpha(ADim);
 
  assert(!FParameterGMapV->getDrawingMap()->
	 isMarked(ADart,FParameterGMapV->getMarkRemoved(ADim)));
  CDart* res = ADart->getAlpha(ADim);
  while ( FParameterGMapV->getDrawingMap()->
	  isMarked(res,FParameterGMapV->getMarkRemoved(ADim)) )
    {
      res = res->getAlpha(ADim+1)->getAlpha(ADim);
    }
  return res;
}
//******************************************************************************
void CPrecompileDart::drawOneEdge(CDart * ADart)
{
  if  ( !FParameterGMapV->getModeSimplification() ||
	FParameterGMapV->getDrawingMap()->isFree0(ADart) )
    return;
  
  CDart* start = FParameterGMapV->getDartWithEmbedding(ADart);
  CDart* end = FParameterGMapV->getDartWithEmbedding(ADart->getAlpha(0));
  while ( start!=end )
    {
      start = start->getAlpha(0);
      if (start!=end)
	{
	  drawOneDart(start);
	  start = nextDartAlpha(start,1);
	  if (start!=end) drawOneDart(start);
	}
    }
}
//******************************************************************************
void CPrecompileDart::drawOneDart(CDart * ADart)
{
  CVertex * v1 = &FParameterGMapV->getDrawingMap()->getBurstVertex(ADart);

  if (FParameterGMapV->getDrawingMap()->isFree0(ADart))
   {
      // Affichage d'un point pour les brins 0-libres
      glBegin(GL_POINTS);
      PLOT(v1);
      glEnd();
   }
   else
   {
      // Affichage de l'arête pour les brins 0-cousus
     CVertex v2 = FParameterGMapV->getDrawingMap()->
       computeBurstExtremity(ADart);
      glBegin(GL_LINES);
      LINE(v1, &v2);
      glEnd();
   }
}
//******************************************************************************
void CPrecompileDart::drawModel()
{
   glLineWidth(FParameterDart->getLWDarts());
   glPointSize(FParameterDart->getSAloneDarts());

   CVertex m;
   const GLfloat * clSel   = NULL;
   const GLfloat * clUnsel = NULL;
   const GLfloat * clLast  = NULL;
   const GLfloat * cl0Remove= NULL;
   const GLfloat * cl1Remove= NULL;
   int mark = -1;

   CDart * last = NULL;

   if (FParameterSelection != NULL)
   {
      mark = FParameterSelection->getSelectionMark();
      last = FParameterSelection->getLastSelectedDart();
      
      clSel   = FParameterDart->
	getCLSel(FParameterSelection->getSelectionLevel());
      clUnsel = FParameterDart->
	getCLUnsel(FParameterSelection->getSelectionLevel());
      clLast  = FParameterDart->
	getCLLastSel(FParameterSelection->getSelectionLevel());
      cl0Remove= FParameterDart->
	getCL0Remove(FParameterSelection->getSelectionLevel());
      cl1Remove= FParameterDart->
	getCL1Remove(FParameterSelection->getSelectionLevel());
   }
   else
     clSel = FParameterDart->getCLUnsel(0);

   // 1) Draw the remove darts
   if ( FParameterGMapV->getModeSimplification() )
     {
       glColor3fv(cl1Remove);
       for (CDynamicCoverageAll it(FParameterGMapV->getMapEmbedding());
	    it.cont(); ++it)
	 {
	   if ( FParameterGMapV->getMapEmbedding()->
		isMarked(*it, FParameterGMapV->getMarkRemoved(1)))
	     drawOneDart(*it);
	 }
     }

   CDynamicCoverageAll it(FParameterGMapV->getMap());

   // Dessin des brins non sélectionnés :
   for (; it.cont(); ++it)
      if (mark == -1 || !FParameterGMapV->getMap()->isMarked(*it, mark))
	{
	  glColor3fv(clUnsel);
	  drawOneDart(FParameterGMapV->getDartWithEmbedding(*it));
	  if ( FParameterGMapV->getModeSimplification() )
	    {
	      glColor3fv(cl0Remove);
	      drawOneEdge(*it);
	    }
	}

   if (mark != -1)
   {
      // Dessin des brins sélectionnés différents du "last"
     glColor3fv(clSel);
     for (it.reinit(); it.cont(); ++it)
       if (FParameterGMapV->getMap()->isMarked(*it, mark) && *it != last)
	 {
	   drawOneDart(FParameterGMapV->getDartWithEmbedding(*it));
	   drawOneEdge(*it);
	 }
   }

   // Dessin du dernier brin sélectionné
   if (last != NULL)
   {
      glColor3fv(clLast);
      drawOneDart(FParameterGMapV->getDartWithEmbedding(last));
      drawOneEdge(last);
   }
}
//******************************************************************************
#define DMAX (20) // Distance maximum pour la sélection d'un segment.
#define RFREE (6) // Distance maximum pour la sélection d'un point.
//******************************************************************************
float distanceToSegment(float x, float y, float x1, float y1,
                        float x2, float y2)
{
#define SQR(X) (sqr((double)(X)))

   float xa = MIN(x1, x2) - DMAX;
   float xb = MAX(x1, x2) + DMAX;
   float ya = MIN(y1, y2) - DMAX;
   float yb = MAX(y1, y2) + DMAX;

   if (x < xa || x > xb || y < ya || y > yb)
      return INT_MAX;

   // Distance à un point:
   if (x1 == x2 && y1 == y2)
      return sqr(x -x1) + SQR(y - y1) - RFREE * RFREE;

   double k = - double((x1 - x2) * (x - x1) + (y - y1) * (y1 - y2)) /
              (SQR(x1 - x2) + SQR(y1 - y2));

   // On est en dehors de la bande et du coté du point (x1,y1)
   if (k <= 0) return sqr(x -x1) + sqr(y - y1);

   // On est en dehors de la bande et du coté du point (x2,y2)
   if (k >= 1) return sqr(x -x2) + sqr(y - y2);

   // On est dans la bande du segment, il faut calculer la distance à la droite
   return sqr(k*(x2 - x1) + x1 - x) + sqr(k*(y2 - y1) + y1 - y);
}
//******************************************************************************
void CPrecompileDart::pick(int AX, int AY, CView* AView)
{
   assert(AView != NULL);

   CDart* nearest = NULL;
   float bestDist = DMAX * DMAX;

   for (CDynamicCoverageAll it(FParameterGMapV->getMap()); it.cont(); ++it)
   {
      CDart* current = *it;

      CVertex& v1 = FParameterGMapV->getDrawingMap()->
	getBurstVertex(FParameterGMapV->getDartWithEmbedding(current));

      float seg1[3], seg2[3];

      AView->project(v1.getX(), v1.getY(), v1.getZ(), seg1);

      if (FParameterGMapV->getMap()->isFree0(current))
      {
         seg2[0] = seg1[0];
         seg2[1] = seg1[1];
      }
      else
      {
         CVertex e = FParameterGMapV->getDrawingMap()->
	   computeBurstExtremity(FParameterGMapV->getDartWithEmbedding(current));
         AView->project(e.getX(), e.getY(), e.getZ(), seg2);
      }

      float currentDist = distanceToSegment(AX, AY,
                                            seg1[0], seg1[1],
                                            seg2[0], seg2[1]);

      if (currentDist < bestDist)
      {
         nearest  = current;
         bestDist = currentDist;
      }
   }

   if (nearest != NULL)
      FParameterSelection->toggleOrbitSelection(nearest);
}
//******************************************************************************
