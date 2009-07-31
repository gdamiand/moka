/*
 * lib-tools : Divers outils autour de lib-gmapkernel.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-tools
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

#ifndef GRID_3D_HH
#define GRID_3D_HH

/** Classe définissant une grille régulière 3D.
 *
 * @author Nicolas Guiard
 */

#include <iostream>
#include <cassert>
#include <climits>

#include "bounding-box.hh"

template <class T> class CGrid3dIterator;

template <class T>
class CGrid3d
{
public:

  /** @name Constructeurs et destructeur
   */

  //@{

  /** Constructeur avec initialisation
   *
   * @param ASizeI Nombre de divisions en largeur
   * @param ASizeJ Nombre de divisions en hauteur
   * @param ASizeK Nombre de divisions en profondeur
   * @param ABB Boîte englobante de la grille
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle instance de la classe initialisée
   * avec une grille dont les dimensions sont passées en paramètres.
   */
  CGrid3d(int ASizeI, int ASizeJ, int ASizeK,
	  const CBoundingBox & ABB = CBoundingBox());

  /** Destructeur
   *
   * Destruction de l'instance.
   */
  virtual ~CGrid3d();

  //@}

  /** @name Méthodes publiques
   */

  //@{

  int getSizeI() const;
  int getSizeJ() const;
  int getSizeK() const;

  const CBoundingBox & getBoundingBox() const;

  T & getCell(int Ai, int Aj, int Ak) const;
  T & getCell(const CGrid3dIterator<T> & AIter) const;

  CBoundingBox getCellBoundingBox(int Ai, int Aj, int Ak) const;
  CBoundingBox getCellBoundingBox(const CGrid3dIterator<T> & AIter) const;

  bool getCellPosition(const CVertex & APoint,
           int * Ai, int * Aj, int * Ak) const;

  bool getCellPosition(const CVertex & APoint,
           TCoordinate * Ai,
           TCoordinate * Aj,
           TCoordinate * Ak) const;

  void setBoundingBox(const CBoundingBox & ABB);

  void setCell(int Ai, int Aj, int Ak, const T & AObj);
  void setCell(const CGrid3dIterator<T> & AIter, const T & AObj);

  void fillCellsWith(const CBoundingBox & ABB, const T & AObj);

  //@}

private:

  /** @name Méthodes privées
   */

  //@{

  TCoordinate getCellI(TCoordinate Ax) const;
  TCoordinate getCellJ(TCoordinate Ay) const;
  TCoordinate getCellK(TCoordinate Az) const;

  //@}

private:

  /** Attributs privés
   */

  //@{

  /** FGrid
   * Tableau tri-dimensionnel représentant la grille.
   */
  T ***FGrid;

  /** FSizeI, FSizeJ, FSizeK
   * Nombre de divisions selon les 3 axes.
   */
  int FSizeI, FSizeJ, FSizeK;

  /** FBB
   * Boîte englobante de la grille.
   */
  CBoundingBox FBB;

  //@}
};
//******************************************************************************
template <class T>
class CGrid3dIterator
{
public:
  CGrid3dIterator(const CGrid3d<T> & AGrid);
  CGrid3dIterator(const CGrid3d<T> & AGrid, const CBoundingBox & ABB);
  CGrid3dIterator(const CGrid3dIterator<T> & AI);
  virtual ~CGrid3dIterator();

  virtual void reinit();
  virtual bool cont() const;
  virtual void next();

  int getI() const { return Fi; }
  int getJ() const { return Fj; }
  int getK() const { return Fk; }

  T & getCurrent() const { return FGrid.getCell(*this); }
  T & operator*() const { return getCurrent(); }
  T * operator->() const { return &(getCurrent()); }

  CGrid3dIterator<T> & operator++()
  {
    next();
    return *this;
  }

  CGrid3dIterator<T> operator++(int)
  {
    CGrid3dIterator<T> tmp = *this;
    next();
    return tmp;
  }

