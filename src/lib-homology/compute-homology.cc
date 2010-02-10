/*
 * lib-homology : Computation of homology generators.
 * Copyright (C) 2010, Samuel Peltier, Université de Poitiers, Laboratoire SIC-XLIM
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2010, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-homology
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
#include "MatricePMQ.hh"
#include "compute-homology.hh"
using namespace GMap3d;
//******************************************************************************
CHomology::CHomology(CGMapVertex* AMap) :
  FMap(AMap),
  FNbVertices(0),
  FNbEdges(0),
  FNbFaces(0),
  FNbVolumes(0)
{
  for (int i=0;i<3;++i)
    FMatrix[i]=NULL;
}
//******************************************************************************
CHomology::~CHomology()
{
  for (int i=0;i<3;++i)
    delete FMatrix[i];
}
//******************************************************************************
void CHomology::computeIncidence(int ADim)
{
  assert( 0<ADim && ADim<=3 );
  
  unsigned long int currentcell = 0;
  int treated = FMap->getNewMark();

  int index = FMap->getNewDirectInfo();

  // 1) We number each (ADim-1)-cell of the map
  CDynamicCoverageAll it(FMap);
  for (; it.cont(); ++it)
    {
      if (!FMap->isMarked(*it, treated))
  	{
	  CCoverage* it2 = FMap->getDynamicCoverage(*it,ORBIT_CELL[ADim-1]);
	  for(;it2->cont();++(*it2))
	    {
	      FMap->setDirectInfo(**it2,index,(void*)currentcell);
	      FMap->setMark(**it2, treated);
	    }
	  delete it2;
	  ++currentcell;	  
	}
    }
  FMap->negateMaskMark(treated);

  // 2) We run through all the ADim-cell of the map and compute the incidence
  //    number with (ADim-1)-cells
  currentcell = 0;
  for (it.reinit(); it.cont(); ++it)
    {
      if (!FMap->isMarked(*it, treated))
  	{
	  CCoverage* it2 = FMap->getDynamicCoverage(*it,ORBIT_CELL[ADim]);
	  for(;it2->cont();++(*it2))
	    {
	      // On va calculer plusieurs fois les mêmes valeurs.
	      // on pourrait marquer markOrbit(*it2,ORBIT_CELL[ADim]) pour éviter ça
	      FMatrix[ADim-1]->setValPMQ( (long int)FMap->getDirectInfo(**it2, index),
					  currentcell,
					  FMap->computeIncidenceNumber(**it2,ADim-1,**it2));
	      FMap->setMark(**it2, treated);
	    }
	  delete it2;
	  ++currentcell;
	}
    }
  
  FMap->negateMaskMark(treated);
  FMap->freeMark(treated);
  FMap->freeDirectInfo(index);
}
//******************************************************************************
void CHomology::computeHomology()
{
  FMap->countCells(-1,&FNbVertices,&FNbEdges,&FNbFaces,&FNbVolumes,NULL,NULL);  
  
  delete FMatrix[0]; FMatrix[0] = new MatricePMQ(FNbVertices, FNbEdges);
  delete FMatrix[1]; FMatrix[1] = new MatricePMQ(FNbEdges, FNbFaces);

  computeIncidence(1);
  computeIncidence(2);

  FMatrix[0]->smithForm();
  FMatrix[1]->getM()->setMatrice( FMatrix[1]->getM()->multGauche(FMatrix[0]->getQ()));
  FMatrix[1]->getP()->setMatrice( FMatrix[0]->getQinv() );
  FMatrix[1]->getPinv()->setMatrice( FMatrix[0]->getQ() );
  FMatrix[1]->smithForm();
  
  //  FMatrix[2] = new MatricePMQ(FNbVolumes, FNbEdges);
}
//******************************************************************************
/*void CHomology::displayMatrixes()
{
  for (int i=0;i<3;++i)
    if ( FMatrix[i]!=NULL ) 
    }*/
//******************************************************************************
