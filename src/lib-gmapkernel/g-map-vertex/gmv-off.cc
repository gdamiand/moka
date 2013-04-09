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
CDart* CGMapVertex::importOff3D(std::istream & AStream)
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
   AStream << endl;

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

   AStream << endl;

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
