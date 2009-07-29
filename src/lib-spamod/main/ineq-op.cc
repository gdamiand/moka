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
 *  Fichier  : Ineq_Op.cpp                                                    *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des fonctions contenues dans le      *
 * le fichier Ineq_Op.h.                                                      *
 *                                                                            *
 *****************************************************************************/


#include "ineq-op.hh"
#include "definition.hh"
#include "ineq-table-6-att.hh"
#include "ineq-table-2-att.hh"
#include "ineq-use-att.hh"
#include "attribute-vertex.hh"

namespace GMap3d
{

/******************************************************************************
 *  Fonction : void Create_Vertex_Ineq(CGMap * G, CDart * dgm, int markVertex) *
 *----------------------------------------------------------------------------*
 *  Cette fonction calcule les inéquations des sommets de l'objet.            *
 *                                                                            *
 *****************************************************************************/

void Create_Vertex_Ineq(CGMap * G, CDart * dgm, int markVertex)
{
  CAttribute *currentAttribute;
  CVertex *currentPoint;

  // Calcul des inequations du sommet et affectation à l'orbite ORBIT_123.
  currentAttribute = G->getAttribute(dgm,ORBIT_123,VERTEX_ATTRIBUTE_ID);
  currentPoint = (CAttributeVertex*)currentAttribute;
  G->addAttribute(dgm,ORBIT_123,new Ineq_Table6_Att(*currentPoint));

  // Parcours et marquage des brins du sommet.
  CCoverage* DCv = G->getDynamicCoverage(dgm, ORBIT_123);
  for (DCv->reinit(); DCv->cont(); (*DCv)++)
    G->setMark(**DCv, markVertex);
  delete DCv;
}


/******************************************************************************
 *  Fonction : void Create_Edges_Ineq(CGMap * G, CDart * dgm, int markEdge)    *
 *----------------------------------------------------------------------------*
 *  Cette fonction calcule les inéquations des arêtes de l'objet.             *
 *                                                                            *
 *****************************************************************************/

void Create_Edges_Ineq(CGMap * G, CDart * dgm, int markEdge)
{
  CAttribute *currentAttribute;
  CVertex *p1, *p2;

  // Parcours des brins de la face élémentaire.
  CCoverage* DC01 = G->getDynamicCoverage(dgm, ORBIT_01);
  for (DC01->reinit(); DC01->cont(); (*DC01)++)
    {
      CDart* d01 = **DC01;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d01, markEdge))
	{
	  // Coordonnées du point associé au sommet contenant dgm.
	  currentAttribute = G->getAttribute(d01,ORBIT_123,
					       VERTEX_ATTRIBUTE_ID);
	  p1 = (CAttributeVertex*)currentAttribute;
	
	  // Coordonnées du point associé au sommet contenant alpha0(dgm).
	  currentAttribute = G->getAttribute(G->alpha0(d01),ORBIT_123,
					       VERTEX_ATTRIBUTE_ID);
	  p2 = (CAttributeVertex*)currentAttribute;
	
	  // Calcul des inequations et affectation à l'orbite ORBIT_023.
	  G->addAttribute(d01,ORBIT_023,new Ineq_Table6_Att(*p1, *p2));
	
	  // Parcours et marquage des brins de l'orbite ORBIT_023.
	  CCoverage* DCe = G->getDynamicCoverage(d01, ORBIT_023);
	  for (DCe->reinit();DCe->cont(); (*DCe)++)
	    G->setMark(**DCe, markEdge);	
	  delete DCe;
	}
    }
  delete DC01;
}


/******************************************************************************
 *  Fonction : void Create_Face_Ineq(CGMap * G, CDart * dgm, int markFace)     *
 *----------------------------------------------------------------------------*
 *  Cette fonction calcule les inéquations des faces de l'objet.              *
 *                                                                            *
 *****************************************************************************/

