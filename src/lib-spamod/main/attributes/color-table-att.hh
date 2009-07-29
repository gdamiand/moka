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
 *  Fichier  : Color_Table_Att.h                                              *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Color_Table_Att. Cette  *
 * classe permet de stocker un objet de la classe Color_Table en tant         *
 * qu'attribut.                                                               *
 *                                                                            *
 *****************************************************************************/


#ifndef COLOR_TABLE_ATT_H
#define COLOR_TABLE_ATT_H

#include "color-table.hh"
#include "attribute.hh"



/******************************************************************************
 *                              Classe Color_Table_Att                        *
 *****************************************************************************/

class Color_Table_Att : public CAttribute {

 public:

  // Constructeurs.
  Color_Table_Att();
  Color_Table_Att(Color const & c1, Color const & c2,
		  Color const & c3, Color const & c4);
  Color_Table_Att(Color_Table *t);

  // Destructeur.
  virtual ~Color_Table_Att();

  // Surcharge des méthodes de la classe CAttribute.
  TAttributeId getType() const;
  Color_Table * Get_Data();
  CAttribute * copy() const;
  void destroy();
  void save(std::ostream &fd) const;
  void load(std::istream &fd);

 private:

  // Pointeur sur un objet de la classe Color_Table.
  Color_Table * tab;
};

/******************************************************************************
 *  Fichier  : Color_Table_Att.inl                                            *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Color_Table_Att.                                                           *
 *                                                                            *
 *****************************************************************************/


#include "user.hh"

// Constructeurs.
inline
Color_Table_Att::Color_Table_Att()
{
  tab = new Color_Table();
}


inline
Color_Table_Att::Color_Table_Att(Color const & c1, Color const & c2,
				 Color const & c3, Color const & c4)
{
  tab = new Color_Table(c1, c2, c3, c4);
}


inline
Color_Table_Att::Color_Table_Att(Color_Table * t)
{
  tab = t;
}


// Destructeur.
inline
Color_Table_Att::~Color_Table_Att()
{
  delete tab;
}


// Surcharge des méthodes de la classe CAttribute.
inline
TAttributeId Color_Table_Att::getType() const
{
  return COLOR_TABLE_ATTRIBUTE_ID;
}


inline
Color_Table * Color_Table_Att::Get_Data()
{
  return tab;
}


inline
CAttribute * Color_Table_Att::copy() const
{
  Color_Table * t = new Color_Table(*(this->tab));

  return new Color_Table_Att(t);
}


inline
void Color_Table_Att::destroy()
{
  delete this;
}


inline
void Color_Table_Att::save(std::ostream &fd) const
{
  for (int i=0 ; i<4 ; i++)
    fd << *tab->Get_Color(i);
}


inline
void Color_Table_Att::load(std::istream &fd)
{
  Color col;

  for (int i=0; i<4; i++)
  {
    fd >> col;
    tab->Set_Color(i, col);
  }
}

#endif
