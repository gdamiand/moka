/*
 * lib-controler : Un contrôleur générique de scène 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler
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

//******************************************************************************
#ifndef PARAMETER_DRAWING_HH
#define PARAMETER_DRAWING_HH
//******************************************************************************
#include "parameter.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterDrawing regroupe les paramètres généraux pour
 *  l'affichage :
 *  - couleurs de fond, de lumière
 *  - flags pour indiquer si la lumière est active ou non
 *
 *  Cette classe contient simplement les accesseurs et modificateurs
 *  des différentes variables membres.
 */

class CParameterDrawing : public CParameter
{
public:
  /** @name Constructeur et destructeur
   */

  //@{

  /** Constructeur
   */
  CParameterDrawing(int ANbRef = 0);
  CParameterDrawing(const CParameterDrawing &);
  CParameter * copy() const;

  /** Destructeur
   */
  virtual ~CParameterDrawing();

  //@}

  /** @name Sauvegarde et chargement des paramètres
   */

  //@{

  /** save
   *
   *  Méthode permettant de sauver les valeurs des variables membres.
   */
  virtual void save(std::ostream &);

  /** load
   *
   *  Méthode permettant de récupérer les valeurs des variables membres.
   */
  virtual void load(std::istream &);

  /** reinit
   *
   * Méthode pour réinitialiser avec nos valeurs par defaut.
   */
  virtual void reinit();
  
  //@}
  
  /** @name Accesseurs pour les flags
   */

  //@{
  bool getDisplayLight() const; /// pour activer/désactiver la lumière
  void setDisplayLight(bool);
  void invDisplayLight();
  //@}

  /** @name Accesseurs pour les couleurs des axes Ox, Oy et Oz
   */

  //@{
  float         getCLLight(int AIndice) const;
  const float * getCLLight() const;
  void          setCLLight(int AIndice, float AValue);
  void          setCLLight(float AValue0, float AValue1, float AValue2);
  void          setCLLight(const float ATab[3]);

  float         getCLBackground(int AIndice) const;
  const float * getCLBackground() const;
  void          setCLBackground(int AIndice, float AValue);
  void          setCLBackground(float AValue0, float AValue1, float AValue2);
  void          setCLBackground(const float ATab[3]);
  //@}

  /// Identificateur du type de paramètre.
  virtual int getType() const;

  /// Fonctions amies pour lire et écrire dans un flux.
  friend std::ostream& operator << (std::ostream&, const CParameterDrawing &);
  friend std::istream& operator >> (std::istream&, CParameterDrawing &);
			       
private:

  /// Couleurs
  float FCLLight[3];
  float FCLBackground[3];

  /// Les flags pour activer ou désactiver
  bool FDisplayLight;  /// La lumière
};

#endif // PARAMETER_DRAWING_HH
//******************************************************************************
