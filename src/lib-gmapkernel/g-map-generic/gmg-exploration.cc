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
#include <cstring>
using namespace GMap3d;
//******************************************************************************
CDart * CGMapGeneric::go(TMovement ADirection, CDart * ALastDart, TOrbit AOrbit,
			int AMarkNumber, bool ASelect)
{
  assert(ALastDart!=NULL);
  assert(AOrbit==ORBIT_03 || AOrbit==ORBIT_02  || AOrbit==ORBIT_023 ||
	 AOrbit==ORBIT_01 || AOrbit==ORBIT_013 || AOrbit==ORBIT_012);

  static const char * forward[16] =
  {
    /* ----               */ "",
    /* 0---               */ "",
    /* -1--               */ "",
    /* 01-- face  /volume */ "2101",
    /* --2-               */ "",
    /* 0-2- arête /volume */ "1[21]0",
    /* -12-               */ "",
    /* 012- volume/cc     */ "321012",
    /* ---3               */ "",
    /* 0--3 arête /face   */ "10",
    /* -1-3               */ "",
    /* 01-3 face  /cc     */ "2[32]101",
    /* --23               */ "",
    /* 0-23 arête /cc     */ "1[2[32]1]0",
    /* -123               */ "",
    /* 0123               */ ""
  };

  static const char * right[16] =
  {
    /* ----               */ "",
    /* 0---               */ "",
    /* -1--               */ "",
    /* 01-- face  /volume */ "10"     /* 21*/,
    /* --2-               */ "",
    /* 0-2- arête /volume */ "010[2]" /* 21*/,
    /* -12-               */ "",
    /* 012- volume/cc     */ "210"    /*321*/,
    /* ---3               */ "",
    /* 0--3 arête /face   */ "010"    /*  1*/,
    /* -1-3               */ "",
    /* 01-3 face  /cc     */ "10"     /*321*/,
    /* --23               */ "",
    /* 0-23 arête /cc     */ "010[2]" /*321*/,
    /* -123               */ "",
    /* 0123               */ ""
  };

  bool reverse = ADirection==Backward || ADirection==Left;

  const char * series =
    (ADirection==Left || ADirection==Right) ? right[AOrbit] : forward[AOrbit];

  int length = strlen(series);

  // Marquage:
  if (ASelect && !isWholeCellMarked(ALastDart, AOrbit, AMarkNumber))
    {
      markOrbit(ALastDart, AOrbit, AMarkNumber);
      return ALastDart;
    }

  if (!ASelect)
    unmarkOrbit(ALastDart, AOrbit, AMarkNumber);

  // Déplacement:
  CDart * current = ALastDart;
  bool firstSubMove = false;

#define INFINITE (9)

  for (int i=0, depth=0, freezeLevel=INFINITE; i<length; ++i)
    {
      char c = series[reverse ? length-i-1 : i];

      if (c=='[' || c==']')
	{
	  if (reverse ^ (c=='['))
	    {
	      ++depth;
	      firstSubMove = true;
	    }
	  else
	    {
	      --depth;

	      if (depth<freezeLevel)
		freezeLevel = INFINITE;

	      assert(!firstSubMove);
	    }
	}
      else
	{
	  if (depth < freezeLevel)
	    {
	      assert(c=='0' || c=='1' || c=='2' || c=='3');
	      int dim = c-'0';
	
	      if (isFree(current, dim))
		{
		  if (firstSubMove)
		    freezeLevel = depth;
		  else
		    return ALastDart;
		}
	      else
		current = alpha(current, dim);
	    }
	
	  firstSubMove = false;
	}
    }

#undef INFINITE

  // Marquage:
  if (ASelect)
    markOrbit(current, AOrbit, AMarkNumber);
  else
    setMark(current, AMarkNumber);

  return current;
}
//******************************************************************************
CDart * CGMapGeneric::goAsFarAsPossible(TMovement ADirection, CDart * ALastDart,
				       TOrbit AOrbit, int AMarkNumber,
				       bool ASelect)
{
  assert(ALastDart!=NULL);
  assert(AOrbit==ORBIT_03 || AOrbit==ORBIT_02  || AOrbit==ORBIT_023 ||
	 AOrbit==ORBIT_01 || AOrbit==ORBIT_013 || AOrbit==ORBIT_012);

  int treated = getNewMark();

  CDart * current = ALastDart;

  go(ADirection, current, AOrbit, AMarkNumber, ASelect);

  while (!isMarked(current, treated))
    {
      setMark(current, treated);
      current = go(ADirection, current, AOrbit, AMarkNumber, ASelect);
      assert(current!=NULL);
    }

  unmarkAll(treated);
  freeMark(treated);

  return current;
}
//******************************************************************************
