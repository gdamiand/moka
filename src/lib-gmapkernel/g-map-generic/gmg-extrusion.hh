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
 * Teste si la demi-cellule de dimension ADim incidente à ADart
 * (ORBIT_INF[ADim]) peut être extrudée.
 * C'est le cas si tous les brins qui la constituent sont ADim-libres.
 *
 * Si AMarkToExtrude est positif ou nul, il indique les cellules qui
 * doivent être extrudées en parallèle. Ce paramètre est utilisé par la
 * méthode 'canExtrudeByPath'.
 *
 * @param ADart Un brin de la carte
 * @param ADim Une dimension (0, 1 ou 2)
 * @param AMarkToExtrude Un numéro de marque
 * @return Un booléen indiquant si l'extrusion est possible
 */
bool canExtrudeHalfCell(CDart* ADart, int ADim, int AMarkToExtrude = -1);

/**
 * Teste si la cellule de dimension ADim incidente à ADart
 * (ORBIT_CELL[ADim]) peut être extrudée.
 * C'est le cas si tous les brins qui la constituent sont ADim+1-libres ou
 * si tous les brins qui la constituent sont ADim-libres.
 *
 * Si l'extrusion est possible et si au moins un brin de la cellule est
 * ADim-cousu, cela signifie que la cellule va être dupliquée avant
 * l'extrusion. Par exemple pour extruder une face d'un volume, les brins
 * de la face étant 2-cousus il faut créer de nouveaux brins et les
 * 3-coudre à la face avant d'appliquer l'extrusion sur la demi-face
 * nouvellement créée.
 *
 * @param ADart Un brin de la carte
 * @param ADim Une dimension (0, 1 ou 2)
 * @return Un booléen indiquant si l'extrusion est possible
 */
bool canExtrudeCell(CDart* ADart, int ADim);

/**
 * Donne la plus petite dimension D telle que la cellule dimension D
 * incidente à ADart puisse être extrudée.
 * Si l'extrusion est impossible, la méthode retourne la valeur 3.
 *
 * @param ADart Un brin de la carte
 * @return Une dimension d'extrusion (0, 1, 2 ou 3)
 */
int getExtrusionDimension(CDart* ADart);

/**
 * Extrude la cellule de dimension ADim incidente à ADart.
 *
 * Si AMarkExtruded est positif ou nul et si la méthode
 * 'getExtrusionWithAdacentSews' renvoie la valeur 'vrai', la cellule
 * extrudée est cousue aux cellules adjacentes déjà extrudées.
 * Si ADirectInfoIndex est positif ou nul, les cellules adjacentes sont
 * déterminées à partir du champ directInfo correspondant (ce champ
 * mémorise simplement l'image par alpha-ADim de chaque brin).
 * Ceci est particulièrement utile pour la méthode 'extrudeByPath'
 * lorsque le chemin est fermé ou lorsque le brin initial est au milieu
 * du chemin. En effet, dans ce cas là les cellules sont isolées avant
 * extrusion d'où l'impossibilité de retrouver les cellules adjacentes
 * qui ont déjà été extrudées (voir la méthode 'extrudeByPath' pour plus
 * d'informations).
 *
 * Ces cellules de dimension ADim+1 peuvent facilement être repérées
 * car les cellules de dimension ADim à partir desquelles on les alpha
 * obtenues sont marquées avec la marque AMarkExtruded.
 *
 * @param ADart Un brin de la carte
 * @param ADim Une dimension (0, 1 ou 2)
 * @param AExtrusionWithAdjacentSews Un booléen pour indiquer comment
 *   doit s'effectuer l'extrusion. Si vrai', les cellules adjacentes
 *   extrudées en parallèle sont cousues entre elles, sinon elles ne
 *   sont pas cousues entre elles
 * @param AMarkExtruded Un numéro de marque
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @return Un brin de la cellule créée, situé à l'opposé de ADart
 *
 * @precondition canExtrudeCell(ADart, ADim)
 */
CDart* topoExtrude(CDart* ADart, int ADim, bool AExtrusionWithAdjacentSews,
		   int AMarkExtruded = -1, int ADirectInfoIndex = -1);

/**
 * Extrude la cellule de dimension ADim incidente à ADart en s'appuyant sur
 * la méthode 'topoExtrude'.
 *
 * Le paramètre AMarkToExtrude est un numéro de marque indiquant quelles
 * cellules doivent être extrudées. Ce paramètre est utile lors du
 * calcul des plongements.
 *
 * Pour la signification des paramètre AExtrusionWithAdjacentSews et
 * AMarkExtruded, se reporter à la méthode 'topoExtrude'.
 *
 * @param ADart Un brin de la carte
 * @param ADim Une dimension (0, 1 ou 2)
 * @param AExtrusionWithAdjacentSews Un booléen
 * @param AExtrusionCoef Un TCoordinate fixant le coefficient d'extrusion.
 * @param AMarkToExtrude Un numéro de marque
 * @param AMarkExtruded Un numéro de marque
 * @return Un brin de la cellule créée, situé à l'opposé de ADart
 *
 * @precondition canExtrudeCell(ADart, ADim)
 */
