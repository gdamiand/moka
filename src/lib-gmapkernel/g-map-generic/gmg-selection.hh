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
 * Marque tous les brins de l'orbite AOrbit incidente à ADart en utilisant
 * la marque numéro AMarkNumber.
 *
 * @param ADart Un brin quelconque
 * @param AOrbit Une orbite
 * @param AMarkNumber Un numéro de marque
 * @return the number of marked darts
 */
unsigned int markOrbit(CDart* ADart, TOrbit AOrbit, int AMarkNumber);

/**
 * Marque un brin sur deux de l'orbite AOrbit incidente à ADart en
 * utilisant la marque numéro AMarkNumber.
 * Attention: Cette méthode ne fonctionne pas si l'orbite est une orbite de
 * bord !
 *
 * @param ADart Un brin quelconque
 * @param AOrbit Une orbite
 * @param AMarkNumber Un numéro de marque
 *
 * @precondition AOrbit n'est pas une orbite de bord
 * @precondition isOrientable(ADart, AOrbit)
 * @postcondition isMarked(ADart, AMarkNumber)
 * @postcondition Soient b un brin de l'orbite (ADart,AOrbit) et i une
 *   dimension (0, 1, 2 ou 3) tels que alpha(b,i) soit lui aussi sur l'orbite
 *   (ADart, AOrbit). On a :
 *     isMarked(      b   , AMarkNumber) !=
 *     isMarked(alpha(b,i), AMarkNumber)
 */
void halfMarkOrbit(CDart* ADart, TOrbit AOrbit, int AMarkNumber);

/**
 * Démarque un brin sur deux de l'orbite AOrbit incidente à ADart en
 * utilisant la marque numéro AMarkNumber.
 * Attention: Cette méthode ne fonctionne pas si l'orbite est une orbite de
 * bord !
 *
 * @param ADart Un brin quelconque
 * @param AOrbit Une orbite
 * @param AMarkNumber Un numéro de marque
 *
 * @precondition AOrbit n'est pas une orbite de bord
 * @precondition isOrientable(ADart, AOrbit)
 * @postcondition !isMarked(ADart, AMarkNumber)
 * @postcondition Soient b un brin de l'orbite (ADart,AOrbit) et i une
 *   dimension (0, 1, 2 ou 3) tels que alpha(b,i) soit lui aussi sur l'orbite
 *   (ADart, AOrbit). On a :
 *     isMarked(      b   , AMarkNumber) !=
 *     isMarked(alpha(b,i), AMarkNumber)
 */
void halfUnmarkOrbit(CDart* ADart, TOrbit AOrbit, int AMarkNumber);

/**
 * Démarque tous les brins de l'orbite AOrbit incidente à ADart pour la
 * marque numéro AMarkNumber.
 *
 * @param ADart Un brin quelconque
 * @param AOrbit Une orbite
 * @param AMarkNumber Un numéro de marque
 */
void unmarkOrbit(CDart* ADart, TOrbit AOrbit, int AMarkNumber);

/**
 * Démarque tous les brins de la carte pour la marque numéro AMarkNumber.
 *
 * @param AMarkNumber Un numéro de marque
 */
void unmarkAll(int AMarkNumber);

/**
 * Marque avec la marque AMarkNumberDest toutes les cellules correspondant
 * à l'orbite AOrbit dont un brin au moins est marqué avec la marque
 * AMarkNumberSrce.
 *
 * @param AOrbit Une orbite quelconque
 * @param AMarkNumberSrce Un numéro de marque
 * @param AMarkNumberDest Un numéro de marque
 * @return Le nombre de cellules marquées
 *
 * @precondition AMarkNumberSrce!=AMarkNumberDest
 */
int markIncidentCells(TOrbit AOrbit,
		      int AMarkNumberSrce, int AMarkNumberDest);

/**
 * Marque entièrement les cellules correspondant à l'orbite AOrbit dont un
 * brin au moins est marqué.
 *
 * @param AOrbit Une orbite quelconque
 * @param AMarkNumber Un numéro de marque
 * @return Le nombre de cellules marquées
 */
