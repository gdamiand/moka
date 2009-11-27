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
 *  Fichier  : K2_View.cpp                                                    *
 *  Auteur   : DEXET Martine                                                  *
 *----------------------------------------------------------------------------*
 *  Ce fichier contient l'implémentation des méthodes de la classe K2_View.   *
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

#include "matrix.hh"
#include "k2-view.hh"
#include "user.hh"
#include "matrix-att.hh"

using namespace GMap3d;

/******************************************************************************
 *                              Constructeur                                  *
 *****************************************************************************/

K2_View::K2_View(CGMap * GM)
{
  G = GM;
}


/******************************************************************************
 *  Fonction : void K2View::Calculate_Matrix()                               *
 *----------------------------------------------------------------------------*
 *  Cette fonction recherche pour chaque pointel les liaisons possibles avec  *
 * les autres pointels.                                                       *
 *                                                                            *
 *****************************************************************************/

void K2_View::Calculate_Matrix()
{
  CDart * d;
  int markVoxel = G->getNewMark();

  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      d = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d, markVoxel))
	{

	  // Récupération de la matrice principale.
	  Matrix * mat = ((Matrix_Att *)G
	    ->getAttribute(d, ORBIT_0123, MATRIX_ATTRIBUTE_ID))->Get_Data();

	  // Parcours des cases de la matrice et mise à jour des bits qu'elles
	  // contiennent.

	  // Premier parcours suivant le plan (X,Y).
	  for (int k = mat->Get_Z_Min(); k<mat->Get_Z_Max()+1; k++)
	
	    for (int j = mat->Get_Y_Min(); j< mat->Get_Y_Max()+1; j++)
	
	      for (int i = mat->Get_X_Min(); i< mat->Get_X_Max()+1; i++)
		{
		  if (i!=mat->Get_X_Max())
		
		    if (mat->Get_Color(i+1, j, k) != NO_COLOR)
		      mat->Set_Bit(i, j, k, 2);
		
		    else
		      mat->Unset_Bit(i, j, k, 2);
		
		  else
		    mat->Unset_Bit(i, j, k, 2);
		
		  if (j!=mat->Get_Y_Max())
		
		    if (mat->Get_Color(i, j+1, k) != NO_COLOR)
		      mat->Set_Bit(i, j, k, 3);
		
		    else
		      mat->Unset_Bit(i, j, k, 3);
		
		  else
		    mat->Unset_Bit(i, j, k, 3);
		
		  if (i!=mat->Get_X_Max() && j!=mat->Get_Y_Max())
		
		    if ((mat->Get_Color(i+1, j+1, k) != NO_COLOR) &&
			(mat->Get_Color(i+1, j, k) != NO_COLOR) &&
			(mat->Get_Color(i, j+1, k) != NO_COLOR))
		      mat->Set_Bit(i, j, k, 4);
		
		    else
		      mat->Unset_Bit(i, j, k, 4);
		
		  else
		    mat->Unset_Bit(i, j, k, 4);
		}
	
	  // Deuxième parcours suivant le plan (Z,Y).
	  for (int i = mat->Get_X_Min(); i<mat->Get_X_Max()+1; i++)
	
	    for (int j = mat->Get_Y_Min(); j< mat->Get_Y_Max()+1; j++)
	
	      for (int k = mat->Get_Z_Min(); k< mat->Get_Z_Max()+1; k++)
		{
		  if (k!=mat->Get_Z_Max())
		
		    if (mat->Get_Color(i, j, k+1) != NO_COLOR)
		      mat->Set_Bit(i, j, k, 5);
		
		    else
		      mat->Unset_Bit(i, j, k, 5);
		
		  else
		    mat->Unset_Bit(i, j, k, 5);
		
		  if (k!=mat->Get_Z_Max() && j!=mat->Get_Y_Max())
		
		    if ((mat->Get_Color(i, j+1, k+1) != NO_COLOR) &&
			(mat->Get_Color(i, j, k+1) != NO_COLOR) &&
			(mat->Get_Color(i, j+1, k) != NO_COLOR))
		      mat->Set_Bit(i, j, k, 6);
		
		    else
		      mat->Unset_Bit(i, j, k, 6);
		
		  else
		    mat->Unset_Bit(i, j, k, 6);
		}
	
	  // Troisième parcours suivant le plan (X,Z).
	  for (int j = mat->Get_Y_Min(); j<mat->Get_Y_Max()+1; j++)
	
	    for (int k = mat->Get_Z_Min(); k< mat->Get_Z_Max()+1; k++)
	
	      for (int i = mat->Get_X_Min(); i< mat->Get_X_Max()+1; i++)
		{
		  if (k!=mat->Get_Z_Max() && i!=mat->Get_X_Max())
		
		    if ((mat->Get_Color(i+1, j, k+1) != NO_COLOR) &&
			(mat->Get_Color(i, j, k+1) != NO_COLOR) &&
			(mat->Get_Color(i+1, j, k) != NO_COLOR))
		      mat->Set_Bit(i, j, k, 7);

		    else
		      mat->Unset_Bit(i, j, k, 7);
		
		  else
		    mat->Unset_Bit(i, j, k, 7);
		}
	  CCoverage* DC = G->getDynamicCoverage(d, ORBIT_0123);
	  for (DC->reinit(); DC->cont(); (*DC) ++)
	    G->setMark(**DC, markVoxel);
	  delete DC;
	}
    }
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    G->unsetMark(*Cgm, markVoxel);


  G->freeMark(markVoxel);
}


