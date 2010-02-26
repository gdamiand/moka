/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
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

#include <cmath>
#include <cstring>
#include "controler-gmap.hh"
#include "view-precompile.hh"
#include "g-map-vertex.hh"
using namespace std;
using namespace GMap3d;

#define RINT(V) ((double) ((int) (V)))
/*************************************************/

static TViewId FViewId;

bool drawArrows;
bool drawFaces;
bool drawSews;
bool drawGrid;
bool drawVertices;

/*************************************************/

static TCoordinate k0; // coefficients for draw the sew
static TCoordinate kw[4];
static TCoordinate decalageX, decalageY; // the decalage offset in 2D
static TCoordinate coefMultProf;
static TCoordinate profMin;
static bool invertY = true;

//******************************************************************************
// Windows/mapGlXfig.inc
int MAX_PROF         =999;  // profondeur max accepte par xfig

int COUL_DART        =0; //le numero de la couleur perso associe aux divers elements

int COUL_SEW[4]      ={1,2,3,4};

int COUL_DART_NO_SEW0=5;

int COUL_SEL [NB_SELECTION_LEVELS] = { 6 ,  7 ,  8 };
int COUL_LAST[NB_SELECTION_LEVELS] = { 9 , 10 , 11 };

int COUL_VERTEX       =12;
int COUL_GRID         =13;
int COUL_AXE_X        =14;
int COUL_AXE_Y        =15;
int COUL_AXE_Z        =16;
int COUL_FILL_FACE    =17;
int COUL_POINTEL      =18;
int COUL_LIGNEL       =19;
int COUL_DART_VERTEX  =20;

int LARG_DART         =1; //largeurs des lignes des divers elements
int LARG_SEL [NB_SELECTION_LEVELS] = {1,1,1};
int LARG_LAST[NB_SELECTION_LEVELS] = {2,2,2};
int LARG_SEW[4]       ={1,1,1,1};
int LARG_GRID         =1;
int LARG_AXE_X        =1;
int LARG_AXE_Y        =1;
int LARG_AXE_Z        =1;
int LARG_FILL_FACE    =1;
int RAYON_POINTEL     =5;
int LARG_LIGNEL       =3;
int RAYON_DART_VERTEX =3;
float COEF_REDUC_LIGNEL=0.75; // coefficient pour homotethie des lignels
int COEF_DE_REDUCTION=5;      // Car sinon le dessin xfig est beaucoup trop gros
float RAYON =100/COEF_DE_REDUCTION; // rayon d'un brin non cousu par alpha0
int NB_COUL_USER=21; // Doit être > à 15
typedef char coulXfig[8];
coulXfig * COUL_USER= NULL;

//******************************************************************************
void CControlerGMap::initConfig()
{
  assert(NB_COUL_USER>15);

  if ( COUL_USER!=NULL ) delete COUL_USER;
  
  COUL_USER=new coulXfig[NB_COUL_USER];
  for (int i=0;i<NB_COUL_USER;++i) strcpy(COUL_USER[i],"#000000");
  strcpy(COUL_USER[	          1],"#808080"); // 0-coutures
  strcpy(COUL_USER[	          2],"#FF0000"); // 1-coutures
  strcpy(COUL_USER[	          3],"#008000"); // 2-coutures
  strcpy(COUL_USER[	          4],"#0000FF"); // 3-coutures
  strcpy(COUL_USER[  COUL_FILL_FACE],"#DDDDDD"); // faces pleines
  strcpy(COUL_USER[COUL_DART_VERTEX],"#A0A0A0"); // points associés aux brins
}

//******************************************************************************
void CControlerGMap::transfoMmPixel( CVertex & v2d )
{
  // Xfig est en 1200 dpi, et donc le nombre de pixel par mm vaut 1200/25.4
  // (sachant que 1inch==25.4mm)
  float dpmX=(1200/25.4);
  float dpmY=(1200/25.4);
  v2d.setX(static_cast<TCoordinate>((v2d.getX()*dpmX)/COEF_DE_REDUCTION));
  v2d.setY(static_cast<TCoordinate>((v2d.getY()*dpmY)/COEF_DE_REDUCTION));
}

//******************************************************************************
void CControlerGMap::sauveEntete ( ofstream &os, int nbCoul )
{
  os<<"#FIG 3.2"<<endl
    <<"Portrait"<<endl
    <<"Metric"<<endl
    <<"A4"<<endl
    <<"100.00"<<endl
    <<"Single"<<endl
    <<"-2"<<endl
    <<"1200 2"<<endl;
  // Ici, on definit des couleurs personelles. Cela permet ensuite de
  // changer une valeur et cela entraine la modif de tous les brins
  // ayant cette couleur.
  
  initConfig();
  
  if (COUL_USER==NULL)
    for (int i=0;i<nbCoul;++i) os<<"0 "<<32+i<<" #000000"<<endl;
  else
    for (int i=0;i<nbCoul;++i) os<<"0 "<<32+i<<" "<<COUL_USER[i]<<endl;
}
//******************************************************************************
void CControlerGMap::debutComposante( ofstream &os,
				  const CVertex & min, const CVertex & max )
{
  os<<"6 "
    <<int(RINT(min.getX()+decalageX))<<" "
    <<int(RINT(min.getY()+decalageY))<<" "
    <<int(RINT(max.getX()+decalageX))<<" "
    <<int(RINT(max.getY()+decalageY))<<endl;
}

