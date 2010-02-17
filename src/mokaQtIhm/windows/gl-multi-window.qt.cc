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

#include "gl-multi-window.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <QtGui/QCloseEvent>
#include <QtGui/QMouseEvent>

using namespace GMap3d ;


//*******************************************************
// Constructeur
//*******************************************************

GLMultiWindow :: GLMultiWindow ( QWorkspace * parent , Window * owner ,
				 GLWindow * share , SelectBar * selection ) :
  GLWindow       ( VIEW_ISO , parent , owner , share , selection ),
  FCliqued       (0),
  FDoubleCliqued (0)
{
  string str = getViewTypeString();
  setWindowTitle(*HTML::decode(&str));
  parent->addWindow(this);
}


//*******************************************************
// Destructeur
//*******************************************************

GLMultiWindow :: ~GLMultiWindow ( ) { }


//*******************************************************
// Accesseurs
//*******************************************************

TViewId GLMultiWindow::getViewId ( ) const
{ 
  return FViewIds[0] ; 
}

string GLMultiWindow::getViewTypeString() const
{ return string("Multi-vue"); }

TViewId GLMultiWindow :: getCliquedViewId ( ) const
{
  assert (0<=FCliqued && FCliqued<=3 );

  return FViewIds[FCliqued];
}

TViewId GLMultiWindow :: getDoubleCliquedViewId ( ) const
{
  assert (0<=FDoubleCliqued && FDoubleCliqued<=3 );

  return FViewIds[FDoubleCliqued];
}

//*****************************************************
// Cadre
//*****************************************************

int GLMultiWindow :: cadre ( int x , int y ) 
{
  int w = width ( ) / 2 ;
  int h = height ( ) / 2 ;

  if ( x < w )
    {
      if ( y >= h )
	return 0 ;
      else
	return 2 ;
    }
  if ( y > h )
    return 1 ;
  return 3 ;
}


//************************************************
// Methode de fermeture
//************************************************

void GLMultiWindow :: closeEvent (  QCloseEvent * e  )
{
 
  FOwner -> getControler ( ) -> viewDestroy ( FViewIds [ 0 ] ) ;
  FOwner -> getControler ( ) -> viewDestroy ( FViewIds [ 1 ] ) ;
  FOwner -> getControler ( ) -> viewDestroy ( FViewIds [ 2 ] ) ;
  FOwner -> getControler ( ) -> viewDestroy ( FViewIds [ 3 ] ) ;
  if ( e != NULL )
    e -> accept ( ) ;
  else
    close ( ) ;
}



//***************************************************
// Methode d'affichages OpenGL
//***************************************************

void GLMultiWindow :: paintGL ( ) 
{
  // On efface et on active le ciseau
  glDisable(GL_SCISSOR_TEST);
  glClearColor(0,0,0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_SCISSOR_TEST);
  glLoadIdentity ( ) ;

  int x = 0 ; int y = height ( ) / 2 ;
  int w = width ( ) / 2 ; int h = height ( ) / 2 ;

  // On dessine les 4 vues bien dans leur cadre
  for ( unsigned int i = 0 ; i < 4 ; i++ )
    {
      if ( i == 1 || i == 3 )
	x += w ;
      else if ( i== 2 )
	{
	  x -= w ;
	  y-= h ;
	}

      glViewport ( x+1 , y+1 , w-2 , h-2 );
      glScissor ( x+1 , y+1 , w-2 , h-2 ) ;

      FOwner -> getControler ( ) -> viewInit(FViewIds[i]);
      FOwner -> getControler ( ) -> viewDraw(FViewIds[i]);
    }

  // swapBuffers ( ) ;
}

//***************************************************
// Methode d'initialisation OpenGL
//***************************************************

