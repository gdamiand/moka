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
using namespace GMap3d;
//******************************************************************************
// Ancienne version, beaucoup plus lente:
//
// INLINE
// int CGMapGeneric::deleteMarkedDarts(int AMarkNumber)
// {
//   int nbDeleted = 0;
//   CDynamicCoverageAll it(this);
//
//   while (it.cont())
//     {
// 	 CDart * dart= it++;
//
// 	 if (isMarked(dart, AMarkNumber))
// 	   {
// 	     for (int i=3; i>=0; --i)
// 	       if (!isFree(dart,i))
// 		 unsew(dart,i);
// 		
// 	     delMapDart(dart);
// 	     ++nbDeleted;
// 	   }
//     }
//
//   return nbDeleted;
// }
//******************************************************************************
int CGMapGeneric::deleteMarkedDarts(int AMarkNumber)
{
  int nbDeleted = 0;
  CDynamicCoverageAll it(this);

  // Décousures:
  for (; it.cont(); ++it)
    if (isMarked(*it, AMarkNumber))
      for (int i=3; i>=0; --i)
  	if (!isFree(*it, i) && !isMarked(alpha(*it, i), AMarkNumber))
  	  unsew(*it, i);

  // Suppresions:
  for (it.reinit(); it.cont(); )
    {
      CDart * current = it++;

      if (isMarked(current, AMarkNumber))
  	{
  	  delMapDart(current);
  	  ++nbDeleted;
  	}
    }

  return nbDeleted;
}
//******************************************************************************
