/*
 * lib-spamod : Visualisation des objets en discret.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-spamod
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

/******************************************************************************
 *  Fichier  : Voxel_View.cpp                                                 *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe Voxel_View.*
 *                                                                            *
 *****************************************************************************/

#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "voxel-view.hh"
#include "ineq-op.hh"
#include "definition.hh"
#include "matrix-att.hh"
#include "voxel-list.hh"
#include "ineq-table-6.hh"
#include "ineq-table-2.hh"
#include "ineq-table-2-att.hh"
#include "ineq-table-6-att.hh"
#include "ineq-use-att.hh"
#include "ineq-use.hh"

using namespace std;
using namespace GMap3d;

/******************************************************************************
 *                            Constructeur                                    *
 *****************************************************************************/

Voxel_View::Voxel_View(CGMap * GM)
{
  G = GM;
}


/******************************************************************************
 *  Fonction : void VoxelView::Calculate_Int_Bounding_Box(                   *
 *                                            float gmapBoundingBox[6],     *
 *					      int matrixBoundingBox[6])     *
 *----------------------------------------------------------------------------*
 *  Cette fonction récupère le tableau des bornes de la boite englobante de   *
 * l'objet, arrondit les bornes non entières supérieures à l'entier supérieur *
 * et arrondit les bornes non entières inférieures à l'entier inférieur.      *
 *                                                                            *
 *****************************************************************************/

void Voxel_View::Calculate_Int_Bounding_Box(float gmapBoundingBox[6],
					    int matrixBoundingBox[6])
{
  float up, down;

  // Pour chaque borne de la boite englobante.
  for (int i=0 ; i<5 ; i+=2)
    {

      // Calcul de l'arrondit entier des bornes supérieures.
      // X, Y, et Z maximums
      up = ceilf(gmapBoundingBox[i]);
      down = floorf(gmapBoundingBox[i]);

      if (fabs(up - gmapBoundingBox[i])<EPS)
	matrixBoundingBox[i] = (int)up;

      else
	matrixBoundingBox[i] = (int)down;

      // Calcul de l'arrondit entier des bornes inférieures.
      // X, Y et Z minimums
      up = ceilf(-gmapBoundingBox[i+1]);
      down = floorf(-gmapBoundingBox[i+1]);

      if (fabs(down + gmapBoundingBox[i+1])<EPS)
	matrixBoundingBox[i+1] = (int)down;

      else
	matrixBoundingBox[i+1] = (int)up;
    }
}


/******************************************************************************
 *  Fonction : void VoxelView::Create_Matrix()                               *
 *----------------------------------------------------------------------------*
 *  Cette fonction recherche les points entiers appartenant à la              *
 * supercouverture de l'objet et met à jour la matrice principale             *
 *                                                                            *
 *****************************************************************************/