int markIncidentCells(TOrbit AOrbit, int AMarkNumber);

/**
 * Marque avec la marque AMarkNumberDest un brin sur deux des cellules
 * correspondant à l'orbite AOrbit dont un brin au moins est marqué avec la
 * marque AMarkNumberSrce.
 * Lorsqu'une cellule non orientable est marquée, elle est totalement
 * sélectionnée.
 * Voir la méthode 'halfMarkOrbit' pour plus d'informations.
 *
 * @param AOrbit Une orbite qui n'est pas une orbite de bord
 * @param AMarkNumberSrce Un numéro de marque
 * @param AMarkNumberDest Un numéro de marque
 *
 * @precondition AOrbit>=ORBIT_SELF && AOrbit<=Orbit_CC
 * @precondition AMarkNumberSrce!=AMarkNumberDest
 */
void halfMarkIncidentCells(TOrbit AOrbit,
			   int AMarkNumberSrce, int AMarkNumberDest);

/**
 * Marque un brin sur deux des cellules correspondant à l'orbite AOrbit
 *  dont un brin au moins est marqué.
 * Lorsqu'une cellule non orientable est marquée, elle est totalement
 * sélectionnée.
 * Voir la méthode 'halfMarkOrbit' pour plus d'informations.
 *
 * @param AOrbit Une orbite qui n'est pas une orbite de bord
 * @param AMarkNumber Un numéro de marque
 *
 * @precondition AOrbit>=ORBIT_SELF && AOrbit<=Orbit_CC
 */
void halfMarkIncidentCells(TOrbit AOrbit, int AMarkNumber);

/**
 * Marque tous les brins de la carte qui sont à l'intérieur d'une région
 * dont le bord est donné par les brins marqués avec la marque AMarkBorder.
 * ADart est un brin situé à l'intérieur de la région.
 * Les brins sont marqués avec la marque AMarkInterior. Les brins du bord ne
 * sont pas marqués.
 *
 * @param ADart Un brin de la carte
 * @param ADimension Une dimension (2 ou 3)
 * @param AMarkBorder Un numéro de marque
 * @param AMarkInterior Un numéro de marque
 */
void markInterior(CDart* ADart, int ADimension,
		  int AMarkBorder, int AMarkInterior = -1);

/**
 * Cette méthode permet de sélectionner une partie du bord d'une cellule en
 * calculant l'intersection entre cette cellule et une autre qui lui est
 * adjacente.
 *
 * @param AMarkNumber1 un numéro de marque
 * @param AMarkNumber2 un numéro de marque différent de AMarkNumber1
 * @param AOrbit1 une orbite quelconque (orbite de sélection)
 * @param AOrbit2 une orbite quelconque (orbite du résultat)
 * @param AMarkResult la marque où est stocké le résultat
 * @param AInitialMarkIncidentCells un booléen indiquant s'il faut
 *   initialiser les marquages
 */
void markIntersection(int AMarkNumber1, int AMarkNumber2,
		      TOrbit AOrbit1, TOrbit AOrbit2,
		      int AMarkResult,
		      bool AInitialMarkIncidentCells = false);

