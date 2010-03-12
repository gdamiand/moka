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

#include "options-interpolation.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>

// Constructeur
//-------------
OptionsInterpolation :: OptionsInterpolation ( Window * parent , QTabWidget *  parentTab ) 
  :
  QWidget ( parentTab ) ,
  FParent ( parent ) ,
  FUpdate ( false )
{
  QBoxLayout * placement = new QBoxLayout ( QBoxLayout::TopToBottom, this ) ; 

  FNbSubdivision = new QGroupBox("Number of subdivisions");
  QVBoxLayout* nsd_vbl = new QVBoxLayout(FNbSubdivision);

  /** 1ere subdivision **/
  QWidget * WidgetSub0 = new QWidget ( this ) ;
  QBoxLayout * sub0 = new QBoxLayout ( QBoxLayout::LeftToRight, WidgetSub0 ) ;

  FNbDiv [0] = 
        new QLabel ( *HTML::decode("Following the first dimension : ") ) ;
  FSaisieNbDiv [0] = new QSpinBox ;
  FSaisieNbDiv [0] -> setMinimum ( 1 );
  FSaisieNbDiv [0] -> setMaximum ( 1000 );
  FSaisieNbDiv [0] -> setAccelerated(true);
  
  sub0 -> addWidget ( FNbDiv [0], 0, Qt::AlignLeft ) ;
  sub0 -> addWidget ( FSaisieNbDiv [0], 1, Qt::AlignLeft ) ;

  /** 2eme subdivision **/
  QWidget * WidgetSub1 = new QWidget ( this ) ;
  QBoxLayout * sub1 = new QBoxLayout ( QBoxLayout::LeftToRight, WidgetSub1 ) ;
  FNbDiv [1] = 
        new QLabel ( *HTML::decode("Following the second dimension :") ) ;
  FSaisieNbDiv [1] = new QSpinBox ;
  FSaisieNbDiv [1] -> setMinimum ( 1 );
  FSaisieNbDiv [1] -> setMaximum ( 1000 );
  FSaisieNbDiv [1] -> setAccelerated(true);

  sub1 -> addWidget ( FNbDiv [1], 0, Qt::AlignLeft ) ;
  sub1 -> addWidget ( FSaisieNbDiv [1], 1, Qt::AlignLeft ) ;

  /** 3eme subdivision **/
  QWidget * WidgetSub2 = new QWidget ( this ) ;
  QBoxLayout * sub2 = new QBoxLayout ( QBoxLayout::LeftToRight, WidgetSub2 ) ;
  FNbDiv [2] = 
        new QLabel ( *HTML::decode("Following the third dimension : ")) ;
  FSaisieNbDiv [2] = new QSpinBox ;
  FSaisieNbDiv [2] -> setMinimum ( 1 );
  FSaisieNbDiv [2] -> setMaximum ( 1000 );
  FSaisieNbDiv [2] -> setAccelerated(true);
  
  sub2 -> addWidget ( FNbDiv [2], 0, Qt::AlignLeft ) ;
  sub2 -> addWidget ( FSaisieNbDiv [2], 1, Qt::AlignLeft ) ;
  
  nsd_vbl -> addWidget ( WidgetSub0 ) ;
  nsd_vbl -> addWidget ( WidgetSub1 ) ;
  nsd_vbl -> addWidget ( WidgetSub2 ) ;


  /** interpolation **/
  FAffInterpolation = new QGroupBox ( "Interpolation" ) ;
  QVBoxLayout* aip_vbl = new QVBoxLayout(FAffInterpolation);
  
  FFusionInterpolation = new QCheckBox ( *HTML::decode("Merge created mesh with the initial objet") ) ;
  FCoudreInterpolation = new QCheckBox ( *HTML::decode("Sew created meshes corresponding to adjacent cells") ) ;
  
  aip_vbl -> addWidget(FFusionInterpolation);
  aip_vbl -> addWidget(FCoudreInterpolation);

  /** lissage **/
  FAffLissage = new QGroupBox ("Smoothing") ;
  QVBoxLayout* als_vbl = new QVBoxLayout(FAffLissage);

  QWidget * WidgetMethode = new QWidget ;
  QBoxLayout * methode = new QBoxLayout ( QBoxLayout::LeftToRight, WidgetMethode ) ;

  FAffMethode = new QLabel ( *HTML::decode("Method: ") ) ;
  FSaisieMethode = new QComboBox ;
  FSaisieMethode -> addItem ( "Hermite" ) ;
  FSaisieMethode -> addItem ( "Bezier" ) ;
  FSaisieMethode -> addItem ( "Overhauser" ) ;
  FSaisieMethode -> addItem ( "Spline") ;

  methode -> addWidget ( FAffMethode , 0, Qt::AlignLeft) ;
  methode -> addWidget ( FSaisieMethode , 1, Qt::AlignLeft) ;

  FFusionLissage = new QCheckBox ( *HTML::decode("Merge created mesh with the initial objet") ) ;
  FCoudreLissage = new QCheckBox ( *HTML::decode("Sew created meshes corresponding to adjacent cells") ) ;

  als_vbl -> addWidget ( WidgetMethode );
  als_vbl -> addWidget ( FFusionLissage ) ;
  als_vbl -> addWidget ( FCoudreLissage ) ;  
  

  /** Ajout au layout principal **/
   
  placement -> addWidget ( FNbSubdivision );
  placement -> addWidget ( FAffInterpolation ) ;
  placement -> addWidget ( FAffLissage ) ;

  QObject :: connect ( FSaisieNbDiv [0] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSizeX ( ) ) ) ;
  QObject :: connect ( FSaisieNbDiv [1] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSizeY ( ) ) ) ;
  QObject :: connect ( FSaisieNbDiv [2] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSizeZ ( ) ) ) ;
  
  QObject :: connect ( FFusionInterpolation ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT (callbackInterpolationMerges ( ) ) ) ;
  QObject :: connect ( FCoudreInterpolation ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT (callbackInterpolationSews ( ) ) ) ;

  QObject :: connect ( FFusionLissage ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackSmoothMerges ( ) ) ) ;
  QObject :: connect ( FCoudreLissage ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackSmoothSews ( ) ) ) ;
  QObject :: connect ( FSaisieMethode ,
		       SIGNAL (  activated ( int ) ) ,
		       this ,
		       SLOT ( callbackSmoothMethod ( ) ) ) ;

}