void Voxel_View::Create_Matrix()
{
  CAttribute * alpha;
  Matrix_Att * matrix;
  Matrix * mat, * tempMat;
  CDart * d01, * dO, * d;
  Ineq_Table2 * tf;
  Ineq_Table6 * te;
  Ineq_Use * ue;

  float gmapBoundingBox[6];
  int matrixBoundingBox[6];

  // Création d'une marque.
  int markVoxel = G->getNewMark();

  // Parcours des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);

  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      d = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d, markVoxel))
	{
	  // Calcul de la boite englobante de l'objet afin de définir la
	  // taille de la matrice.
	  Calculate_Bounding_Box(G, d, ORBIT_0123, gmapBoundingBox);
	
	  // Calcul des bornes entières de la boite englobante de l'objet.
	  Calculate_Int_Bounding_Box(gmapBoundingBox, matrixBoundingBox);
	
	  // Attribut contenant la matrice.
	  matrix = new Matrix_Att(matrixBoundingBox, NO_COLOR);
	  mat = (Matrix*)matrix->Get_Data();
	
	  // Parcours des brins de la G-carte.
	  CCoverage* DC = G->getDynamicCoverage(d, ORBIT_0123);
	
	  // Recherche des voxels appartenant aux faces.
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    {
	      dO = **DC;
	
	      // Si le brin n'est pas marqué.
	      if (!G->isMarked(dO, markVoxel))
		{
		  // Le brin appartient à une face.
		  if (G->getAttribute(dO,ORBIT_013, INT_ATTRIBUTE_ID))
		    {
		      // Calcul de la boîte englobante de la face.
		      Calculate_Bounding_Box(G, dO, ORBIT_01,
					     gmapBoundingBox);
		
		      // Calcul des bornes entières de la boite englobante
		      // de la face.
		      Calculate_Int_Bounding_Box(gmapBoundingBox,
						 matrixBoundingBox);	
		
		      // Matrice temporaire utilisée pour la face traitée.
		      tempMat = new Matrix(matrixBoundingBox, FACE_COLOR);
	      	
		      // Inéquations de la face.
		      alpha = G->getAttribute(dO, ORBIT_013,
					   INEQ2_ATTRIBUTE_ID);
		      tf = ((Ineq_Table2_Att*)alpha)->Get_Data();
	      	
		      // Recherche des point entiers verifiant les inéquations
		      // de la face.
		      for (int t=0; t<2; t++)
			
			for (int i = matrixBoundingBox[1];
			     i <= matrixBoundingBox[0]; i++)
			  for (int j = matrixBoundingBox[3];
			       j <= matrixBoundingBox[2]; j++)
			    for (int k = matrixBoundingBox[5];
				 k <= matrixBoundingBox[4]; k++)
			      {
				if (!tf->Get_Ineq(t)
				    ->Test_Point(CVertex(i,j,k)))
				  tempMat->Set_Color(i, j, k, NO_COLOR);
			      }
		
		      // Parcourt des brins de la face.
		      CCoverage* DCf = G->getDynamicCoverage(dO, ORBIT_01);
		      for (DCf->reinit();
			   DCf->cont();
			   (*DCf)++)
			{
			  d01 = **DCf;
			
			  // Si le brin n'est pas marqué.
			  if (!G->isMarked(d01, markVoxel))
			    { 		
			      // Inéquations de l'arête.
			      alpha = G->getAttribute(d01, ORBIT_023,
						   INEQ6_ATTRIBUTE_ID);
			      te = ((Ineq_Table6_Att*)alpha)->Get_Data();
			
			      // Visibilité des inéquations de l'arête par
			      // rapport à la face.
			      alpha = G->getAttribute(d01, ORBIT_0,
						   USE_ATTRIBUTE_ID);
			      ue = ((Ineq_Use_Att*)alpha)->Get_Data();
			
			      // Recherche des centres des voxels traversés
			      //par l'arête.
			      for (int t=0; t<6; t++)
				
				if(ue->Get_Use(t))
				  {
				    for (int i = matrixBoundingBox[1];
					 i <= matrixBoundingBox[0]; i++)
				      for (int j = matrixBoundingBox[3];
					   j <= matrixBoundingBox[2]; j++)
					for (int k = matrixBoundingBox[5];
					     k <= matrixBoundingBox[4]; k++)
					  {
					    if (!te->Get_Ineq(t)
						->Test_Point(CVertex(i,j,k)))
					      tempMat->Set_Color(i, j, k,
								  NO_COLOR);
					  }
				  }			
			      G->setMark(d01, markVoxel);
			      G->setMark(G->alpha0(d01), markVoxel);
			    }
			}
		      delete DCf;
		
		
		      // Liste de voxels de la face.
		      Voxel_List* vList = new Voxel_List();
		      list<Voxel*> *l = (list<Voxel*>*)vList->Get_Data();
		
		      // Parcours de la matrice pour créer la liste de voxels
		      // de la face.
		      for (int i = matrixBoundingBox[1];
			   i <= matrixBoundingBox[0]; i++)
			for (int j = matrixBoundingBox[3];
			     j <= matrixBoundingBox[2]; j++)
			  for (int k = matrixBoundingBox[5];
			       k <= matrixBoundingBox[4]; k++)
			    {
			      if (tempMat->Get_Color(i, j, k) == FACE_COLOR)
				{
				  l->push_front(new Voxel(i, j, k));
				  mat->Set_Color(i, j, k, FACE_COLOR);
				}
			    }
		      if(!G->getAttribute(dO,ORBIT_013,PT_LIST_ATTRIBUTE_ID))
			G->addAttribute(dO, ORBIT_013, vList);
		
		      delete tempMat;
		    }	  	
		  else
		    G->setMark(dO, markVoxel);
		}
	    }
	
	  // Demarquage des brins de la G-carte.
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    G->unsetMark(**DC, markVoxel);
	
	  // Recherche des point entiers correspondant aux aretes.
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    {
	      dO = **DC;
	      if (!G->isMarked(dO, markVoxel))
		{
		  if (!G->isFree0(dO))
		    {
		      // Calcul de la boite englobante de l'arête.
		      Calculate_Bounding_Box(G, dO, ORBIT_0,
					     gmapBoundingBox);
		
		      // Calcul des bornes entières de la boite englobante.
		      Calculate_Int_Bounding_Box(gmapBoundingBox,
						 matrixBoundingBox);
		
		      // Matrice temporaire pour l'arête traitee.
		      tempMat = new Matrix(matrixBoundingBox, EDGE_COLOR);
		
		      // Inéquations de l'arête.
		      alpha = G->getAttribute(dO, ORBIT_023,
					   INEQ6_ATTRIBUTE_ID);
		      te = ((Ineq_Table6_Att*)alpha)->Get_Data();
		
		      // Pour chaque inéquation.
		      for (int t=0; t<6; t++)
			
			// Mise à jour de la matrice.
			for (int i=matrixBoundingBox[1];
			     i<=matrixBoundingBox[0]; i++)
			
			  for (int j=matrixBoundingBox[3];
			       j<=matrixBoundingBox[2]; j++)
			
			    for (int k=matrixBoundingBox[5];
				 k<=matrixBoundingBox[4]; k++)
			      {
				if (!te->Get_Ineq(t)
				    ->Test_Point(CVertex(i,j,k)))
				  tempMat->Set_Color(i, j, k, NO_COLOR);
			      }
		
		      // Marquage des brins de l arete.
		      CCoverage* DCe = G->getDynamicCoverage(dO, ORBIT_023);
		      for (DCe->reinit();
			   DCe->cont();
			   (*DCe)++)
			G->setMark(**DCe, markVoxel);
		      delete DCe;
		
		      // Liste de voxels de l'arete.
		      Voxel_List* vList = new Voxel_List();
		      list<Voxel*> *l = (list<Voxel*>*)vList->Get_Data();
		
		
		      // Parcours de la matrice pour créer la liste de voxels
		      // de l'arete.
		      for (int i=matrixBoundingBox[1];
			   i<=matrixBoundingBox[0]; i++)
			
			for (int j=matrixBoundingBox[3];
			     j<=matrixBoundingBox[2]; j++)
			
			  for (int k=matrixBoundingBox[5];
			       k<=matrixBoundingBox[4]; k++)
			    {
			      if (tempMat->Get_Color(i, j, k) == EDGE_COLOR)
				{
				  l->push_front(new Voxel(i, j, k));
				  mat->Set_Color(i, j, k, EDGE_COLOR);
				}
			    }		
		      G->addAttribute(dO, ORBIT_023, vList);
		      delete tempMat;
		    }
		
		  // Le brin est seul.
		  else
		    G->setMark(dO, markVoxel);
		}
	    }
	  // Demarquage des brins de la G-carte.
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    G->unsetMark(**DC, markVoxel);
	
	
	  // Recherche des voxels correspondant aux sommets.
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    {
	      dO = **DC;
	
	      // Si le brin n'est pas marqué.
	      if (!G->isMarked(dO, markVoxel))
		{
		  // Inéquations du sommet.
		  alpha = G->getAttribute(dO, ORBIT_123, INEQ6_ATTRIBUTE_ID);
		  te = ((Ineq_Table6_Att*)alpha)->Get_Data();
		
		  // Boite engllobante du sommet.
		  gmapBoundingBox[0]=te->Get_Ineq(0)->GetW();
		  gmapBoundingBox[1]=te->Get_Ineq(1)->GetW();
		  gmapBoundingBox[2]=te->Get_Ineq(2)->GetW();
		  gmapBoundingBox[3]=te->Get_Ineq(3)->GetW();
		  gmapBoundingBox[4]=te->Get_Ineq(4)->GetW();
		  gmapBoundingBox[5]=te->Get_Ineq(5)->GetW();
		
		  // Calcul des bornes entières de la boite englobante.
		  Calculate_Int_Bounding_Box(gmapBoundingBox,
					     matrixBoundingBox);
		
		  // Matrice temporaire pour le sommet traité.
		  tempMat = new Matrix(matrixBoundingBox, VERTEX_COLOR);
		
		  // Pour chaque inéquation.
		  for (int t=0; t<6; t++)
		
		    // Mise à jour de la matrice.
		    for (int i=matrixBoundingBox[1];
			 i<=matrixBoundingBox[0]; i++)
		
		      for (int j=matrixBoundingBox[3];
			   j<=matrixBoundingBox[2]; j++)
			
			for (int k=matrixBoundingBox[5];
			     k<=matrixBoundingBox[4]; k++)
			  {
			    if (!te->Get_Ineq(t)->Test_Point(CVertex(i,j,k)))
			      tempMat->Set_Color(i, j, k, NO_COLOR);
			  }
		
		  // Marquage des brins du sommet.
		  CCoverage* DCe = G->getDynamicCoverage(dO, ORBIT_123);
		  for(DCe->reinit();
		      DCe->cont();
		      (*DCe)++)
		    G->setMark(**DCe, markVoxel);
		  delete DCe;
		
		  // Liste de voxels du sommet.
		  Voxel_List* vList = new Voxel_List();
		  list<Voxel*> *l = (list<Voxel*>*)vList->Get_Data();
		
		  // Parcours de la matrice pour créer la liste de voxels
		  // du sommet.
		  for (int i=matrixBoundingBox[1];
		       i<=matrixBoundingBox[0]; i++)
		
		    for (int j=matrixBoundingBox[3];
			 j<=matrixBoundingBox[2]; j++)
		
		      for (int k=matrixBoundingBox[5];
			   k<=matrixBoundingBox[4]; k++)
			{
			  if (tempMat->Get_Color(i, j, k) == VERTEX_COLOR)
			    {
			      l->push_front(new Voxel(i, j, k));
			      mat->Set_Color(i, j, k, VERTEX_COLOR);
			    }
			}
		
		  G->addAttribute(dO, ORBIT_123, vList);
		  delete tempMat;
		}
	    }
	
	  G->addAttribute(d, ORBIT_0123, matrix);
	  //Demarquage et libération de la marque.
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    G->unsetMark(**DC, markVoxel);
	
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    G->setMark(**DC, markVoxel);
	  delete DC;
	}
    }
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markVoxel);

  G->freeMark(markVoxel);
}


