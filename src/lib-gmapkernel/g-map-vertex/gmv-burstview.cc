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
void CGMapVertex::setBurstMethod(TBurstMethod AMethod)
{
  assert( FIRST_BURST_METHOD<=AMethod && AMethod<LAST_BURST_METHOD );
  FBurstMethod = AMethod;
}
//------------------------------------------------------------------------------
TBurstMethod CGMapVertex::getBurstMethod() const
{ return FBurstMethod; }
//******************************************************************************
void CGMapVertex::updateAllBurstDarts()
{
  int selected = getNewMark();
  negateMaskMark(selected);
  updateMarkedBurstDarts(selected, false);
  negateMaskMark(selected);
  freeMark(selected);
}
//******************************************************************************
void CGMapVertex::updateMarkedBurstDarts(int AMarkNumber,
                                         bool AUpdateIncidentVolumes)
{
  switch (FBurstMethod)
    {
    case BURST_METHOD_SCALE:
      updateMarkedDartsMethodScale(AMarkNumber, AUpdateIncidentVolumes);
      break;
    case BURST_METHOD_SHIFT:
      updateMarkedDartsMethodShift(AMarkNumber, AUpdateIncidentVolumes);
      break;
    default: assert(false);
    }
}
//******************************************************************************
#define GET_VERTEX(DART,INDEX) (getDirectInfoAsAttributeVertex(DART, INDEX))
//******************************************************************************
void CGMapVertex::updateMarkedDartsMethodScale(int AMarkNumber,
					       bool AUpdateIncidentVolumes)
{
  if (AUpdateIncidentVolumes)
    {
      int selected = getNewMark();
      markIncidentCells(ORBIT_VERTEX, AMarkNumber, selected);
      markIncidentCells(ORBIT_VOLUME, selected);
      updateMarkedBurstDarts(selected, false);
      unmarkAll(selected);
      freeMark(selected);
      return;
    }

  TCoordinate k1 = getBurstCoef(1);
  TCoordinate k2 = getBurstCoef(2);
  TCoordinate k3 = getBurstCoef(3);

  int directIndex = getNewDirectInfo();

  int markV = getNewMark(); // volume mark

  int mark3 = getNewMark(); // volume mark
  int mark2 = getNewMark(); // face mark
  int mark1 = getNewMark(); // edge mark

  pointDirectInfoToAttributeVertex(directIndex);

  // CARTE ENTIÈRE:
  for (CDynamicCoverageAll all(this); all.cont(); ++all)
    if (!isMarked(*all, mark3))
      {
	if (!isMarked(*all, AMarkNumber))
	  {
	    setMark(*all, mark3);
	    setMark(*all, mark2);
	    setMark(*all, mark1);
	  }
	else
	  {
	    // VOLUME:
	    CVertex bary3 =
	      basicBarycenter(*all, ORBIT_012, markV, mark3, directIndex);

	    negateMaskMark(markV);
	
	    for (CBasicDynamicCoverage012 vol(this, *all, markV);
		 vol.cont(); ++vol)
	      if (!isMarked(*vol, mark2))
		{
		  if (!isMarked(*vol, AMarkNumber))
		    {
		      setMark(*vol, mark2);
		      setMark(*vol, mark1);
		    }
		  else
		    {
		      // FACE:
		      CVertex bary2 =
			barycenter(*vol, ORBIT_01, directIndex);
		
		      markOrbit(*vol, ORBIT_01, mark2);
		
		      CVertex delta2 = bary2 - bary3;
		
		      for (CDynamicCoverage01 fac(this, *vol);
			   fac.cont(); ++fac)
			if (!isMarked(*fac, mark1))
			  {
			    if (!isMarked(*fac, AMarkNumber))
			      setMark(*fac, mark1);
			    else
			      {
				// ARÊTE:
				CVertex bary1 =
				  barycenter(*fac, ORBIT_0, directIndex);
				
				markOrbit(*fac, ORBIT_0, mark1);
				
				CVertex delta1 = bary1 - bary2;
				
				CVertex & b1 = getBurstVertex(   *fac );
				CVertex & b2 = getBurstVertex(alpha0(*fac));

				CAttributeVertex & v1 =
				  * GET_VERTEX(   *fac , directIndex);

				CAttributeVertex & v2 =
				  * GET_VERTEX(alpha0(*fac), directIndex);
			
				/* Les lignes suivantes sont équivalentes à:
				 *
				 * b1 = bary3 +
				 *      k3*(delta2 +
				 *          k2*(delta1 +
				 *              k1*(v1-bary1)));
				 *
				 * b2 = bary3 +
				 *      k3*(delta2 +
				 *          k2*(delta1 +
				 *              k1*(v2-bary1)));
				 *
				 * Pour minimiser le nombre d'allocations et de
				 * désallocations, elles sont remplacées par:
				 */
				
				b1.setX(bary3.getX() +
					k3*(delta2.getX() +
					    k2*(delta1.getX() +
						k1*(v1.getX()-bary1.getX()))));
				
				b1.setY(bary3.getY() +
					k3*(delta2.getY() +
					    k2*(delta1.getY() +
						k1*(v1.getY()-bary1.getY()))));
				
				b1.setZ(bary3.getZ() +
					k3*(delta2.getZ() +
					    k2*(delta1.getZ() +
						k1*(v1.getZ()-bary1.getZ()))));
				
				b2.setX(bary3.getX() +
					k3*(delta2.getX() +
					    k2*(delta1.getX() +
						k1*(v2.getX()-bary1.getX()))));
				
				b2.setY(bary3.getY() +
					k3*(delta2.getY() +
					    k2*(delta1.getY() +
						k1*(v2.getY()-bary1.getY()))));
				
				b2.setZ(bary3.getZ() +
					k3*(delta2.getZ() +
					    k2*(delta1.getZ() +
						k1*(v2.getZ()-bary1.getZ()))));
			      }
			  }
		    }
		}

	    negateMaskMark(markV);
	  }
      }

  freeMark(markV);
  negateMaskMark(mark3); freeMark(mark3);
  negateMaskMark(mark2); freeMark(mark2);
  negateMaskMark(mark1); freeMark(mark1);

  freeDirectInfo(directIndex);
}
//******************************************************************************
void CGMapVertex::updateMarkedDartsMethodShift(int AMarkNumber,
					       bool AUpdateIncidentVolumes)
{
  /// Il faut debugger cette méthode et la suivante
  if (AUpdateIncidentVolumes)
    {
      int selected = getNewMark();
      markIncidentCells(ORBIT_VERTEX, AMarkNumber, selected);
      markIncidentCells(ORBIT_VOLUME, selected);
      updateMarkedBurstDarts(selected, false);
      unmarkAll(selected);
      freeMark(selected);
      return;
    }

  int directIndex = getNewDirectInfo();

  int markV = getNewMark(); // volume mark

  int mark3 = getNewMark(); // volume mark
  int mark2 = getNewMark(); // face mark
  int marki = getNewMark(); // mark intersection
  
  pointDirectInfoToAttributeVertex(directIndex);

  // CARTE ENTIÈRE:
  for (CDynamicCoverageAll all(this); all.cont(); ++all)
    if (!isMarked(*all, mark3))
      {
        if (!isMarked(*all, AMarkNumber))
          {
            setMark(*all, mark3);
            setMark(*all, mark2);
          }
        else
          {
            // VOLUME:
            CVertex bary3 =
              basicBarycenter(*all, ORBIT_012, markV, mark3, directIndex);

            negateMaskMark(markV);

            for (CBasicDynamicCoverage012 vol(this, *all, markV);
                 vol.cont(); ++vol)
              if (!isMarked(*vol, mark2))
                {
                  if (!isMarked(*vol, AMarkNumber))
                    setMark(*vol, mark2);
                  else
                    // Traitement d'une face :
                    updateFaceMethodShift(mark2,marki,*vol, bary3, directIndex);
                }

            negateMaskMark(markV);
          }
      }

  freeMark(markV);
  negateMaskMark(mark3); freeMark(mark3);
  negateMaskMark(mark2); freeMark(mark2);
  freeMark(marki);
  freeDirectInfo(directIndex);
}
//------------------------------------------------------------------------------
// A DEBUGGER !!!!!!!!!!!!!
void CGMapVertex::updateFaceMethodShift(int AMarkNumber, int AMarkI,
					CDart* ADart, const CVertex& ABaryVol,
					int ADirectIndex)
{
  TCoordinate k1 = 1 - getBurstCoef(1);
  TCoordinate k2 = 1 - getBurstCoef(2);
  TCoordinate k3 = 1 - getBurstCoef(3);  
  
  halfMarkOrbit(ADart, ORBIT_01, AMarkNumber);
  
  CVertex normalf = - faceNormalVector(ADart);

  CDynamicCoverage01 fac(this, ADart);
  
  for ( ; fac.cont(); ++fac)
    if ( !isMarked(*fac, AMarkNumber) || isFree1(*fac) )
      {
        // Point associé au sommet :
        CAttributeVertex& v = * GET_VERTEX(*fac, ADirectIndex);
	
	// Recherche de la première extrémité en sautant les arêtes de longueur nulle :
	CDart* dartExt1	     = alpha0(*fac);
        CAttributeVertex& v1 = * GET_VERTEX(dartExt1, ADirectIndex);
	if ( !isFree0(*fac) )
	  {
	    while( v1==v && dartExt1!=alpha1(*fac) &&
		   !isFree1(dartExt1) && !isFree0(alpha1(dartExt1)) )
	      {
		//		cout<<"Null edge case 1"<<endl;
		dartExt1 = alpha10(dartExt1);
		v1       = * GET_VERTEX(dartExt1, ADirectIndex);
	      }
	  }

	// Recherche de la deuxième extrémité en sautant les arêtes de longueur nulle :
	CDart* dartExt2	     = isFree1(*fac) ? *fac : alpha10(*fac);
        CAttributeVertex& v2 = * GET_VERTEX(dartExt2, ADirectIndex);
	while( v2==v && dartExt2!=*fac &&
	       !isFree1(dartExt2) && !isFree0(alpha1(dartExt2)) )
	  {
	    //	    cout<<"Null edge case 2"<<endl;
	    dartExt2 = alpha10(dartExt2);
	    v2       = * GET_VERTEX(dartExt2, ADirectIndex);
	  }
	
	// Calcul "normal" :

        CVertex vecteur1 = v - v1;
        CVertex vecteur2 = v2 - v;
	
        CVertex n1 = vecteur1 * normalf; n1 *= k2 / n1.norm();
        CVertex n2 = vecteur2 * normalf; n2 *= k2 / n2.norm();

        // Intersection entre les 2 droites :
	CVertex intersection;
        if ( (v==v1 && v==v2) || n1==-n2 ) // 2eme cas pour les arêtes parallèles et opposés
          intersection = v;
        else
          if ( v==v1 || n1==n2 ) // 2eme cas pour les arêtes parallèles dans le même sens
            intersection = v + n2;
          else
            if ( v==v2 )
              {
                if ( isMarked(*fac, AMarkNumber) )
                  intersection = (v -  (vecteur1.normalized() * k2)) - n1;
                else
                  intersection = (v - (vecteur1.normalized() * k2)) + n1;
              }
            else
              intersection = CGeometry::getLinesIntersection(v + n1, v1 + n1 ,
							     v + n2, v2 + n2 );

	// Affectation des coordonnées éclatées.
	dartExt1 = alpha0(dartExt1);
	while ( dartExt1!=dartExt2 )
	  {
	    setBurstVertex( dartExt1, intersection );
	    setMark       ( dartExt1, AMarkNumber  );
	    
	    if ( !isFree1(dartExt1) )
	      {
		setBurstVertex( alpha1(dartExt1), intersection );
		setMark       ( alpha1(dartExt1), AMarkNumber  );
		dartExt1 = alpha10(dartExt1);
	      }
	    else dartExt1 = dartExt2;
	  }
      }

  return;
  
  unmarkOrbit(ADart, ORBIT_01, AMarkNumber);
  
  // Deuxième passe pour l'éclatement sur les arêtes (k1) et les volumes (k3)
  for (fac.reinit(); fac.cont(); ++fac)
    if (! isMarked(*fac, AMarkNumber))
      {
		
	CVertex& b1 = getBurstVertex(       *fac );
	CVertex& b2 = getBurstVertex(alpha0(*fac));
      
	if ( b1!=b2 )
	  {
	    CVertex vedge = (b1 - b2).normalized();	    
	    // Point associé au sommet :
	    CVertex & v = * GET_VERTEX(*fac, ADirectIndex);
	    // Points associés aux sommets voisins :
	    CVertex & v1 = * GET_VERTEX(alpha0 (*fac), ADirectIndex);
	    
	    CVertex vecteur1 = (v - v1).normalized();
	    
	    
	    b1 -= vedge * k1;
	    b2 += vedge * k1;
	    	    	    	    	    
	    b1 += (ABaryVol - b1) * k3;
	    b2 += (ABaryVol - b2) * k3;
	    
	    //	    TCoordinate cosinus = vecteur1.dot(vedge);
	    
	    if ( ! vedge.isNull() && isNegative(vecteur1.dot(vedge)) )
	      {		
//  		setMark(*fac ,AMarkI);
//              setMark(alpha0(*fac),AMarkI);
	      }
	  }
	
	setMark(       *fac , AMarkNumber);
	setMark(alpha0(*fac), AMarkNumber);
      }

  return;
  
  // Traitement des croisements
  unmarkOrbit(ADart, ORBIT_01, AMarkNumber);

  for (fac.reinit(); fac.cont(); ++fac)
    if ((isMarked(*fac,AMarkI )) &&  ( ! isMarked(*fac, AMarkNumber ) ))
      {
	CVertex& b1 = getBurstVertex(       *fac );
	//Cas ou il y a des intersections consecuitive	
	CVertex& b2 = getBurstVertex(alpha0(*fac));	
	if (b1 != b2)
	  {
	    CVertex vedge = (b1 - b2).normalized();
	    
	    // Point associé au sommet :
	    CVertex & v = * GET_VERTEX(*fac, ADirectIndex);
	    // Points associés aux sommets voisins :
	    CVertex & v1 = * GET_VERTEX(alpha0 (*fac),ADirectIndex);
	    
	    CVertex vecteur1 = (v - v1).normalized();
	    
	    b1 -= vedge * k1;
	    b2 += vedge * k1;
	    
	    //    b1 += (ABaryVol - b1) * k3;
	    // b2 += (ABaryVol - b2) * k3;
	    
	    CVertex & b4 = getBurstVertex(alpha1 (*fac));
	    CVertex & b3 = getBurstVertex(alpha01(*fac));

	    b1 = b2 = b3 = b4 = b1 + (b2 - b1) / 2;
	    
	    setMark(*fac , AMarkNumber);
	    setMark(alpha0(*fac) , AMarkNumber); 

	    CDart * dartb1 = alpha1(alpha0(*fac));
	    
	    while ( isMarked(dartb1, AMarkI))
	      {
		
		CVertex & b5 = getBurstVertex(       dartb1 );
		CVertex & b6 = getBurstVertex(alpha0(dartb1));
		CVertex & b7 = getBurstVertex(alpha10(dartb1));
		CVertex & b8 = getBurstVertex(alpha01(dartb1));

		b5 = b6 = b7 = b8 = b1;
		
		dartb1 = alpha1(alpha0(dartb1));

		setMark(dartb1 , AMarkNumber);                      	      		 	      
		setMark(alpha0(dartb1) , AMarkNumber);
		}
	  }	
      }  
}

//******************************************************************************
#undef GET_VERTEX
//******************************************************************************
