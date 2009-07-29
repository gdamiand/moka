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
 *  Fichier  : Ineq_Table6_Att.h                                             *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Ineq_Table6_Att. Cette *
 * classe permet de stocker un objet de la classe Ineq_Table6 en tant        *
 * qu'attribut.                                                               *
 *                                                                            *
 *****************************************************************************/


#ifndef INEQ_TABLE6_ATT_H
#define INEQ_TABLE6_ATT_H

#include "ineq-table-6.hh"
#include "attribute.hh"



/******************************************************************************
 *                            Classe Ineq_Table6_Att                         *
 *****************************************************************************/

class Ineq_Table6_Att : public CAttribute {

 public:

  // Constructeurs.
  Ineq_Table6_Att();
  Ineq_Table6_Att(CVertex const & p);
  Ineq_Table6_Att(CVertex const & p1, CVertex const & p2);
  Ineq_Table6_Att(Ineq_Table6 *t);

  // Destructeur.
  virtual ~Ineq_Table6_Att();

  // Surcharge des méthodes de la classe CAttribute.
  TAttributeId getType() const;
  Ineq_Table6* Get_Data();
  CAttribute * copy() const;
  void destroy();
  void save(std::ostream &fd) const;
  void load(std::istream &fd);

 private:

  // Pointeur sur un objet de la classe Ineq_Table6.
  Ineq_Table6 * tab;
};

/******************************************************************************
 *  Fichier  : Ineq_Table6_Att.inl                                           *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Ineq_Table6_Att.                                                          *
 *                                                                            *
 *****************************************************************************/

#include "user.hh"

// Constructeurs.
inline
Ineq_Table6_Att::Ineq_Table6_Att()
{
  tab = new Ineq_Table6();
}

inline
Ineq_Table6_Att::Ineq_Table6_Att(CVertex const & p)
{
  tab = new Ineq_Table6(p);
}

inline
Ineq_Table6_Att::Ineq_Table6_Att(CVertex const & p1, CVertex const & p2)
{
  tab = new Ineq_Table6(p1, p2);
}

inline
Ineq_Table6_Att::Ineq_Table6_Att(Ineq_Table6 * t)
{
  tab = t;
}


// Destructeur.
inline
Ineq_Table6_Att::~Ineq_Table6_Att()
{
  delete tab;
}


// Surcharge des méthodes de la classe CAttribute.
inline
TAttributeId Ineq_Table6_Att::getType() const
{
  return INEQ6_ATTRIBUTE_ID;
}

inline
Ineq_Table6 * Ineq_Table6_Att::Get_Data()
{
  return tab;
}

inline
CAttribute * Ineq_Table6_Att::copy() const
{
  Ineq_Table6 * t = new Ineq_Table6(*(this->tab));

  return new Ineq_Table6_Att(t);
}


inline
void Ineq_Table6_Att::destroy()
{
  delete this;
}


inline
void Ineq_Table6_Att::save(std::ostream &fd) const
{
  for (int i=0; i<6; i++)
    fd << *tab->Get_Ineq(i);
}

inline
void Ineq_Table6_Att::load(std::istream &fd)
{
  Inequation iq;

  for (int i=0; i<6; i++)
  {
    fd >> iq;
    tab->Set_Ineq(i, iq);
  }
}

#endif
