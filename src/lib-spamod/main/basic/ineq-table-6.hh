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
 *  Fichier  : Ineq_Table6.h                                                 *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Ineq_Table6. Cette     *
 * classe permet de représenter un tableau d'inéquations, comportant six      *
 * éléments.                                                                  *
 *                                                                            *
 *****************************************************************************/


#ifndef INEQ_TABLE6_H
#define INEQ_TABLE6_H

#include "inequation.hh"



/******************************************************************************
 *                             Classe Ineq_Table6                            *
 *****************************************************************************/

class Ineq_Table6 {

 public:

  // constructeurs.
  Ineq_Table6();
  Ineq_Table6(Ineq_Table6 const & other);
  Ineq_Table6(CVertex const & p);
  Ineq_Table6(CVertex const & p1, CVertex const & p2);

  // Méthode permettant la lecture d'une des équation du tableau.
  Inequation * Get_Ineq(int num);

  // Méthode permettant l'écriture d'une équation dans le tableau.
  void Set_Ineq(int num, Inequation const & i);


 private:

  // Tableau comportant six cases, et contenant des objets de la classe
  // Inequation.
  Inequation tab[6];
};

/******************************************************************************
 *  Fichier  : Ineq_Table6.inl                                               *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Ineq_Table6.                                                              *
 *                                                                            *
 *****************************************************************************/



// constructeurs.
inline
Ineq_Table6::Ineq_Table6()
{}

inline
Ineq_Table6::Ineq_Table6(Ineq_Table6 const & other)
{
  for (int i=0; i<6; i++)
    tab[i] = other.tab[i];
}

inline
Ineq_Table6::Ineq_Table6(CVertex const & p)
{
  tab[1].SetB(0);
  tab[0].SetB(0);
  tab[1].SetC(0);
  tab[0].SetC(0);

  tab[0].SetA(1);
  tab[1].SetA(-1);
  tab[0].SetW(p.getX() + 0.5);
  tab[1].SetW(0.5 - p.getX());

  tab[3].SetA(0);
  tab[2].SetA(0);
  tab[3].SetC(0);
  tab[2].SetC(0);

  tab[2].SetB(1);
  tab[3].SetB(-1);
  tab[2].SetW(p.getY() + 0.5);
  tab[3].SetW(0.5 - p.getY());

  tab[5].SetB(0);
  tab[4].SetB(0);
  tab[5].SetA(0);
  tab[4].SetA(0);

  tab[4].SetC(1);
  tab[5].SetC(-1);
  tab[4].SetW(p.getZ() + 0.5);
  tab[5].SetW(0.5 - p.getZ());
}

inline
Ineq_Table6::Ineq_Table6(CVertex const & p1, CVertex const & p2)
{
  float d;

  tab[1].SetC(0);
  tab[0].SetC(0);

  tab[0].SetA(p1.getY()-p2.getY());
  tab[0].SetB(p2.getX()-p1.getX());	
  tab[1].SetA(-tab[0].GetA());
  tab[1].SetB(-tab[0].GetB());

  d = -(tab[0].GetA()*p1.getX() + tab[0].GetB()*p1.getY());

  tab[0].SetW((fabs(tab[0].GetA()) + fabs(tab[0].GetB()))/2.0);
  tab[1].SetW(tab[0].GetW() + d);
  tab[0].SetW(tab[0].GetW() - d);

  tab[3].SetA(0);
  tab[2].SetA(0);

  tab[2].SetB(p1.getZ()-p2.getZ());
  tab[2].SetC(p2.getY()-p1.getY());
  tab[3].SetB(-tab[2].GetB());
  tab[3].SetC(-tab[2].GetC());

  d = -(tab[2].GetB()*p1.getY() + tab[2].GetC()*p1.getZ());

  tab[2].SetW((fabs(tab[2].GetB()) + fabs(tab[2].GetC()))/2.0);
  tab[3].SetW(tab[2].GetW() + d);
  tab[2].SetW(tab[2].GetW() - d);

  tab[5].SetB(0);
  tab[4].SetB(0);

  tab[4].SetC(p1.getX()-p2.getX());
  tab[4].SetA(p2.getZ()-p1.getZ());	
  tab[5].SetC(-tab[4].GetC());
  tab[5].SetA(-tab[4].GetA());

  d = -(tab[4].GetA()*p1.getX() + tab[4].GetC()*p1.getZ());

  tab[4].SetW((fabs(tab[4].GetA()) + fabs(tab[4].GetC()))/2.0);
  tab[5].SetW(tab[4].GetW() + d);
  tab[4].SetW(tab[4].GetW() - d);

  for (int i=0; i<6; i++)
  {
    if (tab[i].GetA() == 0 && tab[i].GetB() == 0 && tab[i].GetC() == 0)
      tab[i].Set_Unused();
  }
}



// Méthode permettant la lecture d'une des équation du tableau.
inline
Inequation * Ineq_Table6::Get_Ineq(int num)
{
  assert(num >= 0 && num <= 5);

  return &tab[num];
}


// Méthode permettant l'écriture d'une équation dans le tableau.
inline
void Ineq_Table6::Set_Ineq(int num, Inequation const & i)
{
  assert(num >= 0 && num <= 5);

  tab[num] = i;
}

#endif
