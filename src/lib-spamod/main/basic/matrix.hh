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
 *  Fichier  : Matrix.h                                                       *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Matrix. Cette classe    *
 * permet de représenter une matrice de taille quelconque contenant des       *
 * variables de type unsigned char. Un tableau comportant six cases et        *
 * contenant des variables de type int alpha de plus été ajouté pour les besoins  *
 * du programme.                                                              *
 *                                                                            *
 *****************************************************************************/


#ifndef MATRIX_H
#define MATRIX_H

#include "attribute.hh"



/******************************************************************************
 *                               Classe Matrix                                *
 *****************************************************************************/

class Matrix {

 public:

  // Constructeurs.
  Matrix();
  Matrix(Matrix const & mat);
  Matrix(int tab[6], unsigned char color);

  // Destructeur.
  ~Matrix();

  // Méthodes permettant la lecture des variables de la classe.
  int Get_X_Min() const;
  int Get_X_Max() const;
  int Get_Y_Min() const;
  int Get_Y_Max() const;
  int Get_Z_Min() const;
  int Get_Z_Max() const;
  unsigned char Get_Color(int alpha, int b, int c) const;
  bool Get_Bit(int alpha, int b, int c, int bit) const;
  unsigned char Get_Value(int alpha, int b, int c) const;

  // Méthodes permettant l'écriture de variables dans la classe.
  void Set_Color(int alpha, int b, int c, unsigned char color);
  void Set_Bit(int alpha, int b, int c, int bit);
  void Unset_Bit(int alpha, int b, int c, int bit);
  void Set_Value(int alpha, int b, int c, unsigned char val);


 private:

  // Matrice.
  unsigned char ***m;

  // Tableau comportant six cases et contenant des variables de type int.
  int limits[6];
};

/******************************************************************************
 *  Fichier  : Matrix.inl                                                     *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe Matrix.    *
 *                                                                            *
 *****************************************************************************/


#include <cassert>
#include "definition.hh"



// Constructeurs.
inline
Matrix::Matrix()
{
  for (int i=0; i<6; i++)
    limits[i] = 0;

  m = NULL;
}	

inline
Matrix::Matrix(int tab[6], unsigned char color)
{
  for (int i=0; i<6; i++)
    limits[i] = tab[i];
	
  m = new unsigned char**[tab[0]-tab[1]+1];

  for (int i=0; i<limits[0]-limits[1]+1; i++)
  {
    m[i] = new unsigned char*[tab[2]-tab[3]+1];

    for (int j=0; j<tab[2]-tab[3]+1; j++)
    {
      m[i][j] = new unsigned char[tab[4]-tab[5]+1];

      for (int k=0; k<tab[4]-tab[5]+1; k++)
	m[i][j][k] = (m[i][j][k] & 0xFC) | (color & 0x3);
    }
  }
}

inline
Matrix::Matrix(Matrix const & mat)
{
  for (int i=0; i<6; i++)
    limits[i] = mat.limits[i];

  m = new unsigned char**[limits[0]-limits[1]+1];

  for (int i=0; i<limits[0]-limits[1]+1; i++)
  {
    m[i] = new unsigned char*[limits[2]-limits[3]+1];

    for (int j=0; j<limits[2]-limits[3]+1; j++)
    {
      m[i][j] = new unsigned char[limits[4]-limits[5]+1];

	for (int k=0; k<limits[4]-limits[5]+1; k++)
	  m[i][j][k] = mat.m[i][j][k];
    }
  }
}


// Destructeur.	
inline
Matrix::~Matrix()
{
  if (m != NULL)
  {
    for (int i=0; i<limits[0]-limits[1]+1; i++)
    {
      for (int j=0; j<limits[2]-limits[3]+1; j++)
	delete [] m[i][j];
      delete [] m[i];
    }
    delete [] m;
  }
}


// Méthodes permettant la lecture des variables de la classe.
inline
int Matrix::Get_X_Min() const
{
  return limits[1];
}

inline
int Matrix::Get_X_Max() const
{
  return limits[0];
}

inline
int Matrix::Get_Y_Min() const
{
  return limits[3];
}

inline
int Matrix::Get_Y_Max() const
{
  return limits[2];
}

inline
int Matrix::Get_Z_Min() const
{
  return limits[5];
}

inline
int Matrix::Get_Z_Max() const
{
  return limits[4];
}

inline
unsigned char Matrix::Get_Color(int alpha, int b, int c) const
{
  assert(alpha >= limits[1] && alpha <= limits[0] &&
	 b >= limits[3] && b <= limits[2] &&
	 c >= limits[5] && c <= limits[4]);

  return (m[alpha-limits[1]][b-limits[3]][c-limits[5]] & 0x3);
}

inline
bool Matrix::Get_Bit(int alpha, int b, int c, int bit) const
{
  assert(alpha >= limits[1] && alpha <= limits[0] &&
	 b >= limits[3] && b <= limits[2] &&
	 c >= limits[5] && c <= limits[4]);

  return ((m[alpha-limits[1]][b-limits[3]][c-limits[5]] >> bit) & 0x1);
}

inline
unsigned char Matrix::Get_Value(int alpha, int b, int c) const
{
  assert(alpha >= limits[1] && alpha <= limits[0] &&
	 b >= limits[3] && b <= limits[2] &&
	 c >= limits[5] && c <= limits[4]);

  return (m[alpha-limits[1]][b-limits[3]][c-limits[5]]);
}


// Méthodes permettant l'écriture de variables dans la classe.
inline
void Matrix::Set_Color(int alpha, int b, int c, unsigned char color)
{
  assert(alpha >= limits[1] && alpha <= limits[0] &&
	 b >= limits[3] && b <= limits[2] &&
	 c >= limits[5] && c <= limits[4]);

  m[alpha-limits[1]][b-limits[3]][c-limits[5]] =
  (m[alpha-limits[1]][b-limits[3]][c-limits[5]] & 0xFC) | (color & 0x3);
}

inline
void Matrix::Set_Bit(int alpha, int b, int c, int bit)
{
  assert(alpha >= limits[1] && alpha <= limits[0] &&
	 b >= limits[3] && b <= limits[2] &&
	 c >= limits[5] && c <= limits[4]);

  m[alpha-limits[1]][b-limits[3]][c-limits[5]] =
  (m[alpha-limits[1]][b-limits[3]][c-limits[5]] | (0x1 << bit));
}

inline	
void Matrix::Unset_Bit(int alpha, int b, int c, int bit)
{
  assert(alpha >= limits[1] && alpha <= limits[0] &&
	 b >= limits[3] && b <= limits[2] &&
	 c >= limits[5] && c <= limits[4]);

  m[alpha-limits[1]][b-limits[3]][c-limits[5]] =
  (m[alpha-limits[1]][b-limits[3]][c-limits[5]] & (0xFF - (0x1 << bit)));
}

inline
void Matrix::Set_Value(int alpha, int b, int c, unsigned char val)
{
  assert(alpha >= limits[1] && alpha <= limits[0] &&
	 b >= limits[3] && b <= limits[2] &&
	 c >= limits[5] && c <= limits[4]);

  m[alpha-limits[1]][b-limits[3]][c-limits[5]] = val;
}

#endif
