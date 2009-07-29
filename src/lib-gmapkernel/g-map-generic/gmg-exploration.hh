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
 * Les différents mouvements que supporte la méthode 'go'.
 */
typedef enum { Forward, Backward, Left, Right } TMovement;

/**
 * *** MÉTHODE À REVOIR ***
 *
 * Cette méthode permet de marquer ou démarquer facilement une cellule
 * adjacente à celle donnée par le couple (ALastDart, AOrbit).
 * Appelée plusieurs fois (voir la méthode 'goAsFarAsPossible'), elle
 * permet de sélectionner des brins le long d'un chemin.
 *
 * Par exemple, pour couper une sphère en deux le long d'un de ses
 * parallèles, il faut préalablement sélectionner les arêtes qui
 * correspondent à ce parallèle.
 * Il s'agît donc de sélectionner certaines arêtes d'un volume. Ces arêtes
 * correspondent à une orbite 02 (deuxième ligne du tableau). Le tableau
 * indique comment se font les déplacements. Dans ce cas précis, pour
 * avancer on recherche alpha1210(ALastDart). Si on désire tourner à
 * droite, c'est alpha120(ALastDart) qui est recherché, etc...
 * Après avoir tourné à gauche ou à droite, on voit donc que pour continuer
 * son trajet il faut repartir dans le sens contraire de celui dans lequel
 * on est arrivé (si avant de tourner on a fait un go(Forward), il faut
 * continuer le trajet en faisant un go(Backward) et inversement).
 *
 * Si ASelect vaut 'vrai' et si un brin de l'orbite n'est pas marqué, alors
 * tous les brins de l'orbite sont marqués avec la marque AMarkNumber et
 * aucun déplacement n'est effectué (quelle que soit la direction donnée).
 * Si ASelect vaut 'vrai' et si tous les brins de l'orbite sont marqués,
 * alors un déplacement est effectué et tous les brins de l'orbite donnée
 * correspondant au brin atteint sont marqués.
 * Si ASelect vaut 'faux', tous les brins de l'orbite donnée sont démarqués
 * puis un déplacement est effectué. Le brin atteint est alors marqué.
 *
 * S'il n'y a pas de brin atteignable, la méthode retourne ALastDart.
 * Sinon, qu'il y ait déplacement ou pas, la méthode retourne le brin
 * atteint (s'il n'y a pas de déplacement ce brin est donc ALastDart).
 * 
 * Tableau récapitulatif:
 *                          DIRECTION
 * ORBITE                   Forward        Backward       Left     Right
 * |                        |              |              |        |
 * | 0--3 (arête /face  )   | 1        0   |   0        1 |    010 | 010
 * | 0-2- (arête /volume)   | 1[2    1]0   |   0[1    2]1 | [2]010 | 010[2]
 * | 0-23 (arête /cc    )   | 1[2[32]1]0   |   0[1[23]2]1 |   [2]0 | 010[2]
 * | 01-- (face  /volume)   | 2      1 01  |  10 1      2 |     01 | 10
 * | 01-3 (face  /cc    )   | 2  [32]1 01  |  10 1[23]  2 |     01 | 10
 * | 012- (volume/cc    )   | 3 2    1 012 | 210 1    2 3 |    012 | 210
 *
 * Les autres orbites ne sont pas supportées (appeler la méthode avec une
 * d'entre elles provoque un résultat imprévisible).
 * Les crochets [] indiquent les orbites qui peuvent être ignorées.
 * Par exemple, dans le cas de sélection de faces dans une composante
 * connexe (orbite 013), si on effectue un go(Forward) alors que
 * alpha2(ALastDart) est 3-libre, c'est alpha2101(ALastDart) qui sera
 * atteint.
 * Par contre, si ALastDart est 2-libre, ou si alpha2(ALastDart) est
 * 1-libre, ou alpha21(ALastDart) est 0-libre, ou etc... la méthode
 * retourne ALastDart.
 *
 * @param ADirection La direction dans laquelle se fait le déplacement
 * @param ALastDart Le brin de départ
 * @param AOrbit L'orbite indiquant quel type de déplacement effectuer
 * @param AMarkNumber Un numéro de marque
 * @param ASelect Indique s'il faut sélectionner ou désélectionner les brins
 * @return Le brin d'arrivée
 *
 * @precondition L'orbite donnée est mentionnée dans le tableau récapitulatif
 */
CDart* go(TMovement ADirection, CDart* ALastDart, TOrbit AOrbit,
	  int AMarkNumber, bool ASelect = true);

/**
 * Cette méthode permet de sélectionner facilement plusieurs cellules
 * adjacentes de la carte.
 * Elle appelle la méthode 'go' autant de fois que possible, c'est-à-dire
 * jusqu'à atteindre un brin déjà rencontré ou un brin à partir duquel il
 * est impossible d'avancer.
 *
 * @param ADirection La direction dans laquelle se fait le déplacement
 * @param ALastDart Le brin de départ
 * @param AOrbit L'orbite indiquant quel type de déplacement effectuer
 * @param AMarkNumber Un numéro de marque
 * @param ASelect Indique s'il faut sélectionner ou désélectionner les brins
 * @return Le brin d'arrivée
 *
 * @precondition L'orbite donnée est mentionnée dans le tableau
 *   récapitulatif de la méthode 'go'
 */
CDart* goAsFarAsPossible(TMovement ADirection,
			 CDart* ALastDart, TOrbit AOrbit,
			 int AMarkNumber, bool ASelect = true);

//******************************************************************************