virtual CDart* extrudeByNormal(CDart* ADart, int ADim,
			       bool AExtrusionWithAdjacentSews,
			       TCoordinate AExtrusionCoef,
			       int AMarkToExtrude = -1,
			       int AMarkExtruded = -1);

/**
 * Extrude lorsque c'est possible les cellules de dimension ADim qui sont
 * marquées avec la marque AMarkNumber.
 *
 * Cette méthode s'appuie sur la méthode protégée
 * 'extrudeByNormalMarkedCells(int, int, int, bool)'.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (0, 1 ou 2)
 * @param AExtrusionWithAdjacentSews Un booléen
 * @param AExtrusionCoef Un TCoordinate fixant le coefficient d'extrusion.
 * @return Le nombre de cellules extrudées
 */
int extrudeByNormalMarkedCells(int AMarkNumber, int ADim,
			       bool AExtrusionWithAdjacentSews,
			       TCoordinate AExtrusionCoef);

/**
 * Extrude lorsque c'est possible les cellules marquées avec la marque
 * AMarkNumber. La méthode essaie d'abord d'extruder les sommets marqués,
 * puis les arêtes et en enfin les faces.
 *
 * @param AMarkNumber Un numéro de marque
 * @param AExtrusionWithAdjacentSews Un booléen
 * @return Le nombre de cellules extrudées
 */
int intuitiveExtrudeByNormalMarkedCells(int AMarkNumber,
					bool AExtrusionWithAdjacentSews,
					TCoordinate AExtrusionCoef);

/**
 * Teste si l'extrusion de la cellule de dimension ADim incidente à ADart
 * selon le chemin donné par APath est possible.
 *
 * @param ADart Un brin de la carte incident à la cellule à extruder
 * @param ADim Une dimension (1 ou 2)
 * @param APath Un brin de la carte incident au chemin d'extrusion
 * @return Un booléen indiquant si l'extrusion est possible
 */
bool canExtrudeByPath(CDart* ADart, int ADim, CDart* APath);

/**
 * Extrude la cellule de dimension ADim incidente à ADart selon le chemin
 * donné par APath (orbite O1)  en s'appuyant sur la méthode 'topoExtrude'
 * appelée plusieurs fois.
 *
 * Le paramètre ADirectInfoIndex est un indice de champ directInfo dans
 * lequel sont mémorisées les ADim-coutures des brins avant extrusion.
 * Ce paramètre est utile pour la couture des cellules extrudées adjacentes
 * si le chemin est fermé ou si le brin initial est au milieu du chemin.
 * En effet, dans ce cas là la cellule extrudée se retrouve isolée et
 * lorsque ses ex-voisines sont à leur tour extrudées, on ne peut pas
 * déterminer à quelle cellules les cellules issues de l'extrusion doivent
 * être cousues.
 *
 * Pour la signification des paramètre AExtrusionWithAdjacentSews et
 * AMarkExtruded, se reporter à la méthode 'topoExtrude'.
 *
 * @param ADart Un brin de la carte incident à la cellule à extruder
 * @param ADim Une dimension (1 ou 2)
 * @param APath Un brin de la carte incident au chemin d'extrusion
 * @param AExtrusionWithAdjacentSews Un booléen 
 * @param AMarkExtruded Un numéro de marque
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @return Un brin de la dernière cellule créée, situé à l'opposé de ADart
 *
 * @precondition canExtrudeByPath(ADart, ADim, APath)
 */
virtual CDart* extrudeByPath(CDart* ADart, int ADim, CDart* APath,
			     bool AExtrusionWithAdjacentSews,
			     int AMarkExtruded = -1,
			     int ADirectInfoIndex = -1);

/**
 * Extrude lorsque c'est possible selon le chemin donné les cellules de
 * dimension ADim qui sont marquées avec la marque AMarkNumber.
 *
 * Le résultat dépend obtenu de la valeur que donne la méthode
 * 'getExtrusionWithAdjacentSews'.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (1 ou 2)
 * @param APath Un brin de la carte incident au chemin d'extrusion
 * @param AMarkExtruded Un numéro de marque
 * @return Le nombre de cellules extrudées selon le chemin donné
 */
virtual int extrudeByPathMarkedCells(int AMarkNumber, int ADim,
				     CDart* APath,
				     bool AExtrusionWithAdjacentSews);

/**
 * Teste si l'extrusion de la cellule de dimension ADim incidente à ADart
 * autour d'un axe est possible.
 *
 * @param ADart Un brin de la carte incident à la cellule à extruder
 * @param ADim Une dimension (1 ou 2)
 * @param AClosed Un booléen indiquant si la révolution est complète
 *   (angle de 360°)
 * @return Un booléen indiquant si l'extrusion est possible
 */
bool canExtrudeByRevolution(CDart* ADart, int ADim, bool AClosed);

/**
 * Crée un chemin d'extrusion composé de ANbEdges arêtes, ouvert ou fermé
 * selon la valeur de AClosed. Ce chemin est utilisé pour simuler
 * l'extrusion par révolution autour d'un axe.
 *
 * @param AClosed Un booléen indiquant si la révolution est complète
 *   (angle de 360°)
 * @param ANbEdges Le nombre d'arêtes du chemin
 * @return Un brin extrémité du chemin
 */
