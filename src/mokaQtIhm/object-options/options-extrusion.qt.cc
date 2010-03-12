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

#include "options-extrusion.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>

// Constructeur
//-------------
OptionsExtrusion :: OptionsExtrusion ( Window * parent , 
				       QTabWidget *  parentTab ) 
  :
  QWidget ( parentTab )  ,
  FParent ( parent ) ,
  FUpdate ( false )
{
  QBoxLayout * placement = new QBoxLayout ( QBoxLayout :: TopToBottom, this ) ;

  FCoudre = new QCheckBox ( *HTML::decode("Sew cells"
        " adjacent extruded") , this ) ;

  // ---------------------------
  FAffVecteur = 
        new QGroupBox ( "Extrusion following normal vector / thickening" ) ;
  QHBoxLayout * avt_hbl = new QHBoxLayout ( FAffVecteur ) ;
  
  
  FAffCoef = new QLabel ( "Coefficient:" ) ;
  FSaisieCoef = new FloatSpinBox ( 0.1 ) ;
  
  avt_hbl -> addWidget (FAffCoef, 0, Qt::AlignLeft);
  avt_hbl -> addWidget (FSaisieCoef, 1, Qt::AlignLeft);
  
  // ---------------------------
  FAffRevolutionChemin = new QGroupBox(*HTML::decode("Extrusion following "
        "path / Extrusion by revolution"));
  QVBoxLayout* arc_vbl = new QVBoxLayout(FAffRevolutionChemin);

  QWidget * WidgetOrientation = new QWidget ( FAffRevolutionChemin ) ;
  QHBoxLayout * Orientation = new QHBoxLayout ( WidgetOrientation ) ;

  FAffOrientation = new QLabel ( "Initial Orientation :" ) ;
  FChoixOrientation = new QComboBox ;
  FChoixOrientation -> addItem ( "No rotation" ) ;
  FChoixOrientation -> addItem ( "Cells -> Path" ) ;
  FChoixOrientation -> addItem ( "Path -> Cells" ) ;

  Orientation -> addWidget ( FAffOrientation, 0, Qt::AlignLeft ) ;
  Orientation -> addWidget ( FChoixOrientation, 1, Qt::AlignLeft ) ;  

  FOrienter =
     new QCheckBox(*HTML::decode("Orientate extruded cells "));
  FRedimensionner = new QCheckBox(*HTML::decode("Resize "
        "extruded cells ")) ;
  FPonderer = new QCheckBox(*HTML::decode("Weight radius of the "
        " section")) ;
  
  arc_vbl -> addWidget (WidgetOrientation);
  arc_vbl -> addWidget (FOrienter);
  arc_vbl -> addWidget (FRedimensionner);
  arc_vbl -> addWidget (FPonderer);
  
  //--------------------------------
  
  FAffChemin = new QGroupBox ( "Extrusion from path " );
  QHBoxLayout * acm_hbl = new QHBoxLayout ( FAffChemin ) ;
			       
  FAffPlacement = new QLabel ( "Original Position:" ) ;
  FChoixPlacement = new QComboBox ;
  FChoixPlacement -> addItem ( "No translation" ) ;
  FChoixPlacement -> addItem ( "Cells -> Path" ) ;
  FChoixPlacement -> addItem ( "Path -> Cells" ) ;

  acm_hbl -> addWidget(FAffPlacement, 0, Qt::AlignLeft);
  acm_hbl -> addWidget(FChoixPlacement, 1, Qt::AlignLeft);
  
  //---------------------------------
  FAffRevolution = 
        new QGroupBox ( *HTML::decode("Extrusion by revolution"));
  QHBoxLayout * arv_hbl = new QHBoxLayout ( FAffRevolution ) ;
  
  QWidget * Widgetdiv = new QWidget ;
  QHBoxLayout * div = new QHBoxLayout ( Widgetdiv ) ;

  FNbSubdivisions = new QLabel ( "Number of subdivisions:" , Widgetdiv ) ;
  FSaisieNbSubdivisions = new QSpinBox ( Widgetdiv ) ;
  FSaisieNbSubdivisions -> setMinimum ( 3 );
  FSaisieNbSubdivisions -> setMaximum ( 1000 );
  FSaisieNbSubdivisions -> setAccelerated(true);
  
  div -> addWidget ( FNbSubdivisions, 0, Qt::AlignLeft ) ;
  div -> addWidget ( FSaisieNbSubdivisions, 1, Qt::AlignLeft ) ;

  QWidget * WidgetAngle = new QWidget ( FAffRevolution ) ;
  QHBoxLayout * Angle = new QHBoxLayout ( WidgetAngle ) ;
 
  FAffAngle = new QLabel ( *HTML::decode("Angle of revolution:") ) ;
  FSaisieAngle = new FloatSpinBox ( 5 , -360 , 360 , 2 , 5 ) ;
  FSaisieAngle -> setWrapping ( true ) ;

  Angle -> addWidget ( FAffAngle, 0, Qt::AlignLeft ) ;
  Angle -> addWidget ( FSaisieAngle, 1, Qt::AlignLeft ) ;
  
  
  arv_hbl -> addWidget(Widgetdiv);
  arv_hbl -> addWidget(WidgetAngle);


  placement -> addWidget ( FAffVecteur ) ;
  placement -> addWidget ( FAffRevolutionChemin ) ;
  placement -> addWidget ( FAffChemin ) ;
  placement -> addWidget ( FAffRevolution ) ;
  placement -> addWidget ( FCoudre ) ;

  QObject :: connect ( FCoudre,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackAdjacentSews ( ) ) ) ;
  QObject :: connect ( FOrienter ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackDirectCells ( ) ) ) ;
  QObject :: connect ( FRedimensionner ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackScaleCells ( ) ) ) ; 
  QObject :: connect ( FPonderer ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackPonderateSection ( ) ) ) ;
  QObject :: connect ( FSaisieCoef ,
		       SIGNAL ( valueChanged ( double ) ) ,
		       this ,
		       SLOT ( callbackExtrusionCoef ( ) ) ) ;
  QObject :: connect ( FSaisieAngle ,
		       SIGNAL ( valueChanged ( double ) ) ,
		       this ,
		       SLOT ( callbackRevolutionAngle ( ) ) ) ;

  QObject :: connect ( FSaisieNbSubdivisions ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackRevolutionNbEdges ( ) ) ) ;

  QObject :: connect ( FChoixOrientation ,
		       SIGNAL ( activated ( int ) ) ,
		       this ,
		       SLOT ( callbackInitialDirectCells  ( ) ) ) ;
  QObject :: connect ( FChoixPlacement ,
		       SIGNAL ( activated ( int ) ) ,
		       this ,
		       SLOT ( callbackInitialPositionCells ( ) ) ) ;
}


