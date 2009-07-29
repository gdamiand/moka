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
#ifndef DYNAMIC_COVERAGE_HH
#define DYNAMIC_COVERAGE_HH
//******************************************************************************
#include "coverage.hh"
#include "inline-macro.hh"

#include <list>
#include <queue>
#include <stack>
//******************************************************************************
namespace GMap3d
{
  class CDart;
  class CGMapBasic;

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite n'utilisant aucune involution
   * (ORBIT_SELF).
   *
   * @author MOKA TEAM
   */
  class CTemplateDynamicCoverage0 : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin unique de l'orbite
    CDart* FSelf;

    /// Booléen indiquant si le parcours n'est pas terminé.
    bool FDoContinue;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CTemplateDynamicCoverage0(CGMapBasic* AGMap, const CDart* ADart);
    /// Destructeur
    virtual ~CTemplateDynamicCoverage0();

    // @}
    // @name Opérations de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    //@}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite n'utilisant qu'une seule involution.
   *
   * @author MOKA TEAM
   */
  template <int Ai>
  class CTemplateDynamicCoverage1 : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin de départ
    CDart* FDart1;

    /// Booléen indiquant si le parcours n'est pas terminé.
    bool FDoContinue;

    /// Booléen indiquant si l'on est sur le premier brin du parcours.
    bool FDart1Active;

    // @}

  public:	
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CTemplateDynamicCoverage1(CGMapBasic* AGMap, const CDart* ADart);
    /// Destructeur
    virtual ~CTemplateDynamicCoverage1();

    // @}
  	
    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite utilisant deux involutions dont la
   * composée est une involution (<0,2> <0,3> ou <1,3>).
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj>
  class CTemplateDynamicCoverage2a : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin de départ
    CDart* FDart1;
    /// Brin courant
    CDart* FDartCurrent;

    /**
     * Booléen indiquant quelle est la prochaine involution à utiliser pour
     * le parcours (Ai ou Aj).
     */
    bool FEven;

    /// Type de la dernière opération réalisée
    TOperationState FPrevOp;

    // @}

  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CTemplateDynamicCoverage2a(CGMapBasic* AGMap, const CDart* ADart);
    /// Destructeur
    virtual ~CTemplateDynamicCoverage2a();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite utilisant deux involutions dont la
   * composée n'est pas une involution (<0,1> <1,2> ou <2,3>).
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj>
  class CTemplateDynamicCoverage2b : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin de départ
    CDart* FDart1;
    /// Brin courant
    CDart* FDartCurrent;

    /**
     * Sens de déplacement sur l'orbite
     *
     * L'orbite est soit fermée (et forme un cycle), soit ouverte.
     * Dans le cas d'une orbite ouverte, il est nécessaire de repartir du
     * brin de départ dans la deuxième directement lorsqu'on a atteint le
     * premier bord.
     * Ce booléen est à false si le premier bord a déjà été atteint.
     */
    bool FFirstDirection;

    /**
     * Booléen indiquant quelle est la prochaine involution à utiliser pour
     * le parcours (Ai ou Aj).
     */
    bool FNextTryAi;

    /// Type de la dernière opération réalisée
    TOperationState FPrevOp;

    // @}

  public:
    // @name Constructeur et destructeurs
    // @{

    /// Constructeur
    CTemplateDynamicCoverage2b(CGMapBasic* AGMap, const CDart* ADart);
    /// Destructeur
    virtual ~CTemplateDynamicCoverage2b();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite utilisant trois involutions non
   * consécutives (<0,1,3> ou <0,2,3>).
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj, int Ak>
  class CTemplateDynamicCoverage3a : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// On utilise le parcours <Ai,Aj>
    CTemplateDynamicCoverage2b<Ai,Aj> FIt;

    /// Indique si tous les brins de <Ai,Aj> sont liés par Ak
    bool FAkExist;

    /// Indique si le brin courant est sur le même bord que D1 par Ak
    bool FFirstBorder;

    /// Indique si la dernière opération a porté sur Ak
    bool FPrevAk;

    /// Type de la dernière opération réalisée
    TOperationState FPrevOp;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CTemplateDynamicCoverage3a(CGMapBasic* AGMap, const CDart* ADart);
    /// Destructeur
    virtual ~CTemplateDynamicCoverage3a();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur basique correspondant à une orbite utilisant trois involutions
   * consécutives (<0,1,2> ou <1,2,3>).
   * Cet itérateur ne démarque pas les brins lorsqu'il est détruit.
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj, int Ak>
  class CTemplateBasicDynamicCoverage3b : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin courant
    CDart* FDartCurrent;

    /// Marque utilisée pour le marquage des brins déjà traités
    int FMarkNumber;

    /// File d'attente des brins à traiter
    std::queue<CDart*> FToTreat;

    /// Type de la dernière opération réalisée
    TOperationState FPrevOp;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CTemplateBasicDynamicCoverage3b(CGMapBasic* AGMap,
				    const CDart* ADart, int AMarkNumber);
    /// Destructeur
    virtual ~CTemplateBasicDynamicCoverage3b();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite utilisant toutes les involutions
   * (<0,1,2,3>).
   * Cet itérateur ne démarque pas les brins lorsqu'il est détruit.
   *
   * @author MOKA TEAM
   */
  class CTemplateBasicDynamicCoverage4 : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin courant
    CDart* FDartCurrent;

    /// Marque utilisée pour le marquage des brins déjà traités
    int FMarkNumber;

    /// File d'attente des brins à traiter
    std::queue<CDart*> FToTreat;

    /// Type de la dernière opération réalisée
    TOperationState FPrevOp;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CTemplateBasicDynamicCoverage4(CGMapBasic* AGMap, const CDart* ADart,
				   int AMarkNumber);
    /// Destructeur
    virtual ~CTemplateBasicDynamicCoverage4();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite utilisant trois involutions
   * consécutives (<0,1,2> ou <1,2,3>).
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj, int Ak>
  class CTemplateDynamicCoverage3b : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin initial du parcours
    CDart* FDart1;

    /// Marque utilisée pour le marquage des brins déjà traités
    int FMarkNumber;

