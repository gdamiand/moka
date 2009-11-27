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
 *  Fichier  : Analytic_View.cpp                                              *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe            *
 * Analytic_View.                                                             *
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

#include "analytic-view.hh"
#include "ineq-table-6.hh"
#include "ineq-table-6-att.hh"
#include "int-att.hh"
#include "ineq-use-att.hh"
#include "ineq-table-2-att.hh"
#include "ineq-table-2.hh"
#include "ineq-use.hh"
#include "definition.hh"
#include "vector3d.hh"
#include "point-list.hh"
#include "attribute-vertex.hh"

using namespace std;
using namespace GMap3d;
/******************************************************************************
 *                            Constructeur                                    *
 *****************************************************************************/

Analytic_View::Analytic_View(CGMap * GM)
{
  G = GM;
  directInfo = G->getNewDirectInfo();
}


// Destructeur.
Analytic_View::~Analytic_View()
{
  // Libération du champ directInfo.
  G->freeDirectInfo(directInfo);
}


/******************************************************************************
 *  Fonction : void Analytic_View::Create_Vertices_Faces()                    *
 *----------------------------------------------------------------------------*
 *  Cette fonction crée les listes d'affichage des faces.                     *
 *                                                                            *
 *****************************************************************************/

void Analytic_View::Create_Vertices_Faces()
{
  CAttribute* alpha;
  Ineq_Table6* t;
  CVertex* p;


  // Création d'une marque.
  int markVertex = G->getNewMark();

  // Parcours des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      CDart* dgm = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(dgm, markVertex))
	{	
	  // Liste d'afichage du sommet.
	  Point_List* pList = new Point_List();
	  list<CVertex*> *vL = (list<CVertex*>*)pList->Get_Data();

	  // Liste de points du sommet.
	  list<CVertex*> *l = new list<CVertex*>;

	  // Tableau des 6 inequations et coordonnées du point
	  // associées au sommet.
	  alpha = G->getAttribute(dgm,ORBIT_123,INEQ6_ATTRIBUTE_ID);
	  t = ((Ineq_Table6_Att*)alpha)->Get_Data();
	  alpha = G->getAttribute(dgm,ORBIT_123,VERTEX_ATTRIBUTE_ID);
	  p = (CAttributeVertex*)alpha;

	  // Création de la liste de points du sommet.
	  l->push_front(new CVertex(p->getX()+0.5,
				      p->getY()+0.5,
				      p->getZ()+0.5));
	  l->push_front(new CVertex(p->getX()+0.5,
				      p->getY()+0.5,
				      p->getZ()-0.5));
	  l->push_front(new CVertex(p->getX()+0.5,
				      p->getY()-0.5,
				      p->getZ()-0.5));
	  l->push_front(new CVertex(p->getX()+0.5,
				      p->getY()-0.5,
				      p->getZ()+0.5));
	  l->push_front(new CVertex(p->getX()-0.5,
				      p->getY()+0.5,
				      p->getZ()+0.5));
	  l->push_front(new CVertex(p->getX()-0.5,
				      p->getY()+0.5,
				      p->getZ()-0.5));
	  l->push_front(new CVertex(p->getX()-0.5,
				      p->getY()-0.5,
				      p->getZ()-0.5));
	  l->push_front(new CVertex(p->getX()-0.5,
				      p->getY()-0.5,
				      p->getZ()+0.5));
	
	  // Ajout de cette liste à la variable directInfo du brin qui
	  // possède les attributs de l'orbite ORBIT_123.
	
	  G->setDirectInfo(G->getEmbeddingOwner(dgm, ORBIT_123),directInfo,l);
	
	  // Création de la liste d'affichage du sommet.
	  for (int i = 0 ; i<6 ; i++)
	    {
	      // Recherche des inequations visibles du sommet.
	      Inequation *ineq = t->Get_Ineq(i);

	      // Si l'inéquation est visible.
	      if (ineq->Is_Used())
		{
		  // Ajout des points verifiant l'inequation à la liste.
		  switch (i)
		    {
		    case 0:
		      vL->push_front(new CVertex(ineq->GetW(),
						    p->getY()+0.5,
						    p->getZ()+0.5));
		      vL->push_front(new CVertex(ineq->GetW(),
						    p->getY()+0.5,
						    p->getZ()-0.5));
		      vL->push_front(new CVertex(ineq->GetW(),
						    p->getY()-0.5,
						    p->getZ()-0.5));
		      vL->push_front(new CVertex(ineq->GetW(),
						    p->getY()-0.5,
						    p->getZ()+0.5));
		      break;

		    case 1:
		      vL->push_front(new CVertex(-ineq->GetW(),
						    p->getY()+0.5,
						    p->getZ()+0.5));
		      vL->push_front(new CVertex(-ineq->GetW(),
						    p->getY()+0.5,
						    p->getZ()-0.5));
		      vL->push_front(new CVertex(-ineq->GetW(),
						    p->getY()-0.5,
						    p->getZ()-0.5));
		      vL->push_front(new CVertex(-ineq->GetW(),
						    p->getY()-0.5,
						    p->getZ()+0.5));
		      break;

		    case 2:
		      vL->push_front(new CVertex(p->getX()+0.5,
						    ineq->GetW(),
						    p->getZ()+0.5));
		      vL->push_front(new CVertex(p->getX()+0.5,
						    ineq->GetW(),
						    p->getZ()-0.5));
		      vL->push_front(new CVertex(p->getX()-0.5,
						    ineq->GetW(),
						    p->getZ()-0.5));
		      vL->push_front(new CVertex(p->getX()-0.5,
						    ineq->GetW(),
						    p->getZ()+0.5));
		      break;

		    case 3:
		      vL->push_front(new CVertex(p->getX()+0.5,
						    -ineq->GetW(),
						    p->getZ()+0.5));
		      vL->push_front(new CVertex(p->getX()+0.5,
						    -ineq->GetW(),
						    p->getZ()-0.5));
		      vL->push_front(new CVertex(p->getX()-0.5,
						    -ineq->GetW(),
						    p->getZ()-0.5));
		      vL->push_front(new CVertex(p->getX()-0.5,
						    -ineq->GetW(),
						    p->getZ()+0.5));
		      break;

		    case 4:
		      vL->push_front(new CVertex(p->getX()+0.5,
						    p->getY()+0.5,
						    ineq->GetW()));
		      vL->push_front(new CVertex(p->getX()+0.5,
						    p->getY()-0.5,
						    ineq->GetW()));
		      vL->push_front(new CVertex(p->getX()-0.5,
						    p->getY()-0.5,
						    ineq->GetW()));
		      vL->push_front(new CVertex(p->getX()-0.5,
						    p->getY()+0.5,
						    ineq->GetW()));
		      break;

		    case 5:
		      vL->push_front(new CVertex(p->getX()+0.5,
						    p->getY()+0.5,
						    -ineq->GetW()));
		      vL->push_front(new CVertex(p->getX()+0.5,
						    p->getY()-0.5,
						    -ineq->GetW()));
		      vL->push_front(new CVertex(p->getX()-0.5,
						    p->getY()-0.5,
						    -ineq->GetW()));
		      vL->push_front(new CVertex(p->getX()-0.5,
						    p->getY()+0.5,
						    -ineq->GetW()));
		      break;

		    default:
		      break;
		    }
		}
	    }
	
	  // Ajout de la liste d'affichage au sommet en tant qu'attribut.
	  G->addAttribute(dgm, ORBIT_123, pList);

	  // Parcours et marquage des brins du sommet.
	  CCoverage* DCv = G->getDynamicCoverage(dgm, ORBIT_123);
	  for(DCv->reinit(); DCv->cont(); (*DCv)++)
	    G->setMark(**DCv, markVertex);
	  delete DCv;
	}
    }

  // Demarquage et libération de la marque "markVertex".
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markVertex);
  G->freeMark(markVertex);

}


