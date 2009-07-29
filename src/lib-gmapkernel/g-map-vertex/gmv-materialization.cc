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
#include "geometry.hh"
using namespace GMap3d;
//******************************************************************************
CDart * CGMapVertex::materializeBarycenter(int AMarkNumber)
{
  return addMapDart(barycenter(AMarkNumber));
}
//******************************************************************************
CDart * CGMapVertex::materializeAxe(CDart * ADart1, CDart * ADart2)
{
  assert(ADart1!=NULL && ADart2!=NULL);
  assert(!isSameOrbit(ADart1,ADart2, ORBIT_VERTEX));

  ADart1= addMapDart(*findVertex(ADart1));
  ADart2= addMapDart(*findVertex(ADart2));
  linkAlpha0(ADart1,ADart2);
  return ADart2;
}
//******************************************************************************
CDart * CGMapVertex::materializePlane(CDart * ADart1, CDart * ADart2, CDart * ADart3)
{
  assert(ADart1!=NULL && ADart2!=NULL && ADart3!=NULL);
  assert(!isSameOrbit(ADart1,ADart2, ORBIT_VERTEX) &&
	 !isSameOrbit(ADart1,ADart3, ORBIT_VERTEX) &&
	 !isSameOrbit(ADart2,ADart3, ORBIT_VERTEX));

  CDart * d= createTopoTriangle();

  setVertex(      d , *findVertex(ADart3));
  setVertex(alpha01  (d), *findVertex(ADart2));
  setVertex(alpha0101(d), *findVertex(ADart1));
  return d;
}
//******************************************************************************
CDart * CGMapVertex::materializeNormalVector(CDart * ADart)
{
  assert(ADart!=NULL);

  CVertex b= barycenter(ADart, ORBIT_FACE);
  CVertex n= faceNormalVector(ADart);
  assert(!n.isNull());
  n/= n.norm();

  CDart * B= addMapDart(b  );
  CDart * N= addMapDart(b+n);
  linkAlpha0(B,N);
  return N;
}
//******************************************************************************
CDart * CGMapVertex::materializeNormalPlane(CDart * ADart1, CDart * ADart2)
{
  assert(ADart1!=NULL && ADart2!=NULL);
  assert(!isSameOrbit(ADart1,ADart2, ORBIT_VERTEX));

  CTransformationMatrix rotationMatrix(CTransformationMatrix::IdentityMatrix);

  const CVertex & S1= *findVertex(ADart1);
  const CVertex & S2= *findVertex(ADart2);

  rotationMatrix.rotate(S2-S1, 120);

  CVertex V1= CGeometry::getNormalVector(S2-S1);
  CVertex V2= V1; rotationMatrix.applyOn(V2);
  CVertex V3= V2; rotationMatrix.applyOn(V3);

  CDart * d= createTopoTriangle();

  setVertex(      d , S1+V1);
  setVertex(alpha01  (d), S1+V2);
  setVertex(alpha0101(d), S1+V3);
  return d;
}
//******************************************************************************
CDart * CGMapVertex::materializeReferential()
{
  CDart * dart[12];

  int i;

  for (i=0; i<12; ++i)
    dart[i]= addMapDart();

  for (i=0; i<6; ++i)
    linkAlpha0(dart[2*i], dart[2*i+1]);

  for (i=0; i<3; ++i)
    {
      linkAlpha1(dart[4*i],dart[(4*i+11)%12]);
      topoSew2(dart[4*i], dart[4*i+3]);
    }

  setVertex(dart[ 0], ORIGIN);
  setVertex(dart[ 2], OX);
  setVertex(dart[ 6], OY);
  setVertex(dart[10], OZ);

  return dart[0];
}
//******************************************************************************
