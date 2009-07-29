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
#include "g-map-vertex.hh"
#include "mesh-vertex.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CVertex* CMeshVertex::allocVertexArray1(int ADimX)
{
  assert(ADimX>0);

  return new CVertex[ADimX];
}
//------------------------------------------------------------------------------
void CMeshVertex::freeVertexArray1(CVertex* AArray1, int /*ADimX*/)
{
  assert(AArray1!=NULL);

  delete [] AArray1;
}
//******************************************************************************
CVertex** CMeshVertex::allocVertexArray2(int ADimX, int ADimY)
{
  assert(ADimX>0);
  assert(ADimY>0);

  CVertex** result = new CVertex* [ADimX];

  for (int i=0; i<ADimX; ++i)
    result[i] = allocVertexArray1(ADimY);

  return result;
}
//------------------------------------------------------------------------------
void CMeshVertex::freeVertexArray2(CVertex** AArray2, int ADimX, int ADimY)
{
  assert(AArray2!=NULL);

  for (int i=0; i<ADimX; ++i)
    freeVertexArray1(AArray2[i], ADimY);

  delete [] AArray2;
}
//******************************************************************************
CVertex*** CMeshVertex::allocVertexArray3(int ADimX, int ADimY, int ADimZ)
{
  assert(ADimX>0);
  assert(ADimY>0);
  assert(ADimZ>0);

  CVertex*** result = new CVertex** [ADimX];

  for (int i=0; i<ADimX; ++i)
    result[i] = allocVertexArray2(ADimY, ADimZ);

  return result;
}
//------------------------------------------------------------------------------
void CMeshVertex::freeVertexArray3(CVertex*** AArray3,
				   int ADimX, int ADimY, int ADimZ)
{
  assert(AArray3!=NULL);

  for (int i=0; i<ADimX; ++i)
    freeVertexArray2(AArray3[i], ADimY, ADimZ);

  delete [] AArray3;
}
//******************************************************************************
const CVertex** CMeshVertex::allocVertexPtrArray1(int ADimX)
{
  assert(ADimX>0);

  return new const CVertex* [ADimX];
}
//------------------------------------------------------------------------------
void CMeshVertex::freeVertexPtrArray1(const CVertex** AArray1, int /*ADimX*/)
{
  assert(AArray1!=NULL);

  delete [] AArray1;
}
//******************************************************************************
const CVertex*** CMeshVertex::allocVertexPtrArray2(int ADimX, int ADimY)
{
  assert(ADimX>0);
  assert(ADimY>0);

  const CVertex*** result = new const CVertex** [ADimX];

  for (int i=0; i<ADimX; ++i)
    result[i] = allocVertexPtrArray1(ADimY);

  return result;
}
//------------------------------------------------------------------------------
void CMeshVertex::freeVertexPtrArray2(const CVertex*** AArray2,
				      int ADimX, int ADimY)
{
  assert(AArray2!=NULL);

  for (int i=0; i<ADimX; ++i)
    freeVertexPtrArray1(AArray2[i], ADimY);

  delete [] AArray2;
}
//******************************************************************************
const CVertex**** CMeshVertex::allocVertexPtrArray3(int ADimX, int ADimY, int ADimZ)
{
  assert(ADimX>0);
  assert(ADimY>0);
  assert(ADimZ>0);

  const CVertex**** result = new const CVertex*** [ADimX];

  for (int i=0; i<ADimX; ++i)
    result[i] = allocVertexPtrArray2(ADimY, ADimZ);

  return result;
}
//------------------------------------------------------------------------------
void CMeshVertex::freeVertexPtrArray3(const CVertex**** AArray3,
				      int ADimX, int ADimY, int ADimZ)
{
  assert(AArray3!=NULL);

  for (int i=0; i<ADimX; ++i)
    freeVertexPtrArray2(AArray3[i], ADimY, ADimZ);

  delete [] AArray3;
}
//******************************************************************************