/******************************************************************************
 *  Fonction : void Analytic_View::Create_Edges_Faces()                       *
 *----------------------------------------------------------------------------*
 *  Cette fonction crée  les listes d'affichage des arêtes de l'objet.        *
 *                                                                            *
 *****************************************************************************/

void Analytic_View::Create_Edges_Faces()
{
  CAttribute* alpha;
  Ineq_Table6* t;
  list<CVertex*> *pl1, *pl2;
  list<CVertex*>::iterator iter;
  CVertex *p1, *p2;
  CVertex* nearest[4];

  int tmpNb;
  float length;
  bool biplanes = false, noEdge;


  // Création d'une marque.
  int markEdge = G->getNewMark();

  // Parcours des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      CDart* dgm = *Cgm;

      // L'arête n'alpha pas encore été traitée.
      if (!G->isMarked(dgm, markEdge) && !G->isFree0(dgm))
	{
	  // Liste d'affichage de l'arête.
	  Point_List* pList = new Point_List();
	  list<CVertex*> *l = (list<CVertex*>*)pList->Get_Data();

	  // Coordonnées des 2 sommets.
	  alpha = G->getAttribute(dgm,ORBIT_123,VERTEX_ATTRIBUTE_ID);
	  p1 = (CAttributeVertex*)alpha;
	  alpha = G->getAttribute(G->alpha0(dgm),ORBIT_123,
			       VERTEX_ATTRIBUTE_ID);
	  p2 = (CAttributeVertex*)alpha;
	
	  // Vecteur correspondant à l'arête
	  Vector3D edgeVec(p2->getX()-p1->getX(),
			    p2->getY()-p1->getY(),
			    p2->getZ()-p1->getZ());
	
	  // Inequations de l'arête.
	  alpha = G->getAttribute(dgm,ORBIT_023,INEQ6_ATTRIBUTE_ID);
	  t = ((Ineq_Table6_Att*)alpha)->Get_Data();
	
	  // Liste des points des 1er et 2ieme sommets.
	  pl1 = (list<CVertex*>*)G->getDirectInfo(G->getEmbeddingOwner(dgm, ORBIT_123),directInfo);
	  pl2 = (list<CVertex*>*)G->getDirectInfo(G->getEmbeddingOwner(G->alpha0(dgm),ORBIT_123),directInfo);
	
	  // Création de la liste d'affichage.
	  for (int i=0; i<6; i++)
	    {
	      // Recherche des inequations visibles.
	      Inequation *ineq = t->Get_Ineq(i);
		
	      if (ineq->Is_Used())
		{
		  noEdge=false;		

		  // Le segment n'est pas sur un (ou deux) plan(s) du repere.
		  if (ineq->Get_Dim()!=1)
		    {
		      tmpNb = 0;		
		
		      // Recherche des points du 1er sommet verifiant
		      // l'inequation.
		      for (iter = pl1->begin() ;
			   iter != pl1->end() && tmpNb < 2 ;
			   iter++)
			
			if (ineq->Test_Point_Eq(**iter))
			  {
			    nearest[tmpNb] = *iter;
			    tmpNb++;
			  }
		      		
		      // Recherche des points du 2ieme sommet verifiant
		      // l'inequation.
		      for (iter = pl2->begin() ;
			   iter != pl2->end() && tmpNb < 4 ;
			   iter++)
			
			if (ineq->Test_Point_Eq(**iter))
			  {
			    nearest[tmpNb] =  *iter;
			    tmpNb++;
			  }
			  			
		      // Ajout des points à la liste dans l'ordre.
		      // pour permettre l affichage direct.
		      Vector3D v(nearest[2]->getX()-nearest[0]->getX(),
				 nearest[2]->getY()-nearest[0]->getY(),
				 nearest[2]->getZ()-nearest[0]->getZ());
		
		      l->push_front(nearest[0]);
		      l->push_front(nearest[1]);
		
		      if (v == edgeVec)
			{			
			  l->push_front(nearest[3]);
			  l->push_front(nearest[2]);
			}		
		      else
			{			
			  l->push_front(nearest[2]);
			  l->push_front(nearest[3]);
			}	
		    }		
		
		  // Le segment est sur un (ou deux) plan(s) du repère.
		  else
		    {	
		      if ((*p2-*p1).norm()<1.0)
			length = 2.0;		
		      else
			length = 2.0*(*p2-*p1).norm();		

		      tmpNb = 0;		
			
		      // Recherche du (ou des 2) point(s) le(s) plus
		      //proche(s) pour le 1er sommet verifiant l'inequation
		      for (iter = pl1->begin();
			   iter != pl1->end() && tmpNb<4;
			   iter++)
			{
			  if(ineq->Test_Point_Eq(**iter))
			    {			
			      // Le point est plus proche.
			      if ((*p2-**iter).norm()
				  <length-EPS)
				{
				  length =
				    (*p2-**iter).norm();
				  if (tmpNb != 0)
				    nearest[tmpNb] = nearest[0];
				
				  nearest[0] = *iter;
				  biplanes = false;
				
				}
			
			      // Le point est à égale distance.
			      else
				if(fabs((*p2-**iter).norm()
					-length)
				   <= EPS)
				  {				
				    if (tmpNb != 1)
				      nearest[tmpNb] = nearest[1];
				
				    nearest[1] = *iter;
				    biplanes = true;
				  }
			
			      // Le point est plus éloigné.
				else
				  nearest[tmpNb]=*iter;
			
				  tmpNb++;
			    }
			}
		
		      // Le segment est seulement sur un plan du repère.
		      if (!biplanes)
			{			
			  CVertex* tmp;
			  tmpNb = 0;
			
			  if ((*p1-*p2).norm()<1.0)
			    length = 2.0;			
			  else
			    length = 2.0*(*p1-*p2).norm();
			
			  // Equation que doit vérifier le 2ieme point
			  // du 1er sommet.
			  Inequation eq;
			
			  if (i == 0 || i == 1)
			    eq.SetAbcw(0, 0, 1, nearest[0]->getZ());
			
			  else
			    if (i == 2 || i == 3)
			      eq.SetAbcw(1, 0, 0, nearest[0]->getX());
			
			    else
			      eq.SetAbcw(0, 1, 0, nearest[0]->getY());
			  			
			  // recherche du 2ieme point du 1er sommet
			  // verifiant l'equation calculee.
			  for (int i=2; i<4; i++)
			    {
			      if (eq.Test_Point_Eq (*nearest[i]))
				{
				  tmp = nearest[1];
				  nearest[1] = nearest[i];
				  nearest[i] = tmp;
				}
			    }			
			
			  // Recherche des 2 points du 2ieme sommet
			  // verifiant l'inequation.
			  for (iter=pl2->begin();
			       iter!=pl2->end() && tmpNb<4;
			       iter++)
			    {
			      if (ineq->Test_Point_Eq(**iter))
				{
				  Vector3D v((*iter)->getX()
					     -nearest[1]->getX(),
					     (*iter)->getY()
					     -nearest[1]->getY(),
					     (*iter)->getZ()
					     -nearest[1]->getZ());
				
				  // Les deux vecteurs sont égaux.
				  if (edgeVec == v)
				    nearest[2] = *iter;
				
				  // Le point est plus proche.
				  else
				    if((*p1-**iter).norm()
				       <length-EPS){
				      length = (*p1-**iter).norm();
				      nearest[3] = *iter;
				    }
				  tmpNb++;
				}
			    }
			
			
			  // Cas particlier :
			  // Les deux voxels des sommets se chevauchent.
			  if (Vector3D(*nearest[0],
				       *nearest[3]).Scal_Product(edgeVec)
			      <0.0)
			    {
			      Inequation eq2;
			      bool found = false;
			
			      if (nearest[2]->getX() == nearest[3]->getX())
				{
				  eq2.SetAbcw(1,0,0,nearest[3]->getX());
				  if (eq2 != *ineq)
				    found = true;
				}
			      if (nearest[2]->getY() == nearest[3]->getY()
				  && !found)
				{
				  eq2.SetAbcw(0,1,0,nearest[3]->getY());
				  if (eq2 != *ineq)
				    found = true;
				}
			
			      if (nearest[2]->getZ() == nearest[3]->getZ()
				  && !found)
				{
				  eq2.SetAbcw(0,0,1,nearest[3]->getZ());
				  if (eq2 != *ineq)
				    found = true;
				}	
			      nearest[0] = ineq->Intersec (eq, eq2);
			      nearest[3] = nearest[0];
			    }
			}
		
		      // Le segment est sur deux plans.
		      else
			{
			  tmpNb = 0;
			
			  // Recherche des points du 2ieme sommet
			  // verifiant l'inequation.
			  for (iter=pl2->begin();
			       iter!=pl2->end() && tmpNb < 4;
			       iter++)
			    {
			      if(ineq->Test_Point_Eq(**iter))
				{				
				  // Calcul des deux vecteurs pour chaque
				  // point testé.
				  Vector3D v1((*iter)->getX()
					      -nearest[1]->getX(),
					      (*iter)->getY()
					      -nearest[1]->getY(),
					      (*iter)->getZ()
					      -nearest[1]->getZ());
				
				  Vector3D v0((*iter)->getX()
					      -nearest[0]->getX(),
					      (*iter)->getY()
					      -nearest[0]->getY(),
					      (*iter)->getZ()
					      -nearest[0]->getZ());
				
				  // Le premier vecteur est parallèle
				  // au vecteur de l'arête.
				  if ( v1.Vect_Product(edgeVec) ==
				       Vector3D(0, 0, 0))
				    {
				      if (v1 != edgeVec)
					nearest[2] = *iter;
				    }
				
				  // Le deuxieme vecteur est parallèle
				  // au vecteur de l'arete.
				  else
				    if ( v0.Vect_Product(edgeVec) ==
					 Vector3D(0, 0, 0))
				      {
					if (v0 != edgeVec)
					  nearest[3] = *iter;
				      }
				  tmpNb++;
				}
			    }
			
			  if (Vector3D(*nearest[0],	
				       *nearest[3]).Scal_Product(edgeVec)
			      +1<EPS)
			    noEdge=true;
			}		
		      if (!noEdge)
			{
			  // Ajout des 4 points trouves alpha la liste.
			  l->push_front(nearest[0]);
			  l->push_front(nearest[1]);
			  l->push_front(nearest[2]);
			  l->push_front(nearest[3]);
			}
		    }
		}
	    }
		
	  // Ajout de la liste d'affichage à l'arete.
	  G->addAttribute(dgm, ORBIT_023, pList);
	
	  // Parcours et marquage des brins de l'arête.
	  CCoverage* DCe = G->getDynamicCoverage(dgm, ORBIT_023);
	  for(DCe->reinit(); DCe->cont(); (*DCe)++)
	    G->setMark(**DCe, markEdge);
	  delete DCe;	
	}
    }
  // Demarquage et liberation de la marque "markEdge".
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markEdge);
  G->freeMark(markEdge);

}


