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
#ifdef MODULE_SPAMOD
//******************************************************************************
#ifndef PARAMETER_SPAMOD_HH
#define PARAMETER_SPAMOD_HH
//******************************************************************************
#include "parameter.hh"
#include "spamod-api.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterSpamod regroupe les paramètres nécessaires au dessin de
 *  listes du module Spamod.
 */

namespace GMap3d
{
  class CParameterSpamod: public CParameter
  {
  public:
    /** @name Constructeur et destructeur
     */

    //@{

    /** Constructeur
     */
    CParameterSpamod(int ANbRef = 0);
    CParameterSpamod(const CParameterSpamod &);
    CParameter * copy() const;

    /** Destructeur
     */
    virtual ~CParameterSpamod();

    //@}

    /** @name Sauvegarde et chargement des paramètres
     */

    //@{

    /** save
     *
     *  Méthode permettant de sauver les valeurs des variables membres.
     */
    virtual void save(std::ostream&);

    /** load
     *
     *  Méthode permettant de récupérer les valeurs des variables membres.
     */
    virtual void load(std::istream&);

    /** reinit
     *
     *    Reinitialise le paramètre.
     */
    virtual void reinit();

    //@}

    //@{

    /// Définition du mode de visualisation:
    void setViewMode(TSpamodViewMode AMode);
    TSpamodViewMode getViewMode() const;
    void setNextViewMode();

    /// Accessseurs aux paramètres de la vue euclidienne:
    bool getEuclidianParam(int AIndex);
    void setEuclidianParam(int AIndex, bool AValue);

    /// Accessseurs aux paramètres de la vue analytique:
    bool getAnalyticParam(int AIndex);
    void setAnalyticParam(int AIndex, bool AValue);

    /// Accessseurs aux paramètres de la vue voxel:
    bool getVoxelParam(int AIndex);
    void setVoxelParam(int AIndex, bool AValue);

    /// Accessseurs aux paramètres de la vue K2:
    bool getK2Param(int AIndex);
    void setK2Param(int AIndex, bool AValue);

    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /// Fonctions amies pour lire et écrire dans un flux.
    friend std::ostream& operator << (std::ostream&, const CParameterSpamod &);
    friend std::istream& operator >> (std::istream&, CParameterSpamod &);

  private:
    /// Le mode de visualisation :
    TSpamodViewMode FViewMode;

    /* Paramètres de la vue euclidienne:
     * FEuclidianParam[0]: affichage fil de fer,
     * FEuclidianParam[1]: affichage faces pleines.
     */
    bool FEuclidianParam[2];

    /* Paramètres de la vue analytique
     * FAnalyticParam[0]: affichage des sommets,
     * FAnalyticParam[1]: affichage des arêtes,
     * FAnalyticParam[2]: affichage des face,
     * FAnalyticParam[3]: transparence
     */
    bool FAnalyticParam[4];

    /* Paramètres de la vue voxel:
     * FVoxelParam[0]: affichage des sommets,
     * FVoxelParam[1]: affichage des arêtes,
     * FVoxelParam[2]: affichage des faces.
     */
    bool FVoxelParam[3];

    /* Paramètres de la vue K2:
     * FK2Param[0]: affichage des pointels,
     * FK2Param[1]: affichage des lignels,
     * FK2Param[2]: affichage des surfels.
     */
    bool FK2Param[3];
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_SPAMOD_HH
//******************************************************************************
#endif // MODULE_SPAMOD
//******************************************************************************
