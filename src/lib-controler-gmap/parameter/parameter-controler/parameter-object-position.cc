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
#include "parameter-object-position.hh"
#include "transformation-matrix.hh"
#include "geometry.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
/* Paramètres géométriques :
 *
 * C : Center
 * F : FirstVertex
 * N : Normal
 * R : Rotations
 * S : Scale
 * P : Proportions
 * D : Dimensions
 *
 * Dépendances : ("A -> B" signifie "quand on modifie A il mettre à jour B")
 *
 * C -> F
 * F -> NSPD
 * N -> FR
 * R -> NF
 * S -> FD
 * P -> DF
 * D -> F(P)  (P ssi D est différent du vecteur nul)
 */
//******************************************************************************
CParameterObjectPosition::CParameterObjectPosition(bool ANeedRotation,
						   int ANbRef) :
  CParameter   (ANbRef),
  FNeedRotation(ANeedRotation)
{
  reinit();
}
//******************************************************************************
CParameterObjectPosition::
CParameterObjectPosition(const CParameterObjectPosition & AParam) :
  CParameter   (AParam),
  FNeedRotation(AParam.FNeedRotation)
{
  setScale	(AParam.getScale());
  setProportions(AParam.getProportions());
  setCenter	(AParam.getCenter());
  setRotations	(AParam.getRotations());
}
//******************************************************************************
CParameterObjectPosition::~CParameterObjectPosition()
{}
//******************************************************************************
CParameter * CParameterObjectPosition::copy() const
{ return new CParameterObjectPosition(*this); }
//******************************************************************************
void CParameterObjectPosition::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectPosition::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectPosition::reinit()
{
  setScale	(1);
  setProportions(1, 1, 1);
  setCenter	(0, 0, 0);
  setRotations	(0, 0, 0);
}
//******************************************************************************
float CParameterObjectPosition::getScale() const
{ return FScale; }
//******************************************************************************
void CParameterObjectPosition::setScale(float AScale)
{
  if (FScale != AScale)
    {
      putAllNeedToUpdate();

      FScale = AScale;

      // Dimensions:
      FDimensions = 2 * FScale * FProportions;

      // First vertex:
      CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);
      matrix.rotate(FRotations);

      CVertex x(OX);
      matrix.applyOn(x);
      FFirstVertex = FCenter + (getDimensionX() / 2) * x;
    }
}
//******************************************************************************
float CParameterObjectPosition::getProportionX() const
{ return FProportions.getX(); }
float CParameterObjectPosition::getProportionY() const
{ return FProportions.getY(); }
float CParameterObjectPosition::getProportionZ() const
{ return FProportions.getZ(); }

CVertex CParameterObjectPosition::getProportions() const
{ return FProportions; }
//******************************************************************************
void CParameterObjectPosition::setProportionX(float AX)
{ setProportions(AX, FProportions.getY(), FProportions.getZ()); }
void CParameterObjectPosition::setProportionY(float AY)
{ setProportions(FProportions.getX(), AY, FProportions.getZ()); }
void CParameterObjectPosition::setProportionZ(float AZ)
{ setProportions(FProportions.getX(), FProportions.getY(), AZ); }

void CParameterObjectPosition::setProportions(float AX, float AY, float AZ)
{ setProportions(CVertex(AX, AY, AZ)); }

void CParameterObjectPosition::setProportions(const CVertex & AValues)
{
  if (FProportions != AValues)
    {
      putAllNeedToUpdate();

      FProportions = AValues;

      // Dimensions:
      FDimensions = 2 * getScale() * getProportions();

      // First vertex:
      CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);
      matrix.rotate(FRotations);
      CVertex x(OX);
      matrix.applyOn(x);
      FFirstVertex = FCenter + (getDimensionX() / 2) * x;
    }
}
//******************************************************************************
float CParameterObjectPosition::getDimensionX() const
{ return FDimensions.getX(); }
float CParameterObjectPosition::getDimensionY() const
{ return FDimensions.getY(); }
float CParameterObjectPosition::getDimensionZ() const
{ return FDimensions.getZ(); }

