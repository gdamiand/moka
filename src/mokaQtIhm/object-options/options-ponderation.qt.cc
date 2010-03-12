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

#include "options-ponderation.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

// Constructeur
//-------------
OptionsPonderation :: OptionsPonderation ( Window * parent , QTabWidget *  parentTab ) 
  :
  QWidget ( parentTab ) ,
  FParent ( parent ) ,
  FUpdate ( false )
{
  QBoxLayout * placement = new QBoxLayout ( QBoxLayout::TopToBottom, this ) ; 

  /** Partie haute : Distance a **/
  FAffDistance = new QGroupBox ( *HTML::decode("Distance to..."));
  QVBoxLayout* afd_vbl = new QVBoxLayout(FAffDistance);

  FSaisieDistance = new QComboBox ;
  FSaisieDistance -> addItem ( "Vertice" ) ;
  FSaisieDistance -> addItem ( "Line" ) ;
  FSaisieDistance -> addItem ( "Plane" ) ;
  
  QWidget * WidgetDistance2 = new QWidget ;
  QBoxLayout * distance2 = new QBoxLayout ( QBoxLayout::LeftToRight,
                                            WidgetDistance2 ) ;

  FAffSommet = new QLabel ( "Vertex :" ) ;
  
  for (int i = 0 ; i < 3 ; i++ ) {
    FSaisieCoefSommet [i] = new FloatSpinBox ( 0.5 ) ;
  }
  FRecupSommet = new QPushButton ( "Recover" ) ;
  
  FAffCoefSommet [0] = new QLabel ( "X :" ) ;
  FAffCoefSommet [1] = new QLabel ( "Y :" ) ;
  FAffCoefSommet [2] = new QLabel ( "Z :" ) ;

  distance2 -> addWidget ( FAffCoefSommet [0], 0, Qt::AlignLeft ) ;
  distance2 -> addWidget ( FSaisieCoefSommet [0], 1, Qt::AlignLeft ) ; 
  distance2 -> addSpacing ( 10 ) ;
  distance2 -> addWidget ( FAffCoefSommet [1], 0, Qt::AlignLeft ) ;
  distance2 -> addWidget ( FSaisieCoefSommet [1], 1, Qt::AlignLeft ) ; 
  distance2 -> addSpacing ( 10 ) ;
  distance2 -> addWidget ( FAffCoefSommet [2], 0, Qt::AlignLeft ) ;
  distance2 -> addWidget ( FSaisieCoefSommet [2], 1, Qt::AlignLeft ) ;  
  distance2 -> addSpacing ( 10 ) ;
  distance2 -> addWidget ( FRecupSommet ) ;
  

  QWidget * WidgetDistance3 = new QWidget ;
  QBoxLayout * distance3 = new QBoxLayout ( QBoxLayout::LeftToRight, WidgetDistance3 ) ;

  FAffVecteur = new QLabel ( "Vector :" ) ;
  
  for (int i = 0 ; i < 3 ; i++ ) 
     FSaisieCoefVecteur [i] = new FloatSpinBox ( 0.5 ) ;
  
  FRecupVecteur = new QPushButton ( "Recover" ) ;
  
  FAffCoefVecteur [0] = new QLabel ( "X :" ) ;
  FAffCoefVecteur [1] = new QLabel ( "Y :" ) ;
  FAffCoefVecteur [2] = new QLabel ( "Z :" ) ;

  distance3 -> addWidget ( FAffCoefVecteur [0], 0, Qt::AlignLeft ) ;
  distance3 -> addWidget ( FSaisieCoefVecteur [0], 1, Qt::AlignLeft ) ;  
  distance3 -> addSpacing ( 10 ) ;
  distance3 -> addWidget ( FAffCoefVecteur [1], 0, Qt::AlignLeft ) ;
  distance3 -> addWidget ( FSaisieCoefVecteur [1], 1, Qt::AlignLeft ) ;  
  distance3 -> addSpacing ( 10 ) ;
  distance3 -> addWidget ( FAffCoefVecteur [2], 0, Qt::AlignLeft ) ;
  distance3 -> addWidget ( FSaisieCoefVecteur [2], 1, Qt::AlignLeft ) ;  
  distance3 -> addSpacing ( 10 ) ;
  distance3 -> addWidget ( FRecupVecteur ) ;
  
  
  afd_vbl -> addWidget(FSaisieDistance, 0, Qt::AlignCenter);
  afd_vbl -> addWidget(FAffSommet);  
  afd_vbl -> addWidget(WidgetDistance2);
  afd_vbl -> addWidget(FAffVecteur);
  afd_vbl -> addWidget(WidgetDistance3);
  
  /** Partie basse **/
  QWidget * WidgetBas = new QWidget (this) ;
  QBoxLayout * bas = new QBoxLayout ( QBoxLayout::LeftToRight, WidgetBas ) ;

  /** etalonnage des distances **/
  
  FAffEtalonnageDistance = new QGroupBox ( "Distances calibration" );
  FAffEtalonnageDistance -> setCheckable(true);
  FAffEtalonnageDistance -> setChecked(false);

  QVBoxLayout * aed_vbl = new QVBoxLayout ( FAffEtalonnageDistance ) ;
  
  QWidget * WidgetMin1 = new QWidget ;
  QHBoxLayout * min1 = new QHBoxLayout ( WidgetMin1) ;
  FAffMinDistance = new QLabel ( "Minimum :" ) ;
  FSaisieMinDistance = new  FloatSpinBox ( 0.5 ) ;
  
  min1 -> addWidget (  FAffMinDistance, 0, Qt::AlignRight ) ;
  min1 -> addWidget (  FSaisieMinDistance, 0, Qt::AlignLeft ) ;

  QWidget * WidgetMax1 = new QWidget ;
  QHBoxLayout * max1 = new QHBoxLayout ( WidgetMax1 ) ;
  FAffMaxDistance = new QLabel ( "Maximum :" ) ;  
  FSaisieMaxDistance = new  FloatSpinBox ( 0.5 ) ;
  
  max1 -> addWidget (  FAffMaxDistance, 0, Qt::AlignRight ) ;
  max1 -> addWidget (  FSaisieMaxDistance, 0, Qt::AlignLeft ) ;

  aed_vbl -> addWidget(WidgetMin1);
  aed_vbl -> addWidget(WidgetMax1);

  /** Fonction appliquee **/
  
  QWidget * WidgetCentre = new QWidget (this) ;
  QBoxLayout * centre = new QBoxLayout ( QBoxLayout::LeftToRight, WidgetCentre ) ;

  FSaisieFonction = new QComboBox ;
  FSaisieFonction -> addItem ( *HTML::decode("linear") ) ;
  FSaisieFonction -> addItem ( "quadratic" ) ;
  FSaisieFonction -> addItem ( "exponential" ) ;
  FSaisieFonction -> addItem ( "logarithmic" ) ;
  FSaisieFonction -> addItem ( "sinusoidal" ) ;
  FSaisieFonction -> addItem ( "cosinusoidal" ) ;
  
  centre -> addWidget( new QLabel ( *HTML::decode("Applied function: ") ), 0 , Qt::AlignLeft );
  centre -> addWidget( FSaisieFonction , 1 , Qt::AlignLeft ) ;
  
  /** Etalonnage des resultats **/
  
  FAffEtalonnageResultat = 
        new QGroupBox ( *HTML::decode("Calibration of results") );
  FAffEtalonnageResultat -> setCheckable(true);
  FAffEtalonnageResultat -> setChecked(false);
  
  QVBoxLayout * aer_vbl = new QVBoxLayout ( FAffEtalonnageResultat ) ;

  QWidget * WidgetMin2 = new QWidget ;
  QHBoxLayout * min2 = new QHBoxLayout ( WidgetMin2) ;
  FAffMinResultat = new QLabel ( "Minimum:" ) ;
  FSaisieMinResultat = new  FloatSpinBox ( 0.5 ) ;
  
  min2 -> addWidget (  FAffMinResultat, 0, Qt::AlignRight ) ;
  min2 -> addWidget (  FSaisieMinResultat, 0, Qt::AlignLeft ) ;

  QWidget * WidgetMax2 = new QWidget ;
  QHBoxLayout * max2 = new QHBoxLayout ( WidgetMax2 ) ;
  FAffMaxResultat = new QLabel ( "Maximum:" ) ;  
  FSaisieMaxResultat = new  FloatSpinBox ( 0.5 ) ;
  
  max2 -> addWidget (  FAffMaxResultat, 0, Qt::AlignRight ) ;
  max2 -> addWidget (  FSaisieMaxResultat, 0, Qt::AlignLeft ) ;

  aer_vbl -> addWidget(WidgetMin2);
  aer_vbl -> addWidget(WidgetMax2);
  
  bas -> addWidget ( FAffEtalonnageDistance ) ;
  bas -> addWidget ( FAffEtalonnageResultat ) ;

  placement -> addWidget ( FAffDistance ) ;
  placement -> addWidget ( WidgetCentre ) ;
  placement -> addWidget ( WidgetBas ) ;
  


  // On definit toutes les actions a effectuer
  QObject :: connect ( FSaisieDistance , 
		       SIGNAL ( activated ( int ) ) ,
		       this ,
		       SLOT ( callbackDistance ( ) ) ) ;
  QObject :: connect ( FSaisieFonction ,
		       SIGNAL ( activated ( int ) ) ,
		       this ,
		       SLOT ( callbackFunction ( ) ) ) ;
  QObject :: connect ( FAffEtalonnageDistance ,
            SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackToggleEtalonnageDistances ( ) ) ) ;
  QObject :: connect ( FAffEtalonnageResultat ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackToggleEtalonnageResultats ( ) ) ) ;
  QObject :: connect ( FSaisieMinDistance ,
		       SIGNAL ( valueChanged ( double ) ) ,
		       this ,
		       SLOT ( callbackMin1Changed ( ) ) ) ;
  QObject :: connect ( FSaisieMaxDistance ,
		       SIGNAL ( valueChanged ( double ) ) ,
		       this ,
		       SLOT ( callbackMax1Changed ( ) ) ) ;
  QObject :: connect ( FSaisieMinResultat ,
		       SIGNAL ( valueChanged ( double ) ) ,
		       this ,
		       SLOT ( callbackMin2Changed ( ) ) ) ;
  QObject :: connect ( FSaisieMaxResultat ,
		       SIGNAL ( valueChanged ( double ) ) ,
		       this ,
		       SLOT ( callbackMax2Changed ( ) ) ) ;

  for (int i = 0 ; i < 3 ; i ++ ) {
    QObject :: connect ( FSaisieCoefSommet [i] ,
			 SIGNAL ( valueChanged ( double ) ) ,
			 this ,
			 SLOT ( callbackVertexChanged ( ) ) ) ;
  
    QObject :: connect ( FSaisieCoefVecteur [i] ,
			 SIGNAL ( valueChanged ( double ) ) ,
			 this ,
			 SLOT ( callbackVectorChanged ( ) ) ) ;
  }

  QObject :: connect ( FRecupSommet ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackTakeCenter ( ) ) ) ;
  QObject :: connect ( FRecupVecteur ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackTakeVector ( ) ) ) ;
}