//******************************************************************************
int CControlerGMap::computeProf(const CVertex & V)
{
  int p = int(RINT(coefMultProf*(V.getZ()-profMin)));

  if (p<0)
    p=0;

  if (p>MAX_PROF)
    p=MAX_PROF;

  return p;
}

int CControlerGMap::computeProf(const CVertex & V1, const CVertex & V2)
{
  TCoordinate c = (V1.getZ() + V2.getZ())/2;
  
  int p = int(RINT(coefMultProf*(c-profMin)));
  
  if (p<0)
    p=0;

  if (p>MAX_PROF)
    p=MAX_PROF;

  return p;
}
//******************************************************************************
void CControlerGMap::sauvePoint(ofstream &os, const CVertex & v,
				int coul, int larg, int AProf)
{
  os<<"1 4 0 "<<larg<<" "<<32+coul<<" "<<32+coul<<" ";
  if ( AProf==-1 ) os<<computeProf(v); else os << AProf;
  os <<" 0 20 0.000 1 0.0000 ";

  os<<int(RINT(v.getX()+decalageX))<<" "  // Centre x et y
    <<int(RINT(v.getY()+decalageY))<<" "
    <<RAYON<<" "<<RAYON<<" " //Rayon en x et en y
    <<int(RINT(v.getX()+decalageX))<<" "  // Premier point == centre
    <<int(RINT(v.getY()+decalageY))<<" "
    <<int(RINT(v.getX()+decalageX+RAYON))<<" " // deuxieme point (sur le cercle)
    <<int(RINT(v.getY()+decalageY+RAYON))<<endl;
}
//******************************************************************************
void CControlerGMap::sauveDartVertex(std::ofstream &os, const CVertex & v,
				     const CVertex & /*v2*/)
{
  int prof = MAX_PROF;
  
  os<<"1 3 0 "<<RAYON_DART_VERTEX<<" "<<32+COUL_DART_VERTEX
    <<" "<<32+COUL_DART_VERTEX<<" "<<prof<<" 0 20 0.000 1 0.0000 ";

  os<<int(RINT(v.getX()+decalageX))<<" "  // Centre x et y
    <<int(RINT(v.getY()+decalageY))<<" "
    <<RAYON<<" "<<RAYON<<" " //Rayon en x et en y
    <<int(RINT(v.getX()+decalageX))<<" "  // Premier point == centre
    <<int(RINT(v.getY()+decalageY))<<" "
    <<int(RINT(v.getX()+decalageX+RAYON))<<" " // deuxieme point (sur le cercle)
    <<int(RINT(v.getY()+decalageY+RAYON))<<endl;
}
//******************************************************************************
void CControlerGMap::sauveLine(ofstream &os,
			       const CVertex & p1, const CVertex & p2,
			       int coul, int larg, bool arrow, int AProf)
{
  int prof;

  if (AProf<0) prof = computeProf(p1,p2);
  else         prof = AProf;

  os<<"2 1 0 "<<larg<<" "<<32+coul<<" 0 "<< prof <<" 0 -1 0.000 1 0 7 ";
  if (arrow)
    {
      os<<"1 0 2\n"
	<<"     2 0 1.00 60.00 120.00\n";
    }
  else
    os<<"0 0 2\n";

  os<<int(RINT(p1.getX()+decalageX))<<" "
    <<int(RINT(p1.getY()+decalageY))<<" "
    <<int(RINT(p2.getX()+decalageX))<<" "
    <<int(RINT(p2.getY()+decalageY))<<endl;
}

