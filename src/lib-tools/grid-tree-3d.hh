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

#ifndef GRID_TREE_3D_HH
#define GRID_TREE_3D_HH

#include "inline-macro.hh"
#include "grid-3d.hh"

/** Classes définissant un arbre de grilles régulières 3D.
 *
 * @author Nicolas Guiard
 */

//******************************************************************************

class CGridTree3dNode
{
public:

  virtual ~CGridTree3dNode() {};

private:
};

//******************************************************************************

template <class T>
class CGridTree3dLeaf : public CGridTree3dNode
{
public:

  CGridTree3dLeaf() {}
  CGridTree3dLeaf(const T & AData) : FData(AData) {}
  virtual ~CGridTree3dLeaf() {}

  void setData(const T & AData) { FData = AData; }
  const T & getData() const { return FData; }

private:

  T FData;
};

//******************************************************************************

template <class T> class CGridTree3dIterator;

template <class T>
class CGridTree3d : public CGridTree3dNode
{
public:

  /** @name Constructeurs et destructeur
   */

  //@{

  /** Constructeur par défaut
   *
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle instance de la classe
   * initialisée avec...
   */

  CGridTree3d(int ASizeI, int ASizeJ, int ASizeK,
        const CBoundingBox & ABB);

  /** Destructeur
   *
   * Destruction de l'instance.
   */

  virtual ~CGridTree3d();

  //@}

  /** @name Méthodes publiques
   */

  //@{

  int getSizeI() const;
  int getSizeJ() const;
  int getSizeK() const;

  CGrid3d<CGridTree3dNode*> & getGrid();

  const T & getCell(const CGridTree3dIterator<T> & AIter) const;

  CBoundingBox getCellBoundingBox(const CGridTree3dIterator<T> & AIter) const;

  void setCell(const CGridTree3dIterator<T> & AIter, const T & AObj);

  CGridTree3d<T> * splitCellInGrid(const CGridTree3dIterator<T> & AIter,
           int ASizeI, int ASizeJ, int ASizeK);

  //@}

private:

  /** @name Méthodes privées
   */

  //@{

  //@}

private:

  /** Attributs privés
   */

  //@{

  CGrid3d<CGridTree3dNode*> FGrid;

  //@}
};

//******************************************************************************

template <class T>
class CGridTree3dIterator
{
public:
  CGridTree3dIterator(CGridTree3d<T> & AGrid);
  CGridTree3dIterator(CGridTree3d<T> & AGrid, const CBoundingBox & ABB);
  CGridTree3dIterator(const CGridTree3dIterator<T> & AI);
  virtual ~CGridTree3dIterator();

  virtual void reinit();
  virtual bool cont() const;
  virtual void next();

  int getI() const { return FIter.getI(); }
  int getJ() const { return FIter.getJ(); }
  int getK() const { return FIter.getK(); }

  const CGrid3dIterator<CGridTree3dNode*> & getIter() const { return FIter; }

  CGridTree3d<T> & getGridTree() const { return FGrid; }
  const CGridTree3dIterator<T> & getGoodIter() const
  {
    if (FSubIter != NULL)
      return FSubIter->getGoodIter();
    else
      return *this;
  }

  const T & getCurrent() const { return FGrid.getCell(*this); }
  const T & operator*() const { return getCurrent(); }
  const T * operator->() const { return &(getCurrent()); }

  CGridTree3dIterator<T> & operator++()
  {
    next();
    return *this;
  }

  CGridTree3dIterator<T> operator++(int)
  {
    CGridTree3dIterator<T> tmp = *this;
    next();
    return tmp;
  }

  friend std::ostream & operator<<(std::ostream & AO,
           const CGridTree3dIterator<T> & AI)
  {
    AO << AI.FIter;

    if (AI.FSubIter != NULL)
      AO << " --> " << *AI.FSubIter;

    return AO;
  }

protected:
  virtual void createSubIter();
  virtual void deleteSubIter();

protected:
  CGridTree3d<T> & FGrid;
  CGrid3dIterator<CGridTree3dNode*> FIter;
  CGridTree3dIterator<T> * FSubIter;
  CBoundingBox FBB;
};

/******************************************************************************
 * CLASSES BODIES                                                             *
 ******************************************************************************/

template <class T>
CGridTree3d<T>::CGridTree3d(int ASizeI, int ASizeJ, int ASizeK,
          const CBoundingBox & ABB)
  : FGrid(ASizeI, ASizeJ, ASizeK, ABB)
{
//   cout << "Boîte englobante de la grille : ["
//        << ABB.getMinBound() << "," << ABB.getMaxBound() << "]" << endl;

  for (CGrid3dIterator<CGridTree3dNode*> gi(FGrid); gi.cont(); ++gi)
    FGrid.setCell(gi, new CGridTree3dLeaf<T>);
}

//******************************************************************************

template <class T>
CGridTree3d<T>::~CGridTree3d()
{
  for (CGrid3dIterator<CGridTree3dNode*> gi(FGrid); gi.cont(); ++gi)
    delete *gi;
}

//******************************************************************************

template <class T> inline
int CGridTree3d<T>::getSizeI() const
{
  return FGrid.getSizeI();
}

//******************************************************************************