/******************************************************************************
 *  Fonction : void VoxelView::Draw_Vertices(unsigned int cube)               *
 *----------------------------------------------------------------------------*
 *  Cette fonction affiche les voxels des sommets de l'objet.                 *
 *                                                                            *
 *****************************************************************************/

void Voxel_View::Draw_Vertices()
{
  CDart * d;
  int markVoxel = G->getNewMark();

  // Activation de l'éclairage.
  glEnable(GL_LIGHTING);

  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      d = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d, markVoxel))
	{
	  // Matrice principale.
	  CAttribute *alpha = G->getAttribute(d, ORBIT_0123, MATRIX_ATTRIBUTE_ID);
	  Matrix * mat = ((Matrix_Att*)alpha)->Get_Data();
	  	 	
	  // Parcours de la matrice et affichage des voxels.
	  for (int i=mat->Get_X_Min() ; i<=mat->Get_X_Max() ; i++)
	
	    for (int j=mat->Get_Y_Min() ; j<=mat->Get_Y_Max() ; j++)
	
	      for (int k=mat->Get_Z_Min() ; k<=mat->Get_Z_Max() ; k++)
		
		if (mat->Get_Color(i,j,k) == VERTEX_COLOR)
		  {
		    glPushMatrix();
		    glTranslatef(i,j,k);	
		    glutSolidCube(0.9);
		    glPopMatrix();
		  }
	
	
	  CCoverage* DC = G->getDynamicCoverage(d, ORBIT_0123);
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    G->setMark(**DC, markVoxel);
	  delete DC;
	}
    }
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markVoxel);

  G->freeMark(markVoxel);

  // Désactivation de l'éclairage.
  glDisable(GL_LIGHTING);
}


