/*
 * lib-mesh : Opérations de maillage et lissage.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-mesh
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
// Ce fichier fait partie de "mesh-generic.hh"
//******************************************************************************

public:

/**
 * Cette méthode prend en paramètre un brin incident à un carré maillé en
 * dimension ADim. Le bord de ce carré est composé de 4 polyarêtes.
 * La méthode marque les arêtes horizontales de ce carré avec la
 * marque AMarkNumberX et les arêtes verticales avec la marque AMarkNumberY.
 *
 * Cette méthode est utilisée par 'meshMarkedSquares', en association avec
 * la méthode 'isFreeTopoSquareIMeshed'.
 *
 * 2==AReturnedDart             AReturnedDart==3
 *              +** --+-- --+-- **+
 *              |     |     |     |
 *              |     |     |     |
 *              +-- --+-- --+-- --+
 *              |     |     |     |
 *              |     |     |     |
 *              +** --+-- --+-- **+
 *            ADart             AReturnedDart==1
 *
 * @param ADim Une dimension (0, 1 ou 2)
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param AMarkNumberX Un numéro de marque pour les arêtes horizontales
 * @param AMarkNumberY Un numéro de marque pour les arêtes verticales
 * @param AReturnedDart Un entier indiquant quel brin retourner
 * @return Un brin situé sur un autre coin du maillage, dépendant de la
 *   valeur de AReturnedDart (voir schéma)
 */
CDart* markTopoSquareIMeshed(int ADim, CDart* ADart, int ASx, int ASy,
			     int AMarkNumberX, int AMarkNumberY,
			     int AReturnedDart = 3);

/**
 * Cette méthode prend en paramètre un brin incident à un cube maillé en
 * dimension ADim. Le bord de ce carré est composé de 12 polyarêtes.
 * La méthode marque les arêtes de ce cube avec les marques AMarkNumberX,
 * AMarkNumberY et AMarkNumberZ.
 *
 * Cette méthode est utilisée par 'meshMarkedCubes', en association avec la
 * méthode 'isFreeTopoCubeIMeshed'.
 *
 * @param ADim Une dimension (0, 1, 2 ou 3)
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ASz Le nombre de subdivisions selon la troisième dimension
 * @param AMarkNumberX Un numéro de marque pour les arêtes de la première
 *   dimension
 * @param AMarkNumberY Un numéro de marque pour les arêtes de la deuxième
 *   dimension
 * @param AMarkNumberZ Un numéro de marque pour les arêtes de la troisième
 *   dimension
 */
void markTopoCubeIMeshed(int ADim, CDart* ADart,
			 int ASx, int ASy, int ASz,
			 int AMarkNumberX,
			 int AMarkNumberY,
			 int AMarkNumberZ);

//******************************************************************************
