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

//******************************************************************************
#include "spamod-api.hh"

#include "g-map.hh"

#include "topologic-view.hh"
#include "analytic-view.hh"
#include "euclidian-view.hh"
#include "voxel-view.hh"
#include "k2-view.hh"

#include "color-table-att.hh"
#include "user.hh"
#include "gmap-ops.hh"
#include "ineq-op.hh"

//#include "opengl-api.hh"

using namespace GMap3d;

//******************************************************************************
CSpamodAPI::CSpamodAPI(CGMap * AMap)
{
  assert(AMap != NULL);

  FMap = AMap;
  colors = NULL;
  FViewMode = SPAMOD_NONE;

  EV = new Euclidian_View(FMap);
  AV = new Analytic_View(FMap);
  TV = new Topologic_View(FMap);
  VV = new Voxel_View(FMap);
  KV = new K2_View(FMap);

  FEuclidianParam[0] = true;
  FEuclidianParam[1] = true;

  FAnalyticParam [0] = true;
  FAnalyticParam [1] = true;
  FAnalyticParam [2] = true;
  FAnalyticParam [3] = true;

  FVoxelParam 	 [0] = true;
  FVoxelParam 	 [1] = true;
  FVoxelParam 	 [2] = true;

  FK2Param 	 [0] = true;
  FK2Param 	 [1] = true;
  FK2Param 	 [2] = true;
}
//******************************************************************************
CSpamodAPI::~CSpamodAPI()
{
  delete EV;
  delete AV;
  delete TV;
  delete VV;
  delete KV;
}
//******************************************************************************
void CSpamodAPI::computeSpamodEmbeddings()
{
  for (int orbit = ORBIT_SELF; orbit <= ORBIT_CC; ++orbit)
    FUsedOrbits[orbit] = FMap->isOrbitUsed(orbit);

  FMap->setOrbitUsed(ORBIT_0123);
  FMap->setOrbitUsed(ORBIT_123);
  FMap->setOrbitUsed(ORBIT_023);
  FMap->setOrbitUsed(ORBIT_013);
  FMap->setOrbitUsed(ORBIT_01);
  FMap->setOrbitUsed(ORBIT_0);
  FMap->setOrbitUsed(ORBIT_012);
  FMap->setOrbitUsed(ORBIT_SELF);

  Init_Gmap(FMap);

  colors =
    ((Color_Table_Att *) Find_Attribute(FMap, ORBIT_0123,
					COLOR_TABLE_ATTRIBUTE_ID))->Get_Data();

  TV->Create_Darts_View();
  Create_Ineq(FMap);
  Choose_Ineq(FMap);
  AV->Create_Vertices_Faces();
  AV->Create_Edges_Faces();
  AV->Create_Faces();
  VV->Create_Matrix();
  KV->Calculate_Matrix();
}