/******************************************************************************
 *  Fonction : void Analytic_View::Create_Faces()                             *
 *----------------------------------------------------------------------------*
 *  Cette fonction crée les listes d'affichage des faces de l'objet.          *
 *                                                                            *
 *****************************************************************************/

void Analytic_View::Create_Faces()
{
  CAttribute* alpha;
  Ineq_Table2* tf;
  list<CVertex*> *pl;
  list<CVertex*>::iterator iter;
  Ineq_Table6 *te;
  CVertex* p, *lastPt;
  Ineq_Use *u;
  CDart *dgm, *d01;
  Inequation **faceIneq = NULL, **faceIneq2 = NULL;

  int nbPoints, realNbPoints;
  bool testPoint;


  // Création d'une marque.
  int markFace = G->getNewMark();

  // Parcours des brins de la G-carte.
  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      dgm = *Cgm;

      // La face n'alpha pas encore ete traitée.
      if (!G->isMarked(dgm, markFace) &&
	  G->getEmbeddingOwner(dgm, ORBIT_013))
	{
	  // Liste d'affichage de la face.
	  Point_List* pList = new Point_List();
	  list<CVertex*> *l = (list<CVertex*>*)pList->Get_Data();

	  // Tableau des 2 inequations de la face.
	  alpha = G->getAttribute(dgm,ORBIT_013,INEQ2_ATTRIBUTE_ID);
	  tf = ((Ineq_Table2_Att*)alpha)->Get_Data();

	  // Nombre de sommets de la face.
          alpha = G->getAttribute(dgm,ORBIT_013,INT_ATTRIBUTE_ID);
	  nbPoints = *(((Int_Att*)alpha)->Get_Data());

	  // Pour chaque inéquation de la face..
	  for (int i=0 ; i<2 ; i++)
	    {	
	      Inequation *ineq = tf->Get_Ineq(i);

	      // La face est de dimension 1 ( deux des composantes alpha, b ou c
	      // de l'inéquation de la face sont nulles).
	      if (ineq->Get_Dim() == 1)
		{
		  if (i == 0)
		    {
		      int k = 0;

		      // Tableau contenant les inéquations non visibles.
		      faceIneq = new Inequation*[nbPoints];
	
		      // Parcours les brins de la face.
		      CCoverage* DC01 = G->getDynamicCoverage(dgm, ORBIT_01);
		      for (DC01->reinit();
			   DC01->cont();
			   (*DC01)++)
			{
			  d01 = **DC01;
			
			  // Le brin n'alpha pas encore été traité.
			  if (!G->isMarked(d01, markFace))
			    {
			      // Récupération des inéquation de l'arête et du
			      // tableau de type Ineq_Use.
			      alpha = G->getAttribute(d01,ORBIT_023,
						   INEQ6_ATTRIBUTE_ID);
			      te = ((Ineq_Table6_Att*)alpha)->Get_Data();
			      alpha = G->getAttribute(d01,ORBIT_0,
						   USE_ATTRIBUTE_ID);
			      u = ((Ineq_Use_Att*)alpha)->Get_Data();

			      // Recupération des inéquations des arêtes qui
			      // ne sont pas visibles.
			      for (int j=0; j<6; j++)
				if (!u->Get_Use(j) &&
				    te->Get_Ineq(j)->Get_Dim()!=0)
				  faceIneq[k++] = te->Get_Ineq(j);

			      G->setMark(d01, markFace);
			      G->setMark(G->alpha0(d01), markFace);
			    }
			}
		      delete DC01;
		    }
		
		  lastPt = NULL;

		  // Nombre de points de la facette.
		  realNbPoints = 0;
		
		  // Recherche des points de la facette.
		  for (int j=0 ; j<nbPoints ; j++)
		    {
		      testPoint = false;

		      for (int k=j+1 ; k<nbPoints+1 && !testPoint; k++)
			{
			  // Si les deux bords des demi-espaces définis par
			  // les inéquations ne sont pas parallèles.
			  if(Vector3D(faceIneq[j]->GetA(),
				      faceIneq[j]->GetB(),
				      faceIneq[j]->GetC())
			     .Vect_Product(Vector3D(faceIneq[k%nbPoints]
						    ->GetA(),
						    faceIneq[k%nbPoints]
						    ->GetB(),
						    faceIneq[k%nbPoints]
						    ->GetC()))
			     != Vector3D(0,0,0))
			    {
			      testPoint = true;
			
			      // Calcul du point d'intersection.
			      p = ineq->Intersec(*faceIneq[j],
						 *faceIneq[k%nbPoints]);

			      //  Je teste si le point vérifie le reste des
			      // des inéquations du tableau.
			      for (int m=0 ; m<nbPoints && testPoint ; m++)
				{
				  if (m != j && m != k%nbPoints)
				    if (faceIneq[m]->Test_Point(*p) &&
					!faceIneq[m]->Test_Point_Eq(*p))
				      {
					testPoint = false;
					delete p;
				      }
				}
			    }
			
			  // Si un point correct est trouvé.
			  if (testPoint)
			    {
			      // Si ce point est différent du précédent.
			      if (lastPt == NULL ||
				  (lastPt != NULL && *lastPt != *p))
				{
				  // Ajout du point à la liste.
				  l->push_front(p);
				  lastPt = p;
				  realNbPoints++;			
				}
			      else
				delete p;
			    }
			}
		    }

		  if (i == 1)
		    delete [] faceIneq;
		}
		
	      // La face est de dimension 2.
	      else		
		if (ineq->Get_Dim() == 2)
		  {
		    int k = 0;

		    if (i == 0)
		      {
			// Tableau contenant les inéquations non visibles des
			// arêtes.
			faceIneq2 = new Inequation*[nbPoints*2];
	
			// Parcours des brins de la face.
			CCoverage* DC01 = G->getDynamicCoverage(dgm, ORBIT_01);
			for (DC01->reinit();
			     DC01->cont();
			     (*DC01)++)
			  {
			    d01 = **DC01;
			
			    // Le brin n'alpha pas encore été traité.
			    if (!G->isMarked(d01, markFace))
			      {



				// Récupération des inéquations de l'arête et
				// du tableau de type Ineq_Use.
				alpha = G->getAttribute(d01,ORBIT_023,
						     INEQ6_ATTRIBUTE_ID);
				te = ((Ineq_Table6_Att*)alpha)->Get_Data();
				alpha = G->getAttribute(d01,ORBIT_0,
						     USE_ATTRIBUTE_ID);
				u = ((Ineq_Use_Att*)alpha)->Get_Data();
				// Récupération les inéquations des arêtes qui
				// ne sont pas visibles.
				for (int j=0; j<6; j++)
				  {
				    if (!u->Get_Use(j) &&
					te->Get_Ineq(j)->Get_Dim()!=0)
				      {
					faceIneq2[k++] = te->Get_Ineq(j);
				      }
				  }
				    G->setMark(d01, markFace);
				    G->setMark(G->alpha0(d01), markFace);
			      }
			  }

			// Parcours et démarquage des brins de la face.
			for (DC01->reinit();
			     DC01->cont();
			     (*DC01)++)
			  {
			    G->unsetMark(**DC01, markFace);
			  }

			
			delete DC01;

		      }



		    // Tableau contenant les inéquations utilisées pour une
		    // facette.
		    faceIneq = new Inequation*[nbPoints];
		
		    k = 0;

		    // Parcours des brins de la face.
		    CCoverage* DC01 = G->getDynamicCoverage(dgm, ORBIT_01);
		    for (DC01->reinit();
			 DC01->cont();
			 (*DC01)++)
		      {
			d01 = **DC01;
			
			// Le sommet n'alpha pas encore été traité.
			if (!G->isMarked(d01, markFace))
			  {
			    // Récupération de la liste de points du sommet.
			    pl = (list<CVertex*>*)G->getDirectInfo(G->getEmbeddingOwner
								     (d01,ORBIT_123),directInfo);
			
			    // Recherche des équations des inéquations non
			    // visibles des arêtes qui sont vérifiées par les
			    // points vérifiant l'équation de la face.
			    for (iter=pl->begin();
				 iter!=pl->end();
				 iter++) {
			      if(ineq->Test_Point_Eq(**iter))
				{
				  for (int j=2*k; j<2*k+2; j++)
				    {
				      if (faceIneq2[j]
					  ->Test_Point_Eq(**iter))
					// Stockage de l'inéquation dans le
					// tableau
					faceIneq[k] = faceIneq2[j];
				    }
				}
			    }
			    k++;
			
			    G->setMark(d01, markFace);
			    G->setMark(G->alpha1(d01), markFace);
			  }
		      }
		

		    if (i==0)
		      {
			// Parcours et démarquage des brins de la face.
			for (DC01->reinit();
			     DC01->cont();
			     (*DC01)++)
			  G->unsetMark(**DC01, markFace);
		      }
		    delete DC01;

		    lastPt = NULL;

		    // Nombre de sommets de la facette.
		    realNbPoints = 0;
		
		    for (int j=0 ; j<nbPoints ; j++)
		      {
			testPoint = false;
			
			// Pour chaque inéquation du tableau.
			for (k=j+1 ; k<nbPoints+1 && !testPoint; k++)
			  {
			    // Si les deux inéquations ne sont pas parallèles.
			    if(Vector3D(faceIneq[j]->GetA(),
					faceIneq[j]->GetB(),
					faceIneq[j]->GetC())
			       .Vect_Product(Vector3D(faceIneq[k%nbPoints]
						      ->GetA(),
						      faceIneq[k%nbPoints]
						      ->GetB(),
						      faceIneq[k%nbPoints]
						      ->GetC()))
			       != Vector3D(0,0,0))
			      {
				testPoint = true;
				
				// Calcul du point d'intersection.
				p = ineq->Intersec(*faceIneq[j],
						   *faceIneq[k%nbPoints]);

				// Je teste si le point vérifie le reste des
				// inéquations du tableau.
				for (int m=0 ; m<nbPoints && testPoint ; m++)
				  {			
				    if (m != j && m != k%nbPoints)
				      if (faceIneq[m]->Test_Point(*p) &&
					  !faceIneq[m]->Test_Point_Eq(*p))
					    {
					      testPoint = false;
					      delete p;
					    }
				  }
			      }
			
			    // Si un point correct est trouvé.
			    if (testPoint)
			      {
				// Si ce point est différent du précédent.
				if (lastPt == NULL ||
				    (lastPt != NULL && *lastPt != *p))
				  {
				    // Ajout du point à la liste.
				    l->push_front(p);
				    lastPt = p;
				    realNbPoints++;
				  }
				else
				  delete p;
			      }
			  }
		      }
		
		    delete [] faceIneq;
		
		    if (i==1)
		      delete [] faceIneq2;
		  }
		
		
	      // La face est de dimension 3.
		else
		  {

		    // Parcours des brins de la face.
		    CCoverage* DC01 = G->getDynamicCoverage(dgm, ORBIT_01);
		    for (DC01->reinit();
			 DC01->cont();
			 (*DC01)++)
		      {
			d01 = **DC01;
			
			// Le sommet n'alpha pas encore ete traité.
			if (!G->isMarked(d01, markFace))
			  {
			    // Récupération de la liste de points du sommet.
			    pl = (list<CVertex*>*)G->getDirectInfo(G->getEmbeddingOwner
								     (d01, ORBIT_123),directInfo);
			
			    // Recherche du point du sommet verifiant
			    // l'équation de inéquation courante.
			    for (iter=pl->begin();
				 iter!=pl->end();
				 iter++)
			
			      // Ajout du point à la liste.
			      if(ineq->Test_Point_Eq(**iter))
				l->push_front(*iter);


			    G->setMark(d01, markFace);
			    G->setMark(G->alpha1(d01), markFace);
			  }
		      }

		    if (i==0)
		      {
			// Parcours et démarquage des brins de la face.
			for (DC01->reinit();
			     DC01->cont();
			     (*DC01)++)
			  G->unsetMark(**DC01, markFace);
		      }
		    delete DC01;

		    // Affectation du nombre de points de la facette.
		    realNbPoints = nbPoints;
		  }
	    }

	  // Ajout de la liste à la face.
	  if(!G->getAttribute(dgm, ORBIT_013, PT_LIST_ATTRIBUTE_ID))
	      G->addAttribute(dgm, ORBIT_013, pList);


	  // Stockage du nombre de sommets de la facette à la place du nombre
	  // de sommets de la face.
	  alpha = G->getAttribute(dgm, ORBIT_013, INT_ATTRIBUTE_ID);
	  *(((Int_Att*)alpha)->Get_Data()) = realNbPoints;	
	}
      else
	G->setMark(dgm, markFace);
    }

  // Demarquage et liberation de la marque "markEdge".
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markFace);
  G->freeMark(markFace);

}