//******************************************************************************
void CControlerGMap::sauveTriangle(ofstream & os, int coulFill, int larg,
				   CVertex & PC, CVertex & P1, CVertex & P2)
{
  os << "2 1 0 " << larg << " -1 "
     << 32+coulFill << " "
     << (1+computeProf((PC+P1+P2)/3)) << " 0 20 0.000 1 0 7 0 0 " << 4 << endl;

  os << int(RINT(PC.getX()+decalageX)) << " "
     << int(RINT(PC.getY()+decalageY)) << endl;
  os << int(RINT(P1.getX()+decalageX)) << " "
     << int(RINT(P1.getY()+decalageY)) << endl;
  os << int(RINT(P2.getX()+decalageX)) << " "
     << int(RINT(P2.getY()+decalageY)) << endl;
  os << int(RINT(PC.getX()+decalageX)) << " "
     << int(RINT(PC.getY()+decalageY)) << endl;
}
//******************************************************************************
void CControlerGMap::sauveFace(ofstream &os, CVertex *p, int nbPts,
			       int coulFill, int larg, int AProf )
{
  assert(nbPts>=3); // 3 => polygone à 2 arêtes...

  bool compactFaces = isZero(FMap->getBurstCoef(1) - 1.0);

  os<<"2 1 0 " << (compactFaces&&!drawArrows?larg:0) << " "
    << 32+COUL_DART << " " << 32+coulFill << " "
    << (1+AProf) <<" 0 20 0.000 1 0 7 0 0 "
    << nbPts << endl;

  for (int i=0; i<nbPts; ++i)
    {      
      os << int(RINT(p[i].getX()+decalageX)) << " "
	 << int(RINT(p[i].getY()+decalageY)) << endl;
    }
  
  /* Ancienne version de Fred qui sauvait des triangles => pb de profondeur... */
  /*
    CVertex baryFace;
  int i;

  for (i=0; i<nbPts-1; ++i)
    baryFace += p[i];

  baryFace /= nbPts-1;

  bool compactFaces = isZero(FMap->getBurstCoef(1) - 1.0);

  for (i=0; i<nbPts-1; ++i)
    {
      if (compactFaces || (i%2==0))
	{
	  CVertex baryEdge = (p[i]+p[i+1])/2;

	  sauveTriangle(os, coulFill, larg, baryFace, p[i]    , baryEdge);
	  sauveTriangle(os, coulFill, larg, baryFace, baryEdge, p[i+1]  );
	}

      if (!compactFaces && (i%2==1))
	sauveTriangle(os, coulFill, larg, baryFace, p[i], p[i+1]);

       }
  */
}

//******************************************************************************
void CControlerGMap::transfo3dTo2d( const CVertex & v3d, CVertex & v2d )
{
  float res[3];

  FViews[FViewId]->project( v3d.getX(), v3d.getY(), v3d.getZ(), res);
			 
  v2d.setX(res[0]); v2d.setY(res[1]); v2d.setZ(res[2]);

  transfoMmPixel(v2d);

  if (invertY) v2d.setY(-v2d.getY());
}

//******************************************************************************
void CControlerGMap::updateMinMax2d( const CVertex & pts,
				     CVertex & min, CVertex & max, bool init )
{
  CVertex tmp;
  transfo3dTo2d(pts,tmp);

  if (init)
    { min=tmp; max=tmp; }
  else
    {
      if (tmp.getX()<min.getX()) min.setX(tmp.getX());
      if (tmp.getY()<min.getY()) min.setY(tmp.getY());
      if (tmp.getZ()<min.getZ()) min.setZ(tmp.getZ());
      if (tmp.getX()>max.getX()) max.setX(tmp.getX());
      if (tmp.getY()>max.getY()) max.setY(tmp.getY());
      if (tmp.getZ()>max.getZ()) max.setZ(tmp.getZ());
    }
}

//******************************************************************************
void CControlerGMap::getIncludingBoxForDarts( CDart* d, TOrbit orb,
					      CVertex & min, CVertex & max )
{
  CCoverage* cov = FMap->getDynamicCoverage(d,orb);

  if (cov->cont())
    {
      transfo3dTo2d(FMap->getBurstVertex(d),min);
      max=min;
      ++(*cov);
    }

  while(cov->cont())
    {
      updateMinMax2d(FMap->getBurstVertex(**cov),min,max);
      ++(*cov);
    }

  delete cov;
}

#define WEIGHTING(k, v1,v2) ( (1-(k)) * (v1) + (k) * (v2) )
//******************************************************************************
void CControlerGMap::treatSews( ofstream & fout, CDart * dart,
				int mark, int /*mark2*/ )
{
  CDart *darti;
  CVertex vTmp, v2Tmp, v3Tmp, p1, p2;

  for (int i=0; i<=3; ++i)
    if (!FMap->isMarked(FMap->alpha(dart,i),mark))
      {
        darti= FMap->alpha(dart,i);
        if (!FMap->isFree(dart,i) &&
	    getParameterSew(FViewId)->getDisplayAlpha(i))
          {
            if (i==0)
              {
                transfo3dTo2d(FMap->getBurstVertex(dart ), p1);
                transfo3dTo2d(FMap->getBurstVertex(darti), p2);

		vTmp = (p1+p2)/2;

		int prof1 = computeProf(p1, vTmp);
		int prof2 = computeProf(p2, vTmp);

		int prof = prof1<prof2 ? prof1 : prof2;

		if (prof<0) prof=0;
		if (prof>MAX_PROF) prof=MAX_PROF;

		vTmp = (FMap->getBurstVertex(dart) +
			FMap->getBurstVertex(darti))/2;

                v2Tmp=WEIGHTING(kw[0],vTmp, FMap->computeBurstExtremity(dart ));
                v3Tmp=WEIGHTING(kw[0],vTmp, FMap->computeBurstExtremity(darti));
                transfo3dTo2d(v2Tmp,p1);
                transfo3dTo2d(v3Tmp,p2);

                sauveLine(fout,p1,p2,COUL_SEW[i],LARG_SEW[i],false, prof);
              }
            else
              {
		if ( FMap->isFree0(dart) || kw[i]!=1 || dart<FMap->alpha0(dart) )
		  {
		    vTmp =WEIGHTING(kw[i],FMap->getBurstVertex(dart ),
				    FMap->computeBurstExtremity(dart ));
		    v2Tmp=WEIGHTING(kw[i],FMap->getBurstVertex(darti),
				    FMap->computeBurstExtremity(darti));
		    transfo3dTo2d(v2Tmp,p1);
		    transfo3dTo2d(vTmp,p2);
		    
		    sauveLine(fout,p1,p2,COUL_SEW[i],LARG_SEW[i],false);
		  }
              }
          }
      }
}

