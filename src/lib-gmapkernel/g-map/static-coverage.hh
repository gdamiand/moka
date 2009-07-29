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
#ifndef STATIC_COVERAGE_HH
#define STATIC_COVERAGE_HH
//******************************************************************************
#include "coverage.hh"
#include "dynamic-coverage.hh"
#include "inline-macro.hh"

#include <list>
//******************************************************************************
namespace GMap3d
{
  //----------------------------------------------------------------------------
  /**
   * Itérateur statique correspondant à une orbite n'utilisant qu'une seule
   * involution
   *
   * @author MOKA TEAM
   */
  template <int Ai>
  class CTemplateStaticCoverage1 : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin de départ
    CDart* FDart1;

    /// Deuxième brin
    CDart* FDart2;

    /// Booléen indiquant si le parcours n'est pas terminé.
    bool FDoContinue;

    /// Booléen indiquant si l'on est sur le premier brin du parcours.
    bool FDart1Active;

    // @}

  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CTemplateStaticCoverage1(CGMapBasic* AGMap, const CDart* ADart1);
    /// Destructeur
    virtual ~CTemplateStaticCoverage1();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur statique correspondant à une orbite utilisant deux involutions
   * dont la composée est une involution (<0,2> <0,3> ou <1,3>).
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj>
  class CTemplateStaticCoverage2a : public CCoverage
  {
  private:
    // @name Champs privés
    //@{

    /// Tableau de Brins (4 au maximum)
    CDart* FDart[4];

    /// Brin courant
    int FIndex;

    //@}

  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CTemplateStaticCoverage2a(CGMapBasic* AGMap, const CDart* ADart1);
    /// Destructeur
    virtual ~CTemplateStaticCoverage2a();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    int type() const;

    // @}
    };

  //----------------------------------------------------------------------------
  /**
   * Itérateur statique générique
   *
   * @author MOKA TEAM
   */
  class CGenericStaticCoverage : public CCoverage
  {
  protected:
    // @name Champs protégés
    // @{

    std::list<CDart*> FAllDarts;
    std::list<CDart*>::iterator FIt;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    // Constructeur
    CGenericStaticCoverage(CGMapBasic* AGMap);
    // Destructeur
    virtual ~CGenericStaticCoverage();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();

    // @}
    };

  //----------------------------------------------------------------------------
  /**
   * Itérateur statique correspondant à une orbite utilisant deux involutions
   * dont la composée n'est pas une involution (<0,1> <1,2> ou <2,3>).
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj>
  class CTemplateStaticCoverage2b : public CGenericStaticCoverage
  {
  public:
    //@name Constructeurs et destructeur
    // @{

    // Constructeur
    CTemplateStaticCoverage2b(CGMapBasic* AGMap, const CDart* ADart1);
    // Destructeur
    virtual ~CTemplateStaticCoverage2b();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{
    int type() const;
    // @}

    };

  //----------------------------------------------------------------------------
  /**
   * Itérateur statique correspondant à une orbite utilisant trois involutions
   * non consécutives (<0,1,3> ou <0,2,3>).
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj, int Ak>
  class CTemplateStaticCoverage3a : public CGenericStaticCoverage
  {
  public:
    // @name Constructeur et destructeur
    // @{

    // Constructeur
    CTemplateStaticCoverage3a(CGMapBasic* AGMap, const CDart* ADart1);
    // Destructeur
    virtual ~CTemplateStaticCoverage3a();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{
    int type() const;
    // @}

    };

  //----------------------------------------------------------------------------
  /**
   * Itérateur statique correspondant à une orbite utilisant trois involutions
   * consécutives (<0,1,2> ou <1,2,3>).
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj, int Ak>
  class CTemplateStaticCoverage3b : public CGenericStaticCoverage
  {
  public:
    // @name Constructeur et destructeur
    // @{

    // Constructeur
    CTemplateStaticCoverage3b(CGMapBasic* AGMap, const CDart* ADart1);

    // Destructeur
    virtual ~CTemplateStaticCoverage3b();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{
    int type() const;
    // @}

    };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite utilisant toutes les involutions
   * (<0,1,2,3>).
   *
   * @author MOKA TEAM
   */
  class CTemplateStaticCoverage4 : public CGenericStaticCoverage
  {
  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CTemplateStaticCoverage4(CGMapBasic* AGMap, const CDart* ADart1);
    /// Destructeur
    virtual ~CTemplateStaticCoverage4();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{
    int type() const;
    // @}

    };

