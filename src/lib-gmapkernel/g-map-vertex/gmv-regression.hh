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
// Ce fichier fait partie de "g-map-vertex.hh".
//******************************************************************************

public:

/**
 * Calcule la droite de régression correspondant aux sommets dont un brin au
 * moins est marqué avec la marque AMarkNumber.
 * Cette droite est décrite par un sommet lui appartenant et un vecteur
 * directeur.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ABarycenter Un sommet de la droite
 * @param ADirection Un vecteur directeur de la droite
 */
void computeRegressionLine(int AMarkNumber,
			   CVertex* ABarycenter, CVertex* ADirection);

/**
 * Calcule le plan de régression correspondant aux sommets dont un brin au
 * moins est marqué avec la marque AMarkNumber.
 * Ce plan alpha pour équation cartésienne alpha*x + b*y + c*z + d = 0.
 * Les paramètres alpha, b, c et d sont placés dans les paramètres A?.
 *
 * @param AMarkNumber Un numéro de marque
 * @param A? Des pointeurs sur les paramètres de l'équation du plan
 */
void computeRegressionPlane(int AMarkNumber,
			    TCoordinate* AA, TCoordinate* AB,
			    TCoordinate* AC, TCoordinate* AD);

//******************************************************************************