//******************************************************************************
void CControlerGMap::treatDartWithArrow( ofstream & fout, CDart * dart,
					 int /*mark*/, int mark2, int AProf )
{
  CVertex v1;
  CVertex v2,p1,p2;
  int coul = COUL_DART;
  int larg = LARG_DART;

  v1= FMap->getBurstVertex(dart);
  transfo3dTo2d(v1, p1);

  if (FMap->isFree0(dart))
    sauvePoint(fout,p1,COUL_DART_NO_SEW0,LARG_DART, AProf);
  else
    {
      CDart *dart0=FMap->alpha0(dart);

      bool found = false;
      int i;

      for (i=0; !found && i<getNbSelectionLevels(); ++i)
	if (dart==getLastSelectedDart(i) || dart0==getLastSelectedDart(i))
	  {
	    coul=COUL_LAST[i];
	    larg=LARG_LAST[i];
	    found = true;
	  }

      for (i=0; !found && i<getNbSelectionLevels(); ++i)
	if (isDartSelected(dart,i) || isDartSelected(dart0, i))
	  {
	    coul=COUL_SEL[i];
	    larg=LARG_SEL[i];
	    found = true;
	  }

      v1= FMap->getBurstVertex(dart0);
      transfo3dTo2d(v1, p2);

      if (FMap->isMarked(dart,mark2))
	{
	  sauveLine(fout,p1,p2,coul,larg,true, AProf);
 	  if ( drawVertices )
	    sauveDartVertex( fout, p1, p2 );
	}
      else
	{
	  sauveLine(fout,p2,p1,coul,larg,true, AProf);
 	  if ( drawVertices )
	    sauveDartVertex( fout, p2, p1 );
	}
    }
}

//******************************************************************************
void CControlerGMap::treatDartNoArrow( ofstream & fout, CDart * dart,
				       int /*mark*/, int /*mark2*/ )
{
  CVertex v1;
  CVertex v2,p1,p2;
  int coul = COUL_DART;
  int larg = LARG_DART;

  bool found = false;
  int i;

  for (i=0; !found && i<getNbSelectionLevels(); ++i)
    if (dart==getLastSelectedDart(i))
      {
	coul=COUL_LAST[i];
	larg=LARG_LAST[i];
	found = true;
      }

  for (i=0; !found && i<getNbSelectionLevels(); ++i)
    if (isDartSelected(dart,i))
      {
	coul=COUL_SEL[i];
	larg=LARG_SEL[i];
	found = true;
      }

  v1= FMap->getBurstVertex(dart);
  transfo3dTo2d(v1, p1);

  if (FMap->isFree0(dart))
    sauvePoint(fout,p1,COUL_DART_NO_SEW0,LARG_DART);
  else
    {
      CDart *dart0 = FMap->alpha0(dart);

      transfo3dTo2d(FMap->getBurstVertex(dart ), p1);
      transfo3dTo2d(FMap->getBurstVertex(dart0), p2);
      CVertex p0 = (p1+p2)/2;

      v2=FMap->computeBurstExtremity(dart);
      transfo3dTo2d(v2, p2);
      sauveLine(fout,p1,p2,coul,larg,false, computeProf(p1,p0));

      coul=COUL_DART; larg=LARG_DART;

      bool found = false;
      int i;

      for (i=0; !found && i<getNbSelectionLevels(); ++i)
	if (dart0==getLastSelectedDart(i))
	  {
	    coul=COUL_LAST[i];
	    larg=LARG_LAST[i];
	    found = true;
	  }

      for (i=0; !found && i<getNbSelectionLevels(); ++i)
	if (isDartSelected(dart0,i))
	  {
	    coul=COUL_SEL[i];
	    larg=LARG_SEL[i];
	    found = true;
	  }

      v1= FMap->getBurstVertex(dart0);
      transfo3dTo2d(v1, p1);

      v2=FMap->computeBurstExtremity(dart0);
      transfo3dTo2d(v2, p2);
      sauveLine(fout,p1,p2,coul,larg,false, computeProf(p1,p0));

      if ( drawVertices )
	sauveDartVertex( fout, p1, p2);
    }
}