template <class T> inline
int CGridTree3d<T>::getSizeJ() const
{
  return FGrid.getSizeJ();
}

//******************************************************************************

template <class T> inline
int CGridTree3d<T>::getSizeK() const
{
  return FGrid.getSizeK();
}

//******************************************************************************

template <class T> inline
CGrid3d<CGridTree3dNode*> & CGridTree3d<T>::getGrid()
{
  return FGrid;
}

//******************************************************************************

template <class T> inline
const T & CGridTree3d<T>::getCell(const CGridTree3dIterator<T> & AIter) const
{
  const CGridTree3dIterator<T> & it = AIter.getGoodIter();
//   CGridTree3dNode * node = it.getGridTree().getGrid().getCell(it.getIter());
  CGridTree3dNode * node = *it.getIter();
  CGridTree3dLeaf<T> * leaf = dynamic_cast<CGridTree3dLeaf<T>*>(node);

  assert(leaf != NULL);

  return leaf->getData();
}

//******************************************************************************

template <class T> inline
CBoundingBox
CGridTree3d<T>::getCellBoundingBox(const CGridTree3dIterator<T> & AIter) const
{
  const CGridTree3dIterator<T> & it = AIter.getGoodIter();
  return it.getGridTree().getGrid().getCellBoundingBox(it.getIter());
}

//******************************************************************************

template <class T> inline
void CGridTree3d<T>::setCell(const CGridTree3dIterator<T> & AIter,
           const T & AObj)
{
  const CGridTree3dIterator<T> & it = AIter.getGoodIter();
  CGridTree3dNode * node = *it.getIter();
  CGridTree3dLeaf<T> * leaf = dynamic_cast<CGridTree3dLeaf<T>*>(node);

  assert(leaf != NULL);

  leaf->setData(AObj);
}

//******************************************************************************

template <class T> inline
CGridTree3d<T> *
CGridTree3d<T>::splitCellInGrid(const CGridTree3dIterator<T> & AIter,
        int ASizeI, int ASizeJ, int ASizeK)
{
  const CGridTree3dIterator<T> & it = AIter.getGoodIter();
  CGrid3d<CGridTree3dNode*> & grid = it.getGridTree().getGrid();
  CGridTree3dNode * node = *it.getIter();
  CBoundingBox bb = grid.getCellBoundingBox(it.getIter());

  delete node;

  CGridTree3d<T> * new_grid = new CGridTree3d<T>(ASizeI,
             ASizeJ,
             ASizeK,
             bb);
  grid.setCell(it.getIter(), new_grid);

  return new_grid;
}

/******************************************************************************
 * CGridTree3dIterator                                                        *
 ******************************************************************************/

template <class T> inline
CGridTree3dIterator<T>::CGridTree3dIterator(CGridTree3d<T> & AGrid)
  : FGrid(AGrid), FIter(AGrid.getGrid()), FSubIter(NULL), FBB()
{
  reinit();
}

//******************************************************************************

template <class T> inline
CGridTree3dIterator<T>::CGridTree3dIterator(CGridTree3d<T> & AGrid,
              const CBoundingBox & ABB)
  : FGrid(AGrid), FIter(AGrid.getGrid(), ABB), FSubIter(NULL), FBB(ABB)
{
  reinit();
}

//******************************************************************************

template <class T> inline
CGridTree3dIterator<T>::CGridTree3dIterator(const CGridTree3dIterator<T> & AI)
  : FGrid(AI.FGrid), FIter(AI.FIter), FBB(AI.FBB)
{
  if (AI.FSubIter == NULL)
    FSubIter = NULL;
  else
    FSubIter = new CGridTree3dIterator<T>(*AI.FSubIter);
}

//******************************************************************************

template <class T> inline
CGridTree3dIterator<T>::~CGridTree3dIterator()
{
  deleteSubIter();
}

//******************************************************************************

template <class T> inline
void CGridTree3dIterator<T>::reinit()
{
  FIter.reinit();

  deleteSubIter();

  if (cont())
    createSubIter();
}

//******************************************************************************

template <class T> inline
bool CGridTree3dIterator<T>::cont() const
{
  return FIter.cont();
}

//******************************************************************************

template <class T> inline
void CGridTree3dIterator<T>::next()
{
  if (FSubIter != NULL) {
    if (FSubIter->cont())
      FSubIter->next();

    if (!FSubIter->cont()) {
      delete FSubIter;
      FSubIter = NULL;
    }
  }

  if (FSubIter == NULL) {
    FIter.next();

    if (cont())
      createSubIter();
  }
}

//******************************************************************************

template <class T> inline
void CGridTree3dIterator<T>::createSubIter()
{
  CGridTree3d<T> * grid = dynamic_cast<CGridTree3d<T>*>(*FIter);

  if (grid != NULL) {
    if (FBB.isEmpty())
      FSubIter = new CGridTree3dIterator<T>(*grid);
    else
      FSubIter = new CGridTree3dIterator<T>(*grid, FBB);

    assert(FSubIter->cont());
  }
}

//******************************************************************************

template <class T> inline
void CGridTree3dIterator<T>::deleteSubIter()
{
  if (FSubIter != NULL) {
    delete FSubIter;
    FSubIter = NULL;
  }
}

//******************************************************************************

#endif
