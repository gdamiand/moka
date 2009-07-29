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
 *  Fichier  : Vector3D.h                                                     *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la sp�cification de la classe Vector3D. Cette classe  *
 * permet de repr�senter un vecteur � l'aide de trois nombres � virgule       *
 * flottante correspondants aux trois composantes d'un vecteur.               *
 *                                                                            *
 *****************************************************************************/


#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "vertex.hh"



/******************************************************************************
 *                               Classe Vector3D                              *
 *****************************************************************************/

class Vector3D : public CVertex {

 public:

  // Constructeurs.
  Vector3D();
  Vector3D(float vx, float vy, float vz);
  Vector3D(Vector3D const & v);
  Vector3D(CVertex const & p1, CVertex const & p2);

  // M�thode calculant la longueur d'un vecteur.
  float Get_Length();

  // M�thode permettant la normalisation d'un vecteur.
  void Normalize();

  // M�thode calculant le produit scalaire de deux vecteurs.
  float Scal_Product(Vector3D const & v);

  // M�thode calculant le produit vectoriel de deux vecteurs.
  Vector3D Vect_Product(Vector3D const & v);
};

/******************************************************************************
 *  Fichier  : Vector3D.inl                                                   *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'impl�mentation des m�thodes de la classe Vector3D   *
 *                                                                            *
 *****************************************************************************/


// Constructeurs.
inline
Vector3D::Vector3D()
  : CVertex(0.0, 0.0, 0.0)
{
}


inline
Vector3D::Vector3D(float vx, float vy, float vz)
  :CVertex(vx, vy, vz)
{
}

inline
Vector3D::Vector3D(Vector3D const & v)
  :CVertex(v)
{
}

inline
Vector3D::Vector3D(CVertex const & p1, CVertex const & p2)
  :CVertex( p2.getX() - p1.getX(), p2.getY() - p1.getY(), p2.getZ() - p1.getZ())
{
}


// M�thode calculant la longueur d'un vecteur.
inline
float Vector3D::Get_Length()
{
  return norm();
}


// M�thode permettant la normalisation d'un vecteur.
inline
void Vector3D::Normalize()
{
   float n = norm();
  if (!isZero(n)) *this/n;
}


// M�thode calculant le produit scalaire de deux vecteurs.
inline
float Vector3D::Scal_Product(Vector3D const & v)
{
  Vector3D v1 = *this;
  Vector3D v2 = v;

  v1.Normalize();
  v2.Normalize();

  return v1.dot(v2);
}


// M�thode calculant le produit vectoriel de deux vecteurs.
inline
Vector3D Vector3D::Vect_Product(Vector3D const & v)
{
  return Vector3D(getY()*v.getZ() - v.getY()*getZ(),
		  getZ()*v.getX() - v.getZ()*getX(),
		  getX()*v.getY() - v.getX()*getY());
}

#endif
