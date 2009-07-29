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
// Ce fichier fait partie de "g-map-vertex.hh".
//******************************************************************************

public:

/**
 * Utilise la méthode createMesh1 avec un CMesh1VectorDiver.
 *
 * @param AVector Un vecteur contenant les sommets de la polyline
 * @return Un brin extrémité de la polyline
 */
CDart* createPolyline(const std::vector<CVertex> & AVector);

/**
 * Appelle la méthode CGMapGeneric::createTopoPole puis plonge le sommet
 * central du pôle créé avec l'attribut AVertex.
 *
 * @param An Le nombre de triangles qui composent le pôle
 * @param AVertex Le plongement du sommet central du pôle
 * @return Un brin du sommet central du pôle
 */
CDart* createPole(int An, const CVertex & AVertex);

/**
 * Appelle la méthode CGMapGeneric::createTopoOpenedPole puis plonge le
 * sommet central du pôle créé avec l'attribut AVertex.
 *
 * @param An Le nombre de triangles qui composent le pôle
 * @param AVertex Le plongement du sommet central du pôle
 * @return Un brin 2-libre du sommet central du pôle
 */
CDart* createOpenedPole(int An, const CVertex & AVertex);

/**
 * Crée un polygone régulier centré sur l'origine et de vecteur normal OZ.
 * Le premier sommet de la face créée est aux coordonnées (1/2,0,0),
 * c'est-à-dire que le polygone est inscrit dans un cercle de diamètre 1.
 *
 * Le brin b retourné est celui du premier sommet de la face tel que le
 * plongement de alpha0(b) ait une ordonnée positive.
 *
 * @param An Le nombre d'arêtes du polygone
 * @return Un brin de la face créée
 *
 * @precondition An>=3
 */
CDart* createRegularPolygon(int An);

/**
 * Appelle la méthode CGMapGeneric::createTopoSphere puis plonge sphère créée.
 * La sphère obtenue est centrée sur l'origine, son diamètre vaut 1.
 *
 * Le pôle sud  est aux coordonnées (0,0,-1/2).
 * Le pôle nord est aux coordonnées (0,0,+1/2).
 *
 * Les brins ASouthPole et ANorthPole en sortie sont situés sur le méridien
 * passant par le sommet (+1/2,0,0).
 *
 * @param AMeridians Le nombre (non nul) de méridiens de la sphère
 * @param AParallels Le nombre (non nul) de parallèles de la sphère
 * @param ASouthPole L'adresse d'un pointeur sur brin
 * @param ANorthPole L'adresse d'un pointeur sur brin
 */
void createSphere(int AMeridians, int AParallels,
		  CDart** ASouthPole, CDart** ANorthPole);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createSphere' ci-dessus et
 * retourne le brin correspondant à ASouthPole.
 *
 * @param AMeridians Le nombre (non nul) de méridiens de la sphère
 * @param AParallels Le nombre (non nul) de parallèles de la sphère
 * @return Un brin de la sphère
 */
CDart* createSphere(int AMeridians, int AParallels);

/**
 * Appelle la méthode CGMapGeneric::createTopoCylinder puis plonge le
 * cylindre créé.
 * Le cylindre obtenu est axé sur OX, sa hauteur vaut 1, son rayon 1/2.
 *
 * La face incidente à ABorder1 est dans le plan Z=-1/2.
 * La face incidente à ABorder2 est dans le plan Z=+1/2.
 *
 * Les brins AClose1 et AClose2 en sortie sont situés sur le méridien
 * passant par le sommet (+1/2,0,0).
 *
 * @param AMeridians Le nombre (non nul) de méridiens du cylindre
 * @param AParallels Le nombre (non nul) de parallèles du cylindre
 * @param ABorder1 L'adresse d'un pointeur sur brin
 * @param ABorder2 L'adresse d'un pointeur sur brin
 * @param AClose1 Un booléen indiquant s'il faut fermer le bord incident
 *   à ABorder1
 * @param AClose2 Un booléen indiquant s'il faut fermer le bord incident
 *   à ABorder2
 */
void createCylinder(int AMeridians, int AParallels,
		    CDart** ABorder1, CDart** ABorder2,
		    bool AClose1 = true, bool AClose2 = true);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createCylinder' ci-dessus et
 * retourne le brin correspondant à ABorder1.
 *
 * @param AMeridians Le nombre (non nul) de méridiens du cylindre
 * @param AParallels Le nombre (non nul) de parallèles du cylindre
 * @param AClose1 Un booléen indiquant s'il faut fermer le bord incident
 *   à ABorder1 (voir méthode 'createCylinder' ci-dessus)
 * @param AClose2 Un booléen indiquant s'il faut fermer le bord incident
 *   à ABorder2 (voir méthode 'createCylinder' ci-dessus)
 * @return Un brin du cylindre
 */
CDart* createCylinder(int AMeridians, int AParallels,
		      bool AClose1 = true, bool AClose2 = true);