// Destructeur
//------------
OptionsExtrusion ::  ~OptionsExtrusion ( ) 
{
}

// Mise a jour de l'affichage
//---------------------------
void OptionsExtrusion :: update ( ) 
{
  FUpdate = true ;

  CParameterOperations * p = FParent -> getControler ( ) -> getParameterOperations ( ) ;
  
  FCoudre -> setChecked ( p -> getExtrusionAdjacentSews ( ) ) ;
  FChoixPlacement -> setCurrentIndex ( p -> getExtrusionInitialPositionMode ( ) );
  FChoixOrientation -> setCurrentIndex ( p -> getExtrusionInitialDirectionMode ( ) ) ;
  FOrienter -> setChecked ( p -> getExtrusionDirection ( ) ) ;
  FRedimensionner -> setChecked ( p -> getExtrusionScale ( ) ) ;
  FPonderer -> setChecked ( p -> getExtrusionPonderateSection ( ) ) ;
  FSaisieCoef -> setValuef ( p -> getExtrusionCoef ( ) ) ;
  FSaisieNbSubdivisions -> setValue ( p -> getExtrusionRevolutionNbEdges ( ) ) ;
  FSaisieAngle -> setValuef ( p -> getExtrusionRevolutionAngle ( ) ) ;

  FUpdate = false ;
}


