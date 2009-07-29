/*
 * lib-mesh : Opérations de maillage et lissage.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-mesh
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
// Ce fichier fait partie de "mesh-vertex.hh"
//******************************************************************************

public:

/**
 * Récupère les plongements des 4 sommets appartenant à 3 arêtes consécutives
 * de la carte dont ADart est sur l'arête centrale.
 * AVertexArray est donc un paramètre en sortie.
 *
 * Si ADart est sur une polyligne, AVertexArray[0] reçoit le plongement de
 * alpha10(ADart), AVertexArray[1] celui de ADart, AVertexArray[2] celui de
 * alpha0(ADart) et AVertexArray[2] celui de alpha010(ADart).
 *
 * Remarque: La manière dont les 2 points de contrôle situés aux extrémités
 * sont calculés dépend de la dimension du maillage sur lequel se trouve ADart.
 * Par exemple, si ADart est sur une grille, AVertexExtremity[0] reçoit le
 *  plongement de alpha1210(ADart).
 *
 * Si ADirectInfoAlpha0 est positif ou nul, les images par alpha0 des brins
 * sont retrouvées à l'aide de ce champ. Cela est utile par exemple pour
 * lisser une arête dont les arêtes adjacentes sont déjà 1-maillées.
 *
 * @param ADart Un brin de la carte
 * @param AVertexArray Un pointeur sur 4 sommets
 * @param ADirectInfoAlpha0 Un indice de champ directInfo
 */
void get4ControlPoints(CDart* ADart, CVertex* AVertexArray,
		       int ADirectInfoAlpha0 = -1);

/**
 * Récupère les plongements des 16 sommets appartenant à la grille 3*3
 * dont ADart est sur la face centrale.
 *
 * Si ADirectInfoAlpha0 est positif ou nul, les images par alpha0 des brins
 * sont retrouvées à l'aide de ce champ. Cela est utile par exemple pour
 * lisser un carré qui est déjà 1-maillé.
 *
 * @param ADart Un brin de la carte
 * @param AVertexArray Un pointeur sur 4*4 sommets
 * @param ADirectInfoAlpha0 Un indice de champ directInfo
 */
void get16ControlPoints(CDart* ADart, CVertex** AVertexArray,
			int ADirectInfoAlpha0 = -1);

/**
 * Récupère les plongements des 64 sommets appartenant à la grille 3*3*3
 * dont ADart est sur le cube central.
 *
 * Si ADirectInfoAlpha0 est positif ou nul, les images par alpha0 des brins
 * sont retrouvées à l'aide de ce champ. Cela est utile par exemple pour
 * lisser un cube qui est déjà 2-maillé.
 *
 * @param ADart Un brin de la carte
 * @param AVertexArray Un pointeur sur 4*4*4 sommets
 * @param ADirectInfoAlpha0 Un indice de champ directInfo
 */
void get64ControlPoints(CDart* ADart, CVertex*** AVertexArray,
			int ADirectInfoAlpha0 = -1);

//******************************************************************************
/// @name Méthodes permettant de récupérer les plongements de cellules maillées

/**
 * Parcourt le maillage 1d en partant de ADart et fait pointer les éléments
 * du tableau AVertexTable sur les plongements des sommets rencontrés.
 *
 * Si AAlready2MeshedSquare vaut 'faux', le passage d'un sommet à son
 * successeur se fait en utilisant la méthode 'CGMapGeneric::alpha01', sinon il
 * se fait en utilisant la méthode 'CGMapGeneric::alpha0121'.
 *
 * Cette méthode est utile pour récupérer les plongements des contours d'un
 * carré 1 ou 2-maillé. Elle est utilisée par la méthode 'mesh2'.
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions du maillage 1d
 * @param AVertexTable Un tableau de pointeurs sur sommets
 * @param AAlready2MeshedFace Un booléen indiquant l'état de l'arête
 * @return Le brin situé à l'autre extrémité du maillage par rapport à ADart
 *
 * @precondition AVertexTable est de dimension ASx+1
 */
CDart* getMesh1Dive(CDart* ADart, int ASx,
		    const CVertex** AVertexTable /*[ASx+1]*/,
		    bool AAlready2MeshedSquare = false);

/**
 * Parcourt le maillage 2d en partant de ADart et fait pointer les éléments
 * du tableau AVertexTable sur les plongements des sommets rencontrés.
 *
 * Si AAlready3MeshedCube vaut 'faux', le passage d'un sommet à son
 * successeur se fait en utilisant la méthode 'CGMapGeneric::alpha0121', sinon
 * il se fait en utilisant la méthode 'CGMapGeneric::alpha012321'.
 *
 * Cette méthode est utile pour récupérer les plongements des contours d'un
 * cube 2 ou 3-maillé. Elle est utilisée par la méthode 'mesh3'.
 *
 * 2==AReturnedDart             AReturnedDart==3
 *              +** --+-- --+-- **+
 *              |     |     |     |
 *              |     |     |     |
 *              +-- --+-- --+-- --+
 *              |     |     |     |
 *              |     |     |     |
 *              +** --+-- --+-- **+
 *            ADart             AReturnedDart==1
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions du maillage selon la 1ère dimension
 * @param ASy Le nombre de subdivisions du maillage selon la 2ème dimension
 * @param AVertexTable Un tableau de pointeurs sur sommets
 * @param AAlready3MeshedCube Un booléen indiquant l'état de la face
 * @param AReturnedDart Un entier indiquant quel brin retourner
 * @return Un brin situé sur un autre coin du maillage, dépendant de la
 *   valeur de AReturnedDart (voir schéma)
 *
 * @precondition AVertexTable est de dimension (ASx+1)*(ASy+1)
 */
CDart* getMesh2Dive(CDart* ADart, int ASx, int ASy,
		    const CVertex*** AVertexTable /*[ASx+1][ASy+1]*/,
		    bool AAlready3MeshedCube = false,
		    int AReturnedDart = 3);

// @}

//******************************************************************************
