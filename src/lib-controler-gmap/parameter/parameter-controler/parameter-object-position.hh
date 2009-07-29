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
#ifndef PARAMETER_OBJECT_POSITION_HH
#define PARAMETER_OBJECT_POSITION_HH
//******************************************************************************
#include "parameter.hh"
#include "vertex.hh"
#include "transformation-matrix.hh"
//******************************************************************************
namespace GMap3d
{
  class CParameterObjectPosition : public CParameter
  {
  public:
    //@{
    CParameterObjectPosition(bool ANeedRotation, int ANbRef = 0);
    CParameterObjectPosition(const CParameterObjectPosition &);
    virtual ~CParameterObjectPosition();
    virtual CParameter * copy() const;
    //@}

    //@{

    virtual void save(std::ostream &);
    virtual void load(std::istream &);
    virtual void reinit();

    //@}

    //@{

    float getScale() const;

    void setScale(float AScale);

    float getProportionX() const;
    float getProportionY() const;
    float getProportionZ() const;
    CVertex getProportions() const;

    void setProportionX(float AX);
    void setProportionY(float AY);
    void setProportionZ(float AZ);
    void setProportions(float AX, float AY, float AZ);
    void setProportions(const CVertex & AValues);

    float	getDimensionX() const;
    float	getDimensionY() const;
    float	getDimensionZ() const;
    CVertex getDimensions() const;

    void setDimensionX(float AX);
    void setDimensionY(float AY);
    void setDimensionZ(float AZ);
    void setDimensions(float AX, float AY, float AZ);
    void setDimensions(const CVertex & AValues);

    float	getCenterX() const;
    float	getCenterY() const;
    float	getCenterZ() const;
    CVertex getCenter() const;

    void setCenterX(float AX);
    void setCenterY(float AY);
    void setCenterZ(float AZ);
    void setCenter(float AX, float AY, float AZ);
    void setCenter(const CVertex & AVertex);

    float	getRotationX() const;
    float	getRotationY() const;
    float	getRotationZ() const;
    CVertex getRotations() const;

    void setRotationX(float AAlpha);
    void setRotationY(float ABeta);
    void setRotationZ(float AGamma);
    void setRotations(float AAlpha, float ABeta, float AGamma);
    void setRotations(const CVertex & AAngles);

    float	getFirstVertexX() const;
    float	getFirstVertexY() const;
    float	getFirstVertexZ() const;
    CVertex getFirstVertex() const;

    void setFirstVertexX(float AX, bool AKeepNormalVector = false);
    void setFirstVertexY(float AY, bool AKeepNormalVector = false);
    void setFirstVertexZ(float AZ, bool AKeepNormalVector = false);
    void setFirstVertex(float AX, float AY, float AZ,
			bool AKeepNormalVector = false);
    void setFirstVertex(const CVertex & AVertex,
			bool AKeepNormalVector = false);

    float	getNormalVectorX() const;
    float	getNormalVectorY() const;
    float	getNormalVectorZ() const;
    CVertex getNormalVector() const;

    void setNormalVectorX(float AX);
    void setNormalVectorY(float AY);
    void setNormalVectorZ(float AZ);
    void setNormalVector(float AX, float AY, float AZ);
    void setNormalVector(const CVertex & AVector);

    //@}

    /// Récupération de la matrice de transformation :
    CTransformationMatrix getTransformationMatrix() const;

    /// Identificateur du type de paramètre.
    virtual int getType() const;

  private:
    // Origine du repère (centre de l'objet) :
    CVertex FCenter;

    // Facteur d'agrandissement de l'objet :
    float FScale;

    // Proportions de l'objet (déformation dans les trois dimensions) :
    CVertex FProportions;

    // Dimensions de l'objet (FDimensions[i] = FScale * FProportions[i])
    CVertex FDimensions;

    // Position du sommet correspondant au sommet (1,0,0) dans le repère :
    CVertex FFirstVertex;

    // Vecteur correspondant au vecteur (0,0,1) dans le repère :
    CVertex FNormalVector;

    // Rotations appliquées sur les axes :
    CVertex FRotations;

    // Rotation de 90° (utilisé pour sphère, pyramide et cylindre)
    bool FNeedRotation;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_OBJECT_POSITION_HH
//******************************************************************************
