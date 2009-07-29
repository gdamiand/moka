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
// Ce fichier est une partie de "mesh-vertex.hh"
//******************************************************************************

public:

/**
 * Surcharge la méthode CGMapGeneric::mesh1 pour plonger le maillage créé.
 * Le plongement effectué est linéaire.
 *
 * Pour plus d'informations sur les paramètres ADirectInfoAlpha0 et
 * ADirectInfoAssoc, se reporter à la méthode 'CGMapGeneric::mesh1'.
 * Pour plus d'informations concernant les champs AMeshWithMerges,
 * AMeshWithAdjacentSews et AMeshAssociatedCells voir également cette
 * méthode.
 *
 * @param ADart Un brin de l'arête à mailler
 * @param ASx Le nombre de mailles à créer
 * @param AMeshMatrix Matrice utilisée pour le calcul des
 *   plongements des cubiques
 * @param AMeshWithMerges Un booléen
 * @param AMeshWithAdjacentSews Un booléen
 * @param AMeshAssociatedCells Un booléen
 * @param ADirectInfoAlpha0 Un indice de champ directInfo
 * @param ADirectInfoAssoc Un indice de champ directInfo
 *
 * @precondition ASx>0
 * @precondition canMesh1(ADart);
 */
virtual void mesh1(CDart* ADart, int ASx,
		   const CTransformationMatrix * AMeshMatrix,
		   bool AMeshWithMerges = true,
		   bool AMeshWithAdjacentSews = true,
		   bool AMeshAssociatedCells = false,
		   int ADirectInfoAlpha0 = -1, int ADirectInfoAssoc = -1);

/**
 * Surcharge la méthode CGMapGeneric::mesh2 pour plonger le maillage créé.
 * Le plongement effectué est linéaire.
 *
 * Pour plus d'informations sur les paramètres ADirectInfoAlpha0 et
 * ADirectInfoAssoc, se reporter à la méthode 'CGMapGeneric::mesh1'.
 *
 * @param ADart Un brin du carré à mailler
 * @param ASx Le nombre de mailles selon la première dimension
 * @param ASy Le nombre de mailles selon la deuxième dimension
 * @param AMeshMatrix Matrice utilisée pour le calcul des
 *   plongements des cubiques
 * @param AMeshWithMerges Un booléen
 * @param AMeshWithAdjacentSews Un booléen
 * @param AMeshAssociatedCells Un booléen
 * @param AGetAssociatedEmbeddings Un booléen
 * @param AInitialMeshDim La dimension dans laquelle le carré à mailler est
 *   déjà maillé (0 ou 1)
 * @param AFinalMeshDim La dimension dans laquelle la cellule doit être
 *   maillée (1 ou 2)
 * @param ADirectInfoAlpha0 Un indice de champ directInfo
 * @param ADirectInfoAssoc Un indice de champ directInfo
 *
 * @precondition ASx>0
 * @precondition ASy>0
 * @precondition AInitialMeshDim = 0 ou 1
 * @precondition AFinalMeshDim   = 1 ou 2
 * @precondition AInitialMeshDim < AFinalMeshDim
 * @precondition canMesh2(ADart, ASx,ASy, AInitialMeshDim);
 */
virtual void mesh2(CDart* ADart,
		   int ASx, int ASy,
		   const CTransformationMatrix * AMeshMatrix,
		   bool AMeshWithMerges = true,
		   bool AMeshWithAdjacentSews = true,
		   bool AMeshAssociatedCells = false,
		   bool AGetAssociatedEmbeddings = false,
		   int AInitialMeshDim = 0, int AFinalMeshDim = 2,
		   int ADirectInfoAlpha0 = -1, int ADirectInfoAssoc = -1);

/**
 * Surcharge la méthode CGMapGeneric::mesh3 pour plonger le maillage créé.
 * Le plongement effectué est linéaire.
 *
 * Pour plus d'informations sur les paramètres ADirectInfoAlpha0 et
 * ADirectInfoAssoc, se reporter à la méthode 'CGMapGeneric::mesh3'.
 *
 * @param ADart Un brin du cube à mailler
 * @param ASx Le nombre de mailles selon la première dimension
 * @param ASy Le nombre de mailles selon la deuxième dimension
 * @param ASz Le nombre de mailles selon la troisième dimension
 * @param AMeshMatrix Matrice utilisée pour le calcul des
 *   plongements des cubiques
 * @param AMeshWithMerges Un booléen
 * @param AMeshWithAdjacentSews Un booléen
 * @param AMeshAssociatedCells Un booléen
 * @param AInitialMeshDim La dimension dans laquelle le cube à mailler est
 *   déjà maillé (0, 1 ou 2)
 * @param AFinalMeshDim La dimension dans laquelle la cellule doit être
 *   maillée (1, 2 ou 3)
 * @param ADirectInfoAlpha0 Un indice de champ directInfo
 * @param ADirectInfoAssoc Un indice de champ directInfo
 *
 * @precondition ASx>0
 * @precondition ASy>0
 * @precondition ASz>0
 * @precondition AInitialMeshDim = 0, 1 ou 2
 * @precondition AFinalMeshDim   = 1, 2 ou 3
 * @precondition AInitialMeshDim < AFinalMeshDim
 * @precondition canMesh3(ADart, ASx,ASy,ASz, AInitialMeshDim);
 */
virtual void mesh3(CDart* ADart,
		   int ASx, int ASy, int ASz,
		   const CTransformationMatrix * AMeshMatrix,
		   bool AMeshWithMerges = true,
		   bool AMeshWithAdjacentSews = true,
		   bool AMeshAssociatedCells = false,
		   int AInitialMeshDim = 0, int AFinalMeshDim = 3,
		   int ADirectInfoAlpha0 = -1, int ADirectInfoAssoc = -1);

//******************************************************************************