void Create_Face_Ineq(CGMap * G, CDart * dgm, int markFace)
{
  CAttribute *currentAttribute;
  CVertex *currentPoint, *p1, *p2;;

  //Il s'agit d une face.
  if (G->getAttribute(dgm, ORBIT_013, INT_ATTRIBUTE_ID))	
    {
      // Coordonnées du point associé au sommet contenant dgm.
      currentAttribute = G->getAttribute(dgm,ORBIT_123, VERTEX_ATTRIBUTE_ID);
      currentPoint = (CAttributeVertex*)currentAttribute;

      // Coordonnées du point associé au sommet contenant alpha0(dgm).
      currentAttribute = G->getAttribute(G->alpha0(dgm), ORBIT_123,
					   VERTEX_ATTRIBUTE_ID);
      p1 = (CAttributeVertex*)currentAttribute;

      // Coordonnées du point associé au sommet contenant alpha1(alpha0(dgm)).
      currentAttribute = G->getAttribute(G->alpha0(G->alpha1(dgm)),
					   ORBIT_123, VERTEX_ATTRIBUTE_ID);
      p2 = (CAttributeVertex*)currentAttribute;

      //Calcul des inequations de la face et affectation alpha l'orbite ORBIT_013.
      G->addAttribute(dgm,ORBIT_013,new Ineq_Table2_Att(*currentPoint, *p1,
							  *p2));

      //Parcours et marquage des brins de la face.
      CCoverage* DCf = G->getDynamicCoverage(dgm, ORBIT_013);
      for (DCf->reinit(); DCf->cont(); (*DCf)++)
	G->setMark(**DCf, markFace);
      delete DCf;
    }
  else
    G->setMark(dgm, markFace);
}


/******************************************************************************
 *  Fonction : void Create_Ineq(CGMap * G)                                     *
 *----------------------------------------------------------------------------*
 *  Cette fonction calcule les inéquations des cellules de l'objet.           *
 *                                                                            *
 *****************************************************************************/

void Create_Ineq(CGMap *G)
{
  // Création de 3 marques.
  int markVertex = G->getNewMark();
  int markFace = G->getNewMark();
  int markEdge = G->getNewMark();


  // Parcours des brins de la GCarte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      CDart* dgm = *Cgm;

      // Si le brin n'est pas marqué avec la marque markVertex.
      if (!G->isMarked(dgm, markVertex))
	Create_Vertex_Ineq(G, dgm, markVertex);

      // Si le brin n'est pas marqué avec la marque markEdge.
      if (!G->isMarked(dgm, markEdge) && !G->isFree0(dgm))
	Create_Edges_Ineq(G, dgm, markEdge);
	
      // Si le brin n'est pas marqué avec la marque markFace.
      if (!G->isMarked(dgm, markFace))
	Create_Face_Ineq(G, dgm, markFace);	
    }

  // Démarquage et libération des marques.
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      G->unsetMark(*Cgm, markVertex);
      G->unsetMark(*Cgm, markFace);
      G->unsetMark(*Cgm, markEdge);
    }
  G->freeMark(markVertex);
  G->freeMark(markFace);
  G->freeMark(markEdge);

}


/******************************************************************************
 *  Fonction : void Calculate_Bounding_Box(CGMap * G, CDart * d, TOrbit o,       *
 *                                         float minMaxXyz[6])              *
 *----------------------------------------------------------------------------*
 *  Cette fonction calcule la boite englobante des cellules de l'objet.       *
 *                                                                            *
 *****************************************************************************/