void GLMultiWindow :: initializeGL (  ) 
{
  glEnable(GL_DEPTH_TEST); // z-buffer
  glDepthFunc(GL_LEQUAL);

//   glEnable(GL_NORMALIZE);  
  glDisable(GL_BLEND);
  
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
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, Material_Se);
  
  // Couleur des lumieres ---------------------------------------------------
  static const GLfloat Light_Ka[] = {0.1, 0.1, 0.1,  1.0};
  static const GLfloat Light_Kd[] = {1.0, 1.0, 1.0,  1.0};
  static const GLfloat Light_Ks[] = {0.5, 0.5, 0.5,  1.0};

  glLightfv(GL_LIGHT0, GL_AMBIENT , Light_Ka);
  glLightfv(GL_LIGHT0, GL_DIFFUSE , Light_Kd);
  glLightfv(GL_LIGHT0, GL_SPECULAR, Light_Ks);
  //*************************************************************************

  creation ( ) ;
}

//***************************************************
// Initialisation des vues dans le controleur
//***************************************************

void GLMultiWindow :: creation ( )
{
    
  //initializeGL ( ) ;
  
  //   if (FShared==NULL)
  //     initializeGL ();

  if ( FShared==NULL )
    {
      GMap3d :: CControlerGMap* controler = FOwner -> getControler ( ) ;

      FViewIds [ 0 ]  = controler->viewCreate(VIEW_XYZ);
      FViewIds [ 1 ]  = controler->viewCreate(VIEW_XY);
      FViewIds [ 2 ]  = controler->viewCreate(VIEW_XZ);
      FViewIds [ 3 ]  = controler->viewCreate(VIEW_YZ);

      for ( int i = 0 ; i < 4 ; i++ )
	{
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_GRID);
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_AXIS);
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_AIMED_POINT);
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_OBJECT_TRANSFORMATION);
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_PREVIEW);
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_NORMAL_VECTOR);
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_SEW);
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_VERTEX);
#ifdef MODULE_SPAMOD
	  controler->viewAddPrecompile    (FViewIds [ i ], PRECOMPILE_SPAMOD);
	  controler->viewDisablePrecompile(FViewIds [ i ], PRECOMPILE_SPAMOD);
	  controler->getParameterSpamod(FViewIds [ i ])->setViewMode(SPAMOD_NONE);
#endif // MODULE_SPAMOD
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_DART);
	  controler->viewAddPrecompile(FViewIds [ i ], PRECOMPILE_FACE);
	  
	  controler->viewDisablePrecompile(FViewIds [ i ], PRECOMPILE_SEW);
	  
	  controler->viewDisablePrecompile(FViewIds [ i ], PRECOMPILE_VERTEX);
	  
	  controler->viewDisablePrecompile(FViewIds [ i ], PRECOMPILE_FACE);
      
	  controler->viewDisablePrecompile(FViewIds [ i ], PRECOMPILE_NORMAL_VECTOR);
	  
    controler->loadAllParameters(FViewIds [ i ]);
	}
    }
  else
    {
      FViewIds [ 0 ]  = FOwner -> getControler ( )->viewCreateShare(VIEW_XYZ, FShared->getViewId() );
      FViewIds [ 1 ]  = FOwner -> getControler ( )->viewCreateShare(VIEW_XY, FShared->getViewId() );
      FViewIds [ 2 ]  = FOwner -> getControler ( )->viewCreateShare(VIEW_XZ, FShared->getViewId() );
      FViewIds [ 3 ]  = FOwner -> getControler ( )->viewCreateShare(VIEW_YZ, FShared->getViewId() );
    }
}


//**************************************************
// Gestion des evenements souris
//**************************************************


