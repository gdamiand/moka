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
 * Extrude la cellule de dimension ADim incidente à ADart.
 * Si AMarkToExtrude est positive ou nulle, cette marque indique les cellules
 * qui doivent être extrudées. Selon que les cellules voisines de la cellule
 * en cours doivent être extrudées ou pas et selon la valeur que renvoie la
 * méthode 'CGMapGeneric::getExtrusionWithAdacentSews', les plongements ne
 * sont pas calculés de la même manière.
 *
 * La méthode appelle 'CGMapGeneric::extrudeByNormal' puis calcule les
 * plongements des sommets topologiques créés.
 *
 * Pour la signification du paramètre AMarkToExtrude, se reporter à la
 * méthode 'CGMapGeneric::extrudeByNormal'.
 * Pour la signification des paramètres AMarkExtruded et
 * AExtrusionWithAdjacentSews, se reporter à la méthode
 * 'CGMapGeneric::topoExtrude'.
 *
 * @param ADart Un brin de la carte
 * @param ADim Une dimension (0, 1 ou 2)
 * @param AExtrusionWithAdjacentSews Un booléen
 * @param AExtrusionCoef Un TCoordinate fixant le coefficient d'extrusion.
 *   Ce coefficient est utilisé pour pondérer le vecteur d'extrusion.
 * @param AMarkToExtrude Une marque indiquant les cellules à extruder
 * @param AMarkExtruded Une marque indiquant les cellules déjà extrudées
 * @return Un brin de la cellule créée, situé à l'opposé de ADart
 *
 * @precondition canExtrudeCell(ADart, ADim)
 */
CDart* extrudeByNormal(CDart* ADart, int ADim,
		       bool AExtrusionWithAdjacentSews,
		       TCoordinate AExtrusionCoef,
		       int AMarkToExtrude = -1, int AMarkExtruded = -1);

/**
 * Extrude la cellule de dimension ADim incidente à ADart selon le chemin
 * donné par APath (orbite O1).
 *
 * La méthode appelle 'CGMapGeneric::extrudeByPath' puis calcule les
 * plongements des sommets topologiques créés.
 *
 * Pour la signification du paramètre AMarkExtruded, se reporter à la
 * méthode 'CGMapGeneric::topoExtrude'.
 *
 * Pour la signification du paramètre ADirectInfoIndex, se reporter à la
 * méthode 'CGMapGeneric::extrudeByPath'.
 *
 * Si APonderationPath est différent de NULL, une homothétie est appliquée
 * sur chaque section de l'objet obtenu après extrusion. Chaque homothétie
 * est centrée sur le sommet du chemin d'extrusion lui correspondant.
 * APonderationDart désigne un chemin dont les sommets (plus précisément la
 * valeur donnée par 'CVertex::getY' appliquée sur ces sommets) donnent les
 * coefficients d'homothétie.
 *
 * Remarque: Le nombre d'arêtes du chemin peut être différent du nombre
 * d'arêtes de la fonction de pondération. Si c'est le cas, le paramètre
 * de pondération est calculé par interpolation linéaire.
 *
 * Remarque: Si la fonction de pondération s'annule pour une valeur donnée,
 * tous les sommets de la section correspondante sont confondus. Cette
 * section réduite à un sommet se propage alors jusqu'à la fin de
 * l'extrusion.
 *
 * @param ADart Un brin de la carte incident à la cellule à extruder
 * @param ADim Une dimension (1 ou 2)
 * @param APath Un brin de la carte incident au chemin d'extrusion
 * @param APonderationPath Un brin de la carte
 * @param AExtrusionWithAdjacentSews Un booléen 
 * @param ARotateExtrudedCells Un booléen indiquant si les objets
 *   extrudés doivent être orientés
 * @param AScaleExtrudedCells Un booléen indiquant si les objets
 *   extrudés doivent être redimensionnés
 * @param AMarkExtruded Une marque indiquant les cellules déjà extrudées
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @return Un brin de la dernière cellule créée, situé à l'opposé de ADart
 *
 * @precondition canExtrudeByPath(ADart, ADim, APath)
 * @precondition APonderationPath==NULL || ! isClosedPolyline(APonderationPath)
 */
