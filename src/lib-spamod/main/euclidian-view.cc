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
 *  Fichier  : Euclidian_View.cpp                                             *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Euclidian_View.                                                            *
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

#include "euclidian-view.hh"
#include "vertex.hh"
#include "attribute-vertex.hh"
#include "vector3d.hh"
#include "color-table.hh"
#include "color-table-att.hh"
#include "gmap-ops.hh"
#include "user.hh"

using namespace std;
using namespace GMap3d;

/******************************************************************************
 *                            Constructeur                                    *
 *****************************************************************************/

Euclidian_View::Euclidian_View(CGMap * GM)
{
 G = GM;
}



/******************************************************************************
 *  Fonction : void EuclidianView::Wire_Draw()                               *
 *----------------------------------------------------------------------------*
 *  Cette fonction parcourt la G-carte en affichant ses arêtes. Si la G-map   *
 * contient des brins seuls (i.e. non reliés à un autre brins), les sommets   *
 * correspondants seront représentés par un point.                            *
 *                                                                            *
 *****************************************************************************/

void Euclidian_View::Wire_Draw()
{
  CAttribute * alpha;
  CVertex * p1, * p2;
  Color_Table * color;

  // Création d'une marque.
  int markEdge = G->getNewMark();


  // Couleurs des éléments de visualisation.
  color = ((Color_Table_Att*)Find_Attribute(G, ORBIT_0123,
				       COLOR_TABLE_ATTRIBUTE_ID))->Get_Data();


  // Parcourt des brins de la G-carte.
  CDynamicCoverageAll C(G);
  for (C.reinit(); C.cont(); C++)
    {
      CDart* dgm = *C;

      if (!G->isMarked(dgm, markEdge))
	{
	  // Coordonnées du sommet.
	  alpha = G->getAttribute(dgm, ORBIT_123, VERTEX_ATTRIBUTE_ID);
	  p1 = (CAttributeVertex*)alpha;
	
	  // Cas où le brin est seul.
	  if (G->isFree0(dgm))
	    {
	      // Affichage du sommet
	      glColor3f(color->Get_Color(0)->Get_R(),
			color->Get_Color(0)->Get_G(),
			color->Get_Color(0)->Get_B());
	      glPointSize(2.0);
	      glBegin(GL_POINTS);
	      glVertex3f(p1->getX(), p1->getY(), p1->getZ());
	      glEnd();
	      glPointSize(1.0);

	      // Marquage du brin.
	      G->setMark(dgm, markEdge);
	    }

	  // Autres cas de G-carte.
	  else	
	    {
	      // Coordonnées de l'extrémité de l'arête.
	      alpha = G->getAttribute(G->alpha0(dgm), ORBIT_123,
				   VERTEX_ATTRIBUTE_ID);
	      p2 = (CAttributeVertex*)alpha;


	      // Partie de code optionnel.
	      /*
	      // Affichage des extrémités.
	      glPointSize(2.0);
	      glColor3f(color->Get_Color(0)->Get_R(),
			color->Get_Color(0)->Get_G(),
			color->Get_Color(0)->Get_B());
	
	      glBegin(GL_POINTS);
	      glVertex3f(p1->getX(), p1->getY(), p1->getZ());
	
	      if (!G->Is_Marked(dgm->Get_Alpha0(), markEdge))
	        glVertex3f(p2->getX(), p2->getY(), p2->getZ());
	
	      glEnd();
	      glPointSize(1.0);
	      */


	      // Affichage de l'arête.
	      glLineWidth(2.0);
	      glBegin(GL_LINES);
	      glColor3f(color->Get_Color(1)->Get_R(),
			color->Get_Color(1)->Get_G(),
			color->Get_Color(1)->Get_B());	      	      	
	      glVertex3f(p1->getX(), p1->getY(), p1->getZ());	
	      glVertex3f(p2->getX(), p2->getY(), p2->getZ());	
	      glEnd();
	      glLineWidth(1.0);
	

	      // Marquage des brins de l'arête affichée.
	      CCoverage* DC = G->getDynamicCoverage(dgm, ORBIT_023);
	      for (DC->reinit();
		   DC->cont();
		   (*DC)++)
		G->setMark(**DC, markEdge);
	      delete DC;
	    }
	}
    }

  // Démarquage des brins et libération de la marque.
  for (C.reinit(); C.cont(); C++)
    G->unsetMark(*C, markEdge);
  G->freeMark(markEdge);

}



/******************************************************************************
 *  Fonction : void EuclidianView::Solid_Draw()                              *
 *----------------------------------------------------------------------------*
 *  Cette fonction parcourt la G-carte en affichant ses faces. Si la G-carte  *
 * contient des chemins, les arêtes sont affichées. De même, les sommets      *
 * correspondants aux brins seuls (i.e. non reliés à un autre brin) seront    *
 * représentés par un point.                                                  *
 *                                                                            *
 *****************************************************************************/

