/*
 * lib-mesh : Opérations de maillage et lissage.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-mesh
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
#include "mesh-generic.hh"
#include "g-map-generic.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
#define SUCC(DART) ((((CGMapGeneric*) FMap)->*succ)(DART))
//------------------------------------------------------------------------------
CDart* CMeshGeneric::markTopoSquareIMeshed(int ADim, CDart* ADart,
					   int ASx, int ASy,
					   int AMarkNumberX, int AMarkNumberY,
					   int AReturnedDart)
{
  assert(ADim>=0 && ADim<=2);
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(isTopoSquareIMeshed(ADim, ADart, ASx,ASy));
  assert(AMarkNumberX>0);
  assert(AMarkNumberY>0);
  assert(AReturnedDart>=1 && AReturnedDart<=3);

  CDart* returned = NULL;

  if (ADim==0)
    ASx = ASy = 1;

  CDart* (CGMapGeneric::* succ) (CDart*) const =
    ADim<2 ? & CGMapGeneric::alpha01 : & CGMapGeneric::alpha0121;

  CDart* current = ADart;

  for (int round=0; round<2; ++round)
    {
      FMap->markOrbit(current, ORBIT_23, AMarkNumberX);

      for (int i=1; i<ASx; ++i)
  	current = SUCC(current);

      current = FMap->alpha0(current);
      FMap->markOrbit(current, ORBIT_23, AMarkNumberX);

      if (round==0 && AReturnedDart==1)
  	returned = current;

      if (round==1 && AReturnedDart==2)
  	returned = current;

      current = FMap->alpha1(current);

      FMap->markOrbit(current, ORBIT_23, AMarkNumberY);
      for (int j=1; j<ASy; ++j)
  	current = SUCC(current);

      current = FMap->alpha0(current);

      FMap->markOrbit(current, ORBIT_23, AMarkNumberY);

      current = FMap->alpha1(current);

      if (round==0 && AReturnedDart==3)
  	returned = current;
    }

  return returned;
}
//******************************************************************************
void CMeshGeneric::markTopoCubeIMeshed(int ADim, CDart* ADart,
  				       int ASx, int ASy, int ASz,
  				       int AMarkNumberX,
  				       int AMarkNumberY,
  				       int AMarkNumberZ)
{
  assert(ADim>=0 && ADim<=3);
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);
  assert(isTopoCubeIMeshed(ADim, ADart, ASx,ASy,ASz));
  assert(AMarkNumberX>0);
  assert(AMarkNumberY>0);
  assert(AMarkNumberZ>0);

  if (ADim==0)
    ASx = ASy = ASz = 1;

  CDart* current;

  current = markTopoSquareIMeshed(ADim, ADart,
  				  ASx,ASy, AMarkNumberX,AMarkNumberY, 3);
  /* --- */ markTopoSquareIMeshed(ADim, FMap->alpha12(current),
  				  ASy,ASz, AMarkNumberY,AMarkNumberZ   );

  current = markTopoSquareIMeshed(ADim, FMap->alpha12(ADart),
  				  ASy,ASz, AMarkNumberY,AMarkNumberZ, 3);
  /* --- */ markTopoSquareIMeshed(ADim, FMap->alpha12(current),
  				  ASz,ASx, AMarkNumberZ,AMarkNumberX   );

  current = markTopoSquareIMeshed(ADim, FMap->alpha1212(ADart),
  				  ASz,ASx, AMarkNumberZ,AMarkNumberX, 3);
  /* --- */ markTopoSquareIMeshed(ADim, FMap->alpha12(current),
  				  ASx,ASy, AMarkNumberX,AMarkNumberY   );
}
//------------------------------------------------------------------------------
#undef SUCC
//******************************************************************************