void Calculate_Bounding_Box(CGMap * G, CDart * d, TOrbit o, float minMaxXyz[6])
{
  CDart * dO;

  // Création d'une marque.
  int markVertex = G->getNewMark();

  // Calcul de la boite englobante de l'objet complet.
  if (d == NULL && o == ORBIT_0123)
    {
      // Parcours des brins de la G-carte.
      CDynamicCoverageAll Cgm(G);
      Cgm.reinit();
      dO = *Cgm;

      // Récupération des inéquations du sommet contenant le premier brin du
      // parcours.
      CAttribute * alpha = G->getAttribute(dO,ORBIT_123,INEQ6_ATTRIBUTE_ID);
      Ineq_Table6 * t = ((Ineq_Table6_Att*)alpha)->Get_Data();

      // Stockage des paramètres w des inéquations.
      for (int i=0; i<6; i++)
	minMaxXyz[i] = t->Get_Ineq(i)->GetW();

      // Parcours et marquage des brins du sommet.
      CCoverage* DCv = G->getDynamicCoverage(dO, ORBIT_123);
      for (DCv->reinit(); DCv->cont(); (*DCv)++)
	G->setMark(**DCv, markVertex);
      delete DCv;

      // Parcours des autres brins de la G-carte.
      for (; Cgm.cont(); Cgm++)
	{
	  dO = *Cgm;
	
	  // Si le brin n'est pas marqué.
	  if (!G->isMarked(dO, markVertex))
	    {
	      // Récupération des inéquations du sommet contenant dO.
	      alpha = G->getAttribute(dO,ORBIT_123,INEQ6_ATTRIBUTE_ID);
	      t = ((Ineq_Table6_Att*)alpha)->Get_Data();
	
	      // Comparaison des paramètres w des inéquations récupérées avec
	      // les valeurs du tableau.
	      for (int i=0; i<6; i++)
		if (minMaxXyz[i] < t->Get_Ineq(i)->GetW())
		  minMaxXyz[i] = t->Get_Ineq(i)->GetW();
	    }
	  // Parcours et marquage des brins du sommet.
	  CCoverage* DCv = G->getDynamicCoverage(dO, ORBIT_123);
	  for (DCv->reinit();
	       DCv->cont();
	       (*DCv)++)
	    G->setMark(**DCv, markVertex);
	  delete DCv;
	}
      // Parcours et démarquage des brins de la G-carte.
      for (Cgm.reinit(); Cgm.cont(); Cgm++)
	G->unsetMark(*Cgm, markVertex);
    }

  // Calcul de la boite englobante d'une certaine cellule.
  else
  {
      // Récupération des inéquations du sommet contenant d.
      CAttribute * alpha = G->getAttribute(d,ORBIT_123,INEQ6_ATTRIBUTE_ID);
      Ineq_Table6 * t = ((Ineq_Table6_Att*)alpha)->Get_Data();

      // Stockage des paramètres w des inéquations.
      for (int i=0; i<6; i++)
	minMaxXyz[i] = t->Get_Ineq(i)->GetW();

      // Parcours des brins de l'orbite donnée en paramètre de la fonction.
      CCoverage* DCe = G->getDynamicCoverage(d, o);
      for (DCe->reinit(); DCe->cont(); (*DCe)++)
	{
	  dO = **DCe;

	  // Si le brin n'est pas marqué.
	  if (!G->isMarked(dO, markVertex))
	    {
	      // Récupération des inéquations du sommet contenant dO.
	      alpha = G->getAttribute(dO,ORBIT_123,INEQ6_ATTRIBUTE_ID);
	      t = ((Ineq_Table6_Att*)alpha)->Get_Data();
	
	      // Comparaison des paramètres w des inéquations récupérées avec
	      // les valeurs du tableau.
	      for (int i=0; i<6; i++)
		if (minMaxXyz[i] < t->Get_Ineq(i)->GetW())
		  minMaxXyz[i] = t->Get_Ineq(i)->GetW();
	
	      // Marquage des brins de l'orbite spécifiée.
	      if (o == ORBIT_01)
		{
		  G->setMark(dO, markVertex);
		  G->setMark(G->alpha1(dO), markVertex);
		}
	      else
		if (o == ORBIT_012)
		  {
		    G->setMark(dO, markVertex);
		    G->setMark(G->alpha1(dO), markVertex);
		    G->setMark(G->alpha2(dO), markVertex);
		  }
		else
		  if (o == ORBIT_0)
		    G->setMark(dO, markVertex);
		  else
		    {
		     G->setMark(dO, markVertex);
		     G->setMark(G->alpha1(dO), markVertex);
		     G->setMark(G->alpha2(dO), markVertex);
		     G->setMark(G->alpha3(dO), markVertex);
		    }
	    }
	  }

      // Démarquage des brins de la G-carte.
      for (DCe->reinit(); DCe->cont(); (*DCe)++)
	G->unsetMark(**DCe, markVertex);
      delete DCe;
    }
  // Libération de la marque.
  G->freeMark(markVertex);
}


/******************************************************************************
 *  Fonction : void Ineq_Vertex_Set_Unused(CGMap * G, CDart * d, TOrbit o,      *
 *                                         float minMaxXyz[6])              *
 *----------------------------------------------------------------------------*
 *  Cette fonction met à jour les variables used des inéquations des cellules *
 * de l'objet.                                                                *
 *                                                                            *
 *****************************************************************************/

