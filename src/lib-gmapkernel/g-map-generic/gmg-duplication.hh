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
 * Duplique les brins marqués et respecte dans la mesure du possible les
 * coutures.
 * Si ADirectInfoIndex est positif ou nul, chaque brin dupliqué est pointé
 * par le brin à partir duquel il a été créé (par le biais du champ
 * directInfo correspondant).
 *
 * Lorsqu'un champ A-Sew vaut 'faux', aucune couture de dimension A-Sew
 * n'est effectuée entre les brins créés.
 *
 * @param AMarkNumber Un numéro de marque
 * @param AMap La carte dans laquelle créer la copie
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @param ADuplicateEmbeddings Un booléen indiquant s'il faut ou non
 *   dupliquer les plongements des brins
 * @param A-Sew Des booléens indiquant quelles coutures doivent être
 *   préservées
 * @return Le nombre de brins dupliqués
 */
int duplicateMarkedDarts(int AMarkNumber, CGMapGeneric* AMap=NULL,
			 int ADirectInfoIndex = -1,
			 bool ADuplicateEmbeddings = true,
			 bool A0Sew = true, bool A1Sew = true,
			 bool A2Sew = true, bool A3Sew = true);

/**
 * Duplique les brins correspondant à l'orbite donnée en dupliquant les
 * plongements et préservant les coutures qui peuvent l'être.
 *
 * Si ADirectInfoIndex est positif ou nul, chaque brin dupliqué est pointé
 * par le brin à partir duquel il a été créé (par le biais du champ
 * directInfo correspondant).
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 * @param AMap La carte dans laquelle créer la copie
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @param ADuplicateEmbeddings Un booléen indiquant s'il faut ou non
 *   dupliquer les plongements des brins
 * @return Le brin dupliqué correspondant à ADart
 */
CDart* duplicateDarts(CDart* ADart, TOrbit AOrbit,
		      CGMapGeneric* AMap=NULL,
		      int ADirectInfoIndex = -1,
		      bool ADuplicateEmbeddings = true);

protected:

/**
 * Coud les brins dupliqués entre eux.
 * Les brins traités sont ceux parcourus qui sont marqués avec la marque
 * AMarkNumber.
 *
 * @param ACoverage Un itérateur quelconque
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @param AMap La carte dans laquelle créer la copie
 * @param A-Sew Des booléens indiquant quelles coutures doivent être
 *        traitées
 */
void sewDuplicatedDarts(CCoverage* ACoverage,
			int AMarkNumber, int ADirectInfoIndex,
			CGMapGeneric* AMap,
			bool A0Sew = true, bool A1Sew = true,
			bool A2Sew = true, bool A3Sew = true);

/**
 * Duplique les plongements des brins dupliqués.
 *
 * @param ACoverage Un itérateur quelconque
 * @param AMarkInitial Un numéro de marque
 * @param AMarkDuplicated Un numéro de marque
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @param AMap La carte dans laquelle créer la copie
 */
void duplicateEmbeddingsOfDuplicatedDarts(CCoverage* ACoverage,
					  int AMarkInitial,
					  int AMarkDuplicated,
					  int ADirectInfoIndex,
					  CGMapGeneric* AMap);

//******************************************************************************