//******************************************************************************
void CControlerGMap::treatFace( ofstream & fout, CDart * initDart,
				int mark, int mark2 )
{
  CDart *dart;

  // On parcours ici la face
  CDynamicCoverage01 covF(FMap,initDart);
  while ( covF.cont() )
    {
      if ( !FMap->isMarked(*covF,mark) ) //brin non encore parcouru
        //!FMap->isMarked(FMap->alpha0(*covF),mark) ) // ni son alpha0
        {
          assert(!FMap->isMarked(FMap->alpha0(*covF),mark));
          dart=*covF;

          if (drawArrows) treatDartWithArrow(fout,dart,mark,mark2);
          else treatDartNoArrow(fout,dart,mark,mark2);

          if (drawSews)
            {
              treatSews(fout,dart,mark,mark2);
              if (!FMap->isFree0(dart))
                treatSews(fout,FMap->alpha0(dart),mark,mark2);
            }

          FMap->setMark(dart,mark);
          FMap->setMark(FMap->alpha0(dart),mark);
        }
      ++covF;
    } // Fin de parcours de face
}

//******************************************************************************
void CControlerGMap::treatFilledFace( ofstream & fout, CDart * dart,
				      int mark, int mark2 )
{
  int nbPts=0;
  CVertex v1;
  
  if (drawArrows && FMap->isMarked(dart,mark2))
    dart=FMap->alpha0(dart);

  // On parcours ici la face
  CDynamicCoverage01 covF(FMap,dart);
  while ( covF.cont() )
    {
      ++nbPts;
      if (FMap->isFree0(*covF) || FMap->isFree1(*covF))
        {
          treatFace(fout,dart,mark,mark2);  // si la face n'est pas
          return;               // fermée, on ne peut pas la dessiner pleine !!!
        }
      ++covF;
    }

  bool compactFaces = isZero(FMap->getBurstCoef(1) - 1.0);

  if (compactFaces)
    nbPts/=2;  //car en g carte donc on alpha compte les sommets 2 fois

  CVertex baryFace;
  CVertex * tabV = new CVertex[nbPts+1];
  nbPts=0;

  for (covF.reinit(); covF.cont();)
    {
      v1= FMap->getBurstVertex(*covF);
      transfo3dTo2d(v1, tabV[nbPts++]);
      baryFace += tabV[nbPts-1];

      if (compactFaces && covF.cont())
	++covF;
      
      if (covF.cont())
	++covF;
    }

  baryFace -= tabV[nbPts-1];
  baryFace /= nbPts-1;

  for (covF.reinit(); covF.cont();)
    {
      if ( compactFaces ) // Ici on marque, sinon c'est fait dans la méthode treatFace
	{
	  if (drawArrows)
	    {
	      treatDartWithArrow(fout,*covF,mark,mark2,computeProf(baryFace));
	    }
	  
          if (drawSews)
            {
              treatSews(fout,*covF,mark,mark2);
              if (!FMap->isFree0(*covF))
                treatSews(fout,FMap->alpha0(*covF),mark,mark2);
            }
	  
	  FMap->setMark(*covF,mark);
          FMap->setMark(FMap->alpha0(*covF),mark);
	}      

      if (compactFaces && covF.cont())
	++covF;
      
      if (covF.cont())
	++covF;
    }

  tabV[nbPts++] = tabV[0];

  // On commence par sauver la face externe
  if ( !compactFaces )
    treatFace(fout,dart,mark,mark2);
  
  sauveFace(fout,tabV,nbPts,COUL_FILL_FACE,LARG_FILL_FACE,computeProf(baryFace));

  delete []tabV;
}