  friend std::ostream & operator<<(std::ostream & AO,
           const CGrid3dIterator<T> & AI)
  {
    return (AO << "(" << AI.Fi << "," << AI.Fj << "," << AI.Fk << ")");
  }

protected:
  const CGrid3d<T> & FGrid;
  int Fi, Fj, Fk;
  int FMinI, FMinJ, FMinK, FMaxI, FMaxJ, FMaxK;
};
//******************************************************************************

/******************************************************************************
 * CLASSES BODIES                                                             *
 ******************************************************************************/
template <class T>
CGrid3d<T>::CGrid3d(int ASizeI, int ASizeJ, int ASizeK,
        const CBoundingBox & ABB)
  : FSizeI(ASizeI), FSizeJ(ASizeJ), FSizeK(ASizeK), FBB(ABB)
{
  assert(ASizeI > 0);
  assert(ASizeJ > 0);
  assert(ASizeK > 0);
  assert(!ABB.isEmpty());

  FGrid = new T**[ASizeI];
  for (int i=0 ; i<FSizeI ; i++) {
    FGrid[i] = new T*[ASizeJ];
    for (int j=0 ; j<FSizeJ ; j++)
      FGrid[i][j] = new T[ASizeK];
  }
}
//******************************************************************************
template <class T>
CGrid3d<T>::~CGrid3d()
{
  for (int i=0 ; i<FSizeI ; i++) {
    for (int j=0 ; j<FSizeJ ; j++)
      delete [] FGrid[i][j];
    delete [] FGrid[i];
  }
  delete [] FGrid;
}
//******************************************************************************
template <class T> inline
int CGrid3d<T>::getSizeI() const
{
  return FSizeI;
}
//******************************************************************************
template <class T> inline
int CGrid3d<T>::getSizeJ() const
{
  return FSizeJ;
}
//******************************************************************************
template <class T> inline
int CGrid3d<T>::getSizeK() const
{
  return FSizeK;
}
//******************************************************************************
template <class T> inline
const CBoundingBox & CGrid3d<T>::getBoundingBox() const
{
  return FBB;
}
//******************************************************************************
template <class T> inline
T & CGrid3d<T>::getCell(int Ai, int Aj, int Ak) const
{
//   if (Ai < 0 || Ai >= FSizeI ||
//       Aj < 0 || Aj >= FSizeJ ||
//       Ak < 0 || Ak >= FSizeK) {
//     cout << "Ai = " << Ai << " Aj = " << Aj << " Ak = " << Ak << endl;
//   }

  assert(Ai >= 0 && Ai < FSizeI);
  assert(Aj >= 0 && Aj < FSizeJ);
  assert(Ak >= 0 && Ak < FSizeK);

  return FGrid[Ai][Aj][Ak];
}
//******************************************************************************
template <class T> inline
T & CGrid3d<T>::getCell(const CGrid3dIterator<T> & AIter) const
{
  return getCell(AIter.getI(), AIter.getJ(), AIter.getK());
}
//******************************************************************************
template <class T>
CBoundingBox
CGrid3d<T>::getCellBoundingBox(int Ai, int Aj, int Ak) const
{
  assert(Ai >= 0 && Ai < FSizeI);
  assert(Aj >= 0 && Aj < FSizeJ);
  assert(Ak >= 0 && Ak < FSizeK);

  CVertex diff = FBB.getMaxBound() - FBB.getMinBound();

  diff.setX(diff.getX() / (TCoordinate)FSizeI);
  diff.setY(diff.getY() / (TCoordinate)FSizeJ);
  diff.setZ(diff.getZ() / (TCoordinate)FSizeK);

  CVertex min = (FBB.getMinBound() +
     CVertex(diff.getX() * (TCoordinate)Ai,
       diff.getY() * (TCoordinate)Aj,
       diff.getZ() * (TCoordinate)Ak));

  return CBoundingBox(min, min + diff);
}
//******************************************************************************
template <class T>
CBoundingBox
CGrid3d<T>::getCellBoundingBox(const CGrid3dIterator<T> & AIter) const
{
  return getCellBoundingBox(AIter.getI(), AIter.getJ(), AIter.getK());
}
//******************************************************************************
template <class T>
bool CGrid3d<T>::getCellPosition(const CVertex & APoint,
         int * Ai, int * Aj, int * Ak) const
{
  TCoordinate i, j, k;

  i = getCellI(APoint.getX());
  j = getCellJ(APoint.getY());
  k = getCellK(APoint.getZ());

  *Ai = ((i >= (TCoordinate)INT_MAX) ? INT_MAX :
   (i <= (TCoordinate)INT_MIN) ? INT_MIN :
   (int)i);
  *Aj = ((j >= (TCoordinate)INT_MAX) ? INT_MAX :
   (j <= (TCoordinate)INT_MIN) ? INT_MIN :
   (int)j);
  *Ak = ((k >= (TCoordinate)INT_MAX) ? INT_MAX :
   (k <= (TCoordinate)INT_MIN) ? INT_MIN :
   (int)k);

  return (*Ai >= 0 && *Ai < FSizeI &&
    *Aj >= 0 && *Aj < FSizeJ &&
    *Ak >= 0 && *Ak < FSizeK);
}
//******************************************************************************
template <class T>
bool CGrid3d<T>::getCellPosition(const CVertex & APoint,
         TCoordinate * Ai,
         TCoordinate * Aj,
         TCoordinate * Ak) const
{
  *Ai = getCellI(APoint.getX());
  *Aj = getCellJ(APoint.getY());
  *Ak = getCellK(APoint.getZ());

  return (*Ai >= 0.0 && *Ai < (TCoordinate)FSizeI &&
    *Aj >= 0.0 && *Aj < (TCoordinate)FSizeJ &&
    *Ak >= 0.0 && *Ak < (TCoordinate)FSizeK);
}
//******************************************************************************
template <class T> inline
void CGrid3d<T>::setBoundingBox(const CBoundingBox & ABB)
{
  FBB = ABB;
}
//******************************************************************************
template <class T> inline
void CGrid3d<T>::setCell(int Ai, int Aj, int Ak, const T & AObj)
{
  assert(Ai >= 0 && Ai < FSizeI);
  assert(Aj >= 0 && Aj < FSizeJ);
  assert(Ak >= 0 && Ak < FSizeK);

  FGrid[Ai][Aj][Ak] = AObj;
}
//******************************************************************************
template <class T> inline
void CGrid3d<T>::setCell(const CGrid3dIterator<T> & AIter, const T & AObj)
{
  setCell(AIter.getI(), AIter.getJ(), AIter.getK(), AObj);
}
//******************************************************************************
template <class T>
void CGrid3d<T>::fillCellsWith(const CBoundingBox & ABB, const T & AObj)
{
  assert(FBB.isInBox(ABB.getMinBound()) && FBB.isInBox(ABB.getMaxBound()));

  int min_i, min_j, min_k, max_i, max_j, max_k;

  getCellPosition(ABB.getEpsMinBound(), &min_i, &min_j, &min_k);
  getCellPosition(ABB.getEpsMaxBound(), &max_i, &max_j, &max_k);

  if (min_i < 0) min_i = 0;
  if (min_j < 0) min_j = 0;
  if (min_k < 0) min_k = 0;

  if (max_i >= FSizeI) max_i = FSizeI - 1;
  if (max_j >= FSizeJ) max_j = FSizeJ - 1;
  if (max_k >= FSizeK) max_k = FSizeK - 1;

  for (int i=min_i ; i<=max_i ; i++)
    for (int j=min_j ; j<=max_j ; j++)
      for (int k=min_k ; k<=max_k ; k++)
  FGrid[i][j][k] = AObj;
}
//******************************************************************************
template <class T>
TCoordinate CGrid3d<T>::getCellI(TCoordinate Ax) const
{
  TCoordinate size = FBB.getMaxBound().getX() - FBB.getMinBound().getX();

  if (size == 0.0)
    return 0.0;
  else
    return (((Ax - FBB.getMinBound().getX()) * FSizeI) / size);
}
//******************************************************************************
template <class T>
TCoordinate CGrid3d<T>::getCellJ(TCoordinate Ay) const
{
  TCoordinate size = FBB.getMaxBound().getY() - FBB.getMinBound().getY();

  if (size == 0.0)
    return 0.0;
  else
    return (((Ay - FBB.getMinBound().getY()) * FSizeJ) / size);
}
//******************************************************************************
template <class T>
TCoordinate CGrid3d<T>::getCellK(TCoordinate Az) const
{
  TCoordinate size = FBB.getMaxBound().getZ() - FBB.getMinBound().getZ();

  if (size == 0.0)
    return 0.0;
  else
    return (((Az - FBB.getMinBound().getZ()) * FSizeK) / size);
}
/******************************************************************************
 * CGrid3dIterator                                                            *
 ******************************************************************************/