CVertex CParameterObjectPosition::getDimensions() const
{ return FDimensions; }
//******************************************************************************
void CParameterObjectPosition::setDimensionX(float AX)
{ setDimensions(AX, FDimensions.getY(), FDimensions.getZ()); }
void CParameterObjectPosition::setDimensionY(float AY)
{ setDimensions(FDimensions.getX(), AY, FDimensions.getZ()); }
void CParameterObjectPosition::setDimensionZ(float AZ)
{ setDimensions(FDimensions.getX(), FDimensions.getY(), AZ); }

void CParameterObjectPosition::setDimensions(float AX, float AY, float AZ)
{ setDimensions(CVertex(AX,AY,AZ)); }

void CParameterObjectPosition::setDimensions(const CVertex & AValues)
{
  if (FDimensions != AValues)
    {
      putAllNeedToUpdate();

      FDimensions = AValues;

      // Proportions:
      if (! FDimensions.isNull())
	FProportions = isZero(FScale) ? ORIGIN : FDimensions / 2 / getScale();

      // First vertex:
      CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);
      matrix.rotate(getRotations());
      CVertex x(OX);
      matrix.applyOn(x);
      FFirstVertex = FCenter + (getDimensionX() / 2) * x;
    }
}
//******************************************************************************
float CParameterObjectPosition::getCenterX() const
{ return FCenter.getX(); }
float CParameterObjectPosition::getCenterY() const
{ return FCenter.getY(); }
float CParameterObjectPosition::getCenterZ() const
{ return FCenter.getZ(); }

CVertex CParameterObjectPosition::getCenter() const
{ return FCenter; }
//******************************************************************************
void CParameterObjectPosition::setCenterX(float AX)
{ setCenter(AX, FCenter.getY(), FCenter.getZ()); }
void CParameterObjectPosition::setCenterY(float AY)
{ setCenter(FCenter.getX(), AY, FCenter.getZ()); }
void CParameterObjectPosition::setCenterZ(float AZ)
{ setCenter(FCenter.getX(), FCenter.getY(), AZ); }

void CParameterObjectPosition::setCenter(float AX, float AY, float AZ)
{ setCenter(CVertex(AX,AY,AZ)); }

void CParameterObjectPosition::setCenter(const CVertex & AVertex)
{
  if (FCenter != AVertex)
    {
      putAllNeedToUpdate();

      FCenter = AVertex;

      // First vertex:
      CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);
      matrix.rotate(getRotations());
      CVertex x(OX);
      matrix.applyOn(x);
      FFirstVertex = FCenter + (getDimensionX() / 2) * x;
    }
}
//******************************************************************************
float CParameterObjectPosition::getRotationX() const
{ return FRotations.getX(); }
float CParameterObjectPosition::getRotationY() const
{ return FRotations.getY(); }
float CParameterObjectPosition::getRotationZ() const
{ return FRotations.getZ(); }

CVertex CParameterObjectPosition::getRotations() const
{
  return FRotations;
}
//******************************************************************************
void CParameterObjectPosition::setRotationX(float AAlpha)
{ setRotations(AAlpha, FRotations.getY(), FRotations.getZ()); }
void CParameterObjectPosition::setRotationY(float ABeta)
{ setRotations(FRotations.getX(), ABeta, FRotations.getZ()); }
void CParameterObjectPosition::setRotationZ(float AGamma)
{ setRotations(FRotations.getX(), FRotations.getY(), AGamma); }

void CParameterObjectPosition::setRotations(float AAlpha,
					    float ABeta,
					    float AGamma)
{ setRotations(CVertex(AAlpha, ABeta, AGamma)); }

void CParameterObjectPosition::setRotations(const CVertex & AAngles)
{
  if (FRotations != AAngles)
    {
      putAllNeedToUpdate();

      FRotations = AAngles;

      CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);
      matrix.rotate(AAngles);

      // First vertex:
      CVertex x(OX);
      matrix.applyOn(x);
      FFirstVertex = FCenter + (getDimensionX() / 2) * x;

      // Normal vector:
      CVertex z(OZ);
      matrix.applyOn(z);
      FNormalVector = z;
    }
}
//******************************************************************************
float CParameterObjectPosition::getFirstVertexX() const
{ return FFirstVertex.getX(); }
float CParameterObjectPosition::getFirstVertexY() const
{ return FFirstVertex.getY(); }
float CParameterObjectPosition::getFirstVertexZ() const
{ return FFirstVertex.getZ(); }

