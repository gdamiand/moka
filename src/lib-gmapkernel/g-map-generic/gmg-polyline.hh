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
 * Teste si le brin ADart appartient à une polyligne isolée, c'est-à-dire
 * si tous les brins de la face incidente à ADart sont 2 et 3-libres.
 *
 * @param ADart Un brin de la carte
 * @return 'vrai' si ADart appartient à une polyligne
 */
bool isIsolatedPolyline(CDart* ADart);

/**
 * Teste si le brin ADart appartient à une polyligne dont tous les brins
 * sont 0 et 1-cousus.
 * Remarque: Certains brins de la polyligne peuvent être 2 ou 3-cousus.
 *
 * @param ADart Un brin de la carte
 * @return 'vrai' si ADart appartient à une face fermée
 */
bool isClosedPolyline(CDart* ADart);

/**
 * Calcule le nombre de sommets qui composent la face incidente à ADart.
 *
 * @param ADart Un brin de la carte
 * @return Un nombre de sommets
 */
int getNbPolylineVertices(CDart* ADart);

/**
 * Calcule le nombre d'arêtes qui composent la face incidente à ADart.
 *
 * Si AConsider0FreeDartsAsEdges vaut 'vrai' et si ADart est incident à une
 * face ouverte, les éventuels brins 0-libres situés au bout de cette face
 * sont comptabilisés comme des arêtes.
 *
 * Si AConsider0FreeDartsAsEdges vaut 'faux', les éventuels brins 0-libres
 * situés au bout d'une face ouverte ne sont pas considérés dans le
 * décompte du nombre d'arêtes.
 *
 * @param ADart Un brin de la carte
 * @param AConsider0FreeDartsAsEdges Un booléen indiquant comment
 *   considérer les brins 0-libres
 * @return Un nombre d'arêtes
 */
int getNbPolylineEdges(CDart* ADart, bool AConsider0FreeDartsAsEdges = true);

/**
 * Prolonge une polyligne (en rajoutant une arête à son bout).
 * Si ADart vaut NULL, crée un premier brin d'une nouvelle polyligne.
 *
 * @param ADart Un brin constituant le bout d'une polyligne existante
 * @return Le nouveau bout de la polyligne
 *
 * @precondition isIsolatedPolyline(CDart)
 */
CDart* prolongPolyline(CDart* ADart);

/**
 * Détruit la dernière arête (dont le brin 0-libre est donné par ADart)
 * d'une polyligne.
 *
 * @param ADart Le bout d'une polyligne
 * @return Le nouveau bout de la polyligne
 *
 * @precondition isIsolatedPolyline(ADart)
 */
CDart* shortenPolyline(CDart* ADart);

/**
 * Ferme une polyligne pour former une face.
 *
 * @param ADart Un brin quelconque d'une polyligne existante
 * @return Un brin de l'arête créée pour fermer la polyligne
 *
 * @precondition isIsolatedPolyline(CDart)
 */
CDart* closePolyline(CDart* ADart);

//******************************************************************************