template <class T> inline
CGrid3dIterator<T>::CGrid3dIterator(const CGrid3d<T> & AGrid)
  : FGrid(AGrid),
    FMinI(0), FMinJ(0), FMinK(0),
    FMaxI(AGrid.getSizeI() - 1),
    FMaxJ(FGrid.getSizeJ() - 1),
    FMaxK(FGrid.getSizeK() - 1)
{
  reinit();
}
//******************************************************************************
template <class T> inline
CGrid3dIterator<T>::CGrid3dIterator(const CGrid3d<T> & AGrid,
            const CBoundingBox & ABB)
  : FGrid(AGrid)
{
  FGrid.getCellPosition(ABB.getEpsMinBound(), &FMinI, &FMinJ, &FMinK);
  FGrid.getCellPosition(ABB.getEpsMaxBound(), &FMaxI, &FMaxJ, &FMaxK);

  if (FMinI < 0) FMinI = 0;
  if (FMinJ < 0) FMinJ = 0;
  if (FMinK < 0) FMinK = 0;

  if (FMaxI >= FGrid.getSizeI()) FMaxI = FGrid.getSizeI() - 1;
  if (FMaxJ >= FGrid.getSizeJ()) FMaxJ = FGrid.getSizeJ() - 1;
  if (FMaxK >= FGrid.getSizeK()) FMaxK = FGrid.getSizeK() - 1;

  reinit();
}
//******************************************************************************
template <class T> inline
CGrid3dIterator<T>::CGrid3dIterator(const CGrid3dIterator<T> & AI)
  : FGrid(AI.FGrid), Fi(AI.Fi), Fj(AI.Fj), Fk(AI.Fk),
    FMinI(AI.FMinI), FMinJ(AI.FMinJ), FMinK(AI.FMinK),
    FMaxI(AI.FMaxI), FMaxJ(AI.FMaxJ), FMaxK(AI.FMaxK)
{
}
//******************************************************************************
template <class T> inline
CGrid3dIterator<T>::~CGrid3dIterator()
{
}
//******************************************************************************
template <class T> inline
void CGrid3dIterator<T>::reinit()
{
  Fi = FMinI;
  Fj = FMinJ;
  Fk = FMinK;
}
//******************************************************************************
template <class T> inline
bool CGrid3dIterator<T>::cont() const
{
  return (Fi <= FMaxI && Fj <= FMaxJ && Fk <= FMaxK);
}
//******************************************************************************
template <class T> inline
void CGrid3dIterator<T>::next()
{
  assert(cont());

  if (Fi < FMaxI)
    Fi++;
  else {
    Fi = FMinI;

    if (Fj < FMaxJ)
      Fj++;
    else {
      Fj = FMinJ;

      if (Fk < FMaxK)
  Fk++;
      else {
  Fi = FMaxI + 1;
  Fj = FMaxJ + 1;
  Fk = FMaxK + 1;
      }
    }
  }
}
//******************************************************************************
#endif
//******************************************************************************
