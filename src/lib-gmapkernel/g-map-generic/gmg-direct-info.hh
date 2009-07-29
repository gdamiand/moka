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
 * Alloue ABytes octets au champ directInfo[ADirectInfoIndex] des brins
 * marqués avec la marque AMarkNumber.
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, l'allocation n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est marqué.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param ABytes Le nombre d'octets à allouer
 * @param AOrbitUsed Une orbite quelconque
 */
void allocMemoryToDirectInfo(int AMarkNumber, int ADirectInfoIndex,
			     int ABytes, TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Alloue ABytes octets au champ directInfo[ADirectInfoIndex] des brins
 * correspondant à l'orbite donnée par le couple (ADart, AOrbit).
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, l'allocation n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est sur l'orbite (ADart, AOrbit).
 *
 * @param ADart Un brin quelconque de la carte
 * @param AOrbit Une orbite
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param ABytes Le nombre d'octets à allouer
 * @param AOrbitUsed Une orbite quelconque
 */
void allocMemoryToDirectInfo(CDart* ADart, TOrbit AOrbit,
			     int ADirectInfoIndex,
			     int ABytes, TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Libère l'espace mémoire affecté au champ directInfo[ADirectInfoIndex]
 * des brins marqués avec la marque AMarkNumber.
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, l'allocation n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est marqué.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param AOrbitUsed Une orbite quelconque
 */
void freeMemoryFromDirectInfo(int AMarkNumber, int ADirectInfoIndex,
			      TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Libère l'espace mémoire affecté au champ directInfo[ADirectInfoIndex]
 * des brins marqués avec la marque AMarkNumber.
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, la libération n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est marqué.
 *
 * @param ADart Un brin quelconque de la carte
 * @param AOrbit Une orbite
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param AOrbitUsed Une orbite quelconque
 */
void freeMemoryFromDirectInfo(CDart* ADart, TOrbit AOrbit,
			      int ADirectInfoIndex,
			      TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Pour chaque brin b de la carte, copie la valeur de l'image par alpha
 * AAlphaIndex de b dans son champ direcInfo d'indice ADirectInfoIndex.
 *
 * Remarque: La méthode 'getDirectInfoAsDart' permet de récupérer aisément
 * les valeurs sauvées.
 *
 * @param AAlphaIndex Une dimension (0, 1, 2 ou 3)
 * @param ADirectInfoIndex Un indice de champ directInfo
 */
void saveAlphaInDirectInfo(int AAlphaIndex, int ADirectInfoIndex);

//******************************************************************************
