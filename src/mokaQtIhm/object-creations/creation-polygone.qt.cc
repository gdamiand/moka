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

#include "creation-polygone.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QLabel>

/*******************************************************************/
/* CLASSE CreationPolygone                                         */
/*******************************************************************/

//***********************************************
// Constructeur
//***********************************************
CreationPolygone :: CreationPolygone ( Window * parent ,
				       GMap3d :: CControlerGMap * controler )
  :
  CreationObjet ( parent , 
                  HTML::decode("Create Regular Polygon" ),
                  controler ),
  FModifie ( true )
{
  
  // Creation des widgets
  FAffNom = new QLabel ( *HTML::decode(" Edge number: ") , this ) ;
  
  FSaisieNom = new QSpinBox ( this ) ;
  FSaisieNom -> setMinimum ( 3 );
  FSaisieNom -> setMaximum ( 999 );
  FSaisieNom -> setValue ( 6 ) ;
  FSaisieNom -> setAccelerated(true);
  
  FAffLon = 
        new QLabel ( *HTML::decode("Edge length: ") , this ) ;
  FSaisieLon = new FloatSpinBox ( 0.2 , 0 , 1000 , 3 , 0.5 ) ;
  
  addWidget(FAffNom);
  addWidget(FSaisieNom);
  addWidget(new SpaceWidget(SPACE_PIX_SIZE,1));
  addWidget(FAffLon);
  addWidget(FSaisieLon);
  addWidget(new SpaceWidget(SPACE_PIX_SIZE,1));

  // Creation des boutons
  
  FChoix = new BoutonsCreation ( this ) ;

  //*** Creation de la boite de positonnement ******
  FPosit = new BoitePositionnement ( this , parent , OBJECT_POLYGON ) ;

  update ( ) ;
  repaint ( ) ;
  
  QObject::connect ( FSaisieNom , SIGNAL ( valueChanged ( int ) ) , this,
                     SLOT (  callbackEdgesNumber() ) ) ;

  QObject::connect ( FSaisieLon , SIGNAL ( valueChanged ( double ) ) , this,
                     SLOT (  callbackEdgesLength() ) ) ;
}

//****************************************************
// Destructeur
//****************************************************
CreationPolygone :: ~CreationPolygone ( ) { }

void CreationPolygone :: update()
{
  FModifie = false ;
  CreationObjet :: update ( ) ;
  FSaisieLon -> setValuef ( FControler -> getPolygonEdgesLength ( ) ) ;
  FPosit -> update ( ) ;
  FModifie = true ;
}

//*****************************************************
// SLOTS
//*****************************************************
void CreationPolygone :: callbackEdgesNumber()
{
  if ( FModifie )
    setEdgesNumber(getEdgesNumber()); 
}

void CreationPolygone:: callbackEdgesLength()
{
  if ( FModifie )
    setEdgesLength(getEdgesLength()); 
}

// Accessseurs en lecture
//-----------------------
int CreationPolygone :: getEdgesNumber() const
{
  return FSaisieNom -> value ();
}

TCoordinate CreationPolygone :: getEdgesLength() const
{
  return FSaisieLon -> valuef ();
}

// Accesseurs en ecriture
// ----------------------
void CreationPolygone :: setEdgesNumber(int n)
{
  FControler -> setPolygonEdgesNumber ( n ) ;
  update ( ) ;
  repaint ( ) ;
}

void CreationPolygone :: setEdgesLength(TCoordinate l)
{
  FControler -> setPolygonEdgesLength ( l ) ;
//  FPosit -> update ( ) ;
  update ( ) ;
  repaint ( ) ;
}
