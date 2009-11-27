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
 *  Fichier  : Topologic_View.h                                               *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Topologic_View.                                                            *
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

#include "user.hh"
#include "topologic-view.hh"
#include "vertex.hh"
#include "vector3d.hh"
#include "attribute-vertex.hh"

using namespace GMap3d;

/******************************************************************************
 *                                 Constructeur                               *
 *****************************************************************************/

Topologic_View::Topologic_View(CGMap * GM)
{
  G = GM;
}



/******************************************************************************
 *  Fonction : void TopologicView::Create_Face_Darts_View(CDart * dgm,        *
 *                                                         int markDart)     *
 *----------------------------------------------------------------------------*
 *  Cette fonction calcule les coordonnées des points représentant les brins  *
 * de la G-carte afin que les liaisons alpha0, alpha1 et alpha2 soient        *
 * visibles.                                                                  *
 *                                                                            *
 *****************************************************************************/

void Topologic_View::Create_Face_Darts_View(CDart * dgm, int markDart)
{
  CAttribute * alpha;
  CDart * de;
  CVertex * p1, * p2, p3, p4;
  Vector3D v;
  int nbPoints = 0;
  CVertex center(0.0,0.0,0.0);

  // Création d'une marque.
  int markCenter = G->getNewMark();


  // Parcourt des brins de l'arête ou de la face.
  CCoverage* DCe = G->getDynamicCoverage(dgm, ORBIT_01);
  for (DCe->reinit(); DCe->cont(); (*DCe)++)
    {
      de = **DCe;

      if (!G->isMarked(de, markCenter))
	{
	  // Coordonnées du sommet.
	  alpha = G->getAttribute(de,ORBIT_123,VERTEX_ATTRIBUTE_ID);
	  p2 = (CAttributeVertex*)alpha;
	
	  // Recherche du barycentre de l'arête ou de la face.
	  center.setX(center.getX() + p2->getX());
	  center.setY(center.getY() + p2->getY());
	  center.setZ(center.getZ() + p2->getZ());
			
	  nbPoints++;

	  // Marquage des brins appartenants au même sommet avec
	  // la marque markCenter.
	  G->setMark(de, markCenter);
	  if (!G->isFree1(de))
	    G->setMark(G->alpha1(de), markCenter);
	}
    }

  // Calcul des coordonées du barycentre de l'arête ou de la face.
  center.setX(center.getX()/(float)nbPoints);
  center.setY(center.getY()/(float)nbPoints);
  center.setZ(center.getZ()/(float)nbPoints);

		
  // Parcourt des brins de l'arête ou de la face.
  for (DCe->reinit();
       DCe->cont();
       (*DCe)++)
    {
      de = **DCe;

      // Démarquage du brin pour la marque markCenter.
      G->unsetMark(de, markCenter);

      if (!G->isMarked(de, markDart))
	{
	  // Coordonnées du sommet correspondant au brin "de".
	  alpha = G->getAttribute(de, ORBIT_123, VERTEX_ATTRIBUTE_ID);
	  p1 = (CAttributeVertex*)alpha;
	
	  // Coordonnées du sommet correspondant à l'image du
	  // brin "de" par la relation alpha0.
	  alpha = G->getAttribute(G->alpha0(de),ORBIT_123,VERTEX_ATTRIBUTE_ID);
	  p2 = (CAttributeVertex*)alpha;
			
	  // Calcul des coordonnées des sommets qui seront
	  // utilisés pour l'affichage.
	  v.setXYZ(p2->getX()-p1->getX(),
		    p2->getY()-p1->getY(),
		    p2->getZ()-p1->getZ());
	
	  // Cas où le brin n'est pas relié à autre brin par
	  // alpha1.
	  if (G->isFree1(de))
	    p3 = *p1;
	
	  // Cas où le brin est relié à un autre brin par alpha1.
	  else
	    {
	      p3.setXYZ(p1->getX() + v.getX()/10.0,
			 p1->getY() + v.getY()/10.0,
			 p1->getZ() + v.getZ()/10.0);
	    }
	
	  // Cas où l'image du brin par alpha0 n'est pas reliée
	  // à autre brin par alpha1.
	  if (G->isFree1(G->alpha0(de)))
	    p4 = *p2;
	
	  // Cas où l'image du brin par alpha0 est reliée à un
	  // autre brin par alpha1.
	  else
	    {
	      p4.setXYZ(p2->getX() - v.getX()/10.0,
			 p2->getY() - v.getY()/10.0,
			 p2->getZ() - v.getZ()/10.0);
	    }
	
	  // Cas où le brin est relié à un autre brin par la
	  // relation alpha2.
	  if (!G->isFree2(de))
	    {
	      // Calcul des coordonnées des sommets qui seront
	      // utilisés pour l'affichage.
	      v.setXYZ(center.getX()-p3.getX(),
			center.getY()-p3.getY(),
			center.getZ()-p3.getZ());
	      p3.setXYZ(p3.getX() + v.getX()/5.0,
			 p3.getY() + v.getY()/5.0,
			 p3.getZ() + v.getZ()/5.0);
	
	      v.setXYZ(center.getX()-p4.getX(),
			center.getY()-p4.getY(),
			center.getZ()-p4.getZ());
	      p4.setXYZ(p4.getX() + v.getX()/5.0,
			 p4.getY() + v.getY()/5.0,
			 p4.getZ() + v.getZ()/5.0);
	    }
	
	  // Affectation de l'attribut à l'orbite Orbit_SELF.
	  G->addAttribute(de,ORBIT_SELF,new CAttributeVertex(p3));
	  	
	  // Marquage du brin "de" avec la marque markDart.
	  G->setMark(de, markDart);
	
	
	  // Affectation de l'attribut à l'orbite Orbit_SELF.
	  G->addAttribute(G->alpha0(de),ORBIT_SELF,
			   new CAttributeVertex(p4));
	
	  // Marquage de l'image du brin "de" par la relation
	  // alpha0 avec la marque markDart.
	  G->setMark(G->alpha0(de), markDart);	
	}		
    }
  delete DCe;

  // Libération de la marque.
  G->freeMark(markCenter);
}



