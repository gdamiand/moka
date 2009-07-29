/*
 * lib-controler : Un contrôleur générique de scène 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler
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
#include "view-precompile.hh"
#include "precompile.hh"
#include "precompile-aimed-point.hh"
#include "parameter-eye-position.hh"
#include "parameter-aimed-position.hh"
#include "parameter-drawing.hh"
using namespace std;
//******************************************************************************
CViewPrecompile::CViewPrecompile(CParameterEyePosition  * AEyePosition,
                                 CParameterAimedPosition* AAimedPosition,
                                 CParameterDrawing  * ADrawing) :
      FParameterEyePosition(AEyePosition),
      FParameterAimedPosition(AAimedPosition),
      FParameterDrawing(ADrawing)
{
   assert(FParameterEyePosition != NULL);
   assert(FParameterAimedPosition != NULL);
   assert(FParameterDrawing != NULL);

   FParameterEyePosition->incNbRef();
   FParameterAimedPosition->incNbRef();
   FParameterDrawing->incNbRef();
}
//******************************************************************************
CViewPrecompile::~CViewPrecompile()
{
   FParameterEyePosition->decNbRef();
   FParameterAimedPosition->decNbRef();
   FParameterDrawing->decNbRef();
}
//******************************************************************************
void CViewPrecompile::initScene()
{
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glClearColor(FParameterDrawing->getCLBackground(0),
                FParameterDrawing->getCLBackground(1),
                FParameterDrawing->getCLBackground(2),
                1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//******************************************************************************
void CViewPrecompile::drawScene()
{
   if (isEnabled())
   {
      // Positionnement dans la scène
      setScenePosition();

      // Affichage du contenu de la scène
      display();
   }
}
//******************************************************************************
void CViewPrecompile::unproject(float Ax, float Ay, float ARes[])
{
   GLdouble xp, yp, zp;

   // Récupération du point de mire :
   float xm = FParameterAimedPosition->getLookAt(0);
   float ym = FParameterAimedPosition->getLookAt(1);
   float zm = FParameterAimedPosition->getLookAt(2);

   // Projection du point de mire :
   GLdouble dummy, zMire;
   gluProject(xm, ym, zm,
              FModelViewMatrix, FProjectionMatrix, FViewport,
              & dummy, & dummy, & zMire);

   // "Déprojection" du point (Ax,Ay,zMire) :
   gluUnProject(Ax, Ay, zMire,
                FModelViewMatrix, FProjectionMatrix, FViewport,
                & xp, & yp, & zp);

   // Écriture du résultat :
   ARes[0] = xp;
   ARes[1] = yp;
   ARes[2] = zp;
}
//******************************************************************************
void CViewPrecompile::project(float Ax, float Ay, float Az, float ARes[])
{
   GLdouble x, y, z;

   // Projection du point (x,y,z) :
   gluProject(Ax, Ay, Az, FModelViewMatrix, FProjectionMatrix, FViewport,
              &x, &y, &z);

   // Écriture du résultat :
   ARes[0] = x;
   ARes[1] = y;
   ARes[2] = z;
}
//******************************************************************************
CVertex CViewPrecompile::getLookAt()
{
   return CVertex(FParameterAimedPosition->getLookAt(0),
                  FParameterAimedPosition->getLookAt(1),
                  FParameterAimedPosition->getLookAt(2));
}
//******************************************************************************
CVertex CViewPrecompile::getEyeDirection()
{
   GLdouble xp, yp, zp;

   // Récupération du point de mire :
   float xm = FParameterAimedPosition->getLookAt(0);
   float ym = FParameterAimedPosition->getLookAt(1);
   float zm = FParameterAimedPosition->getLookAt(2);

   // Projection du point de mire :
   GLdouble xMire, yMire, zMire;
   gluProject(xm, ym, zm,
              FModelViewMatrix, FProjectionMatrix, FViewport,
              & xMire, & yMire, & zMire);

   // "Déprojection" du point (xMire,yMire,zMire+10) :
   gluUnProject(xMire, yMire, zMire + 10,
                FModelViewMatrix, FProjectionMatrix, FViewport,
                & xp, & yp, & zp);

   // Écriture du résultat :
   CVertex vect(xm - xp, ym - yp, zm - zp);

   assert(! vect.isNull());

   return vect / vect.norm();
}
//******************************************************************************
void CViewPrecompile::update()
{
   list<CPrecompile*>::iterator it = FListPrecompile.begin();

   while (it != FListPrecompile.end())
   {
      (*it)->update();
      ++it;
   }
}
//******************************************************************************
void CViewPrecompile::display()
{
   if (isEnabled())
   {
      list<CPrecompile*>::iterator it = FListPrecompile.begin();

      while (it != FListPrecompile.end())
      {
         (*it)->draw();
         ++it;
      }
   }
}
//******************************************************************************
CPrecompile* CViewPrecompile::findPrecompile(TPrecompile APrecompileType)
{
   list<CPrecompile*>::iterator it = FListPrecompile.begin();

   while (it != FListPrecompile.end())
   {
      if ((*it)->getType() == APrecompileType)
         return *it;

      ++it;
   }

   return NULL;
}
//******************************************************************************
void CViewPrecompile::addPrecompile(CPrecompile* APrecompile)
{
   assert(APrecompile != NULL);
   assert(findPrecompile(APrecompile->getType()) == NULL);

   FListPrecompile.push_back(APrecompile);

   APrecompile->incNbView();
   if (isEnabled()) APrecompile->incNbEnabledView();
}
//******************************************************************************
CPrecompile* CViewPrecompile::removePrecompile(TPrecompile APrecompileType)
{
   CPrecompile* precompile = findPrecompile(APrecompileType);

   if (precompile != NULL)
   {
      FListPrecompile.remove(precompile);
      if (isEnabled()) precompile->decNbEnabledView();

      // ATTENTION: A faire en dernier car peut entraîner la destruction du
      // précompile
      precompile->decNbView();
   }

   return precompile;
}
//******************************************************************************
CPrecompile* CViewPrecompile::removePrecompile(CPrecompile* APrecompile)
{
   assert(APrecompile != NULL);

   return removePrecompile(APrecompile->getType());
}
//******************************************************************************
CParameterEyePosition & CViewPrecompile::getEyePosition() const
{ return *FParameterEyePosition; }
//******************************************************************************
CParameterAimedPosition & CViewPrecompile::getAimedPosition() const
{ return *FParameterAimedPosition; }
//******************************************************************************
void CViewPrecompile::setScenePositionInitialize()
{
   glGetIntegerv(GL_VIEWPORT, FViewport);
   FRatio = (1.0 * FViewport[2]) / FViewport[3];

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
}
//******************************************************************************
void CViewPrecompile::setScenePositionFinalize()
{
   glGetDoublev(GL_PROJECTION_MATRIX, FProjectionMatrix);
   glGetDoublev(GL_MODELVIEW_MATRIX,  FModelViewMatrix);
}
//******************************************************************************
CParameter* CViewPrecompile::getParameter(TParameter AParameterType) const
{
   switch (AParameterType)
   {
      case PARAMETER_EYE_POSITION  : return FParameterEyePosition;
      case PARAMETER_AIMED_POSITION: return FParameterAimedPosition;
      case PARAMETER_DRAWING       : return FParameterDrawing;
   }

   return NULL;
}
//******************************************************************************
CParameterEyePosition* CViewPrecompile::getParameterEyePosition() const
{ return FParameterEyePosition; }
//******************************************************************************
CParameterAimedPosition* CViewPrecompile::getParameterAimedPosition() const
{ return FParameterAimedPosition; }
//******************************************************************************
CParameterDrawing* CViewPrecompile::getParameterDrawing() const
{ return FParameterDrawing; }
//******************************************************************************
void CViewPrecompile::setParameter(CParameter* AParameter)
{
   switch (AParameter->getType())
   {
      case PARAMETER_EYE_POSITION:
         setParameterEyePosition(static_cast<CParameterEyePosition*>
                                 (AParameter));
         break;
      case PARAMETER_AIMED_POSITION:
         setParameterAimedPosition(static_cast<CParameterAimedPosition*>
                                   (AParameter));
         break;
      case PARAMETER_DRAWING:
         setParameterDrawing(static_cast<CParameterDrawing*>(AParameter));
         break;
   }
}
//******************************************************************************
void CViewPrecompile::groupParameter(CViewPrecompile* AView,
                                     TParameter AParameterType)
{
   assert(AView != NULL);

   CParameter* parameter = getParameter(AParameterType);

   if (parameter != NULL) AView->setParameter(parameter);
}
//******************************************************************************
void CViewPrecompile::ungroupParameter(TParameter AParameterType)
{
   CParameter* parameter = getParameter(AParameterType);

   if (parameter != NULL && parameter->getNbRef() > 1)
      setParameter(parameter->copy());
}
//******************************************************************************
void CViewPrecompile::setParameterEyePosition(CParameterEyePosition* AParam)
{
   assert(AParam != NULL);

   if (AParam != FParameterEyePosition)
   {
      AParam->incNbRef();
      FParameterEyePosition->decNbRef();
      FParameterEyePosition = AParam;

      for (list<CPrecompile*>::iterator it = FListPrecompile.begin();
            it != FListPrecompile.end(); ++it)
         (*it)->setParameter(AParam);
   }
}
//******************************************************************************
void CViewPrecompile::setParameterAimedPosition(CParameterAimedPosition*
      AParam)
{
   assert(AParam != NULL);

   if (AParam != FParameterAimedPosition)
   {
      AParam->incNbRef();
      FParameterAimedPosition->decNbRef();
      FParameterAimedPosition = AParam;

      for (list<CPrecompile*>::iterator it = FListPrecompile.begin();
            it != FListPrecompile.end(); ++it)
         (*it)->setParameter(AParam);
   }
}
//******************************************************************************
void CViewPrecompile::setParameterDrawing(CParameterDrawing* AParam)
{
   assert(AParam != NULL);

   if (AParam != FParameterDrawing)
   {
      AParam->incNbRef();
      FParameterDrawing->decNbRef();
      FParameterDrawing = AParam;

      for (list<CPrecompile*>::iterator it = FListPrecompile.begin();
            it != FListPrecompile.end(); ++it)
         (*it)->setParameter(AParam);
   }
}
//******************************************************************************
void CViewPrecompile::enable()
{
   CView::enable();
   for (list<CPrecompile*>::iterator it = FListPrecompile.begin();
         it != FListPrecompile.end(); ++it)
      (*it)->incNbEnabledView();
}
//******************************************************************************
void CViewPrecompile::disable()
{
   CView::disable();
   for (list<CPrecompile*>::iterator it = FListPrecompile.begin();
         it != FListPrecompile.end(); ++it)
      (*it)->decNbEnabledView();
}
//******************************************************************************
void CViewPrecompile::pick(int AX, int AY)
{
   for (list<CPrecompile*>::iterator it = FListPrecompile.begin();
         it != FListPrecompile.end(); ++it)
      (*it)->pick(AX, AY, this);
}
//******************************************************************************
