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
 *  Fichier  : Color_Table.h                                                  *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Color_Table. Cette      *
 * classe permet de représenter un tableau de couleurs comportant quatre      *
 * éléments.                                                                  *
 *                                                                            *
 *****************************************************************************/


#ifndef COLOR_TABLE_H
#define COLOR_TABLE_H

#include "color.hh"



/******************************************************************************
 *                               Classe Color_Table                           *
 *****************************************************************************/

class Color_Table {

 public:

  // Constructeurs.
  Color_Table();
  Color_Table(Color_Table const & other);
  Color_Table(Color const & c1, Color const & c2,
	      Color const & c3, Color const & c4);

  // Méthode permettant la lecture d'une des couleurs du tableau.
  Color * Get_Color(int num);

  // Méthode permettant l'écriture d'une couleurs dans le tableau.
  void Set_Color(int num, Color const & i);


 private:

  // Tableau comportant 4 cases, et contenant des objets de la classe Color.
  Color tab[4];
};

/******************************************************************************
 *  Fichier  : Color_Table.inl                                                *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe Color_Table*
 *                                                                            *
 *****************************************************************************/


#include "vector3d.hh"



// Constructeurs.
inline
Color_Table::Color_Table()
{}

inline
Color_Table::Color_Table(Color_Table const & other)
{
  for (int i=0 ; i<4 ; i++)
    tab[i] = other.tab[i];
}

inline
Color_Table::Color_Table(Color const & c1, Color const & c2,
			 Color const & c3, Color const & c4)
{
  tab[0] = c1;
  tab[1] = c2;
  tab[2] = c3;
  tab[3] = c4;
}


// Méthode permettant la lecture d'une des couleurs du tableau.
inline
Color * Color_Table::Get_Color(int num)
{
  assert(num >= 0 && num <= 3);
  return &tab[num];
}


// Méthode permettant l'écriture d'une couleurs dans le tableau.
inline
void Color_Table::Set_Color(int num, Color const & i)
{
  assert(num >= 0 && num <= 3);
  tab[num] = i;
}

#endif
