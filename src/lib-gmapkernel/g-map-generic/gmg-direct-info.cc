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
#include "g-map-generic.hh"
#include "alt-stdint.hh"
using namespace GMap3d;
//******************************************************************************
void CGMapGeneric::allocMemoryToDirectInfo(int AMarkNumber,
					   int ADirectInfoIndex,
					   int ABytes, TOrbit AOrbitUsed)
{
  assert(ABytes>0);

  if (AOrbitUsed==ORBIT_NONE)
    {
      for (CDynamicCoverageAll it(this); it.cont(); ++it)
	if (isMarked(*it, AMarkNumber))
	  setDirectInfo(*it, ADirectInfoIndex, new uint8_t[ABytes]);
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

		setDirectInfo(owner, ADirectInfoIndex, new uint8_t[ABytes]);

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
void CGMapGeneric::allocMemoryToDirectInfo(CDart * ADart, TOrbit AOrbit,
					   int ADirectInfoIndex,
					   int ABytes, TOrbit AOrbitUsed)
{
  assert(ADart!=NULL);

  int mark = getNewMark();

  markOrbit(ADart, AOrbit, mark);
  allocMemoryToDirectInfo(mark, ADirectInfoIndex, ABytes, AOrbitUsed);
  unmarkOrbit(ADart, AOrbit, mark);

  freeMark(mark);
}
//******************************************************************************
void CGMapGeneric::freeMemoryFromDirectInfo(int AMarkNumber,
					    int ADirectInfoIndex,
					    TOrbit AOrbitUsed)
{
  if (AOrbitUsed==ORBIT_NONE)
    {
      for (CDynamicCoverageAll it(this); it.cont(); ++it)
	if (isMarked(*it, AMarkNumber))
	  delete (uint8_t *) getDirectInfo(*it, ADirectInfoIndex);
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

		delete (uint8_t *) getDirectInfo(owner, ADirectInfoIndex);

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
void CGMapGeneric::freeMemoryFromDirectInfo(CDart * ADart, TOrbit AOrbit,
					    int ADirectInfoIndex,
					    TOrbit AOrbitUsed /*ORBIT_NONE*/)
{
  assert(ADart!=NULL);

  int mark = getNewMark();

  markOrbit(ADart, AOrbit, mark);
  freeMemoryFromDirectInfo(mark, ADirectInfoIndex, AOrbitUsed);
  unmarkOrbit(ADart, AOrbit, mark);

  freeMark(mark);
}
//******************************************************************************
void CGMapGeneric::saveAlphaInDirectInfo(int AAlphaIndex, int ADirectInfoIndex)
{
  assert(0 <= AAlphaIndex && AAlphaIndex <= 3);

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    setDirectInfo(*it, ADirectInfoIndex, alpha(*it, AAlphaIndex));
}
//******************************************************************************
