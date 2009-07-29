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
 *  Fichier  : Point_List.h                                                   *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Point_List. Cette       *
 * classe permet de stocker une liste d'objets de la classe Point3D en tant   *
 * qu'attribut.                                                               *
 *                                                                            *
 *****************************************************************************/


#ifndef POINT_LIST_H
#define POINT_LIST_H

#include "attribute.hh"
#include "vertex.hh"
#include "user.hh"

#include <list>
#include <iostream>

/******************************************************************************
 *                                Classe Point_List                           *
 *****************************************************************************/

class Point_List : public CAttribute {

 public:

  // Constructeurs.
  Point_List();
  Point_List(std::list<CVertex*> * l);

  // Destructeur.
  virtual ~Point_List();

  // Surcharge des méthodes de la classe CAttribute.
  TAttributeId getType() const;
  std::list<CVertex*> * Get_Data();
  CAttribute * copy() const;
  void destroy();
  void save(std::ostream &fd) const;
  void load(std::istream &fd);

 private:

  // Pointeur sur une liste d'objets de la classe CVertex.
  std::list<CVertex*> *lst;
};

/******************************************************************************
 *  Fichier  : Point_List.inl                                                 *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Point_List.                                                                *
 *                                                                            *
 *****************************************************************************/


// Constructeurs.
inline
Point_List::Point_List()
{
  lst = new std::list<CVertex*>;
}

inline
Point_List::Point_List(std::list<CVertex*> * l)
{
  lst = l;
}


// Destructeur.
inline
Point_List::~Point_List()
{
  delete lst;
}


// Surcharge des méthodes de la classe CAttribute.
inline
TAttributeId Point_List::getType() const
{
  return PT_LIST_ATTRIBUTE_ID;
}

inline
std::list<CVertex*> * Point_List::Get_Data()
{
  return lst;
}

inline
CAttribute * Point_List::copy() const
{
  Point_List * l = new Point_List();
  std::list<CVertex*>::iterator li;

  for (li=lst->begin(); li!=lst->end(); li++)
    l->lst->push_front(*li);

  return l;
}

inline
void Point_List::destroy()
{
  delete this;
}

inline
void Point_List::save(std::ostream &fd)const
{
  std::list<CVertex*>::iterator li;

  for (li=lst->begin(); li!=lst->end(); li++)
    fd << *li;
}

inline
void Point_List::load(std::istream &fd)
{
}


#endif
