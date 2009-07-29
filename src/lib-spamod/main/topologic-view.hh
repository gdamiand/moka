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
 *  Fichier  : Topologic_View.h                                               *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Topologic_View. Cette   *
 * classe permet de construire et d'afficher les éléments constituants la vue *
 * topologique d'un objets.                                                   *
 *                                                                            *
 *****************************************************************************/


#ifndef TOPOLOGIC_VIEW_H
#define TOPOLOGIC_VIEW_H

#include "g-map.hh"



/******************************************************************************
 *                         Classe Topologic_View                              *
 *****************************************************************************/

namespace GMap3d
{

class Topologic_View {

 public:

  // Constructeur.
  Topologic_View(CGMap * GM);

  // Calcul des coordonnées des points à afficher (un point par brin) pour que
  // les liaisons alpha0, alpha1 et alpha2 soient visibles.
  void Create_Face_Darts_View(CDart * dgm, int markDart);

  // Calcul des coordonnées des points à afficher (un point par brin) pour que
  // les liaisons alpha3 soient visibles.
  void Create_Volume_Darts_View(CDart * dgm, int markVolume);

  // Calcul des coordonnées des points à afficher pour représenter les brins.
  void Create_Darts_View();

  // Affichage des brins.
  void Darts_Draw();

  // Affichage des liaison "alpha i".
  void Alpha_Draw(int i);


 private:

  // G-carte devant être visualisée.
  CGMap * G;

};

} // namespace GMap3d

#endif
