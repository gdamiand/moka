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

void computeHomology(CGMapVertex& g, const char* txt)
{
  // Compute the homology on the simplified representation.
  CChrono c;
  c.start();

  CHomology h(&g);
  h.computeVolumicHomology();

  c.stop();
  std::cout<<"Time for homology ";
  c.display(txt);

  std::cout<<"Memory used "<<txt<<": "<<h.size()<<" bytes."<<endl;

  // Display Betti numbers
  std::cout<<"Betti numbers Free "<<txt<<" (0,1,2,3): "<<h.getH0FreeGenerators()
          <<", "<<h.getH1FreeGenerators()<<", "<<h.getH2FreeGenerators()
         <<", "<<h.getH3FreeGenerators()<<endl;
  std::cout<<"Betti numbers Torsion "<<txt<<" (1,2): "<<h.getH1TorsionGenerators()
          <<", "<<h.getH2TorsionGenerators()<<endl;
}

void process(char* file, int mode,bool withoriginal, const char*txt)
{
  CChrono c;
  CGMapVertex g1;
  CExtractionImage ext1(&g1);
  if ( !ext1.extractOneRegionVoxels(file,0,0,3,0,0,0,0) )
    //if ( !ext1.extractOneRegionVoxels(file,0,0,3,65535,65535,65535,0) )
  {
    cout<<"Problem during extraction of voxels from "<<file<<endl;
    exit(EXIT_FAILURE);
  }

  // g1.randomizeDarts();

  if ( withoriginal)
  {
    std::cout<<"###################### ORIGINAL GMAP ######################\n";
    displayCharacteristics(g1, "Original map");
    //computeHomology(g1, "Homology original map");
  }
  
  std::cout<<"###################### "<<txt<<" ######################\n";
  c.start();
  // Here simplify the map at its maximum
  g1.simplify3DObject(mode);
  c.stop();
  std::string txt2("Map simplified " + std::string(txt));
  displayCharacteristics(g1, txt2.c_str());
  txt2 = std::string("Simplif time ") + std::string(txt);
  c.display(txt2.c_str());
  
  computeHomology(g1, txt);
}

int main(int argc, char** argv)
{
  if ( argc==1 || !strcmp(argv[1],"-?") || !strcmp(argv[1],"-h") )
  {
    cout<<"Usage1 : a.out image (3D png). Pour calculer l'homology de"<<endl
       <<"  l'objet blanc dans l'image."<<endl;
    exit(EXIT_FAILURE);
  }


  process(argv[1], FACE_REMOVAL | EDGE_REMOVAL | VERTEX_REMOVAL |
          EDGE_CONTRACTION | FACE_CONTRACTION | VOLUME_CONTRACTION,
          true, "remov & contract");

  process(argv[1], FACE_REMOVAL | EDGE_REMOVAL | VERTEX_REMOVAL,
          false, "removal only   ");

  //  process(argv[1], EDGE_CONTRACTION | FACE_CONTRACTION | VOLUME_CONTRACTION,
  //          true, "contraction only");

  {
    std::cout<<"###################### DIFFERENT SIMPLIFICATIONS ######################\n";
    CGMapVertex g1;
    CExtractionImage ext1(&g1);
    if ( !ext1.extractOneRegionVoxels(argv[1],0,0,3,0,0,0,0) )
      //if ( !ext1.extractOneRegionVoxels(argv[1],0,0,3,65535,65535,65535,0) )
    {
      cout<<"Problem during extraction of voxels from "<<argv[1]<<endl;
      exit(EXIT_FAILURE);
    }
    g1.simplify3DObject(FACE_REMOVAL);
    displayCharacteristics(g1, "Map after face removal: ");
    g1.simplify3DObject(EDGE_REMOVAL);
    displayCharacteristics(g1, "Map after edge removal: ");
    g1.simplify3DObject(VERTEX_REMOVAL);
    displayCharacteristics(g1, "Map after vertex removal: ");
    g1.simplify3DObject(EDGE_CONTRACTION);
    displayCharacteristics(g1, "Map after edge contraction: ");
    g1.simplify3DObject(FACE_CONTRACTION);
    displayCharacteristics(g1, "Map after face contraction: ");
  }

  return EXIT_SUCCESS;
}
