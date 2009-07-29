/*
 * lib-corefinement : Opérations de corafinement.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-corefinement
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

// #define DEBUG_MESSAGES
#include "message-macros.hh"

#include "boolean-operations.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************

ostream & operator << (ostream & AStream, TBooleanOperation ABoolOp)
{
  switch (ABoolOp) {
  case BO_Union:
    AStream << "Union";
    break;
  case BO_Intersection:
    AStream << "Intersection";
    break;
  case BO_Difference1:
    AStream << "Difference1";
    break;
  case BO_Difference2:
    AStream << "Difference2";
    break;
  default:
    AStream << "Unknown";
    break;
  }

  return (AStream);
}

//******************************************************************************

CBooleanOperations::CBooleanOperations(CGMapVertex * AMap,
				       CDart * AObject1,
				       CDart * AObject2,
				       bool ACalculateOrientation,
				       int AVertexDI)
  : FMap(AMap), FObject1(AObject1), FObject2(AObject2),
    FCalculateOrientation(ACalculateOrientation), FVertexDI(AVertexDI)
{
  assert(AMap != NULL);

  FObject1Mark = FMap->getNewMark();
  FObject2Mark = FMap->getNewMark();

  if (FObject1 != NULL)
    FMap->markOrbit(FObject1, ORBIT_CC, FObject1Mark);
  
  if (FObject2 != NULL)
    FMap->markOrbit(FObject2, ORBIT_CC, FObject2Mark);
}

//******************************************************************************

CBooleanOperations::~CBooleanOperations()
{
//   if (FObject1 != NULL)
//     FMap->unmarkOrbit(FObject1, ORBIT_CC, FObject1Mark);

//   if (FObject2 != NULL)
//     FMap->unmarkOrbit(FObject2, ORBIT_CC, FObject2Mark);

  FMap->unmarkAll(FObject1Mark);
  FMap->unmarkAll(FObject2Mark);
  FMap->freeMark(FObject1Mark);
  FMap->freeMark(FObject2Mark);
}

//******************************************************************************

CDart * CBooleanOperations::getObject1() const
{
  return FObject1;
}

//******************************************************************************

CDart * CBooleanOperations::getObject2() const
{
  return FObject2;
}

//******************************************************************************

void CBooleanOperations::setObject1(CDart * AObject)
{
  FObject1 = AObject;
}

//******************************************************************************

void CBooleanOperations::setObject2(CDart * AObject)
{
  FObject2 = AObject;
}

//******************************************************************************

bool CBooleanOperations::isComputationPossible() const
{
  return (FObject1 != NULL && FObject2 != NULL &&
	  !FMap->isSameOrbit(FObject1, FObject2, ORBIT_CC));
}

//******************************************************************************

bool CBooleanOperations::computeResults(bitset<NB_MARKS> ACopyMarks)
{
  assert(isComputationPossible());

  ACopyMarks[getObject1Mark()] = true;
  ACopyMarks[getObject2Mark()] = true;
  bool result = corefineObjects(ACopyMarks);
  extendMarks();

  return result;
}

//******************************************************************************

void CBooleanOperations::markResult(TBooleanOperation AOperation,
				    int AMark)
{
  DEBUG_FUNCTION;

  CDynamicCoverageCC cov(FMap, FObject1);

  switch (AOperation) {
  case BO_Union:
    for (; cov.cont(); cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) &&
	  !FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AMark);
    }
    break;

  case BO_Intersection:
    for (; cov.cont(); cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) &&
	  FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AMark);
    }
    break;

  case BO_Difference1:
    for (; cov.cont(); cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) &&
	  !FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AMark);
    }
    break;

  case BO_Difference2:
    for (; cov.cont(); cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) &&
	  FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AMark);
    }
    break;
  }
}

//******************************************************************************

void CBooleanOperations::markAllButResult(TBooleanOperation AOperation,
					  int AMark)
{
  DEBUG_FUNCTION;

  CDynamicCoverageCC cov(FMap, FObject1);

  switch (AOperation) {
  case BO_Union:
    for (; cov.cont(); cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) ||
	  FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AMark);
    }
    break;

  case BO_Intersection:
    for (; cov.cont(); cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) ||
	  !FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AMark);
    }
    break;

  case BO_Difference1:
    for (; cov.cont(); cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) ||
	  FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AMark);
    }
    break;

  case BO_Difference2:
    for (; cov.cont(); cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) ||
	  !FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AMark);
    }
    break;
  }
}

//******************************************************************************

void CBooleanOperations::markResults(TBooleanOperation AOperation,
				     int AKeepMark, int ADiscardMark)
{
  DEBUG_FUNCTION;

  CDynamicCoverageCC cov(FMap, FObject1);

  switch (AOperation) {
  case BO_Union:
    for (; cov.cont(); cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) &&
	  !FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AKeepMark);
      else
	FMap->setMark(*cov, ADiscardMark);
    }
    break;

  case BO_Intersection:
    for (; cov.cont(); cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) &&
	  FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AKeepMark);
      else
	FMap->setMark(*cov, ADiscardMark);
    }
    break;

  case BO_Difference1:
    for (; cov.cont(); cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) &&
	  !FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AKeepMark);
      else
	FMap->setMark(*cov, ADiscardMark);
    }
    break;

  case BO_Difference2:
    for (; cov.cont(); cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) &&
	  FMap->isMarked(*cov, FObject2Mark))
	FMap->setMark(*cov, AKeepMark);
      else
	FMap->setMark(*cov, ADiscardMark);
    }
    break;
  }
}

//******************************************************************************

CDart * CBooleanOperations::getDartFromResult(TBooleanOperation AOperation)
{
  DEBUG_FUNCTION;

  CDart *d = NULL;
  CDynamicCoverageCC cov(FMap, FObject1);

  switch (AOperation) {
  case BO_Union:
    for (; cov.cont() && !d; cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) &&
	  !FMap->isMarked(*cov, FObject2Mark))
	d = *cov;
    }
    break;

  case BO_Intersection:
    for (; cov.cont() && !d; cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) &&
	  FMap->isMarked(*cov, FObject2Mark))
	d = *cov;
    }
    break;

  case BO_Difference1:
    for (; cov.cont() && !d; cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) &&
	  !FMap->isMarked(*cov, FObject2Mark))
	d = *cov;
    }
    break;

  case BO_Difference2:
    for (; cov.cont() && !d; cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) &&
	  FMap->isMarked(*cov, FObject2Mark))
	d = *cov;
    }
    break;
  }

  return d;
}

//******************************************************************************

CDart * CBooleanOperations::getDartOutOfResult(TBooleanOperation AOperation)
{
  DEBUG_FUNCTION;

  CDart *d = NULL;
  CDynamicCoverageCC cov(FMap, FObject1);

  switch (AOperation) {
  case BO_Union:
    for (; cov.cont() && !d; cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) ||
	  FMap->isMarked(*cov, FObject2Mark))
	d = *cov;
    }
    break;

  case BO_Intersection:
    for (; cov.cont() && !d; cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) ||
	  !FMap->isMarked(*cov, FObject2Mark))
	d = *cov;
    }
    break;

  case BO_Difference1:
    for (; cov.cont() && !d; cov++) {
      if (!FMap->isMarked(*cov, FObject1Mark) ||
	  FMap->isMarked(*cov, FObject2Mark))
	d = *cov;
    }
    break;

  case BO_Difference2:
    for (; cov.cont() && !d; cov++) {
      if (FMap->isMarked(*cov, FObject1Mark) ||
	  !FMap->isMarked(*cov, FObject2Mark))
	d = *cov;
    }
    break;
  }

  return d;
}

//******************************************************************************

void CBooleanOperations::keepResult(TBooleanOperation AOperation,
				    list<CDart*> * ACompoundList)
{
  DEBUG_FUNCTION;

  int keep_mark = FMap->getNewMark();
  int discard_mark = FMap->getNewMark();

//   FObject1 = FObject2 = getDartFromResult(AOperation);

  markResults(AOperation, keep_mark, discard_mark);
  if (FMap->isMarked(FObject1, discard_mark)) FObject1 = NULL;
  if (FMap->isMarked(FObject2, discard_mark)) FObject2 = NULL;
  FMap->deleteMarkedDarts(discard_mark);

  if (ACompoundList)
    getMarkedCompounds(keep_mark, ACompoundList);
  else
    FMap->unmarkAll(keep_mark);
  
//   assert(FMap->isWholeMapUnmarked(keep_mark));
//   assert(FMap->isWholeMapUnmarked(discard_mark));
  FMap->freeMark(keep_mark);
  FMap->freeMark(discard_mark);
}

//******************************************************************************

void CBooleanOperations::keepAllButResult(TBooleanOperation AOperation,
					  list<CDart*> * ACompoundList)
{
  DEBUG_FUNCTION;

  int keep_mark = FMap->getNewMark();
  int discard_mark = FMap->getNewMark();

//   FObject1 = FObject2 = getDartFromResult(AOperation);

  markResults(AOperation, discard_mark, keep_mark);
  if (FMap->isMarked(FObject1, discard_mark)) FObject1 = NULL;
  if (FMap->isMarked(FObject2, discard_mark)) FObject2 = NULL;
  FMap->deleteMarkedDarts(discard_mark);

  if (ACompoundList)
    getMarkedCompounds(keep_mark, ACompoundList);
  else
    FMap->unmarkAll(keep_mark);
  
//   assert(FMap->isWholeMapUnmarked(keep_mark));
//   assert(FMap->isWholeMapUnmarked(discard_mark));
  FMap->freeMark(keep_mark);
  FMap->freeMark(discard_mark);
}

//******************************************************************************

CGMapVertex * CBooleanOperations::getMap() const
{
  return FMap;
}

//******************************************************************************

int CBooleanOperations::getObject1Mark() const
{
  return FObject1Mark;
}

//******************************************************************************

int CBooleanOperations::getObject2Mark() const
{
  return FObject2Mark;
}

//******************************************************************************

bool CBooleanOperations::calculateOrientation() const
{
  return FCalculateOrientation;
}

//******************************************************************************

int CBooleanOperations::getVertexDI() const
{
  return FVertexDI;
}

//******************************************************************************

void CBooleanOperations::getMarkedCompounds(int AMark, list<CDart*> * AList)
{
  for (CDynamicCoverageAll cov(FMap); cov.cont(); ++cov)
    if (FMap->isMarked(*cov, AMark)) {
      FMap->unmarkOrbit(*cov, ORBIT_CC, AMark);
      AList->push_back(*cov);
    }
}

//******************************************************************************
