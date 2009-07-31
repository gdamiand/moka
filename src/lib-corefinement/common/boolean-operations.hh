/*
 * lib-corefinement : Opérations de corafinement.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-corefinement
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

#ifndef BOOLEAN_OPERATIONS_HH
#define BOOLEAN_OPERATIONS_HH

#include "g-map-vertex.hh"

/** Classe définissant les opérations booléennes entre deux objets
 *
 * @author Nicolas Guiard
 */

/** @name Type énuméré définissant les différentes opérations possibles
 */

//@{

typedef enum {BO_Union, BO_Intersection,
	      BO_Difference1, BO_Difference2} TBooleanOperation;

/** opérateur d'affichage
 *
 * @param AStream Le flux dans lequel écrire
 * @param ABoolOp La valeur à écrire
 * @return Le flux modifié
 *
 * Ceci permet de définir le format d'affichage pour le type énuméré
 * TBooleanOperation.
 */

std::ostream & operator << (std::ostream & AStream, TBooleanOperation ABoolOp);

//@}

namespace GMap3d
{

class CBooleanOperations
{
public:

  /** @name Constructeurs et destructeur
   */

  //@{

  /** Constructeur par défaut
   *
   * @param AMap La G-Carte contenant les objets
   * @param AObject1 Le premier objet
   * @param AObject2 Le second objet
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle instance de la classe initialisée avec
   * deux objets.
   */

  CBooleanOperations(CGMapVertex * AMap, CDart * AObject1, CDart * AObject2,
		     bool ACalculateOrientation = true, int AVertexDI = -1);

  /** Destructeur
   *
   * Destruction de l'instance.
   */

  virtual ~CBooleanOperations();

  //@}

  /** @name Méthodes publiques
   */

  //@{

  CDart * getObject1() const;
  CDart * getObject2() const;

  void setObject1(CDart * AObject);
  void setObject2(CDart * AObject);

  bool isComputationPossible() const;

  bool computeResults(std::bitset<NB_MARKS> ACopyMarks = 0);

  void markResult(TBooleanOperation AOperation, int AMark);
  void markAllButResult(TBooleanOperation AOperation, int AMark);
  void markResults(TBooleanOperation AOperation, int AKeepMark,
		   int ADiscardMark);

  CDart * getDartFromResult(TBooleanOperation AOperation);
  CDart * getDartOutOfResult(TBooleanOperation AOperation);

  void keepResult(TBooleanOperation AOperation,
		  std::list<CDart*> * ACompoundList = NULL);
  void keepAllButResult(TBooleanOperation AOperation,
			std::list<CDart*> * ACompoundList = NULL);

  //@}

protected:

  /** @name Méthodes protégées
   */

  //@{

  CGMapVertex * getMap() const;

  int getObject1Mark() const;
  int getObject2Mark() const;

  bool calculateOrientation() const;
  int getVertexDI() const;

  void getMarkedCompounds(int AMark, std::list<CDart*> * AList);

  virtual bool corefineObjects(std::bitset<NB_MARKS> ACopyMarks) = 0;
  virtual void extendMarks() = 0;

  //@}

private:

  /** Attributs privés
   */

  //@{

  CGMapVertex * FMap;

  CDart * FObject1;
  CDart * FObject2;

  int FObject1Mark;
  int FObject2Mark;

  bool FCalculateOrientation;
  int FVertexDI;

  //@}
};

} // namespace GMap3d

#endif
