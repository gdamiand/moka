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
#ifndef STREAMS_HH
#define STREAMS_HH
//******************************************************************************
#include "math-extension.hh"  // pour la définition du type tcoordinate
#include "alt-stdint.hh"
#include "inline-macro.hh"

#include <iostream>
//******************************************************************************

/**
 * Bibliothèque de fonctions facilitant l'utilisation des flots.
 *
 * @author MOKA TEAM
 */

// @name Conversions tableau de 8 bits <-> octet
// @{

/**
 * Convertit un tableau de 8 booléens en un octet non signé.
 * Les premiers éléments du tableau (0, 1, ...) constituent les bits de poids
 * faibles du résultat.
 * Les derniers éléments du tableau (7, 6, ...) constituent les bits de poids
 * forts du résultat.
 *
 * @param ABoolArray Un tableau de 8 booléens
 * @return L'octet correspondant aux 8 booléens
 */
uint8_t bool2char(bool ABoolArray[/*8*/]);

/**
 * Convertit un octet non signé en un tableau de 8 booléens.
 * Les bits de poids faibles de AChar sont stockés dans les premiers éléments
 * du tableau (0, 1, ...).
 * Les bits de poids forts de AChar sont stockés dans les derniers éléments
 * du tableau (7, 6, ...).
 *
 * @param AChar Un octet (non signé)
 * @param ABoolArray Un tableay de 8 booléens
 */
void char2bool(uint8_t AChar, bool ABoolArray[/*8*/]);

// @}
// @name Lecture et écriture d'éléments en mode binaire ou ascii
// @{

/**
 * Passe en mode mode binaire.
 */
void setBinaryMode();

/**
 * Passe en mode ascii.
 */
void setAsciiMode();

/**
 * Écrit la valeur de ABool dans le flot AStream, en tenant compte du mode de
 * sauvegarde courant (binaire ou ascii).
 *
 * @param AStream Un flot
 * @param ABool Un booléen
 */
void writeBool(std::ostream& AStream, bool ABool);

/**
 * Écrit la valeur de AChar dans le flot AStream, en tenant compte du mode de
 * sauvegarde courant (binaire ou ascii).
 *
 * @param AStream Un flot
 * @param AChar Un entier non signé codé sur 1 octet
 */
void writeChar(std::ostream& AStream, uint8_t AChar);

/**
 * Écrit la valeur de AInt dans le flot AStream, en tenant compte du mode de
 * sauvegarde courant (binaire ou ascii).
 *
 * @param AStream Un flot
 * @param AInt Un entier non signé codé sur 4 octets
 */
void writeInt(std::ostream& AStream, unsigned long int AInt);

/**
 * Écrit la valeur de ACoord dans le flot AStream, en tenant compte du mode de
 * sauvegarde courant (binaire ou ascii).
 *
 * @param AStream Un flot
 * @param ACoord Un flottant
 */
void writeCoord(std::ostream& AStream, const TCoordinate& ACoord);

/**
 * Lit un booléen dans le flot AStream, en tenant compte du mode de chargement
 * courant (binaire ou ascii).
 *
 * @param AStream Un flot
 * @return Un booléen
 */
bool readBool(std::istream& AStream);

/**
 * Lit un entier codé sur 1 octet dans le flot AStream, en tenant compte du
 * mode de chargement courant (binaire ou ascii).
 *
 * @param AStream Un flot
 * @return Un entier non signé codé sur 1 octet
 */
uint8_t readChar(std::istream& AStream);

/**
 * Lit un entier codé sur 4 octets dans le flot AStream, en tenant compte du
 * mode de chargement courant (binaire ou ascii).
 *
 * @param AStream Un flot
 * @return Un entier non signé codé sur 4 octets
 */
unsigned long int readInt(std::istream& AStream);

/**
 * Lit un flottant dans le flot AStream, en tenant compte du mode de chargement
 * courant (binaire ou ascii).
 *
 * @param AStream Un flot
 * @return Un flottant
 */
TCoordinate readCoord(std::istream& AStream);

/**
 * Écrit un caractère corresondant à une tabulation dans le flot si le mode
 * ascii est le mode courant.
 * En mode binaire, cette fonction n'a aucun effet.
 *
 * @param AStream Un flot
 */
void writeTab(std::ostream& AStream);

/**
 * Écrit un caractère correspondant à un espace dans le flot si le mode ascii
 * est le mode courant.
 * En mode binaire, cette fonction n'a aucun effet.
 *
 * @param AStream Un flot
 */
void writeSpc(std::ostream& AStream);

/**
 * Écrit un caractère correspondant à un retour à la ligne dans le flot si le
 * mode ascii est le mode courant.
 * En mode binaire, cette fonction n'a aucun effet.
 *
 * @param AStream Un flot
 */
void writeRet(std::ostream& AStream);

// @}

//******************************************************************************
#include INCLUDE_INLINE("streams.icc")
//******************************************************************************
#endif // STREAMS_HH
//******************************************************************************
