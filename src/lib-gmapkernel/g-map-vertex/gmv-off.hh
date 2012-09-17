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
 * Importe une carte 2D à partir d'un flot au format ASCII Object File Format.
 * Les brins qui composent la carte avant le chargement ne sont pas détruits.
 *
 * @param AStream Un flot dans lequel lire le contenu de la carte à charger
 * @return Un brin de la carte chargée si le chargement s'est bien
 * déroulé, NULL sinon
 */
CDart* importOff2D(std::istream & AStream);

/**
 * Importe une carte 3D à partir d'un flot au format ASCII Object File Format.
 * Les brins qui composent la carte avant le chargement ne sont pas détruits.
 *
 * @param AStream Un flot dans lequel lire le contenu de la carte à charger
 * @return Un brin de la carte chargée si le chargement s'est bien
 * déroulé, NULL sinon
 */
CDart* importOff3D(std::istream & AStream);

/**
 * Sous fonction utile pour ajouter une arête entre les sommets
 * numéros AV1 et AV2. AInitVertices contient les sommets de départ
 * AIndex est le numéro du direct info permettant d'étiquetter
 * les brins par leur sommet incident, et APrec est le brin précédent
 * de la face en cours de construction.
 */
CDart* addEdgeOFF( std::vector< CVertex >& AInitVertices,
                   unsigned long int AV1, unsigned long int AV2,
                   int AIndex, CDart* APrec );

/**
 * La face incidente à ADart vient d'être créé. Cherche à coudre toute
 * ses arêtes par alpha2 en utilisant pour cela les arêtes existantes
 * se trouvant dans ATestVertices et le directInfo AIndex étiquettant les
 * brins avec le numéro du sommets incident.
 */
void linkFaceAlpha2OFF( std::vector< std::list<CDart*> >& ATestVertices,
                        int AIndex, CDart* ADart );

/** 
 * @return la dimension du fichier off en lisant l'entête: 2, 3 ou -1 si erreur
 */
int getOffDimension(const char * AFilename);
      
/**
 * Importe une carte à partir d'un fichier au format ASCII Object File Format.
 *  en lisant l'entête pour savoir si c'est un fichier off2D ou off3D.
 * Les brins qui composent la carte avant le chargement ne sont pas détruits.
 *
 * @param AFilename Un nom de fichier
 * @return Un brin de la carte chargée si le chargement s'est bien
 * déroulé, NULL sinon
 */
CDart* importOff(const char * AFilename);

/**
 * Exporte la carte courante au format off2D.
 * @param AStream Un flot dans lequel écrire le contenu de la carte
 * @return 'vrai' si l'export s'est bien déroulé
 */
bool exportOff2D(std::ostream & AStream);

/**
 * Exporte la carte courante au format off3D.
 * @param AStream Un flot dans lequel écrire le contenu de la carte
 * @return 'vrai' si l'export s'est bien déroulé
 */
bool exportOff3D(std::ostream & AStream);

/**
 * Exporte la carte courante au format off2D ou off3D selon la dimension de la
 * carte courante.
 * @param AFilename Le nom du fichier dans lequel écrire le contenu de la carte
 * @return 'vrai' si l'export s'est bien déroulé
 */
bool exportOff(const char * AFilename);

/**
 * Exporte la carte courante au format off3D.
 * @param AFilename Le nom du fichier dans lequel écrire le contenu de la carte
 * @return 'vrai' si l'export s'est bien déroulé
 */
bool exportOff3D(const char * AFilename);