CVertex CParameterObjectPosition::getFirstVertex() const
{ return FFirstVertex; }
//******************************************************************************
void CParameterObjectPosition::setFirstVertexX(float AX, bool AKeepNormalVector)
{
  setFirstVertex(AX, FFirstVertex.getY(), FFirstVertex.getZ(),
		 AKeepNormalVector);
}
void CParameterObjectPosition::setFirstVertexY(float AY, bool AKeepNormalVector)
{
  setFirstVertex(FFirstVertex.getX(), AY, FFirstVertex.getZ(),
		 AKeepNormalVector);
}
void CParameterObjectPosition::setFirstVertexZ(float AZ, bool AKeepNormalVector)
{
  setFirstVertex(FFirstVertex.getX(), FFirstVertex.getY(), AZ,
		 AKeepNormalVector);
}

void CParameterObjectPosition::setFirstVertex(float AX, float AY, float AZ,
					      bool AKeepNormalVector)
{ setFirstVertex(CVertex(AX,AY,AZ), AKeepNormalVector); }

void CParameterObjectPosition::setFirstVertex(const CVertex & AVertex,
					      bool AKeepNormalVector)
{
  if (FFirstVertex != AVertex)
    {
      putAllNeedToUpdate();

      FFirstVertex = AVertex;

      CVertex V(FFirstVertex - FCenter);

      // Proportions:
      if (isZero(FProportions.getX()))
	FProportions.setX(1);

      // Scale:
      FScale = V.norm() / FProportions.getX();

      // Dimensions:
      FDimensions = 2 * FScale * FProportions;

      // Normal vector: (ancien test : ! isZero(V.dot(FNormalVector)))
      if (! V.isNull() && ! AKeepNormalVector)
    	FNormalVector = V * (isZero(V.getY()) ? OY : OX);

      // Rotations:
      FRotations = CGeometry::getAngles(V, FNormalVector);
    }
}
//******************************************************************************
float CParameterObjectPosition::getNormalVectorX() const
{ return FNormalVector.getX(); }
float CParameterObjectPosition::getNormalVectorY() const
{ return FNormalVector.getY(); }
float CParameterObjectPosition::getNormalVectorZ() const
{ return FNormalVector.getZ(); }

CVertex CParameterObjectPosition::getNormalVector() const
{ return FNormalVector; }
//******************************************************************************
void CParameterObjectPosition::setNormalVectorX(float AX)
{ setNormalVector(AX, FNormalVector.getY(), FNormalVector.getZ()); }
void CParameterObjectPosition::setNormalVectorY(float AY)
{ setNormalVector(FNormalVector.getX(), AY, FNormalVector.getZ()); }
void CParameterObjectPosition::setNormalVectorZ(float AZ)
{ setNormalVector(FNormalVector.getX(), FNormalVector.getY(), AZ); }

void CParameterObjectPosition::setNormalVector(float AX, float AY, float AZ)
{ setNormalVector(CVertex(AX, AY, AZ)); }

void CParameterObjectPosition::setNormalVector(const CVertex & AVector)
{
  if (FNormalVector != AVector)
    {
      putAllNeedToUpdate();

      FNormalVector = AVector;

      // Rotations:
      FRotations = CGeometry::getAngles(FFirstVertex - FCenter, FNormalVector);

      // First vertex:
      CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);
      matrix.rotate(FRotations);
      CVertex x(OX);
      matrix.applyOn(x);
      FFirstVertex = FCenter + (getDimensionX() / 2) * x;
    }
}
//******************************************************************************
CTransformationMatrix CParameterObjectPosition::getTransformationMatrix() const
{
  CTransformationMatrix matrix(CTransformationMatrix::IdentityMatrix);

  if (FNeedRotation)
    matrix.rotate(OY, 90);

  matrix.scale(FDimensions);
  matrix.rotate(FRotations);
  matrix.translate(FCenter);

  return matrix;
}
//******************************************************************************
int CParameterObjectPosition::getType() const
{ return PARAMETER_OBJECT_POSITION; }
//******************************************************************************