  //----------------------------------------------------------------------------
  /**
   * Itérateur statique permettant de parcourir tous les brins de la carte.
   *
   * @author MOKA TEAM
   */
  class CStaticCoverageAll : public CGenericStaticCoverage
  {
  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CStaticCoverageAll(CGMapBasic* AGMap);

    /// Destructeur
    virtual ~CStaticCoverageAll();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{
    int type() const;
    // @}

    };

  //----------------------------------------------------------------------------
  /**
   * Itérateur statique correspondant à une orbite de bord.
   *
   * @author MOKA TEAM
   */
  class CStaticBorderCoverage : public CGenericStaticCoverage
  {
  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CStaticBorderCoverage(CGMapBasic* AGMap, const CDart* ADart1, int);

    /// Destructeur
    virtual ~CStaticBorderCoverage();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{
    int type() const;
    // @}

    };

  //----------------------------------------------------------------------------
  // @name Noms usuels des itérateurs
  // @{

  class CTemplateDynamicCoverage0;

  typedef CTemplateDynamicCoverage0        CTemplateStaticCoverage0;
  typedef CTemplateStaticCoverage0         CStaticCoverageSelf;

  typedef CTemplateStaticCoverage1<0>      CStaticCoverage0;
  typedef CTemplateStaticCoverage1<1>      CStaticCoverage1;
  typedef CTemplateStaticCoverage1<2>      CStaticCoverage2;
  typedef CTemplateStaticCoverage1<3>      CStaticCoverage3;

  typedef CTemplateStaticCoverage2a<0,2>   CStaticCoverage02;
  typedef CTemplateStaticCoverage2a<0,3>   CStaticCoverage03;
  typedef CTemplateStaticCoverage2a<1,3>   CStaticCoverage13;

  typedef CTemplateStaticCoverage2b<0,1>   CStaticCoverage01;
  typedef CTemplateStaticCoverage2b<1,2>   CStaticCoverage12;
  typedef CTemplateStaticCoverage2b<2,3>   CStaticCoverage23;

  typedef CTemplateStaticCoverage3a<0,1,3> CStaticCoverage013;
  typedef CTemplateStaticCoverage3a<2,3,0> CStaticCoverage023;

  typedef CTemplateStaticCoverage3b<0,1,2> CStaticCoverage012;
  typedef CTemplateStaticCoverage3b<1,2,3> CStaticCoverage123;

  typedef CTemplateStaticCoverage4         CStaticCoverage0123;

  /// Itérateurs correspondant aux cellules
  typedef CStaticCoverage123      CStaticCoverageVertex;
  typedef CStaticCoverage023      CStaticCoverageEdge;
  typedef CStaticCoverage013      CStaticCoverageFace;
  typedef CStaticCoverage012      CStaticCoverageVolume;
  typedef CStaticCoverage0123     CStaticCoverageCC;

