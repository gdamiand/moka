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
 * Cherche si la carte *this est un motif de la carte AMap.
 * @param AMap la carte dans laquelle on va chercher le motif.
 * @param ANbMatched un pointeur vers un entier. Si non nul, paramètre
 * retour qui va contenir le nombre de brins maximum matché. 
 * @return -1 si motif non trouvé, le numéro du direct info contenant 
 * l'injection sinon (valide pour les brins marqués pour AMarkToTest)
 * Retourne le premier motif trouvé.
 */
int findMotif( CGMapVertex* AMap, unsigned int* ANbMatched=NULL );
    
/**
 * Compte le nombre de fois que la carte *this est motif de la carte AMap.
 * @param AMap la carte dans laquelle on va chercher le motif.
 * @param ANbMatched un pointeur vers un entier. Si non nul, paramètre
 * retour qui va contenir le nombre de brins maximum matché. 
 * @return le nombre de motifs trouvés.
 */
unsigned int countNumberOfMotifs( CGMapVertex* AMap, 
				  unsigned int* ANbMatched=NULL  );
    
/**
 * Cherche si la carte *this est un motif de la carte AMap à partir du
 * brin AFromDart dans la carte this et ADestDart dans la carte AMap.
 * AMarkTreated est le numéro de marque utilisé pour marquer les brins
 * déjà traités.
 * @param ANbMatched un pointeur vers un entier. Si non nul, paramètre
 * retour qui va contenir le nombre de brins matché. 
 * @return vrai ssi motif trouvé, faux sinon. Dans les 2 cas, le champ
 * directInfo[AIndex] contient l'injection affectée (partiellement en
 * cas d'échec.
 */
bool findMotifFrom( CDart* AFromDart, unsigned int AMarkTreated,
		    unsigned int AIndex,
		    CGMapVertex* AMap, CDart* ADestDart,
		    unsigned int AMarkTreated2,
		    unsigned int* ANbMatched=NULL );

/// Met le champ directinfo du motif d'origine ADart à Null.
/// Le parcours du motif utilise la marque AMark.
/// En même temps, la carte AMap est démarqué pour la marque AMark2.
void unmarkMotifMark(CDart* ADart, int AMark, int AIndex,
		     CGMapVertex* AMap, CDart* ADart2, int AMark2);

//******************************************************************************