/******************************************************************************
 *  Fonction : void VoxelView::Draw_Edges(unsigned int cube)                 *
 *----------------------------------------------------------------------------*
 *  Cette fonction affiche les voxels des arêtes de l'objet.                  *
 *                                                                            *
 *****************************************************************************/

void Voxel_View::Draw_Edges()
{
  CDart * d;
  int markVoxel = G->getNewMark();

  // Activation de l'éclairage.
  glEnable(GL_LIGHTING);

  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      d = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d, markVoxel))
	{	
	  // Matrice principale.
	  CAttribute *alpha = G->getAttribute(d, ORBIT_0123, MATRIX_ATTRIBUTE_ID);
	  Matrix * mat = ((Matrix_Att*)alpha)->Get_Data();
	  	
	  // Parcours de la matrice et affichage des voxels.
	  for (int i=mat->Get_X_Min() ; i<=mat->Get_X_Max() ; i++)
	
	    for (int j=mat->Get_Y_Min() ; j<=mat->Get_Y_Max() ; j++)
	
	      for (int k=mat->Get_Z_Min() ; k<=mat->Get_Z_Max() ; k++)
		
		if (mat->Get_Color(i,j,k) == EDGE_COLOR)
		  {
		    glPushMatrix();
		    glTranslatef(i,j,k);	
		    glutSolidCube(0.9);
		    glPopMatrix();
		  }
	  CCoverage* DC = G->getDynamicCoverage(d, ORBIT_0123);
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    G->setMark(**DC, markVoxel);
	  delete DC;
	}
    }
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markVoxel);

  G->freeMark(markVoxel);

  // Désactivation de l'éclairage.
  glDisable(GL_LIGHTING);
}

		
/******************************************************************************
 *  Fonction : void VoxelView::Draw_Faces(unsigned int cube)                 *
 *----------------------------------------------------------------------------*
 *  Cette fonction affiche les voxels des faces de l'objet.                   *
 *                                                                            *
 *****************************************************************************/

