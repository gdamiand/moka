/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-gmapkernel
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
// Ce fichier fait partie de "g-map-vertex.hh".
//******************************************************************************

//@TODO: export ?

public:

struct triplet
{
  triplet()
  {}
  
  triplet(CVertex* i, CVertex* j, CVertex* k) : v0(i), v1(j), v2(k)
  {}
  CVertex* v0, *v1, *v2;

  bool operator< (const triplet& other) const
  {
    return
      v0<other.v0 ||
         (v0==other.v0 && v1<other.v1) ||
      (v0==other.v0 && v1==other.v1 && v2<other.v2);
  }
};


/**
 * Importe une carte 3D à partir d'un flot au format ASCII tetmesh.
 * Les brins qui composent la carte avant le chargement ne sont pas détruits.
 *
 * @param AStream Un flot dans lequel lire le contenu de la carte à charger
 * @return Un brin de la carte chargée si le chargement s'est bien
 * déroulé, NULL sinon
 */
CDart* importTetmesh(std::istream & AStream);

/**
 * Importe une carte à partir d'un fichier au format ASCII tetmesh.
 * Les brins qui composent la carte avant le chargement ne sont pas détruits.
 *
 * @param AFilename Un nom de fichier
 * @return Un brin de la carte chargée si le chargement s'est bien
 * déroulé, NULL sinon
 */
CDart* importTetmesh(const char * AFilename);

/**
 * Sous fonction utile pour ajouter un triangle entre les sommets donnés.
 * faces contient la liste de toutes les faces déjà créées.
 * 3-sew le nouveau triangle si la face opposée existe déjà.
 */
CDart* createNewTriangle(CVertex* v0, CVertex* v1, CVertex* v2,
                         std::map<triplet, CDart*>& faces);

/**
 * Crée un nouveau tétraèdre ebtre les sommets donnés.
 * Ce tétra est 3-cousu aux tétra adjacent déjà existants.
 */
CDart* createNewTetra(CVertex* v0, CVertex* v1, CVertex* v2,
                      CVertex* v3, std::map<triplet, CDart*>& faces);


