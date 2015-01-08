/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 * Copyright (C) 2014, Sebastien Horna, CNRS, Xlim-SIC,
 *               sebastien.horna@univ-poitiers.fr,
 *
 * This file is part of lib-gmapvertex
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
 *
 * Authors : Sébastien Horna and Abdoulaye Diakité (2014)
 *
 */

//******************************************************************************
#ifdef WITH_ASSIMP
//******************************************************************************
#include "g-map-vertex.hh"
#include "time.hh"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

using namespace GMap3d;

//******************************************************************************
CDart *CGMapVertex::importWithAssimp(const char* AFilename)
{
  // Create an instance of the Importer class
  Assimp::Importer importer;  
   
  const aiScene* scene =
    importer.ReadFile( AFilename, aiProcess_PreTransformVertices|
                       aiProcess_JoinIdenticalVertices  );
  
  // If the import failed, return NULL.
  if( !scene) return NULL;    
  
  // Retrieve the geometry node of a model from an input file
  aiMesh** allmeshes = scene->mMeshes;
  uint n_meshes = scene->mNumMeshes;
  uint n_materials = scene->mNumMaterials;
    
  CDart *prec = NULL, *first = NULL;
  // Store the number of vertices of all the sub-meshes of the model
  std::vector<uint> nb_of_vertices;
  
  for(uint m=0; m<n_meshes; m++)
  {
    aiMesh* mesh = allmeshes[m];
    aiVector3D* vertices = mesh->mVertices;
    aiFace* faces = mesh->mFaces;
    
    uint n_vertices = mesh->mNumVertices;
    uint n_faces = mesh->mNumFaces;
    uint n_half_edges = 0;
    nb_of_vertices.push_back(n_vertices);
    
    // Lecture des sommets à charger:
    std::vector< CVertex > initVertices;
    std::vector< std::list<CDart*> > testVertices;
    unsigned long int v1, v2, vf;
    
    unsigned int nbSommets = n_vertices;
    unsigned int nbFaces = n_faces;
    unsigned int doubleNbAretes = 0;
    
    // read in all vertices
    for (uint i = 0; i < n_vertices; i++)
    {
      initVertices.push_back(CVertex(vertices[i].x,
                                     vertices[i].y,
                                     vertices[i].z));
      testVertices.push_back(std::list<CDart*>());
      --nbSommets;
    }
    
    int index = getNewDirectInfo();
    
    for ( unsigned int i = 0; i < n_faces; ++i )
    {
      const unsigned int nbPts = faces[i].mNumIndices;
      prec  = NULL;  first = NULL;
      v1 = faces[i].mIndices[0];
      vf = v1;
      assert(v1 < initVertices.size());
      
      for(unsigned int j  = 1; j < nbPts; ++j)
      {
        v2 = faces[i].mIndices[j];
        assert(v2 < initVertices.size());
        prec = addEdgeOFF(initVertices, v1, v2, index, prec);
        if (first == NULL) first = alpha0(prec);
        v1 = v2;
      }
      
      prec = addEdgeOFF(initVertices, v1, vf, index, prec);
      
      linkAlpha1(first, prec);
      linkAlpha1(alpha3(first), alpha3(prec));
      
      linkFaceAlpha2OFF(testVertices, index, first);
    }
    
    freeDirectInfo(index);
  }
    
  return first;
}
//******************************************************************************
#endif // WITH_ASSIMP
//******************************************************************************
