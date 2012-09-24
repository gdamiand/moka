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
  std::cout<<txt<<": ";
  c.display("Homology computation time");

  std::cout<<"Memory used: "<<h.size()<<" bytes."<<endl;

  // Display Betti numbers
  std::cout<<"Betti numbers Free (0,1,2,3): "<<h.getH0FreeGenerators()
           <<", "<<h.getH1FreeGenerators()<<", "<<h.getH2FreeGenerators()
           <<", "<<h.getH3FreeGenerators()<<endl;
  std::cout<<"Betti numbers Torsion (1,2): "<<h.getH1TorsionGenerators()
           <<", "<<h.getH2TorsionGenerators()<<endl;
}

int main(int argc, char** argv)
{
  if ( argc==1 || !strcmp(argv[1],"-?") || !strcmp(argv[1],"-h") )
  {
    cout<<"Usage1 : a.out image (3D png). Pour calculer l'homology de"<<endl
        <<"  l'objet blanc dans l'image."<<endl;
    exit(EXIT_FAILURE);
  }

  // First we compute the 3G-map of the white voxels.
  CGMapVertex g1, g2, g3, g4;
  CExtractionImage ext1(&g1);
  CExtractionImage ext2(&g2);
  //CExtractionImage ext3(&g3);
  CExtractionImage ext4(&g4);
  if ( !ext1.extractOneRegionVoxels(argv[1],0,0,3,65535,65535,65535,0) ||
       !ext2.extractOneRegionVoxels(argv[1],0,0,3,65535,65535,65535,0) ||
       //!ext3.extractOneRegionVoxels(argv[1],0,0,3,65535,65535,65535,0) ||
       !ext4.extractOneRegionVoxels(argv[1],0,0,3,65535,65535,65535,0) )
  {
    cout<<"Problem during extraction of voxels from "<<argv[1]<<endl;
    exit(EXIT_FAILURE);
  }
  
  // g1.randomizeDarts();
  
  displayCharacteristics(g1, "Map before simplification:");
  // computeHomology(g1, "original map");

  CChrono c;
  c.start();
  // Here simplify the map
  g1.simplify3DObject(FACE_REMOVAL | EDGE_REMOVAL | VERTEX_REMOVAL |
                     EDGE_CONTRACTION | FACE_CONTRACTION |
                     VOLUME_CONTRACTION);
  c.stop();
  c.display("Simplification total time");

  c.reset();
  c.start();
  g2.simplify3DObject(FACE_REMOVAL | EDGE_REMOVAL | VERTEX_REMOVAL);
  c.stop();
  c.display("Simplification removals only time");

  /*c.reset();
  c.start();
  g3.simplify3DObject(EDGE_CONTRACTION | FACE_CONTRACTION |
                      VOLUME_CONTRACTION);
  c.stop();
  c.display("Simplification contractions only time");*/

  // g.save("simplify-map.moka");

  displayCharacteristics(g2, "Map after removals only: ");
  // displayCharacteristics(g3, "Map after contractions only: ");
  displayCharacteristics(g1, "Map after removals and contractions: ");

  computeHomology(g2, "simplif removals only");
  //computeHomology(g3, "simplif contractions only");
  computeHomology(g1, "simplif removal+contractions");

  g4.simplify3DObject(FACE_REMOVAL);
  displayCharacteristics(g4, "Map after face removal: ");
  g4.simplify3DObject(EDGE_REMOVAL);
  displayCharacteristics(g4, "Map after edge removal: ");
  g4.simplify3DObject(VERTEX_REMOVAL);
  displayCharacteristics(g4, "Map after vertex removal: ");
  g4.simplify3DObject(EDGE_CONTRACTION);
  displayCharacteristics(g4, "Map after edge contraction: ");
  g4.simplify3DObject(FACE_CONTRACTION);
  displayCharacteristics(g4, "Map after face contraction: ");

  return EXIT_SUCCESS;
}
