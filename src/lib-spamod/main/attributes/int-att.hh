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
 *  Fichier  : Int_Att.h                                                      *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Int_Att. Cette          *
 * classe permet de stocker un entier en tant qu'attribut.                    *
 *                                                                            *
 *****************************************************************************/


#ifndef INT_ATT_H
#define INT_ATT_H

#include "attribute.hh"



/******************************************************************************
 *                               Classe Int_Att                               *
 *****************************************************************************/

class Int_Att : public CAttribute {

 public:

  // Constructeurs.
  Int_Att();
  Int_Att(int v);

  // Destructeur.
  virtual ~Int_Att();

  // Surcharge des méthodes de la classe CAttribute.
  TAttributeId getType() const;
  int * Get_Data();
  CAttribute * copy() const;
  void destroy();
  void save(std::ostream &fd) const;
  void load(std::istream &fd);

 private:

  // Pointeur sur un entier.
  int * value;
};

/******************************************************************************
 *  Fichier  : Int_Att.inl                                                    *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Int_Att.                                                                   *
 *                                                                            *
 *****************************************************************************/

#include "user.hh"

// Constructeurs.
inline
Int_Att::Int_Att()
{
  value = new int;
}

inline
Int_Att::Int_Att(int v)
{
  value = new int(v);
}


// Destructeur.
inline
Int_Att::~Int_Att()
{
  delete value;
}


// Surcharge des méthodes de la classe CAttribute.
inline
TAttributeId Int_Att::getType() const
{
  return INT_ATTRIBUTE_ID;
}

inline
int * Int_Att::Get_Data()
{
  return value;
}

inline
CAttribute * Int_Att::copy() const
{
  return new Int_Att(*value);
}

inline
void Int_Att::destroy()
{
  delete this;
}


inline
void Int_Att::save(std::ostream &fd) const
{
  fd << *value << std::endl;
}

inline
void Int_Att::load(std::istream &fd)
{
}

#endif