//******************************************************************************
void CControlerGMap::saveGridIncludingBox( ofstream & fout )
{
  CVertex v1, v2, p1, p2;
  CVertex vmin(0,0,0), vmax(0,0,0);
  bool init=true;

  // Axis:
  {
    // X
    v1=CVertex(0,0,0); updateMinMax2d(v1,vmin,vmax,init); init=false;
    v1=CVertex(1,0,0); updateMinMax2d(v1,vmin,vmax,init);
    // Y
    v1=CVertex(0,1,0); updateMinMax2d(v1,vmin,vmax,init);
    // Z
    v1=CVertex(0,0,1); updateMinMax2d(v1,vmin,vmax,init);
  }
	
  // Grid:
  {
    int  size= getParameterGrid(FViewId)->getLGGrid();
    bool xy  = getParameterGrid(FViewId)->getDisplayGridXY();
    bool xz  = getParameterGrid(FViewId)->getDisplayGridXZ();
    bool yz  = getParameterGrid(FViewId)->getDisplayGridYZ();
    bool px  = getParameterGrid(FViewId)->getDisplayPositiveGridX();
    bool py  = getParameterGrid(FViewId)->getDisplayPositiveGridY();
    bool pz  = getParameterGrid(FViewId)->getDisplayPositiveGridZ();
    int  xmin= px ? 0 : -size;
    int  ymin= py ? 0 : -size;
    int  zmin= pz ? 0 : -size;

    for (int i=-size; i<=+size; ++i)
      {
	if (xy)
	  {
	    /* X direction: */
	    if (i>=0 || !py)
	      {
		v1=CVertex(xmin, i, 0); updateMinMax2d(v1,vmin,vmax,init);
		init=false;
		v1=CVertex(size, i, 0); updateMinMax2d(v1,vmin,vmax,init);
	      }
	    /* Y direction: */
	    if (i>=0 || !px)
	      {
		v1=CVertex(i, ymin, 0); updateMinMax2d(v1,vmin,vmax,init);
		init=false;
		v1=CVertex(i, size, 0); updateMinMax2d(v1,vmin,vmax,init);
	      }
	  }
	if (xz)
	  {
	    /* X direction: */
	    if (i>=0 || !pz)
	      {
		v1=CVertex(xmin, 0, i); updateMinMax2d(v1,vmin,vmax,init);
		init=false;
		v1=CVertex(size, 0, i); updateMinMax2d(v1,vmin,vmax,init);
	      }
	    /* Z direction: */
	    if (i>=0 || !px)
	      {
		v1=CVertex(i, 0, zmin); updateMinMax2d(v1,vmin,vmax,init);
		init=false;
		v1=CVertex(i, 0, size); updateMinMax2d(v1,vmin,vmax,init);
	      }
	  }
	if (yz)
	  {
	    /* Y direction: */
	    if (i>=0 || !pz)
	      {
		v1=CVertex(0, ymin, i); updateMinMax2d(v1,vmin,vmax,init);
		init=false;
		v1=CVertex(0, size, i);updateMinMax2d(v1,vmin,vmax,init);
	      }
	    /* Z direction: */
	    if (i>=0 || !py)
	      {
		v1=CVertex(0, i, zmin); updateMinMax2d(v1,vmin,vmax,init);
		init=false;
		v1=CVertex(0, i, size); updateMinMax2d(v1,vmin,vmax,init);
	      }
	  }
      }
  }

  debutComposante(fout,vmin,vmax);
}

//******************************************************************************
void CControlerGMap::treatGrid(ofstream & fout)
{
  CVertex v1, v2, p1, p2;

  saveGridIncludingBox(fout);

  // Axis:
  {
    // X
    v1=CVertex(0,0,0);
    v2=CVertex(1,0,0);
    transfo3dTo2d(v1,p1); transfo3dTo2d(v2,p2);
    sauveLine(fout,p1,p2,COUL_AXE_X,LARG_AXE_X,false);
    // Y
    v2=CVertex(0,1,0);
    transfo3dTo2d(v2,p2);
    sauveLine(fout,p1,p2,COUL_AXE_Y,LARG_AXE_Y,false);
    // Z
    v2=CVertex(0,0,1);
    transfo3dTo2d(v2,p2);
    sauveLine(fout,p1,p2,COUL_AXE_Z,LARG_AXE_Z,false);
  }
	
  // Grid:
  {
    int  size= getParameterGrid(FViewId)->getLGGrid();
    bool xy  = getParameterGrid(FViewId)->getDisplayGridXY();
    bool xz  = getParameterGrid(FViewId)->getDisplayGridXZ();
    bool yz  = getParameterGrid(FViewId)->getDisplayGridYZ();
    bool px  = getParameterGrid(FViewId)->getDisplayPositiveGridX();
    bool py  = getParameterGrid(FViewId)->getDisplayPositiveGridY();
    bool pz  = getParameterGrid(FViewId)->getDisplayPositiveGridZ();
    int  xmin= px ? 0 : -size;
    int  ymin= py ? 0 : -size;
    int  zmin= pz ? 0 : -size;

    for (int i=-size; i<=+size; ++i)
      {
	if (xy)
	  {
	    /* X direction: */
	    if (i>=0 || !py)
	      {
		v1=CVertex(xmin, i, 0); v2=CVertex(size, i, 0);
		transfo3dTo2d(v1,p1); transfo3dTo2d(v2,p2);
		sauveLine(fout,p1,p2,COUL_GRID,LARG_GRID,false);
	      }
	    /* Y direction: */
	    if (i>=0 || !px)
	      {
		v1=CVertex(i, ymin, 0); v2=CVertex(i, size, 0);
		transfo3dTo2d(v1,p1); transfo3dTo2d(v2,p2);
		sauveLine(fout,p1,p2,COUL_GRID,LARG_GRID,false);
	      }
	  }
	if (xz)
	  {
	    /* X direction: */
	    if (i>=0 || !pz)
	      {
		v1=CVertex(xmin, 0, i); v2=CVertex(size, 0, i);
		transfo3dTo2d(v1,p1); transfo3dTo2d(v2,p2);
		sauveLine(fout,p1,p2,COUL_GRID,LARG_GRID,false);
	      }
	    /* Z direction: */
	    if (i>=0 || !px)
	      {
		v1=CVertex(i, 0, zmin); v2=CVertex(i, 0, size);
		transfo3dTo2d(v1,p1); transfo3dTo2d(v2,p2);
		sauveLine(fout,p1,p2,COUL_GRID,LARG_GRID,false);
	      }
	  }
	if (yz)
	  {
	    /* Y direction: */
	    if (i>=0 || !pz)
	      {
		v1=CVertex(0, ymin, i); v2=CVertex(0, size, i);
		transfo3dTo2d(v1,p1); transfo3dTo2d(v2,p2);
		sauveLine(fout,p1,p2,COUL_GRID,LARG_GRID,false);
	      }
	    /* Z direction: */
	    if (i>=0 || !py)
	      {
		v1=CVertex(0, i, zmin); v2=CVertex(0, i, size);
		transfo3dTo2d(v1,p1); transfo3dTo2d(v2,p2);
		sauveLine(fout,p1,p2,COUL_GRID,LARG_GRID,false);
	      }
	  }
      }
  }
  fout<<"-6"<<endl;
}