    /// Itérateur basique utilisé ne démarquant pas les brins
    CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>* FIt;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CTemplateDynamicCoverage3b(CGMapBasic* AGMap, const CDart* ADart);
    /// Destructeur
    virtual ~CTemplateDynamicCoverage3b();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
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
  class CTemplateDynamicCoverage4 : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin initial du parcours
    CDart* FDart1;

    /// Marque utilisée pour le marquage des brins déjà traités
    int FMarkNumber;

    /// Itérateur basique utilisé ne démarquant pas les brins
    CTemplateBasicDynamicCoverage4* FIt;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CTemplateDynamicCoverage4(CGMapBasic* AGMap, const CDart* ADart);
    /// Destructeur
    virtual ~CTemplateDynamicCoverage4();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite utilisant trois involutions
   * consécutives (<0,1,2> ou <1,2,3>).
   *
   * Cet itérateur est optimisé pour les cas où le parcours doit être répété
   * plusieurs fois à la suite.
   *
   * @author MOKA TEAM
   */
  template <int Ai, int Aj, int Ak>
  class CTemplateMultiDynamicCoverage3b : public CCoverage
  {
  private:
    /// @name Champs privés
    // @{

    /// Marque utilisée pour le marquage des brins déjà traités
    int FMarkNumber;

    /// Itérateur basique utilisé ne démarquant pas les brins
    CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>* FIt1;

    /// brins déjà traités mémorisés dans une liste lors du premier parcours
    std::list<CDart*> FAllCoverDarts;

    /// Pour les parcours suivants, c'est la liste qui est parcourue
    std::list<CDart*>::iterator FIt2;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CTemplateMultiDynamicCoverage3b(CGMapBasic* AGMap, const CDart* ADart);
    /// Destructeur
    virtual ~CTemplateMultiDynamicCoverage3b();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite utilisant toutes les involutions
   * (<0,1,2,3>).
   *
   * Cet itérateur est optimisé pour les cas où le parcours doit être répété
   * plusieurs fois à la suite  .
   *
   * @author MOKA TEAM
   */
  class CTemplateMultiDynamicCoverage4 : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Marque utilisée pour le marquage des brins déjà traités
    int FMarkNumber;
 
    /// Itérateur basique utilisé ne démarquant pas les brins
    CTemplateBasicDynamicCoverage4* FIt1;

    /// brins déjà traités mémorisés dans une liste lors du premier parcours
    std::list<CDart*> FAllCoverDarts;

    /// Pour les parcours suivants, c'est la liste qui est parcourue
    std::list<CDart*>::iterator FIt2;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CTemplateMultiDynamicCoverage4(CGMapBasic* AGMap, const CDart* ADart);
    /// Destructeur
    virtual ~CTemplateMultiDynamicCoverage4();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur permettant de parcourir tous les brins de la carte.
   *
   * @author MOKA TEAM
   */
  class CDynamicCoverageAll : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin courant
    CDart* FCurrent;

    // @}

  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CDynamicCoverageAll(CGMapBasic* AGMap);
    /// Destructeur
    virtual ~CDynamicCoverageAll();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur basique correspondant à une orbite de bord.
   * Cet itérateur ne démarque pas les brins lorsqu'il est détruit.
   *
   * @author MOKA TEAM
   */
  class CBasicBorderCoverage : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin courant
    CDart* FCurrentDart;

    /// Marque utilisée pour le marquage des brins déjà traités
    int FMarkNumber;

    /// La dimension du bord
    int FBorderDim;

    /// Liste des brins à traiter
    std::stack<CDart*> FToTreat;

    /// Type de la dernière opération réalisée
    TOperationState FPrevOp;

    // @}

  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CBasicBorderCoverage(CGMapBasic* AGMap, const CDart* ADart, int, int);
    /// Destructeur
    virtual ~CBasicBorderCoverage();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  /**
   * Itérateur correspondant à une orbite de bord.
   *
   * @author MOKA TEAM
   */
  class CDynamicBorderCoverage : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin initial du parcours
    CDart* FDart1;

    /// La dimension du bord
    int FBorderDim;

    /// Marque utilisée pour le marquage des brins déjà traités
    int FMarkNumber;

    /// Itérateur basique utilisé ne démarquant pas les brins
    CBasicBorderCoverage* FIt;

    // @}

  public:
    // @name Constructeurs et destructeur
    // @{

    /// Constructeur
    CDynamicBorderCoverage(CGMapBasic* AGMap, const CDart* ADart, int);
    /// Destructeur
    virtual ~CDynamicBorderCoverage();

    // @}

    // @name Opérateurs de parcours (voir CCoverage)
    // @{

    void reinit();
    void operator++();
    CDart* operator++(int);
    bool cont();
    CDart* operator*();
    TOperationState prevOperationType();
    int type() const;

    // @}
  };

  //----------------------------------------------------------------------------
  // @name Noms usuels des itérateurs
  // @{

  typedef CTemplateDynamicCoverage0         CDynamicCoverageSelf;

  typedef CTemplateDynamicCoverage1<0>      CDynamicCoverage0;
  typedef CTemplateDynamicCoverage1<1>      CDynamicCoverage1;
  typedef CTemplateDynamicCoverage1<2>      CDynamicCoverage2;
  typedef CTemplateDynamicCoverage1<3>      CDynamicCoverage3;

  typedef CTemplateDynamicCoverage2a<0,2>   CDynamicCoverage02;
  typedef CTemplateDynamicCoverage2a<0,3>   CDynamicCoverage03;
  typedef CTemplateDynamicCoverage2a<1,3>   CDynamicCoverage13;

  typedef CTemplateDynamicCoverage2b<0,1>   CDynamicCoverage01;
  typedef CTemplateDynamicCoverage2b<1,2>   CDynamicCoverage12;
  typedef CTemplateDynamicCoverage2b<2,3>   CDynamicCoverage23;

  typedef CTemplateDynamicCoverage3a<0,1,3> CDynamicCoverage013;
  typedef CTemplateDynamicCoverage3a<2,3,0> CDynamicCoverage023;

  typedef CTemplateDynamicCoverage3b<0,1,2> CDynamicCoverage012;
  typedef CTemplateDynamicCoverage3b<1,2,3> CDynamicCoverage123;

  typedef CTemplateDynamicCoverage4         CDynamicCoverage0123;

