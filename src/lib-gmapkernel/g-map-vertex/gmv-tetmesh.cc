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
#include <sstream>
#include <map>
using namespace std;
using namespace GMap3d;

//******************************************************************************
CDart* CGMapVertex::createNewTriangle(CVertex* v0, CVertex* v1, CVertex* v2,
                                      map<triplet, CDart*>& faces)
{
  CDart * d= createTopoTriangle();

  setVertex(          d , *v0);
  setVertex(alpha01  (d), *v1);
  setVertex(alpha0101(d), *v2);

  triplet t, topposite;
  CDart * dmin;
  
  if ( v0<v1 )
  {
    if ( v0<v2 ) // v0 is the min
    {
      t=triplet(v0, v1, v2);
      topposite=triplet(v0, v2, v1);
      dmin=d;
    }
    else // v2 is the min
    {
      t=triplet(v2, v0, v1);
      topposite=triplet(v2, v1, v0);
      dmin=alpha0101(d);
    }
  }
  else
  {
    if ( v1<v2 ) // v1 is the min
    {
      t=triplet(v1, v2, v0);
      topposite=triplet(v1, v0, v2);
      dmin=alpha01(d);
    }
    else // v2 is the min
    {
      t=triplet(v2, v0, v1);
      topposite=triplet(v2, v1, v0);
      dmin=alpha0101(d);
    }
  }

  assert( faces.count(t)==0 );
  faces[t]= dmin;

  map<triplet, CDart*>::iterator it=faces.find(topposite);

  if ( it!=faces.end() )
  {
    sew3(alpha1(dmin), it->second);
  }
  
  return d;
}
//******************************************************************************
CDart* CGMapVertex::createNewTetra(CVertex* v0, CVertex* v1, CVertex* v2,
                                   CVertex* v3, map<triplet, CDart*>& faces)
{
  CDart* d1 = createNewTriangle(v0, v1, v2, faces);
  CDart* d2 = createNewTriangle(v0, v3, v1, faces);
  CDart* d3 = createNewTriangle(v0, v2, v3, faces);
  CDart* d4 = createNewTriangle(v1, v3, v2, faces);

  sew2(d1,          alpha1(d2));
  sew2(alpha1(d1),  d3);
  sew2(alpha01(d1), alpha1(d4));

  sew2(d2,          alpha1(d3));
  sew2(alpha01(d2), alpha0(d4));

  sew2(alpha01(d3), alpha010(d4));

  return d1;
}
//******************************************************************************
CDart* CGMapVertex::importTetmesh(std::istream & AStream)
{
   // Lecture des sommets à charger:
   vector< CVertex > points;
   size_t i, num_vertices, num_tetras;
   string line;
   map<triplet, CDart*> faces;
   CDart* first=NULL;
   std::stringstream sStream;
   
   // skip Vertices label
   std::getline(AStream, line);
   assert( line=="Vertices" );
   
   std::getline(AStream, line); // num_vertices x
   sStream << line;
   sStream >> num_vertices;
   sStream.clear();

   // reverse the order of the vertices
   points.reserve(num_vertices);
   
   for(i = 0; i < num_vertices; ++i)
   {
     float x, y, z;
     std::getline(AStream, line);
     sStream << line;
     sStream >> x >> y >> z;
     std::getline(sStream, line);
     sStream.clear();
     
     points.push_back(CVertex(x, y, z));
   }

   // skip Tetrahedra label
   std::getline(AStream, line);
   assert( line=="Tetrahedra" );

   // read number of tetraeders
   std::getline(AStream, line); // num_tetras x
   sStream << line;
   sStream >> num_tetras;
   sStream.clear();

   for(i = 0; i < num_tetras; ++i)
   {
     int v0,v1,v2,v3;
     std::getline(AStream, line);
     sStream << line;
     sStream >> v0 >> v1 >> v2 >> v3;
     std::getline(sStream, line);
     sStream.clear();

     CDart* dart=createNewTetra
       (&points[v0-1], &points[v1-1], &points[v2-1], &points[v3-1], faces);

     if (first==NULL) first=dart;
   }
   
   return first;
}
//******************************************************************************
CDart* CGMapVertex::importTetmesh(const char * AFilename)
{
   ifstream stream;
   stream.open(AFilename);
   return importTetmesh(stream);
}
//******************************************************************************
