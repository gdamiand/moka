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
#include "precompile-face.hh"
#include "parameter-gmap-vertex.hh"
#include "parameter-face.hh"
#include <cassert>
#include <cstdlib>
using namespace GMap3d;
//******************************************************************************
CPrecompileFace::CPrecompileFace(CParameterGMapVertex * AParameterGMapV,
				 CParameterFace       * AParameterFace ) :
  FParameterGMapV(AParameterGMapV),
  FParameterFace (AParameterFace),
  FTess(NULL)
{
  assert(FParameterGMapV!=NULL && FParameterFace!=NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterFace->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileFace::CPrecompileFace(const CPrecompileFace & APrecompile) :
  CPrecompile    (APrecompile),
  FParameterGMapV(APrecompile.FParameterGMapV),
  FParameterFace (static_cast<CParameterFace*>
		  (APrecompile.FParameterFace->copy())),
  FTess(NULL)
{
  assert(FParameterGMapV!=NULL && FParameterFace!=NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterFace->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileFace::~CPrecompileFace()
{
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterFace->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompileFace::copy() const
{ return new CPrecompileFace(*this); }
//******************************************************************************
void CPrecompileFace::setParameter(CParameter * AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_GMAP_VERTEX:
      setGMapVertex(static_cast<CParameterGMapVertex *>(AParameter));
      break;
    case PARAMETER_FACE:
      setFace(static_cast<CParameterFace *>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompileFace::getParameter() const
{ return FParameterFace; }
//******************************************************************************
void CPrecompileFace::setGMapVertex(CParameterGMapVertex* AParameterGMapV)
{
  assert(AParameterGMapV != NULL);
  AParameterGMapV->addPrecompileToUpdate(this);
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterGMapV = AParameterGMapV;
  setToUpdate();
}
//******************************************************************************
void CPrecompileFace::setFace(CParameterFace * AFace)
{
  assert(AFace != NULL);
  AFace->addPrecompileToUpdate(this);
  FParameterFace->removePrecompileToUpdate(this);
  FParameterFace = AFace;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileFace::getType() const
{ return PRECOMPILE_FACE; }
//******************************************************************************
// monCombine :
//   Fonction nécessaire si on veut afficher les faces auto-intersectantes.
//   Sans cette fonction, ces faces sont affichées en fil de fer.
static void monCombine(GLdouble c[3], void *d[4], GLfloat w[4], void **out)
{
  GLdouble *nv = (GLdouble *) malloc(sizeof(GLdouble)*3);

  nv[0] = c[0];

  nv[1] = c[1];

  nv[2] = c[2];

  *out = nv;
}

/* TODO use openGL to draw the darts ?
static void edgeFlagDataCB(GLboolean flag, void* lpContext)
{ glEdgeFlag(flag); }
  
gluTessCallback(FTess, GLU_TESS_EDGE_FLAG_DATA,
(GLvoid (WINAPI *) ( )) & edgeFlagDataCB);
*/

//******************************************************************************
void CPrecompileFace::drawModel()
{
  if (FParameterFace->getBLFace()<1)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
  
  bool bursted1 = ! areEqual(FParameterGMapV->getDrawingMap()->getBurstCoef(1), 1.0);
  int treated   = FParameterGMapV->getDrawingMap()->getNewMark();

#ifdef _WINDOWS
#define WINAPI __stdcall
#else
#define WINAPI
#endif
  
  FTess = gluNewTess();
  gluTessCallback(FTess, GLU_TESS_BEGIN,
		  (GLvoid (WINAPI *) ( )) & glBegin    );
  gluTessCallback(FTess, GLU_TESS_END,
		  (GLvoid (WINAPI *) ( )) & glEnd      );
  gluTessCallback(FTess, GLU_TESS_VERTEX,
		  (GLvoid (WINAPI *) ( )) & glVertex3dv);
  gluTessCallback(FTess, GLU_TESS_COMBINE,
		  (GLvoid (WINAPI *) ( )) & monCombine );

   // Couleur globale :
  if ( !FParameterFace->getRandomCLFace  () &&
       !FParameterFace->getRandomCLVolume() )
    glColor4f(FParameterFace->getCLFace(0),
	      FParameterFace->getCLFace(1),
	      FParameterFace->getCLFace(2),
	      FParameterFace->getBLFace());

  for (CDynamicCoverageAll it(FParameterGMapV->getDrawingMap()); it.cont(); ++it)
    if (! FParameterGMapV->getDrawingMap()->isMarked(*it, treated))
      {
	if ( FParameterFace->getRandomCLVolume() )
	  setRandomColor(*it, 3);

	for (CDynamicCoverage012 it2(FParameterGMapV->getDrawingMap(), *it);
	     it2.cont(); ++it2)
	  if (! FParameterGMapV->getDrawingMap()->isMarked(*it2, treated))
	    {
	      if (FParameterGMapV->getDrawingMap()->isClosedPolyline(*it2))
		// && FParameterGMapV->getMap()->isPlanarPolyline(*it)
		{
		  int nbSommetsFace = FParameterGMapV->getDrawingMap()->
		    getNbPolylineVertices(*it2);
		  if (nbSommetsFace > 2)
		    {
			if ( FParameterFace->getRandomCLFace() )
			  setRandomColor(*it2	, 2);

		      drawFilledFace(*it2, nbSommetsFace, bursted1);
		    }
		}
	      FParameterGMapV->getDrawingMap()->markOrbit(*it2, ORBIT_01, treated);
	    }
      }

  FParameterGMapV->getDrawingMap()->negateMaskMark(treated);
  FParameterGMapV->getDrawingMap()->freeMark(treated);
  gluDeleteTess(FTess);
  
  glDisable(GL_BLEND);
}
//------------------------------------------------------------------------------
void CPrecompileFace::setRandomColor(CDart* ADart, int ADimension)
{
  assert( ADimension!=0 );
  
  float r,v,b;

  if ( FParameterFace->getRandomColorGeometry() )
    {
      CVertex bary = FParameterGMapV->getMap()->
	barycenter(ADart, ORBIT_CELL[ADimension]);
      //fabs(v)-floor(fabs(v))
      r = bary.getX();
      v = bary.getY();
      b = bary.getZ();
#undef FLT
    }
  else
    { 
      long int c = 0;
      
      CCoverage* cov = FParameterGMapV->getMap()->
	getDynamicCoverage(ADart, ORBIT_CELL[ADimension]);
      
      for ( ; cov->cont(); ++ *cov)
	c += (long int) ** cov;
      
      delete cov;

      srand(c);
      r = double(rand())/RAND_MAX;
      v = double(rand())/RAND_MAX;
      b = double(rand())/RAND_MAX;      

      //std::cout<<"c="<<c<<"  "<<"r,v,b="<<r<<", "<<v<<", "<<b<<std::endl;
    }

  glColor4f(r, v, b, FParameterFace->getBLFace());
}
//------------------------------------------------------------------------------
void CPrecompileFace::drawFilledFace(CDart* ADart, int ANbSommets,
				     bool ABursted)
{
  gluTessBeginPolygon(FTess, NULL);
  gluTessBeginContour(FTess);
  
  GLdouble* data = new GLdouble[(ABursted ? 6 : 3) * (ANbSommets+1)];
  
  CVertex pred;
  int numeroSommet = 0;

  // Parcours de la face
  for (CDynamicCoverage01 cov(FParameterGMapV->getDrawingMap(), ADart);
       cov.cont(); ++cov)
    {
      // Récupérer les coordonnées associées au brins
      CVertex & v = FParameterGMapV->getDrawingMap()->getBurstVertex(* cov);
      
      if (cov.prevOperationType() == OP_NONE || pred != v)
	{
	  data[  (numeroSommet*3)] = v.getX();
	  data[1+(numeroSommet*3)] = v.getY();
	  data[2+(numeroSommet*3)] = v.getZ();

 	  gluTessVertex(FTess, &data[numeroSommet*3], &data[numeroSommet*3]);
	  ++ numeroSommet;
	  pred = v;
	}
    }

  gluTessEndContour(FTess);
  gluTessEndPolygon(FTess);

  delete [] data;
}
//******************************************************************************