// Destructeur
//------------
OptionsInterpolation ::  ~OptionsInterpolation ( ) 
{
}


// Mise a jour de l'affichage
//---------------------------
void OptionsInterpolation :: update ( )
{
  FUpdate = true ;  
  CParameterOperations* p = FParent -> getControler ( ) -> getParameterOperations ( ) ; 
  
  FSaisieNbDiv [ 0 ] -> setValue ( p -> getMeshNbSubdivisionsX ( ) ) ;
  FSaisieNbDiv [ 1 ] -> setValue ( p -> getMeshNbSubdivisionsY ( ) ) ;
  FSaisieNbDiv [ 2 ] -> setValue ( p -> getMeshNbSubdivisionsZ ( ) ) ;

  FFusionInterpolation -> setChecked ( p -> getInterpolationMerges ( ) ) ;
  FCoudreInterpolation -> setChecked ( p -> getInterpolationSews ( ) ) ;

  FCoudreInterpolation -> setEnabled ( p -> isActiveInterpolationSews ( ) ) ;

  FFusionLissage -> setChecked ( p -> getSmoothMerges ( ) ) ;
  FCoudreLissage -> setChecked ( p -> getSmoothSews ( ) ) ;

  FFusionLissage -> setEnabled ( p -> isActiveSmoothMerges ( ) ) ;
  FCoudreLissage -> setEnabled ( p -> isActiveSmoothSews ( ) ) ;

  FSaisieMethode -> setCurrentIndex ( p -> getSmoothMethod ( ) ) ;

  FUpdate = false ;
}


