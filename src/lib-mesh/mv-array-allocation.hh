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
 * Alloue l'espace mémoire nécessaire pour stocker le plongement de ADimX
 * sommets.
 *
 * @param ADimX La taille du tableau
 * @return Un pointeur sur un tableau de ADimX sommets
 */
CVertex* allocVertexArray1(int ADimX);

/**
 * Libère l'espace mémoire occupé par AArray1.
 *
 * @param AArray1 Un tableau créé avec la méthode 'allocVertexArray1'
 * @param ADimX La taille du tableau
 */
void freeVertexArray1(CVertex* AArray1, int ADimX);

/**
 * Alloue l'espace mémoire nécessaire pour stocker le plongement de
 * ADimX*ADimY sommets.
 *
 * @param ADimX La première dimension du tableau
 * @param ADimY La deuxième dimension du tableau
 * @return Un pointeur sur un tableau de ADimX*ADimY sommets
 */
CVertex** allocVertexArray2(int ADimX, int ADimY);

/**
 * Libère l'espace mémoire occupé par AArray2.
 *
 * @param AArray2 Un tableau créé avec la méthode 'allocVertexArray2'
 * @param ADimX La première dimension du tableau
 * @param ADimY La deuxième dimension du tableau
 */
void freeVertexArray2(CVertex** AArray2, int ADimX, int ADimY);

/**
 * Alloue l'espace mémoire nécessaire pour stocker le plongement de
 * ADimX*ADimY*ADimZ sommets.
 *
 * @param ADimX La première dimension du tableau
 * @param ADimY La deuxième dimension du tableau
 * @param ADimZ La troisième dimension du tableau
 * @return Un pointeur sur un tableau de ADimX*ADimY*ADimZ sommets
 */
CVertex*** allocVertexArray3(int ADimX, int ADimY, int ADimZ);

/**
 * Libère l'espace mémoire occupé par AAray3.
 *
 * @param AArray3 Un tableau créé avec la méthode 'allocVertexArray3'
 * @param ADimX La première dimension du tableau
 * @param ADimY La deuxième dimension du tableau
 * @param ADimZ La troisième dimension du tableau
 */
void freeVertexArray3(CVertex*** AArray3,
		      int ADimX, int ADimY, int ADimZ);

/**
 * Alloue l'espace mémoire nécessaire pour stocker ADimX pointeurs sur sommet.
 *
 * @param ADimX La taille du tableau
 * @return Un pointeur sur un tableau de ADimX pointeurs sur sommets
 */
const CVertex** allocVertexPtrArray1(int ADimX);

/**
 * Libère l'espace mémoire occupé par AArray1.
 *
 * @param AArray1 Un tableau créé avec la méthode 'allocVertexPtrArray1'
 * @param ADimX La taille du tableau
 */
void freeVertexPtrArray1(const CVertex** AArray1, int ADimX);

/**
 * Alloue l'espace mémoire nécessaire pour stocker ADimX*ADimY pointeurs sur
 * sommets.
 *
 * @param ADimX La première dimension du tableau
 * @param ADimY La deuxième dimension du tableau
 * @return Un pointeur sur un tableau de ADimX*ADimY pointeurs sur sommets
 */
const CVertex*** allocVertexPtrArray2(int ADimX, int ADimY);

/**
 * Libère l'espace mémoire occupé par AArray2.
 *
 * @param AArray2 Un tableau créé avec la méthode 'allocVertexPtrArray2'
 * @param ADimX La première dimension du tableau
 * @param ADimY La deuxième dimension du tableau
 */
void freeVertexPtrArray2(const CVertex*** AArray2, int ADimX, int ADimY);

/**
 * Alloue l'espace mémoire nécessaire pour stocker ADimX*ADimY*ADimZ
 * pointeurs sur sommets.
 *
 * @param ADimX La première dimension du tableau
 * @param ADimY La deuxième dimension du tableau
 * @param ADimZ La troisième dimension du tableau
 * @return Un pointeur sur un tableau de ADimX*ADimY*ADimZ pointeurs sur
 *   sommets
 */
const CVertex**** allocVertexPtrArray3(int ADimX, int ADimY, int ADimZ);

/**
 * Libère l'espace mémoire occupé par AArray3.
 *
 * @param AArray3 Un tableau créé avec la méthode 'allocVertexPtrArray3'
 * @param ADimX La première dimension du tableau
 * @param ADimY La deuxième dimension du tableau
 * @param ADimZ La troisième dimension du tableau
 */
void freeVertexPtrArray3(const CVertex**** AArray3,
			 int ADimX, int ADimY, int ADimZ);

//******************************************************************************