/******************************************************************************
 *  Fonction : void Analytic_View::Draw_List()                                *
 *----------------------------------------------------------------------------*
 *  Cette fonction permet d'afficher les polygones dont les points sont       *
 * contenus dans une liste en calculant au préalable la normale à leur face.  *
 *                                                                            *
 *****************************************************************************/

void Analytic_View::Draw_List(list<CVertex*> *l, int nbPoints) const
{
  if (nbPoints >= 3)
    {
      Vector3D normal;
      CVertex ** tab = new CVertex*[nbPoints];

      int count = 0;

      // Parcours de la liste de points.
      list<CVertex*>::iterator iter;
      for (iter=l->begin();
	   iter!=l->end();
	   iter++)
	{
	  // Stockage des points dans un tableau temporaire.
	  tab[count] = *iter;
	  count++;
	
	  if (count == nbPoints)
	    {
	      // Calcul de la normale à la face
	      Vector3D v1(*tab[0], *tab[1]);
	      Vector3D v2(*tab[1], *tab[2]);
	      normal = v1.Vect_Product(v2);

	      if (normal.norm()>EPS)
		normal.Normalize();

	      // Affichage du polygone
	      glBegin(GL_POLYGON);
	      glNormal3f(normal.getX(), normal.getY(), normal.getZ());
	      for (int i=0; i<nbPoints; i++)
		glVertex3f(tab[i]->getX(), tab[i]->getY(), tab[i]->getZ());
	      glEnd();

	      count = 0;
	    }
	}

      delete [] tab;
    }
}


