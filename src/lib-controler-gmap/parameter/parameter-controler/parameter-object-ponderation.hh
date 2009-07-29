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
#ifndef PARAMETER_OBJECT_PONDERATION_HH
#define PARAMETER_OBJECT_PONDERATION_HH
//******************************************************************************
#include "controler-gmap-types.hh"
#include "parameter.hh"
#include "vertex.hh"
#include "transformation-matrix.hh"
//******************************************************************************
namespace GMap3d
{
  class CParameterObjectPonderation : public CParameter
  {
  public:
    //@{
    CParameterObjectPonderation(int ANbRef = 0);
    CParameterObjectPonderation(const CParameterObjectPonderation &);
    virtual ~CParameterObjectPonderation();
    virtual CParameter * copy() const;
    //@}

    //@{

    virtual void save(std::ostream&);
    virtual void load(std::istream&);
    virtual void reinit();
  
    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /*@name Paramètres de pondération :
     */
    //@{

    void             setPonderationType(TPonderationType APonderationType);
    TPonderationType getPonderationType() const;

    /// Centre de pondération :
    void    setPonderationCenter(const CVertex & APonderationCenter);
    CVertex getPonderationCenter() const;

    /* Vecteur directeur de l'axe de pondération, ou vecteur normal du plan
     * de pondération (inutilisé pour la pondération centrale) :
     */
    void    setPonderationVector(const CVertex & APonderationVector);
    CVertex getPonderationVector() const;

    /* Activation / désactivation de l'étalonnage en entrée (avant l'application
     * de la fonction de pondération :
     */
    void setPonderationStretchIn(bool AActive);
    bool getPonderationStretchIn() const;

    /// Définition des bornes d'étalonnage en entrée :
    void  setPonderationStretchInMin(float AMin);
    float getPonderationStretchInMin() const;

    void  setPonderationStretchInMax(float AMax);
    float getPonderationStretchInMax() const;

    /// Fonction de pondération :
    void setPonderationFunctionType(TFunctionType AFunctionType);
    TFunctionType getPonderationFunctionType() const;

    /* Activation / désactivation de l'étalonnage en sortie (après l'application
     * de la fonction de pondération :
     */
    void setPonderationStretchOut(bool AActive);
    bool getPonderationStretchOut() const;

    /// Définition des bornes d'étalonnage en sortie :
    void  setPonderationStretchOutMin(float AMin);
    float getPonderationStretchOutMin() const;

    void  setPonderationStretchOutMax(float AMax);
    float getPonderationStretchOutMax() const;

    //@}

  private:
    /// Paramètres de pondération :
    TPonderationType FPonderationType;
    CVertex FPonderationCenter;
    CVertex FPonderationVector;
    bool FPonderationStretchIn;
    float FPonderationStretchInMin, FPonderationStretchInMax;
    TFunctionType FPonderationFunctionType;
    bool FPonderationStretchOut;
    float FPonderationStretchOutMin, FPonderationStretchOutMax;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_OBJECT_PONDERATION_HH
//******************************************************************************