  /// Itérateurs correspondant aux cellules
  typedef CDynamicCoverage123  CDynamicCoverageVertex;
  typedef CDynamicCoverage023  CDynamicCoverageEdge;
  typedef CDynamicCoverage013  CDynamicCoverageFace;
  typedef CDynamicCoverage012  CDynamicCoverageVolume;
  typedef CDynamicCoverage0123 CDynamicCoverageCC;

  /// Itérateurs basiques correspondant aux cellules
  /// Attention, ces itérateurs ne démarquent pas les brins
  typedef CTemplateBasicDynamicCoverage3b<0,1,2> CBasicDynamicCoverage012;
  typedef CTemplateBasicDynamicCoverage3b<1,2,3> CBasicDynamicCoverage123;
  typedef CTemplateBasicDynamicCoverage4         CBasicDynamicCoverage0123;

  typedef CBasicDynamicCoverage012  CBasicDynamicCoverageVolume;
  typedef CBasicDynamicCoverage123  CBasicDynamicCoverageVertex;
  typedef CBasicDynamicCoverage0123 CBasicDynamicCoverageCC;

  /// Itérateurs pour parcours multiples correspondant aux cellules
  /// Ces itérateurs mémorisent les brins de l'orbite dans une liste lors du
  /// premier parcours.
  typedef CTemplateMultiDynamicCoverage3b<0,1,2> CMultiDynamicCoverage012;
  typedef CTemplateMultiDynamicCoverage3b<1,2,3> CMultiDynamicCoverage123;
  typedef CTemplateMultiDynamicCoverage4         CMultiDynamicCoverage0123;

  typedef CMultiDynamicCoverage012  CMultiDynamicCoverageVolume;
  typedef CMultiDynamicCoverage123  CMultiDynamicCoverageVertex;
  typedef CMultiDynamicCoverage0123 CMultiDynamicCoverageCC;

  // @}

} // namespace GMap3d

//******************************************************************************
//******************************************************************************
//******************************************************************************
//- INLINE CODE
//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
#include "g-map-basic.hh"
#include "dart.hh"

