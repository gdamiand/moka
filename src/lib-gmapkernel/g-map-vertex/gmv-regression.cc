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
void CGMapVertex::computeRegressionLine(int AMarkNumber,
					CVertex * ABarycenter,
					CVertex * ADirection)
{
  assert(ABarycenter != NULL);
  assert(ADirection != NULL);

  int N = 0;

  // Calcul du barycentre des sommets marqués:
  CVertex b = barycenter(AMarkNumber);

  // Calcul de la variance en X et des covariances X/Y et X/Z:
  TCoordinate vX  = 0;
  TCoordinate vXY = 0;
  TCoordinate vXZ = 0;

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    markOrbit(*it, ORBIT_VERTEX, treated);
	    CVertex & v = * findVertex(*it);

	    ++N;
	    vX  += sqr(v.getX() - b.getX());

	    vXY += (v.getX() - b.getX()) * (v.getY() - b.getY());
	    vXZ += (v.getX() - b.getX()) * (v.getZ() - b.getZ());
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);

  if (N > 0)
    {
      vX  /= N;
      vXY /= N;
      vXZ /= N;
    }

  /* La droite de régression 3D alpha pour équation paramétrique:
   *
   * x = b.getX() + t * vX
   * y = b.getY() + t * vXY
   * z = b.getZ() + t * vXZ
   */

  * ABarycenter = b;
  * ADirection = CVertex(vX, vXY, vXZ);
}
//******************************************************************************
void CGMapVertex::computeRegressionPlane(int AMarkNumber,
					 TCoordinate * AA, TCoordinate * AB,
					 TCoordinate * AC, TCoordinate * AD)
{
  assert(AA != NULL);
  assert(AB != NULL);
  assert(AC != NULL);
  assert(AD != NULL);

  int N = 0;

  // Calcul du barycentre des sommets marqués:
  CVertex b = barycenter(AMarkNumber);

  // Calcul des variances en X et en Y et des covariances X/Z et Y/Z:
  TCoordinate vX  = 0;
  TCoordinate vY  = 0;
  TCoordinate vXY = 0;
  TCoordinate vXZ = 0;
  TCoordinate vYZ = 0;

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    markOrbit(*it, ORBIT_VERTEX, treated);
	    CVertex & v = * findVertex(*it);

	    ++N;
	    vX  += sqr(v.getX() - b.getX());
	    vY  += sqr(v.getY() - b.getY());

	    vXY += (v.getX() - b.getX()) * (v.getY() - b.getY());
	    vXZ += (v.getX() - b.getX()) * (v.getZ() - b.getZ());
	    vYZ += (v.getY() - b.getY()) * (v.getZ() - b.getZ());
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);

  if (N > 0)
    {
      vX  /= N;
      vY  /= N;
      vXY /= N;
      vXZ /= N;
      vYZ /= N;
    }

  /* Le plan de régression 3D alpha pour équation cartésienne:
   *
   * z = alpha*x + b*y + c
   *
   * Avec: alpha * vX  + b * vXY = vXZ
   *       alpha * vXY + b * vY  = vYZ
   *
   *
   *
   * alpha = vY*vXZ
   * b = vX*vYZ
   * c = - vX*vY
   * d = - b.getX()*vXZ - b.getY()*vYZ + b.getZ()*vX*vY
   */

  for (float i=-1; i<=+1; i += 0.1)
    for (float j=-1; j<=+1; j += 0.1)
      {
	TCoordinate x = b.getX() + i * vX ;
	TCoordinate y = b.getY() + j * vXY;
	TCoordinate z = b.getZ() + i * vXZ + j * vYZ;

  	addMapDart(CVertex(x,y,z));
      }

  * AA = vY*vXZ;
  * AB = vX*vYZ;
  * AC = - vX*vY;
  * AD = - b.getX()* *AA - b.getY()* *AB - b.getZ()* *AC;
}
//******************************************************************************