void Ineq_Vertex_Set_Unused(CGMap * G, CDart * d, TOrbit o, float minMaxXyz[6])
{
  CDart * dO;
  CAttribute * alpha;
  Ineq_Table6 * t;

  // Création d'une marque.
  int markVertex = G->getNewMark();

  // Parcours des brins de l'orbite o.
  CCoverage* DCe = G->getDynamicCoverage(d, o);
  for (DCe->reinit(); DCe->cont(); (*DCe)++)
    {
      dO = **DCe;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(dO, markVertex))
	{
	  // Récupération des inéquations du sommet contenant dO.
	  alpha = G->getAttribute(dO,ORBIT_123,INEQ6_ATTRIBUTE_ID);
	  t = ((Ineq_Table6_Att*)alpha)->Get_Data();

	  // Mise à jour de la variable used des inéquations.
	  for (int i=0; i<6; i++)
	    if (fabs(minMaxXyz[i] - t->Get_Ineq(i)->GetW())>EPS)
	      t->Get_Ineq(i)->Set_Unused();
	
	  // Marquage des brins de l'orbite spécifiée.
	  if (o == ORBIT_01)
	    {
	      G->setMark(dO, markVertex);
	      G->setMark(G->alpha1(dO), markVertex);
	    }
	  else
	    if (o == ORBIT_012)
	      {
		G->setMark(dO, markVertex);
		G->setMark(G->alpha1(dO), markVertex);
		G->setMark(G->alpha2(dO), markVertex);
	      }
	    else
	      G->setMark(dO, markVertex);
	}
    }

  // Démarquage des brins de l'orbite o.
  for (DCe->reinit(); DCe->cont(); (*DCe)++)
    G->unsetMark(**DCe, markVertex);
  G->freeMark(markVertex);

  delete DCe;
}


/******************************************************************************
 *  Fonction : void Compare_Ineq_Vertex(CGMap * G, CDart * d, int markVertex)  *
 *----------------------------------------------------------------------------*
 *  Cette fonction recherche les inéquations visibles des sommets pour la     *
 * cellule de plus grande dimension contenant d.                              *
 *                                                                            *
 *****************************************************************************/

void Compare_Ineq_Vertex(CGMap* G, CDart* d, int markVertex)
{
  float minMaxXyz[6];

  // Le brin appartient à une face.
  if (G->getEmbeddingOwner(d, ORBIT_013))
    {
      // Calcul de la boite englobante.
      Calculate_Bounding_Box(G, d, ORBIT_01, minMaxXyz);

      // Mise à jour des variables used.
      Ineq_Vertex_Set_Unused(G, d, ORBIT_01, minMaxXyz);

      // Marquage des brins de la face.
      CCoverage* DCv = G->getDynamicCoverage(d, ORBIT_01);
      for (DCv->reinit(); DCv->cont(); (*DCv)++)
	G->setMark(**DCv, markVertex);
      delete DCv;
    }

  // Le brin appartient à une arete.
  else
    {	
      if (!G->isFree0(d))
	{	
	  // Calcul de la boite englobante.
	  Calculate_Bounding_Box(G, d, ORBIT_0, minMaxXyz);

	  // Mise à jour des variables used.
	  Ineq_Vertex_Set_Unused(G, d, ORBIT_0, minMaxXyz);

	  // Marquage de d et alpha0(d).
	  G->setMark(d, markVertex);
	  G->setMark(G->alpha0(d), markVertex);
	}
      // Le brin est seul
      else
	// Marquage du brin.
	G->setMark(d, markVertex);
    }
}


/******************************************************************************
 *  Fonction : void Compare_Ineq_Edge(CGMap * G, CDart * d, int markEdge)      *
 *----------------------------------------------------------------------------*
 *  Cette fonction recherche les inéquations visibles des arêtes pour les     *
 * faces contenant d.                                                         *
 *                                                                            *
 *****************************************************************************/

