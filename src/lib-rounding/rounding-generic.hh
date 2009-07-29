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
#ifndef ROUNDING_GENERIC_HH
#define ROUNDING_GENERIC_HH
//******************************************************************************
namespace GMap3d
{
  class CDart;
  class CGMapGeneric;

  class CRoundingGeneric
  {
  public:
    /// Constructeur
    CRoundingGeneric(CGMapGeneric* AMap);

    /// Destructeur
    ~CRoundingGeneric();

    /**
     * Chanfreine la cellule de dimension ACellDim incidente à ADart.
     * ???
     *
     * @param ADart Un brin de la carte
     * @param ACellDim La dimension de la cellule à chanfreiner (0, 1 ou 2)
     * @param AMaxDim Une dimension ???
     */
    void chamferCell(CDart* ADart, int ACellDim, int AMaxDim);

    /**
     * Chanfreine le sommet incident à ADart.
     * ???
     *
     * @param ADart Un brin de la carte
     * @param AMaxDim Une dimension ???
     */
    void chamferVertex(CDart* ADart, int AMaxDim);

    /**
     * Chanfreine l'arête incidente à ADart.
     * ???
     *
     * @param ADart Un brin de la carte
     * @param AMaxDim Une dimension ???
     */
    void chamferEdge(CDart* ADart, int AMaxDim);

  protected:

    /**
     * Crée une chaîne de brins (b1, b2, ..., bn) cousue à ADart
     * ADart et b1 sont cousus par ADimMin+1,
     * b1 et b2 sont cousus par ADimMin+2, ...
     * bn-1 et bn sont cousus par ADimMax.
     *
     * Au total, (ADimMax-ADimMin)-1 brins sont créés.
     *
     * @param ADart Un brin de la carte
     * @param ADimMin Une dimension
     * @param ADimMax Une dimension
     *
     * @precondition 0 <= ADimMin < ADimMax <= 3
     */
    void addDartChain(CDart* ADart, int ADimMin, int ADimMax);

    /**
     * ???
     *
     * @param ADart1 Un brin de la carte
     * @param ADart2 Un brin de la carte
     * @param ADim Une dimension
     * @param ADimMin Une dimension
     * @param ADimMax Une dimension
     *
     * @precondition ADim != ADimMin
     * @precondition ADimMin < ADimMax
     */
    void chamfreinLink(CDart* ADart1, CDart* ADart2,
		       int ADim, int ADimMin, int ADimMax);

    /**
     * Cette méthode est utile pour le chanfreinage classique de sommet.
     *
     * @param ADart Un brin de la carte
     * @param ACellDim Une dimension (0, 1, 2 ou 3)
     * @return Une dimension
     *
     * Retourne ???
     */
    int getVertexRoundingDimension(CDart* ADart, int ACellDim);

    /**
     * Cette méthode est utile pour le chanfreinage classique d'arête.
     *
     * Retourne ???
     *
     * @param ADart Un brin de la carte
     * @param ACellDim Une dimension (0, 1, 2 ou 3)
     * @return Une dimension
     */
    int getEdgeRoundingDimension(CDart* ADart, int ACellDim);

    /**
     * Supprime l'arête incidente à ADart.
     *
     * @param ADart Un brin de la carte
     */
    void removeEdge(CDart* ADart);

  private:
    CGMapGeneric* FMap;
  };
}
//******************************************************************************
#endif // ROUNDING_GENERIC_HH
//******************************************************************************