// Destructeur
//------------
OptionsPonderation :: ~OptionsPonderation ( ) 
{
}

// Mise a jour de l'affichage
//---------------------------
void OptionsPonderation :: update ( )
{ 
  FUpdate = true ;
  
  CParameterObjectPonderation * p = FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ;
  
  FSaisieCoefSommet [ 0 ] -> setValuef ( p -> getPonderationCenter ( ) . getX ( ) ) ;
  FSaisieCoefSommet [ 1 ] -> setValuef ( p -> getPonderationCenter ( ) . getY ( ) ) ;
  FSaisieCoefSommet [ 2 ] -> setValuef ( p -> getPonderationCenter ( ) . getZ ( ) ) ;

  FSaisieCoefVecteur [ 0 ] -> setValuef ( p -> getPonderationVector ( ) . getX ( ) ) ;
  FSaisieCoefVecteur [ 1 ] -> setValuef ( p -> getPonderationVector ( ) . getY ( ) ) ;
  FSaisieCoefVecteur [ 2 ] -> setValuef ( p -> getPonderationVector ( ) . getZ ( ) ) ;

  // Pour distancetype 
  FSaisieDistance -> setCurrentIndex ( p->getPonderationType ( ) ) ;

  const char * vectorLabs [] = {"", "Direction Vector: ", "Normal Vector: "};

  for ( int i = 0 ; i < 3 ; ++i )
    if ( i == p -> getPonderationType ( ) )
      FAffVecteur -> setText ( vectorLabs [ i ] ) ;
  
  for ( int i = 0 ; i < 3 ; ++i )
    FSaisieCoefVecteur [ i ] -> setEnabled ( p -> getPonderationType ( ) != 0 ) ;
  
  FRecupVecteur -> setEnabled ( p -> getPonderationType ( ) != 0 ) ;

  // pour functiontype
  FSaisieFonction -> setCurrentIndex ( p->getPonderationFunctionType ( ) ) ;

  // Pour les min et max
  FSaisieMinDistance -> setValuef ( p -> getPonderationStretchInMin ( ) ) ;
  FSaisieMaxDistance -> setValuef ( p -> getPonderationStretchInMax ( ) ) ;
  FSaisieMinResultat -> setValuef ( p -> getPonderationStretchOutMin ( ) ) ;
  FSaisieMaxResultat -> setValuef ( p -> getPonderationStretchOutMax ( ) ) ;

  // Pour activer / desactiver les min et max
  FAffEtalonnageDistance -> setChecked ( p -> getPonderationStretchIn ( ) ) ;
  FAffEtalonnageResultat -> setChecked ( p -> getPonderationStretchOut ( ) ) ;

  FUpdate = false ;
}