CDart* extrudeByPath(CDart* ADart, int ADim, CDart* APath,
		     bool AExtrusionWithAdjacentSews,
		     bool ARotateExtrudedCells,
		     bool AScaleExtrudedCells,
		     CDart* APonderationPath = NULL,
		     int AMarkExtruded = -1,
		     int ADirectInfoIndex = -1);

/**
 * Surcharge la méthode 'CGMapGeneric::extrudeByPathMarkedCells'.
 * Les cellules sont éventuellement positionnées perpendiculairement
 * au chemin avant d'effectuer l'extrusion (voir méthodes
 * 'setExtrusionInitialPosition' et 'setExtrusionInitialDirection').
 *
 * Attention: si 'getExtrusionInitialDirection' retourne la valeur 'vrai',
 * le vecteur normal de chaque cellule à extruder est calculé, ce qui
 * implique qu'un brin sur deux au maximum pour chaque cellule doit
 * être sélectionné pour éviter toute ambiguïté.
 *
 * Pour la signification des paramètres APonderationDart,ARotateExtrudedCells
 * et AScaleExtrudedCells, se reporter à la méthode 'extrudeByPath'.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (1 ou 2)
 * @param APath Un brin de la carte incident au chemin d'extrusion
 * @param AExtrusionWithAdjacentSews Un booléen
 * @param AInitialTranslateExtrudedCells Un entier indiquant si les
 *   objets extrudés doivent être translatés sur le premier
 *   sommet du chemin. 0 : Aucune translation. 1 : Les objets
 *   extrudés sont initialement translatés sur le chemin.
 *   2 : Le chemin est initialement déplacé sur le barycentre
 *   des cellules à extruder. 
 * @param AInitialRotateExtrudedCells Un entier indiquant comment
 *   les objets à extruder et le chemin doivent être orientés
 *   l'un par rapport à l'autre avant l'extrusion. 0 : Aucune
 *   rotation. 1 : Les objets extrudés sont initialement orientés
 *   perpendiculairement à la première arête du chemin. 2 : Le
 *   chemin est initialement orienté de manière à être perpendiculaire
 *   aux cellules à extruder.
 * @param ARotateExtrudedCells Un booléen
 * @param AScaleExtrudedCells Un booléen
 * @param APonderationPath Un brin de la carte
 * @return Le nombre de cellules extrudées selon le chemin donné
 */
int extrudeByPathMarkedCells(int AMarkNumber, int ADim, CDart* APath,
			     bool AExtrusionWithAdjacentSews,
			     int AInitialTranslateExtrudedCells,
			     int AInitialRotateExtrudedCells,
			     bool ARotateExtrudedCells,
			     bool AScaleExtrudedCells,
			     CDart* APonderationPath = NULL);

/**
 * Crée un chemin d'extrusion composé de ANbEdges arêtes, ouvert ou fermé
 * selon la valeur de AAngle. Ce chemin est utilisé pour simuler l'extrusion
 * par révolution autour d'un axe.
 *
 * @param AAxeVertex Un sommet de l'axe de révolution
 * @param AAxeDirection Un vecteur directeur de l'axe de révolution
 * @param AFirstVertex La position du 1er sommet du chemin
 * @param AAngle L'angle de révolution (compris entre -360 et +360)
 * @param ANbEdges Le nombre d'arêtes du chemin
 * @return Un brin extrémité du chemin
 *
 * @precondition !AAxeDirection.isNull()
 * @precondition ANbEdges >= 3
 */
CDart* createRevolutionPath(const CVertex & AAxeVertex,
			    const CVertex & AAxeDirection,
			    const CVertex & AFirstVertex,
			    TCoordinate AAngle, int ANbEdges);

