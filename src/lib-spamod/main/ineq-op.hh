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
 *  Fichier  : Ineq_Op.h                                                      *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient les spécifications des routines qui permettent de     *
 * créer et de choisir les bonnes inéquations pour la vue analytique.         *
 *                                                                            *
 *****************************************************************************/


#ifndef INEQ_OP_H
#define INEQ_OP_H

#include "g-map.hh"
#include "inequation.hh"

namespace GMap3d
{

// Calcul des inéquations des sommets de l'objet.
void Create_Vertex_Ineq(CGMap * G, CDart * dgm, int markVertex);

// Calcul des inéquatons des arêtes de l'objet.
void Create_Edges_Ineq(CGMap * G, CDart * dgm, int markEdge);

// Calcul des inéquations des faces de l'objet.
void Create_Face_Ineq(CGMap * G, CDart * dgm, int markFace);

// Calcul des inéquations des cellules de l'objet.
void Create_Ineq(CGMap *G);

// Calcul de la boite englobante d'une cellule.
void Calculate_Bounding_Box(CGMap * G, CDart * d, TOrbit o, float minMaxXyz[6]);

// Mise à jour de la variable used des inéquations visibles des sommets.
void Ineq_Vertex_Set_Unused(CGMap * G, CDart * d, TOrbit o, float minMaxXyz[6]);

// Choix des inéquations visibles des sommets pour l'objet.
void Compare_Ineq_Vertex(CGMap* G, CDart* d, int markVertex);

// Choix des inéquations visibles des arêtes pour l'objet.
void Compare_Ineq_Edge(CGMap* G, CDart* d, int markEdge);

// Choix des inéquations visibles de l'objet.
void Choose_Ineq(CGMap* G);

} // namespace GMap3d

#endif
