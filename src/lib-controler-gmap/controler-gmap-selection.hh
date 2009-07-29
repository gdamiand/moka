/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
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
// Ce fichier est une partie de "controler-gmap.hh".
//******************************************************************************
private:

  ///  Pour la sélection par boîte englobante : pour sélectionner ou
  ///  désélectionner les brins dans la boite
  bool FDeselectMode;
 
public:
//@{

/** getNbSelectionLevels
 *
 *  @return le nombre de niveaux de sélections
 */
int  getNbSelectionLevels() const;

//@{ Accesseurs (get/set/setNext) au niveau de sélection courant.
int  getSelectionLevel    ()             const;
int  getNextSelectionLevel(int AIndex=1) const;
//@}

//@{ Accesseurs (get/set/setNext) à l'orbite de sélection.
TOrbit getSelectionOrbit    () const;
void   setSelectionOrbit    (TOrbit AOrbit);
void   setNextSelectionOrbit();

bool   getHalfSelectionOrbit() const;
void   setHalfSelectionOrbit(bool ABool);
//@}

//@{ Accesseurs au numéro de marque associé à un niveau de sélection.

/// Le numéro de marque du niveau de sélection courant
int getSelectionMark    ()             const;

/// Le numéro de marque d'un niveau donné
int getSelectionMark	(int ALevel)   const;

/// Le numéro de marque relatif par rapport au niveau courant.
/// Par exemple si AIndex==1, récupère la marque suivant le niveau courant.
int getNextSelectionMark(int AIndex=1) const;
//@}

//@{ Accesseurs aux derniers brins sélectionnés.
CDart* getLastSelectedDart    ()             const;
CDart* getLastSelectedDart    (int ALevel)   const;
CDart* getNextLastSelectedDart(int AIndex=1) const;

void unsetLastSelectedDart    ();
void unsetLastSelectedDart    (int ALevel);
void unsetNextLastSelectedDart(int AIndex=1);
void unsetAllLastSelectedDarts();
//@}

//@{ Pour sélectionner un brin

// Test de la sélection d'un brin
bool isDartSelected    (CDart* ADart)               const;
bool isDartSelected    (CDart* ADart, int ALevel)   const;
bool isDartNextSelected(CDart* ADart, int AIndex=1) const;

// Sélection d'un brin
void selectDart    (CDart* ADart, int ALevel);
void selectDart    (CDart* ADart);
void selectNextDart(CDart* ADart, int AIndex=1);

// Désélection d'un brin
void deselectDart    (CDart* ADart, int ALevel);
void deselectDart    (CDart* ADart);
void deselectNextDart(CDart* ADart, int AIndex=1);

// Pour sélectionner ou désélectionner un brin selon son état actuel.
void toggleDartSelection    (CDart* ADart, int ALevel);
void toggleDartSelection    (CDart* ADart);
void toggleDartNextSelection(CDart* ADart, int AIndex=1);

//@}

//@{ Pour sélectionner une orbite (l'orbite getSelectionOrbit)

// Sélection d'une orbite
void selectOrbit    (CDart* ADart);
void selectOrbit    (CDart* ADart, int ALevel);
void selectNextOrbit(CDart* ADart, int AIndex=1);

// Désélection d'une orbite
void deselectOrbit    (CDart* ADart);
void deselectOrbit    (CDart* ADart, int ALevel);
void deselectNextOrbit(CDart* ADart, int AIndex=1);

// Pour sélectionner ou déséllectionner une orbite selon son état actuel.
void toggleOrbitSelection    (CDart* ADart);
void toggleOrbitSelection    (CDart* ADart, int ALevel);
void toggleOrbitNextSelection(CDart* ADart, int AIndex=1);

//@}

//@{ Récuparation d'un sommet sélectionné / d'un axe dans le niveau suivant
bool takeCenter(CVertex & AResult);
bool takeVector(CVertex & AResult);
//@}

//@{ Sélection par boite englobante
void setModeDeselectionAtStop();
bool getModeDeselectionAtStop() const;

void modeSelectionOperationStop();
//@}

//******************************************************************************
