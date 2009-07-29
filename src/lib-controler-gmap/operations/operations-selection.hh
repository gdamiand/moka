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
// Ce fichier fait partie de "controler-gmap-operations.hh".
//******************************************************************************

public:

/// Change de niveau de sélection.
void setSelectionLevel    (int ALevel);
void setNextSelectionLevel(int AIndex=1);

/// Démarque tout les brins pour le niveau de sélection courant.
bool unmarkAllCurrentSelectionLevel();

/// Inverse la marque de tout les brins pour le niveau de sélection courant.
bool negateAllCurrentSelectionLevel();

/// Échange les marques de tout les brins entre les niveaux ALevel1 et ALevel2.
bool swapSelectionLevels(int ALevel1, int ALevel2);

/// Échange les marques du niveau courant avec le niveau suivant.
bool swapCurrentSelectionLevelWithNext();

/// Sélection de tout les brins incidents aux brins sélectionnés
/// (pour l'orbite courante)
bool selectIncidents();

bool selectBorders	   (int ADimension);
bool intuitiveSelectBorders();
bool selectNonPlanarFaces  ();

/// Marquages
bool markIntersection(int AInitialDim, int AFinalDim);
bool markInterior    (int ADimension);

/// Test de matching : cherche si la carte dans le fichier AFilename
/// est un motif de la carte courante. Si oui, sélectionne le matching.
bool findMotifOff(const char* AFilename);
bool findMotifMoka(const char* AFilename);
bool countNumberOfMotifsMoka(const char* AFilename);
bool countNumberOfMotifsOff(const char* AFilename);

//******************************************************************************
