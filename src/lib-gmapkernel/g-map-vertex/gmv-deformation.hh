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
 * Applique la matrice à tous les sommets appartenant à l'orbite AOrbit
 * incidente au brin ADart.
 * Un sommet est modifié si au moins un brin de son orbite sommet appartient
 * à l'orbite AOrbit incidente à ADart.
 * Si ADirectInfoVertex est positif ou nul, la matrice de transformation est
 * appliquée sur le sommet stocké dans le champ directInfo correspondant du
 * brin, et le résultat est placé dans l'attribut sommet (stocké dans la
 * liste d'embeddings).
 * Si ADirectInfoVertex est strictement négatif, la matrice de transformation
 * modifie directement l'attribut sommet.
 *
 * @param AMatrix Une matrice de transformation
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 * @param ADirectInfoVertex Indique où récupérer les coordonnées initiales
 */
void applyMatrix(const CTransformationMatrix & AMatrix, CDart* ADart,
		 TOrbit AOrbit, int ADirectInfoVertex = -1);

/**
 * Applique la matrice à tous les sommets marqués avec la marque AMarkNumber.
 * Un sommet est modifié si au moins un brin de son orbite sommet est marqué.
 * Si ADirectInfoVertex est positif ou nul, la matrice de transformation est
 * appliquée sur le sommet stocké dans le champ directInfo correspondant du
 * brin, et le résultat est placé dans l'attribut sommet (stocké dans la
 * liste d'embeddings).
 * Si ADirectInfoVertex est strictement négatif, la matrice de transformation
 * modifie directement l'attribut sommet.
 *
 * @param AMatrix Une matrice de transformation
 * @param AMarkNumber Un numéro de marque permettant de reconnaître les
 *   sommets sur lesquels il faut appliquer la transformation
 * @param ADirectInfoVertex Indique où récupérer les coordonnées initiales
 */
void applyMatrix(const CTransformationMatrix & AMatrix, int AMarkNumber,
		 int ADirectInfoVertex = -1);

/**
 * Pour chaque sommet dont un brin au moins est marqué avec la marque
 * AMarkNumber, calcule la distance euclidienne entre le plongement de ce
 * sommet et le sommet AVertex puis place le résultat dans le champ
 * directInfo[ADirectInfoIndex] du brin possédant le plongement sommet.
 *
 * Avant d'appeler cette méthode, il convient de réserver de la mémoire
 * pour stocker les résultats. Cette réservation se fait avec la méthode
 * 'CGMapGeneric::allocMemoryToDirectInfo' avec ABytes = sizeof(TCoordinate) et
 * AOrbitUsed = ORBIT_VERTEX.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex Un indice indiquant quel directInfo utiliser
 * @param AVertex Un sommet quelconque de l'espace
 */
void computeDistanceToVertex(int AMarkNumber, int ADirectInfoIndex,
			     const CVertex & AVertex);

/**
 * Pour chaque sommet dont un brin au moins est marqué avec la marque
 * AMarkNumber, calcule la distance euclidienne entre le plongement de ce
 * sommet et la droite donnée par le sommet ALineVertex et le vecteur
 * directeur ALineDirection puis place le résultat dans le champ
 * directInfo[ADirectInfoIndex] du brin possédant le plongement sommet.
 *
 * Avant d'appeler cette méthode, il convient de réserver de la mémoire
 * pour stocker les résultats. Cette réservation se fait avec la méthode
 * 'CGMapGeneric::allocMemoryToDirectInfo' avec ABytes = sizeof(TCoordinate) et
 * AOrbitUsed = ORBIT_VERTEX.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex Un indice indiquant quel directInfo utiliser
 * @param ALineVertex Un sommet quelconque de la droite
 * @param ALineDirection Un vecteur directeur de la droite
 *
 * @precondition ! ALineDirection.isNull()
 */
void computeDistanceToLine(int AMarkNumber, int ADirectInfoIndex,
			   const CVertex & ALineVertex,
			   const CVertex & ALineDirection);

/**
 * Pour chaque sommet dont un brin au moins est marqué avec la marque
 * AMarkNumber, calcule la distance euclidienne entre le plongement de ce
 * sommet et le plan donné par le sommet APlaneVertex et le vecteur
 * normal APlaneNormal puis place le résultat dans le champ
 * directInfo[ADirectInfoIndex] du brin possédant le plongement sommet.
 *
 * Avant d'appeler cette méthode, il convient de réserver de la mémoire
 * pour stocker les résultats. Cette réservation se fait avec la méthode
 * 'CGMapGeneric::allocMemoryToDirectInfo' avec ABytes = sizeof(TCoordinate) et
 * AOrbitUsed = ORBIT_VERTEX.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex Un indice indiquant quel directInfo utiliser
 * @param APlaneVertex Un sommet quelconque du plan
 * @param APlaneNormal Un vecteur normal du plan
 *
 * @precondition ! APlaneNormal.isNull()
 */
void computeDistanceToPlane(int AMarkNumber, int ADirectInfoIndex,
			    const CVertex & APlaneVertex,
			    const CVertex & APlaneNormal);

