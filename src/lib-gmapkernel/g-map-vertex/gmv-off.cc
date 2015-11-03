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
#include "streams.hh"
#include "multivector.hh"
#include <cstring>
#include <map>
using namespace std;
using namespace GMap3d;

//******************************************************************************
void CGMapVertex::linkFaceAlpha2OFF(vector< list<CDart*> >& ATestVertices,
                                    int AIndex, CDart* ADart)
{
  CDart* current = ADart;
  do
  {
    unsigned long int v1 = (unsigned long int)getDirectInfo(current, AIndex);
    unsigned long int v2 = (unsigned long int)getDirectInfo(alpha0(current), AIndex);

    list<CDart*>& tmp1 = ATestVertices[v1];
    list<CDart*>& tmp2 = ATestVertices[v2];

    // Recherche s'il existe déjà une arête v1->v2. Si oui couture par alpha2.
    list<CDart*>::iterator it;
    CDart* foundEdge = NULL;
    for (it = tmp1.begin(); foundEdge == NULL && it != tmp1.end(); ++it)
    {
      if (((unsigned long int)getDirectInfo(alpha0(*it), AIndex)) == v2)
      {
        foundEdge = *it;
      }
    }

    if (foundEdge != NULL)
    {
      // Le vecteur associé à l'arête autour de laquelle on tourne.
      CVertex edge = edgeVector(foundEdge);
      CVertex vect1 =
          CGeometry::getNormalVector(faceNormalVector(foundEdge), edge);

      CVertex vect2;
      CVertex vectNew =
          CGeometry::getNormalVector(faceNormalVector(current), edge);

      // On va tourner autour de l'arête pour trouver l'endroit ou
      // insérer la face courante.
      bool here = false;
      bool roundingError = false;
      CDart* first = foundEdge;

      TCoordinate angle1, angle2;

      if (!isFree2(foundEdge))
        while (!here)
        {
          vect2 = CGeometry::getNormalVector
              (faceNormalVector(alpha23(foundEdge)), edge);

          angle1 = positifAngle
              (CGeometry::getAngle(vect1, vect2, edge));

          angle2 =
              positifAngle
              (CGeometry::getAngle(vect1, vectNew, edge)) +
              positifAngle
              (CGeometry::getAngle(vectNew, vect2, edge));

          if (areEqual(angle1, angle2))
            here = true;
          else
          {
            vect1 = vect2;
            foundEdge = alpha23(foundEdge);
          }

          // Pour gérer les erreurs d'arrondi...
          if (!here && foundEdge == first)
          {
            roundingError = true;
            here = true;
          }
        }

      if (!roundingError)
      {
        if (isFree2(foundEdge))
        {
          sew2(alpha3(current), foundEdge);
          sew2(current, alpha3(foundEdge));
        }
        else
        {
          CDart* prev1 = alpha2(foundEdge);
          unsew2(foundEdge);

          sew2(alpha3(current), foundEdge);
          sew2(current, prev1);
        }
      }
    }
    else
    {
      // On ajoute les 2 brins dart1 et dart2 dans les listes associées à
      // v1 et v2 pour la prochaine recherche d'arête.
      tmp1.push_back(current);
      tmp2.push_back(alpha03(current));

      // tmp1.push_back(alpha3(dart1));
      // tmp2.push_back(alpha3(dart2));
    }

    current = alpha01(current);
  }
  while (current != ADart);
}
//******************************************************************************
struct ForLoadOff
{
  unsigned long int index;
  CMultivector multivector;
};
//******************************************************************************
void  CGMapVertex::linkFacesAlpha2OFF_VSF(vector< list<CDart*> >& ATestVertices,
                                          int AIndex)
{
  CDart *dart,*dartneighbour;
  vector<CDart*>  subVector;
  vector< int >   order;
  vector< nklein::GeometricAlgebra< double, 4 > > MvVector;
  list<CDart*>::iterator it1,it2;
  unsigned long int v1,v2;

  /** each point */
  for( v1=0; v1 < ATestVertices.size(); ++v1)
  {
    list<CDart*> &List1=ATestVertices[v1];
    dart=NULL;
    for(it1=List1.begin();it1!=List1.end();++it1)//! point 1
    {
      /** each dart not alpha2 */
      if((*it1)->isFree2())
      {
        dart=(*it1);
        v2 = ((ForLoadOff*)getDirectInfo(alpha0(dart), AIndex))->index;
        list<CDart*> &List2=ATestVertices[v2];
        subVector.clear();
        for(it2=List2.begin();it2!=List2.end();++it2)//! iterate on point2
        {
          if((*it2)->isFree2())
          {
            if(v1==((ForLoadOff*)getDirectInfo
                    (alpha0((*it2)), AIndex))->index)
              subVector.push_back((*it2));//! darts (+) at v2=>v1
          }
        }
        /** Something to sew? */
        std::cout<<v2<<"=>"<<v1<<"{"<<subVector.size()<<"}\n";
        if(subVector.size()>1)
        {
          if(subVector.size()>2)
          {
            /** order the pencil of planes */
            MvVector.clear();
            for(int i=0;i<subVector.size();++i)
            {
              dart=subVector[i];
              MvVector.push_back(((ForLoadOff*)getDirectInfo(dart, AIndex))->
                                 multivector.getMD());
            }
            CGeometry::sortPencil2(MvVector,order);
          }
          else //! just 2 faces to sew, no need to order
          {
            order.clear();
            order.push_back(0);
            order.push_back(1);
          }
          /** sew according to the order */
          for(int i=0; i<order.size();++i)
          {
            dart=subVector[order[i]];//(+)
            int j=((i+1)<order.size()?(i+1):0);
            dartneighbour=subVector[order[j]];//(+)
            sew2(dart,alpha3(dartneighbour));//(+)..(-)
          }
        }
        /** nothing to do */
      }
    }
  }
}

