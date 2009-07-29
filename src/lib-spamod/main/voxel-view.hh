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
 *  Fichier  : Voxel_View.h                                                   *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Voxel_View. Cette classe*
 * permet d'afficher les éléments constituant la vue "voxel" d'un objet.      *
 *                                                                            *
 *****************************************************************************/


#ifndef VOXEL_VIEW_H
#define VOXEL_VIEW_H

#include "g-map.hh"



/******************************************************************************
 *                            Classe Voxel_View                               *
 *****************************************************************************/

namespace GMap3d
{

class Voxel_View {

 public:

  // Constructeur.
  Voxel_View(CGMap * GM);

  // Calcule de la boite englobante d'un objet.
  void Calculate_Int_Bounding_Box(float gmapBoundingBox[6],
				  int matrixBoundingBox[6]);

  // Recherche des points appartenant à la supercouverture de l'objet.
  void Create_Matrix();

  // Affichage des voxels associés aux sommets.
  void Draw_Vertices();

  // Affichage des voxels associés aux arêtes.
  void Draw_Edges();

  // Affichage des voxels associés aux faces.
  void Draw_Faces();


 private:

  // G-carte devant être visualisée.
  CGMap * G;
};

} // namespace GMap3d

#endif