void Euclidian_View::Solid_Draw()
{
  int nbPoints;
  CAttribute * alpha;
  CVertex * p1, * p2;
  Vector3D normal;

  // Création d'une marque.
  int markFace = G->getNewMark();


  // Activation des paramètres de visualisation.
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glShadeModel(GL_SMOOTH);



  // Parcourt des brins de la G-carte.
  CDynamicCoverageAll C(G);
  for (C.reinit(); C.cont(); C++)
    {
      CDart* dgm = *C;

      if (!G->isMarked(dgm, markFace))
	{
	  // Cas où le brin est seul.
	  if (G->isFree0(dgm))
	    {
	      // Coordonnées du sommet.
	      alpha = G->getAttribute(dgm, ORBIT_123, VERTEX_ATTRIBUTE_ID);
	      p1 = (CAttributeVertex*)alpha;
	
	      // Affichage du sommet.
	      glPointSize(2.0);
	      glBegin(GL_POINTS);
	      glVertex3f(p1->getX(), p1->getY(), p1->getZ());
	      glEnd();
	      glPointSize(1.0);
	
	      // Marquage du brin.
	      G->setMark(dgm, markFace);
	    }

	  // Cas où le brin appartient à une ligne polygonale.
	  else
	    if (!G->getAttribute(dgm, ORBIT_013, INT_ATTRIBUTE_ID))
	      {
		// Affichage de la ligne polygonale.
		glLineWidth(2.0);
		glBegin(GL_LINES);

		// Parcourt des brins de la ligne polygonale.
		CCoverage* DC01 = G->getDynamicCoverage(dgm, ORBIT_01);
		for (DC01->reinit();
		     DC01->cont();
		     (*DC01)++)
		  {
		    CDart* d01 = **DC01;
		
		    if (!G->isMarked(d01, markFace))
		      {
			// Coordonnées des extrémités de l'arête.
			alpha = G->getAttribute(d01, ORBIT_123,
					     VERTEX_ATTRIBUTE_ID);
			p1 = (CAttributeVertex*)alpha;
			
			alpha = G->getAttribute(G->alpha0(d01), ORBIT_123,
					     VERTEX_ATTRIBUTE_ID);
			p2 = (CAttributeVertex*)alpha;

			glVertex3f(p1->getX(), p1->getY(), p1->getZ());
			glVertex3f(p2->getX(), p2->getY(), p2->getZ());

			// Marquage des brins de l'arête élémentaire.
			G->setMark(d01, markFace);
			G->setMark(G->alpha0(d01), markFace);
		      }
		  }
		glEnd();
		glLineWidth(1.0);
	
		delete DC01;
	      }

	  // Cas où le brin appartient à une face fermée.
	    else
	      {
		list<CVertex*> ptList;
		nbPoints = 0;
		
		// Parcourt des brins de la face élémentaire.
		CCoverage* DC01 = G->getDynamicCoverage(dgm, ORBIT_01);
		for (DC01->reinit();
		     DC01->cont();
		     (*DC01)++)
		  {
		    CDart* d01 = **DC01;
		
		    if (!G->isMarked(d01, markFace))
		      {		
			// Coordonnées du sommet.
			alpha = G->getAttribute(d01, ORBIT_123,
					     VERTEX_ATTRIBUTE_ID);
			
			// Ajout du sommet dans la liste.
			ptList.push_front((CAttributeVertex*)alpha);
			nbPoints++;
			
			
			// Marquage des brins appartenant au même sommet.
			G->setMark(d01, markFace);
			G->setMark(G->alpha1(d01), markFace);
			
			// Cas où la face est reliée à une autre face par
			// la liaison alpha3.
			if (!G->isFree3(d01))
			  {
			    G->setMark(G->alpha3(d01), markFace);
			    G->setMark(G->alpha3(G->alpha1(d01)),markFace);
			  }
		      }
		  }

		// Le nombre de points est supérieur à 3.
		if (nbPoints >= 3)
		  {
		    // Calcul du vecteur normal à la face.
		    CVertex* tab[3];
		    list<CVertex*>::iterator iter;
		    int i=0;
		    for(iter=ptList.begin();iter!=ptList.end() && i<3; iter++)
		      tab[i++]=*iter;

		    Vector3D v1(*tab[1],*tab[0]);
		
		    Vector3D v2(*tab[2],*tab[1]);
		
		    normal = v1.Vect_Product(v2);
		
		    normal.Normalize();

		    //Activation de l'éclairage.
		    glEnable(GL_LIGHTING);
		
		    // Affichage de la face.
		    glBegin(GL_POLYGON);
		    glNormal3f(normal.getX(),
			       normal.getY(),
			       normal.getZ());
		
		    for (iter=ptList.begin();
			 iter!=ptList.end();
			 iter++)
		      {
			CVertex *curPt = *iter;
			glVertex3f(curPt->getX(),
				   curPt->getY(),
				   curPt->getZ());
		      }
		    glEnd();
		
		    //Désactivation de l'éclairage.
		    glDisable(GL_LIGHTING);
		  }
		
		// Le nombre de points de la face est égal 2.
		else
		  {
		    // Affichage de la face.
		    CVertex* tab[2];
		    list<CVertex*>::iterator iter;
		    int i=0;
		    for(iter=ptList.begin();iter!=ptList.end() && i<2; iter++)
		      tab[i++]=*iter;

		    p1 = tab[0];
		    glLineWidth(2.0);
		    glBegin(GL_LINES);
		    glVertex3f(p1->getX(), p1->getY(), p1->getZ());
		    p1 = tab[1];
		    glVertex3f(p1->getX(), p1->getY(), p1->getZ());
		    glEnd();
		    glLineWidth(1.0);
		  }
		
		delete DC01;
	      }
	}
    }


  // Démarquage des brins et libération de la marque.
  for (C.reinit(); C.cont(); C++)
    G->unsetMark(*C, markFace);
  G->freeMark(markFace);


  //Désactivation des paramètres de visualisation.
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  glShadeModel(GL_FLAT);
}
