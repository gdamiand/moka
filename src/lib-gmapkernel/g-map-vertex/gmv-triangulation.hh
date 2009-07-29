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
 * Surcharge la méthode 'triangulateEdge' de la classe CGMapGeneric pour
 * plonger le sommet qu'elle crée.
 *
 * @param ADart Un brin de la carte
 * @return Un brin du sommet topologique créé
 */
CDart* triangulateEdge(CDart* ADart);

/**
 * Surcharge la méthode 'triangulateFace' de la classe CGMapGeneric pour
 * plonger le sommet qu'elle crée.
 *
 * @param ADart Un brin de la carte
 * @return Un brin du sommet topologique créé
 */
CDart* triangulateFace(CDart* ADart);

/**
 * Surcharge la méthode 'triangulateVolume' de la classe CGMapGeneric pour
 * plonger le sommet qu'elle crée.
 *
 * @param ADart Un brin de la carte
 * @return Un brin du sommet topologique créé
 */
CDart* triangulateVolume(CDart* ADart);


/**
 * @return vrai ssi l'arête incidente à AEdge doit être inversée.
 */
bool shouldSwapEdge(CDart * AEdge, int AVertexDI = -1);

/**
 * Tourne l'arête incidente à AEdge. Si AVertexDI est != -1, met à
 * jour le directInfo correspondant pour qu'il pointe sur les bons
 * sommets. L'arête est tournée selon le schéma suivant:
 *         p[0]
 *         /|#\
 *        / || \ 
 *       /  ||  \
 *  p[1] ===  === p[3]
 *       \  ||  /
 *        \ || /
 *         \||/
 *         p[2]
 *
 * #  brin AEdge
 * || arête d'origine
 * == futur arête
 */
bool swapEdge(CDart * AEdge, int AVertexDI = -1);

/**
 * Triangulation "géométrique" de la face incidente à AFace.
 * Si ANewEdgesMark !=-1, les nouvelles arêtes sont marquées avec
 * cette marque. Si AVertexDI,  met à jour le directInfo correspondant
 * pour qu'il pointe sur les bons sommets.
 */
void triangulateGeoFace(CDart * AFace, int ANewEdgesMark=-1,
			int AVertexDI=-1);

/**
 * Triangulation "géométrique" de toutes les faces marquées avec
 * AMark.
 * Si ANewEdgesMark !=-1, les nouvelles arêtes sont marquées avec
 * cette marque. Si AVertexDI,  met à jour le directInfo correspondant
 * pour qu'il pointe sur les bons sommets.
 */
void triangulateMarkedFaces(int AMark, int ANewEdgesMark=-1,
			    int AVertexDI=-1);

/**
 * @return vrai ssi le point APoint se trouve à l'intérieur du
 * triangle (AVertex1, AVertex2, AVertex3) de normale ANormal.
 */
bool isPointInTriangle(const CVertex & APoint, const CVertex & AVertex1,
		       const CVertex & AVertex2, const CVertex & AVertex3,
		       const CVertex & ANormal);

/**
 * @return le coefficient du triangle (AVertex1, AVertex2, AVertex3)
 */
TCoordinate getTriangleCoef(const CVertex & AVertex1, const CVertex & AVertex2,
			    const CVertex & AVertex3);

//******************************************************************************
