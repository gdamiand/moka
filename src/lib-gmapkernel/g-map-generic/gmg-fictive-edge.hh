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

// Décale l'arête fictive incidente à ADart entre l'arete incidente
// à ADart2 et celle incidente à alpha1(ADart2)
virtual void shiftOneFictiveEdge (CDart* ADart, CDart* ADart2);

// ADart est un brin d'une arête.
// Décale toute les arête incidente au sommet incident à ADart,
// exepté l'arête incidente à ADart.
// (marche uniquement en 2D)
// @return le nombre d'arêtes décalées
unsigned int shiftAllEdgesIncidentToVertex(CDart* ADart);

// ADart est un brin d'une arête réelle.
// Décale toute les arête fictive adjacentes trouvées en partant de
// alpha12(ADart) et en tournant autour du sommet par alpha12(d)
// (marche uniquement en 2D)
void shiftAllAdjacentFictiveEdges(CDart* ADart, int AMark);

// ADart est un brin d'une arête réelle incident à un sommet de degré réel 2.
// Décale toute les arête fictive incidente à ce sommet.
// (marche uniquement en 2D)
void shiftAllFictiveEdges(CDart* ADart, int AMark);

// Décalle toute les arêtes fictives incidentes au sommet désigné par ADart.
// Marche en 3D.
void shiftAllFictiveEdgesAroundEdge(CDart* ADart, int AMark);
  
// Est-ce qu'il existe une arête fictive incidente au sommet incident à ADart ?
bool existFictiveEdgeIncidentTo(CDart*ADart, int AMark);

/**
 *  @param ADart Un brin de la carte
 *  @param AResDart  l'adresse d'un brin : paramètre retour
 *  @param AMark la marque pour les arêtes fictives
 *  @return 0 ssi le sommet est fictif (cas de degré réel 0) ;
 *          1 ssi la seule arête réelle incidente au sommet est
 *            une boucle ;
 *          2 ssi le sommet est de degré réel 2 et que les 2 arêtes réelles
 *            ne sont pas des boucles. Dans ce cas uniquement, *AResDart
 *            contiendra un des brin de ces arêtes incident au sommet ;
 *          3 ssi soit il y a deux arêtes réelles incidente au sommet,
 *            et au moins une est une boucle, soit le degré réel est > 2.
 */
int findVertexType(CDart* ADart, CDart** AResDart, int AMark);
    
/**
 *  @param ADart Un brin de la carte
 *  @return Un brin d'une arête non-boucle incidente au sommet
 *             désigné par ADArt s'il en existe une
 *          NULL Sinon
 */
CDart* findIncidentEdgeNonLoop(CDart* ADart);

/**
 *  @param ADart  Un brin de la carte qui doit appartenir à une arête non fictive
 *  @param AMark  la marque pour les arêtes fictives
 *  @param AMark2 la marque dont on veut marquer les faces réelles
 *  @return le nombre de brins marqués
 *
 *  Marque avec AMark2 la face réelle incidente à ADart (cad la face obtenue en
 *  sautant les arêtes fictives)
 */
int markRealFace(CDart* ADart, int AMark, int AMark2);

/**
 *  @param ADart  Un brin de la carte qui doit appartenir à une arête non fictive
 *  @param AMark  la marque pour les arêtes fictives
 *  @param AMark2 la marque dont on veut démarquer les faces réelles
 *  @return le nombre de brins démarqués
 *
 *  Démarque avec AMark2 la face réelle incidente à ADart (cad la face obtenue en
 *  sautant les arêtes fictives)
 */
int unmarkRealFace(CDart* ADart, int AMark, int AMark2);

//******************************************************************************
