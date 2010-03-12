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

#include "creation-brin.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QPixmap>
#include <QtGui/QLabel>


/*******************************************************************/
/* CLASSE creationBrin                                             */
/*******************************************************************/

// Constructeur
//-------------

CreationBrin :: CreationBrin ( Window * parent ,
			       GMap3d :: CControlerGMap * controler ) :
  CreationObjet(parent , HTML::decode("Creation Dart/Polyline/Face" ),
                controler ),
  FSouris ( false )
{
  //************ Creation des widgets ***********
  
  FAffX = new QLabel ( "  X : " ) ;
  FSaisieX = new FloatSpinBox ( 0.5 , -1000 ,1000 , 2 , 0.1 ) ;
  FAffY = new QLabel ( "  Y : " ) ;
  FSaisieY = new FloatSpinBox ( 0.5 , -1000 ,1000 , 2 , 0.1 ) ;
  FAffZ = new QLabel ( "  Z : " ) ;
  FSaisieZ = new FloatSpinBox ( 0.5 , -1000 ,1000 , 2 , 0.1 ) ;

  addWidget(new QLabel ( "  Position : " ));
  addWidget(FAffX);
  addWidget(FSaisieX);
  addWidget(FAffY);
  addWidget(FSaisieY);
  addWidget(FAffZ);
  addWidget(FSaisieZ);
  addWidget(new SpaceWidget(SPACE_PIX_SIZE,1));
  
  //*********** Creation des boutons **************
  QIcon p1 ( ":/boutonCreer.png" ) ;
  QIcon p2 ( ":/boutonSupprimer.png" ) ;
  
  FNouveau = new QPushButton ( p1 , *HTML::decode("Create new") , 
                               this ) ;
  FSupprimer = new QPushButton ( p2 , "Delete last" , this ) ;
  
  QObject::connect ( FNouveau , SIGNAL ( clicked ( ) ) , this,
 		     SLOT ( prolong ( ) ) ) ;
  QObject::connect ( FSupprimer , SIGNAL ( clicked ( ) ) , this,
		     SLOT (shorten ( ) ) ) ;
  
  addWidget(FNouveau);
  addWidget(FSupprimer);
  addWidget(new SpaceWidget(SPACE_PIX_SIZE,1));
  
  FChoix = new BoutonsCreation ( this ) ;  
  FChoix -> getOptions ( ) -> setEnabled ( false ) ;

  clearLastVertex( ) ;
  FChoix -> reinit ( ) ;
  update ( ) ;
  
   // Ecoutes
  QObject::connect ( FSaisieX , SIGNAL ( valueChanged ( double ) ) , this,
                     SLOT ( lastVertexChanged ( ) ) ) ;
  QObject::connect ( FSaisieY , SIGNAL ( valueChanged ( double ) ) , this,
                     SLOT ( lastVertexChanged ( ) ) ) ;
  QObject::connect ( FSaisieZ , SIGNAL ( valueChanged ( double ) ) , this,
                     SLOT ( lastVertexChanged ( ) ) ) ;
}

// Destructeur
//------------

CreationBrin :: ~CreationBrin ( ) { }

void CreationBrin :: update ( ) 
{
  FSouris = true ;
  if ( FControler -> getParameterPolylinePosition() -> getNbVertices()  > 0 )
    setNewVertex ( FControler -> 
  		   getParameterPolylinePosition() -> 
  		   getLastVertex() );
  else
    clearLastVertex();
  FSouris = false ;
}


// Accesseurs en ecriture
//------------------------
void CreationBrin :: setNewVertex ( const CVertex & v )
{
  // On se met en mode souris
  FSouris = true ;

  FSaisieX -> setValuef ( v . getX ( ) ) ;
  FSaisieY -> setValuef ( v . getY ( ) ) ;
  FSaisieZ -> setValuef ( v . getZ ( ) ) ;

  FSouris = false ;
}

void CreationBrin :: clearLastVertex ( )
{
  FSaisieX -> setValuef ( 0 ) ;
  FSaisieY -> setValuef ( 0 ) ;
  FSaisieZ -> setValuef ( 0 ) ;
}

void CreationBrin :: lastVertexChanged ( )
{
  if ( ! FSouris )
  {
    CVertex vertex ( FSaisieX -> valuef ( ) ,
                     FSaisieY -> valuef ( ) ,
                     FSaisieZ -> valuef ( ) ) ;
      
    if (FControler -> getParameterPolylinePosition()->getNbVertices()==0)
      FControler -> getParameterPolylinePosition()->addVertex(vertex);
    else
      FControler -> getParameterPolylinePosition()->setLastVertex(vertex);
    repaint ( ) ;
  }
}

// SLOTS
//-------
void CreationBrin :: close ( )
{
  //  TODO getDialogWindow()->getViewsWindow()->polylineClose();
}

void CreationBrin :: prolong ( )
{
  CVertex vertex( FSaisieX -> valuef ( ) ,
                  FSaisieY -> valuef ( ) ,
                  FSaisieZ -> valuef ( ) ) ;
  
  FControler -> getParameterPolylinePosition (  ) -> addVertex ( vertex ) ;
  update ( ) ;
  repaint ( ) ;
}

void CreationBrin :: shorten ( )
{
  if (FControler -> getParameterPolylinePosition( ) -> getNbVertices ( ) > 0 )
  {
    FControler -> getParameterPolylinePosition ( ) -> removeLastVertex ( ) ;
    update ( ) ;
    repaint ( ) ;
  }
}