/**
 * Pour chacun des brins de l'orbite déterminée par ADart et AOrbit,
 * positionne la marque AMarkNumberTo sur le même état que la marque
 * AMarkNumberFrom.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markCopy(int AMarkNumberFrom, int AMarkNumberTo,
	      CDart* ADart, TOrbit AOrbit);

/**
 * Pour chacun des brins de la carte,
 * positionne la marque AMarkNumberTo sur le même état que la marque
 * AMarkNumberFrom.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markCopy(int AMarkNumberFrom, int AMarkNumberTo);

/**
 * Pour chacun des brins de la carte,
 * positionne la marque AMarkNumberTo à vrai si la marque AMarkNumberFrom
 * est positionnée à vrai.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markAdd(int AMarkNumberFrom, int AMarkNumberTo);

/**
 * Pour chacun des brins de l'orbite déterminée par ADart et AOrbit,
 * positionne la marque AMarkNumberTo à vrai si la marque AMarkNumberFrom
 * est positionnée à vrai.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markAdd(int AMarkNumberFrom, int AMarkNumberTo,
	     CDart* ADart, TOrbit AOrbit);

/**
 * Pour chacun des brins de la carte,
 * positionne la marque AMarkNumberTo à faux si la marque AMarkNumberFrom est
 * positionnée à faux.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markAnd(int AMarkNumberFrom, int AMarkNumberTo);

/**
 * Pour chacun des brins de l'orbite déterminée par ADart et AOrbit,
 * positionne la marque AMarkNumberTo à faux si la marque AMarkNumberFrom
 * est positionnée à faux.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markAnd(int AMarkNumberFrom, int AMarkNumberTo,
	     CDart* ADart, TOrbit AOrbit);

/**
 * Pour chacun des brins de la carte,
 * positionne la marque AMarkNumberTo à faux si la marque AMarkNumberFrom
 * est positionnée à vrai.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markSub(int AMarkNumberFrom, int AMarkNumberTo);

/**
 * Pour chacun des brins de l'orbite déterminée par ADart et AOrbit,
 * positionne la marque AMarkNumberTo à faux si la marque AMarkNumberFrom
 * est positionnée à vrai.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markSub(int AMarkNumberFrom, int AMarkNumberTo,
	     CDart* ADart, TOrbit AOrbit);

/**
 * Pour chacun des brins de la carte,
 * positionne la marque AMarkNumberTo à vrai si la marque AMarkNumberFrom
 * est positionnée à vrai, puis positionne la marque AMarkNumberFrom à
 * faux.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markMove(int AMarkNumberFrom, int AMarkNumberTo);

/**
 * Pour chacun des brins de l'orbite déterminée par ADart et AOrbit,
 * positionne la marque AMarkNumberTo à vrai si la marque AMarkNumberFrom
 * est positionnée à vrai, puis positionne la marque AMarkNumberFrom à faux.
 *
 * @param AMarkNumberFrom Un numéro de marque
 * @param AMarkNumberTo Un numéro de marque
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 *
 * @precondition AMarkNumberFrom!=AMarkNumberTo
 */
void markMove(int AMarkNumberFrom, int AMarkNumberTo,
	      CDart* ADart, TOrbit AOrbit);

/**
 * Teste si tous les brins de la cellule déterminée par le brin ADart et
 * l'orbite AOrbit sont marqués avec la marque AMarkNumber.
 *
 * @param ADart Un brin quelconque de la carte
 * @param AOrbit Une orbite quelconque
 * @param AMarkNumber Un numéro de marque
 * @return Un booléen
 */
bool isWholeCellMarked(CDart* ADart, TOrbit AOrbit, int AMarkNumber);

/**
 * Cherche dans la carte les cellules correspondant à AOrbit dont au moins
 * un brin est marqué pour le numéro de marque AMarkNumber donné.
 * Pour chacune d'entre elles, un brin marqué est choisi et l'un des
 * paramètres ADart- est positionné sur ce brin.
 * Si plus de 3 cellules sont marquées, seules les trois premières trouvées
 * sont mémorisées et la valeur retournée est supérieure à trois.
 * Si moins de 3 cellules sont marquées, les paramètres non utilisés sont
 * positionnés à NULL.
 * Si ALastMarked ne vaut pas NULL, il désigne un brin prioritaire sur les
 * autres. Si ce brin est marqué, il est placé en première position (ADart1
 * pointe sur lui).
 *
 * @param AOrbit Une orbite quelconque
 * @param AMarkNumber Un numéro de marque
 * @param ALastMarked Un pointeur sur le dernier brin marqué
 * @param ADart1 Une référence sur un pointeur sur un brin
 * @param ADart2 Une référence sur un pointeur sur un brin
 * @param ADart3 Une référence sur un pointeur sur un brin
 * @return Le nombre d'orbites marquées
 */
int getMarkedCells(TOrbit AOrbit, int AMarkNumber,
		   CDart* ALastMarked = NULL,
		   CDart* * ADart1 = NULL,
		   CDart* * ADart2 = NULL,
		   CDart* * ADart3 = NULL);

//******************************************************************************
