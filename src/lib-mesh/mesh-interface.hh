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
#ifndef MESH_INTERFACE_HH
#define MESH_INTERFACE_HH
//******************************************************************************
#include "mesh-vertex.hh"
#include <cstdlib>
//******************************************************************************
class CTransformationMatrix;

namespace GMap3d
{
  class CGMapVertex;

  class CMesh
  {
  public:
    /// Constructeur
    CMesh(CGMapVertex* AMap);

    /// Destructeur
    ~CMesh();

    /**
     * Maille chaque arête dont un brin au moins est marqué, lorsque cela est
     * possible.
     *
     * Si ADirectInfoAssoc est strictement négatif, deux cas se présentent:
     *   + Si FMeshWithMerges vaut 'vrai', la méthode se réserve un champ
     *     directInfo et sauvegarde l'état des coutures par alpha0 à l'aide
     *     de la méthode 'saveAlphaInDirectInfo' avant d'effectuer les
     *     maillages.
     *   + Si FMeshWithMerges vaut 'faux', la méthode se réserve un champ
     *     directInfo qui va être utilisé pour associer les maillages créés aux
     *     arêtes dont ils sont issus.
     *
     * Si ADirectInfoAssoc est positif ou nul, la méthode considère que le
     * champ directInfo alpha déjà été réservé et initialisé.
     * Ce cas se produit lorsque 'meshMarkedEdges' est appelée par
     * 'meshMarkedSquares' ou 'meshMarkedCubes'.
     *
     * Pour plus d'informations concernant les champs ADirectInfoAlpha0,
     * ADirectInfoAssoc, AMeshWithMerges, AMeshWithAdjacentSews, et
     * AMeshAssociatedCells, se reporter à la méthode 'mesh1'.
     *
     * @param AMarkNumber Un numéro de marque
     * @param ASx Le nombre de subdivisions à créer sur chaque arête à mailler
     * @param AMeshMatrix Matrice utilisée pour le calcul des
     *   plongements des cubiques
     * @param AMeshWithMerges Un booléen
     * @param AMeshWithAdjacentSews Un booléen
     * @param AMeshAssociatedCells Un booléen
     * @param ADirectInfoAssoc Un indice de champ directInfo
     * @return Le nombre de maillages effectués
     */
    int meshMarkedEdges(int AMarkNumber, int ASx,
			const CTransformationMatrix* AMeshMatrix = NULL,
			bool AMeshWithMerges = true,
			bool AMeshWithAdjacentSews = true,
			bool AMeshAssociatedCells = false,
			int ADirectInfoAlpha0 = -1, int ADirectInfoAssoc = -1);

    /**
     * Maille chaque carré dont un brin au moins est marqué, lorsque cela est
     * possible.
     * Pour la signification des paramètres AInitialMeshDim et AFinalMeshDim,
     * se reporter à la méthode 'mesh2'.
     *
     * Pour plus d'informations concernant les champs ADirectInfoAlpha0 et
     * ADirectInfoAssoc, se reporter à la méthode 'mesh1'.
     * Pour plus d'informations concernant les champs AMeshWithMerges,
     * AMeshWithAdjacentSews et AMeshAssociatedCells voir la méthode 'mesh1'.
     *
     * @param AMarkNumber Un numéro de marque
     * @param ASx Le nombre de mailles à créer selon la première dimension
     * @param ASy Le nombre de mailles à créer selon la deuxième dimension
     * @param AMeshMatrix Matrice utilisée pour le calcul des
     *   plongements des cubiques
     * @param AMeshWithMerges Un booléen
     * @param AMeshWithAdjacentSews Un booléen
     * @param AMeshAssociatedCells Un booléen
     * @param AInitialMeshDim La dimension du maillage des cellules de départ
     *   (0 ou 1)
     * @param AFinalMeshDim La dimension dans laquelle les cellules doivent
     *   être maillées (1 ou 2)
     * @param ADirectInfoAlpha0 Un indice de champ directInfo
     * @param ADirectInfoAssoc Un indice de champ directInfo
     * @return Le nombre de maillages effectués
     */
    int meshMarkedSquares(int AMarkNumber,
			  int ASx = 1, int ASy = 1,
			  const CTransformationMatrix* AMeshMatrix = NULL,
			  bool AMeshWithMerges = true,
			  bool AMeshWithAdjacentSews = true,
			  bool AMeshAssociatedCells = false,
			  int AInitialMeshDim = 0, int AFinalMeshDim = 2,
			  int ADirectInfoAlpha0 = -1,
			  int ADirectInfoAssoc = -1);

    /**
     * Maille chaque cube dont un brin au moins est marqué, lorsque cela est
     * possible.
     * Pour la signification des paramètres AInitialMeshDim et AFinalMeshDim,
     * se reporter à la méthode 'mesh3'.
     *
     * Pour plus d'informations concernant les champs ADirectInfoAlpha0 et
     * ADirectInfoAssoc, se reporter à la méthode 'mesh1'.
     * Pour plus d'informations concernant les champs AMeshWithMerges,
     * AMeshWithAdjacentSews et AMeshAssociatedCells voir la méthode 'mesh1'.
     *
     * @param AMarkNumber Un numéro de marque
     * @param ASx Le nombre de mailles à créer selon la première dimension
     * @param ASy Le nombre de mailles à créer selon la deuxième dimension
     * @param ASy Le nombre de mailles à créer selon la troisième dimension
     * @param AMeshMatrix Matrice utilisée pour le calcul des
     *   plongements des cubiques
     * @param AMeshWithMerges Un booléen
     * @param AMeshWithAdjacentSews Un booléen
     * @param AMeshAssociatedCells Un booléen
     * @param AInitialMeshDim La dimension du maillage des cellules de départ
     *   (0, 1 ou 2)
     * @param AFinalMeshDim La dimension dans laquelle les cellules doivent
     *   être maillées (1, 2 ou 3)
     * @param ADirectInfoAlpha0 Un indice de champ directInfo
     * @param ADirectInfoAssoc Un indice de champ directInfo
     * @return Le nombre de maillages effectués
     */
    int meshMarkedCubes(int AMarkNumber,
			int ASx = 1, int ASy = 1, int ASz = 1,
			const CTransformationMatrix* AMeshMatrix = NULL,
			bool AMeshWithMerges = true,
			bool AMeshWithAdjacentSews = true,
			bool AMeshAssociatedCells = false,
			int AInitialMeshDim = 0, int AFinalMeshDim = 3,
			int ADirectInfoAlpha0 = -1, int ADirectInfoAssoc = -1);

  private:
    CGMapVertex* FMap;
    CMeshVertex FMesh;
  };
}
//******************************************************************************
#endif // MESH_INTERFACE_HH
//******************************************************************************
