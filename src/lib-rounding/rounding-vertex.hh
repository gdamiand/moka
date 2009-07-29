/*
 * lib-rounding : Opérations de chamfreinage.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-rounding
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
#ifndef ROUNDING_VERTEX_HH
#define ROUNDING_VERTEX_HH
//******************************************************************************
#include "rounding-generic.hh"
#include "math-extension.hh"
//******************************************************************************
namespace GMap3d
{
  class CCoverage;
  class CGMapVertex;
  class CRoundingInterface;
  
  class CRoundingVertex : public CRoundingGeneric
  {
    friend class CRoundingInterface;

  public:
    /// Constructeur
    CRoundingVertex(CGMapVertex* AMap);

    /// Destructeur
    ~CRoundingVertex();

    /**
     * Arrondit les sommets dont un brin au moins est marqué.
     *
     * @param AMarkNumber Un numéro de marque
     * @param ADig un booléen indiquant si on ajoute ou retire de la matière
     * @return Le nombre de sommets arrondis
     */
    int roundMarkedVertices(int AMarkNumber, bool ADig);

    /**
     * Arrondit les arêtes dont un brin au moins est marqué.
     * À chaque brin marqué doit être associé un coefficient (TCoordinate) à
     * l'aide du champ directInfo d'indice ADirectInfoParam.
     *
     * Si A3D est vrai, les arêtes sont chanfreinées en dimension 3, sinon elles
     * sont chanfreinées en dimension 2.
     *
     * Si ADig est vrai...
     *
     * @param AMarkNumber Un numéro de marque
     * @param A3D Un booléen indiquant dans quelle dimension doivent être
     *   arrondies les arêtes
     * @param ADig Un booléen ???
     * @param ASetBack Un booléen ???
     * @return Le nombre d'arêtes arrondies
     *
     * @precondition ADig => A3D
     */
    int roundMarkedEdges(int AMarkNumber, bool A3D, bool ADig, bool ASetBack);

  protected:

    /**
     * Récupère la valeur du coefficient d'arrondi associé au brin ADart
     * sur le champ directinfo indiqué.
     */
    TCoordinate getDartRoundingCoef(CDart* ADart, int ADimension) const;

    /**
     * Fixe la valeur du coefficient d'arrondi associé au brin ADart sur
     * le champ directinfo indiqué.
     */
    void setDartRoundingCoef(CDart* ADart, int ADimension,
			     TCoordinate AValue) const;

    // @name Méthodes auxiliaires pour le chanfreinage d'arêtes
    // @{
    void doListExtension(int AMarkNumber, int AMarkInitial, 
  			 int AMarkSelected, int AMarkTreated,
  			 int AMarkVertexToDestroy, CCoverage* ACov);

    void doMinMaxCalcul(int ADirectCoef1, int AMarkSelected, int AMarkTreated,
  			int AMarkInitial, int AMarkMax, bool A3D,
  			CCoverage* ACov);

    void doNewVerticesCalcul(int AMarkSelected, int AMarkTreated,
  			     int AMarkInitial, int AMarkMax,
  			     int ADirectIndexOld, int ADirectIndexNew,
  			     bool ASetback, bool A3D, CCoverage* ACov);

    int doEdgeChamferingInRounding(int AMarkTreated, int AMarkInitial, 
  				   bool ADig,  bool A3D, CCoverage* ACov);

    void doVerticesChamferingInRounding(int AMarkTreated, int AMarkInitial, 
  					bool ADig, bool A3D, CCoverage* ACov);
    // @}

  private:
    CGMapVertex* FMap;

    int FCoefs[2];
  };
}
//******************************************************************************
#endif // ROUNDING_VERTEX_HH
//******************************************************************************
