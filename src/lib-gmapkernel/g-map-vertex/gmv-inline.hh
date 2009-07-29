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
 * Destruction de tous les brins de la carte.
 * Cette méthode appelle la méthode 'empty' de la classe CGMapBasic, puis
 * sélectionne l'orbite ORBIT_VERTEX comme orbite utilisée.
 */
void empty();

/**
 * Crée une nouvelle instance de la classe CDartVertex.
 *
 * @return Un nouveau brin, de type CDartVertex
 */
virtual CDart* newDart();

/**
 *  Supprime le brin passé en paramètre (de type CDartVertex)
 *
 *  @param ADart Le brin à supprimer
 */
virtual void delDart(CDart* ADart);

/**
 * Crée une nouvelle instance de CDartVertex.
 * Le brin créé n'est pas plongé.
 *
 * @return Un nouveau brin, de type CDartVertex
 */
CDart* addMapDart();

/**
 * Crée une nouvelle instance de CDartVertex, dont le plongement est donné par
 * AVertex.
 *
 * @param AVertex Un sommet
 * @return Un nouveau brin de type CDartVertex
 */
CDartVertex* addMapDart(const CVertex & AVertex);

/**
 * Retourne le coefficient d'éclatement de la dimension ADim.
 *
 * @param ADim Une dimension
 * @return Une valeur comprise entre 0 et 1
 */
TCoordinate getBurstCoef(int ADim) const;

/**
 * Positionne le coefficient d'éclatement de la dimension ADim à la valeur
 * ACoef.
 *
 * @param ADim Une dimension
 * @param ACoef Une valeur comprise entre 0 et 1.
 */
void setBurstCoef(int ADim, TCoordinate ACoef);

/**
 * Renvoie le sommet associé à un brin, en parcourant si nécessaire tous les
 * brins du sommet.
 *
 * @param ADart Un brin de la carte
 * @return Le plongement sommet du brin
 */
CAttributeVertex* findVertex(CDart* ADart);

/**
 * Renvoie le sommet associé à un brin.
 * La méthode renvoie NULL si le plongement sommet n'est pas porté par ADart.
 *
 * @param ADart Un brin de la carte
 * @return Le plongement sommet du brin
 */
CAttributeVertex* getVertex(CDart* ADart) const;

/**
 * Affecte le plongement AVertex au brin ADart.
 *
 * @param ADart Un brin de la carte
 * @param AVertex Un sommet
 *
 * @precondition findVertex(ADart)==NULL
 */
void setVertex(CDart* ADart, const CVertex & AVertex);
void setVertex(CDart* ADart, CAttributeVertex* AVertex);

/**
 * Ecrase s'il existe le plongement sommet du brin ADart avec le sommet AVertex.
 * Si le sommet topologique incident à ADart n'est pas plongé, la méthode
 * est équivalente à un appel à 'setVertex'.
 *
 * @param ADart Un brin de la carte
 * @param AVertex Un sommet
 */
void updateVertex(CDart* ADart, const CVertex & AVertex);

/**
 * Supprime s'il existe le plongement sommet du brin ADart.
 *
 * @param ADart Un brin de la carte
 */
void delVertex(CDart* ADart);

/**
 * Détache le plongement sommet du sommet topologique incident à ADart.
 * Si le sommet n'est pas plongé, la méthode retourne NULL.
 *
 * @param ADart Un brin de la carte
 */
CAttributeVertex* removeVertex(CDart* ADart);

/**
 * Retourne le plongement éclaté de la première extrémité du brin ADart donné.
 *
 * @param ADart Un brin de la carte
 * @return Le plongement éclaté de ADart
 */
CVertex & getBurstVertex(CDart* ADart) const;

/**
 * Affecte le plongement éclaté de la première extrémité du brin ADart donné.
 *
 * @param ADart Un brin de la carte
 * @param AVertex Les nouvelles coordonnées
 */
void setBurstVertex(CDart* ADart, const CVertex & AVertex);

/**
 * Retourne le plongement éclaté de la deuxième extrémité du brin ADart donné.
 *
 * @param ADart Un brin de la carte
 * @return Le plongement éclaté de l'extrémité du brin ADart donné
 */
CVertex computeBurstExtremity(CDart* ADart) const;

/**
 * Retourne le champ directInfo demandé du brin ADart après l'avoir
 * retypé en pointeur sur un attribut sommet.
 *
 * @param ADart Un brin de la carte
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @return Un pointeur sur un attribut sommet
 */
CAttributeVertex* getDirectInfoAsAttributeVertex(CDart* ADart,
						 int ADirectInfoIndex) const;

/**
 * Retourne le champ directInfo demandé du brin ADart après l'avoir
 * retypé en pointeur sur un sommet.
 *
 * @param ADart Un brin de la carte
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @return Un pointeur sur un sommet
 */
CVertex* getDirectInfoAsVertex(CDart* ADart, int ADirectInfoIndex) const;

/**
 * Retourne le champ directInfo demandé du brin ADart après l'avoir
 * retypé en pointeur sur une coordonnée (TCoordinate).
 
 * @param ADart Un brin de la carte
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @return Un pointeur sur une coordonnée
 */
TCoordinate* getDirectInfoAsCoord(CDart* ADart, int ADirectInfoIndex) const;

//******************************************************************************