/******************************************************************************
 *  Fonction : void Analytic_View::Vertices_Draw()                            *
 *----------------------------------------------------------------------------*
 *  Cette fonction affiche les facettes des sommets.                          *
 *                                                                            *
 *****************************************************************************/

void Analytic_View::Vertices_Draw()
{
  int markVertex = G->getNewMark();

  // Activation de l'éclairage
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glShadeModel(GL_SMOOTH);

  // Création d'un parcours des brins de la G-carte.
  CDynamicCoverageAll C(G);

  // Parcours des sommets.
  for (C.reinit(); C.cont(); C++)
    {
      CDart* d = *C;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d, markVertex))
	{
	  // Récupération de la liste d'affichage du sommet.
	  CAttribute * alpha = G->getAttribute(d,ORBIT_123, PT_LIST_ATTRIBUTE_ID);

          // Affichage des facettes.
          Draw_List(((Point_List*)alpha)->Get_Data(), 4);

	  // Marquage des brins de l'orbite ORBIT_123.
	  CCoverage* DCv = G->getDynamicCoverage(d, ORBIT_123);
	  for(DCv->reinit(); DCv->cont(); (*DCv)++)
	    G->setMark(**DCv, markVertex);
	  delete DCv;
	  }
    }

  // Parcours et démarquage des brins de la G-carte.
  for (C.reinit(); C.cont(); C++)
    G->unsetMark(*C, markVertex);
  G->freeMark(markVertex);

  // Désactivation de l'éclairage
  glDisable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  glShadeModel(GL_FLAT);
}



