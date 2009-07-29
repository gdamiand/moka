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
 *  Fichier  : Color.h                                                        *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient la spécification de la classe Color. Cette classe     *
 * permet de représenter une couleur à l'aide de trois nombres à virgule      *
 * flottante correspondants aux trois composantes R, G et B d'une couleur.    *
 *                                                                            *
 *****************************************************************************/


#ifndef COLOR_H
#define COLOR_H



/******************************************************************************
 *                              Classe Color                                  *
 *****************************************************************************/

class Color {

 public:

  // Constructeurs.
  Color();
  Color(float cr, float cg, float cb);
  Color(const Color & C);

  // Méthodes permettant la lecture des composantes R, G et B.
  float Get_R() const;
  float Get_G() const;
  float Get_B() const;

  // Méthodes permettant l'écriture des composantes R, G et B.
  void Set_R(float val);
  void Set_G(float val);
  void Set_B(float val);
  void Set_RGB(float vx, float vy, float vz);

  // Opérateur de transfert << et >>.
  friend std::ostream & operator<<(std::ostream & s,Color const & v);
  friend Color& operator >> (std::istream& s, Color & i);


 private:

  // Variables représentant les composantes R, G et B d'une couleur.
  float r,g,b;

};

/******************************************************************************
 *  Fichier  : Color.inl                                                      *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe Color      *
 *                                                                            *
 *****************************************************************************/


// Constructeurs.
inline
Color::Color()
{
  r = g = b = 0.0;
}

inline
Color::Color(const Color & C)
{
  r = C.r;
  g = C.g;
  b = C.b;
}

inline
Color::Color(float cr, float cg, float cb)
{
  r = cr;
  g = cg;
  b = cb;
}


// Méthodes permettant la lecture des composantes R, G et B.
inline
float Color::Get_R() const
{
  return r;
}

inline
float Color::Get_G() const
{
  return g;
}

inline
float Color::Get_B() const
{
  return b;
}


// Méthodes permettant l'écriture des composantes R, G et B.
inline
void Color::Set_R(float val)
{
  r = val;
}

inline
void Color::Set_G(float val)
{
  g = val;
}

inline
void Color::Set_B(float val)
{
  b = val;
}

inline
void Color::Set_RGB(float vr, float vg, float vb)
{
  r = vr;
  g = vg;
  b = vb;
}


// Opérateur de transfert << et >>.
inline
std::ostream & operator<<(std::ostream & s,Color const & v)
{
  s << v.Get_R() << " " << v.Get_G() << " " << v.Get_B() << std::endl;
  return s;
}


inline
Color& operator >> (std::istream& s, Color & i)
{
  float alpha, b, c;

  s >> alpha >> b >> c;

  i.Set_R(alpha);
  i.Set_G(b);
  i.Set_R(c);

  return i;
}

#endif
