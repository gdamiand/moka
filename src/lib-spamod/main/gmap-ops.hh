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
 *  Fichier  : Gmap_Ops.h                                                     *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient des routines permettant d'effectuer quelques          *
 * opérations sur les G-cartes.                                               *
 *                                                                            *
 *****************************************************************************/


#ifndef GMAP_OPS_H
#define GMAP_OPS_H

#include "g-map.hh"
#include "vertex.hh"

namespace GMap3d
{

// Calcul du centre de la boite englobante d'un objet.
CVertex Calculate_Center(CGMap *G);

// Initialisation de la G-carte.
void Init_Gmap(CGMap * G);

// Recherche d'un attribut dans une G-carte.
CAttribute * Find_Attribute(CGMap * G, TOrbit o, TAttributeId typeAttr);

} // namespace GMap3d

#endif
