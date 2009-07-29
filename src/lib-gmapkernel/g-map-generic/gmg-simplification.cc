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
#include <stack>
using namespace GMap3d;
using namespace std;
//******************************************************************************
int CGMapGeneric::deleteFlatFaces()
{
   int mark = getNewMark();
   negateMaskMark(mark);
   int nb = deleteMarkedFlatFaces(mark);
   negateMaskMark(mark);
   freeMark(mark);
   return nb;
}
//------------------------------------------------------------------------------
int CGMapGeneric::deleteMarkedFlatFaces(int AMarkNumber)
{
   return contractMarkedCells(AMarkNumber, 2, true);
}
//******************************************************************************
int CGMapGeneric::deleteFlatVolumes()
{
   int mark = getNewMark();
   negateMaskMark(mark);
   int nb = deleteMarkedFlatVolumes(mark);
   negateMaskMark(mark);
   freeMark(mark);
   return nb;
}
//------------------------------------------------------------------------------
int CGMapGeneric::deleteMarkedFlatVolumes(int AMarkNumber)
{
   return contractMarkedCells(AMarkNumber, 3, true);
}
//------------------------------------------------------------------------------
int CGMapGeneric::removeMarkedDanglingEdges(int AMarkNumber)
{
   int toDelete = getNewMark();
   int nb = 0;
   std::stack<CDart*> FToTreat; // Pile des arêtes à traiter
   CDart* current = NULL;

   markIncidentCells(ORBIT_EDGE, AMarkNumber);

   CDynamicCoverageAll it(this);
   while (it.cont())
   {
      while (! FToTreat.empty())
      {
         current = FToTreat.top();
         FToTreat.pop();

         if (isMarked(current, AMarkNumber) && !isMarked(current, toDelete))
         {
            if (alpha12(current) == alpha21(current))
               FToTreat.push(alpha1(current));

            if (alpha012(current) == alpha021(current))
               FToTreat.push(alpha01(current));

            remove(current, 1, false);
            markOrbit(current, ORBIT_EDGE, toDelete);
            ++nb;
         }
      }

      if (isMarked(*it, AMarkNumber) && !isMarked(*it, toDelete) &&
            alpha1(*it) == alpha2(*it))
      {
         if (alpha12(*it) == alpha21(*it))
            FToTreat.push(alpha1(*it));

         if (alpha012(*it) == alpha021(*it))
            FToTreat.push(alpha01(*it));

         remove(*it, 1, false);
         markOrbit(*it, ORBIT_EDGE, toDelete);
         ++nb;
      }

      ++it;
   }

   for (it.reinit(); it.cont();)
   {
      if (isMarked(*it, toDelete))
         delMapDart(it++);
      else
         ++it;
   }

   freeMark(toDelete);
   return nb;
}
//------------------------------------------------------------------------------
int CGMapGeneric::removeDanglingEdges()
{
   int mark = getNewMark();
   negateMaskMark(mark);
   int nb = removeMarkedDanglingEdges(mark);
   negateMaskMark(mark);
   freeMark(mark);
   return nb;
}
//******************************************************************************