void Voxel_View::Draw_Faces()
{
  CDart * d;
  int markVoxel = G->getNewMark();

  // Activation de l(éclairage.
  glEnable(GL_LIGHTING);

  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      d = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d, markVoxel))
	{

	  // Matrice principale.
	  CAttribute *alpha = G->getAttribute(d, ORBIT_0123, MATRIX_ATTRIBUTE_ID);
	  Matrix * mat = ((Matrix_Att*)alpha)->Get_Data();
	  	
	  // Parcours de la matrice et affichage des voxels.
	  for (int i=mat->Get_X_Min() ; i<=mat->Get_X_Max() ; i++)
	
	    for (int j=mat->Get_Y_Min() ; j<=mat->Get_Y_Max() ; j++)
	
	      for (int k=mat->Get_Z_Min() ; k<=mat->Get_Z_Max() ; k++)
		
		if (mat->Get_Color(i,j,k) == FACE_COLOR)
		  {
		    glPushMatrix();
		    glTranslatef(i,j,k);	
		    glutSolidCube(0.9);
		    glPopMatrix();
		  }
	  CCoverage* DC = G->getDynamicCoverage(d, ORBIT_0123);
	  for (DC->reinit(); DC->cont(); (*DC)++)
	    G->setMark(**DC, markVoxel);
	  delete DC;
	}
    }
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markVoxel);

  G->freeMark(markVoxel);

  // Désactivation de l'éclairage.
  glDisable(GL_LIGHTING);
}
