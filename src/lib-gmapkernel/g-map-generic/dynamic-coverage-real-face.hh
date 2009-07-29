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
#ifndef DYNAMIC_COVERAGE_REAL_FACE_HH
#define DYNAMIC_COVERAGE_REAL_FACE_HH
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
   * Itérateur correspondant à une orbite face (<0,1,3>) en sautant les arêtes
   * fictives (i.e. les arêtes marquées avec AMark).
   *
   * @author MOKA TEAM
   */
  class CDynamicCoverageRealFace : public CCoverage
  {
  private:
    // @name Champs privés
    // @{

    /// Brin de départ
    CDart* FDart1;
    /// Brin courant
    CDart* FDartCurrent;

    /// Marque pour les arêtes fictives.
    int FFictiveMark;
    
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
     * le parcours (A0 ou A1).
     */
    bool FNextTryA0;

    /// Indique si tous les brins de <A0,A1> sont liés par A3
    bool FA3Exist;

    /// Indique si le brin courant est sur le même bord que D1 par A3
    bool FFirstBorder;

    /// Type de la dernière opération réalisée
    TOperationState FPrevOp;

    // @}

  public:
    // @name Constructeur et destructeurs
    // @{

    /// Constructeur
    CDynamicCoverageRealFace(CGMapBasic* AGMap, const CDart* ADart, int AMark);
    /// Destructeur
    virtual ~CDynamicCoverageRealFace();

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
// CDynamicCoverageRealFace
//******************************************************************************
inline
void CDynamicCoverageRealFace::reinit()
{
  FDartCurrent	  = FDart1;
  FNextTryA0	  = true;
  FFirstBorder	  = true;
  FFirstDirection = true;
  FPrevOp	  = OP_NONE;
}
//******************************************************************************
inline
CDynamicCoverageRealFace::
CDynamicCoverageRealFace(CGMapBasic* AGMap, const CDart* ADart1, int AMark) :
  CCoverage   (AGMap),
  FDart1      ((CDart*) ADart1),
  FFictiveMark(AMark)
{
  FA3Exist = !FMap->isFree3(ADart1);
  reinit();
}
//******************************************************************************
inline
CDynamicCoverageRealFace::~CDynamicCoverageRealFace()
{
}
//******************************************************************************
inline
bool CDynamicCoverageRealFace::cont()
{
  return FPrevOp != OP_END;
}
//******************************************************************************
inline
void CDynamicCoverageRealFace::operator++()
{
  assert(cont());

  if ( FA3Exist && FPrevOp!=OP_ALPHA3 )
    {
      FDartCurrent =  FMap->alpha3(FDartCurrent);
      FFirstBorder = !FFirstBorder;
      FPrevOp      = OP_ALPHA3;
    }
  else
    {
      int nextAlpha = 0;
     // 1) On passe au brin suivant de la face
      if ( !FNextTryA0 ) nextAlpha=1;

      if ( FMap->isFree(FDartCurrent, nextAlpha) )
	{
	  if ( !FFirstDirection || FMap->isFree1(FDart1) )
	    FPrevOp = OP_END; // Fin du parcours.
	  else
	    {
	      FDartCurrent    = FMap->alpha1(FDart1); // On fait le jump.
	      FFirstDirection = false;
	      FNextTryA0      = true;
	      FPrevOp         = OP_JUMP;
	    }
	}
      else
	{	  
	  FDartCurrent = FMap->alpha(FDartCurrent, nextAlpha);
	  FNextTryA0 = !FNextTryA0;
	  FPrevOp    = (nextAlpha==0?OP_ALPHA0:OP_ALPHA1);
	}

      if ( FPrevOp!=OP_END )
	{
	  // 2) Si ce brin appartient à une arête fictive, on va tourner autour du
	  //    sommet jusqu'a tomber sur une arête réelle.
	  while ( FMap->isMarked(FDartCurrent, FFictiveMark) && // C'est une arête fictive
		  !FMap->isFree2(FDartCurrent) &&               // si c'est un brin du bord
		  !FMap->isFree1(FMap->alpha2(FDartCurrent)) )
	    {
	      FDartCurrent = FMap->alpha1(FMap->alpha2(FDartCurrent));
	    }
	  
	  // 3) Si on est toujours sur une arête fictive, il faut re-essayer de jumper
	  if ( FMap->isMarked(FDartCurrent, FFictiveMark) )
	    { 
	      if ( !FFirstDirection || FMap->isFree1(FDart1) )
		FPrevOp = OP_END; // Fin du parcours.
	      else
		{
		  FDartCurrent    = FMap->alpha1(FDart1); // On fait le jump.
		  FFirstDirection = false;
		  FNextTryA0      = true;
		  FPrevOp         = OP_JUMP;
		}
	    }
	}
    }

  if ( FDartCurrent==FDart1 )
    FPrevOp = OP_END;  
}
//******************************************************************************
inline
TOperationState CDynamicCoverageRealFace::prevOperationType()
{
  return FPrevOp;
}
//******************************************************************************
inline
CDart* CDynamicCoverageRealFace::operator*()
{
  assert(cont());
  return FDartCurrent;
}
//******************************************************************************
inline
CDart* CDynamicCoverageRealFace::operator++(int)
{
  CDart* tmp = operator*();

  operator++();
  return tmp;
}
//******************************************************************************
inline
int CDynamicCoverageRealFace::type() const
{
  return COVERAGE_REAL_FACE;
}
//******************************************************************************
} // namespace GMap3d
//******************************************************************************
#endif // DYNAMIC_COVERAGE_REAL_FACE_HH
//******************************************************************************
