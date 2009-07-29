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
 * Calcule la plus grande dimension pour laquelle ADart est cousu avec un
 * autre brin. Si le brin n'est cousu ni par alpha0, ni par alpha1, ni par
 * alpha2, ni par alpha3, la valeur retournée est -1.
 *
 * Remarque: Cette méthode n'est plus utilisée par les méthodes
 * intuitiveIsolateMarkedCells et intuitiveMergeMarkedCells.
 *
 * @param ADart Un brin de la carte
 * @return La plus grande dimension pour laquelle le brin n'est pas libre
 *
 * @postcondition La valeur retournée vaut 3, 2, 1, 0 ou -1.
 */
int getMergeDimension(CDart* ADart);

/**
 * Teste si les deux cellules de dimension ADim désignées par les brins
 * ADart1 et ADart2 peuvent être fusionnées.
 *
 * @param ADart1 Un pointeur sur un premier brin
 * @param ADart2 Un pointeur sur un deuxième brin
 * @param ADim Une dimension
 * @return Un booléen indiquant si les deux cellules peuvent être fusionnées.
 */
bool canMerge(CDart* ADart1, CDart* ADart2, int ADim);

/**
 * Fusionne les deux cellules de dimension ADim désignées par les brins
 * ADart1 et ADart2.
 *
 * @param ADart1 Un pointeur sur un premier brin
 * @param ADart2 Un pointeur sur un deuxième brin
 * @param ADim Une dimension
 * @param ADeleteDarts Un booléen indiquant s'il faut supprimer les brins
 *   correspondant à la cellule de dimension ADim-1 située entre les
 *   cellules de dimension ADim incidentes à ADart1 et ADart2
 *
 * @precondition canMerge(ADart1, ADart2, ADim)
 */
void merge(CDart* ADart1, CDart* ADart2, int ADim, bool ADeleteDarts = false);

/**
 * Isole lorsque cela est possible les cellules de dimension ADim en
 * fusionnant entre elles les cellules de dimension ADim+1 qui leurs sont
 * incidentes.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (0, 1 ou 2)
 * @param ADeleteDarts Un booléen indiquant si les cellules isolées doivent
 *   être détruites
 * @return Le nombre de cellules isolées
 */
int isolateMarkedCells(int AMarkNumber, int ADim,
		       bool ADeleteDarts = false);

/**
 * Isole lorsque cela est possible les cellules dont un brin au moins est
 * marqué.
 * La méthode procède en 3 passes, avec priorité aux cellules de dimension
 * élevée (faces).
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADeleteDarts Un booléen indiquant si les cellules isolées doivent
 *   être détruites
 * @return Le nombre de cellules isolées
 */
int intuitiveIsolateMarkedCells(int AMarkNumber, bool ADeleteDarts = false);

/**
 * Fusionne lorsque cela est possible (avec leur cellule incidente) les
 * cellules dont un brin au moins est marqué.
 * Les cellules de dimension ADim-1 qui doivent être isolées sont
 * déterminées par la méthode protégée 'markCellsToBeIsolated'.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (1, 2 ou 3)
 * @param ADeleteDarts Un booléen indiquant si la fusion doit se faire en
 *   supprimant des brins
 * @return Le nombre de fusions effectuées
 */
int mergeMarkedCells(int AMarkNumber, int ADim,
		     bool ADeleteDarts = false);

/**
 * Fusionne lorsque cela est possible (avec leurs cellule incidente) les
 * cellules dont un brin au moins est marqué.
 * Les fusions sont faites en 3 passes, avec priorité aux fusions de
 * cellules de dimension élevée (volumes).
 * Lors de chaque passe, les cellules qui doivent être isolées sont
 * déterminées par la méthode protégée 'markCellsToBeIsolated'.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADeleteDarts Un booléen indiquant si la fusion doit se faire en
 *   supprimant des brins
 * @return Le nombre de fusions effectuées
 */
int intuitiveMergeMarkedCells(int AMarkNumber, bool ADeleteDarts = false);

protected:

/**
 * Cette méthode auxiliaire isole (uniquement lorsque cela est possible si
 * AVerif=='vrai') les cellules de dimension ADim dont un brin au moins est
 * sélectionné, c'est-à-dire qu'elle fusionne les cellules de dimension
 * ADim+1 incidentes aux cellules de dimension ADim marquées.
 * Lorsqu'une cellule est isolée, elle est marquée comme traitée avec la
 * marque AMarkTreated (si AMarkTreated>=0).
 *
 * Si AVerif=='faux', les cellules marquées doivent impérativement pouvoir
 * être isolées.
 *
 * La méthode est utilisée par:
 *              isolateMarkedCells(int, int, bool),
 *     intuitiveIsolateMarkedCells(int,      bool),
 *                mergeMarkedCells(int, int, bool) et
 *       intuitiveMergeMarkedCells(int,      bool).
 *
 * @param AMarkNumber Un numéro de marque indiquant les brins sélectionnés
 * @param ADim La dimension des cellules qui doivent être isolées
 * @param ADeleteDarts Un booléen indiquant si les cellules doivent être
 *   supprimées une fois isolées
 * @param AVerif Un booléen indiquant si pour chaque cellule marquée il
 *   faut vérifier si l'opération d'isolement est possible
 * @param AMarkTreated Un numéro de marque indiquant (si positif ou nul)
 *   les brins déjà traités et dont il ne faut donc pas se préoccuper
 * @return Le nombre de cellules isolées
 *
 * @precondition AMarkTreated!=AMarkNumber
 * @precondition AVerif=='faux' ou
 *   toutes les cellules marquées peuvent être isolées
 */
int isolateMarkedCells(int AMarkNumber, int ADim, bool ADeleteDarts,
		       bool AVerif, int AMarkTreated = -1);

/**
 * Marque avec la marque AMarkDestination tous les brins de chaque cellule
 * de dimension ADim qui peut être isolée (c'est-à-dire qui est ADim-cousue
 * et de degré inférieur ou égal à 2) et qui est incidente à des cellules
 * de dimension ADim+1 dont un brin au moins est marqué avec la marque
 * AMarkSource.
 * Voir le corps de la méthode pour plus de détail.
 *
 * La méthode est utilisée par:
 *                mergeMarkedCells(int, int, bool) et
 *       intuitiveMergeMarkedCells(int,      bool).
 *
 * @param AMarkSource Un numéro de marque
 * @param ADim Une dimension (celle des cellules à isoler)
 * @param AMarkDestination Un numéro de marque
 * @param AMarkTreated Un numéro de marque indiquant (si positif ou nul)
 *   les brins déjà traités, et dont il ne faut donc pas se préoccuper
 *
 * @precondition Aucun brin n'est marqué avec la marque AMarkDestination
 * @precondition 0 <= ADim <= 2
 */
void markCellsToBeIsolated(int AMarkSource, int ADim, int AMarkDestination,
			   int AMarkTreated = -1);

//******************************************************************************