/******************************************************************************
 *  Fonction : void Analytic_View::Edges_Draw()                               *
 *----------------------------------------------------------------------------*
 *  Cette fonction affiche les facettes des arêtes.                           *
 *                                                                            *
 *****************************************************************************/

void Analytic_View::Edges_Draw()
{
  // Création d'une marque
  int markEdge = G->getNewMark();

  // Création d'un parcours des brins de la G-carte.
  CDynamicCoverageAll C(G);

  // Activation de l'éclairage
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glShadeModel(GL_SMOOTH);

  // Parcours des arêtes.
  for (C.reinit(); C.cont(); C++)
    {
      CDart* d = *C;

      //Si le brin n'est pas marqué.
      if (!G->isMarked(d, markEdge) && !G->isFree0(d))
	{
	  // Récupération de la liste d'affichage de l'arête.
	  CAttribute* alpha = G->getAttribute(d, ORBIT_023,PT_LIST_ATTRIBUTE_ID);

	  // Affichage des facettes.
          Draw_List(((Point_List*)alpha)->Get_Data(), 4);

	  // Parcours et marquage des brins de l'arête.
	  CCoverage* DCe = G->getDynamicCoverage(d, ORBIT_023);
	  for(DCe->reinit(); DCe->cont(); (*DCe)++)
	    G->setMark(**DCe, markEdge);
	  delete DCe;
	}
    }

  // Parcours et démarquage des brins de la G-carte.
  for (C.reinit(); C.cont(); C++)
    G->unsetMark(*C, markEdge);
  G->freeMark(markEdge);


  // Désactivation de l'éclairage
  glDisable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  glShadeModel(GL_FLAT);
}


