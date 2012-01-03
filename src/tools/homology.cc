/*
 * Homology : Homology computation of a 3D objects made of voxels
 * Copyright (C) 2012, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of Homology
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
#include <cstdlib>
#include <cstring>
#include "g-map-vertex.hh"
#include "extraction-images.hh"
#include "compute-homology.hh"
#include "chrono.hh"

using namespace std;
using namespace GMap3d;

int main(int argc, char** argv)
{
  if ( argc==1 || !strcmp(argv[1],"-?") || !strcmp(argv[1],"-h") )
    {
      cout<<"Usage1 : a.out image (3D png). Pour calculer l'homology de"<<endl
          <<"  l'objet blanc dans l'image."<<endl;
      exit(EXIT_FAILURE);
    }

  // First we compute the 3G-map of the white voxels.
  CGMapVertex g;
  CExtractionImage ext(&g);
  if ( !ext.extractOneRegionVoxels(argv[1],0,0,3,65535,65535,65535,0) )
  {
    cout<<"Problem during extraction of voxels from "<<argv[1]<<endl;
    exit(EXIT_FAILURE);
  }

  //g.save("extract-map.moka");

  CChrono c;
  c.start();
  // Here simplify the map
  g.simplify3DObject();
  c.stop();
  c.display("Simplification time");

  // g.save("simplify-map.moka");
  
  int nbdarts, nbvertices, nbedges, nbfaces, nbvolumes, nbcc;
  g.getGlobalCharacteristics(&nbdarts,&nbvertices,&nbedges,
                             &nbfaces,&nbvolumes,&nbcc,
                             NULL,NULL,NULL,NULL);
  std::cout<<"Map after simplification: darts="<<nbdarts
           <<", vertices="<<nbvertices
           <<", edges="<<nbedges
           <<", faces="<<nbfaces
           <<", volumes="<<nbvolumes
           <<", cc="<<nbcc<<std::endl;

  // Now compute the homology on the simplified representation.
  CChrono c2;
  c.start();
  c2.start();
  
  CHomology h(&g);
  h.computeVolumicHomology();

  c.stop();
  c2.stop();
  c2.display("Homology computation time");

  c.display("TOTAL:");  
  
  std::cout<<"Memory used: "<<h.size()<<" bytes."<<endl;

  // Display Betti numbers
  std::cout<<"Betti numbers Free (0,1,2,3): "<<h.getH0FreeGenerators()
           <<", "<<h.getH1FreeGenerators()<<", "<<h.getH2FreeGenerators()
           <<", "<<h.getH3FreeGenerators()<<endl;
  std::cout<<"Betti numbers Torsion (1,2,3): "<<h.getH1TorsionGenerators()
           <<", "<<h.getH2TorsionGenerators()<<endl;
  
  exit(EXIT_SUCCESS);
}
