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

/******************************************************************************
 *  Fichier  : K2_View.h                                                      *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe K2_View. Cette classe   *
 * permet d'afficher les éléments constituant la vue K2 d'un objet.           *
 *                                                                            *
 *****************************************************************************/


#ifndef K2_VIEW_H
#define K2_VIEW_H

#include "g-map.hh"



/******************************************************************************
 *                                 Classe K2_View                             *
 *****************************************************************************/

namespace GMap3d
{

class K2_View {

 public:

  // Constructeur.
  K2_View(CGMap * GM);

  // Stockage des informations concernant les pointels voisins d'un pointel.
  void Calculate_Matrix();

  // Affichage des pointels.
  void Draw_Pointels();

  // Affichage des lignels.
  void Draw_Lignels();

  // Affichage des surfels.
  void Draw_Surfels();

 private:

  // G-carte devant être visualisée.
  CGMap * G;
};

} // namespace GMap3d

#endif