/******************************************************************************
 *  Fonction : void TopologicView::Create_Volume_Darts_View(CDart * dgm,      *
 *                                                           int markVolume) *
 *----------------------------------------------------------------------------*
 *  Cette fonction calcule les coordonnées des points représentant les brins  *
 * de la G-carte afin que les liaisons alpha3 soient visibles.                *
 *                                                                            *
 *****************************************************************************/

void Topologic_View::Create_Volume_Darts_View(CDart * dgm, int markVolume)
{
  CAttribute * alpha;
  CDart * de;
  CVertex * p1;
  Vector3D v;
  int nbPoints = 0;
  CVertex center(0.0,0.0,0.0);

  // Création d'une marque.
  int markCenter = G->getNewMark();


  // Parcourt des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      dgm = *Cgm;

      if (!G->isMarked(dgm, markVolume))
	{
	  // Cas où le brin est relié par alpha3 à un autre brin.
	  if (!G->isFree3(dgm))
	    {	
	      // Parcourt des brins du volume.
	      CCoverage* DCe = G->getDynamicCoverage(dgm, ORBIT_012);
	      for (DCe->reinit();
		   DCe->cont();
		   (*DCe)++)
		{
		  de = **DCe;
	
		  if (!G->isMarked(de, markCenter))
		    {
		      // Coordonnées du sommet.
		      alpha = G->getAttribute(de,ORBIT_123,VERTEX_ATTRIBUTE_ID);
		      p1 = (CAttributeVertex*)alpha;

		      // Recherche du barycentre du volume.
		      center.setX(center.getX() + p1->getX());
		      center.setY(center.getY() + p1->getY());
		      center.setZ(center.getZ() + p1->getZ());
		
		      nbPoints++;
		
		      // Marquage des brins appartenants au même sommet avec
		      // la marque markCenter.
		      G->setMark(de, markCenter);
		      if (!G->isFree1(de))
			G->setMark(G->alpha1(de), markCenter);
		    }		
		}
	
	      // Calcul du barycentre du volume.
	      center.setX(center.getX()/(float)nbPoints);
	      center.setY(center.getY()/(float)nbPoints);
	      center.setZ(center.getZ()/(float)nbPoints);
	

	      // Parcourt des brins du volume.
	      for (DCe->reinit();
		   DCe->cont();
		   (*DCe)++)
		{
		  de = **DCe;	

		  // Démarquage du brin pour la marque markCenter.
		  G->unsetMark(G->alpha0(de), markCenter);
		
		  if (!G->isMarked(de, markVolume))
		    {
		      // cas où le brin est relié à un autre brin par la
		      // liaison alpha3.
		      if (!G->isFree3(de))
			{
			  // Coordonnées du sommet.
			  alpha = G->getAttribute(de, ORBIT_SELF,
					     VERTEX_ATTRIBUTE_ID);
			  p1 = (CAttributeVertex*)alpha;

			  // Calcul des coordonnées des sommets qui seront
			  // utilisé pour l'affichage.
			  v.setXYZ(center.getX()-p1->getX(),
				    center.getY()-p1->getY(),
				    center.getZ()-p1->getZ());

			  p1->setXYZ(p1->getX() + v.getX()/5.0,
				      p1->getY() + v.getY()/5.0,
				      p1->getZ() + v.getZ()/5.0);
			}
		
		      // Marquage du brin avec la marque markVolume.
		      G->setMark(de, markVolume);			
		    }
		}
	      delete DCe;
	    }

	  // Cas où le brin n'est pas relié par alpha3 à un autre brin.
	  else
	    G->setMark(dgm, markVolume);
	}
    }

  // Libération de la marque.
  G->freeMark(markCenter);
}



