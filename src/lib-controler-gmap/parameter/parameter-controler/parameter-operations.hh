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
#ifndef PARAMETER_OPERATIONS_HH
#define PARAMETER_OPERATIONS_HH
//******************************************************************************
#include "controler-gmap-types.hh"
#include "parameter.hh"
#include "vertex.hh"
#include "transformation-matrix.hh"
//******************************************************************************
namespace GMap3d
{
  class CParameterOperations : public CParameter
  {
  public:
    //@{
    CParameterOperations(int ANbRef = 0);
    CParameterOperations(const CParameterOperations &);
    virtual ~CParameterOperations();
    virtual CParameter * copy() const;
    //@}

    //@{

    virtual void save(std::ostream &);
    virtual void load(std::istream &);
    virtual void reinit();

    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /*@name Paramètres de plaquage
     */
    //@{
    void setRotateCells(bool ABool);
    bool getRotateCells() const;

    void setScaleCells(bool ABool);
    bool getScaleCells() const;

    void setTranslateCells(bool ABool);
    bool getTranslateCells() const;
    //@}

    /*@name Paramètres de maillage
     */
    //@{
    int getMeshNbSubdivisionsX() const;
    int getMeshNbSubdivisionsY() const;
    int getMeshNbSubdivisionsZ() const;

    void setMeshNbSubdivisionsX(int ASx);
    void setMeshNbSubdivisionsY(int ASy);
    void setMeshNbSubdivisionsZ(int ASz);

    int  getMeshSubdivisionDimensionFrom() const;
    void setMeshSubdivisionDimensionFrom(int ADim);

    int  getMeshSubdivisionDimensionTo  () const;
    void setMeshSubdivisionDimensionTo  (int ADim);

    bool getInterpolationMerges() const;
    void setInterpolationMerges(bool ABool);

    bool getInterpolationSews() const;
    void setInterpolationSews(bool ABool);

    bool isActiveInterpolationSews() const;
    //@}

    /*@name Paramètres de lissage
     */
    //@{
    bool getSmoothMerges() const;
    void setSmoothMerges(bool ABool);

    bool getSmoothSews() const;
    void setSmoothSews(bool ABool);

    TSmoothType getSmoothMethod() const;
    void        setSmoothMethod(TSmoothType AValue);
    const CTransformationMatrix & getMeshMatrix() const;

    bool isActiveSmoothMerges() const;
    bool isActiveSmoothSews  () const;
    //@}

    /*@name Paramètres d'extrusion
     */
    //@{
    float getExtrusionCoef() const;
    void  setExtrusionCoef(float ACoef);

    float getExtrusionRevolutionAngle() const;
    void  setExtrusionRevolutionAngle(float AAngle);

    bool getExtrusionAdjacentSews() const;
    void setExtrusionAdjacentSews(bool ABool);

    int  getExtrusionInitialPositionMode() const;
    void setExtrusionInitialPositionMode(int AValue);

    int  getExtrusionInitialDirectionMode() const;
    void setExtrusionInitialDirectionMode(int AValue);

    bool getExtrusionDirection() const;
    void setExtrusionDirection(bool ABool);

    bool getExtrusionScale() const;
    void setExtrusionScale(bool ABool);

    bool getExtrusionPonderateSection() const;
    void setExtrusionPonderateSection(bool ABool);

    int  getExtrusionRevolutionNbEdges() const;
    void setExtrusionRevolutionNbEdges(int AValue);
    //@}

    /*@name Paramètres d'arrondi
     */
    //@{
    float getDefaultRoundingCoefForVertices() const;
    void  setDefaultRoundingCoefForVertices(float ACoef);

    float getDefaultRoundingCoefForEdges() const;
    void  setDefaultRoundingCoefForEdges(float ACoef);

    float getDefaultRoundingCoefForFaces() const;
    void  setDefaultRoundingCoefForFaces(float ACoef);
    //@}
  
  private:
    // Paramètres de plaquage :
    bool FRotateCells;
    bool FScaleCells;
    bool FTranslateCells;

    // Paramètres de maillage :
    int  FMeshNbSubdivisions[3];
    bool FInterpolationMerges;
    bool FInterpolationSews  ;
  
    // Paramètres de lissage :
    bool        FSmoothMerges;
    bool        FSmoothSews;
    TSmoothType FSmoothMethod;
  
    // Paramètres d'extrusion :
    float FExtrusionCoef;
    float FExtrusionRevolutionAngle;
    bool  FExtrusionAdjacentSews;
    int   FExtrusionInitialPositionMode;
    int   FExtrusionInitialDirectionMode;
    bool  FExtrusionDirection;
    bool  FExtrusionScale;
    bool  FExtrusionPonderateSection;
    int   FExtrusionRevolutionNbEdges;

    // Paramètres de chanfreinage :
    float FRoundingDefaultCoefs[3];
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_OPERATIONS_HH
//******************************************************************************
