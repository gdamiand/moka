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
#include <iostream>
#include <sstream>
#include <cstdlib>
using namespace std;
using namespace GMap3d;
//******************************************************************************
bool CGMapGeneric::belongToSameOrientedOrbit(CDart* ADart1, CDart* ADart2,
					     TOrbit AOrbit)
{
  int mark = getNewMark();

  halfMarkOrbit(ADart1, AOrbit, mark);
  bool same = isMarked(ADart2, mark);
  unmarkOrbit(ADart1, AOrbit, mark);

  freeMark(mark);

  return same;
}
//******************************************************************************
bool CGMapGeneric::isOrientable(CDart * ADart, TOrbit AOrbit)
{
  assert(ADart!=NULL);
  assert(AOrbit>=ORBIT_SELF && AOrbit<=ORBIT_CC);

  if (AOrbit!=ORBIT_VERTEX && AOrbit!=ORBIT_VOLUME && AOrbit!=ORBIT_CC)
    return true;

  bool orientable = true;

  bool usedDim[4];
  int selected = getNewMark();

  for (int dim=0; dim<=3; ++dim)
    usedDim[dim] = AND_ORBIT(AOrbit, ORBIT_DIM[dim]) != ORBIT_SELF;

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  setMark(**cov, selected);

  for (; cov->cont() && orientable; ++(*cov))
    if (isMarked(**cov, selected))
      {
	for (int dim=0; dim<=3; ++dim)
	  if (usedDim[dim] && !isFree(**cov, dim) &&
	      isMarked(alpha(**cov, dim), selected))
	    orientable = false;
      }
    else
      {
	for (int dim=0; dim<=3; ++dim)
	  if (usedDim[dim] && !isFree(**cov, dim))
	    setMark(alpha(**cov, dim), selected);
      }

  delete cov;

  unmarkOrbit(ADart, AOrbit, selected);
  freeMark(selected);

  return orientable;
}
//******************************************************************************
unsigned int CGMapGeneric::countNonLocalDegreeTwoEdges()
{
  unsigned int res = 0;
  int mark = getNewMark();
  CDynamicCoverageAll it(this);
  for (; it.cont(); ++it)
    {
      if ( !isMarked(*it, mark) )
	{
	  if (!isLocalDegreeTwoSup(*it, 1)) ++res;
	  markOrbit(*it, ORBIT_EDGE,mark);
	}
    }
  negateMaskMark(mark);
  freeMark(mark);
  return res;
}
//******************************************************************************
void CGMapGeneric::countBorders(int /*AMarkNumber*/,
				int * ANb0, int * ANb1, int * ANb2, int * ANb3)
{
  int * count[4] = { ANb0, ANb1, ANb2, ANb3 };
  int dim[4];
  int borderMark[4];
  int nbAsked = 0;
  int i;

  // Initialisations:
  for (i=0; i<4; ++i)
    if (count[i] != NULL)
      {
	count[nbAsked] = count[i];
	* count[nbAsked] = 0;
	dim[nbAsked] = i;
	borderMark[nbAsked] = getNewMark();
	++nbAsked;
      }

  CDynamicCoverageAll it(this);

  // Comptage et marquage des bords:
  for (; it.cont(); ++it)
    for (i=0; i<nbAsked; ++i)
      if (isFree(*it, dim[i]) && !isMarked(*it, borderMark[i]))
	{
	  ++ * count[i];
	  markOrbit(*it, ORBIT_BORDER[dim[i]], borderMark[i]);
	}

  // Démarquage:
  for (it.reinit(); it.cont(); ++it)
    for (i=0; i<nbAsked; ++i)
      if (isMarked(*it, borderMark[i]))
	unmarkOrbit(*it, ORBIT_BORDER[dim[i]], borderMark[i]);

  // Libérations:
  for (i=0; i<nbAsked; ++i)
    freeMark(borderMark[i]);
}
//******************************************************************************
void CGMapGeneric::countBorders(CDart * ADart, TOrbit AOrbit,
				int * ANb0, int * ANb1, int * ANb2, int * ANb3)
{
  assert(ADart != NULL);

  int * count[4] = { ANb0, ANb1, ANb2, ANb3 };
  int dim[4];
  int borderMark[4];
  int nbAsked = 0;
  int i;
  
  // Initialisations:
  for (i=0; i<4; ++i)
    if (count[i] != NULL)
      {
	count[nbAsked] = count[i];
	* count[nbAsked] = 0;
	dim[nbAsked] = i;
	borderMark[nbAsked] = getNewMark();
	++nbAsked;
      }

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  // Comptage et marquage des bords:
  for (; cov->cont(); ++(*cov))
    for (i=0; i<nbAsked; ++i)
      if (isFree(**cov, dim[i]) && !isMarked(**cov, borderMark[i]))
	{
	  ++ * count[i];
	  markOrbit(**cov, ORBIT_BORDER[dim[i]], borderMark[i]);
	}

  // Démarquage:
  for (cov->reinit(); cov->cont(); ++(*cov))
    for (i=0; i<nbAsked; ++i)
      if (isMarked(**cov, borderMark[i]))
	unmarkOrbit(**cov, ORBIT_BORDER[dim[i]], borderMark[i]);

  // Libérations:
  delete cov;

  for (i=0; i<nbAsked; ++i)
    freeMark(borderMark[i]);
}
//******************************************************************************
void CGMapGeneric::countCells(int /*AMarkNumber*/,
			      int * ANb0, int * ANb1, int * ANb2,
			      int * ANb3, int * ANb4, int * ANbDarts)
{
  int * count[5] = { ANb0, ANb1, ANb2, ANb3, ANb4 };
  int dim[5];
  int cellMark[5];
  int nbAsked = 0;
  int nbDarts = 0;
  int i;
  
  // Initialisations:
  for (i=0; i<5; ++i)
    if (count[i] != NULL)
      {
	count[nbAsked] = count[i];
	* count[nbAsked] = 0;
	dim[nbAsked] = i;
	cellMark[nbAsked] = getNewMark();
	++nbAsked;
      }

  // Comptage et marquage des cellules:
  CDynamicCoverageAll it(this);
  for (; it.cont(); ++it, ++nbDarts)
    for (i=0; i<nbAsked; ++i)
      if (!isMarked(*it, cellMark[i]))
	{
	  ++ * count[i];
	  markOrbit(*it, ORBIT_CELL[dim[i]], cellMark[i]);
	}

  if (ANbDarts != NULL)
    * ANbDarts = nbDarts;

  // Démarquage:
  for (it.reinit(); it.cont(); ++it)
    for (i=0; i<nbAsked; ++i)
      if (isMarked(*it, cellMark[i]))
	unmarkOrbit(*it, ORBIT_CELL[dim[i]], cellMark[i]);

  // Libérations:
  for (i=0; i<nbAsked; ++i)
    freeMark(cellMark[i]);
}
//******************************************************************************
void CGMapGeneric::countCells(CDart * ADart, TOrbit AOrbit,
			      int * ANb0, int * ANb1, int * ANb2,
			      int * ANb3, int * ANb4, int * ANbDarts)
{
  assert(ADart != NULL);

  int * count[5] = { ANb0, ANb1, ANb2, ANb3, ANb4 };
  int dim[5];
  int cellMark[5];
  int nbAsked = 0;
  int nbDarts = 0;
  int i;

  // Initialisations:
  for (i=0; i<5; ++i)
    if (count[i] != NULL)
      {
	count[nbAsked] = count[i];
	* count[nbAsked] = 0;
	dim[nbAsked] = i;
	cellMark[nbAsked] = getNewMark();
	++nbAsked;
      }

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  // Comptage et marquage des cellules:
  for (; cov->cont(); ++(*cov), ++nbDarts)
    for (i=0; i<nbAsked; ++i)
      if (!isMarked(**cov, cellMark[i]))
	{
	  ++ * count[i];
	  markOrbit(**cov, ORBIT_CELL[dim[i]], cellMark[i]);
	}

  if (ANbDarts != NULL)
    * ANbDarts = nbDarts;

  // Démarquage:
  for (cov->reinit(); cov->cont(); ++(*cov))
    for (i=0; i<nbAsked; ++i)
      if (isMarked(**cov, cellMark[i]))
	unmarkOrbit(**cov, ORBIT_CELL[dim[i]], cellMark[i]);

  // Libérations:
  delete cov;

  for (i=0; i<nbAsked; ++i)
    freeMark(cellMark[i]);
}
//******************************************************************************
void CGMapGeneric::getGlobalCharacteristics(int * ANbDarts,
					    int * ANbVertices, int * ANbEdges,
					    int * ANbFaces, int * ANbVolumes,
					    int * ANbCC,
					    int * ANb0Borders,
					    int * ANb1Borders,
					    int * ANb2Borders,
					    int * ANb3Borders)
{
  int mark = getNewMark();

  negateMaskMark(mark);

  countCells  (mark, ANbVertices, ANbEdges, ANbFaces, ANbVolumes, ANbCC,
	       ANbDarts);

  countBorders(mark, ANb0Borders, ANb1Borders, ANb2Borders, ANb3Borders);

  negateMaskMark(mark);
  freeMark(mark);
}
//******************************************************************************
void CGMapGeneric::getSurfaceCharacteristics(CDart * ADart,
					     int * ANbDarts,
					     int * ANbVertices,
					     int * ANbEdges,
					     int * ANbFaces,
					     int * ANb0Borders,
					     int * ANb1Borders,
					     int * ANb2Borders,
					     int * ANb2BordersWhenClosed,
					     int * AEuler,
					     int * AOrient,
					     int * AGenus)
{
  assert(ADart != NULL);

  int nd   ; // nbDarts
  int nc[3]; // nbCells
  int nb[3]; // nbBorders

  countCells  (ADart, ORBIT_VOLUME, & nc[0], & nc[1], & nc[2], NULL, NULL, & nd);
  countBorders(ADart, ORBIT_VOLUME, & nb[0], & nb[1], & nb[2], NULL);

  if (ANbDarts    != NULL) * ANbDarts    = nd;
  if (ANbVertices != NULL) * ANbVertices = nc[0];
  if (ANbEdges    != NULL) * ANbEdges    = nc[1];
  if (ANbFaces    != NULL) * ANbFaces    = nc[2];
  if (ANb0Borders != NULL) * ANb0Borders = nb[0];
  if (ANb1Borders != NULL) * ANb1Borders = nb[1];
  if (ANb2Borders != NULL) * ANb2Borders = nb[2];

  bool needGenus  = AGenus  != NULL;
  bool needOrient = AOrient != NULL || needGenus;
  bool needEuler  = AEuler  != NULL || needOrient;

  int euler = 0, orient = 0, genus = 0;

  if (needEuler)
    {
      // Réservation des ressources:
      int memoAlpha3 = getNewDirectInfo();
      int newDart = getNewMark();

      CDynamicCoverageVolume vol(this, ADart);

      // Isolation du volume:
      for (; vol.cont(); ++vol)
	{
	  setDirectInfo(*vol, memoAlpha3, alpha3(*vol));
	  (*vol)->setFree3();
	}

      // Fermetures:
      negateMaskMark(newDart);

      for (vol.reinit(); vol.cont(); ++vol)
	if (isFree0(*vol))
	  CGMapGeneric::stopUp(*vol, 0);

      for (vol.reinit(); vol.cont(); ++vol)
	if (isFree1(*vol))
          CGMapGeneric::stopUp(*vol, 1);

      negateMaskMark(newDart);

      // Recalcul du nombre de cellules et du nombre de bords:
      countCells  (ADart, ORBIT_VOLUME,
		   & nc[0], & nc[1], &nc[2], NULL, NULL, NULL);
      countBorders(ADart, ORBIT_VOLUME, & nb[0], & nb[1], & nb[2], NULL);

      if (ANbVertices != NULL) * ANbVertices = nc[0];
      if (ANbEdges    != NULL) * ANbEdges    = nc[1];
      if (ANbFaces    != NULL) * ANbFaces    = nc[2];
      if (ANb0Borders != NULL) * ANb0Borders = nb[0];
      if (ANb1Borders != NULL) * ANb1Borders = nb[1];
      if (ANb2Borders != NULL) * ANb2Borders = nb[2];
      
      if (ANb2BordersWhenClosed != NULL)
	* ANb2BordersWhenClosed = nb[2];

      // Caractéristique d'Euler:
      euler = nc[0] - nc[1] + nc[2];

      // Suppression des brins créés lors de la fermeture:
      for (vol.reinit(); vol.cont(); )
	{
	  CDart * current = vol++;

	  if (isMarked(current, newDart))
	    {
	      alpha0(current)->setFree0();
	      alpha1(current)->setFree1();
	      alpha2(current)->setFree2();

	      delMapDart(current);
	    }
	}

      // Restauration des 3-coutures:
      for (vol.reinit(); vol.cont(); ++vol)
	(*vol)->setAlpha3(getDirectInfoAsDart(*vol, memoAlpha3));

      // Libération des ressources:
      freeDirectInfo(memoAlpha3);
      freeMark(newDart);

      // Écriture du résultat:
      if (AEuler != NULL)
	* AEuler = euler;
    }

  if (needOrient)
    {
      orient =
	isOrientable(ADart, ORBIT_VOLUME)
	? 0
	: 2 - abs((euler + nb[2]) % 2);

      if (AOrient != NULL)
	* AOrient = orient;
    }

  if (needGenus)
    {
      assert((euler + nb[2] + orient) % 2 == 0);

      genus = 1 - (euler + nb[2] + orient) / 2;

      if (AGenus != NULL)
	* AGenus = genus;
    }
}
//******************************************************************************
std::ostream& CGMapGeneric::displayCharacteristics(std::ostream &AOs)
{
  int nbd, nbv, nbe, nbf, nbvol, nbcc;
  getGlobalCharacteristics(&nbd, &nbv, &nbe, &nbf, &nbvol, &nbcc,
			   NULL, NULL, NULL, NULL);
  AOs<<"Nb of darts= "<<nbd<<"; vertices= "<<nbv<<"; edges="<<nbe
     <<"; faces="<<nbf<<"; volumes="<<nbvol<<"; cc="<<nbcc;
  return AOs;
}
//******************************************************************************
string CGMapGeneric::getSurfaceName(int AB2, int AQ, int AG)
{
  assert(0 <= AB2);
  assert(0 <= AQ && AQ <= 2);
  assert(0 <= AG);

  ostringstream result;

  result << "S(" << AB2 << "," << AQ << "," << AG << ")" << ": ";

  switch (AQ)
    {
    case 0:
      // Surface orientable:
      {
	if (AG == 0)
	  // Sans trou:
	  {
	    switch (AB2)
	      {
	      case 0 : result << "Sphere"; break;
	      case 1 : result << "Disk"; break;
	      case 2 : result << "Strip" ; break;
	      default: result << "Sphere with " << AB2 << " borders";
	      }
	  }
	else
	  // Avec trou(s):
	  {
	    result << "Torus";

	    if (AG > 1)
	      result << " with " << AG << " tunnels";

	    if (AB2 > 0)
	      result << (AG > 1 ? " and" : " with")
		     << " " << AB2 << " border" << (AB2 > 1 ? "s" : "");
	  }
      }
      break;
    case 1:
      // Surface non orientable type "ruban de Möbius":
      {
	if (AB2 == 0)
	  result << "Projective plane";
	else
	  result << "Möbius strip";

	if (AG > 0)
	  result << " with " << AG << " tunnel" << (AG > 1 ? "s" : "");

	if (AB2 > 1)
	  result << (AG > 0 ? " and " : " with ") << AB2 << " borders";
      }
      break;
    case 2:
      // Surface non orientable type "bouteille de Klein":
      {
	result << "Klein bottle";

	if (AG > 0)
	  result << " with " << AG << " tunnel" << (AG > 1 ? "s" : "");

	if (AB2 > 0)
	  result << (AG > 0 ? " and " : " with ")
		 << AB2 << " border" << (AB2 > 1 ? "s" : "");
      }
      break;
    default:
      assert(false);
    }

  return result.str();
}
//******************************************************************************
bool CGMapGeneric::isConnex()
{
  bool connex = true;
  int reached = getNewMark();

  // Marquage d'une composante connexe:
  markOrbit(getFirstDart(), ORBIT_CC, reached);

  // Parcours des brins de la G-carte:
  for (CDynamicCoverageAll it(this); it.cont() && connex; ++it)
    if (! isMarked(*it, reached))
      connex = false;

  if (connex)
    negateMaskMark(reached);
  else
    unmarkAll(reached);

  freeMark(reached);

  return connex;
}
//******************************************************************************
bool CGMapGeneric::checkTopology()
{
  // 1: On vérifie que pour tout brin B, on alpha B == alpha(alpha(B,i),i)
  for (int dim=0; dim<=3; ++dim)
    for (CDynamicCoverageAll it(this); it.cont(); ++it)
      if (!isFree(*it, dim) && *it != alpha(alpha(*it,dim),dim))
	{
	  cerr << "CGMapGeneric::integrity: The dart " << *it
	       << " does not satisfy the constraint "
	       << "alpha" << dim << "(alpha" << dim << "(brin)) == brin." << endl;

	  return false;
	}

  // 2: On vérifie que les involutions sont respectées:
  bool ok = true;

  TOrbit involutions[3] = { ORBIT_02, ORBIT_03, ORBIT_13 };
  int    alphaI     [3] = {       0 ,       0 ,       1  };
  int    alphaJ     [3] = {        2,        3,        3 };

  int treated = getNewMark();

  for (int invoIndex=0; invoIndex<3 && ok; ++invoIndex)
    {
      TOrbit orbit = involutions[invoIndex];
      int ai = alphaI[invoIndex];
      int aj = alphaJ[invoIndex];

      for (CDynamicCoverageAll it(this); it.cont() && ok; ++it)
	if (!isMarked(*it, treated))
	  {
	    if (isFree(*it, ai) || isFree(*it, aj))
	      setMark(*it, treated);
	    else
	      {
		ok =
		  alpha( alpha(*it, ai) , aj) == alpha( alpha(*it, aj) , ai);

		if (!ok)
		  {
		    cerr << "CGMapGeneric::checkTopology: The involution "
			 << (invoIndex==0 ? "02" : invoIndex==1 ? "03" : "13")
			 << " is not satisfied for dart " << *it
			 << "." << endl;
		  }

		markOrbit(*it, orbit, treated);
	      }
	  }

      if (ok)
	negateMaskMark(treated);
      else
	unmarkAll(treated);
    }

  freeMark(treated);

  return ok;
}
//******************************************************************************
bool CGMapGeneric::checkEmbeddings(TOrbit AOrbit, int AAttributeIdentity,
				   bool AEmbeddingMustExist)
{
  assert(isOrbitUsed(AOrbit));

  bool ok = true;

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	int i = 0;
	
	CCoverage * cov = getDynamicCoverage(*it, AOrbit);
	
	for (; cov->cont(); ++(*cov))
	  {
	    if ((**cov)->getAttribute(AOrbit, AAttributeIdentity) != NULL)
	      ++i;
	
	    setMark(**cov, treated);
	  }
	
	delete cov;

	if (i>1 || (i==0 && AEmbeddingMustExist))
	  {
	    static const char * ORBIT_NAMES[16] =
	    {
	      "dart","0","1","01","2","02","12","volume","3","03","13","face",
	      "23","edge","vertex","connected component"
	    };

	    cerr << "CGMapGeneric::checkEmbeddings: The cell '"
		 << ORBIT_NAMES[AOrbit] << "' incident to dart " << *it << " ";
	
	    if (i==0)
	      cerr << "has no embedding!" << endl;
	    else
	      cerr << " has " << i << " embeddings!" << endl;
	
	    ok = false;
	  }
      }

  negateMaskMark(treated);
  freeMark(treated);

  return ok;
}
//******************************************************************************
bool CGMapGeneric::isIClosedOrbit(CDart * ADart, int ADimension, TOrbit AOrbit)
{
  assert(ADart!=NULL);
  assert(0<=ADimension && ADimension<=3);

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  bool closed = true;

  for (; closed && cov->cont(); (*cov)++)
    closed = ! isFree(**cov, ADimension);

  delete cov;

  return closed;
}
//------------------------------------------------------------------------------
bool CGMapGeneric::isIFreeOrbit(CDart * ADart, int ADimension, TOrbit AOrbit)
{
  assert(ADart!=NULL);
  assert(0<=ADimension && ADimension<=3);

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  bool free = true;

  for (; free && cov->cont(); (*cov)++)
    free = isFree(**cov, ADimension);

  delete cov;

  return free;
}
//******************************************************************************
int CGMapGeneric::getMapDimension()
{
   int res = -1;
   int i = 0;
   for (CDynamicCoverageAll it(this); it.cont(); ++it)
   {
      for (i = res + 1; i < 4; ++i)
         if ( !isFree(*it, i) )
         {
            res = i;
            if (res == 3) return res;
         }
   }

   return res;
}
//******************************************************************************
bool CGMapGeneric::isDanglingFace(CDart* ADart)
{
  CDart* precDangling = ADart;
    
  // Go to dart preceding the dangling part
  while( alpha2(precDangling)==alpha3(precDangling) &&
         alpha10(precDangling)!=ADart )
  {
    // Work only for faces without boundary.
    assert( !isFree1(precDangling) && !isFree0(alpha1(precDangling)) );
    assert( !isFree2(precDangling) && !isFree3(precDangling) );
    precDangling=alpha10(precDangling);
  }
  
  if ( alpha2(precDangling)==alpha3(precDangling) )
    return false; // Here the face is isolated  
  
  // Here we are not in a dangling part. Now search the beginning of a
  // dangling part.
  CDart* current = precDangling;  
  do
  {
    // Work only for faces without boundary.
    assert( !isFree0(current) && !isFree1(alpha0(current)) );
    assert( !isFree2(current) && !isFree3(current) );
    current=alpha01(current);
  }
  while( current!=precDangling && alpha2(current)!=alpha3(current) );

  if ( current==precDangling )
    return false; // Here the face does not have a dangling part.

  // Here current is the first dart of a dangling part, and we know there is
  // at least one non dangling part.
  do
  {
    assert( !isFree0(current) && !isFree1(alpha0(current)) );
    assert( !isFree2(current) && !isFree3(current) );
    current=alpha01(current);
  }
  while( alpha2(current)==alpha3(current) );

  // Here current is the first dart of a non dangling part. This part must
  // be connected till the end of the face.
  do
  {
    current=alpha01(current);
    //    if ( alpha2(current)==alpha3(current) ) return false;
    
    CDart* d1 = current;
    do
    {
      if ( isFree2(d1) || isFree1(alpha2(d1)) ) return false;
      if ( alpha2(d1)==alpha3(d1) ) return false;
      d1 = alpha21(d1);
    }
    while ( d1!=current);
  }
  while( current!=precDangling);
  return true;
}
//******************************************************************************
