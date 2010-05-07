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
CHomology::CHomology(CGMapVertex* AMap, int AMark) :
  FMap(AMap),
  FMark(AMark),
  FNbVertices(0),
  FNbEdges(0),
  FNbFaces(0),
  FNbVolumes(0),
  FShowH0(false),
  FShowH1free(true),
  FShowH1torsion(true),
  FShowH2free(false),
  FShowH2torsion(false),
  FShowH3(false)
{
  // std::cout<<"Marks: "<<AMark1<<", "<<AMark2<<std::endl;
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
unsigned long CHomology::size() const
{
  unsigned long res = 0;
  if ( FMatrix[0]!=NULL ) res += FMatrix[0]->size() + FCells[0].size();
  if ( FMatrix[1]!=NULL ) res += FMatrix[1]->size() + FCells[1].size();
  if ( FMatrix[2]!=NULL ) res += FMatrix[2]->size() + FCells[2].size();
  return  res;
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
void CHomology::computeIncidence(int ADim, bool AComputeNextCells)
{
  assert( 0<=ADim && ADim<3 );
	
  long int currentcell = 1;
  long int number = 0;
	
  int treated = FMap->getNewMark();
  int positive = FMap->getNewMark();
  int index = FMap->getNewDirectInfo();
	
  int treated2 = -1;
  long int currentcell2 = 1;
  if ( AComputeNextCells ) treated2 = FMap->getNewMark();

  // 1) We number each (ADim)-cell of the map
  CDynamicCoverageAll it(FMap);
  for (; it.cont(); ++it)
    {
      if (!FMap->isMarked(*it, treated))
	{
	  if ( ADim==1 ) FMap->markOrbit(*it,ORBIT_23,positive);
	  else if ( ADim==2 )
	    { FMap->halfMarkOrbit(*it,ORBIT_01,positive); }
			
	  FCells[ADim][currentcell-1]=*it;
	  //       if ( ADim==0 )
	  // 	std::cout<<"sommet["<<currentcell-1<<"] : "<<*FMap->findVertex(*it)<<std::endl;      
			
	  CCoverage* it2 = FMap->getDynamicCoverage(*it,ORBIT_CELL[ADim]);
	  for(;it2->cont();++(*it2))
	    {
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
      if ( treated2!=-1 && !FMap->isMarked(*it, treated2) )
	{
	  FCells[ADim+1][currentcell2-1]=*it;
	  FMap->markOrbit(*it, ORBIT_CELL[ADim+1],treated2);
	   ++currentcell2;
	}
    }		
	
  FMap->negateMaskMark(treated);
  if ( treated2!=-1 )
    {
      FMap->negateMaskMark(treated2);
      FMap->freeMark(treated2);
    }
	
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
					
		  // if ( val!=0 ) val = 1;
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
bool CHomology::computeSurfacicHomology()
{
  FMap->countCells(-1,&FNbVertices,&FNbEdges,&FNbFaces,NULL,NULL,NULL);  
	
  delete FMatrix[0]; FMatrix[0] = new MatricePMQ(FNbVertices, FNbEdges);
  delete FMatrix[1]; FMatrix[1] = new MatricePMQ(FNbEdges,    FNbFaces);
  delete FMatrix[2]; FMatrix[2] = NULL;
	
  FCells[0].clear(); FCells[0].reserve(FNbVertices);
  FCells[1].clear(); FCells[1].reserve(FNbEdges);  
  FCells[2].clear(); FCells[2].reserve(FNbFaces);  
	
  if (FCells[0].capacity()<FNbVertices ||
      FCells[1].capacity()<FNbEdges ||
      FCells[2].capacity()<FNbFaces ||
      !FMatrix[0]->valid() ||
      !FMatrix[1]->valid())
    {
      FCells[0].clear(); FCells[1].clear(); FCells[2].clear();
      delete FMatrix[0]; delete FMatrix[1]; 
      FMatrix[0]=NULL; FMatrix[1]=NULL;
      return false;
    }
	
  computeIncidence(0);
  computeIncidence(1,true);

  FMatrix[0]->smithForm();

  FMatrix[1]->getM()->multGauche(FMatrix[0]->getQinv());
  FMatrix[1]->getP()->setMatrice( FMatrix[0]->getQ());
  FMatrix[1]->getPinv()->setMatrice(FMatrix[0]->getQinv());
  FMatrix[1]->smithForm();

  FNbCycleDim0 = FMatrix[0]->getM()->getnbli();
  FNbCycleDim1 = FMatrix[0]->getM()->nbCycle();
  FNbCycleDim2 = FMatrix[1]->getM()->nbCycle();
  FNbCycleDim3 = 0;

  FNbBordFaibleDim0 = FMatrix[0]->getM()->getnbcol()-FNbCycleDim1;
  FNbBordFaibleDim1 = FMatrix[1]->getM()->getnbcol()-FNbCycleDim2;
  FNbBordFaibleDim2 = 0;

  FNbGenLibreDim0 = FNbCycleDim0 - FNbBordFaibleDim0;
  FNbGenLibreDim1 = FNbCycleDim1 - FNbBordFaibleDim1;
  FNbGenLibreDim2 = FNbCycleDim2;
  FNbGenLibreDim3 = 0;
  
  FNbGenTorsionDim1 = FMatrix[1]->getM()->nbTorsion();
  FNbGenTorsionDim2 = 0;

  updateSelectedDarts();
	
  return true;
}
//******************************************************************************
bool CHomology::computeVolumicHomology()
{
  FMap->countCells(-1,&FNbVertices,&FNbEdges,&FNbFaces,&FNbVolumes,NULL,NULL);  
	
  delete FMatrix[0]; FMatrix[0] = new MatricePMQ(FNbVertices, FNbEdges);
  delete FMatrix[1]; FMatrix[1] = new MatricePMQ(FNbEdges,    FNbFaces);
  delete FMatrix[2]; FMatrix[2] = new MatricePMQ(FNbFaces,    FNbVolumes);
	
  FCells[0].clear(); FCells[0].reserve(FNbVertices);
  FCells[1].clear(); FCells[1].reserve(FNbEdges);  
  FCells[2].clear(); FCells[2].reserve(FNbFaces);  
	
  if (FCells[0].capacity()<FNbVertices ||
      FCells[1].capacity()<FNbEdges ||
      FCells[2].capacity()<FNbFaces ||
      !FMatrix[0]->valid() ||
      !FMatrix[1]->valid() ||
      !FMatrix[2]->valid())
    {
      FCells[0].clear(); FCells[1].clear(); FCells[2].clear();
      delete FMatrix[0]; delete FMatrix[1]; delete FMatrix[2];
      FMatrix[0]=NULL; FMatrix[1]=NULL; FMatrix[2]=NULL; 
      return false;
    }
	
  computeIncidence(0);
  computeIncidence(1);
  computeIncidence(2);
	
  FMatrix[0]->smithForm();

//   std::cout<<"FMatrix[1]->getM() avant Smith"<<std::endl;
//   FMatrix[1]->getM()->affiche();
  
  FMatrix[1]->getM()->multGauche(FMatrix[0]->getQinv());
  FMatrix[1]->getP()->setMatrice( FMatrix[0]->getQ());
  FMatrix[1]->getPinv()->setMatrice(FMatrix[0]->getQinv());
  FMatrix[1]->smithForm();

//   std::cout<<"FMatrix[1]->getM() après Smith"<<std::endl;
//   FMatrix[1]->getM()->affiche();

//   std::cout<<"FMatrix[2]->getM() avant Smith"<<std::endl;
//   FMatrix[2]->getM()->affiche();

  FMatrix[2]->getM()->multGauche(FMatrix[1]->getQinv());
  FMatrix[2]->getP()->setMatrice(FMatrix[1]->getQ());
  FMatrix[2]->getPinv()->setMatrice(FMatrix[1]->getQinv());
  FMatrix[2]->smithForm();	

//   std::cout<<"FMatrix[2]->getM() après Smith"<<std::endl;
//   FMatrix[2]->getM()->affiche();

  FNbCycleDim0 = FMatrix[0]->getM()->getnbli();
  FNbCycleDim1 = FMatrix[0]->getM()->nbCycle();
  FNbCycleDim2 = FMatrix[1]->getM()->nbCycle();
  FNbCycleDim3 = FMatrix[2]->getM()->nbCycle();
	
  FNbBordFaibleDim0 = FMatrix[0]->getM()->getnbcol()-FNbCycleDim1;
  FNbBordFaibleDim1 = FMatrix[1]->getM()->getnbcol()-FNbCycleDim2;
  FNbBordFaibleDim2 = FMatrix[2]->getM()->getnbcol()-FNbCycleDim3;
	
  FNbGenLibreDim0 = FNbCycleDim0 - FNbBordFaibleDim0;
  FNbGenLibreDim1 = FNbCycleDim1 - FNbBordFaibleDim1;
  FNbGenLibreDim2 = FNbCycleDim2 - FNbBordFaibleDim2;
  FNbGenLibreDim3 = FNbCycleDim3;

  FNbGenTorsionDim1 = FMatrix[1]->getM()->nbTorsion();
  FNbGenTorsionDim2 = FMatrix[2]->getM()->nbTorsion();
	
  updateSelectedDarts();
	
  return true;
}

//******************************************************************************
unsigned int CHomology::getH0FreeGenerators()
{ return FNbGenLibreDim0; }
//------------------------------------------------------------------------------
unsigned int CHomology::getH1FreeGenerators()
{ return FNbGenLibreDim1; }
//------------------------------------------------------------------------------
unsigned int CHomology::getH1TorsionGenerators()
{ return FNbGenTorsionDim1; }
//------------------------------------------------------------------------------
unsigned int CHomology::getH2FreeGenerators()
{ return FNbGenLibreDim2; }
//------------------------------------------------------------------------------
unsigned int CHomology::getH2TorsionGenerators()
{ return FNbGenTorsionDim2; }
//------------------------------------------------------------------------------
unsigned int CHomology::getH3FreeGenerators()
{ return FNbGenLibreDim3; }
//******************************************************************************
bool CHomology::getShowH0() const
{ return FShowH0; }
bool CHomology::getShowH1free() const
{ return FShowH1free; }
bool CHomology::getShowH1torsion() const
{ return FShowH1torsion; }
bool CHomology::getShowH2free() const
{ return FShowH2free; }
bool CHomology::getShowH2torsion() const
{ return FShowH2torsion; }
bool CHomology::getShowH3() const
{ return FShowH3; }
//******************************************************************************
void CHomology::setShowH0(bool AValue)
{
  if ( AValue!=FShowH0 )
    {
      FShowH0=AValue;
      updateSelectedDarts();
    }
}
//------------------------------------------------------------------------------
void CHomology::setShowH1free(bool AValue)
{
  if ( AValue!=FShowH1free )
    {
      FShowH1free=AValue;
      updateSelectedDarts();
    }
}
//------------------------------------------------------------------------------
void CHomology::setShowH1torsion(bool AValue)
{
  if ( AValue!=FShowH1torsion )
    {
      FShowH1torsion=AValue;
      updateSelectedDarts();
    }
}
//------------------------------------------------------------------------------
void CHomology::setShowH2free(bool AValue)
{
  if ( AValue!=FShowH2free )
    {
      FShowH2free=AValue;
      updateSelectedDarts();
    }
}
//------------------------------------------------------------------------------
void CHomology::setShowH2torsion(bool AValue)
{
  if ( AValue!=FShowH2torsion )
    {
      FShowH2torsion=AValue;
      updateSelectedDarts();
    }
}
//------------------------------------------------------------------------------
void CHomology::setShowH3(bool AValue)
{
  if ( AValue!=FShowH3 )
    {
      FShowH3=AValue;
      updateSelectedDarts();
    }
}
//******************************************************************************
void CHomology::updateSelectedDarts()
{
  if ( FMatrix[0]==NULL || FMark==-1 ) return;
	
  // 1) We unmark all the darts
  for (CDynamicCoverageAll it(FMap); it.cont(); ++it)
    FMap->unsetMark(*it,FMark);
	
  // 2) we mark the required generators.
  if ( FShowH0 )
    {
      //marquage des cellules faisant partie des H0
      for (int j=FNbBordFaibleDim0;j<FNbBordFaibleDim0+FNbGenLibreDim0;++j)
	{
	  for (int i=0;i<FMatrix[0]->getP()->getnbli();++i)
	    {
	      if (FMatrix[0]->getP()->getVal(i,j)!=0)
		{
		  //marquerLibre la d cellule numero i
		  FMap->markOrbit(FCells[0][i],ORBIT_VERTEX,FMark);
		  //		  std::cout<<"Generateur libre "<<j<<" - Cellule libre: "<<i<<" ("<<FCells[1][i]
		  //			   <<")"<<std::endl;
		}
	    }
	}
    }
	
  if ( FShowH1free )
    {
      //marquage des cellules faisant partie des générateurs libres
      //int deb_libre=nb_bf;
      //int fin_libre=nb_bf+nb_l;
      for (int j=FNbBordFaibleDim1;j<FNbBordFaibleDim1+FNbGenLibreDim1;++j)
	{
	  for (int i=0;i<FMatrix[1]->getP()->getnbli();++i)
	    {
	      if(FMatrix[1]->getP()->getVal(i,j)!=0)
		{
		  //marquerLibre la d cellule numero i
		  FMap->markOrbit(FCells[1][i],ORBIT_EDGE,FMark);
		  //		  std::cout<<"Generateur libre "<<j<<" - Cellule libre: "<<i<<" ("<<FCells[1][i]
		  //			   <<")"<<std::endl;
		}
	    }
	}
    }
	
  if ( FShowH1torsion )
    {
      //marquage des cellules faisant partie des générateurs de torsion
      for (int j=0;j<FNbGenTorsionDim1;j++){
	for (int i=0;i<FMatrix[1]->getP()->getnbli();++i)
	  {
	    if(FMatrix[1]->getP()->getVal(i,j)!=0)
	      {
		//marquerTorsion la d cellule numero i
		FMap->markOrbit(FCells[1][i],ORBIT_EDGE,FMark);
		//		std::cout<<"Cellule torsion: "<<i<<" ("<<FCells[1][i]
		//			 <<")"<<std::endl;
	      }
	  }
      }
    }
	
  if ( FShowH2free )
    {
      if ( FMatrix[2]!=NULL )
	{
	  //marquage des cellules faisant partie des H2 libres
	  for (int j=FNbBordFaibleDim2;j<FNbBordFaibleDim2+FNbGenLibreDim2;++j)
	    {
	      for (int i=0;i<FMatrix[2]->getP()->getnbli();++i)
		{
		  if(FMatrix[2]->getP()->getVal(i,j)!=0)
		    {
		      //marquerLibre la d cellule numero i
		      FMap->markOrbit(FCells[2][i],ORBIT_FACE,FMark);
		      //		  std::cout<<"Generateur libre "<<j<<" - Cellule libre: "<<i<<" ("<<FCells[1][i]
		      //			   <<")"<<std::endl;
		    }
		}
	    }
	}
      else
	{	
	  //marquage des cellules faisant partie des H2 libres
	  for (int j=0;j<FNbCycleDim2;++j)
	    {
	      for (int i=0;i<FMatrix[1]->getQ()->getnbli();++i)
		{
		  if(FMatrix[1]->getQ()->getVal(i,j)!=0)
		    {
		      //marquerLibre la d cellule numero i
		      FMap->markOrbit(FCells[2][i],ORBIT_FACE,FMark);
		    }
		}
	    }
	}
    }

  if ( FShowH2torsion && FMatrix[2]!=NULL )
    {
      //marquage des cellules faisant partie des H2 libres
      for (int j=0;j<FNbGenTorsionDim2;++j )
	{
	  for(int i=0;i<FMatrix[2]->getP()->getnbli();++i)
	    {
	      if(FMatrix[2]->getP()->getVal(i,j)!=0)
		{
		  //marquerLibre la d cellule numero i
		  FMap->markOrbit(FCells[2][i],ORBIT_FACE,FMark);
		  //		  std::cout<<"Generateur libre "<<j<<" - Cellule libre: "<<i<<" ("<<FCells[1][i]
		  //			   <<")"<<std::endl;
		}
	    }
	}
    }
}
