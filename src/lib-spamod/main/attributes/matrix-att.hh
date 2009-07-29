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
 *  Fichier  : Matrix_Att.h                                                   *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Matrix_Att. Cette       *
 * classe permet de stocker un objet de la classe Matrix en tant              *
 * qu'attribut.                                                               *
 *                                                                            *
 *****************************************************************************/


#ifndef MATRIX_ATT_H
#define MATRIX_ATT_H

#include "matrix.hh"
#include "attribute.hh"



/******************************************************************************
 *                              Classe Matrix_Att                             *
 *****************************************************************************/

class Matrix_Att : public CAttribute
{
public:

  // Constructeurs.
  Matrix_Att();
  Matrix_Att(int tab[6], unsigned char color);

  // Destructeur.
  virtual ~Matrix_Att();

  // Surcharge des méthodes de la classe CAttribute.
  TAttributeId getType() const;
  Matrix * Get_Data();
  CAttribute * copy() const;
  void destroy();
  void save(std::ostream &fd) const;
  void load(std::istream &fd);

private:

  // Pointeur sur un objet de la classe Matrix.
  Matrix * mat;

};

/******************************************************************************
 *  Fichier  : Matrix_Att.inl                                                 *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Matrix_Att.                                                                *
 *                                                                            *
 *****************************************************************************/

#include "user.hh"

// Constructeurs.
inline
Matrix_Att::Matrix_Att()
{
  mat = NULL;
}

inline
Matrix_Att::Matrix_Att(int tab[6], unsigned char color)
{
  mat = new Matrix(tab,color);
}


// Destructeur.
inline
Matrix_Att::~Matrix_Att()
{
  delete mat;
}


// Surcharge des méthodes de la classe CAttribute.
inline
TAttributeId Matrix_Att::getType() const
{
  return MATRIX_ATTRIBUTE_ID;
}

inline
Matrix * Matrix_Att::Get_Data()
{
  return mat;
}

inline
CAttribute * Matrix_Att::copy() const
{
  Matrix_Att * m = new Matrix_Att();
  m->mat = new Matrix(*mat);
	
  return m;
}

inline
void Matrix_Att::destroy()
{
  delete this;
}


inline
void Matrix_Att::save(std::ostream &fd) const
{
  fd << mat->Get_X_Max() << " " << mat->Get_X_Min() << " "
     << mat->Get_Y_Max() << " " << mat->Get_Y_Min() << " "
     << mat->Get_Z_Max() << " " << mat->Get_Z_Min() << std::endl;

  for (int k=mat->Get_Z_Min(); k<=mat->Get_Z_Max(); k++)
    for (int j=mat->Get_Y_Min(); j<=mat->Get_Y_Max(); j++)
    {
      for (int i=mat->Get_X_Min(); i<=mat->Get_X_Max(); i++)
	fd << (int)mat->Get_Value(i,j,k) << " ";

      fd << std::endl;
    }
}

inline
void Matrix_Att::load(std::istream &fd)
{
}

#endif
