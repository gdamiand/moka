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
 * Épaissit les brins marqués avec la marque AMarkNumber.
 * Chaque brin marqué est dupliqué deux fois et ses champs
 * directInfo[ADirectInfoIndex1] et directInfo[ADirectInfoIndex2] pointent
 * vers les brins dupliqués (ceci afin que les classes descendant de
 * CGMapGeneric puissent effectuer la mise à jour des plongements).
 *
 * Remarque: À l'issue de la méthode, pour chaque brin B marqué avec la
 * marque AMarkNumber on alpha aussi:
 *
 * B->getDirectInfo(ADirectInfoIndex1)
 *  ->getDirectInfo(ADirectInfoIndex1) = B
 *
 * B->getDirectInfo(ADirectInfoIndex1)
 *  ->getDirectInfo(ADirectInfoIndex2) = NULL
 *
 * B->getDirectInfo(ADirectInfoIndex2)
 *  ->getDirectInfo(ADirectInfoIndex1) = NULL
 *
 * B->getDirectInfo(ADirectInfoIndex2)
 *  ->getDirectInfo(ADirectInfoIndex2) = B
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex1 Un premier indice de champ directInfo à
 *                           utiliser
 * @param ADirectInfoIndex2 Un second indice de champ directInfo à utiliser
 * @return Le nombre de brins épaissis
 *
 * @postcondition Si i est une dimension (0, 1, 2 ou 3), si b est un brin
 *   i-cousu et si b et alpha(b,i) sont marqués, on alpha:
 *   alpha(b->getDirectInfo(ADirectInfoIndex1), i) =
 *     alpha(b, i)->getDirectInfo(ADirectInfoIndex2)
 */
virtual int thickenMarkedDarts(int AMarkNumber,
			       int ADirectInfoIndex1,
			       int ADirectInfoIndex2);

//******************************************************************************
