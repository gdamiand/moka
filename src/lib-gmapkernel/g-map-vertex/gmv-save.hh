/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-gmapkernel
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
// Ce fichier fait partie de "g-map-vertex.hh".
//******************************************************************************

public:

/**
 * Numérote les n brins de la carte de 0 à n-1 en utilisant un champ
 * directInfo.
 *
 * @param ADirectInfoIndex Un indice indiquant où placer les numéros des brins
 */
void giveNumberToDarts(int ADirectInfoIndex);

/**
 * Détermine le format du fichier dont le nom est passé en paramètre.
 *
 * @param AFilename Un nom de fichier
 * @return Le format du fichier
 */
static TSaveFormat getFileFormat(const char* AFilename);

/**
 * Retourne la chaîne de caractère placée en début d'un fichier de sauvegarde
 * et permettant la reconnaissance du format utilisé lors de la sauvegarde.
 *
 * @param AFormat Un type de format
 * @return Une chaîne de caractères
 */
static const char* getFileHeader(TSaveFormat AFormat);

/**
 * Sauvegarde le contenu de la carte dans un fichier.
 *
 * @param AFilename Un nom de fichier
 * @param AFormat Un type de format de sauvegarde
 * @return 'vrai' si la sauvegarde s'est bien déroulée
 */
bool save(const char* AFilename, TSaveFormat AFormat=AsciiFormat);

/**
 * Sauvegarde le contenu de la carte dans un flot.
 *
 * @param AStream Un flot dans lequel écrire le contenu de la carte
 * @param AFormat Un type de format de sauvegarde
 * @return 'vrai' si la sauvegarde s'est bien déroulée
 */
virtual bool save(std::ostream & AStream, TSaveFormat AFormat=AsciiFormat);

/**
 * Charge la carte à partir d'un fichier.
 * Les brins qui composent la carte avant le chargement ne sont pas détruits.
 *
 * @param AFilename Un nom de fichier
 * @param AFormat Un type de format de fichier
 * @return Un brin de la carte chargée si le chargement s'est bien
 * déroulé, NULL sinon
 */
CDart* load(const char* AFilename, TSaveFormat AFormat);

/**
 * Charge la carte à partir d'un flot.
 * Les brins qui composent la carte avant le chargement ne sont pas détruits.
 *
 * @param AStream Un flot dans lequel lire le contenu de la carte à charger
 * @param AFormat Un type de format de fichier
 * @return Un brin de la carte chargée si le chargement s'est bien
 * déroulé, NULL sinon
 */
virtual CDart* load(std::istream & AStream, TSaveFormat AFormat);

/**
 * Charge la carte à partir d'un fichier en détectant automatiquement
 * le format..
 * Les brins qui composent la carte avant le chargement ne sont pas détruits.
 *
 * @param AFilename Un nom de fichier
 * @return Un brin de la carte chargée si le chargement s'est bien
 * déroulé, NULL sinon
 */
CDart* load(const char* AFilename);

//******************************************************************************