  // Autres itérateurs définis directement :
  // CStaticCoverageAll(CGMapBasic*)
  // CStaticBorderCoverage(CGMapBasic*, CDart*, int dim)

} // namespace GMap3d

//******************************************************************************
//******************************************************************************
//******************************************************************************
//- INLINE CODE
//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "g-map-basic.hh"
#include "dynamic-coverage.hh"
#include <cassert>
//******************************************************************************
namespace GMap3d
{
//******************************************************************************
// CTemplateStaticCoverage1
//******************************************************************************
template <int Ai>
inline
void CTemplateStaticCoverage1<Ai>::reinit()
{
  FDoContinue = true;
  FDart1Active = true;
}
//******************************************************************************
template <int Ai>
inline
CTemplateStaticCoverage1<Ai>::CTemplateStaticCoverage1(CGMapBasic* AGMap,
						       const CDart* ADart1) :
  CCoverage(AGMap), FDart1((CDart*) ADart1)
{
  if (AGMap->isFree(FDart1, Ai))
    FDart2 = NULL;
  else
    FDart2 = AGMap->alpha(FDart1, Ai);

  reinit();
}
//******************************************************************************
template <int Ai>
inline
CTemplateStaticCoverage1<Ai>::~CTemplateStaticCoverage1()
{
}
//******************************************************************************
template <int Ai>
inline
bool CTemplateStaticCoverage1<Ai>::cont()
{
  return FDoContinue;
}

//******************************************************************************
template <int Ai>
inline
void CTemplateStaticCoverage1<Ai>::operator++()
{
  if (! FDart1Active || FMap->isFree(FDart1, Ai))
    FDoContinue = false;
  else
    FDart1Active = false;
}
//******************************************************************************
template <int Ai>
inline
CDart* CTemplateStaticCoverage1<Ai>::operator*()
{
  assert(cont());

  if (FDart1Active)
    return FDart1;
  else
    return FDart2;
}
//******************************************************************************
template <int Ai>
inline
CDart* CTemplateStaticCoverage1<Ai>::operator ++(int)
{
  CDart* tmp = operator*();
  operator++();
  return tmp;
}
//******************************************************************************
template <int Ai>
inline
int CTemplateStaticCoverage1<Ai>::type() const
{
  return COVERAGE1;
}
//******************************************************************************
// CTemplateStaticCoverage2a
//******************************************************************************
template <int Ai, int Aj>
inline
void CTemplateStaticCoverage2a<Ai,Aj>::reinit()
{
  FIndex = 0;
}
//******************************************************************************
template <int Ai, int Aj>
inline
CTemplateStaticCoverage2a<Ai,Aj>::CTemplateStaticCoverage2a(CGMapBasic* AGMap,
							    const CDart* ADart1)
  : CCoverage(AGMap)
{
  int i = 0;
  CTemplateDynamicCoverage2a<Ai,Aj> it(AGMap, ADart1);

  while (it.cont())
    FDart[i++] = it++;

  while (i<4)
    FDart[i++] = NULL;

  reinit();
}
//******************************************************************************
template <int Ai, int Aj>
inline
CTemplateStaticCoverage2a<Ai,Aj>::~CTemplateStaticCoverage2a()
{
}
//******************************************************************************
template <int Ai, int Aj>
inline
bool CTemplateStaticCoverage2a<Ai,Aj>::cont()
{
  if (FIndex > 3)
    return false;
  else
    return FDart[FIndex] != NULL;
}
//******************************************************************************
template <int Ai, int Aj>
inline
void CTemplateStaticCoverage2a<Ai,Aj>::operator++()
{
  assert (cont());
  ++ FIndex;
}
//******************************************************************************
template <int Ai, int Aj>
inline
CDart* CTemplateStaticCoverage2a<Ai,Aj>::operator*()
{
  assert(cont());
  return FDart[FIndex];
}
//******************************************************************************
template <int Ai, int Aj>
inline
CDart* CTemplateStaticCoverage2a<Ai,Aj>::operator ++(int)
{
  CDart* tmp = operator *();
  operator++();
  return tmp;
}
//******************************************************************************
template <int Ai, int Aj>
inline
int CTemplateStaticCoverage2a<Ai,Aj>::type() const
{
  return COVERAGE2A;
}
//******************************************************************************
// CGenericStaticCoverage
//******************************************************************************
inline
void CGenericStaticCoverage::reinit()
{
  FIt = FAllDarts.begin();
}
//******************************************************************************
inline
CGenericStaticCoverage::CGenericStaticCoverage(CGMapBasic* AGMap) :
  CCoverage(AGMap)
{
}
//******************************************************************************
inline
CGenericStaticCoverage::~CGenericStaticCoverage()
{
}
//******************************************************************************
inline
bool CGenericStaticCoverage::cont()
{
  return FIt != FAllDarts.end();
}
//******************************************************************************
inline
void CGenericStaticCoverage::operator++()
{
  assert(cont());

  ++ FIt;
}
//******************************************************************************
inline
CDart* CGenericStaticCoverage::operator*()
{
  assert(cont());

  return * FIt;
}
//******************************************************************************
inline
CDart* CGenericStaticCoverage::operator++(int)
{
  CDart* tmp = operator *();
  operator ++();
  return tmp;
}
//******************************************************************************
// CTemplateStaticCoverage2b
//******************************************************************************
template <int Ai, int Aj>
inline
CTemplateStaticCoverage2b<Ai,Aj>::CTemplateStaticCoverage2b(CGMapBasic* AGMap,
							    const CDart* ADart1)
  : CGenericStaticCoverage(AGMap)
{
  CTemplateDynamicCoverage2b<Ai,Aj> it(AGMap, ADart1);

  while (it.cont())
    FAllDarts.push_back(it++);

  reinit();
}
//******************************************************************************
template <int Ai, int Aj>
inline
CTemplateStaticCoverage2b<Ai,Aj>::~CTemplateStaticCoverage2b()
{
}
//******************************************************************************
template <int Ai, int Aj>
inline
int CTemplateStaticCoverage2b<Ai,Aj>::type() const
{
  return COVERAGE2B;
}
//******************************************************************************
// CTemplateStaticCoverage3a
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateStaticCoverage3a<Ai,Aj,Ak>::
CTemplateStaticCoverage3a(CGMapBasic* AGMap, const CDart* ADart1) :
  CGenericStaticCoverage(AGMap)
{
  CTemplateDynamicCoverage3a<Ai,Aj,Ak> it(AGMap, ADart1);

  while (it.cont())
    FAllDarts.push_back(it++);

  reinit();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateStaticCoverage3a<Ai,Aj,Ak>::~CTemplateStaticCoverage3a()
{
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
int CTemplateStaticCoverage3a<Ai,Aj,Ak>::type() const
{
  return COVERAGE3A;
}
//******************************************************************************
// CTemplateStaticCoverage3b
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateStaticCoverage3b<Ai,Aj,Ak>::
CTemplateStaticCoverage3b(CGMapBasic* AGMap, const CDart* ADart1) :
  CGenericStaticCoverage(AGMap)
{
  int markNumber = AGMap->getNewMark();
  CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak> it(AGMap, ADart1, markNumber);

  while (it.cont())
    FAllDarts.push_back(it++);

  FIt = FAllDarts.begin();

  while (FIt != FAllDarts.end())
    {
      AGMap->unsetMark(* FIt, markNumber);
      ++ FIt;
    }

  AGMap->freeMark(markNumber);

  reinit();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateStaticCoverage3b<Ai,Aj,Ak>::~CTemplateStaticCoverage3b()
{
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
int CTemplateStaticCoverage3b<Ai,Aj,Ak>::type() const
{
  return COVERAGE3B;
}
//******************************************************************************
// CTemplateStaticCoverage4
//******************************************************************************
inline
CTemplateStaticCoverage4::CTemplateStaticCoverage4(CGMapBasic* AGMap,
						   const CDart* ADart1) :
  CGenericStaticCoverage(AGMap)
{
  int markNumber = AGMap->getNewMark();
  CTemplateBasicDynamicCoverage4 it(AGMap, ADart1, markNumber);

  while (it.cont())
    FAllDarts.push_back(it++);

  FIt = FAllDarts.begin();

  while (FIt != FAllDarts.end())
    {
      AGMap->unsetMark(* FIt, markNumber);
      ++ FIt;
    }

  AGMap->freeMark(markNumber);

  reinit();
}
//******************************************************************************
inline
CTemplateStaticCoverage4::~CTemplateStaticCoverage4()
{
}
//******************************************************************************
inline
int CTemplateStaticCoverage4::type() const
{
  return COVERAGE4;
}
//******************************************************************************
// CStaticCoverageAll
//******************************************************************************
inline
CStaticCoverageAll::CStaticCoverageAll(CGMapBasic* AGMap)
  : CGenericStaticCoverage(AGMap)
{
  CDynamicCoverageAll it(AGMap);

  while (it.cont())
    FAllDarts.push_back(it++);

  reinit();
}
//******************************************************************************
inline
CStaticCoverageAll::~CStaticCoverageAll()
{
}
//******************************************************************************
inline
int CStaticCoverageAll::type() const
{
  return COVERAGE_ALL;
}
//******************************************************************************
// CStaticBorderCoverage
//******************************************************************************
inline
CStaticBorderCoverage::CStaticBorderCoverage(CGMapBasic* AGMap,
					     const CDart* ADart1,
					     int ADimension) :
  CGenericStaticCoverage(AGMap)
{
  int markNumber = AGMap->getNewMark();
  CBasicBorderCoverage it(AGMap, ADart1, markNumber, ADimension);

  while (it.cont())
    FAllDarts.push_back(it++);

  FIt = FAllDarts.begin();

  while (FIt != FAllDarts.end())
    {
      AGMap->unsetMark(* FIt, markNumber);
      ++ FIt;
    }

  AGMap->freeMark(markNumber);

  reinit();
}
//******************************************************************************
inline
CStaticBorderCoverage::~CStaticBorderCoverage()
{
}
//******************************************************************************
inline
int CStaticBorderCoverage::type() const
{
  return COVERAGE_BORDER;
}
//******************************************************************************
} // namespace GMap3d
//******************************************************************************
#endif // STATIC_COVERAGE_HH
//******************************************************************************