/**
 * Répartit sur le segment [AMin,AMax] par changement de repère 1d les
 * valeurs (de type TCoordinate) pointées par le champ
 * directInfo[ADirectInfoIndex] des brins porteurs d'un attribut sommet.
 * Les sommets pris en compte sont ceux dont un brin au moins est marqué avec
 * la marque AMarkNumber.
 * Remarque: AMin peut être supérieur à AMax.
 *
 * @param AMarkNumber Un numéro de marque indiquant les sommets sélectionnés
 * @param ADirectInfoIndex Un indice indiquant où sont stockées les valeurs
 * @param AMin La borne inférieure du segment de normalisation
 * @param AMax La borne supérieure du segment de normalisation
 */
void normalizeParameter(int AMarkNumber, int ADirectInfoIndex,
			TCoordinate AMin, TCoordinate AMax);

/**
 * Applique une fonction sur les valeurs (de type TCoordinate) pointées par le
 * champ directInfo[ADirectInfoIndex] des brins porteurs d'un attribut sommet.
 * Les sommets pris en compte sont ceux dont un brin au moins est marqué avec
 * la marque AMarkNumber.
 * Les différents types de fonctions sont répertoriés dans le fichier
 * "math-extension.hh".
 *
 * @param AMarkNumber Un numéro de marque indiquant les sommets sélectionnés
 * @param ADirectInfoIndex Un indice indiquant où sont stockées les valeurs
 * @param AFunctionType Le type de fonction à appliquer
 */
void applyFunctionOnParameter(int AMarkNumber, int ADirectInfoIndex,
			      TFunctionType AFunctionType);

/**
 * Applique une translation sur tous les sommets marqués avec la marque
 * AMarkNumber.
 * Un sommet est modifié si au moins un brin de son orbite sommet est marqué.
 *
 * Si ADirectInfoVertex est positif ou nul, la translation est appliquée sur
 * le sommet stocké dans le champ directInfo correspondant du brin, et le
 * résultat est placé dans l'attribut sommet (stocké dans la liste
 * d'embeddings).
 * Si ADirectInfoVertex est strictement négatif, l'attribut sommet est
 * directement modifié.
 *
 * Si ADirectInfoParam est positif ou nul, la translation est pondérée avec
 * la valeur stockée dans le champ directInfo[ADirectInfoParam].
 *
 * @param AMarkNumber Un numéro de marque permettant de reconnaître les
 *   sommets sur lesquels il faut appliquer la translation
 * @param AVector Le vecteur de translation
 * @param ADirectInfoVertex Indique où récupérer les coordonnées initiales
 * @param ADirectInfoParam Indique où récupérer le paramètre de pondération
 */
void translate(int AMarkNumber, const CVertex & AVector,
	       int ADirectInfoVertex = -1, int ADirectInfoParam = -1);

/**
 * Applique une rotation sur tous les sommets marqués avec la marque
 * AMarkNumber.
 * Un sommet est modifié si au moins un brin de son orbite sommet est marqué.
 *
 * Si ADirectInfoVertex est positif ou nul, la rotation est appliquée sur
 * le sommet stocké dans le champ directInfo correspondant du brin, et le
 * résultat est placé dans l'attribut sommet (stocké dans la liste
 * d'embeddings).
 * Si ADirectInfoVertex est strictement négatif, l'attribut sommet est
 * directement modifié.
 *
 * Si ADirectInfoParam est positif ou nul, l'angle de rotation est pondéré
 * avec la valeur stockée dans le champ directInfo[ADirectInfoParam].
 *
 * @param AMarkNumber Un numéro de marque permettant de reconnaître les
 *   sommets sur lesquels il faut appliquer la rotation
 * @param AAxeVertex Un sommet de l'axe de rotation
 * @param AAxeDirection Un vecteur directeur de l'axe de rotation
 * @param ADirectInfoVertex Indique où récupérer les coordonnées initiales
 * @param ADirectInfoParam Indique où récupérer le paramètre de pondération
 *
 * @precondition !ALineDirection.isNull()
 */
void rotate(int AMarkNumber,
	    const CVertex & AAxeVertex, const CVertex & AAxeDirection,
	    TCoordinate AAngle,
	    int ADirectInfoVertex = -1, int ADirectInfoParam = -1);

/**
 * Applique un étirement sur tous les sommets marqués avec la marque
 * AMarkNumber.
 * Un sommet est modifié si au moins un brin de son orbite sommet est marqué.
 *
 * Si ADirectInfoVertex est positif ou nul, l'étirement est appliqué sur
 * le sommet stocké dans le champ directInfo correspondant du brin, et le
 * résultat est placé dans l'attribut sommet (stocké dans la liste
 * d'embeddings).
 * Si ADirectInfoVertex est strictement négatif, l'attribut sommet est
 * directement modifié.
 *
 * Si ADirectInfoParam est positif ou nul, l'étirement est pondéré avec
 * la valeur stockée dans le champ directInfo[ADirectInfoParam].
 *
 * @param AMarkNumber Un numéro de marque permettant de reconnaître les
 *   sommets sur lesquels il faut appliquer l'étirement
 * @param ACenter Le centre d'étirement
 * @param ACoef Les coefficients d'étirement
 * @param ADirectInfoVertex Indique où récupérer les coordonnées initiales
 * @param ADirectInfoParam Indique où récupérer le paramètre de pondération
 */
