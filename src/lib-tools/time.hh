/*
 * lib-tools : Divers outils autour de lib-gmapkernel.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-tools
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

#ifndef TIME_HH
#define TIME_HH

#ifdef _WINDOWS
#include <windows.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

#include <ctime>
#include <iostream>

#include "inline-macro.hh"

/** Classe définissant un temps avec une précision en millisecondes.
 *
 * @author Nicolas Guiard
 */

class CTime
{
public:

  /** @name Constructeurs
   */

  //@{

  /** Constructeur par défaut
   *
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle instance de la classe initialisée
   * avec un temps correspondant à l'heure courante.
   */

  CTime();

#ifdef _WINDOWS

  /** Constructeur avec initialisation
   *
   * @param ASec Un temps en seconde
   * @param AMilliSec Les millisecondes
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle instance de la classe initialisée
   * avec un temps de ASec secondes et AMilliSec millisecondes.
   */

  CTime(long ASec, long AMilliSec = 0);

#else // _WINDOWS

  /** Constructeur avec initialisation
   *
   * @param ASec Un temps en seconde
   * @param AMicroSec Les microsecondes
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle instance de la classe initialisée
   * avec un temps de ASec secondes et AMicroSec microsecondes.
   */

  CTime(long ASec, long AMicroSec = 0);

#endif // _WINDOWS

  //@}

  /** @name Méthodes publiques
   */

  //@{

#ifdef _WINDOWS

  /** setTime
   *
   * @param ASec Un temps en seconde
   * @param AMilliSec Les millisecondes
   *
   * Cette méthode permet de modifier le temps courant en lui affectant un
   * temps de ASec secondes et AMilliSec millisecondes.
   */

  void setTime(long ASec, long AMilliSec = 0);

  /** getTime
   *
   * @param ASec Le temps en seconde
   * @param AMilliSec Les microsecondes
   *
   * Cette méthode permet de récupérer le temps courant en le stockant dans les
   * paramètres ASec et AMilliSec.
   */

  void getTime(long * ASec, long * AMilliSec) const;

#else // _WINDOWS

  /** setTime
   *
   * @param ASec Un temps en seconde
   * @param AMicroSec Les microsecondes
   *
   * Cette méthode permet de modifier le temps courant en lui affectant un
   * temps de ASec secondes et AMicroSec microsecondes.
   */

  void setTime(long ASec, long AMicroSec = 0);

  /** getTime
   *
   * @param ASec Le temps en seconde
   * @param AMicroSec Les microsecondes
   *
   * Cette méthode permet de récupérer le temps courant en le stockant dans les
   * paramètres ASec et AMicroSec.
   */

  void getTime(long * ASec, long * AMicroSec) const;

#endif // _WINDOWS

  /** updateTime
   *
   * Cette méthode permet de récupérer l'heure courante.
   */

  void updateTime();

  /** displayTime
   *
   * @param AStream Le flot dans lequel afficher le temps
   *
   * Cette méthode permet d'afficher le temps.
   */

  void displayTime(std::ostream & AStream, bool ADisplayAll = true) const;

  /** operator +
   *
   * @param ATime Le temps à ajouter
   * @return Le résultat de l'addition
   *
   * Cette opérateur permet de calculer la somme de deux temps.
   */

  CTime operator + (const CTime & ATime) const;

  /** operator -
   *
   * @param ATime Le temps à soustraire
   * @return Le résultat de la soustraction
   *
   * Cette opérateur permet de calculer la différence entre de deux temps.
   */

  CTime operator - (const CTime & ATime) const;

  /** operator +=
   *
   * @param ATime Le temps à ajouter
   * @return Le nouveau temps
   *
   * Cette opérateur permet d'ajouter un temps au temps courant.
   */

  const CTime & operator += (const CTime & ATime);

  /** operator -=
   *
   * @param ATime Le temps à soustraire
   * @return Le nouveau temps
   *
   * Cette opérateur permet de soustraire un temps au temps courant.
   */

  const CTime & operator -= (const CTime & ATime);
  
  //@}

private:

  /** @name Attributs privés
   */

  //@{

  /** FTime
   * La structure "C" contenant les informations sur le temps.
   */

#ifdef _WINDOWS
  struct _timeb FTime;
#else
  struct timeval FTime;
#endif

  //@}
};

std::ostream & operator << (std::ostream & AStream,
				      const CTime & ATime);
  
#include INCLUDE_INLINE("time.icc")

#endif
