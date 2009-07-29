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
 *  Fichier  : Gmap_Ops.cpp                                                   *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient des routines permettant d'effectuer quelques          *
 * opérations sur les G-cartes.                                               *
 *                                                                            *
 *****************************************************************************/


#include "gmap-ops.hh"
#include "definition.hh"
#include "int-att.hh"
#include "attribute-vertex.hh"
#include "color-table-att.hh"
#include <iostream>
#include <fstream>
//******************************************************************************
namespace GMap3d
{

/******************************************************************************
 *  Fonction : Point3D Calculate_Center(Gmap * G)                             *
 *----------------------------------------------------------------------------*
 *  Cette fonction parcourt la G-carte en récupérant les coordonnées des      *
 * points contenus dans les sommets de la G-carte. Ensuite, elle calcule le   *
 * centre de ces coordonnées.                                                 *
 *                                                                            *
 *****************************************************************************/

CVertex Calculate_Center(CGMap * G)
{
  CVertex result, min, max;

  bool first = true;

  // Création d'une marque.
  int markVertex = G->getNewMark();

  // Parcours des brins de la GCarte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      CDart* dgm = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(dgm, markVertex))
	{
	  // Récupération du point associé au sommet contenant dgm.
	  CAttribute *alpha = G->getAttribute(dgm,ORBIT_123,VERTEX_ATTRIBUTE_ID);
	  CVertex *p = (CAttributeVertex*)alpha;
	
	  if (first)
	    {
	      min = max = *p;
	      first = false;
	    }
	  else
	    {
	      if (p->getX() > max.getX())
		max.setX(p->getX());
	      else if (p->getX() < min.getX())
		min.setX(p->getX());

	      if (p->getY() > max.getY())
		max.setY(p->getY());
	      else if (p->getY() < min.getY())
		min.setY(p->getY());

	      if (p->getZ() > max.getZ())
		max.setZ(p->getZ());
	      else if (p->getZ() < min.getZ())
		min.setZ(p->getZ());
	    }

	  // Marquage des brins du sommet.
	  CCoverage* DCv = G->getDynamicCoverage(dgm, ORBIT_123);
	  for (DCv->reinit();
	       DCv->cont();
	       (*DCv)++)
	    G->setMark(**DCv, markVertex);
	  delete DCv;
	}
    }

  // Démarquage et libération de la marque.
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
      G->unsetMark(*Cgm, markVertex);
  G->freeMark(markVertex);

  // Calcul du centre.
  result.setX((min.getX()+max.getX())/2.0);
  result.setY((min.getY()+max.getY())/2.0);
  result.setZ((min.getZ()+max.getZ())/2.0);

  return result;
}


/******************************************************************************
 *  Fonction : void Init_Gmap(CGMap * G)                                       *
 *----------------------------------------------------------------------------*
 *  Cette fonction initialise la G-carte en ajoutant le nombre de points      *
 * des faces dans leur orbite et en ajoutant un tableau de couleur à la       *
 * G-carte.                                                                   *
 *                                                                            *
 *****************************************************************************/

void Init_Gmap(CGMap * G)
{
  CDart *dgm, *d01;

  bool isFace;
  int nbDarts;

  // Création d'une marque.
  int markFace = G->getNewMark();

  // Récupération du tableau de couleurs dans la G-carte.
  if (Find_Attribute(G, ORBIT_0123, COLOR_TABLE_ATTRIBUTE_ID) == NULL)
    {
      G->addAttribute(G->getFirstDart(), ORBIT_0123,
		      new Color_Table_Att(Color(1.0, 1.0, 0.0),
					  Color(0.0, 0.0, 1.0),
					  Color(0.0, 1.0, 0.0),
					  Color(0.8, 0.8, 0.8)));
    }

  // Parcours des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      dgm = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(dgm, markFace))
	{
	  // Le brin est seul.
	  if (G->isFree0(dgm))
	    G->setMark(dgm, markFace);
	  	
	  // Le brin n'est pas seul.
	  else
	    {
	      // Le nombre de points de la face est déjà stoqué dans l'orbite.
	      if (G->getAttribute(dgm, ORBIT_013, INT_ATTRIBUTE_ID))
		{
		  // Marquage des brins de la face.
		  CCoverage* DCf = G->getDynamicCoverage(dgm, ORBIT_01);
		  for (DCf->reinit();
		       DCf->cont();
		       (*DCf)++)
		    G->setMark(**DCf, markFace);

		  delete DCf;
		}
	      else
		{
		  isFace = true;
		  nbDarts = 0;
		
		  // Parcours de l'orbite ORBIT_01.
		  CCoverage* DCf = G->getDynamicCoverage(dgm, ORBIT_01);
		  for (DCf->reinit();
		       DCf->cont();
		       (*DCf)++)
		    {
		      d01 = **DCf;		
		
		      // Si le brin n'est pas marqué.
		      if (!G->isMarked(d01, markFace))
			{
			  nbDarts++;
			  if (G->isFree1(d01))
			    isFace = false;
			  G->setMark(d01, markFace);
			}
		    }
		  delete DCf;

		  if (isFace)
		
		    // Ajout du nombre de sommets de la face dans son orbite.
		    G->addAttribute(dgm, ORBIT_013, new Int_Att(nbDarts/2));
		}	
	
	    }
	}
    }

  // Demarquage et liberation de la marque.
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markFace);
  G->freeMark(markFace);
}


/******************************************************************************
 *  Fonction : CAttribute * Find_Attribute(CGMap * G, TOrbit o, TAttributeId typeAttr) *
 *----------------------------------------------------------------------------*
 *  Cette fonction parcourt la G-carte jusqu'à ce qu'elle trouve le type      *
 * d'attribut passé en paramètre.                                             *
 *                                                                            *
 *****************************************************************************/

CAttribute * Find_Attribute(CGMap * G, TOrbit o, TAttributeId typeAttr)
{
  CAttribute * alpha = NULL;
  CDart * dgm;

  // Création d'une marque.
  int mark = G->getNewMark();

  // Parcours des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit();
       Cgm.cont() && (alpha == NULL);
       Cgm++)
    {
      dgm = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(dgm, mark))
	{
	  // Récupèration de  l'attribut s'il existe
	  alpha = G->getAttribute(dgm, o, typeAttr);

	  // Si l'attribut n'alpha pas été trouvé.
	  if (alpha == NULL)
	    {
	      // Marquage des brins de l'orbite.
	      CCoverage* DC = G->getDynamicCoverage(dgm, o);
	      for (DC->reinit();
		   DC->cont();
		   (*DC)++)
		G->setMark(**DC, mark);
	      delete DC;
	    }
	}
    }

  // Demarquage et liberation de la marque.
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
      G->unsetMark(*Cgm, mark);
  G->freeMark(mark);

  return alpha;
}
//******************************************************************************
} // namespace GMap3d
//******************************************************************************
