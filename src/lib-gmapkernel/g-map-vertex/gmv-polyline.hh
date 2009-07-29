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
 * Prolonge une polyligne (en rajoutant une arête à son bout).
 * Si ADart vaut NULL, crée un premier brin d'une nouvelle polyligne.
 *
 * @param ADart Un brin constituant le bout d'une polyligne
 * @param AVertex Le plongement du sommet à rajouter au bout de la polyligne
 */
CDart* prolongPolyline(CDart* ADart, const CVertex & AVertex);

/**
 * Teste si une face est plane.
 *
 * @param ADart Un brin quelconque de la carte
 * @return 'vrai' si tous les sommets de la face sont situés sur un même plan
 */
bool isPlanarPolyline(CDart* ADart);

/**
 * Marque toutes les faces non planaires de la carte.
 *
 * @param AMarkNumber Un numéro de marque
 * @return 'vrai' si une face non plane au moins alpha été trouvée
 */
bool markNonPlanarPolylines(int AMarkNumber);

//******************************************************************************