//******************************************************************************
CDart* CGMapVertex::importOff2D(std::istream & AStream)
{
  // Lecture des sommets à charger:
  vector< CVertex > initVertices;
  vector< list<CDart*> > testVertices;

  string txt;
  TCoordinate x, y;
  CDart *d1 = NULL, *d2 = NULL;
  unsigned int v1, v2;

  AStream >> txt;
  if (txt != "OFF2D")
  {
    cout << "Problème d'entête : fichier non OFF" << endl;
    return NULL;
  }

  unsigned int nbSommets = 0;
  unsigned int nbAretes = 0;

  AStream >> nbSommets >> nbAretes;
  while (nbSommets > 0)
  {
    if (!AStream.good())
    {
      cout << "Problème de lecture : pas assez de sommets" << endl;
      return NULL;
    }

    AStream >> x >> y;
    initVertices.push_back(CVertex(x, y, 0));
    testVertices.push_back(list<CDart*>());
    --nbSommets;
  }

  while (nbAretes > 0)
  {
    if (!AStream.good())
    {
      cout << "Problème de lecture : pas assez d'arêtes" << endl;
      return NULL;
    }

    // Le nombre de sommets
    AStream >> v1 >> v2; AStream.ignore(256,'\n');
    --nbAretes;

    assert(v1 < initVertices.size());
    assert(v2 < initVertices.size());

    d1 = addMapDart(initVertices[v1]);
    d2 = addMapDart(initVertices[v2]);
    linkAlpha0(d1, d2);

    linkAlpha2(d1, addMapDart());
    linkAlpha2(d2, addMapDart());
    linkAlpha0(alpha2(d1), alpha2(d2));

    testVertices[v1].push_back(d1);
    testVertices[v2].push_back(alpha2(d2));
  }

  map<double, CDart*> tabDart;
  list<CDart*>::iterator it;
  map<double, CDart*>::iterator it2;

  CDart* first=NULL;
  CDart* prec=NULL;
  CVertex vectNormal(0, 0, 1);
  CVertex vect1, vect2, sommet;

  for (unsigned int i = 0; i < initVertices.size(); ++i)
  {
    it = testVertices[i].begin();
    if (it != testVertices[i].end()) // Si la liste n'est pas vide.
    {
      // 1. on insère tout les brins en les triant selon l'angle
      // avec first
      tabDart.clear();
      first = *it;

      sommet = *findVertex(*it);
      vect1 = *findVertex(alpha0(*it)) - sommet;
      ++it;
      while (it != testVertices[i].end())
      {
        vect2 = *findVertex(alpha0(*it)) - sommet;
        tabDart.insert
            (pair<double, CDart*>
             (positifAngle(CGeometry::getAngle(vect1, vect2, vectNormal)),
              *it));
        ++it;
      }

      // 2. On parcours le tableau des brins trié et on couds.
      it2 = tabDart.begin();
      prec = first;
      while (it2 != tabDart.end())
      {
        sew1(prec, alpha2(it2->second));
        prec = it2->second;
        ++it2;
      }
      sew1(prec, alpha2(first));
    }
  }

  // On retourne un brin chargé (n'importe lequel)
  return first;
}
//******************************************************************************
CDart* CGMapVertex::addEdgeOFF(vector< CVertex >& AInitVertices,
                               unsigned long int AV1, unsigned long int AV2,
                               int AIndex, CDart* APrec)
{
  CDart* dart1 = addMapDart(AInitVertices[AV1]);
  CDart* dart2 = addMapDart();

  setDirectInfo(dart1, AIndex, (void*)AV1);
  setDirectInfo(dart2, AIndex, (void*)AV2);

  // Coutures par alpha0 et alpha1
  linkAlpha0(dart1, dart2);

  linkAlpha3(dart1, addMapDart());
  linkAlpha3(dart2, addMapDart());
  linkAlpha0(alpha3(dart1), alpha3(dart2));

  setDirectInfo(alpha3(dart1), AIndex, (void*)AV1);
  setDirectInfo(alpha3(dart2), AIndex, (void*)AV2);

  if (APrec != NULL)
  {
    linkAlpha1(APrec, dart1);
    linkAlpha1(alpha3(APrec), alpha3(dart1));
  }

  return dart2;
}
//******************************************************************************
CDart* CGMapVertex::addEdgeOFF_VSF(vector< CVertex >& AInitVertices,
                                   vector< list<CDart*> >& ATestVertices,
                                   unsigned long int AV1,
                                   unsigned long int AV2,
                                   int AIndex, CDart* APrec,
                                   CVertex AVertex,
                                   nklein::GeometricAlgebra< double,4 > &faceplane)
{
   list<CDart*>& tmp1 = ATestVertices[AV1];
   list<CDart*>& tmp2 = ATestVertices[AV2];

   ForLoadOff *ford1 = new ForLoadOff;
   ForLoadOff *ford2 = new ForLoadOff;
   ford1->index=AV1;
   ford1->multivector=CGeometry::getMVectorPLV(faceplane,AInitVertices[AV1],AInitVertices[AV2],AVertex, 1, 1);
   ford2->index=AV2;
   ford2->multivector=CGeometry::getMVectorPLV(faceplane,AInitVertices[AV2],AInitVertices[AV1],AVertex,-1,-1);

   CDart* dart1 = addMapDart(AInitVertices[AV1]);
   setDirectInfo(dart1, AIndex, ford1);

   CDart* dart2 = addMapDart();
   setDirectInfo(dart2, AIndex, ford2);

   //! Coutures par alpha0: dart1---dart2
   linkAlpha0(dart1, dart2);

   //! The other pair
   //! dart3---dart4
   //! dart1---dart2
   ForLoadOff *ford3 = new ForLoadOff;
   ForLoadOff *ford4 = new ForLoadOff;
   ford3->index=AV1;
   ford3->multivector=CGeometry::getMVectorPLV(faceplane,AInitVertices[AV1],AInitVertices[AV2],AVertex, 1,-1);
   ford4->index=AV2;
   ford4->multivector=CGeometry::getMVectorPLV(faceplane,AInitVertices[AV2],AInitVertices[AV1],AVertex,-1, 1);

   CDart* dart3 = addMapDart();
   setDirectInfo(dart3, AIndex, ford3);

   CDart* dart4 = addMapDart();
   setDirectInfo(dart4, AIndex, ford4);

   linkAlpha3(dart1, dart3);
   linkAlpha3(dart2, dart4);

   linkAlpha0(alpha3(dart1), alpha3(dart2));

   tmp1.push_back(dart1); //! the positive volumes are the proxies at each vertex
   tmp2.push_back(alpha3(dart2));

   //! Cose alpha1 si hay anteriores
   if (APrec != NULL)
   {
      linkAlpha1(APrec, dart1);
      linkAlpha1(alpha3(APrec), alpha3(dart1));
   }

   return dart2;
}
//******************************************************************************
/*CDart* CGMapVertex::importOff3D(std::istream & AStream)
{
  // Lecture des sommets à charger:
  vector< CVertex > initVertices;
  vector< list<CDart*> > testVertices;

  string txt;
  TCoordinate x, y, z;
  CDart *prec = NULL, *first = NULL;
  unsigned int i, n;
  unsigned long int v1, v2, vf;

  AStream >> txt;
  if (txt != "OFF" && txt != "OFF3D")
  {
    cout << "Problème d'entête : fichier non OFF" << endl;
    return NULL;
  }

  unsigned int nbSommets = 0;
  unsigned int nbFaces = 0;
  unsigned int doubleNbAretes = 0;

  AStream >> nbSommets >> nbFaces >> doubleNbAretes;
  while (nbSommets > 0)
  {
    if (!AStream.good())
    {
      cout << "Problème de lecture : pas assez de sommets" << endl;
      return NULL;
    }

    AStream >> x >> y >> z;
    initVertices.push_back(CVertex(x, y, z));
    testVertices.push_back(list<CDart*>());
    --nbSommets;
  }

  if(nbSommets!=0)
  {
    cout<<"Pas assez de sommets dans le fichier."<< endl;
    return NULL;
  }

  int index = getNewDirectInfo();

  while (nbFaces > 0)
  {
    if (!AStream.good())
    {
      cout << "Problème de lecture : pas assez de faces" << endl;
      freeDirectInfo(index);
      return NULL;
    }

    // Le nombre de sommets
    AStream >> n;
    prec  = NULL;
    first = NULL;

    // Le premier sommet.
    AStream >> v1; --n;
    vf = v1;
    assert(v1 < initVertices.size());

    // Les autres.
    for (i = 0;i < n;++i)
    {
      AStream >> v2;
      assert(v2 < initVertices.size());

      prec = addEdgeOFF(initVertices, v1, v2, index,
                        prec);

      if (first == NULL) first = alpha0(prec);

      v1 = v2;
    }
    AStream.ignore(256,'\n'); // Ignore the end of the line.

    prec = addEdgeOFF(initVertices, v1, vf, index,
                      prec);

    linkAlpha1(first, prec);
    linkAlpha1(alpha3(first), alpha3(prec));

    linkFaceAlpha2OFF(testVertices, index, first);

    --nbFaces;
  }

  freeDirectInfo(index);

  return first;
}*/
//******************************************************************************
/**
 *  Compute sense of the outer polygon and correct the sense of its holes
 *  they must have the opposite sense.
 *  The baricentre is used as auxiliary point afterwards to associate a plane
 *  to each dart. Therefore it should not coincide with any point of the face
 *  it is also checked here.
 *  (v1).....(v1) OUTER , (vi1).....(vi1) hole1 , ...
 */
