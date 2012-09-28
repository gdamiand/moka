/*
 * convert-off-to-moka : Convert a off file into a correspondinf moka file.
 * Copyright (C) 2012, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of Moka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "g-map-vertex.hh"
#include "chrono.hh"
#include "extraction-images.hh"

using namespace std;
using namespace GMap3d;

void displayCharacteristics(CGMapVertex& g, const char* txt)
{
  int nbdarts, nbvertices, nbedges, nbfaces, nbvolumes, nbcc;
  g.getGlobalCharacteristics(&nbdarts,&nbvertices,&nbedges,
                             &nbfaces,&nbvolumes,&nbcc,
                             NULL,NULL,NULL,NULL);
  std::cout<<txt<<": darts="<<nbdarts
           <<", vertices="<<nbvertices
           <<", edges="<<nbedges
           <<", faces="<<nbfaces
           <<", volumes="<<nbvolumes
           <<", cc="<<nbcc<<std::endl;
}

int main(int argc, char** argv)
{
  if ( argc==1 || !strcmp(argv[1],"-?") || !strcmp(argv[1],"-h") )
  {
    cout<<"Usage1 : a.out off. Pour convertir le off en moka file."<<endl;
    return EXIT_FAILURE;
  }

  CGMapVertex m1;

  /*
    CExtractionImage ext1(&m1);
    if ( !ext1.extractOneRegionVoxels(argv[1],0,0,3,0,0,0,0) )
    {
    cout<<"Problem during extraction of voxels from "<<argv[1]<<endl;
    return EXIT_FAILURE;
    }
    m1.simplify3DObject(FACE_REMOVAL);
    m1.save("gmap.moka");
    return EXIT_SUCCESS;
  */
  
  ifstream file(argv[1]);
  if ( !file.is_open() || m1.importOff3D(file)==NULL ) // !m1.load(argv[1]))
  {
    cout<<"Erreur lors de l'import du off "<<argv[1]<<endl;
    return EXIT_FAILURE;
  }
  
  int thisMark = m1.getNewMark();
  int treated = m1.getNewMark();
  for ( CDynamicCoverageAll it(&m1); it.cont(); ++it)
  {
    if(!m1.isFree(*it,3) && !m1.isMarked(*it,treated))
    {
      //Parcours du volume à la main pour traiter le cas des faces alpha_2 = alpha_3
      std::queue<CDart*> parcours;
      parcours.push(*it);
      
      while(!parcours.empty())
      {
        CDart *current = parcours.front();
        parcours.pop();
        
        if(!m1.isMarked(current,treated))
        {
          m1.setMark(current,thisMark);
          m1.setMark(current,treated);
          m1.setMark(m1.alpha3(current),treated);

          for ( int i=0; i<2; ++i) // case i=0 and i=1
            if( !m1.isFree(current,i) &&
                !m1.isMarked(m1.alpha(current,i),treated) )
            {
              parcours.push(m1.alpha(current,i));
            }

          //Special case for alpha2: if alpha2=alpha3, we must not use alpha2
          // otherwise we get the second half shape.
          if( !m1.isFree2(current) &&
              !m1.isMarked(m1.alpha2(current),treated) &&
              m1.alpha2(current) != m1.alpha3(current)&&
              m1.alpha20(current) != m1.alpha3(current) )
          {
            parcours.push(m1.alpha2(current));
          }
        }
      }
    }
  }
  m1.markIncidentCells(ORBIT_01, thisMark);
  m1.deleteMarkedDarts(thisMark);

  m1.negateMaskMark(treated);
  m1.freeMark(treated);
  m1.freeMark(thisMark);

  stringstream s;
  s<<argv[1]<<".moka";
  m1.save(s.str().c_str());

  return EXIT_SUCCESS;
}
