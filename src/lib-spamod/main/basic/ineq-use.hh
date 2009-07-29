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
 *  Fichier  : Ineq_Use.h                                                     *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Ineq_Use. Cette classe  *
 * permet de représenter un tableau de booléens, comportant six éléments.     *
 *                                                                            *
 *****************************************************************************/


#ifndef INEQ_USE_H
#define INEQ_USE_H



/******************************************************************************
 *                                Classe Ineq_Use                             *
 *****************************************************************************/

class Ineq_Use {

 public:

  // Constructeur.
  Ineq_Use();

  // Méthode permettant la lecture d'un des booléens du tableau.
  bool Get_Use(int i);

  // Méthodes permettant l'écriture d'un booléen dans le tableau.
  void Set_Use(int i, bool u);
  void Set_Used(int i);
  void Set_Unused(int i);


 private:

  // Tableau comportant 6 éléments et contenant des variables booléennes.
  bool tab[6];
};

/******************************************************************************
 *  Fichier  : Ineq_Use.inl                                                   *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe Ineq_Use.  *
 *                                                                            *
 *****************************************************************************/



// Constructeur.
inline
Ineq_Use::Ineq_Use()
{
  for (int i=0; i<6; i++)
    tab[i] = true;
}


// Méthode permettant la lecture d'un des booléens du tableau.
inline
bool Ineq_Use::Get_Use(int i)
{
  return tab[i];
}


// Méthodes permettant l'écriture d'un booléen dans le tableau.
inline
void Ineq_Use::Set_Use(int i, bool u)
{
  tab[i] = u;
}

inline
void Ineq_Use::Set_Used(int i)
{
  tab[i] = true;
}

inline
void Ineq_Use::Set_Unused(int i)
{
  tab[i] = false;
}

#endif
