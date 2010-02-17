/*
 * Moka : Un modeleur de 3-G-cartes.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of Moka
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

#include "gl-window.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QCloseEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

using namespace GMap3d ;

//*****************************************************
// Constructeurs
//*****************************************************
GLWindow :: GLWindow(TView AViewType , QWorkspace * parent ,
                     Window * owner , SelectBar * selection) :
      QGLWidget(QGLFormat(QGL::SampleBuffers), parent , 0, Qt::SubWindow),
      FViewType(AViewType),
      FShared(NULL),
      isShared(false),
      FOwner(owner),
      FSelection(selection),
      FDragMode(false)
{
   string str = getViewTypeString();
   setWindowTitle(*HTML::decode(&str));
   parent->addWindow(this);
   setAcceptDrops(true);
}

GLWindow :: GLWindow(TView AViewType , QWorkspace * parent ,
                     Window * owner , GLWindow * share ,
                     SelectBar * selection) :
      QGLWidget(parent, share , Qt::SubWindow) ,
      FViewType(AViewType) ,
      FShared(share) ,
      isShared(true) ,
      FOwner(owner),
      FSelection(selection),
      FDragMode(false)
{
   string str = getViewTypeString();
   setWindowTitle(*HTML::decode(&str));
   parent->addWindow(this);
   setAcceptDrops(true);
}
//******************************************************
// Destructeur
//******************************************************
GLWindow :: ~GLWindow() { }
//******************************************************
// Accesseurs
//******************************************************
TView GLWindow :: getViewType() const
{
   return FViewType;
}

TViewId GLWindow :: getViewId() const
{
   return FViewId;
}

string GLWindow::getViewTypeString() const
{
   switch (FViewType)
   {
      case VIEW_XYZ: return string("Vue 3D");
      case VIEW_XY:  return string("Vue XY");
      case VIEW_XZ:  return string("Vue XZ");
      case VIEW_YZ:  return string("Vue YZ");
   }

   return string("");
}

TViewId GLWindow :: getCliquedViewId() const
{ return FViewId; }

TViewId GLWindow :: getDoubleCliquedViewId() const
{ return FViewId; }
//********************************************************
// Fermeture fenetre
//********************************************************
void GLWindow::closeEvent(QCloseEvent * e)
{
   // Si c'est la vue mere, on empeche sa fermeture
   if (FViewId == 0)
      if (e != NULL)
         e -> ignore() ;
      else
         return ;
   else
   {
      // Sinon on la detruit
      FOwner->getControler()->viewDestroy(FViewId) ;
      if (e != NULL)
         e -> accept() ;
      else
         close() ;
   }
}
//************************************************************
// Dessin de la scene
//************************************************************
void GLWindow::paintGL()
{
  QPainter p(this);

  makeCurrent();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Reset OpenGL parameters
  glEnable(GL_DEPTH_TEST); // z-buffer
  glDepthFunc(GL_LEQUAL);
  //  resizeGL(width(), height());  
  
  glClearColor(0,0,0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity() ;
  
  FOwner -> getControler() -> viewInit(FViewId);  
  FOwner -> getControler() -> viewDraw(FViewId) ;
  
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  if (FDragMode)
    {
      p.setPen(Qt::white) ;
      p.drawRect(FStartX, FStartY, FCurX - FStartX, FCurY - FStartY);
      if ( FOwner->getControler()->getModeDeselectionAtStop() )
	p.drawText(FStartX, FStartY, "Deselect");
      else
	p.drawText(FStartX, FStartY, "Select");
    }
  p.end();
}
//************************************************************
// Modif de la taille de la fenetre
//************************************************************
void GLWindow :: resizeGL(int width , int height)
{
  glViewport(0,0,width,height);
  
//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();
//   glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
//   glMatrixMode(GL_MODELVIEW);
 }
//************************************************************
// Initialisation OpenGL
//************************************************************
void GLWindow :: initializeGL()
{
  glEnable(GL_DEPTH_TEST); // z-buffer
  glDepthFunc(GL_LEQUAL);

  // glEnable(GL_NORMALIZE);

  glDisable(GL_BLEND);
  // glEnable(GL_CULL_FACE);
   
   glEnable(GL_POLYGON_MODE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   ::glEnable(GL_POLYGON_OFFSET_FILL);
  ::glPolygonOffset(1.0f,1.0f);

   // Proprietes des surfaces ----------------------------------------------
   static const GLfloat Material_Ks[] = {0.5, 0.5, 0.5,  1.0};
   static const GLfloat Material_Ke[] = {0.0, 0.0, 0.0,  1.0};
   static const GLfloat Material_Se = 100;

   glEnable(GL_COLOR_MATERIAL);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR , Material_Ks);
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION , Material_Ke);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Material_Se);

   // Couleur des lumieres ----------------------------------------------------
   static const GLfloat Light_Ka[] = {0.1, 0.1, 0.1,  1.0};
   static const GLfloat Light_Kd[] = {1.0, 1.0, 1.0,  1.0};
   static const GLfloat Light_Ks[] = {0.5, 0.5, 0.5,  1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT , Light_Ka);
   glLightfv(GL_LIGHT0, GL_DIFFUSE , Light_Kd);
   glLightfv(GL_LIGHT0, GL_SPECULAR, Light_Ks);
   //**************************************************************************

   creation() ;
}
//************************************************************
// Creation de la vue dans le controleur
//************************************************************
void GLWindow :: creation()
{
   if (FShared == NULL)
   {
      GMap3d :: CControlerGMap* controler = FOwner -> getControler() ;

      FViewId = controler->viewCreate(FViewType);

      controler->viewAddPrecompile(FViewId, PRECOMPILE_GRID);
      controler->viewAddPrecompile(FViewId, PRECOMPILE_AXIS);
      controler->viewAddPrecompile(FViewId, PRECOMPILE_AIMED_POINT);
      controler->viewAddPrecompile(FViewId,
                                   PRECOMPILE_OBJECT_TRANSFORMATION);
      controler->viewAddPrecompile(FViewId, PRECOMPILE_PREVIEW);
      controler->viewAddPrecompile(FViewId, PRECOMPILE_NORMAL_VECTOR);
      controler->viewAddPrecompile(FViewId, PRECOMPILE_SEW);
      controler->viewAddPrecompile(FViewId, PRECOMPILE_VERTEX);
#ifdef MODULE_SPAMOD
      controler->viewAddPrecompile(FViewId, PRECOMPILE_SPAMOD);
      controler->viewDisablePrecompile(FViewId, PRECOMPILE_SPAMOD);
      controler->getParameterSpamod(FViewId)->setViewMode(SPAMOD_NONE);
#endif // MODULE_SPAMOD
      controler->viewAddPrecompile(FViewId, PRECOMPILE_DART);
      controler->viewAddPrecompile(FViewId, PRECOMPILE_FACE);

      controler->viewDisablePrecompile(FViewId, PRECOMPILE_SEW);
      controler->viewDisablePrecompile(FViewId, PRECOMPILE_VERTEX);
      controler->viewDisablePrecompile(FViewId, PRECOMPILE_FACE);
      controler->viewDisablePrecompile(FViewId, PRECOMPILE_NORMAL_VECTOR);

      // On charge les paramètres de l'utilisateur
      controler->loadAllParameters(FViewId);

      // Sauf pour le point de vue et de la position de l'oeil
      // pour la vue mère (i.e. la première fois)
      controler->getParameterAimedPosition(FViewId)->reinit();
      controler->getParameterEyePosition(FViewId)->reinit();
   }
   else
   {
      FViewId = FOwner -> getControler() ->
                viewCreateShare(FViewType , FShared->FViewId) ;
   }
}
//*************************************************************
// Gestion des evenements
//*************************************************************
void GLWindow :: mousePressEvent(QMouseEvent * eb)
{
   CControlerGMap * controler = FOwner -> getControler() ;
   int y =  this -> height() - eb -> y() ;

   if (eb->modifiers() == Qt::ControlModifier) // <CONTROL> key is pressed:
   {
      switch (eb -> button())
      {
         case  Qt::LeftButton:  // bouton gauche de la souris

            controler ->
            operationModeIndependentStart(MODE_OPERATION_SCENE_TRANSLATION ,
                                          FViewId, int (eb -> x()), y) ;
            break;
         case Qt::MidButton :  // bouton milieu
            controler ->
            operationModeIndependentStart(MODE_OPERATION_SCENE_ROTATION,
                                          FViewId, int(eb->x()), y);
            break;
         case Qt::RightButton : // bouton droit
            controler ->
            operationModeIndependentStart(MODE_OPERATION_SCENE_SCALE,
                                          FViewId, int(eb->x()), y);
            break;
         default : {}
      }
   }
   else
   {
      // Clic du bouton gauche de la souris
      if (eb -> button() == Qt :: LeftButton)
      {
         controler->
         setHalfSelectionOrbit(eb -> modifiers() == Qt :: ShiftModifier) ;
         controler->operationModeStart(FViewId, int(eb->x()), y) ;

         CreationObjet * boite = FOwner -> getCreationActive() ;

         if (boite != NULL)
            boite -> update() ;

         dialogOperations * op = FOwner -> getOperationActive() ;

         if (op != NULL)
            op -> update() ;

         OptionsCarac * carac = FOwner -> getOptionsCaracActive() ;
         if (carac != NULL)
            carac -> update() ;

         FOwner -> repaint() ;

	 if (controler->getMode() == MODE_OPERATION_SELECTION)
	   {
	     FStartX = int (eb -> x()); FStartY = int (eb -> y());
	     FCurX = FStartX; FCurY = FStartY;
	     FDragMode = true;
	     
	     if (eb -> modifiers() == Qt :: ShiftModifier)
	       controler -> setModeDeselectionAtStop();
	   }
      }
      else if (eb -> button() == Qt::MidButton)
      {
         if (controler->isInCreationMode())
         {
            CreationObjet * boite = FOwner -> getCreationActive() ;

            if (boite != NULL)
               boite -> create() ;
         }
         else
         {
            FSelection -> niveauSuivant() ;
#ifdef MODULE_ARCHITECTURE
            FOwner -> getFlapSelection() -> nextLevel();
#endif
         }
      }
      else if (eb -> button() == Qt :: RightButton)
      {
         FOwner -> getControler() -> lookAtMouseClick(FViewId, eb -> x(), y);
         repaint();
      }
   }
}

void GLWindow :: mouseMoveEvent(QMouseEvent * em)
{
   int y =  this -> height() - em -> y() ;
   FOwner -> getControler() -> operationModeMove(int (em -> x()), y) ;
   CreationObjet * boite = FOwner -> getCreationActive() ;
   if (boite != NULL)
      boite -> update() ;

   dialogOperations * op = FOwner -> getOperationActive() ;

   if (op != NULL)
      op -> update() ;

   if (FDragMode)
   { FCurX = int (em -> x()); FCurY = int (em -> y()); }

   FOwner -> repaint() ;
   this -> update() ;
}

void GLWindow :: mouseReleaseEvent(QMouseEvent * eb)
{
   CControlerGMap * controler = FOwner -> getControler();
   FDragMode = false;
   int y =  this -> height() - eb -> y() ;

   controler -> operationModeStop(int(eb->x()), y);

   CreationObjet * boite = FOwner -> getCreationActive() ;
   if (boite != NULL) boite -> update() ;

   dialogOperations * op = FOwner -> getOperationActive() ;
   if (op != NULL) op -> update() ;

   FOwner -> repaint() ;
   this -> update() ;
}

void GLWindow :: mouseDoubleClickEvent(QMouseEvent * e)
{
   if (e -> button() == Qt::LeftButton)
   {
      FOwner -> setDoubleCliquee(this) ;
   }
   else
   {
      mousePressEvent(e);
   }
}