void Compare_Ineq_Edge(CGMap* G, CDart* d, int markEdge)
{
  CAttribute* alpha, * au;
  CDart* d01;
  CVertex* p;

  bool badIneq;

  // Création d'une marque.
  int markTemp = G->getNewMark();

  // Le brin appartient à une face.
  if (G->getEmbeddingOwner(d, ORBIT_013))
    {
      // Recuperation des inequations de l arete.
      alpha = G->getAttribute(d,ORBIT_023,INEQ6_ATTRIBUTE_ID);
      Ineq_Table6* t = ((Ineq_Table6_Att*)alpha)->Get_Data();

      // Tableau de type Inequ_Use pourl'arête traitée.
      au = new Ineq_Use_Att();
      Ineq_Use* u = ((Ineq_Use_Att*)au)->Get_Data();

      // Pour chaque inéquation de l'arête.
      for(int j=0; j < 6; j++)
	{
	  badIneq = false;
	  Inequation i = *t->Get_Ineq(j);

	  if (j%2 == 0)
	    {
	      // Calcul de l'inequation du demiEspace contenant l'arête.
	      i.SetW((i.GetW() - t->Get_Ineq(j+1)->GetW())/2.0);
	    }
	  else
	    {
	      // Calcul de l'inequation du demi-espace contenant l'arête.
	      i.SetW((i.GetW() - t->Get_Ineq(j-1)->GetW())/2.0);
	      }
	
	  // Parcours des brins de l'orbite ORBIT_01.
	  CCoverage* DCf = G->getDynamicCoverage(d, ORBIT_01);
	  for (DCf->reinit();
	       DCf->cont() && !badIneq;
	       (*DCf)++)
	    {
	      d01 = **DCf;

	      // Si le brin n'est pas marqué.
	      if (!G->isMarked(d01, markTemp))
		{
		  // Récupération du point associé au sommet contenant d01.
		  alpha = G->getAttribute(d01,ORBIT_123, VERTEX_ATTRIBUTE_ID);
		  p = (CAttributeVertex*)alpha;

		  // Je teste si le point récupéré ne vérifie pas une des  2
		  // inequations.
		  if(!t->Get_Ineq(j)->Test_Point (*p) || !i.Test_Point (*p))
		    {
		      badIneq = true;

		      // Mise à jour du paramètre used de l'inéquation et de la
		      // case correspondante dans le tableau de type Ineq_Use.
		      t->Get_Ineq(j)->Set_Unused();
		      u->Set_Unused(j);
		    }
		  // Marquage de d01 et de alpha1(d01)
		  G->setMark(d01, markTemp);
		  G->setMark(G->alpha1(d01), markTemp);
		}
	    }
	
	  // Démarquage des brins de l'orbite ORBIT_01.
	  for (DCf->reinit();
	       DCf->cont();
	       (*DCf)++)
	    G->unsetMark(**DCf, markTemp);
	  delete DCf;
	}
      G->addAttribute(d,ORBIT_0,au);
    }
  //Libération de la marque.
  G->freeMark(markTemp);

  // Marquage de d;
  G->setMark(d, markEdge);

  // Le brin n est pas seul
  if(!G->isFree0(d))

    // Marquage de son image par alpha0.
    G->setMark(G->alpha0(d), markEdge);
}


/******************************************************************************
 *  Fonction : void Choose_Ineq(CGMap * G)                                     *
 *----------------------------------------------------------------------------*
 *  Cette fonction recherche les inéquations visibles pour l'objet.           *
 *                                                                            *
 *****************************************************************************/

void Choose_Ineq(CGMap* G)
{
  // Création de 3 marques.
  int markEdge = G->getNewMark();
  int markVertex = G->getNewMark();

  // Parcours des brins de la GCarte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      CDart* dgm = *Cgm;

      // Le brin n'est pas marqué par la marque markVertex.
      if (!G->isMarked(dgm, markVertex))
	Compare_Ineq_Vertex(G, dgm, markVertex);

      // Le brin n'est pas marqué par la marque markEdge .
      if (!G->isMarked(dgm, markEdge))
	Compare_Ineq_Edge(G, dgm, markEdge);
    }

  // Demarquage et liberation des marques.
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      G->unsetMark(*Cgm, markVertex);
      G->unsetMark(*Cgm, markEdge);
    }
  G->freeMark(markVertex);
  G->freeMark(markEdge);

}

} // namespace GMap3d