/**
 * Appelle la méthode CGMapGeneric::createTopoPyramid puis plonge la
 * pyramide créée.
 * La pyramide obtenue est axée sur OZ, sa hauteur vaut 1, le rayon de sa
 * base 1/2.
 *
 * La base (incidente à ABaseDart) est dans le plan Z=-1/2.
 * La pointe (incidente à APoleDart) est aux coordonnées (0,0,+1/2).
 *
 * Les brins ABaseDart et APoleDart en sortie sont situés sur le méridien
 * passant par le sommet (+1/4,0,0).
 *
 * @param AMeridians Le nombre (non nul) de méridiens de la pyramide
 * @param AParallels Le nombre (non nul) de parallèles de la pyramide
 * @param ABaseDart L'adresse d'un pointeur sur brin
 * @param APoleDart L'adresse d'un pointeur sur brin
 * @param ACloseBase Un booléen indiquant s'il faut fermer le bord incident
 *   à ABaseDart
 */
void createPyramid(int AMeridians, int AParallels,
		   CDart** ABaseDart, CDart** APoleDart,
		   bool ACloseBase = true);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createPyramid' ci-dessus et
 * retourne le brin correspondant à ABaseDart.
 *
 * @param AMeridians Le nombre (non nul) de méridiens de la pyramide
 * @param AParallels Le nombre (non nul) de parallèles de la pyramide
 * @param ACloseBase Un booléen indiquant s'il faut fermer le bord incident
 *   à ABaseDart (voir méthode 'createPyramid' ci-dessus)
 * @return Un brin du cylindre
 */

CDart* createPyramid(int AMeridians, int AParallels,
		     bool ACloseBase = true);

/**
 * Appelle la méthode CGMapGeneric::createTopoTorus puis plonge le tore créé.
 * Le tore obtenu est axé sur OZ, c'est-à-dire que l'axe OZ passe dans le
 * trou du tore sans l'intersecter.
 *
 * Le brin AEquator en sortie est situé sur le sommet de coordonnées (+1/2,0,0).
 *
 * On alpha donc les relations suivantes:
 *
 * k = r/R  ,   r+R = 1/2  ,   r = k/2/(1+k)  ,   R = 1/2/(1+k)
 *
 * @param AMeridians Le nombre (non nul) de méridiens du tore
 * @param AParallels Le nombre (non nul) de parallèles du tore
 * @param ARadiusProportion La valeur r/R (petit rayon sur grand grand rayon)
 * @param AEquator L'adresse d'un pointeur sur brin
 */
void createTorus(int AMeridians, int AParallels,
		 TCoordinate ARadiusProportion,
		 CDart** AEquator);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createTorus' ci-dessus et
 * retourne le brin correspondant à AEquator.
 *
 * @param AMeridians Le nombre (non nul) de méridiens du tore
 * @param AParallels Le nombre (non nul) de parallèles du tore
 * @param ARadiusProportion La valeur r/R (petit rayon sur grand grand rayon)
 * @return Un brin du tore
 */
CDart* createTorus(int AMeridians, int AParallels,
		   TCoordinate ARadiusProportion);

/**
 * Appelle la méthode 'CGMapGeneric::createTopoSquareIMeshed' et plonge le
 * carré maillé créé.
 * Le carré est centré sur l'origine et son vecteur normal est OZ.
 * Les coins de la face créée sont aux coordonnées (+-1/2,+-1/2,0).
 *
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param AMeshDimension La dimension du maillage (0 ou 2)
 * @param ASquareCorners Un tableau de dimension 2*2 de pointeurs sur brin
 */
void createSquareIMeshed(int ASx, int ASy, int AMeshDimension,
			 CDart* ASquareCorners[2][2]);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createSquareIMeshed' ci-dessus
 * et retourne le brin correspondant à ASquareCorners[0][0].
 *
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param AMeshDimension La dimension du maillage (0 ou 2)
 */
CDart* createSquareIMeshed(int ASx, int ASy, int AMeshDimension);

/**
 * Appelle la méthode 'CGMapGeneric::createCubeIMeshed' puis plonge le cube
 * créé. Le cube obtenu est centré sur l'origine et son côté vaut 1.
 *
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ASz Le nombre de subdivisions selon la troisième dimension
 * @param AMeshDimension La dimension du maillage (0, 1 ou 2)
 * @param ACreatedFaces Un tableau de dimension 3*2 de booléens
 * @param AFacesCorners Un tableau de dimension 3*2*2*2 de pointeurs sur brin
 */
void createCubeIMeshed(int ASx, int ASy, int ASz,
		       int AMeshDimension,
		       bool ACreatedFaces[3][2],
		       CDart* AFacesCorners[3][2][2][2]);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createCubeIMeshed'
 * ci-dessus et retourne un brin du cube.
 *
 * ATTENTION: Selon les valeurs de AFace--, l'objet créé peut ne pas être
 * connexe !
 *
 * Si l'objet créé est connexe, ADart1 est positionné sur un de ses brins et
 * la valeur NULL est affectée à ADart2.
 * Si l'objet créé n'est pas connexe, ADart1 est positionné sur un brin de la
 * première composante et ADart2 sur un brin de la deuxième composante.
 *
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ASz Le nombre de subdivisions selon la troisième dimension
 * @param AMeshDimension La dimension du maillage (0, 1 ou 2)
 * @param ACreatedFaces Un tableau de dimension 3*2 de booléens
 * @param ADart1 Un brin du cube
 * @param ADart2 Un brin du cube
 */
void createCubeIMeshed(int ASx, int ASy, int ASz,
		       int AMeshDimension,
		       bool ACreatedFaces[3][2],
		       CDart** ADart1, CDart** ADart2);

//******************************************************************************