virtual CDart* createRevolutionPath(bool AClosed, int ANbEdges);

/**
 * Détruit un chemin de révolution créé avec la méthode 'createRevolutionPath'.
 *
 * @param ADart Un brin du chemin de révolution
 *
 * @precondition isIsolatedPolyline(ADart)
 */
void destroyRevolutionPath(CDart* ADart);

/**
 * Extrude la cellule de dimension ADim incidente à ADart selon autour d'un
 * axe. La méthode crée un chemin constitué de ANbEdges arêtes qui est
 * fermé ou ouvert selon la valeur de AClosed puis appelle la méthode
 * 'extrudeByPath'.
 *
 * Pour la signification des paramètres AMarkExtruded et ADirectInfoIndex,
 * se reporter à la méthode 'extrudeByPath'.
 *
 * @param ADart Un brin de la carte incident à la cellule à extruder
 * @param ADim Une dimension (1 ou 2)
 * @param AClosed Un booléen indiquant si la révolution est complète
 *   (angle de 360°)
 * @param ANbEdges Le nombre de sections à créer
 * @param AExtrusionWithAdjacentSews Un booléen 
 * @param AMarkExtruded Un numéro de marque
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @return Un brin de la dernière cellule créée, situé à l'opposé de ADart
 *
 * @precondition canExtrudeByRevolution(ADart, ADim, AClosed)
 * @precondition ANbEdges >= 3
 */
virtual CDart* extrudeByRevolution(CDart* ADart, int ADim,
				   bool AClosed, int ANbEdges,
				   bool AExtrusionWithAdjacentSews,
				   int AMarkExtruded = -1,
				   int ADirectInfoIndex = -1);

/**
 * Extrude par révolution lorsque c'est possible les cellules de
 * dimension ADim qui sont marquées avec la marque AMarkNumber.
 *
 * Le résultat dépend obtenu de la valeur que donne la méthode
 * 'getExtrusionWithAdjacentSews'.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (1 ou 2)
 * @param AClosed Un booléen indiquant si la révolution est complète
 *   (angle de 360°)
 * @param ANbEdges Le nombre de sections à créer
 * @param AMarkExtruded Un numéro de marque
 * @return Le nombre de cellules extrudées
 */
virtual int extrudeByRevolutionMarkedCells(int AMarkNumber, int ADim,
					   bool AClosed, int ANbEdges,
					   bool AExtrusionWithAdjacentSews);

protected:

/**
 * Marque entièrement les cellules de dimension ADim qui doivent être
 * extrudées.
 * Cette méthode ne fait rien d'autre qu'appeler 'markIncidentCells', mais
 * elle peut être surchargée par les classes qui dérivent de CGMapGeneric
 * pour ne marquer que la moitié des cellules à extruder, ceci afin de
 * résoudre les problèmes d'orientation de l'extrusion dans le cadre de
 * l'extrusion selon un vecteur normal.
 *
 * @param ADim Une dimension (0, 1 ou 2)
 * @param AMarkNumberSrce Un numéro de marque source
 * @param AMarkNumberDest Un numéro de marque destination
 */
virtual void markCellsToBeExtrudedByNormal(int ADim,
					   int AMarkNumberSrce,
					   int AMarkNumberDest);

/**
 * Extrude lorsque c'est possible les cellules de dimension ADim qui sont
 * marquées avec la marque AMarkNumber.
 *
 * Le résultat obtenu dépend de la valeur que donne la méthode
 * 'getExtrusionWithAdjacentSews'.
 *
 * Si AMarkTreated est positif ou nul, les cellules extrudées sont marquées
 * comme telles avec la marque AMarkTreated. Ceci permet de ne pas extruder
 * plusieurs fois la même cellule dans des dimensions différentes (voir
 * méthode 'intuitiveExtrudeMarkedCells').
 *
 * Remarque: Comme CGMapGeneric ne traite pas les plongements, aucune
 * vérification sur les brins sélectionnés n'est effectuée.
 * Par exemple, en dimension 2 si un brin d'une face est sélectionné et si
 * sont alpha0 l'est aussi, on ne peut pas effectuer le calcul des
 * plongements car il y alpha ambiguïté sur le sens de la normale de la
 * face à extruder. Cependant, topologiquement il n'y alpha aucun problème.
 * Cette méthode devra donc être surchargée dans les classes dérivant de
 * CGMapGeneric.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (0, 1 ou 2)
 * @param AMarkTreated Un numéro de marque
 * @param AExtrusionWithAdjacentSews Un booléen
 * @param AExtrusionCoef Un TCoordinate fixant le coefficient d'extrusion.
 * @return Le nombre de cellules extrudées
 */
int extrudeByNormalMarkedCells(int AMarkNumber, int ADim, int AMarkTreated,
			       bool AExtrusionWithAdjacentSews,
			       TCoordinate AExtrusionCoef);

//******************************************************************************
