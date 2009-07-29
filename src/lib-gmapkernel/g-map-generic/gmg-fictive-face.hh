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

// Décale la face incidente à ADart pour qu'elle fasse le tour de la face
// par l'autre coté. AFictiveFaceMark  la marque pour les faces fictives,
// et ADeleteMark une marque pour les brins à détruire.
virtual void shiftOneFictiveFace (CDart* ADart,
				  int AFictiveFaceMark,
				  int ADeleteMark);

// Décale la face incidente à ADart le long d'une arête réelle pendante.
// AFictiveFaceMark  la marque pour les faces fictives,
// et ADeleteMark une marque pour les brins à détruire.
virtual void shiftOneFictiveFaceAlongDanglingEdge(CDart* ADart,
						  int AFictiveFaceMark,
						  int ADeleteMark );


// ADart est un brin d'une face réelle incident à une arête de degré réel 2.
// Décale toute les faces fictive incidente à cette arête.
// AFictiveFaceMark  la marque pour les faces fictives,
// et ADeleteMark une marque pour les brins à détruire.
void shiftAllFictiveFaces(CDart* ADart, int AFictiveFaceMark,
			  int ADeleteMark );

/**
 * Retourne vrai ssi la face fictive incidente au brin ADart
 * peut être décallée sans entraîner la création de non-variété
 * AFictiveFaceMark  la marque pour les faces fictives,
 */
bool canShiftOneFictiveFace( CDart* ADart, int AFictiveFaceMark );

/**
 * Retourne vrai ssi les faces fictives incidente à l'arête désignée
 * par ADart peuvent être décallées sans entraîner la création de non-variétés
 * AFictiveFaceMark  la marque pour les faces fictives,
 */
bool canShiftAllFictiveFaces( CDart* ADart, int AFictiveFaceMark );

  /**
 *  @param ADart Un brin de la carte
 *  @param AResDart  l'adresse d'un brin : paramètre retour
 *  @param AFictiveFaceMark la marque pour les faces fictives
 *  @return 0 ssi l'arête est incidente qu'a des faces fictives
 *            (cas de degré réel 0) ;
 *          1 ssi une seule face réelle est incidente à l'arête et
 *                si l'arête de cette face réelle est pendante.
 *                Dans ce cas, *AResDart contiendra un brin de la face réelle.
 *          2 ssi l'arête est de degrée réel 2 (cad incidente à 2 faces réelles
 *                distinctes). Dans ce cas, *AResDart un brin de ces
 *                faces réelles incidentes à l'arête ;
 *          3 ssi le degré réel est > 2 ou de degré réel 1 et l'arête n'est
 *                pas pendante.
 */
int findEdgeType(CDart* ADart, CDart** AResDart, int AFictiveFaceMark);    

//******************************************************************************
