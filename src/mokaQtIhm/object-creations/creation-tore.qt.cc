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

#include "creation-tore.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>

/*******************************************************************/
/* CLASSE  creationTore                                        */
/*******************************************************************/

CreationTore ::  CreationTore ( Window * parent ,
				GMap3d :: CControlerGMap * controler ) :
  CreationObjet ( parent , HTML::decode("Create torus") , 
                  controler )
{
  QWidget * widget = new QWidget ;  
  
  QHBoxLayout * wid_hbl = new QHBoxLayout ( widget );
  wid_hbl -> setSpacing(0);
  wid_hbl -> setContentsMargins (0,0,0,0);
  
  FAffSub = new QLabel ( "  Subdivisions: " ) ;
  FAffPar = new QLabel ( *HTML::decode("Parallels: ") ) ;  
  FSaisiePar = new QSpinBox ;  
  FSaisiePar -> setMinimum ( 3 );
  FSaisiePar -> setMaximum ( 100000 );
  FSaisiePar -> setAccelerated(true);
  
  FAffMer = new QLabel ( *HTML::decode("Meridians: ") ) ;
  FSaisieMer = new QSpinBox ;
  FSaisieMer -> setMinimum ( 3 );
  FSaisieMer -> setMaximum ( 100000 );
  FSaisieMer -> setAccelerated(true);

  FAffRayon = new QLabel ( "Small radius/Large radius : " ) ;
  
  FSaisieRayon = new QSlider ( Qt::Horizontal ) ;
  FSaisieRayon -> setTickPosition ( QSlider::TicksBothSides ) ;
  FSaisieRayon -> setMinimum ( 0 ) ;
  FSaisieRayon -> setMaximum ( 100 ) ;
  FSaisieRayon -> setSingleStep ( 1 ) ;
  FSaisieRayon -> setFixedWidth (200);

  FAffValRayon = new QLabel ;
  
  wid_hbl -> addWidget(FAffSub);
  wid_hbl -> addWidget(FAffPar);
  wid_hbl -> addWidget(FSaisiePar);
  wid_hbl -> addSpacing(SPACE_PIX_SIZE);
  wid_hbl -> addWidget(FAffMer);
  wid_hbl -> addWidget(FSaisieMer);
  wid_hbl -> addSpacing(SPACE_PIX_SIZE);
  wid_hbl -> addWidget(FAffRayon);
  wid_hbl -> addWidget(FSaisieRayon);
  wid_hbl -> addWidget(FAffValRayon);
  wid_hbl -> addSpacing(SPACE_PIX_SIZE);

  FChoix = new BoutonsCreation ( this, wid_hbl ) ;
    
  wid_hbl -> addStretch(1);
  addWidget(widget);

  //*** Creation de la boite de positonnement ******
  FPosit = new BoitePositionnement ( this , parent , OBJECT_TORUS ) ;
  
  update ( ) ;
  
  QObject::connect ( FSaisiePar , SIGNAL ( valueChanged ( int ) ) , this,
                     SLOT ( callbackParallels ( ) ) ) ;
  QObject::connect ( FSaisieMer , SIGNAL ( valueChanged ( int ) ) , this,
                     SLOT ( callbackMeridians ( ) ) ) ;
  QObject::connect ( FSaisieRayon , SIGNAL ( valueChanged ( int ) ) , this ,
                     SLOT ( callbackRR ( ) ) ) ;

//  QObject::connect ( FSaisieRayon , SIGNAL ( valueChanged ( int ) ) ,
//                     this , SLOT ( callbackVal ( ) ) ) ;
}

CreationTore :: ~CreationTore ( ) 
{
}

// Affichage
//----------
void CreationTore::update()
{
  CreationObjet :: update ( ) ;
  
  FSaisieMer->setValue(FControler->getParameterCreation()
        ->getTorusNbMeridians());
  FSaisiePar->setValue(FControler->getParameterCreation()
        ->getTorusNbParallels());
  FSaisieRayon->setValue(int(FControler->getParameterCreation()
        ->getTorusRadiusRapport()*100.0)) ;
}

// SLOTS
//-------
void CreationTore :: callbackMeridians ( )
{ 
  setMeridians ( getMeridians ( ) ) ; 
}

void CreationTore :: callbackParallels ( )
{ 
  setParallels ( getParallels ( ) ) ; 
}

void CreationTore :: callbackRR ( )
{
  setRR ( getRR ( ) ) ; 
}

/*
void CreationTore :: callbackVal ( )
{
  char ValRayon [5] ;
  sprintf (ValRayon , "%5.2f", (float (FSaisieRayon -> value ( ) )) / 100.0 ) ;
  FAffValRayon -> setText ( ValRayon ) ;

}
*/

// Accesseurs en lecture
//-----------------------
int CreationTore :: getMeridians() const
{ 
  return FSaisieMer -> value (); 
}

int CreationTore :: getParallels() const
{ 
  return FSaisiePar -> value ();
}

float CreationTore::getRR() const
{
  float v = FSaisieRayon -> value () / 100.0 ; 
  return v ; 
}

// Accesseurs en ecriture
//------------------------
void CreationTore::setMeridians(int m)
{
  FControler -> getParameterCreation()->setTorusNbMeridians(m);
  update( ) ;
  repaint ( ) ;
}

void CreationTore::setParallels(int p)
{
  FControler -> getParameterCreation()->setTorusNbParallels(p);
  update( ) ;
  repaint ( ) ;
}

void CreationTore::setRR(float v)
{
  assert(v>=0); assert(v<=1);
  FControler->getParameterCreation()->setTorusRadiusRapport(v);
  update ( ) ;
  repaint ( ) ;
}