//******************************************************************************
void CControlerGMap::sauveCarteXfig( TViewId AViewId, ofstream & AFout,
				     bool AArrow, bool AFace, bool ASews,
				     bool AGrid, bool AVertices, bool AInvert )
{
  CVertex min, max, vTmp, v2Tmp, v3Tmp;
  CVertex v1;

  FViewId      = AViewId;
  drawArrows   = AArrow;
  drawFaces    = AFace;
  drawSews     = ASews;
  drawGrid     = AGrid;
  drawVertices = AVertices;
  invertY      = AInvert;
  
  // Initialisation for draw the sew
  k0 = FMap->getBurstCoef(0);
  for (int i=0; i<4; i++) kw[i]= getParameterSew(FViewId)->getSewPosCoef(i);

//******************************************************************************
  sauveEntete(AFout,NB_COUL_USER);  //Nombre de couleur ?
  // une brin, une pour chaque couture...
  // Pour les 2 selections, pour les 2 last dart, ...

//******************************************************************************
  CDynamicCoverageAll it(FMap);

  v1  = FMap->getBurstVertex(*it);
  transfo3dTo2d(v1,min);
  max = min;

  while ( it.cont() )
    {
      v1= FMap->getBurstVertex(it++);
      updateMinMax2d(v1,min,max);
    }

  // Here min and max define the including box in 2d of the burst map.
  decalageX=5-min.getX();
  decalageY=5-min.getY();
  coefMultProf=MAX_PROF/(max.getZ()-min.getZ());
  profMin=min.getZ();

  //////////////////  Save the grid
  if (drawGrid)
    treatGrid(AFout);

//******************************************************************************
  int mark=FMap->getNewMark();
  int mark2=FMap->getNewMark();  // Used for orientation

  it.reinit();
  while ( it.cont() )
    {
      if (!FMap->isMarked(*it,mark)) //brin non encore parcouru
        {
          // On parcours la composante connexe incidente
          CDynamicCoverage0123 cov(FMap,*it);
	  while ( cov.cont() )
	    {
              FMap->setMark(*cov,mark);
              if (!FMap->isMarked(FMap->alpha0(*cov),mark2) &&
                  !FMap->isMarked(FMap->alpha1(*cov),mark2) &&
                  !FMap->isMarked(FMap->alpha3(*cov),mark2) &&
                  !FMap->isMarked(FMap->alpha2(*cov),mark2) )
                FMap->setMark(*cov,mark2);
	      ++cov;
            }
        }
      ++it;
    }

  FMap->negateMaskMark(mark);

  // On parcours tout les brins
  it.reinit();
  while ( it.cont() )
    {
      if (!FMap->isMarked(*it,mark)) //brin non encore parcouru
	{
          // On parcours la composante connexe incidente
	  getIncludingBoxForDarts(*it,ORBIT_0123,min,max);
	  debutComposante(AFout,min,max);

          CDynamicCoverage0123 covCc(FMap,*it);
	  while ( covCc.cont() )
	    {
	      if (!FMap->isMarked(*covCc,mark)) //brin non encore parcouru
		{
                  getIncludingBoxForDarts(*covCc,ORBIT_012,min,max);
                  debutComposante(AFout,min,max);

                  // On parcours le volume incident
                  CDynamicCoverageVolume covV(FMap,*covCc);
                  while ( covV.cont() )
                    {
                      if (!FMap->isMarked(*covV,mark)) //brin non traité
                        {
                          getIncludingBoxForDarts(*covV,ORBIT_01,min,max);
                          debutComposante(AFout,min,max);

                          if (AFace)
                            treatFilledFace(AFout,*covV,mark,mark2);
                          else
                            treatFace(AFout,*covV,mark,mark2);

                          AFout<<"-6\n";  //pour fermer la composante xfig
                        }  //Fin du parcours d'une region
		      ++covV;
                    } // Fin du parcours d'un volume
                  AFout<<"-6\n";  //pour fermer la composante xfig
                }
	      ++covCc;
            } // Fin du parcours de tout
          AFout<<"-6\n";  //pour fermer la composante xfig
        }
      ++it;
    }

  FMap->negateMaskMark(mark);
  FMap->freeMark(mark);
  FMap->unmarkAll(mark2);
  FMap->freeMark(mark2);
}
//******************************************************************************
void CControlerGMap::treatPointel(ofstream &os, CDart *d, int mark)
{
  CDynamicCoverage123 it(FMap,d);
  while(it.cont()) FMap->setMark(it++,mark);

  CVertex *v1 = FMap->findVertex(d);

  CVertex p1;
  transfo3dTo2d(*v1, p1);

  sauvePoint(os,p1,COUL_POINTEL,RAYON_POINTEL);
}
//******************************************************************************
void CControlerGMap::treatLignel(ofstream &os, CDart *d, int mark)
{
  CDynamicCoverage023 it(FMap,d);
  while(it.cont()) FMap->setMark(it++,mark);

  CVertex *v1 = FMap->findVertex(d);
  CVertex *v2 = FMap->findVertex(FMap->alpha0(d));

  CVertex edgeBary= (*v1 + *v2)/2;

  CVertex p1 = edgeBary + COEF_REDUC_LIGNEL*(*v1-edgeBary);
  CVertex p2 = edgeBary + COEF_REDUC_LIGNEL*(*v2-edgeBary);

  CVertex pp1, pp2;
  transfo3dTo2d(p1, pp1);
  transfo3dTo2d(p2, pp2);

  sauveLine(os,pp1,pp2,COUL_LIGNEL,LARG_LIGNEL,false);
}
//******************************************************************************
//  Ici on sauve toujours en face pleine, sans fleche, mais avec les
//  elements intervoxels, cad les lignels et les pointels, et sans
//  coutures.
void CControlerGMap::sauveCarteIntervoxelXfig( TViewId AViewId,
					       ofstream & AFout,
					       bool AInvert )
{
  CVertex min, max, vTmp, v2Tmp, v3Tmp;
  CVertex v1;

  FViewId = AViewId;
  invertY = AInvert;

  k0= FMap->getBurstCoef(0);

  drawArrows = false;
  
//******************************************************************************
  sauveEntete(AFout,NB_COUL_USER);  //Nombre de couleur ?
  // une brin, une pour chaque couture...
  // Pour les 2 selections, pour les 2 last dart, ...

//******************************************************************************
  CDynamicCoverageAll it(FMap);

  v1= FMap->getBurstVertex(*it);
  transfo3dTo2d(v1,min);
  max=min;

  while ( it.cont() )
    {
      v1= FMap->getBurstVertex(it++);
      updateMinMax2d(v1,min,max);
    }

  // Here min and max define the including box in 2d of the burst map.
  decalageX=5-min.getX();
  decalageY=5-min.getY();
  coefMultProf=MAX_PROF/(max.getZ()-min.getZ());
  profMin=min.getZ();

//******************************************************************************
  int mark  = FMap->getNewMark();
  int mark2 = FMap->getNewMark();
  int mark3 = FMap->getNewMark();

  // On parcours tout les brins
  it.reinit();
  while ( it.cont() )
    {
      if (!FMap->isMarked(*it,mark)) //brin non encore parcouru
	{
          // On parcours la composante connexe incidente
	  getIncludingBoxForDarts(*it,ORBIT_0123,min,max);
	  debutComposante(AFout,min,max);

          CDynamicCoverage0123 covCc(FMap,*it);
	  while ( covCc.cont() )
	    {
	      if (!FMap->isMarked(*covCc,mark)) //brin non encore parcouru
		{
                  getIncludingBoxForDarts(*covCc,ORBIT_012,min,max);
                  debutComposante(AFout,min,max);

                  // On parcours le volume incident
                  CDynamicCoverageVolume covV(FMap,*covCc);
                  while ( covV.cont() )
                    {
                      if (!FMap->isMarked(*covV,mark)) //brin non traité
                        {
                          getIncludingBoxForDarts(*covV,ORBIT_01,min,max);
                          debutComposante(AFout,min,max);

                          treatFilledFace(AFout,*covV,mark,0);

                          AFout<<"-6\n";  //pour fermer la composante xfig
			}  //Fin du parcours d'une region
		      ++covV;
                    } // Fin du parcours d'un volume
                  AFout<<"-6\n";  //pour fermer la composante xfig
                }
	      ++covCc;
            } // Fin du parcours de tout
          AFout<<"-6\n";  //pour fermer la composante xfig
        }
	
      if (!FMap->isMarked(*it,mark2))
	treatLignel(AFout,*it,mark2);

      if (!FMap->isMarked(*it,mark3))
	treatPointel(AFout,*it,mark3);			

      ++it;
    }

  FMap->negateMaskMark(mark);
  FMap->freeMark(mark);
  FMap->negateMaskMark(mark2);
  FMap->freeMark(mark2);
  FMap->negateMaskMark(mark3);
  FMap->freeMark(mark3);
}

#undef WEIGHTING
//******************************************************************************
