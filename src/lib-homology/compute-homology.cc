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
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CHomology::CHomology(CGMapVertex* AMap, int AMark1, int AMark2) :
    FMap(AMap),
    FMark1(AMark1),
    FMark2(AMark2),
    FNbVertices(0),
    FNbEdges(0),
    FNbFaces(0),
    FNbVolumes(0)
{
  std::cout<<"Marks: "<<AMark1<<", "<<AMark2<<std::endl;
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
int CHomology::computeIncidenceNumber(CDart* ADart, int ADim,
				      CDart* ADart2, int AIndex)
{
  assert((0 <= ADim) && (ADim < 3));

  // 1) marquer m1 la i cell orientée incidence à ADart
  // 2) marquer m2 la i+1 cell orientée incidence à ADart2
  
  // 2) Parcourir a01,...,a0i incidente à d2


  // si brin non traité
  //    si brin marqué m1 alors +1
  //    sinon -1
  //    marquer traité orbite a0,...,i-1
  
  int incidenceNumber=0;

  int m2oriented = FMap->getNewMark();
  if ( ADim==0 )
    {
      FMap->setMark(ADart2,m2oriented);
      FMap->setMark(ADart2->getAlpha(0),m2oriented);
    }
  else
    {
      FMap->halfMarkOrbit(ADart2, ORBIT_INF[ADim+1], m2oriented);
    }
  
  int treated = FMap->getNewMark();
  
  CCoverage * cov = FMap->getDynamicCoverage(ADart, ORBIT_CELL[ADim]);
  for (; cov->cont(); ++(*cov))
  {
    if ( !FMap->isMarked(**cov,treated) && FMap->isMarked(**cov,m2oriented) )
    {
      if ( (ADim>0 && (long int)FMap->getDirectInfo(**cov,AIndex)>0) ||
	   (ADim==0 && **cov==ADart2) )
	{
	  // std::cout<<"inc incidenceNumber"<<std::endl;
	  ++incidenceNumber;
	}
      else
	{
	  // std::cout<<"dec incidenceNumber"<<std::endl;
	  --incidenceNumber;
	}

     FMap-> markOrbit(**cov,ORBIT_INF[ADim],treated);
    }
  }
  
  for (cov->reinit(); cov->cont(); ++(*cov))
  {
    if ( FMap->isMarked(**cov,treated) )
    {
      FMap->unmarkOrbit(**cov,ORBIT_INF[ADim],treated);
    }
  }
  delete cov;

  if ( ADim==0 )
    {
      FMap->unsetMark(ADart2,m2oriented);
      FMap->unsetMark(ADart2->getAlpha(0),m2oriented);
    }
  else
    {
      FMap->halfUnmarkOrbit(ADart2, ORBIT_INF[ADim+1], m2oriented);
    }
  
  assert(FMap->isWholeMapUnmarked(treated));
  assert(FMap->isWholeMapUnmarked(m2oriented));
  
  // Libérations:
  FMap->freeMark(treated);
  FMap->freeMark(m2oriented);

  return incidenceNumber;
}
//******************************************************************************
void CHomology::computeIncidence(int ADim)
{
  assert( 0<=ADim && ADim<3 );
  
  long int currentcell = 1;
  long int number = 0;
  
  int treated = FMap->getNewMark();
  int positive = FMap->getNewMark();
  int index = FMap->getNewDirectInfo();

  // 1) We number each (ADim)-cell of the map
  CDynamicCoverageAll it(FMap);
  for (; it.cont(); ++it)
  {
    // We take this coverage to unmark all darts.
    if ( FMark1!=-1 ) FMap->unsetMark(*it,FMark1);
    if ( FMark2!=-1 ) FMap->unsetMark(*it,FMark2);
    
    if (!FMap->isMarked(*it, treated))
    {
      if ( ADim==1 ) FMap->markOrbit(*it,ORBIT_23,positive);
      else if ( ADim==2 )
	{ FMap->halfMarkOrbit(*it,ORBIT_01,positive); }
      
      CCoverage* it2 = FMap->getDynamicCoverage(*it,ORBIT_CELL[ADim]);
      for(;it2->cont();++(*it2))
	{
	  FCells[ADim][currentcell-1]=**it2;
	  
	  if ( ADim==1 && !FMap->isMarked(**it2,positive) )
	    number = -currentcell;
	  else if (ADim==2 && !FMap->isMarked(**it2,positive) &&
		   !FMap->isMarked((**it2)->getAlpha(3),positive))
	    number = -currentcell;
	  else
	    number = currentcell;

	  //std::cout<<"Number="<<number<<std::endl;
	  
        FMap->setDirectInfo(**it2,index,(void*)number);
        FMap->setMark(**it2, treated);
      }

      if ( ADim==1 ) FMap->unmarkOrbit(*it,ORBIT_23,positive);
      else if ( ADim==2 )
	{ FMap->halfUnmarkOrbit(*it,ORBIT_01,positive); }      
      
      delete it2;
      ++currentcell;
    }
  }
  FMap->negateMaskMark(treated);

  // 2) We run through all the (ADim+1)-cell of the map and compute the incidence
  //    number with ADim-cells
  currentcell = 0;
  for ( it.reinit(); it.cont(); ++it )
  {
    if ( !FMap->isMarked(*it, treated) )
    {
      CCoverage* it2 = FMap->getDynamicCoverage(*it,ORBIT_CELL[ADim+1]);
      for( ;it2->cont();++(*it2) )
      {
	int i = (long int)FMap->getDirectInfo(**it2, index);
	int icorrected = i;

	if ( i<0 ) icorrected = -i;
	--icorrected;	

	if ( FMatrix[ADim]->getValPMQ( icorrected, currentcell)==0 )
	  {
	    int val = computeIncidenceNumber(**it2, ADim, *it, index);
	    
	    // std::cout<<"Val="<<val<<", puis ";
	    
	    //if ( i<0 ) {  val = -val; }
	    
	    // std::cout<<"("<<i<<","<<currentcell<<")="<<val<<std::endl;
	    
	    FMatrix[ADim]->setValPMQ( icorrected, currentcell, val);
	  }
	
	FMap->setMark(**it2, treated);
      }
      delete it2;
      ++currentcell;
      // std::cout<<"Next cell : "<<currentcell<<std::endl;
    }
  }
  
  FMap->negateMaskMark(treated);

  assert(FMap->isWholeMapUnmarked(treated));
  
  FMap->freeMark(treated);
  FMap->freeDirectInfo(index);
}
//******************************************************************************
void CHomology::computeHomology()
{
  FMap->countCells(-1,&FNbVertices,&FNbEdges,&FNbFaces,&FNbVolumes,NULL,NULL);  
  
  delete FMatrix[0]; FMatrix[0] = new MatricePMQ(FNbVertices, FNbEdges);
  delete FMatrix[1]; FMatrix[1] = new MatricePMQ(FNbEdges, FNbFaces);


  FCells[0].clear(); FCells[0].reserve(FNbVertices);
  FCells[1].clear(); FCells[1].reserve(FNbEdges);  
    
  computeIncidence(0);
  computeIncidence(1);

  
  std::cout<<"Matrice d'incidence Sommet-Arete:"<<std::endl;    
  FMatrix[0]->getM()->affiche();
  
  std::cout<<"Matrice d'incidence Arete-Face:"<<std::endl;    
  FMatrix[1]->getM()->affiche();

  
  FMatrix[0]->smithForm();
  
  FMatrix[1]->getM()->setMatrice( FMatrix[1]->getM()->
                                  multGauche(FMatrix[0]->getQ()) );
  FMatrix[1]->getP()->setMatrice( FMatrix[0]->getQinv() );
  FMatrix[1]->getPinv()->setMatrice( FMatrix[0]->getQ() );
  FMatrix[1]->smithForm();

  std::cout<<"Matrice Homologie H0:"<<std::endl;  
  FMatrix[0]->getM()->affiche();

  std::cout<<"Matrice Homologie H1:"<<std::endl;  
  FMatrix[1]->getM()->affiche();

//   std::cout<<"Matrice P pour dim 1:"<<std::endl;  
//   FMatrix[1]->getP()->affiche();  

  int nb_t  = FMatrix[1]->getM()->nbTorsion();
  int nb_z0 = FMatrix[0]->getM()->nbCycle();
  int nb_z1 = FMatrix[1]->getM()->nbCycle();
  int nb_bf = FMatrix[1]->getM()->getnbcol()-nb_z1;
  int nb_l=nb_z0-nb_bf;

  if ( FMark2!=-1 )
    {
      //marquage des cellules faisant partie des générateurs de torsion
      for (int j=0;j<nb_t;j++){
	for(int i=0;i<FMatrix[1]->getP()->getnbcol();i++)
	  {
	    if(FMatrix[1]->getP()->getVal(i,j)!=0)
	      {
		//marquerTorsion la d cellule numero i
		FMap->markOrbit(FCells[1][i],ORBIT_EDGE,FMark2);
		std::cout<<"Cellule torsion: "<<i<<" ("<<FCells[1][i]
			 <<")"<<std::endl;
	      }
	  }
      }
    }

  if ( FMark1!=-1 )
    {
      //marquage des cellules faisant partie des générateurs libres
      int deb_libre=nb_bf;
      int fin_libre=nb_bf+nb_l;
      for (int j=deb_libre;j<fin_libre;j++)
	{
	  for(int i=0;i<FMatrix[1]->getP()->getnbcol();i++)
	    {
	      if(FMatrix[1]->getP()->getVal(i,j)!=0)
		{
		  //marquerLibre la d cellule numero i
		  FMap->markOrbit(FCells[1][i],ORBIT_EDGE,FMark1);
		  std::cout<<"Cellule libre: "<<i<<" ("<<FCells[1][i]
			   <<")"<<std::endl;
		}
	    }
	}
    }
}
//******************************************************************************
/*void CHomology::displayMatrixes()
{
  for (int i=0;i<3;++i)
    if ( FMatrix[i]!=NULL ) 
    }*/
//******************************************************************************
