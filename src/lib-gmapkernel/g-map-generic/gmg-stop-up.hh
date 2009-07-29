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
// Ce fichier fait partie de "g-map-generic.hh".
//******************************************************************************

public:

/**
 * Bouche le bord de dimension ADim incident au brin ADart avec
 * une cellule de dimension ADart.
 * La fonction est virtuelle afin de pouvoir être surchargée pour gérer les
 * problèmes de plongement. Par exemple, dans le cas de sommets plongés le
 * bouchage d'un 0-bord entraîne la création d'un nouveau sommet
 * topologique.
 *
 * @param ADart Un brin quelconque d'un bord de dimension ADim
 * @param ADim Une dimension
 */
virtual void stopUp(CDart* ADart, int ADim);

/**
 * Bouche les bords de dimension ADim incident aux brins sélectionnés.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension
 * @return Le nombre de bords qui ont été bouchés
 */
int stopUpMarkedBorders(int AMarkNumber, int ADim);

/**
 * Bouche les bords incidents aux brins sélectionnés, en déterminant de
 * manière automatique la dimension des bords.
 *
 * @param AMarkNumber Un numéro de marque
 * @return Le nombre de bords qui ont été bouchés
 */
int intuitiveStopUpMarkedBorders(int AMarkNumber);

//******************************************************************************