void CGMapVertex::computeOFFSenses_VSF(vector< list<int> >& face,
                                       vector< CVertex >& AInitVertices,
                                       nklein::GeometricAlgebra< double,4 > &
                                       faceplane,
                                       CVertex &baricentro,
                                       vector< int > &faceseq)
{
  nklein::GeometricAlgebra< double, 4 >* Points[face.size()],B,I,Line;
  nklein::GeometricAlgebra< double, 4 > planeOuter, planeOuterD;
  nklein::GeometricAlgebra< double, 4 > planeHole, planeHoleD;
  double tol=0.00001;
  bool okBaricentre;
  CVertex Vaux;
  list< int >::iterator jit;
  int v1,nPoints=0;

  I[e0|e1|e2|e3]=1;

  /** points in projective space */
  B[e0]=1;
  B[e1]=baricentro.getX();
  B[e2]=baricentro.getY();
  B[e3]=baricentro.getZ();

  for(int i=0;i<face.size();i++)
    Points[i]=new nklein::GeometricAlgebra< double, 4 >[face[i].size()];

  for(int i=0;i<face.size();i++)
  {
    int j=0;
    for(jit=face[i].begin(); jit != face[i].end() ;jit++)//iterate list
    {
      Points[i][j][e0]=1;
      Points[i][j][e1]=AInitVertices[(*jit)].getX();
      Points[i][j][e2]=AInitVertices[(*jit)].getY();
      Points[i][j][e3]=AInitVertices[(*jit)].getZ();
      ++j;++nPoints;
    }
  }

  /** avoid baricentre coincidence with any point in face */
  /** but keep its "co-planarity" */
  int ii,jj,ik,jk;
  int weight=0;
  do
  {
    okBaricentre=true;
    /** check okBaricentre */
    for( ii=0;ii<face.size();ii++)
    {
      for( jj=0;jj<face[ii].size();++jj)
      {
        Line=B^Points[ii][jj];
        if(fabs(Line[e0|e1])<tol &&
           fabs(Line[e0|e2])<tol &&
           fabs(Line[e0|e3])<tol &&
           fabs(Line[e1|e2])<tol &&
           fabs(Line[e1|e3])<tol &&
           fabs(Line[e2|e3])<tol)
        {
          okBaricentre=false;
          break;
        }//! SAME POINT!
      }
      if(!okBaricentre) break;
    }
    /** new baricentre */
    if(!okBaricentre)
    {
      ik=0;jk=0;++weight;
      if(ik==ii && jk==jj) jk=1;
      Vaux.setXYZ(Points[ik][jk][e1],Points[ik][jk][e2],Points[ik][jk][e3]);
      baricentro=(baricentro*((double)nPoints)+Vaux*((double)weight))/
          ((double)(nPoints+weight));
      B[e0]=1;
      B[e1]=baricentro.getX();
      B[e2]=baricentro.getY();
      B[e3]=baricentro.getZ();
    }
  }while(!okBaricentre);

  /** planes: (vi)....(vi) */
  // here the coplanarity could be checked
  for(int i=0; i<face.size() ;i++)
  {
    planeHole=0;
    for(int j=0; j<(face[i].size()-1);++j)
    {
      if(i==0)//outer
      {
        planeOuter=planeOuter+B^Points[i][j]^Points[i][j+1];
      }
      else//holes
      {
        planeHole=planeHole+B^Points[i][j]^Points[i][j+1];
      }
    }

    if(i==0)
    {
      //            planeOuterD=planeOuter*I;
      //            if(planeOuterD[e1]>0)
      //                sense=1;
      //            else if(planeOuterD[e1]<0)
      //                sense=-1;
      //            else
      //                sense=0; //OJO hay que manejar esto de otro modo
      planeOuterD=planeOuter*I;
      faceplane=planeOuterD;
    }
    else /** outer and hole same sense => reverse */
    {
      planeHoleD=planeHole*I;
      //            if((planeOuterD[e0]*planeHoleD[e0])>0) //OJO:usar funcion cmpPlaneSense()
      //                face[i].reverse();
      if( CGeometry::cmpPlaneSense(planeOuterD,planeHoleD)==1)
        face[i].reverse();
    }
  }

  /** rewrite sequence */
  /** (v1)v2...(v1)vi..vi(v1)vi..vi */
  v1=face[0].front();
  for(jit=face[0].begin(); jit != face[0].end() ;jit++)//iterate list outer
    faceseq.push_back((*jit));

  faceseq.pop_back();

  for(int i=1;i<face.size();i++)
  {
    faceseq.push_back(v1);
    for(jit=face[i].begin(); jit != face[i].end() ;jit++)//iterate list hole-i
      faceseq.push_back((*jit));
  }

  /** destroy */
  for(int i=0;i<face.size();i++)
    delete [] Points[i];
}
//******************************************************************************
//CDart* CGMapVertex::importOff3D_VSF(std::istream & AStream)
CDart* CGMapVertex::importOff3D(std::istream & AStream)
{
  vector< CVertex > initVertices;
  vector< list<CDart*> > testVertices;
  vector< list<int> > face;
  nklein::GeometricAlgebra< double, 4 > planeD;
  vector < int > facesequence;
  int sense;
  CVertex point;

  string txt;
  TCoordinate x, y, z;
  CDart *prec = NULL, *first = NULL;
  unsigned int i, n,nFaceVertex,npol,npoints;
  unsigned long int v1, v2, vf;

  AStream >> txt;
  //if (txt != "OFFVSF" && txt != "OFF3DVSF")
  if (txt != "OFF" && txt != "OFF3D")
  {
    cout << "Input problem : file is not OFF format" << endl;
    return NULL;
  }

  unsigned int nbSommets = 0;
  unsigned int nbFaces = 0;
  unsigned int doubleNbAretes = 0;

  AStream >> nbSommets >> nbFaces >> doubleNbAretes;

  while (nbSommets > 0)
  {
    if (!AStream.good())
    {
      cout << "Input problem : vertex" << endl;
      return NULL;
    }

    AStream >> x >> y >> z;
    initVertices.push_back(CVertex(x, y, z));//!< crea el vértice
    testVertices.push_back(list<CDart*>());//!< crea una lista de dardos para este vértice
    --nbSommets;
  }

  if(nbSommets!=0) return NULL;//**VIC**

  /** Pide indice en DirectInfo para guardar el indice del vertice de dardo*/
  int index = getNewDirectInfo();

  /** Lectura de las caras */
  while (nbFaces > 0)
  {
    if (!AStream.good())
    {
      cout << "Input problem : faces" << endl;
      freeDirectInfo(index);
      return NULL;
    }

    /** number of points in face */
    AStream >> n;
    nFaceVertex=n;
    prec  = NULL;
    first = NULL;
    if(!face.empty())
      face.clear();
    if(!facesequence.empty())
      facesequence.clear();
    npol=0;npoints=0;

    /** points=> baricentre, polygons in face are classified */
    /** (v1)v2..... */
    /** (v1) v2 ...(v1)vi...vi (v1)vi...vi */
    AStream >> v1; --n;
    vf=v1;
    assert(v1 < initVertices.size());
    point=initVertices[v1];++npoints;
    ++npol;
    face.push_back(list<int>());
    face[npol-1].push_back(v1);
    for (i=0;i < n;++i)
    {
      AStream >> v2;
      assert(v2 < initVertices.size());
      if(v2==v1)
      {
        //face[npol-1].pop_back();
        ++npol;face.push_back(list<int>());
      }
      else
      {
        point=point+initVertices[v2];++npoints;
        face[npol-1].push_back(v2);
      }
    }
    /** fix repeted first vertex with no holes i.e 5 27 0 4 3 27 */
    for(i=0;i< face.size();++i)
    {
      if(face[i].size()==0)
      {
        //std::cout<<"AQUI!!\n";
        face.pop_back();
        break;
      }
    }
    face[0].push_back(v1);

    point=point+initVertices[v1];;++npoints;
    AStream.ignore(256,'\n'); //! Ignore the end of the line of the face.

    point=point/((double)npoints); //! Baricentre counts repeated points orignal sequence
    computeOFFSenses_VSF(face,initVertices,planeD,point,facesequence);

    /** each side : named by the starting point 0..(n-1) */
    n=facesequence.size();
    for (i = 1;i < n;++i)
    {
      //AStream >> v2;
      //assert(v2 < initVertices.size());
      //point=point+initVertices[v2];
      v2=facesequence[i];//! read now from the vector

      prec = addEdgeOFF_VSF(initVertices,testVertices, v1, v2, index, prec,point,planeD);

      if (first == NULL) first = alpha0(prec);

      v1 = v2;
    }
    //AStream.ignore(256,'\n'); // Ignore the end of the line.
    //point=point/nFaceVertex; //! Baricentre

    /** cierra la cara lado 0 */
    prec = addEdgeOFF_VSF(initVertices,testVertices, v1, vf, index, prec,point,planeD);

    linkAlpha1(first, prec);
    linkAlpha1(alpha3(first), alpha3(prec));

    /** conecta la cara a las otras caras ya leidas */
    //linkFaceAlpha2OFF(testVertices, index, first);

    --nbFaces;
  }

  /** once all faces are read sew-alpha2 all faces to form volumes*/
  linkFacesAlpha2OFF_VSF(testVertices,index);

  freeDirectInfo(index);

  return first;
}
//******************************************************************************
int CGMapVertex::getOffDimension(const char * AFilename)
{
  ifstream stream;
  stream.open(AFilename);
  string txt;
  stream >> txt;
  stream.close();

  if (txt == "OFF2D")      return 2;
  else if (txt == "OFF" || txt == "OFF3D") return 3;

  return -1;
}   
//******************************************************************************
CDart* CGMapVertex::importOff(const char * AFilename)
{
  ifstream stream;
  stream.open(AFilename);
  int dim = getOffDimension(AFilename);

  switch (dim)
  {
    case 2: return importOff2D(stream); break;
    case 3: return importOff3D(stream); break;
  }
  return NULL;
}
//******************************************************************************
bool CGMapVertex::exportOff2D(std::ostream & AStream)
{
  CDynamicCoverageAll it(this);

  // 1) Numérotation des sommets de 0 à n-1
  int directInfoIndex = getNewDirectInfo();
  int markVertex = getNewMark();
  int markEdge   = getNewMark();
  unsigned long int nbV = 0;
  unsigned long int nbE = 0;
  for (it.reinit(); it.cont(); ++it)
  {
    if (!isMarked(*it, markVertex))
    {
      for (CBasicDynamicCoverageVertex it2(this, *it, markVertex);
           it2.cont(); ++it2)
        setDirectInfo(*it2, directInfoIndex, (void*)nbV);
      ++nbV;
    }
    if (!isMarked(*it, markEdge))
    {
      markOrbit(*it, ORBIT_EDGE, markEdge);
      ++nbE;
    }
  }

  // 2) Sauvegarde de l'entête
  AStream << "OFF2D" << endl;
  AStream << nbV << " " << nbE << endl;
  AStream << endl;

  // 3) Sauvegarde des sommets
  negateMaskMark(markVertex);
  for (it.reinit(); it.cont(); ++it)
  {
    if (!isMarked(*it, markVertex))
    {
      CVertex* v = findVertex(*it);
      AStream << v->getX() << " " << v->getY() << endl;
      markOrbit(*it, ORBIT_VERTEX, markVertex);
    }
  }

  AStream << endl;

  // 4) Sauvegarde des arêtes
  negateMaskMark(markEdge);
  for (it.reinit(); it.cont(); ++it)
  {
    if (!isMarked(*it, markEdge))
    {
      AStream << (unsigned long int)getDirectInfo(*it, directInfoIndex)
              << " " << (unsigned long int)getDirectInfo(alpha0(*it),
                                                         directInfoIndex) << endl;
      markOrbit(*it, ORBIT_EDGE, markEdge);
    }
  }

  negateMaskMark(markVertex); freeMark(markVertex);
  negateMaskMark(markEdge); freeMark(markEdge);
  freeDirectInfo(directInfoIndex);

  return true;
}
//******************************************************************************
bool CGMapVertex::exportOff3D(std::ostream & AStream)
{
  CDynamicCoverageAll it(this);

  // 1) Numérotation des sommets de 0 à n-1
  int directInfoIndex = getNewDirectInfo();
  int markVertex = getNewMark();
  int markFace   = getNewMark();
  unsigned long int nbV = 0;
  unsigned int nbE = 0; // TODO compter les arêtes
  unsigned int nbF = 0;

  for (it.reinit(); it.cont(); ++it)
  {
    if (!isMarked(*it, markVertex))
    {
      for (CBasicDynamicCoverageVertex it2(this, *it, markVertex);
           it2.cont(); ++it2)
        setDirectInfo(*it2, directInfoIndex, (void*)nbV);
      ++nbV;
    }
    if (!isMarked(*it, markFace))
    {
      bool faceOuverte = false;
      for (CDynamicCoverage01 itFace(this,*it); itFace.cont(); ++itFace)
      {
        if (isFree0(*it)||isFree1(*it)) faceOuverte=true;
        setMark(alpha3(*itFace), markFace);
        setMark(*itFace, markFace);
      }
      if ( !faceOuverte ) ++nbF;
    }
  }

  // 3) Sauvegarde de l'entête
  AStream << "OFF" << endl;
  AStream << nbV << " " << nbF << " " << nbE << endl;

  // 4) Sauvegarde des sommets
  negateMaskMark(markVertex);
  for (it.reinit(); it.cont(); ++it)
  {
    if (!isMarked(*it, markVertex))
    {
      CVertex* v = findVertex(*it);
      AStream << v->getX() << " " << v->getY() << " " << v->getZ() << endl;
      markOrbit(*it, ORBIT_VERTEX, markVertex);
    }
  }

  // 5) Sauvegarde des faces (sauf les faces ouvertes)
  negateMaskMark(markFace);
  for (it.reinit(); it.cont(); ++it)
  {
    if (!isMarked(*it, markFace))
    {
      CDynamicCoverage01 itFace(this,*it);
      int n=0; bool faceOuverte = false;
      for (; !faceOuverte && itFace.cont(); ++itFace)
      {
        if (isFree0(*it)||isFree1(*it)) faceOuverte=true;
        ++n;
      }

      if ( !faceOuverte )
      {
        // Sauvegarde du nombre de sommet de la face
        AStream << (n/2) <<" ";
        itFace.reinit();
        while( itFace.cont() )
        {
          AStream << (unsigned long int)getDirectInfo(*itFace, directInfoIndex)
                  << " ";

          setMark(alpha3(*itFace), markFace);
          setMark(itFace++, markFace); assert(itFace.cont());

          setMark(alpha3(*itFace), markFace);
          setMark(itFace++, markFace);
        }
        AStream << endl;
      }
      else
        markOrbit(*it, ORBIT_FACE, markFace);
    }
  }

  negateMaskMark(markVertex); freeMark(markVertex);
  negateMaskMark(markFace); freeMark(markFace);
  freeDirectInfo(directInfoIndex);

  return true;
}
//******************************************************************************
bool CGMapVertex::exportOff(const char * AFilename)
{
  ofstream stream;
  stream.open(AFilename, ios::trunc);

  int dim = getMapDimension();
  bool res = false;

  if ( dim==3 ) res = exportOff3D(stream);
  else          res = exportOff2D(stream);

  stream.close();
  return res;
}
//******************************************************************************
bool CGMapVertex::exportOff3D(const char * AFilename)
{
  ofstream stream;
  stream.open(AFilename, ios::trunc);

  bool res = exportOff3D(stream);

  stream.close();
  return res;
}
//******************************************************************************