// Slots
//------

void OptionsExtrusion :: callbackAdjacentSews ( )
{
  if ( ! FUpdate )
    setAdjacentSews ( FCoudre -> isChecked ( ) ) ;
}

void OptionsExtrusion :: callbackInitialPositionCells ( )
{
  if ( ! FUpdate )
    setInitialPosition ( FChoixPlacement -> currentIndex ( ) ) ;
}

void OptionsExtrusion :: callbackInitialDirectCells ( )
{
  if ( ! FUpdate )
    setInitialDirection ( FChoixOrientation -> currentIndex ( ) ) ;
}

void OptionsExtrusion :: callbackDirectCells ( )
{
  if ( ! FUpdate )
    setDirection ( FOrienter -> isChecked ( ) ) ;
}

void OptionsExtrusion :: callbackScaleCells ( )
{
  if ( ! FUpdate )
    setScale ( FRedimensionner -> isChecked ( ) ) ;
}

void OptionsExtrusion :: callbackPonderateSection ( )
{
  if ( ! FUpdate )
    setPonderation ( FPonderer -> isChecked ( ) ) ;
}

void OptionsExtrusion :: callbackExtrusionCoef ( )
{
  if ( ! FUpdate )
    setExtrusionCoef ( FSaisieCoef -> valuef ( ) ) ;
}

void OptionsExtrusion :: callbackRevolutionNbEdges ( )
{
  if ( ! FUpdate )
    setRevolutionNbEdges ( FSaisieNbSubdivisions -> value ( ) ) ;
}

void OptionsExtrusion :: callbackRevolutionAngle ( )
{
  if ( ! FUpdate )
    setRevolutionAngle ( FSaisieAngle -> valuef ( ) ) ;
}


// Accesseurs en ecriture
void OptionsExtrusion :: setAdjacentSews ( bool b )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setExtrusionAdjacentSews ( b ) ;
  update ( ) ;
}

void OptionsExtrusion :: setInitialPosition ( int m )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setExtrusionInitialPositionMode ( m ) ;
  update ( ) ;
}

void OptionsExtrusion :: setInitialDirection ( int m )
{
    FParent -> getControler ( ) -> getParameterOperations ( ) 
      -> setExtrusionInitialDirectionMode ( m ) ;
  update ( ) ;
}

void OptionsExtrusion :: setDirection ( bool b )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setExtrusionDirection ( b ) ;
  update ( ) ;
}

void OptionsExtrusion :: setScale ( bool b )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setExtrusionScale ( b ) ;
  update ( ) ;
}

void OptionsExtrusion :: setPonderation ( bool b )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setExtrusionPonderateSection ( b ) ;
  update ( ) ;
}

void OptionsExtrusion :: setExtrusionCoef ( TCoordinate K )
{
  FParent -> getControler ( ) -> getParameterOperations ( ) 
    -> setExtrusionCoef ( K ) ;
  update ( ) ;
}

void OptionsExtrusion :: setRevolutionNbEdges ( int n )
{
  assert ( n >= 3 ) ;
  
  FParent -> getControler ( ) -> getParameterOperations ( )
    -> setExtrusionRevolutionNbEdges ( n ) ;
  update ( ) ;
}

void OptionsExtrusion :: setRevolutionAngle ( TCoordinate alpha )
{
  while ( alpha < -360 )
    alpha += 360 ;

  while ( alpha > +360 )
    alpha -= 360 ;

  CParameterOperations * p = FParent -> getControler ( ) -> getParameterOperations ( ) ;

  if ( alpha == p -> getExtrusionRevolutionAngle ( ) )
    return;

  p -> setExtrusionRevolutionAngle ( alpha ) ;
  update ( ) ;
}
