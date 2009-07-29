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
 *  Fichier  : Voxel.h                                                        *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Voxel. Cette classe     *
 * permet de représenter ce centre d'un voxel à l'aide de trois coordonnées   *
 * entières.                                                                  *
 *                                                                            *
 *****************************************************************************/


#ifndef VOXEL_H
#define VOXEL_H



/******************************************************************************
 *                                  Classe Voxel                              *
 *****************************************************************************/

class Voxel {

 public:

  // Constructeurs.
  Voxel();
  Voxel(int alpha, int b, int c);

  // Méthodes permettant la lecture des coordonnées.
  int Get_X() const;
  int Get_Y() const;
  int Get_Z() const;

  // Méthode permettant l'écriture des coordonnées.
  void Set_XYZ(int alpha, int b, int c);

  // Opérateur de transfert <<.
  friend std::ostream & operator<<(std::ostream & s,Voxel * v);


 private:

  // Variables représentant les coordonnées du centre du voxel.
  int x, y, z;
};

/******************************************************************************
 *  Fichier  : Voxel.inl                                                      *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe Voxel      *
 *                                                                            *
 *****************************************************************************/


// Constructeurs.
inline
Voxel::Voxel()
  :x(0), y(0), z(0)
{}

inline
Voxel::Voxel(int alpha, int b, int c)
  :x(alpha), y(b), z(c)
{}


// Méthodes permettant la lecture des coordonnées.
inline
int Voxel::Get_X() const
{
  return x;
}

inline
int Voxel::Get_Y() const
{
  return y;
}

inline
int Voxel::Get_Z() const
{
  return z;
}


 // Méthode permettant l'écriture des coordonnées.
inline
void Voxel::Set_XYZ(int alpha, int b, int c)
{
  x = alpha;
  y = b;
  z = c;
}


// Opérateur de transfert <<.
inline
std::ostream & operator<<(std::ostream & s,Voxel * v)
{
  s << v->Get_X() << " " << v->Get_Y() << " " << v->Get_Z() << std::endl;
  return s;
}

#endif
