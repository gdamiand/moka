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
#include "parameter-creation.hh"
#include "vertex.hh"
#include <cassert>
using namespace std;
using namespace GMap3d;
//******************************************************************************
#define DEFAULT_SPHERE_NB_MERIDIANS   (12)
#define DEFAULT_SPHERE_NB_PARALLELS   ( 6)

#define DEFAULT_CYLINDER_NB_MERIDIANS (10)
#define DEFAULT_CYLINDER_NB_PARALLELS ( 4)

#define DEFAULT_PYRAMID_NB_MERIDIANS  ( 8)
#define DEFAULT_PYRAMID_NB_PARALLELS  ( 8)

#define DEFAULT_TORUS_NB_MERIDIANS    (12)
#define DEFAULT_TORUS_NB_PARALLELS    ( 6)
//******************************************************************************
CParameterCreation::CParameterCreation(int ANbRef) :
  CParameter		   (ANbRef),
  FPolygonNbEdges	   (6),
  FMeshDimension	   (3),
  FMeshSubdivisionDimension(0),
  FCreatedFaces		   (CUBE_ALL),
  FCylinderNbMeridians	   (DEFAULT_CYLINDER_NB_MERIDIANS),
  FCylinderNbParallels	   (DEFAULT_CYLINDER_NB_PARALLELS),
  FCylinderClosedUp	   (true),
  FCylinderClosedDown	   (true),
  FPyramidNbMeridians	   (DEFAULT_PYRAMID_NB_MERIDIANS),
  FPyramidNbParallels	   (DEFAULT_PYRAMID_NB_PARALLELS),
  FPyramidClosed	   (true),
  FSphereNbMeridians	   (DEFAULT_SPHERE_NB_MERIDIANS),
  FSphereNbParallels	   (DEFAULT_SPHERE_NB_PARALLELS),
  FTorusNbMeridians	   (DEFAULT_TORUS_NB_MERIDIANS),
  FTorusNbParallels	   (DEFAULT_TORUS_NB_PARALLELS),
  FTorusRadiusRapport	   (0.5)
{
  // Maillages :
  FMeshNbSubdivisions[0] = 3;
  FMeshNbSubdivisions[1] = 3;
  FMeshNbSubdivisions[2] = 3;
}
//******************************************************************************
CParameterCreation::CParameterCreation(const CParameterCreation & AParam) :
  CParameter               (AParam),
  FPolygonNbEdges	   (AParam.FPolygonNbEdges),
  FMeshDimension	   (AParam.FMeshDimension),
  FMeshSubdivisionDimension(AParam.FMeshSubdivisionDimension),
  FCreatedFaces		   (AParam.FCreatedFaces),
  FCylinderNbMeridians	   (AParam.FCylinderNbMeridians),
  FCylinderNbParallels	   (AParam.FCylinderNbParallels),
  FCylinderClosedUp	   (AParam.FCylinderClosedUp),
  FCylinderClosedDown	   (AParam.FCylinderClosedDown),
  FPyramidNbMeridians	   (AParam.FPyramidNbMeridians),
  FPyramidNbParallels	   (AParam.FPyramidNbParallels),
  FPyramidClosed	   (AParam.FPyramidClosed),
  FSphereNbMeridians	   (AParam.FSphereNbMeridians),
  FSphereNbParallels	   (AParam.FSphereNbParallels),
  FTorusNbMeridians	   (AParam.FTorusNbMeridians),
  FTorusNbParallels	   (AParam.FTorusNbParallels),
  FTorusRadiusRapport	   (AParam.FTorusRadiusRapport)
{
  FMeshNbSubdivisions[0] = AParam.FMeshNbSubdivisions[0];
  FMeshNbSubdivisions[1] = AParam.FMeshNbSubdivisions[1];
  FMeshNbSubdivisions[2] = AParam.FMeshNbSubdivisions[2];
}
//******************************************************************************
CParameterCreation::~CParameterCreation()
{}
//******************************************************************************
CParameter * CParameterCreation::copy() const
{ return new CParameterCreation(*this); }
//******************************************************************************
int CParameterCreation::getPolygonNbEdges() const
{ return FPolygonNbEdges; }
//******************************************************************************
void CParameterCreation::setPolygonNbEdges(int ANbEdges)
{
  assert(ANbEdges >= 2);

  if ( FPolygonNbEdges!=ANbEdges )
    {
      putAllNeedToUpdate();
      FPolygonNbEdges= ANbEdges;
    }
}
//******************************************************************************
int CParameterCreation::getMeshNbSubdivisionsX() const
{
  return FMeshNbSubdivisions[0];
}
//******************************************************************************
int CParameterCreation::getMeshNbSubdivisionsY() const
{
  return FMeshNbSubdivisions[1];
}
//******************************************************************************
int CParameterCreation::getMeshNbSubdivisionsZ() const
{
  return FMeshNbSubdivisions[2];
}
//******************************************************************************
void CParameterCreation::setMeshNbSubdivisionsX(int ASx)
{
  assert(ASx > 0);
  if ( FMeshNbSubdivisions[0]!=ASx )
    {
      putAllNeedToUpdate();
      FMeshNbSubdivisions[0]= ASx;
    }
}
//******************************************************************************
void CParameterCreation::setMeshNbSubdivisionsY(int ASy)
{
  assert(ASy > 0);
  if ( FMeshNbSubdivisions[1]!=ASy )
    {
      putAllNeedToUpdate();
      FMeshNbSubdivisions[1]= ASy;
    }
}
//******************************************************************************
void CParameterCreation::setMeshNbSubdivisionsZ(int ASz)
{
  assert(ASz > 0);
  if ( FMeshNbSubdivisions[2]!=ASz )
    {
      putAllNeedToUpdate();
      FMeshNbSubdivisions[2] = ASz;
    }
}
//******************************************************************************
int CParameterCreation::getMeshDimension() const
{
  return FMeshDimension;
}
//******************************************************************************
void CParameterCreation::setMeshDimension(int ADimension)
{
  assert(1 <= ADimension && ADimension <= 3);

  if ( FMeshDimension!=ADimension )
    {
      putAllNeedToUpdate();
      FMeshDimension= ADimension;

      if (FMeshDimension < FMeshSubdivisionDimension)
	FMeshSubdivisionDimension= FMeshDimension;
    }
}
//******************************************************************************
int CParameterCreation::getMeshSubdivisionDimension() const
{
  return FMeshSubdivisionDimension;
}
//******************************************************************************
void CParameterCreation::setMeshSubdivisionDimension(int ADimension)
{
  assert(0 <= ADimension && ADimension <= 3);

  if ( FMeshSubdivisionDimension!=ADimension )
    {
      putAllNeedToUpdate();
      FMeshSubdivisionDimension= ADimension;

      if (FMeshDimension < FMeshSubdivisionDimension)
	FMeshDimension= FMeshSubdivisionDimension;
    }
}
//******************************************************************************
bool CParameterCreation::getMeshCreatedFacesActive() const
{
  return FMeshSubdivisionDimension < 3;
}
//******************************************************************************
unsigned char CParameterCreation::getMeshCreatedFaces() const
{
  return FCreatedFaces;
}
//******************************************************************************
bool CParameterCreation::getMeshCreatedFace(unsigned char AFace) const
{
  assert(AFace == CUBE_X1 || AFace == CUBE_X2 ||
	 AFace == CUBE_Y1 || AFace == CUBE_Y2 ||
	 AFace == CUBE_Z1 || AFace == CUBE_Z2);

  return FCreatedFaces & AFace;
}
//******************************************************************************
void CParameterCreation::setMeshCreatedFaces(unsigned char AFaces)
{
  assert(AFaces <= CUBE_ALL);
  if ( FCreatedFaces!=AFaces )
    {
      putAllNeedToUpdate();
      FCreatedFaces= AFaces;
    }
}
//******************************************************************************
void CParameterCreation::setMeshCreatedFace(unsigned char AFace, bool AValue)
{
  assert(AFace == CUBE_X1 || AFace == CUBE_X2 ||
	 AFace == CUBE_Y1 || AFace == CUBE_Y2 ||
	 AFace == CUBE_Z1 || AFace == CUBE_Z2);

  putAllNeedToUpdate();
  if (AValue)
    FCreatedFaces |= AFace;
  else
    FCreatedFaces &= ~ AFace;
}
//******************************************************************************
int CParameterCreation::getCylinderNbMeridians() const
{
  return FCylinderNbMeridians;
}
//******************************************************************************
void CParameterCreation::setCylinderNbMeridians(int ANb)
{
  assert(ANb > 0);
  if ( FCylinderNbMeridians!=ANb )
    {
      putAllNeedToUpdate();
      FCylinderNbMeridians = ANb;
    }
}
//******************************************************************************
int CParameterCreation::getCylinderNbParallels() const
{
  return FCylinderNbParallels;
}
//******************************************************************************
void CParameterCreation::setCylinderNbParallels(int ANb)
{
  assert(ANb > 0);
  if ( FCylinderNbParallels!=ANb )
    {
      putAllNeedToUpdate();
      FCylinderNbParallels= ANb;
    }
}
//******************************************************************************
bool CParameterCreation::getCylinderClosedUp() const
{
  return FCylinderClosedUp;
}
//******************************************************************************
void CParameterCreation::setCylinderClosedUp(bool AClosed)
{
  if ( FCylinderClosedUp!= AClosed )
    {
      putAllNeedToUpdate();
      FCylinderClosedUp= AClosed;
    }
}
//******************************************************************************
bool CParameterCreation::getCylinderClosedDown() const
{
  return FCylinderClosedDown;
}
//******************************************************************************
void CParameterCreation::setCylinderClosedDown(bool AClosed)
{
  if ( FCylinderClosedDown!=AClosed )
    {
      putAllNeedToUpdate();
      FCylinderClosedDown= AClosed;
    }
}
//******************************************************************************
int CParameterCreation::getPyramidNbMeridians() const
{
  return FPyramidNbMeridians;
}
//******************************************************************************
void CParameterCreation::setPyramidNbMeridians(int ANb)
{
  assert(ANb > 0);
  if ( FPyramidNbMeridians!=ANb )
    {
      putAllNeedToUpdate();
      FPyramidNbMeridians= ANb;
    }
}
//******************************************************************************
int CParameterCreation::getPyramidNbParallels() const
{
  return FPyramidNbParallels;
}
//******************************************************************************
void CParameterCreation::setPyramidNbParallels(int ANb)
{
  assert(ANb > 0);
  if ( FPyramidNbParallels!=ANb )
    {
      putAllNeedToUpdate();
      FPyramidNbParallels= ANb;
    }
}
//******************************************************************************
bool CParameterCreation::getPyramidClosed() const
{
  return FPyramidClosed;
}
//******************************************************************************
void CParameterCreation::setPyramidClosed(bool AClosed)
{
  if ( FPyramidClosed!=AClosed )
    {
      putAllNeedToUpdate();
      FPyramidClosed= AClosed;
    }
}
//******************************************************************************
int CParameterCreation::getSphereNbMeridians() const
{
  return FSphereNbMeridians;
}
//******************************************************************************
void CParameterCreation::setSphereNbMeridians(int ANb)
{
  assert(ANb > 0);
  if ( FSphereNbMeridians!=ANb )
    {
      putAllNeedToUpdate();
      FSphereNbMeridians= ANb;
    }
}
//******************************************************************************
int CParameterCreation::getSphereNbParallels() const
{
  return FSphereNbParallels;
}
//******************************************************************************
void CParameterCreation::setSphereNbParallels(int ANb)
{
  assert(ANb > 0);
  if ( FSphereNbParallels!=ANb )
    {
      putAllNeedToUpdate();
      FSphereNbParallels= ANb;
    }
}
//******************************************************************************
int CParameterCreation::getTorusNbMeridians() const
{
  return FTorusNbMeridians;
}
//******************************************************************************
void CParameterCreation::setTorusNbMeridians(int ANb)
{
  assert(ANb > 0);
  if ( FTorusNbMeridians!=ANb )
    {
      putAllNeedToUpdate();
      FTorusNbMeridians= ANb;
    }
}
//******************************************************************************
int CParameterCreation::getTorusNbParallels() const
{
  return FTorusNbParallels;
}
//******************************************************************************
void CParameterCreation::setTorusNbParallels(int ANb)
{
  assert(ANb > 0);
  if ( FTorusNbParallels!=ANb )
    {
      putAllNeedToUpdate();
      FTorusNbParallels= ANb;
    }
}
//******************************************************************************
float CParameterCreation::getTorusRadiusRapport() const
{
  return FTorusRadiusRapport;
}
//******************************************************************************
void CParameterCreation::setTorusRadiusRapport(float AValue)
{
  assert(0.0 <= AValue && AValue <= 1.0);
  if ( FTorusRadiusRapport!=AValue )
    {
      putAllNeedToUpdate();
      FTorusRadiusRapport= AValue;
    }
}
//******************************************************************************
void CParameterCreation::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterCreation::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterCreation::reinit()
{}
//******************************************************************************
int CParameterCreation::getType() const
{ return  PARAMETER_CREATION; }
//******************************************************************************
