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
#include "g-map-vertex.hh"
using namespace GMap3d;
//******************************************************************************
void CGMapVertex::allocCoordToDirectInfo(int AMarkNumber,
					 int ADirectInfoIndex,
					 TOrbit AOrbitUsed)
{
  if (AOrbitUsed==ORBIT_NONE)
    {
      for (CDynamicCoverageAll it(this); it.cont(); ++it)
	if (isMarked(*it, AMarkNumber))
	  setDirectInfo(*it, ADirectInfoIndex, new TCoordinate);
    }
  else
    {
      assert(isOrbitUsed(AOrbitUsed));
      int treated = getNewMark();

      for (CDynamicCoverageAll it(this); it.cont(); ++it)
	if (!isMarked(*it, treated))
	  {
	    if (isMarked(*it, AMarkNumber))
	      {
		CDart * owner = getEmbeddingOwner(*it, AOrbitUsed);
		assert(owner!=NULL);

		setDirectInfo(owner, ADirectInfoIndex, new TCoordinate);

		markOrbit(owner, AOrbitUsed, treated);
	      }
	    else
	      setMark(*it, treated);
	  }

      negateMaskMark(treated);
      freeMark(treated);
    }
}
//******************************************************************************
void CGMapVertex::allocCoordToDirectInfo(CDart * ADart, TOrbit AOrbit,
					 int ADirectInfoIndex,
					 TOrbit AOrbitUsed)
{
  assert(ADart!=NULL);

  int mark = getNewMark();

  markOrbit(ADart, AOrbit, mark);
  allocCoordToDirectInfo(mark, ADirectInfoIndex, AOrbitUsed);
  unmarkOrbit(ADart, AOrbit, mark);

  freeMark(mark);
}
//******************************************************************************
void CGMapVertex::duplicateVertexToDirectInfo(int AMarkNumber,
					      int ADirectInfoNumber)
{
  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);

	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		(*cov)->setDirectInfo(ADirectInfoNumber,
				      new CVertex(* getVertex(*cov)));
	
	      else
		(*cov)->setDirectInfo(ADirectInfoNumber, NULL);
	    }
	else
	  {
	    setMark(*it, treated);
	    (*it)->setDirectInfo(ADirectInfoNumber, NULL);
	  }
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::deleteDuplicatedVertex(int ADirectInfoNumber)
{
  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    delete getDirectInfoAsVertex(*it, ADirectInfoNumber);
}
//******************************************************************************
void CGMapVertex::updateDirectInfoWithVertex(int ADirectInfoNumber)
{
  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if ((*it)->getDirectInfo(ADirectInfoNumber) != NULL)
      * getDirectInfoAsVertex(*it, ADirectInfoNumber) =	* getVertex(*it);
}
//******************************************************************************
void CGMapVertex::pointDirectInfoToAttributeVertex(int ADirectInfoIndex)
{
  int treated= getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	CAttributeVertex* vertex = getVertex(*it);

	if (vertex!=NULL)
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov,treated);
	      (*cov)->setDirectInfo(ADirectInfoIndex, vertex);
	    }
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::pointDirectInfoToAttributeVertex(int ADirectInfoIndex,
					      CDart* ADart)
{
  assert(ADart!=NULL);

  CAttributeVertex* vertex = findVertex(ADart);

  assert(vertex!=NULL);

  for (CDynamicCoverageVertex cov(this, ADart); cov.cont(); ++cov)
    (*cov)->setDirectInfo(ADirectInfoIndex, vertex);
}
//******************************************************************************
void CGMapVertex::pointDirectInfoToAttributeVertex(int ADirectInfoIndex,
						   CDart* ADart, TOrbit AOrbit)
{
  int treated= getNewMark();
  TOrbit orbit= AND_ORBIT(AOrbit,ORBIT_VERTEX);
  
  CCoverage* it=getDynamicCoverage(ADart,AOrbit);
  
  for (; it->cont(); ++(*it))
    if (!isMarked(**it, treated))
      {
	CAttributeVertex* vertex = findVertex(**it);
	
	if (vertex!=NULL)
	  {
	    CCoverage* it2 = getDynamicCoverage(**it, orbit);
	    for (; it2->cont(); ++(*it2))
	      {
		setMark(**it2,treated);
		(**it2)->setDirectInfo(ADirectInfoIndex, vertex);
	      }
	    
	    delete it2;
	  }
      }

  delete it;
  
  unmarkOrbit(ADart, AOrbit, treated);
  freeMark(treated);
}
//******************************************************************************