/**
 * Extrude la cellule de dimension ADim incidente à ADart selon autour d'un
 * axe. La méthode crée un chemin constitué de ANbEdges arêtes qui est fermé
 * ou ouvert selon la valeur de AAngle puis appelle la méthode
 * 'extrudeByPath'.
 *
 * Pour la signification des paramètres AMarkExtruded, ADirectInfoIndex,
 * APonderationPath, AExtrusionWithAdjacentSews, ARotateExtrudedCells,
 * AScaleExtrudedCells se reporter à la méthode 'extrudeByPath'.
 *
 * @param ADart Un brin de la carte incident à la cellule à extruder
 * @param ADim Une dimension (1 ou 2)
 * @param AAxeVertex Un sommet de l'axe de révolution
 * @param AAxeDirection Un vecteur directeur de l'axe de révolution
 * @param AFirstVertex La position du barycentre des cellules à extruder
 * @param AAngle L'angle de révolution (compris entre -360 et +360)
 * @param ANbEdges Le nombre de sections à créer
 * @param AExtrusionWithAdjacentSews Un booléen
 * @param ARotateExtrudedCells Un booléen
 * @param AScaleExtrudedCells Un booléen
 * @param APonderationPath Un brin de la carte
 * @param AMarkExtruded Un numéro de marque
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @return Un brin de la dernière cellule créée, situé à l'opposé de ADart
 *
 * @precondition canExtrudeByRevolution(ADart, ADim, closed),
 *   avec closed = vrai si AAngle/360 est une valeur entière
 * @precondition !AAxeDirection.isNull()
 * @precondition ANbEdges >= 3
 */
CDart* extrudeByRevolution(CDart* ADart, int ADim,
			   const CVertex & AAxeVertex,
			   const CVertex & AAxeDirection,
			   const CVertex & AFirstVertex,
			   TCoordinate AAngle, int ANbEdges,
			   bool AExtrusionWithAdjacentSews,
			   bool ARotateExtrudedCells,
			   bool AScaleExtrudedCells,
			   CDart* APonderationPath = NULL,
			   int AMarkExtruded = -1,
			   int ADirectInfoIndex = -1);

/**
 * Calcule le chemin correspondant à l'axe donné et appelle la méthode
 * 'extrudeByPathMarkedCells'.
 * Les cellules sont éventuellement positionnées perpendiculairement
 * au chemin avant d'effectuer l'extrusion (voir la méthode
 * 'setExtrusionInitialDirection').
 *
 * Attention: si 'getExtrusionInitialDirection' retourne la valeur 'vrai',
 * le vecteur normal de chaque cellule à extruder est calculé, ce qui
 * implique qu'un brin sur deux au maximum pour chaque cellule doit
 * être sélectionné pour éviter toute ambiguïté.
 *
 * Pour la signification du paramètre APonderationDart,
 * AExtrusionWithAdjacentSews, ARotateExtrudedCells et AScaleExtrudedCells
 * se reporter à la méthode 'extrudeByPath'.
 * Pour la signification des paramètres AInitialTranslateExtrudedCells et
 * AInitialRotateExtrudedCells, voir la méthode 'extrudeByPathMarkedCells'.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (1 ou 2)
 * @param AAxeVertex Un sommet de l'axe de révolution
 * @param AAxeDirection Un vecteur directeur de l'axe de révolution
 * @param ANbEdges Le nombre de sections à créer
 * @param AExtrusionWithAdjacentSews Un booléen
 * @param AInitialTranslateExtrudedCells Un entier
 * @param AInitialRotateExtrudedCells Un entier   
 * @param APonderationPath Un brin de la carte
 * @return Le nombre de cellules extrudées selon l'axe donné
 *
 * @precondition !AAxeDirection.isNull()
 * @precondition ANbEdges >= 3
 */
int extrudeByRevolutionMarkedCells(int AMarkNumber, int ADim,
				   const CVertex & AAxeVertex,
				   const CVertex & AAxeDirection,
				   TCoordinate AAngle, int ANbEdges,
				   bool AExtrusionWithAdjacentSews,
				   int AInitialTranslateExtrudedCells,
				   int AInitialRotateExtrudedCells,
				   bool ARotateExtrudedCells,
				   bool AScaleExtrudedCells,
				   CDart* APonderationPath = NULL);

protected:

/**
 * Marque un brin sur deux des cellules de dimension ADim qui doivent être
 * extrudées.
 * Voir CGMapGeneric::markCellsToBeExtruded pour plus d'informations.
 *
 * @param ADim Une dimension (0, 1 ou 2)
 * @param AMarkNumberSrce Un numéro de marque source
 * @param AMarkNumberDest Un numéro de marque destination
 */
void markCellsToBeExtrudedByNormal(int ADim,
				   int AMarkNumberSrce,
				   int AMarkNumberDest);

//******************************************************************************