/******************************************************************************
 *  Fonction : void Analytic_View::Faces_Draw()                               *
 *----------------------------------------------------------------------------*
 *  Cette fonction affiche les facettes des faces.                            *
 *                                                                            *
 *****************************************************************************/

void Analytic_View::Faces_Draw()
{
  CAttribute *alpha;

  int nbPoints;

  // Création d'une marque.
  int markFace = G->getNewMark();

  // Création d'un parcours des brins de la G-carte.
  CDynamicCoverageAll C(G);

  // Activation de l'éclairage
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glShadeModel(GL_SMOOTH);

  // Parcours des faces.
  for (C.reinit(); C.cont(); C++)
    {
      CDart* d = *C;

      // Si le brin n'est pas marqué.
      if(!G->isMarked(d, markFace) && G->getEmbeddingOwner(d, ORBIT_013))
	{
	  // Récupération du nombre de sommets de la face.
	  alpha = G->getAttribute(d, ORBIT_013, INT_ATTRIBUTE_ID);
	  nbPoints = *(((Int_Att*)alpha)->Get_Data());

	  // Récupération de la liste d'affichage de la face.
	  alpha = G->getAttribute(d, ORBIT_013, PT_LIST_ATTRIBUTE_ID);

	  // Affichage des facettes.
          Draw_List(((Point_List*)alpha)->Get_Data(), nbPoints);

	  // Marquage des brins de l'orbite ORBIT_013.
	  CCoverage* DCf = G->getDynamicCoverage(d, ORBIT_013);
	  for(DCf->reinit(); DCf->cont(); (*DCf)++)
	    G->setMark(**DCf, markFace);
	  delete DCf;
	}
    }

  // Parcours et démarquage des brins de la G-carte.
  for (C.reinit(); C.cont(); C++)
    G->unsetMark(*C, markFace);
  G->freeMark(markFace);


  // Désactivation de l'éclairage
  glDisable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  glShadeModel(GL_FLAT);
}
