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
bool CGMapGeneric::canQuadrangulateFace(CDart * ADart,
					bool AInsertVertices /*true*/)
{
  int nb = getNbPolylineEdges(ADart);
  return (nb != 0) && (AInsertVertices || nb % 2 == 0);
}
//******************************************************************************
CDart * CGMapGeneric::quadrangulateFace(CDart * ADart, bool AInsertVertices)
{
  assert(ADart!=NULL);
  assert(canQuadrangulateFace(ADart, AInsertVertices));

  int treated = getNewMark();

  // Insertion de nouveaux sommets (chaque arête est coupée en deux):
  if (AInsertVertices)
    {
      for (CStaticCoverage01 cov(this, ADart); cov.cont(); ++cov)
	if (!isMarked(*cov, treated))
	  {
	    markOrbit(*cov, ORBIT_0, treated);
	    negateMaskMark(treated);
	    insertVertex(*cov);
	    negateMaskMark(treated);
	  }

      unmarkOrbit(ADart, ORBIT_01, treated);
    }

  // Quadrangulation:
  assert(canQuadrangulateFace(ADart, false));

  CStaticCoverage01 cov(this, ADart);
  int toTreat = getNewMark();

  // Marquage des sommets qui doivent être traités (un sur deux):
  for (; cov.cont(); ++cov)
    {
      if (isMarked(*cov, toTreat))
	{
	  if (!isFree1(*cov))
	    setMark(alpha1(*cov), toTreat);
	}
      else
	{
	  if (!isFree0(*cov))
	    setMark(alpha0(*cov), toTreat);
	}
    }

  for (cov.reinit(); cov.cont(); setMark(*cov, treated), ++cov)
    if (isMarked(*cov, toTreat))
      {
	CDart * n1 = addMapDart();
	CDart * n2 = addMapDart(); linkAlpha0(n1,n2);
	
	CDart * nn1 = NULL, * nn2 = NULL, * d3;
	
	if (isFree3(*cov))
	  d3=NULL;
	else
	  {
	    d3=alpha3(*cov);
	    nn1=addMapDart();
	    nn2=addMapDart(); linkAlpha0(nn1,nn2);
	    linkAlpha3(n1,nn1);
	    linkAlpha3(n2,nn2);
	  }
	
  	if (!isFree0(*cov) && !isFree1(alpha0(*cov)) &&
	    !isFree0(alpha01(*cov)) && isMarked(alpha010(*cov), treated))
	  {
	    linkAlpha1(n2,alpha01010(*cov));
	    if (d3!=NULL) linkAlpha1(nn2,alpha01010(d3));
	  }
	
	if (!isFree1(*cov) && isMarked(alpha1(*cov), treated))
	  {
	    topoSew2(n1,alpha1(alpha1(*cov)));
	    if (d3!=NULL) topoSew2(nn1,alpha1(alpha1(d3)));
	  }
	
	linkAlpha1(*cov,n1);
	if (d3!=NULL) linkAlpha1(d3,nn1);
      }

  for (cov.reinit(); cov.cont(); ++cov)
    {
      unsetMark(*cov, treated);
      unsetMark(*cov, toTreat);
    }

  freeMark(treated);
  freeMark(toTreat);

  if (isFree0(ADart))
    ADart = alpha1(ADart);

  assert(checkTopology());

  return alpha010(ADart);
}
//******************************************************************************
int CGMapGeneric::quadrangulateMarkedFaces(int AMarkNumber,
					    bool AInsertVertices)
{
  int nbQuadrangulated = 0;

  int selected = getNewMark();

  // Sélection des faces qui peuvent être quadrangulées:

  int treated = getNewMark();
  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    if (canQuadrangulateFace(*it, AInsertVertices))
	      setMark(*it, selected);

	    markOrbit(*it, ORBIT_FACE, treated);
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);

  // Insertion de nouveaux sommets (chaque arête est coupée en deux):

  if (AInsertVertices)
    {
      int edges = getNewMark();

      markIncidentCells(ORBIT_01, selected, edges);

      insertVertexOnMarkedEdges(edges);

      unmarkAll(edges);
      freeMark(edges);
    }

  // Quadrangulation:

  for (it.reinit(); it.cont(); ++it)
    if (isMarked(*it, selected))
      {
	quadrangulateFace(*it, false);
	++nbQuadrangulated;
	unsetMark(*it, selected);
      }

  freeMark(selected);

  return nbQuadrangulated;
}
//******************************************************************************