void OptionsInterpolation :: callbackSizeX ( )
{ 
  if ( ! FUpdate ) 
    setSizeX ( FSaisieNbDiv [ 0 ] -> value ( ) ) ;
}

void OptionsInterpolation :: callbackSizeY ( )
{
  if ( ! FUpdate )
    setSizeY ( FSaisieNbDiv [ 1 ] -> value ( ) ) ;
}

void OptionsInterpolation :: callbackSizeZ ( )
{
  if ( ! FUpdate )
    setSizeZ ( FSaisieNbDiv [ 2 ] -> value ( ) ) ;
}

void OptionsInterpolation :: callbackInterpolationMerges ( )
{
  if ( ! FUpdate )
    setInterpolationMerges ( FFusionInterpolation -> isChecked ( ) ) ;
}

void OptionsInterpolation :: callbackInterpolationSews ( )
{
  if ( ! FUpdate )
    setInterpolationSews ( FCoudreInterpolation -> isChecked ( ) ) ;
}

void OptionsInterpolation :: callbackSmoothMethod ( )
{
  if ( ! FUpdate )
    setSmoothMethod ( FSaisieMethode -> currentIndex ( ) ) ;
}

void OptionsInterpolation :: callbackSmoothMerges ( )
{
  if ( ! FUpdate )
    setSmoothMerges ( FFusionLissage -> isChecked ( ) ) ;
}

void OptionsInterpolation :: callbackSmoothSews ( )
{
  if ( ! FUpdate )
    setSmoothSews ( FCoudreLissage -> isChecked ( ) ) ;
}

// Accesseur en lecture
//---------------------
int OptionsInterpolation :: getSmoothMethod ( ) const
{ 
  return FParent -> getControler ( ) -> 
    getParameterOperations ( ) -> getSmoothMethod ( ) ;
}

bool OptionsInterpolation :: getInterpolationMerges ( ) const
{ 
  return FParent -> getControler ( ) -> 
    getParameterOperations ( ) -> getInterpolationMerges ( ) ;
}

bool OptionsInterpolation :: getInterpolationSews ( ) const
{ 
  return FParent -> getControler ( ) -> 
    getParameterOperations ( ) -> getInterpolationSews ( ) ;
}

bool OptionsInterpolation :: getSmoothMerges ( ) const
{ 
  return FParent -> getControler ( ) -> 
    getParameterOperations ( ) -> getSmoothMerges ( ) ;
}

bool OptionsInterpolation :: getSmoothSews ( ) const
{ return FParent -> getControler ( ) -> 
    getParameterOperations ( ) -> getSmoothSews ( ) ;
}

// Accesseurs en ecriture
//-----------------------
void OptionsInterpolation :: setSizeX ( int s )
{
  assert ( s >= 0 ) ;
  FParent -> getControler ( ) -> 
    getParameterOperations ( ) -> setMeshNbSubdivisionsX ( s ) ;
  update ( ) ;
}

void OptionsInterpolation :: setSizeY ( int s )
{
  assert ( s >= 0 ) ;
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    ->setMeshNbSubdivisionsY ( s ) ;
  update ( ) ;
}

void OptionsInterpolation :: setSizeZ ( int s )
{
  assert ( s >= 0 ) ;
  FParent -> getControler ( ) -> 
    getParameterOperations ( ) -> setMeshNbSubdivisionsZ ( s ) ;
  update ( ) ;
}

void OptionsInterpolation :: setInterpolationMerges ( bool b )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setInterpolationMerges ( b ) ;
  update ( ) ;
}

void OptionsInterpolation :: setInterpolationSews ( bool b )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setInterpolationSews ( b ) ;
  update ( ) ;
}

void OptionsInterpolation :: setSmoothMethod ( int m )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setSmoothMethod ( m ) ;
  update ( ) ;
}

void OptionsInterpolation :: setSmoothMerges ( bool b )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setSmoothMerges ( b ) ;
  update ( ) ;
}

void OptionsInterpolation :: setSmoothSews ( bool b )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setSmoothSews ( b ) ;
  update ( ) ;
}
