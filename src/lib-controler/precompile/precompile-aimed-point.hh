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
#ifndef PRECOMPILE_AIMED_POINT_HH
#define PRECOMPILE_AIMED_POINT_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */
//******************************************************************************
#include "precompile.hh"
#include "parameter-aimed-point.hh"
//******************************************************************************
  class CParameterAimedPosition;

  /** La classe CPrecompileAimedPoint permet l'affichage du point que l'on
   *  regarde.
   */
  class CPrecompileAimedPoint : public CPrecompile
  {
  public:
    //@name Constructeur et destructeur.
    //@{

    /** Constructeur
     *
     * @param AParameterAimedPoint Les paramètres de dessin du point que l'on
     *                             regarde
     * @param AParameterAimedPosition La position du point que l'on regarde
     */
    CPrecompileAimedPoint(CParameterAimedPoint*, CParameterAimedPosition*);
    CPrecompileAimedPoint(const CPrecompileAimedPoint &);
    CPrecompile* copy() const;

    /** Destructeur
     */
    virtual ~CPrecompileAimedPoint();
    //@}

    void        setParameter(CParameter* AParameter);
    CParameter* getParameter() const;

    /** getType
     *
     *  Retourne le type du precompile.
     */
    virtual TPrecompile getType() const;

    /** setAimedPosition
     *
     *  Modifie le paramètre PARAMETER_AIMED_POSITION associé au précompile.
     */
    void setAimedPosition(CParameterAimedPosition* APosition);

    /** setAimedPoint
     *
     *  Modifie le paramètre PARAMETER_AIMED_POINT associé au précompile.
     */
    void setAimedPoint(CParameterAimedPoint* APoint);

  protected:

    /** drawModel
     *
     *  Cette méthode affiche le point que l'on regarde.
     */
    virtual void drawModel();


  private:
    /// Les différents paramètres
    CParameterAimedPoint 	  * FParameterAimedPoint;
    CParameterAimedPosition * FParameterAimedPosition;
  };

//******************************************************************************
#endif // PRECOMPILE_AIMED_POINT_HH
//******************************************************************************