void GLMultiWindow :: mousePressEvent ( QMouseEvent * eb ) 
{
  CControlerGMap * controler = FOwner -> getControler ( ) ;
  int y =  this -> height ( ) - eb -> y ( ) ;

  FCliqued = cadre ( eb -> x() , y );
  
  if ( eb -> modifiers () == Qt :: ControlModifier ) // <CONTROL> key is pressed:
  {
    switch ( eb -> button ( ) )
    {
      case  Qt::LeftButton:  // bouton gauche de la souris

        controler -> 
            operationModeIndependentStart(MODE_OPERATION_SCENE_TRANSLATION ,
                                          FViewIds [ cadre ( eb -> x() , y ) ], 
                                          int ( eb -> x ( ) ) ,
                                          y ) ;
        break;
        case Qt::MidButton :  // bouton milieu
          controler -> 
              operationModeIndependentStart(MODE_OPERATION_SCENE_ROTATION,
                                            FViewIds[ cadre ( eb -> x() , y ) ],
                                            int(eb->x ( ) ),
                                            y );

          break;
          case Qt::RightButton : // bouton droit
            controler -> 
                operationModeIndependentStart(MODE_OPERATION_SCENE_SCALE,
                                              FViewIds[ cadre ( eb -> x() , y ) ],
                                              int(eb->x ( )),
                                              y );

            break;
            default : {}
    }
  }
  else
  {
      // Clic du bouton gauche de la souris
    if ( eb -> button ( ) == Qt :: LeftButton )
    { 
      controler -> setHalfSelectionOrbit 
          ( eb -> modifiers ( ) == Qt :: ShiftModifier ) ;
      controler -> operationModeStart( FViewIds[ cadre ( eb -> x() , y ) ] ,
                                       int(eb->x ( )) ,
                                       y ) ;
    
      CreationObjet * boite = FOwner -> getCreationActive ( ) ;
    
      if ( boite != NULL )
        boite -> update ( ) ;

      dialogOperations * op = FOwner -> getOperationActive ( ) ;
    
      if ( op != NULL )
        op -> update ( ) ;
  
      FOwner -> repaint( ) ;
    }

    // 
    else if ( eb -> button ( ) == Qt::MidButton)
    {
      if (controler->isInCreationMode())
      {
        CreationObjet * boite = FOwner -> getCreationActive ( ) ;
        
        if ( boite != NULL )
          boite -> create ( ) ;
      }
      else
      {
        FSelection -> niveauSuivant ( ) ;
              
        #ifdef MODULE_ARCHITECTURE 
        FOwner -> getFlapSelection() -> nextLevel();
        #endif
      }
    }
    else if ( eb -> button ( ) == Qt :: RightButton )
    { 
      FOwner -> getControler ( ) -> lookAtMouseClick(FViewIds[cadre(eb -> x(),y)],eb -> x(),y);
      repaint();
    }
  }
}



void GLMultiWindow :: mouseMoveEvent ( QMouseEvent * em ) 
{

  int y =  this -> height ( ) - em -> y ( ) ;
  FOwner -> getControler ( ) -> operationModeMove ( int ( em -> x ( ) ), y ) ;
  CreationObjet * boite = FOwner -> getCreationActive ( ) ;
  if ( boite != NULL )
    boite -> update ( ) ;
  
  dialogOperations * op = FOwner -> getOperationActive ( ) ;
  
  if ( op != NULL )
    op -> update ( ) ;

  FOwner -> repaint ( ) ;
  this -> update () ;
}




void GLMultiWindow :: mouseReleaseEvent ( QMouseEvent * eb ) 
{
  int y =  this -> height ( ) - eb -> y () ;
  FOwner -> getControler() -> operationModeStop (int(eb->x()),
  y );
  CreationObjet * boite = FOwner -> getCreationActive ( ) ;
  if ( boite != NULL )
    boite -> update ( ) ;
  
  dialogOperations * op = FOwner -> getOperationActive ( ) ;
  if ( op != NULL )
    op -> update ( ) ;
    
  FOwner -> repaint ( ) ;
  this -> update () ;
  
}



void GLMultiWindow :: mouseDoubleClickEvent ( QMouseEvent * e ) 
{
  if ( e -> button ( ) == Qt::LeftButton )
  {
    int y =  this -> height ( ) - e -> y ( ) ;
    FDoubleCliqued = cadre ( e -> x() , y );
    FOwner -> setDoubleCliquee ( this ) ;
  }
  else
  {
    mousePressEvent(e);
  }
  
}