void scale(int AMarkNumber,
	   const CVertex & ACenter, const CVertex & ACoef,
	   int ADirectInfoVertex = -1, int ADirectInfoParam = -1);

/**
 * Applique un étirement sur tous les sommets marqués avec la marque
 * AMarkNumber.
 * Pour plus d'informations, voir la méthode 'scale' ci-dessus.
 *
 * Remarque: La différence avec la méthode précédente réside dans le
 * coefficient d'homothétie. Ici ce coefficient est un nombre réel, ce qui
 * correspond à une homothétie avec des coefficients identiques dans les
 * trois dimensions.
 *
 * @param AMarkNumber Un numéro de marque permettant de reconnaître les
 *   sommets sur lesquels il faut appliquer l'étirement
 * @param ACenter Le centre d'étirement
 * @param ACoef Le coefficient d'étirement
 * @param ADirectInfoVertex Indique où récupérer les coordonnées initiales
 * @param ADirectInfoParam Indique où récupérer le paramètre de pondération
 */
void scale(int AMarkNumber,
	   const CVertex & ACenter, TCoordinate ACoef,
	   int ADirectInfoVertex = -1, int ADirectInfoParam = -1);

/**
 * Applique une homothétie de coefficient ACoef selon l'axe déterminé par
 * le sommet AAxeVertex et le vecteur AAxeDirection sur tous les sommets
 * marqués avec la marque AMarkNumber.
 *
 * Si ADirectInfoVertex est positif ou nul, l'homothétie est appliquée sur
 * le sommet stocké dans le champ directInfo correspondant du brin, et le
 * résultat est placé dans l'attribut sommet (stocké dans la liste
 * d'embeddings).
 * Si ADirectInfoVertex est strictement négatif, l'attribut sommet est
 * directement modifié.
 *
 * Si ADirectInfoParam est positif ou nul, l'étirement est pondéré avec
 * la valeur stockée dans le champ directInfo[ADirectInfoParam].
 *
 * @param AMarkNumber Un numéro de marque permettant de reconnaître les
 *   sommets sur lesquels il faut appliquer l'homothétie
 * @param AAxeVertex Un sommet de l'axe d'homothétie
 * @param AAxeDirection Un vecteur direction de l'axe d'homothétie
 * @param ACoef Le coefficient d'homothétie
 * @param ADirectInfoVertex Indique où récupérer les coordonnées initiales
 * @param ADirectInfoParam Indique où récupérer le paramètre de pondération
 *
 * @precondition !AAxeDirection.isNull()
 */
void axialScale(int AMarkNumber,
		const CVertex & AAxeVertex, const CVertex & AAxeDirection,
		TCoordinate ACoef,
		int ADirectInfoVertex = -1, int ADirectInfoParam = -1);

/**
 * Applique une homothétie de coefficient ACoef selon le plan déterminé par
 * le sommet APlaneVertex et le vecteur APlaneNormal sur tous les sommets
 * marqués avec la marque AMarkNumber.
 *
 * Si ADirectInfoVertex est positif ou nul, l'homothétie est appliquée sur
 * le sommet stocké dans le champ directInfo correspondant du brin, et le
 * résultat est placé dans l'attribut sommet (stocké dans la liste
 * d'embeddings).
 * Si ADirectInfoVertex est strictement négatif, l'attribut sommet est
 * directement modifié.
 *
 * Si ADirectInfoParam est positif ou nul, l'étirement est pondéré avec
 * la valeur stockée dans le champ directInfo[ADirectInfoParam].
 *
 * @param AMarkNumber Un numéro de marque permettant de reconnaître les
 *   sommets sur lesquels il faut appliquer l'homothétie
 * @param APlaneVertex Un sommet du plan d'homothétie
 * @param APlaneNormal Un vecteur normal du plan d'homothétie
 * @param ACoef Le coefficient d'homothétie
 * @param ADirectInfoVertex Indique où récupérer les coordonnées initiales
 * @param ADirectInfoParam Indique où récupérer le paramètre de pondération
 *
 * @precondition ! APlaneNormal.isNull()
 */
void planarScale(int AMarkNumber,
		 const CVertex & APlaneVertex, const CVertex & APlaneNormal,
		 TCoordinate ACoef,
		 int ADirectInfoVertex = -1, int ADirectInfoParam = -1);

/**
 * Projette orthogonalement les sommets dont un brin au moins est marqué
 * sur le plan d'équation AA * x + AB * y + AC * z + AD = 0.
 *
 * @param AMarkNumber Un numéro de marque
 * @param A? Les paramètres de l'équation cartésienne du plan
 */
void orthoProjectOnPlane(int AMarkNumber,
			 TCoordinate AA,
			 TCoordinate AB,
			 TCoordinate AC,
			 TCoordinate AD);

//******************************************************************************