/******************************************************************************
 *  Fonction : void K2View::Draw_Pointels()                                  *
 *----------------------------------------------------------------------------*
 *  Cette fonction affiche les pointels de la vue K2 d'un objet.              *
 *                                                                            *
 *****************************************************************************/

void K2_View::Draw_Pointels()
{
  CDart * d;
  int markVoxel = G->getNewMark();

  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      d = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d, markVoxel))
	{
	
	  // Récupération de la matrice principale.
	  CAttribute *alpha = G->getAttribute(d, ORBIT_0123, MATRIX_ATTRIBUTE_ID);
	  Matrix * mat = ((Matrix_Att*)alpha)->Get_Data();
	
	  // Affichage des pointels.
	  glPointSize(4.0);
	  glBegin(GL_POINTS);
	
	  for (int i=mat->Get_X_Min() ; i<=mat->Get_X_Max() ; i++)
	    for (int j=mat->Get_Y_Min() ; j<=mat->Get_Y_Max() ; j++)
	      for (int k=mat->Get_Z_Min() ; k<=mat->Get_Z_Max() ; k++)
		
		if (mat->Get_Color(i,j,k) != NO_COLOR)
		  glVertex3f(i, j, k);
	  glEnd();
	  glPointSize(1.0);

	  CCoverage* DC = G->getDynamicCoverage(d, ORBIT_0123);
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
 *  Fonction : void K2View::Draw_Lignels()                                   *
 *----------------------------------------------------------------------------*
 *  Cette fonction affiche les lignels de la vue K2 d'un objet.               *
 *                                                                            *
 *****************************************************************************/

void K2_View::Draw_Lignels()
{
  CDart * d;
  int markVoxel = G->getNewMark();

  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      d = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d, markVoxel))
	{

	  // Récupération de la matrice principale.
	  CAttribute *alpha = G->getAttribute(d, ORBIT_0123, MATRIX_ATTRIBUTE_ID);
	  Matrix * mat = ((Matrix_Att*)alpha)->Get_Data();
	
	  // Affichage des lignels.
	  glLineWidth(2.0);
	  glBegin(GL_LINES);

	  for (int i=mat->Get_X_Min() ; i<=mat->Get_X_Max() ; i++)
	    for (int j=mat->Get_Y_Min() ; j<=mat->Get_Y_Max() ; j++)
	      for (int k=mat->Get_Z_Min() ; k<=mat->Get_Z_Max() ; k++)
		
		if (mat->Get_Color(i,j,k) != NO_COLOR)
		  {
		    if (mat->Get_Bit(i, j, k, 2))
		      {
			glVertex3f(i, j, k);
			glVertex3f(i+1, j, k);
		      }
		
		    if (mat->Get_Bit(i, j, k, 3))
		      {
			glVertex3f(i, j, k);
			glVertex3f(i, j+1, k);
		      }
		
		    if (mat->Get_Bit(i, j, k, 5))
		      {
			glVertex3f(i, j, k);
			glVertex3f(i, j, k+1);
		      }
		  }
	  glEnd();
	  glLineWidth(1.0);

	  CCoverage* DC = G->getDynamicCoverage(d, ORBIT_0123);
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
 *  Fonction : void K2View::Draw_Surfels()                                   *
 *----------------------------------------------------------------------------*
 *  Cette fonction affiche les surfels de la vue K2 d'un objet.               *
 *                                                                            *
 *****************************************************************************/

void K2_View::Draw_Surfels()
{
  CDart * d;
  int markVoxel = G->getNewMark();

  // Activation de l'éclairage.
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  CDynamicCoverageAll Cgm(G);
  for (Cgm.reinit(); Cgm.cont(); Cgm++)
    {
      d = *Cgm;

      // Si le brin n'est pas marqué.
      if (!G->isMarked(d, markVoxel))
	{

	  // Récupération de la matrice principale.
	  CAttribute *alpha = G->getAttribute(d, ORBIT_0123, MATRIX_ATTRIBUTE_ID);
	  Matrix * mat = ((Matrix_Att*)alpha)->Get_Data();
	
	  // Affichage des surfels.
	  glBegin(GL_QUADS);
	
	  for (int i=mat->Get_X_Min() ; i<=mat->Get_X_Max() ; i++)
	    for (int j=mat->Get_Y_Min() ; j<=mat->Get_Y_Max() ; j++)
	      for (int k=mat->Get_Z_Min() ; k<=mat->Get_Z_Max() ; k++)
		
		if (mat->Get_Color(i,j,k) != NO_COLOR)
		  {
		    if (mat->Get_Bit(i, j, k, 4))
		      {
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(i+0.2, j+0.2, k);
			glVertex3f(i+0.8, j+0.2, k);
			glVertex3f(i+0.8, j+0.8, k);
			glVertex3f(i+0.2, j+0.8, k);
		      }
		    if (mat->Get_Bit(i, j, k, 6))
		      {
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3f(i, j+0.2, k+0.2);
			glVertex3f(i, j+0.8, k+0.2);
			glVertex3f(i, j+0.8, k+0.8);
			glVertex3f(i, j+0.2, k+0.8);
		      }
		    if (mat->Get_Bit(i, j, k, 7))
		      {
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(i+0.2, j, k+0.2);
			glVertex3f(i+0.2, j, k+0.8);
			glVertex3f(i+0.8, j, k+0.8);
			glVertex3f(i+0.8, j, k+0.2);
		      }
		  }
	  glEnd();
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
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}