//******************************************************************************
void CSpamodAPI::deleteSpamodEmbeddings()
{
  CAttribute * attr;

  for (CDynamicCoverageAll dgm(FMap); dgm.cont(); dgm++)
    {
      // ORBIT_SELF, VERTEX_ATTRIBUTE_ID:
      attr = (*dgm)->removeAttribute(ORBIT_SELF, VERTEX_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_0, USE_ATTRIBUTE_ID:
      attr = (*dgm)->removeAttribute(ORBIT_0, USE_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_123, VOXEL_LIST_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_123, VOXEL_LIST_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_VERTEX, INEQ6_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_123, INEQ6_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_VERTEX, PT_LIST_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_123, PT_LIST_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_EDGE, INEQ6_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_023, INEQ6_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_EDGE, PT_LIST_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_023, PT_LIST_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_EDGE, VOXEL_LIST_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_023, VOXEL_LIST_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_FACE, INT_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_013, INT_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_FACE, INEQ2_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_013, INEQ2_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_FACE, PT_LIST_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_013, PT_LIST_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_FACE, VOXEL_LIST_ATTRIBUTE_ID
      attr = (*dgm)->removeAttribute(ORBIT_013, VOXEL_LIST_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_CC, MATRIX_ATTRIBUTE_ID:
      attr = (*dgm)->removeAttribute(ORBIT_0123, MATRIX_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;

      // ORBIT_CC, COLOR_TABLE_ATTRIBUTE_ID:
      attr = (*dgm)->removeAttribute(ORBIT_0123, COLOR_TABLE_ATTRIBUTE_ID);
      if (attr != NULL) delete attr;
    }

  for (int orbit = ORBIT_SELF; orbit <= ORBIT_CC; ++orbit)
    if (FMap->isOrbitUsed(orbit) && ! FUsedOrbits[orbit])
      FMap->unsetOrbitUsed(orbit);

  colors = NULL;
}
//******************************************************************************
void CSpamodAPI::setViewMode(TSpamodViewMode AMode)
{
  if (FViewMode == AMode)
    return;

  TSpamodViewMode oldMode = FViewMode;
  FViewMode = AMode;

  if (FViewMode == SPAMOD_NONE)
    // Libération des attributs Spamod:
    deleteSpamodEmbeddings();
  else
    if (oldMode == SPAMOD_NONE)
      // Calcul des attributs SpaMod:
      computeSpamodEmbeddings();
}
//******************************************************************************
TSpamodViewMode CSpamodAPI::getViewMode() const
{
  return FViewMode;
}
//******************************************************************************
bool CSpamodAPI::getEuclidianParam(int i)
{
  assert(0<=i && i<=1);
  return FEuclidianParam[i];
}
//******************************************************************************
void CSpamodAPI::setEuclidianParam(int i, bool val)
{
  assert(0<=i && i<=1);
  FEuclidianParam[i] = val;
}
//******************************************************************************
bool CSpamodAPI::getAnalyticParam(int i)
{
  assert(0<=i && i<=3);
  return FAnalyticParam[i];
}
//******************************************************************************
void CSpamodAPI::setAnalyticParam(int i, bool val)
{
  assert(0<=i && i<=3);
  FAnalyticParam[i] = val;
}
//******************************************************************************
bool CSpamodAPI::getVoxelParam(int i)
{
  assert(0<=i && i<=2);
  return FVoxelParam[i];
}
//******************************************************************************
void CSpamodAPI::setVoxelParam(int i, bool val)
{
  assert(0<=i && i<=2);
  FVoxelParam[i] = val;
}
//******************************************************************************
bool CSpamodAPI::getK2Param(int i)
{
  assert(0<=i && i<=2);
  return FK2Param[i];
}
//******************************************************************************
void CSpamodAPI::setK2Param(int i, bool val)
{
  assert(0<=i && i<=2);
  FK2Param[i] = val;
}
//******************************************************************************
void CSpamodAPI::drawMap()
{
  Color * color;

  // Propriétés des surfaces.
  GLfloat Material_Ks[] = {0.5, 0.5, 0.5, 1.0};
  GLfloat Material_Ke[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat Material_Se = 200;

  // Couleur des lumières.
  GLfloat Light_Ka[] = {0.1, 0.1, 0.1, 1.0};
  GLfloat Light_Kd[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat Light_Ks[] = {0.5, 0.5, 0.5, 1.0};

  GLfloat Light_Kambient[] = {0.4, 0.4, 0.4, 1.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,Light_Kambient);

  //glPushAttrib(GL_ALL_ATTRIB_BITS);

  // Active le test de profondeur.
  //glEnable(GL_DEPTH_TEST);
  //Active l'affectation simplifiée de couleurs.
  glEnable(GL_COLOR_MATERIAL);
  // Allume la lumière n°1.
  glEnable(GL_LIGHT0);
  // Allume la lumière n°2.
  //glEnable(GL_LIGHT1);

  //GLfloat LightPos0[] = {1000,1000,1000,1};
  //GLfloat LightPos1[] = {-1000,-1000,-1000,1};


  //glLightfv(GL_LIGHT0,GL_POSITION,LightPos0);
  //glLightfv(GL_LIGHT1,GL_POSITION,LightPos1);

  // Active l'éclairage des deux côtés des polygones.
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  // Couleur de fond
  //glClearColor(0.0, 0.0, 0.0, 1.0);

  // Mode de transparence à utiliser lors de l'activation de la transparence.
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Modification des propriétés de la matière courante.
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Material_Ks);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Material_Ke);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material_Se);

  // Modification de la couleur de la lumière n°1.
  glLightfv(GL_LIGHT0,GL_AMBIENT, Light_Ka);
  glLightfv(GL_LIGHT0,GL_DIFFUSE, Light_Kd);
  glLightfv(GL_LIGHT0,GL_SPECULAR, Light_Ks);

  // Modification de la couleur de la lumière n°2.
  glLightfv(GL_LIGHT1,GL_AMBIENT, Light_Ka);
  glLightfv(GL_LIGHT1,GL_DIFFUSE, Light_Kd);
  glLightfv(GL_LIGHT1,GL_SPECULAR, Light_Ks);

  switch (FViewMode)
    {
    case SPAMOD_TOPOLOGICAL:
      {
	glPointSize(3.0);

	// Affichage des brins.
	glColor3f(1.0, 1.0, 0.0);

	// Creation de l'affichage Topologique (brins).
	TV->Darts_Draw();
	
	glPointSize(1.0);
	
	glLineWidth(2.0);

	// Affichage des liaisons alpha0.
	//if (parameters[0] == true)
	//{
	glColor3f(1.0, 0.0, 0.0);
	// Creation de l'affichage Topologique (alpha i).
	TV->Alpha_Draw(0);
	//}

	// Affichage des liaisons alpha1.
	//if (parameters[1] == true)
	//{
	glColor3f(0.0, 0.8, 0.0);
	// Creation de l'affichage Topologique (alpha i).
	TV->Alpha_Draw(1);
	//}

	// Affichage des liaisons alpha2.
	//if (parameters[2] == true)
	//{
	glColor3f(0.0, 0.0, 1.0);
	// Creation de l'affichage Topologique (alpha i).
	TV->Alpha_Draw(2);
	//}

	// Affichage des liaisons alpha3.
	//if (parameters[3] == true)
	//{
	glColor3f(0.7, 0.7, 0.7);
	// Creation de l'affichage Topologique (alpha i).
	TV->Alpha_Draw(3);
	//}

	glLineWidth(1.0);
      }
      break;
    case SPAMOD_EUCLIDIAN:
      {


	if (FEuclidianParam[0])
	  EV->Wire_Draw();



	if (FEuclidianParam[1])
	  {
	    // glEnable(GL_BLEND);
	    // glDepthMask(GL_FALSE);

	    color = colors->Get_Color(2);

	    glColor4f(color->Get_R(),
		      color->Get_G(),
		      color->Get_B(),
		      0.7);
	
	    EV->Solid_Draw();

	    // glDepthMask(GL_TRUE);
	    // glDisable(GL_BLEND);
	  }

      }
      break;
    case SPAMOD_ANALYTIC:
      {
       	bool translucid = getAnalyticParam(3);

	//GL::enableLight();

	if (getAnalyticParam(0))
	  {
	    color = colors->Get_Color(0);
	    glColor4f(color->Get_R(), color->Get_G(), color->Get_B(),
		      translucid ? 0.5 : 1.0);
	    AV->Vertices_Draw();
	  }

	if (getAnalyticParam(1))
	  {
	    color = colors->Get_Color(1);
	    glColor4f(color->Get_R(), color->Get_G(), color->Get_B(),
		      translucid ? 0.5 : 1.0);
	    AV->Edges_Draw();
	  }

	if (getAnalyticParam(2))
	  {
	    if (translucid)
	      {
		// FEuclidianView->Solid_Draw(FColors);
		// glEnable(GL_BLEND);
		// glDepthMask(GL_FALSE);
	      }

	    color = colors->Get_Color(2);
	    glColor4f(color->Get_R(), color->Get_G(), color->Get_B(),
		      translucid ? 0.5 : 1.0);
	    AV->Faces_Draw();

	    if (translucid)
	      {
		// glDepthMask(GL_TRUE);
		// glDisable(GL_BLEND);
	      }
	  }

      }
      break;
    case SPAMOD_VOXEL:
      {
	//GL::enableLight();

	if (getVoxelParam(0))
	  {
	    color = colors->Get_Color(0);
	    glColor3f(color->Get_R(), color->Get_G(), color->Get_B());
	    VV->Draw_Vertices();
	  }

	if (getVoxelParam(1))
	  {
	    color = colors->Get_Color(1);
	    glColor3f(color->Get_R(), color->Get_G(), color->Get_B());
	    VV->Draw_Edges();
	  }

	if (getVoxelParam(2))
	  {
	    color = colors->Get_Color(2);
	    glColor3f(color->Get_R(), color->Get_G(), color->Get_B());
	    VV->Draw_Faces();
	  }
      }
      break;
    case SPAMOD_K2:
      {
	if (getK2Param(0))
	  {
	    color = colors->Get_Color(0);
	    //GL::disableLight();
	    glColor3f(color->Get_R(), color->Get_G(), color->Get_B());
	    KV->Draw_Pointels();
	  }

	if (getK2Param(1))
	  {
	    color = colors->Get_Color(3);
	    //GL::disableLight();
	    glColor3f(color->Get_R(), color->Get_G(), color->Get_B());
	    KV->Draw_Lignels();
	  }

	if (getK2Param(2))
	  {
	    // glEnable(GL_BLEND);
	    // glDepthMask(GL_FALSE);

	    color = colors->Get_Color(2);
	    //GL::enableLight();
	    glColor4f(color->Get_R(), color->Get_G(), color->Get_B(), 0.8);
	    KV->Draw_Surfels();

	    // glDepthMask(GL_TRUE);
	    // glDisable(GL_BLEND);
	  }
      }
      break;
    default:
      {
      }
    }
  //glDisable(GL_LIGHT0);
  //glDisable(GL_LIGHT1);

  //glPopAttrib();

}
//******************************************************************************
