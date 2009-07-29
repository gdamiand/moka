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
 *  Fichier  : Analytic_View.h                                                *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Analytic_View. Cette    *
 * classe permet d'afficher les éléments constituant la vue analytique d'un   *
 * objet.                                                                     *
 *                                                                            *
 *****************************************************************************/


#ifndef ANALYTIC_VIEW_H
#define ANALYTIC_VIEW_H

#include "g-map.hh"
#include "vertex.hh"



/******************************************************************************
 *                              Classe AnalyticView                          *
 *****************************************************************************/

namespace GMap3d
{

class Analytic_View {

 public:

  // Constructeur.
  Analytic_View(CGMap * GM);

  // Destructeur.
  ~Analytic_View();

  // Creation des listes d'affichage des sommets.
  void Create_Vertices_Faces();

  // Création des listes d'affichage des faces.
  void Create_Edges_Faces();

  // Creation des listes d'affichage des faces.
  void Create_Faces();

  // Affichage des facettes des sommets.
  void Vertices_Draw();

  // Affichage des facettes des arêtes.
  void Edges_Draw();

  // Affichage des facettes des faces.
  void Faces_Draw();


 private:

  // G-carte devant être visualisée.
  CGMap * G;

  // Champ direct info
  int directInfo;

  // Affichage des polygones contenus dans une liste de points.
  void Draw_List(std::list<CVertex*> *l, int nbPoints) const;
};

} // namespace GMap3d

#endif
