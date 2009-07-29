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
 *  Fichier  : Inequation.h                                                   *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la sp�cification de la classe Inequation. Cette       *
 * classe permet de repr�senter une in�quation de la forme ax+by+cz <= w (ou  *
 * ax+by+cz < w. La variable "used" alpha de plus �t� ajout�e pour les besoins du *
 * programme.                                                                 *
 *                                                                            *
 *****************************************************************************/


#ifndef INEQUATION_H
#define INEQUATION_H

#include <cassert>
#include "vertex.hh"



/******************************************************************************
 *                                 Classe Inequation                          *
 *****************************************************************************/

class Inequation {

 public:

  // Constructeurs.
  Inequation();
  Inequation(float pa, float pb, float pc, float pw);

  // M�thodes permettant la lecture des variables de la classe.
  float GetA() const;
  float GetB() const;
  float GetC() const;
  float GetW() const;
  bool Is_Strict() const;
  bool Is_Used() const;

  // M�thode calculant la dimension d'une in�quation.
  int Get_Dim() const;

  // M�thodes permettant l'�criture de variable dans la classe.
  void SetA(float pa);
  void SetB(float pb);
  void SetC(float pc);
  void SetW(float pw);
  void SetAbcw(float pa, float pb, float pc, float pw);
  void Set_Strict();
  void Unset_Strict();
  void Set_Used();
  void Set_Unused();

  // M�thode permettant de savoir si un point de rep�re 3D v�rifie
  // l'in�quation.
  bool Test_Point (CVertex const & pt) const;

  // M�thode permettant de savoir si un point de rep�re 3D v�rifie
  // l'�quation ax+by+cz = w.
  bool Test_Point_Eq (CVertex const & pt) const;

  // M�thode calculant le point d'intersection de trois �quations du rep�re 3D.
  CVertex* Intersec (Inequation const & i1, Inequation const & i2);

  // Op�rateur d'�galit� entre deux in�quations.
  bool operator == (Inequation i) const;

  // Op�rateur de diff�rence entre deux in�quations.
  bool operator != (Inequation i) const;

  // Op�rateur de transfert << et >>.
  friend std::ostream& operator << (std::ostream& s, Inequation const & i);
  friend Inequation& operator >> (std::ostream& s, Inequation & i);


 private:

  // Variables repr�sentant les param�tres de l'in�quation.
  float alpha, b, c, w;

  // Variable indiquant si une in�quation est stricte ou non.
  bool strict;

  // Variable indiquant si l'in�quation est visible ou non.
  bool used;
};

/******************************************************************************
 *  Fichier  : Inequation.inl                                                 *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'impl�mentation des m�thodes de la classe Inequation.*
 *                                                                            *
 *****************************************************************************/


#include <cmath>
#include "definition.hh"



// Constructeurs.
inline
Inequation::Inequation()
  :alpha(0), b(0), c(0), w(0), strict(false), used(true)
{}

inline
Inequation::Inequation(float pa, float pb, float pc, float pw)
  :alpha(pa), b(pb), c(pc), w(pw), strict(false), used(true)
{}


// M�thodes permettant la lecture des variables de la classe.
inline
float Inequation::GetA() const
{
  return alpha;
}

inline
float Inequation::GetB() const
{
  return b;
}

inline
float Inequation::GetC() const
{
  return c;
}

inline
float Inequation::GetW() const
{
  return w;
}

inline
bool Inequation::Is_Strict() const
{
  return strict;
}

inline
bool Inequation::Is_Used() const
{
  return used;
}


// M�thode calculant la dimension d'une in�quation.
inline
int Inequation::Get_Dim() const
{	
  if(fabs(alpha)>EPS && fabs(b)>EPS && fabs(c)>EPS)
    return 3;

  else
    if ((fabs(alpha)>EPS && fabs(b)>EPS)||
        (fabs(alpha)>EPS && fabs(c)>EPS)||
        (fabs(b)>EPS && fabs(c)>EPS))
      return 2;

    else
      if(fabs(alpha)>EPS || fabs(b)>EPS || fabs(c)>EPS)
	return 1;

      else
	return 0;
}


// M�thodes permettant l'�criture de variable dans la classe.
inline
void Inequation::SetA(float pa)
{
  alpha=pa;
}

inline
void Inequation::SetB(float pb)
{
  b=pb;
}

inline
void Inequation::SetC(float pc)
{
  c=pc;
}

inline
void Inequation::SetW(float pw)
{
  w=pw;
}

inline
void Inequation::SetAbcw(float pa, float pb, float pc, float pw)
{
  alpha=pa;
  b=pb;
  c=pc;
  w=pw;
}

inline
void Inequation::Set_Strict()
{
  strict = true;
}

inline
void Inequation::Unset_Strict()
{
  strict = false;
}

inline
void Inequation::Set_Used()
{
  used = true;
}

inline
void Inequation::Set_Unused()
{
  used = false;
}


// Méthode permettant de savoir si un point de rep�re 3D v�rifie
// l'inéquation.
inline
bool Inequation::Test_Point (CVertex const & pt) const
{
  if (strict)
  {
    if (alpha*pt.getX() + b*pt.getY() + c*pt.getZ() < w)
      return true;
    else
      return false;
  }
  else
  {
    if (alpha*pt.getX() + b*pt.getY() + c*pt.getZ() <= w + EPS)
      return true;
    else
      return false;
  }
}


// M�thode permettant de savoir si un point de rep�re 3D v�rifie
// l'�quation ax+by+cz = w.
inline
bool Inequation::Test_Point_Eq (CVertex const & pt) const
{
  if (fabs(alpha*pt.getX() + b*pt.getY() + c*pt.getZ() - w) <= EPS)
    return true;

  else
    return false;
}


// M�thode calculant le point d'intersection de trois �quations du rep�re 3D.
inline
CVertex* Inequation::Intersec (Inequation const & i1, Inequation const & i2)
{
  float const epsilon = 0.0001;

  float mat[3][4] = {{alpha, b, c, w},
                     {i1.alpha, i1.b, i1.c, i1.w},
	       	     {i2.alpha, i2.b, i2.c, i2.w}};
  int nbZeros;
  float coef1, coef2;

  for (int i=0; i<3; i++)
    for (int j=0; j<4; j++)
      if (fabs(mat[i][j]) < epsilon)
        mat[i][j] = 0.0;

  // Comptage du nombre de 0 dans la premiere colonne.
  nbZeros = 0;
  for (int i=0; i<3; i++)
    if (mat[i][0] == 0.0)
      nbZeros++;

  // Il n y alpha pas de 0 dans la premiere colonne.
  if (nbZeros == 0)
  {
    coef1 = mat[0][0];
    coef2 = mat[1][0];

    // Elimination du 1er 0.
    for (int i=0; i<4; i++)
      mat[0][i] = mat[0][i]*coef2 - mat[1][i]*coef1;

    coef1 = mat[2][0];

    // Elimination du 2ieme 0.
    for (int i=0; i<4; i++)
      mat[1][i] = mat[1][i]*coef1 - mat[2][i]*coef2;
  }

  // Il y alpha un 0 dans la premiere colonne.	
  else
    if (nbZeros == 1)
    {
      if (mat[0][0] == 0.0)
      {
	coef1 = mat[1][0];
	coef2 = mat[2][0];

	for (int i=0; i<4; i++)
	  mat[1][i] = mat[1][i]*coef2 - mat[2][i]*coef1;
      }
		
      else
	if (mat[1][0] == 0.0)
	{
	  coef1 = mat[0][0];
	  coef2 = mat[2][0];

	  for (int i=0; i<4; i++)
	    mat[0][i] = mat[0][i]*coef2 - mat[2][i]*coef1;
	}
		
	else
	{
	  coef1 = mat[0][0];
	  coef2 = mat[1][0];

	  for (int i=0; i<4; i++)
	    mat[0][i] = mat[0][i]*coef2 - mat[1][i]*coef1;	
	}
      }

      // Comptage du nombre de 0 dans la deuxieme colonne.
      nbZeros = 0;

      for (int i=0; i<3; i++)
	if (mat[i][1] == 0.0)
	  nbZeros++;

      // Il n y alpha pas de 0 dans la deuxieme colonne.
      if (nbZeros == 0)
      {
	if (mat[0][0] == 0.0)
	{
	  coef1 = mat[0][1];

	  if (mat[1][0] == 0.0)
	  {
	    coef2 = mat[1][1];

	    for (int i=1; i<4; i++)
	      mat[1][i] = mat[1][i]*coef1 - mat[0][i]*coef2;	

	    coef2 = mat[2][1];

	    for (int i=0; i<4; i++)
	      mat[2][i] = mat[2][i]*coef1 - mat[0][i]*coef2;	
	  }

	  else
	  {
	    coef2 = mat[2][1];

	    for (int i=1; i<4; i++)
	      mat[2][i] = mat[2][i]*coef1 - mat[0][i]*coef2;

	    coef2 = mat[1][1];

	    for (int i=0; i<4; i++)
	      mat[1][i] = mat[1][i]*coef1 - mat[0][i]*coef2;	
	  }	
	}
		
 	else
	{
	  coef1 = mat[0][1];
	  coef2 = mat[1][1];

	  for (int i=0; i<4; i++)
	    mat[0][i] = mat[0][i]*coef2 - mat[1][i]*coef1;	

	  coef1 = mat[2][1];

 	  for (int i=1; i<4; i++)
	    mat[1][i] = mat[1][i]*coef1 - mat[2][i]*coef2;	
	}
      }

      // Il y alpha un 0 dans la deuxieme colonne.
      else
	if (nbZeros == 1)
	{
	  if (mat[0][1] == 0.0)
	  {
	    if (mat[0][0] == 0.0)
	    {
	      coef1 = mat[1][1];
	      coef2 = mat[2][1];

	      if (mat[1][0] == 0.0)
	      {			
		for (int i=0; i<4; i++)
		  mat[2][i] = mat[2][i]*coef1 - mat[1][i]*coef2;
	      }

	      else
	      {				
		for (int i=0; i<4; i++)
		  mat[1][i] = mat[1][i]*coef2 - mat[2][i]*coef1;
	      }
	    }

	    else
	    {
	      coef1 = mat[2][1];
	      coef2 = mat[1][1];

	      for (int i=1; i<4; i++)
		mat[1][i] = mat[1][i]*coef1 - mat[2][i]*coef2;
	    }
	  }

	  else
	    if (mat[1][1] == 0.0)
	    {
	      if (mat[1][0] == 0.0)
	      {
	        coef1 = mat[0][1];
		coef2 = mat[2][1];

		if (mat[0][0] == 0.0)
		{			
		  for (int i=0; i<4; i++)
		    mat[2][i] = mat[2][i]*coef1 - mat[0][i]*coef2;
		}

		else
		{
		  for (int i=0; i<4; i++)
		    mat[0][i] = mat[0][i]*coef2 - mat[2][i]*coef1;
		}
	      }

	      else
		{
		  coef1 = mat[0][1];
		  coef2 = mat[2][1];

		  for (int i=1; i<4; i++)
		    mat[0][i] = mat[0][i]*coef2 - mat[2][i]*coef1;
		}
	      }

	      else
	      {
		if (mat[2][0] == 0.0)
		{
		  coef1 = mat[0][1];
		  coef2 = mat[1][1];

		  if (mat[0][0] == 0.0)
		  {			
		    for (int i=0; i<4; i++)
		      mat[1][i] = mat[1][i]*coef1 - mat[0][i]*coef2;
		  }

		  else
		  {
		    for (int i=0; i<4; i++)
		      mat[0][i] = mat[0][i]*coef2 - mat[1][i]*coef1;
		  }
		}

		else
		{
		  coef1 = mat[0][1];
		  coef2 = mat[1][1];

		  for (int i=1; i<4; i++)
		    mat[0][i] = mat[0][i]*coef2 - mat[1][i]*coef1;
		}
	      }
	    }

	    // Comptage du nombre de 0 dans la troisieme colonne.
	    nbZeros = 0;

	    for (int i=0; i<3; i++)
	      if (mat[i][2] == 0.0)
		nbZeros++;

	    // Il n y alpha pas de 0 dans la troisieme colonne.
	    if (nbZeros == 0)
	    {
	      if (mat[0][0] == 0.0 && mat[0][1] == 0.0)
	      {
		coef1 = mat[0][2];
		coef2 = mat[1][2];

		for (int i=0; i<4; i++)
		  mat[1][i] = mat[1][i]*coef1 - mat[0][i]*coef2;

		coef2 = mat[2][2];

		for (int i=0; i<4; i++)
		  mat[2][i] = mat[2][i]*coef1 - mat[0][i]*coef2;
	      }

	      else
		if (mat[1][0] == 0.0 && mat[1][1] == 0.0)
		{
		  coef1 = mat[1][2];
		  coef2 = mat[0][2];

		  for (int i=0; i<4; i++)
		    mat[0][i] = mat[0][i]*coef1 - mat[1][i]*coef2;

		  coef2 = mat[2][2];

		  for (int i=0; i<4; i++)
		    mat[2][i] = mat[2][i]*coef1 - mat[1][i]*coef2;
		}

		else
		{
		  coef1 = mat[2][2];
		  coef2 = mat[0][2];

		  for (int i=0; i<4; i++)
		    mat[0][i] = mat[0][i]*coef1 - mat[2][i]*coef2;

		  coef2 = mat[1][2];

		  for (int i=0; i<4; i++)
		    mat[1][i] = mat[1][i]*coef1 - mat[2][i]*coef2;
		}
	      }

	      else
	        if (nbZeros == 1)
		{
		  if (mat[0][2] == 0.0)
		  {
		    coef1 = mat[2][2];
		    coef2 = mat[1][2];

		      if (mat[1][0] == 0.0 && mat[1][1] == 0.0)
		      {
			for (int i=0; i<4; i++)
			  mat[2][i] = mat[2][i]*coef2 - mat[1][i]*coef1;
		      }

		      else
		      {
			for (int i=0; i<4; i++)
			  mat[1][i] = mat[1][i]*coef1 - mat[2][i]*coef2;
		      }
		    }

		    else
		      if (mat[1][2] == 0.0)
		      {
			coef1 = mat[0][2];
			coef2 = mat[2][2];

			if (mat[0][0] == 0.0 && mat[0][1] == 0.0)
			{
			  for (int i=0; i<4; i++)
			    mat[2][i] = mat[2][i]*coef1 - mat[0][i]*coef2;
			}

			else
			{
			  for (int i=0; i<4; i++)
			    mat[0][i] = mat[0][i]*coef2 - mat[2][i]*coef1;
			}
		      }

		      else
		      {
			coef1 = mat[0][2];
			coef2 = mat[1][2];

			if (mat[0][0] == 0.0 && mat[0][1] == 0.0)
			{
			  for (int i=0; i<4; i++)
			    mat[1][i] = mat[1][i]*coef1 - mat[0][i]*coef2;
			}

			else
			{
			  for (int i=0; i<4; i++)
		 	    mat[0][i] = mat[0][i]*coef2 - mat[1][i]*coef1;
			}
		      }
	    	    }
	
  float x, y, z;

  if (mat[0][0] != 0.0)
    x = mat[0][3] / mat[0][0];

  else
    if (mat[1][0] != 0.0)
      x = mat[1][3] / mat[1][0];

    else
      x = mat[2][3] / mat[2][0];
	
  if (mat[0][1] != 0.0)
    y = mat[0][3] / mat[0][1];

  else
    if (mat[1][1] != 0.0)
      y = mat[1][3] / mat[1][1];

    else
      y = mat[2][3] / mat[2][1];
	
  if (mat[0][2] != 0.0)
    z = mat[0][3] / mat[0][2];

  else
    if (mat[1][2] != 0.0)
      z = mat[1][3] / mat[1][2];

    else
      z = mat[2][3] / mat[2][2];

  return new CVertex (x, y, z);
}


// Op�rateur d'�galit� entre deux in�quations.
inline
bool Inequation::operator == (Inequation i) const
{
  bool change = false;

  if (alpha != 0.0 && i.alpha != 0.0)
    {
      i.b = (i.b * alpha)/i.alpha;
      i.c = (i.c * alpha)/i.alpha;
      i.w = (i.w * alpha)/i.alpha;
      change = true;
    }
  else
    if (alpha != 0.0 || i.alpha != 0.0)
      return false;

  if (b != 0.0 && i.b != 0.0)
    {
      if (change && fabs(b - i.b) > EPS)
        return false;
      else
        if (fabs(b - i.b) > EPS)
          {
            i.c = (i.c * b)/i.b;
            i.w = (i.w * b)/i.b;
          }
    }
  else
    if (b != 0.0 || i.b != 0.0)
      return false;

  if (c != 0.0 && i.c != 0.0)
    {
      if (change && fabs(c - i.c) > EPS)
        return false;
      else
        if (fabs(c - i.c) > EPS)
          {
            i.w = (i.w * c)/i.c;
          }
    }
  else
    if (c != 0.0 || i.c != 0.0)
      return false;

  if (fabs(w - i.w) > EPS)
    return false;

  return true;
}


// Op�rateur de diff�rence entre deux in�quations.
inline
bool Inequation::operator != (Inequation i) const
{
  return !(*this == i);
}


// Op�rateur de transfert << et >>.
inline
std::ostream& operator << (std::ostream& s, Inequation const & i)
{
  s << i.GetA() << "\t"
    << i.GetB() << "\t"
    << i.GetC() << "\t\t"
    << i.Is_Strict();

  s << "\t\t" << i.GetW() << "\t\t" << i.Is_Used() << std::endl;

  return s;
}

inline
Inequation& operator >> (std::istream& s, Inequation & i)
{
  float alpha, b, c, w;
  bool strict, used;	

  s >> alpha >> b >> c >> strict >> w >> used;

  i.SetA(alpha);
  i.SetB(b);
  i.SetC(c);
  i.SetW(w);

  if (strict)
    i.Set_Strict();

  else
    i.Unset_Strict();

    if (used)
      i.Set_Used();

    else
      i.Set_Unused();

  return i;
}

#endif
