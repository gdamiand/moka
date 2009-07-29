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
#include "precompile-preview.hh"
#include "parameter-preview.hh"
#include "parameter-object-position.hh"
#include "parameter-polyline-position.hh"
#include "parameter-creation.hh"
#include "transformation-matrix.hh"
#include "vertex.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CPrecompilePreview::CPrecompilePreview(CParameterPreview          * APreview,
				       CParameterObjectPosition   * APosition,
				       CParameterCreation         * AParams,
				       CParameterPolylinePosition * APolylinePosition) :
  FParameterPreview         (APreview),
  FParameterPosition        (APosition),
  FParameterCreation        (AParams),
  FParameterPolylinePosition(APolylinePosition)
{
  assert(FParameterPreview != NULL);
  assert(FParameterPosition != NULL);
  assert(FParameterCreation != NULL);
  assert(FParameterPolylinePosition != NULL);

  FParameterPreview->addPrecompileToUpdate(this);
  FParameterPosition->addPrecompileToUpdate(this);
  FParameterCreation->addPrecompileToUpdate(this);
  FParameterPolylinePosition->addPrecompileToUpdate(this);
  disable();
}
//******************************************************************************
CPrecompilePreview::CPrecompilePreview(const CPrecompilePreview & APrecompile) :
  CPrecompile               (APrecompile),
  FParameterPreview	    (static_cast<CParameterPreview*>
			     (APrecompile.FParameterPreview->copy())),
  FParameterPosition   	    (APrecompile.FParameterPosition),
  FParameterCreation   	    (APrecompile.FParameterCreation),
  FParameterPolylinePosition(APrecompile.FParameterPolylinePosition)
{
  assert(FParameterPreview != NULL);
  assert(FParameterPosition != NULL);
  assert(FParameterCreation != NULL);
  assert(FParameterPolylinePosition != NULL);

  FParameterPreview->addPrecompileToUpdate(this);
  FParameterPosition->addPrecompileToUpdate(this);
  FParameterCreation->addPrecompileToUpdate(this);
  FParameterPolylinePosition->addPrecompileToUpdate(this);
  disable();
}
//******************************************************************************
CPrecompilePreview::~CPrecompilePreview()
{
  FParameterPreview->removePrecompileToUpdate(this);
  FParameterPosition->removePrecompileToUpdate(this);
  FParameterCreation->removePrecompileToUpdate(this);
  FParameterPolylinePosition->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompilePreview::copy() const
{ return new CPrecompilePreview(*this); }
//******************************************************************************
void CPrecompilePreview::setParameter(CParameter * AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_PREVIEW:
      setPreview(static_cast<CParameterPreview *>(AParameter));
      break;
    case PARAMETER_OBJECT_POSITION:
      setPosition(static_cast<CParameterObjectPosition *>(AParameter));
      break;
    case PARAMETER_CREATION:
      setCreation(static_cast<CParameterCreation *>(AParameter));
      break;
    case PARAMETER_POLYLINE_POSITION:
      setPolylinePosition(static_cast<CParameterPolylinePosition *>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompilePreview::getParameter() const
{ return FParameterPreview; }
//******************************************************************************
void CPrecompilePreview::setPreview(CParameterPreview * APreview)
{
  assert(APreview != NULL);
  APreview->addPrecompileToUpdate(this);
  FParameterPreview->removePrecompileToUpdate(this);
  FParameterPreview = APreview;
  setToUpdate();
}
//******************************************************************************
void CPrecompilePreview::setPosition(CParameterObjectPosition * APosition)
{
  assert(APosition != NULL);
  APosition->addPrecompileToUpdate(this);
  FParameterPosition->removePrecompileToUpdate(this);
  FParameterPosition = APosition;
  setToUpdate();
}
//******************************************************************************
void CPrecompilePreview::setCreation(CParameterCreation * ACreation)
{
  assert(ACreation != NULL);
  ACreation->addPrecompileToUpdate(this);
  FParameterCreation->removePrecompileToUpdate(this);
  FParameterCreation = ACreation;
  setToUpdate();
}
//******************************************************************************
void CPrecompilePreview::setPolylinePosition(CParameterPolylinePosition *
					     APolylinePosition)
{
  assert(APolylinePosition != NULL);
  APolylinePosition->addPrecompileToUpdate(this);
  FParameterPolylinePosition->removePrecompileToUpdate(this);
  FParameterPolylinePosition = APolylinePosition;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompilePreview::getType() const
{ return PRECOMPILE_PREVIEW; }
//******************************************************************************
void CPrecompilePreview::setCurrentMode(TMode AMode)
{
  if ( AMode!=FCurrentMode )
    {
      FCurrentMode = AMode;
      setToUpdate();
    }
}
//******************************************************************************
void CPrecompilePreview::drawModel()
{
  switch(FCurrentMode)
    {
    case MODE_CREATION_CYLINDER: drawCylinder(); break;
    case MODE_CREATION_MESH    : drawMesh(); 	 break;
    case MODE_CREATION_POLYGON : drawPolygon();  break;
    case MODE_CREATION_POLYLINE: drawPolyline(); break;
    case MODE_CREATION_PYRAMID : drawPyramid();  break;
    case MODE_CREATION_SPHERE  : drawSphere(); 	 break;
    case MODE_CREATION_TORUS   : drawTorus(); 	 break;
    }
}
//******************************************************************************
void CPrecompilePreview::drawCylinder()
{
  // Paramètres :
  int M = FParameterCreation->getCylinderNbMeridians();
  int P = FParameterCreation->getCylinderNbParallels();
  int i,j;

  // Matrice de transformation :
  CTransformationMatrix globalMatrix =
    FParameterPosition->getTransformationMatrix();

  // Calcul des coordonnées des sommets :
  CVertex** V;
  V = new CVertex*[P+1];
  for (i=0; i<=P; ++i)
    V[i] = new CVertex[M];

  CVertex vertex((OX-OZ) / 2);
  CTransformationMatrix rotationMatrix(CTransformationMatrix::IdentityMatrix);
  rotationMatrix.rotate(OZ, 360.0/M);

  for (i=0; i<M; ++i)
    {
      for (j=0; j<=P; ++j)
	{
	  V[j][i] = vertex;
	  globalMatrix.applyOn(V[j][i]);
	  if (j<P)
	    vertex += OZ/P;
	}

      vertex -= OZ;
      rotationMatrix.applyOn(vertex);
    }

  // Initialisations OpenGL :
  glLineWidth(FParameterPreview->getLWPreview());
  glColor3fv(FParameterPreview->getCLPreview());

  // Méridiens :
  glBegin(GL_LINES);

  for (i=0; i<M; ++i)
    LINE(&V[0][i],&V[P][i]);

  glEnd();

  // Parallèles :
  for (j=0; j<=P; ++j)
    {
      glBegin(GL_LINE_LOOP);

      for (i=0; i<M; ++i)
	PLOT(&V[j][i]);

      glEnd();
    }

  // Faces absentes :
  TCoordinate x[2],y[2];
	
  for (i=0; i<2; ++i)
    {
      int k0 = (1+4*i)*M/8;

      TCoordinate x1= dCos(360.0/M* k0   );
      TCoordinate y1= dSin(360.0/M* k0   );
      TCoordinate x2= dCos(360.0/M*(k0+1));
      TCoordinate y2= dSin(360.0/M*(k0+1));

      TCoordinate t = (x1-y1) / (x1-x2+y2-y1);
		
      x[i] = (x1 + t*(x2-x1)) / 2;
      y[i] = (y1 + t*(y2-y1)) / 2;
    }

  // Initialisations OpenGL :
  glColor3fv(FParameterPreview->getCLPreviewBad());
  glBegin(GL_LINES);

  CVertex v0, v1;

  if (! FParameterCreation->getCylinderClosedUp())
    {
      v0.setXYZ(x[0], y[0],-0.5); globalMatrix.applyOn(v0);
      v1.setXYZ(x[1], y[1],-0.5); globalMatrix.applyOn(v1); LINE(& v0, & v1);
      v0.setXYZ(x[0],-y[0],-0.5); globalMatrix.applyOn(v0);
      v1.setXYZ(x[1],-y[1],-0.5); globalMatrix.applyOn(v1); LINE(& v0, & v1);
    }

  if (! FParameterCreation->getCylinderClosedDown())
    {
      v0.setXYZ(x[0], y[0], 0.5); globalMatrix.applyOn(v0);
      v1.setXYZ(x[1], y[1], 0.5); globalMatrix.applyOn(v1); LINE(& v0, & v1);
      v0.setXYZ(x[0],-y[0], 0.5); globalMatrix.applyOn(v0);
      v1.setXYZ(x[1],-y[1], 0.5); globalMatrix.applyOn(v1); LINE(& v0, & v1);
    }

  glEnd();

  for (i=0; i<=P; ++i)
    delete [] V[i];

  delete [] V;
}
//******************************************************************************
void CPrecompilePreview::drawMesh()
{
  // Paramètres :
  int mesh = FParameterCreation->getMeshSubdivisionDimension();

  int sx = FParameterCreation->getMeshNbSubdivisionsX();
  int sy = FParameterCreation->getMeshNbSubdivisionsY();
  int sz = FParameterCreation->getMeshNbSubdivisionsZ();
  unsigned char F = FParameterCreation->getMeshCreatedFaces();

  // Matrice de transformation :
  CTransformationMatrix globalMatrix = FParameterPosition->getTransformationMatrix();
	
  // Initialisations OpenGL :
  glLineWidth(FParameterPreview->getLWPreview());
  glColor3fv(FParameterPreview->getCLPreview());

  glBegin(GL_LINES);

  switch (FParameterCreation->getMeshDimension())
    {
    case 1:
      {
	/* ARETE */
	drawMesh1(globalMatrix, -OX/2, OX, mesh<1 ? 1 : sx);
	break;
      }
    case 2:
      {
	/* RECTANGLE */
	switch (mesh)
	  {
	  case 0:
	    drawMesh1(globalMatrix, (-OX-OY)/2, OX, 1);
	    drawMesh1(globalMatrix, (+OX-OY)/2, OY, 1);
	    drawMesh1(globalMatrix, (-OX+OY)/2, OX, 1);
	    drawMesh1(globalMatrix, (-OX-OY)/2, OY, 1);
	    break;

	  case 1:
	    drawMesh1(globalMatrix, (-OX-OY)/2, OX, sx);
	    drawMesh1(globalMatrix, (+OX-OY)/2, OY, sy);
	    drawMesh1(globalMatrix, (-OX+OY)/2, OX, sx);
	    drawMesh1(globalMatrix, (-OX-OY)/2, OY, sy);
	    break;

	  case 2:
	    drawMesh2(globalMatrix, -(OX+OY)/2, OX, sx, OY, sy);
	    break;
	  }
	break;
      }
    case 3:
      {
	/* CUBE */
	switch (mesh)
	  {
	  case 0:
	    sx = sy = sz = 1;

	  case 1:
	    if ((F & CUBE_X1) || (F & CUBE_Y1))
	      drawMesh1(globalMatrix, (-OX-OY-OZ)/2, OZ, sz);
	    if ((F & CUBE_X1) || (F & CUBE_Y2))
	      drawMesh1(globalMatrix, (-OX+OY-OZ)/2, OZ, sz);
	    if ((F & CUBE_X2) || (F & CUBE_Y1))
	      drawMesh1(globalMatrix, (+OX-OY-OZ)/2, OZ, sz);
	    if ((F & CUBE_X2) || (F & CUBE_Y2))
	      drawMesh1(globalMatrix, (+OX+OY-OZ)/2, OZ, sz);

	    if ((F & CUBE_X1) || (F & CUBE_Z1))
	      drawMesh1(globalMatrix, (-OX-OY-OZ)/2, OY, sy);
	    if ((F & CUBE_X1) || (F & CUBE_Z2))
	      drawMesh1(globalMatrix, (-OX-OY+OZ)/2, OY, sy);
	    if ((F & CUBE_X2) || (F & CUBE_Z1))
	      drawMesh1(globalMatrix, (+OX-OY-OZ)/2, OY, sy);
	    if ((F & CUBE_X2) || (F & CUBE_Z2))
	      drawMesh1(globalMatrix, (+OX-OY+OZ)/2, OY, sy);

	    if ((F & CUBE_Y1) || (F & CUBE_Z1))
	      drawMesh1(globalMatrix, (-OX-OY-OZ)/2, OX, sx);
	    if ((F & CUBE_Y1) || (F & CUBE_Z2))
	      drawMesh1(globalMatrix, (-OX-OY+OZ)/2, OX, sx);
	    if ((F & CUBE_Y2) || (F & CUBE_Z1))
	      drawMesh1(globalMatrix, (-OX+OY-OZ)/2, OX, sx);
	    if ((F & CUBE_Y2) || (F & CUBE_Z2))
	      drawMesh1(globalMatrix, (-OX+OY+OZ)/2, OX, sx);		
	    break;
		
	  case 2:
	    if (F & CUBE_X1)
	      drawMesh2(globalMatrix, (-OX-OY-OZ)/2, OY, sy, OZ, sz);
	    if (F & CUBE_X2)
	      drawMesh2(globalMatrix, (+OX-OY-OZ)/2, OY, sy, OZ, sz);

	    if (F & CUBE_Y1)
	      drawMesh2(globalMatrix, (-OX-OY-OZ)/2, OX, sx, OZ, sz);
	    if (F & CUBE_Y2)
	      drawMesh2(globalMatrix, (-OX+OY-OZ)/2, OX, sx, OZ, sz);

	    if (F & CUBE_Z1)
	      drawMesh2(globalMatrix, (-OX-OY-OZ)/2, OX, sx, OY, sy);
	    if (F & CUBE_Z2)
	      drawMesh2(globalMatrix, (-OX-OY+OZ)/2, OX, sx, OY, sy);
	    break;

	  case 3:
	    drawMesh3(globalMatrix, -(OX+OY+OZ)/2, OX, sx, OY, sy, OZ, sz);
	    break;
	  }
	
	glEnd();
	
	// Faces ignorées :
	if (mesh<3)
	  {
	    CVertex v[2][2][2];
		
	    v[0][0][0] = (-OX-OY-OZ)/2; v[1][0][0]= (+OX-OY-OZ)/2;
	    v[0][0][1] = (-OX-OY+OZ)/2; v[1][0][1]= (+OX-OY+OZ)/2;
	    v[0][1][0] = (-OX+OY-OZ)/2; v[1][1][0]= (+OX+OY-OZ)/2;
	    v[0][1][1] = (-OX+OY+OZ)/2; v[1][1][1]= (+OX+OY+OZ)/2;
	
	    for (int i=0; i<8; ++i)
	      globalMatrix.applyOn(v[i/4][(i/2)%2][i%2]);

	    // Initialisations OpenGL :
	    glColor3fv(FParameterPreview->getCLPreviewBad());
	    glBegin(GL_LINES);

	    if (mesh == 1)
	      sx = sy = sz = 1;
		
	    if (sy==1 && sz==1 &&
		(!(F & CUBE_X1)) && ((F & CUBE_RING_X)==CUBE_RING_X))
	      {
		LINE(& v[0][0][0], & v[0][1][1]);
		LINE(& v[0][0][1], & v[0][1][0]);
	      }

	    if (sy==1 && sz==1 &&
		(!(F & CUBE_X2)) && ((F & CUBE_RING_X)==CUBE_RING_X))
	      {
		LINE(& v[1][0][0], & v[1][1][1]);
		LINE(& v[1][0][1], & v[1][1][0]);
	      }

	    if (sx==1 && sz==1 &&
		(!(F & CUBE_Y1)) && ((F & CUBE_RING_Y)==CUBE_RING_Y))
	      {
		LINE(& v[0][0][0], & v[1][0][1]);
		LINE(& v[0][0][1], & v[1][0][0]);
	      }

	    if (sx==1 && sz==1 &&
		(!(F & CUBE_Y2)) && ((F & CUBE_RING_Y)==CUBE_RING_Y))
	      {
		LINE(& v[0][1][0], & v[1][1][1]);
		LINE(& v[0][1][1], & v[1][1][0]);
	      }

	    if (sx==1 && sy==1 &&
		(!(F & CUBE_Z1)) && ((F & CUBE_RING_Z)==CUBE_RING_Z))
	      {
		LINE(& v[0][0][0], & v[1][1][0]);
		LINE(& v[0][1][0], & v[1][0][0]);
	      }

	    if (sx==1 && sy==1 &&
		(!(F & CUBE_Z2)) && ((F & CUBE_RING_Z)==CUBE_RING_Z))
	      {
		LINE(& v[0][0][1], & v[1][1][1]);
		LINE(& v[0][1][1], & v[1][0][1]);
	      }

	    glEnd();
	  }
	break;
      }
    }
  glEnd();
}
//******************************************************************************
void CPrecompilePreview::drawPolygon()
{
  // Paramètres :
  int n = FParameterCreation->getPolygonNbEdges();

  // Matrice de transformation :
  CTransformationMatrix globalMatrix = FParameterPosition->getTransformationMatrix();

  // Initialisations OpenGL :
  glLineWidth(FParameterPreview->getLWPreview());
  glColor3fv(FParameterPreview->getCLPreview());

  glBegin(GL_LINE_LOOP);
  {
    CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);
    matrix.rotate(OZ, 360.0/n);

    CVertex x(OX/2);
    for (int i=0; i<n; ++i)
      {
	CVertex V(x);
	globalMatrix.applyOn(V);
	PLOT(& V);
	matrix.applyOn(x);
      }
  }
  glEnd();

  // Initialisations OpenGL :
  glColor3fv(FParameterPreview->getCLPreviewBad());

  // Boîte englobante :
  glBegin(GL_LINE_LOOP);
  {
    CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);
    matrix.rotate(OZ, 90.0);

    CVertex x((OX-OY)/2);
    for (int i=0; i<4; ++i)
      {
	CVertex V(x);
	globalMatrix.applyOn(V);
	PLOT(& V);
	matrix.applyOn(x);
      }
  }
  glEnd();
}
//******************************************************************************
void CPrecompilePreview::drawPolyline()
{
  if ( FParameterPolylinePosition->getNbVertices()==0 )
    return;

  // Initialisations OpenGL :
  glColor3fv(FParameterPreview->getCLPreview());

  glPointSize(FParameterPreview->getLWPreviewPoint());
  glBegin(GL_POINTS);
  PLOT(&FParameterPolylinePosition->getLastVertex());
  glEnd();

  glLineWidth(FParameterPreview->getLWPreview());
  glBegin(GL_LINE_STRIP);

  for (unsigned int i=0; i<FParameterPolylinePosition->getNbVertices(); ++i)
    PLOT(&FParameterPolylinePosition->getVertex(i));

  glEnd();
}
//******************************************************************************
void CPrecompilePreview::drawPyramid()
{
  // Paramètres :
  int M = FParameterCreation->getPyramidNbMeridians();
  int P = FParameterCreation->getPyramidNbParallels();
  int i,j;

  // Matrice de transformation :
  CTransformationMatrix globalMatrix = FParameterPosition->getTransformationMatrix();

  // Calcul des coordonnées des sommets :
  CVertex top(OZ/2); globalMatrix.applyOn(top);
  CVertex** V = new CVertex* [P];
  for (i=0; i<P; ++i)
    V[i] = new CVertex[M];

  CTransformationMatrix rotationMatrix(CTransformationMatrix::IdentityMatrix);
  rotationMatrix.rotate(OZ, 360.0/M);

  for (j=0; j<P; ++j)
    {
      CVertex vertex = (0.5*(P-j)/P)*OX + (((float) j)/P-0.5)*OZ;

      for (i=0; i<M; ++i)
	{
	  V[j][i] = vertex;
	  globalMatrix.applyOn(V[j][i]);
	  rotationMatrix.applyOn(vertex);
	}

      vertex += OZ / (P+1);
    }

  // Initialisations OpenGL :
  glLineWidth(FParameterPreview->getLWPreview());
  glColor3fv(FParameterPreview->getCLPreview());

  // Meridiens :
  glBegin(GL_LINES);

  for (i=0; i<M; ++i)
    LINE(&top,&V[0][i]);

  // Parallèles :
  for (j=0; j<P; j++)
    for (i=0; i<M; ++i)
      LINE(& V[j][i], & V[j][(i+1)%M]);

  glEnd();

  // Base ignorée :
  TCoordinate x[2],y[2];
	
  for (i=0; i<2; ++i)
    {
      int k0 = (1+4*i)*M/8;

      TCoordinate x1 = dCos(360.0/M* k0   )/2;
      TCoordinate y1 = dSin(360.0/M* k0   )/2;
      TCoordinate x2 = dCos(360.0/M*(k0+1))/2;
      TCoordinate y2 = dSin(360.0/M*(k0+1))/2;

      TCoordinate t = (x1-y1) / (x1-x2+y2-y1);
		
      x[i] = x1 + t*(x2-x1);
      y[i] = y1 + t*(y2-y1);
    }

  // Initialisations OpenGL :
  glColor3fv(FParameterPreview->getCLPreviewBad());
  glBegin(GL_LINES);
	
  if (! FParameterCreation->getPyramidClosed())
    {
      CVertex v0, v1;

      v0.setXYZ(x[0], y[0],-0.5); globalMatrix.applyOn(v0);
      v1.setXYZ(x[1], y[1],-0.5); globalMatrix.applyOn(v1); LINE(& v0, & v1);
      v0.setXYZ(x[0],-y[0],-0.5); globalMatrix.applyOn(v0);
      v1.setXYZ(x[1],-y[1],-0.5); globalMatrix.applyOn(v1); LINE(& v0, & v1);
    }

  glEnd();

  for (i=0; i<P; ++i)
    delete [] V[i];
  delete [] V;
}
//******************************************************************************
void CPrecompilePreview::drawSphere()
{
  // Paramètres :
  int M = FParameterCreation->getSphereNbMeridians();
  int P = FParameterCreation->getSphereNbParallels();
  int i,j;

  // Matrice de transformation :
  CTransformationMatrix globalMatrix = FParameterPosition->getTransformationMatrix();

  // Calcul des coordonnées :
  CVertex south= -OZ/2; globalMatrix.applyOn(south);
  CVertex north= +OZ/2; globalMatrix.applyOn(north);
  CVertex v0,v1;

  CVertex** grid = new CVertex*[P];
  for (i=0; i<P; ++i)
    grid[i] = new CVertex[M];

  CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);
  matrix.rotate(OZ, 360.0 / M);

  for (j=0; j<P; ++j)
    {
      TCoordinate alpha = 180.0*(j+1)/(P+1) - 90.0;

      CVertex v = dCos(alpha) / 2 * OX + dSin(alpha) / 2 * OZ;

      for (i=0; i<M; ++i)
	{
	  grid[j][i]= v;
	  globalMatrix.applyOn(grid[j][i]);
	  matrix.applyOn(v);
	}
    }

  // Initialisations OpenGL :
  glLineWidth(FParameterPreview->getLWPreview());
  glColor3fv(FParameterPreview->getCLPreview());

  // Méridiens:
  for (i=0; i<M; i++)
    {
      glBegin(GL_LINE_STRIP);
      PLOT(& south);

      for (j=0; j<P; j++)
	PLOT(& grid[j][i]);

      PLOT(& north);
      glEnd();
    }

  // Parallèles:
  for (j=0; j<P; j++)
    {
      glBegin(GL_LINE_LOOP);

      for (i=0; i<M; i++)
	PLOT(& grid[j][i]);

      glEnd();
    }

  for (i=0; i<P; ++i)
    delete [] grid[i];
  delete [] grid;
}
//******************************************************************************
void CPrecompilePreview::drawTorus()
{
  // Paramètres :
  int M = FParameterCreation->getTorusNbMeridians();
  int P = FParameterCreation->getTorusNbParallels();
  float k = FParameterCreation->getTorusRadiusRapport();
  int i,j;

  // Matrice de transformation :
  CTransformationMatrix globalMatrix = FParameterPosition->getTransformationMatrix();

  // Calcul des coordonnées :
  float _R_ = 1/(1+k)/2;
  float  R_ = k/(1+k)/2;

  // Coordonnées des sommets :
  CTransformationMatrix matrix1(CTransformationMatrix::IdentityMatrix);
  CTransformationMatrix matrix2(CTransformationMatrix::IdentityMatrix);
  CTransformationMatrix matrix3(CTransformationMatrix::IdentityMatrix);

  matrix1.rotate(OY, 360.0 / P);
  matrix2.translate(_R_ * OX);
  matrix3.rotate(OZ, 360.0 / M);

  CVertex vertex1(R_ * OX);
  CVertex** grid = new CVertex*[P];
  for (i=0; i<P; ++i)
    grid[i] = new CVertex[M];

  for (j=0; j<P; ++j)
    {
      CVertex vertex2(vertex1);
      matrix2.applyOn(vertex2);

      for (i=0; i<M; ++i)
	{
	  grid[j][i] = vertex2;
	  globalMatrix.applyOn(grid[j][i]);
	  matrix3.applyOn(vertex2);
	}

      matrix1.applyOn(vertex1);
    }

  // Initialisations OpenGL :
  glLineWidth(FParameterPreview->getLWPreview());
  glColor3fv(FParameterPreview->getCLPreview());

  // Méridiens :
  for (i=0; i<M; i++)
    {
      glBegin(GL_LINE_LOOP);
		
      for (j=0; j<P; j++)
	PLOT(& grid[j][i]);
		
      glEnd();
    }

  // Parallèles :
  for (j=0; j<P; j++)
    {
      glBegin(GL_LINE_LOOP);

      for (i=0; i<M; i++)
	PLOT(& grid[j][i]);

      glEnd();
    }

  for (i=0; i<P; ++i)
    delete [] grid[i];
  delete [] grid;
}
//******************************************************************************
#define K (0.015)

void CPrecompilePreview::drawMesh1(const CTransformationMatrix & AMatrix,
				   const CVertex & AOrigin,
				   const CVertex & AVector, int ASx)
{
  CVertex v1, v2;

  if (ASx > 1)
    {
      CVertex current = AOrigin;
      CVertex dx = AVector / ASx;

      for (int i=0; i<=ASx; ++i, current += dx)
	{
	  v1 = current - K*OX; AMatrix.applyOn(v1);
	  v2 = current + K*OX; AMatrix.applyOn(v2); LINE(&v1,&v2);
	
	  v1 = current - K*OY; AMatrix.applyOn(v1);
	  v2 = current + K*OY; AMatrix.applyOn(v2); LINE(&v1,&v2);
	
	  v1 = current - K*OZ; AMatrix.applyOn(v1);
	  v2 = current + K*OZ; AMatrix.applyOn(v2); LINE(&v1,&v2);
	}
    }

  v1 = AOrigin          ; AMatrix.applyOn(v1);
  v2 = AOrigin + AVector; AMatrix.applyOn(v2); LINE(&v1,&v2);
}

#undef K
//******************************************************************************
void CPrecompilePreview::drawMesh2(const CTransformationMatrix & AMatrix,
				   const CVertex & AOrigin,
				   const CVertex & AVectorX, int ASx,
				   const CVertex & AVectorY, int ASy)
{
  CVertex v1, v2;

  CVertex dx = AVectorX / ASx;
  CVertex dy = AVectorY / ASy;

  CVertex current;

  current = AOrigin;
  for (int i=0; i<=ASx; ++i, current += dx)
    {
      v1 = current           ; AMatrix.applyOn(v1);
      v2 = current + AVectorY; AMatrix.applyOn(v2); LINE(& v1, & v2);
    }

  current = AOrigin;
  for (int j=0; j<=ASy; ++j, current += dy)
    {
      v1 = current           ; AMatrix.applyOn(v1);
      v2 = current + AVectorX; AMatrix.applyOn(v2); LINE(& v1, & v2);
    }
}
//******************************************************************************
void CPrecompilePreview::drawMesh3(const CTransformationMatrix & AMatrix,
				   const CVertex & AOrigin,
				   const CVertex & AVectorX, int ASx,
				   const CVertex & AVectorY, int ASy,
				   const CVertex & AVectorZ, int ASz)
{
  CVertex v1, v2;

  CVertex dx = AVectorX / ASx;
  CVertex dy = AVectorY / ASy;
  CVertex dz = AVectorZ / ASz;

  CVertex currentX, currentY, currentZ;
  int i,j,k;

  currentX = AOrigin;
  for (i=0; i<=ASx; ++i, currentX += dx)
    {
      currentY = currentX;

      for (j=0; j<=ASy; ++j, currentY += dy)
	{
	  v1 = currentY           ; AMatrix.applyOn(v1);
	  v2 = currentY + AVectorZ; AMatrix.applyOn(v2); LINE(& v1, & v2);
	}
    }

  currentX = AOrigin;
  for (i=0; i<=ASx; ++i, currentX += dx)
    {
      currentZ = currentX;

      for (k=0; k<=ASz; ++k, currentZ += dz)
	{
	  v1 = currentZ           ; AMatrix.applyOn(v1);
	  v2 = currentZ + AVectorY; AMatrix.applyOn(v2); LINE(& v1, & v2);
	}
    }

  currentY = AOrigin;
  for (j=0; j<=ASy; ++j, currentY += dy)
    {
      currentZ = currentY;

      for (k=0; k<=ASz; ++k, currentZ += dz)
	{
	  v1 = currentZ           ; AMatrix.applyOn(v1);
	  v2 = currentZ + AVectorX; AMatrix.applyOn(v2); LINE(& v1, & v2);
	}
    }
}
//******************************************************************************
