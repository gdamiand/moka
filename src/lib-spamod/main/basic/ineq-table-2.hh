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
 *  Fichier  : Ineq_Table2.h                                                 *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Ineq_Table2. Cette     *
 * classe permet de représenter un tableau d'inéquations, comportant deux     *
 * éléments.                                                                  *
 *                                                                            *
 *****************************************************************************/


#ifndef INEQ_TABLE2_H
#define INEQ_TABLE2_H

#include "inequation.hh"



/******************************************************************************
 *                             Classe Ineq_Table2                            *
 *****************************************************************************/

class Ineq_Table2 {

 public:

  // Constructeurs.
  Ineq_Table2();
  Ineq_Table2(Ineq_Table2 const & other);
  Ineq_Table2(CVertex const & p1, CVertex const & p2, CVertex const & p3);

  // Méthode permettant la lecture d'une des équation du tableau.
  Inequation * Get_Ineq(int num);

  // Méthode permettant l'écriture d'une équation dans le tableau.
  void Set_Ineq(int num, Inequation const & i);


 private:

  // Tableau comportant deux cases, et contenant des objets de la classe
  // Inequation.
  Inequation tab[2];
};

/******************************************************************************
 *  Fichier  : Ineq_Table2.inl                                               *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Ineq_Table2.                                                              *
 *                                                                            *
 *****************************************************************************/


#include "vector3d.hh"



// Constructeurs.
inline
Ineq_Table2::Ineq_Table2()
{}

inline
Ineq_Table2::Ineq_Table2(Ineq_Table2 const & other)
{
  for (int i=0 ; i<2 ; i++)
    tab[i] = other.tab[i];
}

inline
Ineq_Table2::Ineq_Table2(CVertex const & p1,
			   CVertex const & p2,
			   CVertex const & p3)
{
  float d;
  Vector3D normal;

  Vector3D v1(p2.getX()-p1.getX(),
	      p2.getY()-p1.getY(),
	      p2.getZ()-p1.getZ());

  Vector3D v2(p3.getX()-p1.getX(),
	      p3.getY()-p1.getY(),
	      p3.getZ()-p1.getZ());

  normal = v1.Vect_Product(v2);

  tab[0].SetA(normal.getX());
  tab[0].SetB(normal.getY());
  tab[0].SetC(normal.getZ());
  tab[1].SetA(-tab[0].GetA());
  tab[1].SetB(-tab[0].GetB());
  tab[1].SetC(-tab[0].GetC());

  d = -(tab[0].GetA()*p1.getX() +
        tab[0].GetB()*p1.getY() +
        tab[0].GetC()*p1.getZ());

  tab[0].SetW((fabs(tab[0].GetA()) +
                fabs(tab[0].GetB()) +
		fabs(tab[0].GetC()))/2.0);

  tab[1].SetW(tab[0].GetW() + d);
  tab[0].SetW(tab[0].GetW() - d);
}


// Méthode permettant la lecture d'une des équation du tableau.
inline
Inequation * Ineq_Table2::Get_Ineq(int num)
{
  assert(num >= 0 && num <= 1);
  return &tab[num];
}


// Méthode permettant l'écriture d'une équation dans le tableau.
inline
void Ineq_Table2::Set_Ineq(int num, Inequation const & i)
{
  assert(num >= 0 && num <= 1);
  tab[num] = i;
}

#endif