#include <cassert>
//******************************************************************************
namespace GMap3d
{
//******************************************************************************
// CTemplateDynamicCoverage0
//******************************************************************************
inline
void CTemplateDynamicCoverage0::reinit()
{
  FDoContinue = true;
}
//******************************************************************************
inline
int CTemplateDynamicCoverage0::type() const
{
  return COVERAGE0;
}
//******************************************************************************
inline
CTemplateDynamicCoverage0::CTemplateDynamicCoverage0(CGMapBasic* AGMap,
						     const CDart* ADart1) :
  CCoverage(AGMap),
  FSelf	   ((CDart*) ADart1)
{
  reinit();
}
//******************************************************************************
inline
CTemplateDynamicCoverage0::~CTemplateDynamicCoverage0()
{
}
//******************************************************************************
inline
bool CTemplateDynamicCoverage0::cont()
{
  return FDoContinue;
}
//******************************************************************************
inline
void CTemplateDynamicCoverage0::operator ++()
{
  assert(cont());

  FDoContinue = false;
}
//******************************************************************************
inline
CDart* CTemplateDynamicCoverage0::operator*()
{
  assert(cont());

  return FSelf;
}
//******************************************************************************
inline
CDart* CTemplateDynamicCoverage0::operator++(int)
{
  CDart* tmp = operator*();

  operator++();
  return tmp;
}
//******************************************************************************
inline
TOperationState CTemplateDynamicCoverage0::prevOperationType()
{
  return FDoContinue ? OP_NONE : OP_END;
}
//******************************************************************************
// CTemplateDynamicCoverage1
//******************************************************************************
template <int Ai>
inline
void CTemplateDynamicCoverage1<Ai>::reinit()
{
  FDoContinue = true;
  FDart1Active = true;
}
//******************************************************************************
template <int Ai>
inline
CTemplateDynamicCoverage1<Ai>::CTemplateDynamicCoverage1(CGMapBasic* AGMap,
							 const CDart* ADart1) :
  CCoverage(AGMap),
  FDart1   ((CDart*) ADart1)
{
  reinit();
}
//******************************************************************************
template <int Ai>
inline
CTemplateDynamicCoverage1<Ai>::~CTemplateDynamicCoverage1()
{
}
//******************************************************************************
template <int Ai>
inline
bool CTemplateDynamicCoverage1<Ai>::cont()
{
  return FDoContinue;
}
//******************************************************************************
template <int Ai>
inline
void CTemplateDynamicCoverage1<Ai>::operator++()
{
  assert(cont());

  if (! FDart1Active || FMap->isFree(FDart1, Ai))
    FDoContinue = false;

  FDart1Active = false;
}
//******************************************************************************
template <int Ai>
inline
CDart* CTemplateDynamicCoverage1<Ai>::operator*()
{
  assert(cont());

  if (FDart1Active)
    return FDart1;
  else
    return FMap->alpha(FDart1, Ai);
}
//******************************************************************************
template <int Ai>
inline
CDart* CTemplateDynamicCoverage1<Ai>::operator++(int)
{
  CDart* tmp = operator*();

  operator++();
  return tmp;
}
//******************************************************************************
template <int Ai>
inline
TOperationState CTemplateDynamicCoverage1<Ai>::prevOperationType()
{
  if (FDoContinue)
    return FDart1Active ? OP_NONE : OP_ALPHAI;
  else
    return OP_END;
}
//******************************************************************************
template <int Ai>
inline
int CTemplateDynamicCoverage1<Ai>::type() const
{
  return COVERAGE1;
}
//******************************************************************************
// CTemplateDynamicCoverage2a
//******************************************************************************
template <int Ai, int Aj>
inline
void CTemplateDynamicCoverage2a<Ai,Aj>::reinit()
{
  FDartCurrent = FDart1;
  FEven = true;
  FPrevOp = OP_NONE;
}
//******************************************************************************
template <int Ai, int Aj>
inline
CTemplateDynamicCoverage2a<Ai,Aj>::
CTemplateDynamicCoverage2a(CGMapBasic* AGMap, const CDart* ADart1) :
  CCoverage(AGMap),
  FDart1   ((CDart*) ADart1)
{
  reinit();
}
//******************************************************************************
template <int Ai, int Aj>
inline
CTemplateDynamicCoverage2a<Ai,Aj>::~CTemplateDynamicCoverage2a()
{
}
//******************************************************************************
template <int Ai, int Aj>
inline
bool CTemplateDynamicCoverage2a<Ai,Aj>::cont()
{
  return FPrevOp != OP_END;
}
//******************************************************************************
template <int Ai, int Aj>
inline
void CTemplateDynamicCoverage2a<Ai,Aj>::operator++()
{
  assert(cont());

  if (FEven)
    {
      if (FMap->isFree(FDartCurrent, Ai))
        {
          if (FMap->isFree(FDartCurrent, Aj))
	    FPrevOp = OP_END;
          else
            {
              FEven = ! FEven; // <=> à FEven = false
              FDartCurrent = FMap->alpha(FDartCurrent, Aj);
	      FPrevOp = OP_ALPHAJ;
            }
        }
      else
        {
          FEven = ! FEven; // <=> à FEven = false
          FDartCurrent = FMap->alpha(FDartCurrent, Ai);
	  FPrevOp  =OP_ALPHAI;
        }
    }
  else
    {
      if (FMap->alpha(FDartCurrent, Ai) == FDart1)
        {
          if (! FMap->isFree(FDartCurrent, Aj))
            {
              FEven = ! FEven;  // <=> à FEven = true
              FDartCurrent = FMap->alpha(FDartCurrent, Aj);
	      FPrevOp = OP_ALPHAJ;
            }
          else
	    FPrevOp = OP_END;
        }
      else
	FPrevOp = OP_END;
    }
}
//******************************************************************************
template <int Ai, int Aj>
inline
TOperationState CTemplateDynamicCoverage2a<Ai,Aj>::prevOperationType()
{
  return FPrevOp;
}
//******************************************************************************
template <int Ai, int Aj>
inline
CDart* CTemplateDynamicCoverage2a<Ai,Aj>::operator*()
{
  assert(cont());
  return FDartCurrent;
}
//******************************************************************************
template <int Ai, int Aj>
inline
CDart* CTemplateDynamicCoverage2a<Ai,Aj>::operator++(int)
{
  CDart* tmp = operator*();

  operator++();
  return tmp;
}
//******************************************************************************
template <int Ai, int Aj>
inline
int CTemplateDynamicCoverage2a<Ai,Aj>::type() const
{
  return COVERAGE2A;
}
//******************************************************************************
// CTemplateDynamicCoverage2b
//******************************************************************************
template <int Ai, int Aj>
inline
void CTemplateDynamicCoverage2b<Ai,Aj>::reinit()
{
  FDartCurrent = FDart1;
  FFirstDirection = true;
  FNextTryAi = true;
  FPrevOp = OP_NONE;
}
//******************************************************************************
template <int Ai, int Aj>
inline
CTemplateDynamicCoverage2b<Ai,Aj>::
CTemplateDynamicCoverage2b(CGMapBasic* AGMap, const CDart* ADart1) :
  CCoverage(AGMap),
  FDart1   ((CDart*) ADart1)
{
  reinit();
}
//******************************************************************************
template <int Ai, int Aj>
inline
CTemplateDynamicCoverage2b<Ai,Aj>::~CTemplateDynamicCoverage2b()
{
}
//******************************************************************************
template <int Ai, int Aj>
inline
bool CTemplateDynamicCoverage2b<Ai,Aj>::cont()
{
  return FPrevOp != OP_END;
}
//******************************************************************************
template <int Ai, int Aj>
inline
void CTemplateDynamicCoverage2b<Ai,Aj>::operator++()
{
  assert(cont());

  if (FFirstDirection)
    {
      if (FNextTryAi)
        {
          if (FMap->isFree(FDartCurrent, Ai))
            {
	      FFirstDirection = false;
	      if (FMap->isFree(FDart1, Aj))
		FPrevOp = OP_END;
	      else
                {
                  FDartCurrent = FMap->alpha(FDart1, Aj);
                  FPrevOp = OP_JUMP;
                }
            }
          else
            {
	      FDartCurrent = FMap->alpha(FDartCurrent, Ai);
              FNextTryAi = false;
	      FPrevOp = OP_ALPHAI;
            }
        }
      else
        {
          if (FMap->isFree(FDartCurrent, Aj))
            {
              FFirstDirection = false;
	      if (FMap->isFree(FDart1, Aj))
		FPrevOp = OP_END;
	      else
                {
                  FDartCurrent = FMap->alpha(FDart1, Aj);	
                  FNextTryAi = true;
                  FPrevOp = OP_JUMP;
                }
            }
          else
            {
              FDartCurrent = FMap->alpha(FDartCurrent, Aj);
              if (FDartCurrent == FDart1)
		FPrevOp = OP_END;
              else
                {
                  FNextTryAi = true;
                  FPrevOp = OP_ALPHAJ;
                }
            }
        }
    }
  else
    {
      if (FNextTryAi)
	{
	  if (FMap->isFree(FDartCurrent, Ai))
	    FPrevOp = OP_END;
          else
            {
              FDartCurrent = FMap->alpha(FDartCurrent, Ai);
              FNextTryAi = false;
	      FPrevOp = OP_ALPHAI;
            }
        }
      else
	{
          if (FMap->isFree(FDartCurrent, Aj))
	    FPrevOp = OP_END;
          else
	    {
	      FDartCurrent = FMap->alpha(FDartCurrent, Aj);
              FNextTryAi = true;
	      FPrevOp = OP_ALPHAJ;
            }
        }
    }
}
//******************************************************************************
template <int Ai, int Aj>
inline
TOperationState CTemplateDynamicCoverage2b<Ai,Aj>::prevOperationType()
{
  return FPrevOp;
}
//******************************************************************************
template <int Ai, int Aj>
inline
CDart* CTemplateDynamicCoverage2b<Ai,Aj>::operator*()
{
  assert(cont());

  return FDartCurrent;
}
//******************************************************************************
template <int Ai, int Aj>
inline
CDart* CTemplateDynamicCoverage2b<Ai,Aj>::operator++(int )
{
  CDart* tmp = operator*();
  operator++();
  return tmp;
}
//******************************************************************************
template <int Ai, int Aj>
inline
int CTemplateDynamicCoverage2b<Ai,Aj>::type() const
{
  return COVERAGE2B;
}
//******************************************************************************
// CTemplateDynamicCoverage3a
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
void CTemplateDynamicCoverage3a<Ai,Aj,Ak>::reinit()
{
  FFirstBorder = true;
  FPrevAk = false;
  FIt.reinit();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateDynamicCoverage3a<Ai,Aj,Ak>::
CTemplateDynamicCoverage3a(CGMapBasic* AGMap, const CDart* ADart1) :
  CCoverage(AGMap),
  FIt	   (AGMap, ADart1)
{
  FAkExist = ! FMap->isFree(ADart1, Ak);
  reinit();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateDynamicCoverage3a<Ai,Aj,Ak>::~CTemplateDynamicCoverage3a()
{
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
bool CTemplateDynamicCoverage3a<Ai,Aj,Ak>::cont()
{
  return FIt.cont();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
void CTemplateDynamicCoverage3a<Ai,Aj,Ak>::operator++()
{
  assert(cont());

  if (! FAkExist || FPrevAk)
    {
      ++ FIt;

      if (FIt.cont())
	{
	  if (FIt.prevOperationType() == OP_JUMP)
	    FFirstBorder = true;

	  FPrevAk = false;
	}
    }
  else
    {
      FPrevAk = true;
      FFirstBorder = ! FFirstBorder;
    }
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CDart* CTemplateDynamicCoverage3a<Ai,Aj,Ak>::operator*()
{
  assert(cont());

  if (FFirstBorder)
    return * FIt;
  else
    return FMap->alpha(* FIt, Ak);
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CDart* CTemplateDynamicCoverage3a<Ai,Aj,Ak>::operator++(int)
{
  CDart* tmp = operator*();
  operator++();
  return tmp;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
TOperationState CTemplateDynamicCoverage3a<Ai,Aj,Ak>::prevOperationType()
{
  if (FPrevAk)
    return OP_ALPHAK;
  else
    return FIt.prevOperationType();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
int CTemplateDynamicCoverage3a<Ai,Aj,Ak>::type() const
{
  return COVERAGE3A;
}
//******************************************************************************
// CTemplateBasicDynamicCoverage3b
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>::
CTemplateBasicDynamicCoverage3b(CGMapBasic* AGMap, const CDart* ADart1,
				int AMarkNumber) :
  CCoverage   (AGMap),
  FDartCurrent((CDart*) ADart1),
  FMarkNumber (AMarkNumber),
  FPrevOp     (OP_NONE)
{
  FMap->setMark(FDartCurrent, FMarkNumber);
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>::~CTemplateBasicDynamicCoverage3b()
{
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
bool CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>::cont()
{
  return FPrevOp != OP_END;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
void CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>::reinit()
{
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
void CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>::operator++()
{
#define GET_ALPHAI_AND_MARK(D,AI) \
( FMap->setMark(FMap->alpha(D,AI), FMarkNumber), FMap->alpha(D,AI) )

#define IS_UNMARKED(D,AI) \
( ! FMap->isMarked(FMap->alpha(D,AI), FMarkNumber) )

#define PUSH_AND_MARK(D,AI) \
( FToTreat.push(GET_ALPHAI_AND_MARK(D,AI)) )

  assert(cont());

  CDart* nd = NULL;

  if (IS_UNMARKED(FDartCurrent, Ai))
    {
      nd = GET_ALPHAI_AND_MARK(FDartCurrent, Ai);
      FPrevOp = OP_ALPHAI;

      if (IS_UNMARKED(FDartCurrent, Aj))
	PUSH_AND_MARK(FDartCurrent, Aj);

      if (IS_UNMARKED(FDartCurrent, Ak))
	PUSH_AND_MARK(FDartCurrent, Ak);
    }
  else if (IS_UNMARKED(FDartCurrent, Aj)) // not marked aj(d)
    {
      nd = GET_ALPHAI_AND_MARK(FDartCurrent, Aj);
      FPrevOp = OP_ALPHAJ;

      if (IS_UNMARKED(FDartCurrent, Ak))
	PUSH_AND_MARK(FDartCurrent, Ak);
    }
  else if (IS_UNMARKED(FDartCurrent, Ak)) // not marked ak(d)
    {
      nd = GET_ALPHAI_AND_MARK(FDartCurrent, Ak);
      FPrevOp = OP_ALPHAK;
    }
  else if (! FToTreat.empty())
    {
      nd = FToTreat.front();
      FToTreat.pop();
      FPrevOp = OP_JUMP;
    }
  else
    FPrevOp = OP_END;

  FDartCurrent = nd;

#undef PUSH_AND_MARK
#undef IS_SEWED_AND_UNMARKED
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
TOperationState CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>::prevOperationType()
{
  return FPrevOp;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CDart* CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>::operator*()
{
  assert(cont());
  return FDartCurrent;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CDart* CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>::operator ++ ( int )
{
  CDart* tmp = operator*();
  operator ++();
  return tmp;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
int CTemplateDynamicCoverage3b<Ai,Aj,Ak>::type() const
{
  return COVERAGE3B;
}
//******************************************************************************
// CTemplateBasicDynamicCoverage4
//******************************************************************************
inline
CTemplateBasicDynamicCoverage4::
CTemplateBasicDynamicCoverage4(CGMapBasic* AGMap, const CDart* ADart1,
			       int AMarkNumber) :
  CCoverage   (AGMap),
  FDartCurrent((CDart*) ADart1),
  FMarkNumber (AMarkNumber),
  FPrevOp     (OP_NONE)
{
  FMap->setMark(FDartCurrent, FMarkNumber);
}
//******************************************************************************
inline
CTemplateBasicDynamicCoverage4::~CTemplateBasicDynamicCoverage4()
{
}
//******************************************************************************
inline
bool CTemplateBasicDynamicCoverage4::cont()
{
  return FPrevOp != OP_END;
}
//******************************************************************************
inline
void CTemplateBasicDynamicCoverage4::reinit()
{
}
//******************************************************************************
inline
void CTemplateBasicDynamicCoverage4::operator ++()
{
  assert(cont());

  CDart*nd=NULL;

#define IS_SEWED_AND_UNMARKED(D,AI) \
! FMap->isFree(D,AI) && ! FMap->isMarked(FMap->alpha(D,AI), FMarkNumber)

  if (IS_SEWED_AND_UNMARKED(FDartCurrent,0))
    {
      nd = FMap->alpha0(FDartCurrent);
      FMap->setMark(nd, FMarkNumber);
      FPrevOp = OP_ALPHA0;

      if (IS_SEWED_AND_UNMARKED(FDartCurrent, 1))
	{
	  FToTreat.push(FMap->alpha1(FDartCurrent));
	  FMap->setMark(FMap->alpha1(FDartCurrent),FMarkNumber);
	}

      if (IS_SEWED_AND_UNMARKED(FDartCurrent, 2))
	{
	  FToTreat.push(FMap->alpha2(FDartCurrent));
	  FMap->setMark(FMap->alpha2(FDartCurrent), FMarkNumber);
	}

      if (IS_SEWED_AND_UNMARKED(FDartCurrent, 3))
	{
	  FToTreat.push(FMap->alpha3(FDartCurrent));
	  FMap->setMark(FMap->alpha3(FDartCurrent), FMarkNumber);
	}
    }
  else if (IS_SEWED_AND_UNMARKED(FDartCurrent, 1))
    {
      nd = FMap->alpha1(FDartCurrent);
      FMap->setMark(nd, FMarkNumber);
      FPrevOp = OP_ALPHA1;

      if (IS_SEWED_AND_UNMARKED(FDartCurrent, 2))
	{
	  FToTreat.push(FMap->alpha2(FDartCurrent));
	  FMap->setMark(FMap->alpha2(FDartCurrent), FMarkNumber);
	}

      if (IS_SEWED_AND_UNMARKED(FDartCurrent, 3))
	{
	  FToTreat.push(FMap->alpha3(FDartCurrent));
	  FMap->setMark(FMap->alpha3(FDartCurrent), FMarkNumber);
	}
    }
  else if (IS_SEWED_AND_UNMARKED(FDartCurrent, 2))
    {
      nd = FMap->alpha2(FDartCurrent);
      FMap->setMark(nd, FMarkNumber);
      FPrevOp = OP_ALPHA2;

      if (IS_SEWED_AND_UNMARKED(FDartCurrent, 3))
	{
	  FToTreat.push(FMap->alpha3(FDartCurrent));
	  FMap->setMark(FMap->alpha3(FDartCurrent), FMarkNumber);
	}
    }
  else if (IS_SEWED_AND_UNMARKED(FDartCurrent, 3))
    {
      nd = FMap->alpha3(FDartCurrent);
      FMap->setMark(nd, FMarkNumber);
      FPrevOp = OP_ALPHA3;
    }
  else if (! FToTreat.empty())
    {
      nd = FToTreat.front();
      FToTreat.pop();
      FPrevOp = OP_UNKNOWN;
    }
  else
    FPrevOp = OP_END;

#undef IS_SEWED_AND_UNMARKED

  FDartCurrent = nd;
}
//******************************************************************************
inline
TOperationState CTemplateBasicDynamicCoverage4::prevOperationType()
{
  return FPrevOp;
}
//******************************************************************************
inline
CDart* CTemplateBasicDynamicCoverage4::operator*()
{
  assert(cont());

  return FDartCurrent;
}
//******************************************************************************
inline
CDart* CTemplateBasicDynamicCoverage4::operator++(int)
{
  CDart* tmp = operator*();

  operator++();
  return tmp;
}
//******************************************************************************
inline
int CTemplateBasicDynamicCoverage4::type() const
{
  return COVERAGE4;
}
//******************************************************************************
// CTemplateDynamicCoverage3b
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
void CTemplateDynamicCoverage3b<Ai,Aj,Ak>::reinit()
{
  if (FIt != NULL)
    {
      delete FIt;

      FMap->negateMaskMark(FMarkNumber);

      FIt = new CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>
	(FMap, FDart1, FMarkNumber);

      while (FIt->cont())
	++ * FIt;

      FMap->negateMaskMark(FMarkNumber);
      delete FIt;
    }

  FIt = new CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>
    (FMap, FDart1, FMarkNumber);
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateDynamicCoverage3b<Ai,Aj,Ak>::
CTemplateDynamicCoverage3b(CGMapBasic* AGMap, const CDart* ADart1) :
  CCoverage(AGMap),
  FDart1   ((CDart*) ADart1),
  FIt	   (NULL)
{
  FMarkNumber = FMap->getNewMark();
  reinit();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateDynamicCoverage3b<Ai,Aj,Ak>::~CTemplateDynamicCoverage3b()
{
  delete FIt;

  FMap->negateMaskMark(FMarkNumber);

  FIt =
    new CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>(FMap, FDart1, FMarkNumber);

  while (FIt->cont())
    ++ * FIt;

  FMap->negateMaskMark(FMarkNumber);

  delete FIt;

  FMap->freeMark(FMarkNumber);
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
bool CTemplateDynamicCoverage3b<Ai,Aj,Ak>::cont()
{
  return FIt->cont();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
void CTemplateDynamicCoverage3b<Ai,Aj,Ak>::operator ++()
{
  FIt->operator++();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
TOperationState CTemplateDynamicCoverage3b<Ai,Aj,Ak>::prevOperationType()
{
  return FIt->prevOperationType();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CDart* CTemplateDynamicCoverage3b<Ai,Aj,Ak>::operator*()
{
  return FIt->operator*();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CDart* CTemplateDynamicCoverage3b<Ai,Aj,Ak>::operator++(int)
{
  CDart* tmp = operator*();
  operator++();
  return tmp;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
int CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>::type() const
{
  return COVERAGE3B;
}
//******************************************************************************
// CTemplateDynamicCoverage4
//******************************************************************************
inline
void CTemplateDynamicCoverage4::reinit()
{
  if (FIt != NULL)
    {
      delete FIt;

      FMap->negateMaskMark(FMarkNumber);

      FIt = new CTemplateBasicDynamicCoverage4(FMap, FDart1, FMarkNumber);

      while (FIt->cont())
	++ * FIt;

      FMap->negateMaskMark(FMarkNumber);

      delete FIt;
    }

  FIt = new CTemplateBasicDynamicCoverage4(FMap, FDart1, FMarkNumber);
}
//******************************************************************************
inline
CTemplateDynamicCoverage4::CTemplateDynamicCoverage4(CGMapBasic* AGMap,
						     const CDart* ADart1) :
  CCoverage(AGMap),
  FDart1   ((CDart*) ADart1),
  FIt	   (NULL)
{
  FMarkNumber = FMap->getNewMark();
  reinit();
}
//******************************************************************************
inline
CTemplateDynamicCoverage4::~CTemplateDynamicCoverage4()
{
  delete FIt;

  FMap->negateMaskMark(FMarkNumber);

  FIt = new CTemplateBasicDynamicCoverage4(FMap, FDart1, FMarkNumber);

  while (FIt->cont())
    ++ * FIt;

  FMap->negateMaskMark(FMarkNumber);

  delete FIt;

  FMap->freeMark(FMarkNumber);
}
//******************************************************************************
inline
bool CTemplateDynamicCoverage4::cont()
{
  return FIt->cont();
}
//******************************************************************************
inline
void CTemplateDynamicCoverage4::operator++()
{
  FIt->operator++();
}
//******************************************************************************
inline
TOperationState CTemplateDynamicCoverage4::prevOperationType()
{
  return FIt->prevOperationType();
}
//******************************************************************************
inline
CDart* CTemplateDynamicCoverage4::operator*()
{
  return FIt->operator*();
}
//******************************************************************************
inline
CDart* CTemplateDynamicCoverage4::operator++(int)
{
  CDart* tmp = operator*();
  operator++();
  return tmp;
}
//******************************************************************************
inline
int CTemplateDynamicCoverage4::type() const
{
  return COVERAGE4;
}
//******************************************************************************
// CTemplateMultiDynamicCoverage3b
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
void CTemplateMultiDynamicCoverage3b<Ai,Aj,Ak>::reinit()
{
  if (FIt1 != NULL)
    {
      delete FIt1;
      FIt1 = NULL;
    }

  FIt2 = FAllCoverDarts.begin();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateMultiDynamicCoverage3b<Ai,Aj,Ak>::
CTemplateMultiDynamicCoverage3b(CGMapBasic* AGMap, const CDart* ADart1) :
  CCoverage(AGMap)
{
  FMarkNumber = AGMap->getNewMark();
  FIt1 = new CTemplateBasicDynamicCoverage3b<Ai,Aj,Ak>(AGMap, ADart1,
						       FMarkNumber);
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CTemplateMultiDynamicCoverage3b<Ai,Aj,Ak>::~CTemplateMultiDynamicCoverage3b()
{
  if (FIt1 != NULL)
    delete FIt1;

  while (! FAllCoverDarts.empty())
    {
      FMap->unsetMark(FAllCoverDarts.front(), FMarkNumber);
      FAllCoverDarts.pop_front();
    }

  FMap->freeMark(FMarkNumber);
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
bool CTemplateMultiDynamicCoverage3b<Ai,Aj,Ak>::cont()
{
  if (FIt1 != NULL)
    return FIt1->cont();
  else
    return FIt2 != FAllCoverDarts.end();
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
void CTemplateMultiDynamicCoverage3b<Ai,Aj,Ak>::operator++()
{
  if (FIt1 != NULL)
    FIt1->operator++();
  else
    ++FIt2;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
TOperationState CTemplateMultiDynamicCoverage3b<Ai,Aj,Ak>::prevOperationType()
{
  if (FIt1 != NULL)
    return FIt1->prevOperationType();
  else
    return OP_UNKNOWN;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CDart* CTemplateMultiDynamicCoverage3b<Ai,Aj,Ak>::operator*()
{
  assert(cont());

  if (FIt1 != NULL)
    return FIt1->operator*();
  else
    return * FIt2;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
CDart* CTemplateMultiDynamicCoverage3b<Ai,Aj,Ak>::operator++(int)
{
  CDart* tmp = operator*();
  operator ++();
  return tmp;
}
//******************************************************************************
template <int Ai, int Aj, int Ak>
inline
int CTemplateMultiDynamicCoverage3b<Ai,Aj,Ak>::type() const
{
  return COVERAGE3B;
}
//******************************************************************************
// CTemplateMultiDynamicCoverage4
//******************************************************************************
inline
void CTemplateMultiDynamicCoverage4::reinit()
{
  if (FIt1 != NULL)
    {
      delete FIt1;
      FIt1 = NULL;
    }

  FIt2 = FAllCoverDarts.begin();
}
//******************************************************************************
inline
CTemplateMultiDynamicCoverage4::
CTemplateMultiDynamicCoverage4(CGMapBasic* AGMap, const CDart* ADart1) :
  CCoverage(AGMap)
{
  FMarkNumber = AGMap->getNewMark();
  FIt1 = new CTemplateBasicDynamicCoverage4(AGMap, ADart1, FMarkNumber);
}
//******************************************************************************
inline
CTemplateMultiDynamicCoverage4::~CTemplateMultiDynamicCoverage4()
{
  if (FIt1 != NULL)
    delete FIt1;

  while (! FAllCoverDarts.empty())
    {
      FMap->unsetMark(FAllCoverDarts.front(), FMarkNumber);
      FAllCoverDarts.pop_front();
    }

  FMap->freeMark(FMarkNumber);
}
//******************************************************************************
inline
bool CTemplateMultiDynamicCoverage4::cont()
{
  if (FIt1 != NULL)
    return FIt1->cont();
  else
    return FIt2 != FAllCoverDarts.end();
}
//******************************************************************************
inline
void CTemplateMultiDynamicCoverage4::operator++()
{
  assert(cont());

  if (FIt1 != NULL)
    FIt1->operator++();
  else
    ++FIt2;
}
//******************************************************************************
inline
TOperationState CTemplateMultiDynamicCoverage4::prevOperationType()
{
  if (FIt1 != NULL)
    return FIt1->prevOperationType();
  else
    return OP_UNKNOWN;
}
//******************************************************************************
inline
CDart* CTemplateMultiDynamicCoverage4::operator*()
{
  if (FIt1 != NULL)
    return FIt1->operator*();
  else
    return * FIt2;
}
//******************************************************************************
inline
CDart* CTemplateMultiDynamicCoverage4::operator++(int)
{
  CDart* tmp = operator*();
  operator++();
  return tmp;
}
//******************************************************************************
inline
int CTemplateMultiDynamicCoverage4::type() const
{
  return COVERAGE4;
}
//******************************************************************************
// CDynamicCoverageAll
//******************************************************************************
inline
void CDynamicCoverageAll::reinit()
{
  FCurrent = FMap->getFirstDart();
}
//******************************************************************************
inline
CDynamicCoverageAll::CDynamicCoverageAll(CGMapBasic* AGMap)
  : CCoverage(AGMap)
{
  reinit();
}
//******************************************************************************
inline
CDynamicCoverageAll::~CDynamicCoverageAll()
{
}
//******************************************************************************
inline
bool CDynamicCoverageAll::cont()
{
  return FCurrent != NULL;
}
//******************************************************************************
inline
void CDynamicCoverageAll::operator++()
{
  assert(cont());
  FCurrent = FCurrent->getNext();
}
//******************************************************************************
inline
CDart* CDynamicCoverageAll::operator*()
{
  assert(cont());

  return FCurrent;
}
//******************************************************************************
inline
CDart* CDynamicCoverageAll::operator++(int)
{
  CDart* tmp = operator*();
  operator++();
  return tmp;
}
//******************************************************************************
inline
TOperationState CDynamicCoverageAll::prevOperationType()
{
  return OP_UNKNOWN;
}
//******************************************************************************
inline
int CDynamicCoverageAll::type() const
{
  return COVERAGE_ALL;
}
//******************************************************************************
// CBasicBorderCoverage
//******************************************************************************
inline
CBasicBorderCoverage::CBasicBorderCoverage(CGMapBasic* AGMap,
					   const CDart* ADart1,
					   int AMarkNumber, int ADimension) :
  CCoverage   (AGMap),
  FCurrentDart((CDart*) ADart1),
  FMarkNumber (AMarkNumber),
  FBorderDim  (ADimension),
  FPrevOp     (OP_NONE)
{
  assert(FMap->isFree(ADart1, ADimension));
  AGMap->setMark((CDart*) ADart1, FMarkNumber);
}
//******************************************************************************
inline
CBasicBorderCoverage::~CBasicBorderCoverage()
{
}
//******************************************************************************
inline
bool CBasicBorderCoverage::cont()
{
  return FPrevOp != OP_END;
}
//******************************************************************************
inline
void CBasicBorderCoverage::reinit()
{
}
//******************************************************************************
inline
void CBasicBorderCoverage::operator++()
{
  assert(cont());

  CDart* n = FCurrentDart;
  FPrevOp = OP_JUMP;

  if (FBorderDim != 0)
    {
      do
	n = FMap->alpha(FMap->alpha(n,FBorderDim-1),FBorderDim);
      while (! FMap->isFree(n, FBorderDim) &&
	     ! FMap->isFree(n, FBorderDim-1) && n != FCurrentDart);

      if (! FMap->isMarked(n, FMarkNumber) && FMap->isFree(n, FBorderDim))
	{
	  FToTreat.push(n);
	  FMap->setMark(n, FMarkNumber);
	  FPrevOp = OP_TURN_AROUND;
	}
    }

  for (int i=3; i>=0; --i)
    if ((i<FBorderDim-1 || i>FBorderDim+1) &&
	! FMap->isMarked(FMap->alpha(FCurrentDart, i), FMarkNumber))
      {
	FPrevOp = OP_ALPHA0 + i;
	FToTreat.push(FMap->alpha(FCurrentDart, i));
	FMap->setMark(FMap->alpha(FCurrentDart, i), FMarkNumber);
      }

  if (FToTreat.empty())
    FPrevOp = OP_END;
  else
    {
      FCurrentDart = FToTreat.top();
      FToTreat.pop();
    }
}
//******************************************************************************
inline
CDart* CBasicBorderCoverage::operator*()
{
  assert(cont());

  return FCurrentDart;
}
//******************************************************************************
inline
CDart* CBasicBorderCoverage::operator++(int)
{
  CDart* tmp = operator*();
  operator++();
  return tmp;
}
//******************************************************************************
inline
TOperationState CBasicBorderCoverage::prevOperationType()
{
  return FPrevOp;
}
//******************************************************************************
inline
int CBasicBorderCoverage::type() const
{
  return COVERAGE_BORDER;
}
//******************************************************************************
// CDynamicBorderCoverage
//******************************************************************************
inline
void CDynamicBorderCoverage::reinit()
{
  if (FIt != NULL)
    {
      delete FIt;

      FMap->negateMaskMark(FMarkNumber);

      FIt = new CBasicBorderCoverage(FMap, FDart1, FMarkNumber, FBorderDim);

      while (FIt->cont())
	++ * FIt;

      FMap->negateMaskMark(FMarkNumber);

      delete FIt;
    }

  FIt = new CBasicBorderCoverage(FMap, FDart1, FMarkNumber, FBorderDim);
}
//******************************************************************************
inline
CDynamicBorderCoverage::CDynamicBorderCoverage(CGMapBasic* AGMap,
					       const CDart* ADart1, int ADim) :
  CCoverage (AGMap),
  FDart1    ((CDart*) ADart1),
  FBorderDim(ADim),
  FIt	    (NULL)
{
  FMarkNumber = FMap->getNewMark();
  reinit();
}
//******************************************************************************
inline
CDynamicBorderCoverage::~CDynamicBorderCoverage()
{
  while (FIt->cont())
    ++ * FIt;

  delete FIt;

  FMap->negateMaskMark(FMarkNumber);

  FIt = new CBasicBorderCoverage(FMap, FDart1, FMarkNumber, FBorderDim);

  while (FIt->cont())
    ++ * FIt;

  delete FIt;
  FMap->negateMaskMark(FMarkNumber);

  FMap->freeMark(FMarkNumber);
}
//******************************************************************************
inline
bool CDynamicBorderCoverage::cont()
{
  return FIt->cont();
}
//******************************************************************************
inline
void CDynamicBorderCoverage::operator ++()
{
  FIt->operator++();
}
//******************************************************************************
inline
CDart* CDynamicBorderCoverage::operator*()
{
  return FIt->operator*();
}
//******************************************************************************
inline
CDart* CDynamicBorderCoverage::operator ++ ( int )
{
  CDart* tmp = operator*();
  operator++();
  return tmp;
}
//******************************************************************************
inline
TOperationState CDynamicBorderCoverage::prevOperationType()
{
  return FIt->prevOperationType();
}
//******************************************************************************
inline
int CDynamicBorderCoverage::type() const
{
  return COVERAGE_BORDER;
}
//******************************************************************************
} // namespace GMap3d
//******************************************************************************
#endif // DYNAMIC_COVERAGE_HH
//******************************************************************************
