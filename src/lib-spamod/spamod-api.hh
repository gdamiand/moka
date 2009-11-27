/*
 * lib-spamod : Visualisation des objets en discret.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-spamod
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
#ifndef SPAMOD_API_HH
#define SPAMOD_API_HH
//******************************************************************************
#include "spamod-types.hh"

#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <bitset>

class Color_Table;

namespace GMap3d
{
  class CGMap;

  class Topologic_View;
  class Analytic_View;
  class Euclidian_View;
  class Voxel_View;
  class K2_View;
  //****************************************************************************
  class CSpamodAPI
  {
  private:
    CGMap * FMap;

    Topologic_View * TV;
    Euclidian_View * EV;
    Analytic_View  * AV;
    Voxel_View     * VV;
    K2_View        * KV;

    TSpamodViewMode FViewMode;

    /* Pour mémoriser les orbites initialement utilisées dans la carte:
     */

    std::bitset<16> FUsedOrbits;

    /* Couleurs:
     */

    Color_Table * colors;

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

  protected:
    /* Méthode permettant de calculer les embeddings que SpaMod utilise.
     */
    void computeSpamodEmbeddings();

    /* Méthode permettant de détruire les embeddings que SpaMod utilise.
     */
    void deleteSpamodEmbeddings();

  public:
    CSpamodAPI(CGMap * AMap);
    virtual ~CSpamodAPI();

    // Définition du mode de visualisation:
    void setViewMode(TSpamodViewMode);
    TSpamodViewMode getViewMode() const;

    // Accessseurs aux paramètres de la vue euclidienne:
    bool getEuclidianParam(int i);
    void setEuclidianParam(int i, bool val);

    // Accessseurs aux paramètres de la vue analytique:
    bool getAnalyticParam(int i);
    void setAnalyticParam(int i, bool val);

    // Accessseurs aux paramètres de la vue voxel:
    bool getVoxelParam(int i);
    void setVoxelParam(int i, bool val);

    // Accessseurs aux paramètres de la vue K2:
    bool getK2Param(int i);
    void setK2Param(int i, bool val);

    void drawMap();
    };

} // namespace GMap3d
//******************************************************************************
#endif // SPAMOD_API_HH
//******************************************************************************
