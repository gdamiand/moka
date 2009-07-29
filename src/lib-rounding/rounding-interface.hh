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
#ifndef ROUNDING_INTERFACE_HH
#define ROUNDING_INTERFACE_HH
//******************************************************************************
#include "math-extension.hh"
#include "rounding-vertex.hh"

namespace GMap3d
{
  class CDart;
  class CCoverage;
  class CGMapVertex;

  class CRoundingInterface
  {
  public:
    /// Constructeur
    CRoundingInterface(CGMapVertex* AMap);

    /// Destructeur
    ~CRoundingInterface();

    /**
     * Affecte la valeur ACoef au coefficient d'arrondi correspondant aux
     * cellules de dimension ADimension de tous les brins de la carte marqués
     * avec la marque AMarkNumber.
     *
     * @param AMarkNumber Un numéro de marque
     * @param ADimension : 0 (pour les sommets), ou 1 (pour les arêtes)
     * @param ACoef La valeur à affecter
     */
    void setRoundingCoefOfMarkedDarts(int AMarkNumber, int ADimension,
  				      const TCoordinate& ACoef);

    /**
     * Calcule la moyenne des coefficients d'arrondi correspondant aux
     * brins marqués avec la marque AMarkNumber.
     *
     * @param AMarkNumber Un numéro de marque
     * @param ADimension : 0 (pour les sommets), ou 1 (pour les arêtes)
     * @param AAverage La moyenne des coefficients des brins marqués
     * @return true si tous les brins portent le même coefficient
     */
    bool getRoundingCoefOfMarkedDarts(int AMarkNumber, int ADimension,
  				      TCoordinate& AAverage);

    /**
     * Déselectionne tous les brins marqués avec la marque AMarkNumber,
     * puis sélectionne tous les brins correspondant au coefficient d'arrondi
     * suivant le coefficient courant.
     * Le coefficient courant est indiqué par les brins sélectionnés avec la
     * marque AMarkNumber au moment de l'appel de la méthode.
     *
     * @param AMarkNumber un numéro de marque
     * @param ADimensions Une dimension (0 ou 1)
     */
    TCoordinate selectNextRoundingCoef(int AMarkNumber, int ADimension);

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

  private:
    CGMapVertex* FMap;
    CRoundingVertex FRounding;
  };
}
//******************************************************************************
#endif // ROUNDING_INTERFACE_HH
//******************************************************************************