// SLOTS
//------

void OptionsPonderation :: callbackDistance ( )
{
  if ( ! FUpdate )
    setDistanceType ( ( TDistanceType ) FSaisieDistance -> currentIndex ( ) ) ;
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackFunction ( )
{
  if ( ! FUpdate )
    setFunctionType((TFunctionType) FSaisieFonction -> currentIndex ( ) ) ;
  update ( ) ;
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackTakeCenter ( )
{
  CVertex center;
  if ( FParent -> getControler ( ) -> takeCenter ( center ) )
    setCenter ( center ) ;
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackTakeVector ( )
{
  CVertex vector;
  if ( FParent -> getControler ( ) -> takeVector ( vector ) )
    setVector ( vector ) ;
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackToggleEtalonnageDistances ( )
{
  if ( ! FUpdate )
    setEtalonnageDistances ( FAffEtalonnageDistance -> isChecked ( ) ) ;
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackToggleEtalonnageResultats ( )
{
  if ( ! FUpdate )
    setEtalonnageResultats ( FAffEtalonnageResultat -> isChecked ( ) ) ;
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackVertexChanged ( )
{
  if ( ! FUpdate )
    setCenter ( CVertex ( FSaisieCoefSommet [ 0 ] -> valuef ( ) ,
			  FSaisieCoefSommet [ 1 ] -> valuef ( ) ,
			  FSaisieCoefSommet [ 2 ] -> valuef ( ) ) ) ;
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackVectorChanged ( )
{
  if ( ! FUpdate )
    setVector ( CVertex ( FSaisieCoefVecteur [ 0 ] -> valuef ( ) ,
			  FSaisieCoefVecteur [ 1 ] -> valuef ( ) ,
			  FSaisieCoefVecteur [ 2 ] -> valuef ( ) ) ) ;
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackMin1Changed ( )
{
  if ( ! FUpdate )
    setMin1 ( FSaisieMinDistance -> valuef ( ) ) ; 
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackMax1Changed ( )
{
  if ( ! FUpdate )
    setMax1 ( FSaisieMaxDistance -> valuef ( ) ) ; 
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackMin2Changed ( )
{
  if ( ! FUpdate )
    setMin2 ( FSaisieMinResultat -> valuef ( ) ) ; 
}
//------------------------------------------------------------------------------
void OptionsPonderation :: callbackMax2Changed ( )
{
  if ( ! FUpdate )
    setMax2 ( FSaisieMaxResultat -> valuef ( ) ) ; 
}


// Accesseurs
//-----------

void OptionsPonderation :: setCenter ( const CVertex & vertex)
{
  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationCenter ( vertex ) ;
  update();
}
//------------------------------------------------------------------------------
void OptionsPonderation :: setVector ( const CVertex & vertex )
{

  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationVector ( vertex ) ;
  update();
}
//------------------------------------------------------------------------------
void OptionsPonderation :: setDistanceType ( TPonderationType type )
{
  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationType ( type ) ;
  update();
}
//------------------------------------------------------------------------------
void OptionsPonderation :: setFunctionType( TFunctionType type )
{
  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationFunctionType ( type ) ;
  update();
}
//------------------------------------------------------------------------------
void OptionsPonderation :: setMin1 ( TCoordinate v )
{
  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationStretchInMin ( v ) ;
  update();
}
//------------------------------------------------------------------------------
void OptionsPonderation :: setMax1 ( TCoordinate v ) 
{
  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationStretchInMax ( v ) ;
  update();
}
//------------------------------------------------------------------------------
void OptionsPonderation :: setMin2 ( TCoordinate v )
{
  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationStretchOutMin ( v ) ;
  update ( ) ;
}
//------------------------------------------------------------------------------
void OptionsPonderation :: setMax2 ( TCoordinate v )
{
  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationStretchOutMax ( v ) ;
  update();
}
//------------------------------------------------------------------------------
void OptionsPonderation :: setEtalonnageDistances ( bool b )
{
  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationStretchIn ( b ) ;
  update();  
}
//------------------------------------------------------------------------------
void OptionsPonderation :: setEtalonnageResultats ( bool b )
{
  FParent -> getControler ( ) -> getParameterObjectPonderation ( ) ->
    setPonderationStretchOut ( b ) ;
  update();  
}
//******************************************************************************