/******************************************************************************
 *  Fonction : void TopologicView::Create_Darts_View()                       *
 *----------------------------------------------------------------------------*
 *  Cette fonction parcourt la G-carte en calculant et en stockant, pour      *
 * chaque brin, les coordonnées du sommet qui sera utilisé pour l'affichage   *
 * topologique.                                                               *
 *                                                                            *
 *****************************************************************************/

void Topologic_View::Create_Darts_View()
{
  CDart * dgm = NULL;
  CAttribute * alpha;
  CVertex * p1;

  // Création de 2 marques.
  int markDart = G->getNewMark();
  int markVolume = G->getNewMark();


  // Parcourt des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      dgm = *Cgm;

      if (!G->isMarked(dgm, markDart))
	{
	  // Cas où le brin est seul.
	  if (G->isFree0(dgm))
	    {
	      // Coordonnées du sommet.
	      alpha = G->getAttribute(dgm, ORBIT_123, VERTEX_ATTRIBUTE_ID);
	      p1 = (CAttributeVertex*)alpha;

	      // Affectation de l'attribut à l'orbite Orbit_SELF.
	      G->addAttribute(dgm, ORBIT_SELF, new CAttributeVertex(*p1));
	   	
	
	      // Marquage du brin avec la marque markDart.
	      G->setMark(dgm, markDart);
	    }
	
	  // Cas où le brin appartient à une arête ou à une face (ouverte ou
	  // fermée).
	  else
	    // Calcul des coordonnées des points appartenants à une arête ou
	    // une face.
	    Create_Face_Darts_View(dgm, markDart);
	}
    }

  // Calcul des coordonnées des points appartenants à un volume.
  Create_Volume_Darts_View(dgm, markVolume);


  // Démarquage des brins et libération des 2 marques.
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      G->unsetMark(*Cgm, markDart);	
      G->unsetMark(*Cgm, markVolume);
    }
  G->freeMark(markDart);
  G->freeMark(markVolume);


}



/******************************************************************************
 *  Fonction : void TopologicView::Darts_Draw()                              *
 *----------------------------------------------------------------------------*
 *  Cette fonction parcourt la G-carte en affichant les sommets correspondants*
 * aux différents brins.                                                      *
 *                                                                            *
 *****************************************************************************/

void Topologic_View::Darts_Draw()
{
  CDart * dgm;
  CAttribute * alpha;
  CVertex * p;

  // Parcourt des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      dgm = *Cgm;

      // Coordonnées du sommet à visualiser.
      alpha = G->getAttribute(dgm, ORBIT_SELF, VERTEX_ATTRIBUTE_ID);
      p =(CAttributeVertex*)alpha;

      // Affichage du sommet.
      glBegin(GL_POINTS);
      //glColor3f(1.0, 1.0, 0.0);
      glVertex3f(p->getX(), p->getY(), p->getZ());
      glEnd();
    }

}



/******************************************************************************
 *  Fonction : void TopologicView::Alpha_Draw(int i)                         *
 *----------------------------------------------------------------------------*
 *  Cette fonction parcourt la G-carte en affichant les liaisons alpha i      *
 * existantes entre les différents brins.                                     *
 *                                                                            *
 *****************************************************************************/

void Topologic_View::Alpha_Draw(int i)
{
  CDart * dgm;
  CAttribute * alpha;
  CVertex * p,* p1;

  // Création d'une marque.
  int markDart = G->getNewMark();

  // Parcourt des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      dgm = *Cgm;

      if (!G->isMarked(dgm, markDart))
	{
	  // Cas où le brin est relié à un autre brin par la relation alpha0
	  // et où cet autre brin n'alpha pas déjà été traité.
	  if (!G->isFree(dgm,i) &&
	      !G->isMarked(G->alpha(dgm,i), markDart))
	    {
	      // Coordonnées du sommet correspondant au brin "dgm".
	      alpha = G->getAttribute(dgm, ORBIT_SELF, VERTEX_ATTRIBUTE_ID);
	      p =(CAttributeVertex*)alpha;
	
	      // Coordonnées du sommet correspondant à l'autre brin.
	      alpha = G->getAttribute(G->alpha(dgm,i), ORBIT_SELF,
				   VERTEX_ATTRIBUTE_ID);
	      p1 = (CAttributeVertex*)alpha;
	
	      // Affichage de la liaison alpha0.
	      //glColor3f(1.0, 0.0, 0.0);
	      glBegin(GL_LINES);
	      glVertex3f(p->getX(), p->getY(), p->getZ());
	      glVertex3f(p1->getX(), p1->getY(), p1->getZ());
	      glEnd();

	      // Marquage du brin avec la marque markDart.
	      G->setMark(G->alpha(dgm,i), markDart);
	    }

	  // Marquage du brin avec la marque markDart.
	  G->setMark(dgm, markDart);
	}
    }

  // Démarquage des brins et libération de la marque.
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markDart);
  G->freeMark(markDart);

}
