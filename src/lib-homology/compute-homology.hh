/*
 * lib-homology : Computation of homology generators.
 * Copyright (C) 2010, Samuel Peltier, Université de Poitiers, Laboratoire SIC-XLIM
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2010, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-homology
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
#ifndef COMPUTE_HOMOLOGY_HH
#define COMPUTE_HOMOLOGY_HH
//******************************************************************************
class MatricePMQ;

namespace GMap3d
{
  class GMCGMapVertex;

//******************************************************************************
class CHomology
{
public:
  /// Default constructor, itializing the gmap.
  CHomology(CGMapVertex* AMap);

  /// Destructor.
  ~CHomology();
  
  /// Compute the homology of the gmap.
  void computeHomology();

protected:
  /// Compute the incidence matric for cells of dimension ADim.
  void computeIncidence(int ADim);

private:
  /// The gmap.
  CGMapVertex* FMap;

  /// The 3 incidence matrixes.
  MatricePMQ* FMatrix[3];

  /// Number of vertices of the map
  int FNbVertices;
  
  /// Number of edges of the map
  int FNbEdges;
  
  /// Number of faces of the map
  int FNbFaces;

  /// Number of volumes of the map
  int FNbVolumes;  
};
//******************************************************************************
} // end namespace GMap3d
//******************************************************************************
#endif // COMPUTE_HOMOLOGY_HH
//******************************************************************************
