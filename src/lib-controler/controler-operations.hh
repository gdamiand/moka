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
// Ce fichier est une partie de "controler.hh".
//******************************************************************************

public:

// @name Méthodes générales
// @{

/**
 * Retourne si le mode courant permet de faire une opération (exemple couture,
 *  inversion mark...).
 */
virtual bool canApplyOperation(const COperation& AOperation);

/// Exécute l'opération donné en paramètre.
virtual bool applyOperation(const COperation& AOperation);

/// Récupère le dernier message du controleur.
std::string getMessage() const;

/// Fixe le message "AMessage"
void setMessage(const std::string & AMessage);

/// Fixe le message "ANumber AMessage"
void setMessage(int ANumber,  const std::string & AMessage);

/// Fixe le message "ANumber1 ANumber2 AMessage"
void setMessage(int ANumber1, int ANumber2, const std::string & AMessage);

/// Fixe le message "AMessage1 ANumber AMessage2"
void setMessage(const std::string & AMessage1,
		int ANumber, const std::string & AMessage2);

/// Pour enlever les accents (pour l'option NO_ACCENTED_CHARS)
std::string treatAccent(const std::string& AString) const;
//@}

/// Méthodes positionnant le point de vue à l'origine de la scène.
bool lookAtOrigin(TViewId AView);

/// Centre le point de vue aux coordonnees (x,y) de la souris.
bool lookAtMouseClick  (TViewId AView, int x, int y);

// @name Positionnement de l'oeil
// @{
#include "operations-eye-positioning.hh"
// @}

private:
char unaccent(char AChar) const;

//******************************************************************************
