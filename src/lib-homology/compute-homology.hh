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
#include <vector>
class MatricePMQ;

namespace GMap3d
{
  class GMCGMapVertex;

//******************************************************************************
class CHomology
{
public:
  /// Default constructor, itializing the gmap.
  CHomology(CGMapVertex* AMap, int AMark=-1);

  /// Destructor.
  ~CHomology();
  
  /// Compute the homology of the gmap.
  /// @return true if the homology is computed,
  ///         false otherwise (case of not enough memory)
  bool computeHomology();

  /// Return the memory size (in bytes) required for the matrixes.
  unsigned long size() const;

  /// Return the number of H0 free generators
  unsigned int getH0FreeGenerators();
  
  /// Return the number of H1 free generators
  unsigned int getH1FreeGenerators();
  
  /// Return the number of H1 torsion generators
  unsigned int getH1TorsionGenerators();

  /// Get the value of FShowH0
  bool getShowH0() const;
  
  /// Get the value of FShowH1free
  bool getShowH1free() const;
  
  /// Get the value of FShowH1torsion
  bool getShowH1torsion() const;
  
  /// Get the value of FShowH2
  bool getShowH2() const;
  
  /// Set the value of FShowH0 and update the selected darts accordingly.
  void setShowH0(bool AValue);
  
  /// Set the value of FShowH1free and update the selected darts accordingly.
  void setShowH1free(bool AValue);
  
  /// Set the value of FShowH1torsion and update the selected darts accordingly.
  void setShowH1torsion(bool AValue);
  
  /// Set the value of FShowH2 and update the selected darts accordingly.
  void setShowH2(bool AValue);
  
protected:
  /** Compute the incidence matrix for cells of dimension ADim.
   *  The incidence matrix FMatrix[ADim] is updated.
   * @precondition (ADim >= 0) && (ADim < 3)
   */
  void computeIncidence(int ADim);

  /**
   * Compute the incidence number of cell c=(ADart,ADim), relatively to the cell
   * c2=(ADart2,ADim+1). Incidence number is the number of times c is incident to
   * c2. This number is signed depending if the two cells have the same
   * orientation (+) or not (-).
   *
   * @param ADart A dart of the cell c
   * @param ADim The dimension of the cell c
   * @param ADart A adart of the cell c2
   * @param AIndex A direct info which number each ADim cell (+i or -i).
   * @return The signed incidence number of c in c2
   *
   * @precondition (ADim >= 0) && (ADim < 3)
   */
  int computeIncidenceNumber(CDart* ADart, int ADim, CDart* ADart2, int AIndex);

  /// Update the marked darts depending on which generators are drawn.
  void updateSelectedDarts();
  
private:
  /// The gmap.
  CGMapVertex* FMap;

  /// The mark used to select darts.
  int FMark;
  
  /// The 3 incidence matrixes.
  MatricePMQ* FMatrix[3];

  /// Array of cells keeping one dart of each cell.
  std::vector<CDart*> FCells[3];
  
  /// Number of vertices of the map
  int FNbVertices;
  
  /// Number of edges of the map
  int FNbEdges;
  
  /// Number of faces of the map
  int FNbFaces;

  /// Number of volumes of the map
  int FNbVolumes;

  /// Memory size of the different matrixes.
  int FSize;

  /// Informations on generators
  int FNbTorsion, FNbZO, FNbZ1, FNbBordFaible, FNbLibre;

  /// Boolean indicating which generators are drawn.
  bool FShowH0, FShowH1free, FShowH1torsion, FShowH2;
};
//******************************************************************************
} // end namespace GMap3d
//******************************************************************************
#endif // COMPUTE_HOMOLOGY_HH
//******************************************************************************
