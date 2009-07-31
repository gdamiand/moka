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

#ifndef BOOLEAN_OPERATIONS_2D_HH
#define BOOLEAN_OPERATIONS_2D_HH

#include "boolean-operations.hh"

/** Classe définissant éfinissant les opérations booléennes entre deux objets 3D
 *
 * @author Nicolas Guiard
 */

namespace GMap3d
{

  class CBooleanOperations2d : public CBooleanOperations
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

    CBooleanOperations2d(CGMapVertex * AMap, CDart * AObject1, CDart * AObject2,
			 bool ACalculateOrientation = true, int AVertexDI = -1);

    /** Destructeur
     *
     * Destruction de l'instance.
     */

    ~CBooleanOperations2d();

    //@}

    /** @name Méthodes publiques
     */

    //@{

    //@}

  protected:

    /** @name Méthodes protégées
     */

    //@{

    virtual bool corefineObjects(std::bitset<NB_MARKS> ACopyMarks);
    virtual void extendMarks();

    //@}

  private:

    /** Attributs privés
     */

    //@{

    //@}
  };

} // namespace GMap3d

#endif
