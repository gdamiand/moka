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
 *  Fichier  : Voxel_List.h                                                   *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Voxel_List. Cette       *
 * classe permet de stocker une liste d'objets de la classe Voxel en tant     *
 * qu'attribut.                                                               *
 *                                                                            *
 *****************************************************************************/


#ifndef VOXEL_LIST_H
#define VOXEL_LIST_H

#include "voxel.hh"
#include "attribute.hh"

#include <list>
#include <iostream>


/******************************************************************************
 *                                Classe Voxel_List                           *
 *****************************************************************************/

class Voxel_List : public CAttribute {

 public:

  // Constructeur.
  Voxel_List();

  // Destructeur.
  virtual ~Voxel_List();

  // Surcharge des méthodes de la classe CAttribute.
  TAttributeId getType() const;
  void * Get_Data();
  CAttribute * copy() const;
  void destroy();
  void save(std::ostream &fd) const;
  void load(std::istream &fd);

 private:

  // Pointeur sur une liste d'objets de la classe Voxel.
  std::list<Voxel*> * lst;
};


/******************************************************************************
 *  Fichier  : Voxel_List.inl                                                 *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Voxel_List.                                                                *
 *                                                                            *
 *****************************************************************************/

#include "user.hh"


// Constructeur.
inline
Voxel_List::Voxel_List()
{
  lst = new std::list<Voxel*>;
}


// Destructeur.
inline
Voxel_List::~Voxel_List()
{
  std::list<Voxel*>::iterator iter;

  for (iter=lst->begin();
       iter!=lst->end();
       iter++)
    if (*iter != NULL)
      delete *iter;

  delete lst;
}


// Surcharge des méthodes de la classe CAttribute.
inline
TAttributeId Voxel_List::getType() const
{
  return VOXEL_LIST_ATTRIBUTE_ID;
}

inline
void * Voxel_List::Get_Data()
{
  return lst;
}

inline
CAttribute * Voxel_List::copy() const
{
  Voxel_List * l = new Voxel_List();
  std::list<Voxel*>::iterator li;

  for (li=lst->begin(); li!=lst->end(); li++)
    l->lst->push_front(*li);

  return l;
}

inline
void Voxel_List::destroy()
{
  delete this;
}

inline
void Voxel_List::save(std::ostream &fd) const
{
  std::list<Voxel*>::iterator li;

  for (li=lst->begin(); li!=lst->end(); li++)
    fd << *li;
}

inline
void Voxel_List::load(std::istream &fd)
{
}

#endif
