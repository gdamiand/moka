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
#ifndef PARAMETER_HH
#define PARAMETER_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */
//******************************************************************************
#include <iostream>
#include <list>
//******************************************************************************
/** La classe CParameter définie l'interface de ce que sait faite un paramètre :
 *  1. Sauver et charger ses variables membres et retourner son type
 *     (chaque classe paramètre doit avoir un identifiant différent) ; Se
 *     réinitialiser avec nos valeurs par defaut (en dur dans le fichier
 *      param-default-value.hh) ;
 *  2. Gérer un compteur de références (nombre d'objet ayant un pointeur sur ce
 *     paramètres) afin de s'auto détruire quand ce compteur passe à zéro ;
 *  3. Gérer une liste de CPrecompile qui sont abonnés à ce paramètre. Quand le
 *     paramètre est modifié, il positionne le flag FToUpdate de ces precompiles
 *     qui seront alors mis à jour avant leur affichage.
 */
//******************************************************************************
class CPrecompile;

class CParameter
{

public:

  //@{ Constructeurs et destructeurs.
  CParameter(unsigned int ANbRef = 0);
  CParameter(const CParameter & AParameter);
  virtual ~CParameter();

  /** Equivalent d'un constructeur par recopie, mais en méthode virtuelle pour
   *   le polymorphisme.
   */
  virtual CParameter * copy() const = 0;
  //@}

  //@{ Sauvegarde, restauration, et reinitialisation des paramètres.
  virtual void save(std::ostream &) = 0;
  virtual void load(std::istream &) = 0;
  virtual void reinit()             = 0;
  //@}

  //@{ Identificateur du type de paramètre.
  virtual int getType() const = 0;
  //@}

  //@{ Accès au nombre de références sur le paramètre.
  void         incNbRef(unsigned int AValue = 1);
  void         decNbRef(unsigned int AValue = 1);
  unsigned int getNbRef() const;
  //@}

  //@{ Gestion de la liste des CPrecompile
  virtual void addPrecompileToUpdate   (CPrecompile *);
  virtual void removePrecompileToUpdate(CPrecompile *);

  virtual void putAllNeedToUpdate();
  //@}

private:
  /// Compteur de références des objets contenant ce paramètre
  unsigned int FNbRef;

  /// Liste des CPrecompile abonnés à ce paramètre
  std::list<CPrecompile *> FListPrecompile;
};
//******************************************************************************
#endif // PARAMETER_HH
//******************************************************************************
