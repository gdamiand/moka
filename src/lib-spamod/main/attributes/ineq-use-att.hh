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
 *  Fichier  : Ineq_Use_Att.h                                                 *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Ineq_Use_Att. Cette     *
 * classe permet de stocker un objet de la classe Ineq_Use en tant            *
 * qu'attribut.                                                               *
 *                                                                            *
 *****************************************************************************/


#ifndef INEQ_USE_ATT_H
#define INEQ_USE_ATT_H

#include "ineq-use.hh"
#include "attribute.hh"


/******************************************************************************
 *                                 Classe Ineq_Use_Att                        *
 *****************************************************************************/

class Ineq_Use_Att : public CAttribute {

 public:

  // Constructeur.
  Ineq_Use_Att();

  // Destructeur.
  virtual ~Ineq_Use_Att();

  // Surcharge des méthodes de la classe CAttribute.
  TAttributeId getType() const;
  Ineq_Use * Get_Data();
  CAttribute * copy() const;
  void destroy();
  void save(std::ostream &fd) const;
  void load(std::istream &fd);

 private:

  // Pointeur sur un objet de la classe Ineq_Use.
  Ineq_Use * tab;
};

/******************************************************************************
 *  Fichier  : Ineq_Use_Att.inl                                               *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Ineq_Use_Att.                                                              *
 *                                                                            *
 *****************************************************************************/

// Constructeur.
inline
Ineq_Use_Att::Ineq_Use_Att()
{
  tab = new Ineq_Use();
}


// Destructeur.
inline
Ineq_Use_Att::~Ineq_Use_Att()
{
  delete tab;
}


// Surcharge des méthodes de la classe CAttribute.
inline
TAttributeId Ineq_Use_Att::getType() const
{
  return USE_ATTRIBUTE_ID;
}

inline
Ineq_Use * Ineq_Use_Att::Get_Data()
{
  return tab;
}

inline
CAttribute * Ineq_Use_Att::copy() const
{
  Ineq_Use_Att* i = new Ineq_Use_Att();

  for (int j=0; j<6; j++)
    i->tab->Set_Use(j, tab->Get_Use(j));

  return i;
}


inline
void Ineq_Use_Att::destroy()
{
  delete this;
}


inline	
void Ineq_Use_Att::save(std::ostream &fd) const
{
  for (int i=0; i<6; i++)
    fd << tab->Get_Use(i) << " ";	

  fd << std::endl;
}

inline
void Ineq_Use_Att::load(std::istream &fd)
{
}

#endif
