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
 * @param ADart Un pointeur sur un brin
 * @param ADim Une dimension
 * @return Un booléen indiquant si la cellule peut être contractée.
 */
bool canContract(CDart* ADart, int ADim);

/**
 * Contracte la cellule de dimension ADim désignées par le brin ADart.
 *
 * @param ADart Un pointeur sur brin
 * @param ADim Une dimension
 * @param ADeleteDarts Un booléen indiquant s'il faut supprimer les brins
 *   correspondant à la cellule contractée.
 *
 * @precondition canContract(ADart, ADim)
 */
void contract(CDart* ADart, int ADim, bool ADeleteDarts = false);

/**
 * Contracte lorsque cela est possible les cellules dont un brin au moins
 * est marqué.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (1, 2 ou 3)
 * @param ADeleteDarts Un booléen indiquant si la contraction doit se faire
 *   en supprimant des brins
 * @return Le nombre de contractions effectuées
 */
int contractMarkedCells(int AMarkNumber, int ADim,
			bool ADeleteDarts = false);

//******************************************************************************
