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

#include "dialog-homothetie.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QPixmap>
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>

// Constructeur
//-------------
dialogHomothetie :: dialogHomothetie ( Window * parent ,
				       GMap3d :: CControlerGMap * controler ) 
  :
  dialogOperations ( parent , HTML::decode("Homothety" ) ,
		     controler , Qt::BottomToolBarArea ) 
{
  
  /**** Partie gauche ****/
  QWidget * gauche = new QWidget ;
  QGridLayout * bgauche = new QGridLayout ( gauche ) ;
  bgauche -> setSpacing(0);
  bgauche -> setContentsMargins(0,0,0,0); 
  
  FAffParam = new QLabel ( *HTML::decode("  Homothety with respect  ")) ;
  QLabel* FRapport = new QLabel (*HTML::decode("to: ")) ;
  FSaisieParam = new QComboBox ;
  FSaisieParam -> addItem ( "Vertex" ) ;
  FSaisieParam -> addItem ( "Line" ) ;
  FSaisieParam -> addItem ( "Plane" ) ;  
  
  bgauche -> addWidget ( FAffParam, 0, 0, 1, 4 );
  bgauche -> addWidget ( FRapport, 1, 0, Qt::AlignRight | Qt::AlignTop);
  bgauche -> addWidget ( FSaisieParam, 1, 1, 1, 3, 
                         Qt::AlignLeft | Qt::AlignTop) ;
  
  /**** Partie haute ****/
  QWidget * haut = new QWidget ;
  QHBoxLayout * bhaut = new QHBoxLayout ( haut ) ;
  bhaut -> setSpacing(0);
  bhaut -> setContentsMargins(0,0,0,0); 
  
  FAffSommet = new QLabel ( "  Vertex : " ) ;
  FSaisiePonderation = new QCheckBox 
        ( *HTML::decode("Weighted Homothety") , haut ) ;
  
  FSaisie = new champsOperations ( this , bhaut ) ;
  bhaut -> addSpacing ( SPACE_PIX_SIZE ) ;
  bhaut -> addWidget ( FSaisiePonderation ) ;
  bhaut -> addStretch ( 1 );

  /**** Partie basse ****/
  
  QWidget * bas = new QWidget ;
  QHBoxLayout * bbas = new QHBoxLayout ( bas );
  bbas -> setSpacing(0);
  bbas -> setContentsMargins(0,0,0,0); 

  FAffVecteur = new QLabel ;
  FAffCoef = new QLabel ( "Weight : " );

  FSaisieCoef = new FloatSpinBox (0.2);
  FSaisieCoef -> setWrapping ( true ) ;

  FSaisieIso = new QCheckBox ( "iso" ) ;
  QIcon p2 ( ":/boutonAnnuler.png" ) ;
  FClose = new QPushButton ( p2 , "Close" ) ;
  FClose -> setShortcut ( Qt :: Key_Space ) ;
  
  bbas -> addWidget ( FAffVecteur ) ;
  FSaisieVecteur = new champsOperations ( this , bbas ) ;
  bbas -> addSpacing ( SPACE_PIX_SIZE ) ;
  bbas -> addWidget ( FSaisieIso ) ;
  bbas -> addSpacing ( SPACE_PIX_SIZE ) ;
  bbas -> addWidget ( FAffCoef ) ;
  bbas -> addWidget ( FSaisieCoef ) ;
  bbas -> addSpacing ( SPACE_PIX_SIZE ) ;
  bbas -> addWidget ( FClose ) ;
  bbas -> addStretch ( 1 );
  
  /**** Partie basse (iso) ****/
  QWidget * bas2 = new QWidget ;
  QHBoxLayout * bbas2 = new QHBoxLayout ( bas2 );
  bbas2 -> setSpacing(0);
  bbas2 -> setContentsMargins(0,0,0,0); 
  
  FAffCoeffs = new QLabel ( "Weights: " );
  
  FAffX = new QLabel ( "  X : " ) ;
  FSaisieX = new FloatSpinBox (0.2) ;
  FSaisieX -> setWrapping ( true ) ;

  FAffY = new QLabel ( "  Y : " ) ;
  FSaisieY = new FloatSpinBox (0.2) ;
  FSaisieY -> setWrapping ( true ) ;
  
  FAffZ = new QLabel ( "  Z : " ) ;
  FSaisieZ = new FloatSpinBox (0.2) ;
  FSaisieZ -> setWrapping ( true ) ;
  
  bbas2 -> addWidget ( FAffX ) ;
  bbas2 -> addWidget ( FSaisieX ) ;
  bbas2 -> addWidget ( FAffY ) ;
  bbas2 -> addWidget ( FSaisieY ) ;
  bbas2 -> addWidget ( FAffZ ) ;
  bbas2 -> addWidget ( FSaisieZ ) ;
  bbas2 -> addStretch ( 1 );

  // Ajout au layout principal
  QWidget * centre = new QWidget ;
  QGridLayout * gl = new QGridLayout ( centre ); 
  
  gl -> setHorizontalSpacing(0);
  gl -> setVerticalSpacing(5);
  gl -> setContentsMargins(0,0,0,0); 
  gl -> addWidget(FAffSommet, 0, 0, Qt::AlignRight) ;
  gl -> addWidget(FAffVecteur, 1, 0, Qt::AlignRight) ;
  gl -> addWidget(FAffCoeffs, 2, 0, Qt::AlignRight) ;
  gl -> addWidget(haut, 0, 1) ;
  gl -> addWidget(bas, 1, 1) ;
  gl -> addWidget(bas2, 2, 1) ;
  
  // Ajout a la toolbar
  addWidget(gauche);
  addSeparator();
  addWidget(centre);
  
  update ( ) ;
 
  connect ( FSaisieParam , SIGNAL ( activated ( int ) ) ,
            this , SLOT ( callbackDistance ( ) ) ) ;

  connect ( FSaisie -> getSaisieX ( ) , SIGNAL ( valueChanged ( double ) ) , 
            this , SLOT ( callbackCx ( ) ) ) ;

  connect ( FSaisie -> getSaisieY ( ) , SIGNAL ( valueChanged ( double ) ) , 
            this , SLOT ( callbackCy ( ) ) ) ;
  
  connect ( FSaisie -> getSaisieZ ( ) , SIGNAL ( valueChanged ( double ) ) , 
            this , SLOT ( callbackCz ( ) ) ) ;

  connect ( FSaisie ->  getButtonRecuperer ( ) , SIGNAL ( clicked ( ) ) , 
            this, SLOT ( callbackTakeCenter ( ) ) ) ;
  
  connect ( FSaisiePonderation , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackTogglePonderation ( ) ) ) ;

  connect ( FSaisieVecteur -> getSaisieX ( ) , 
            SIGNAL ( valueChanged ( double ) ) , 
            this , SLOT ( callbackVx ( ) ) ) ;
  
  connect ( FSaisieVecteur -> getSaisieY ( ) , 
            SIGNAL ( valueChanged ( double ) ) , 
            this , SLOT ( callbackVy ( ) ) ) ;
  
  connect ( FSaisieVecteur -> getSaisieZ ( ) , 
            SIGNAL ( valueChanged ( double ) ) , 
            this , SLOT ( callbackVz ( ) ) ) ;
     
  connect ( FSaisieVecteur ->  getButtonRecuperer ( ) , 
            SIGNAL ( clicked ( ) ) , 
            this, SLOT ( callbackTakeVector ( ) ) ) ;
  
  connect ( FSaisieCoef , SIGNAL ( valueChanged (double ) ) ,
            this , SLOT ( callbackIsoCoef ( ) ) ) ;
  
  connect ( FSaisieX , SIGNAL ( valueChanged ( double ) ) ,
            this , SLOT ( callbackKx ( ) ) ) ;
  
  connect ( FSaisieY , SIGNAL ( valueChanged ( double ) ) ,
            this , SLOT ( callbackKy ( ) ) ) ;
  
  connect ( FSaisieZ , SIGNAL ( valueChanged ( double ) ) ,
            this , SLOT ( callbackKz ( ) ) ) ;
  
  connect ( FSaisieIso , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleIsoButton ( ) ) ) ;
  
  connect ( FClose , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackClose ( ) ) ) ;
}

// Destructeur
//------------
dialogHomothetie :: ~dialogHomothetie ( ) 
{}
// Mise a jour de l'affichage
//----------------------------
void dialogHomothetie :: update ( )
{
  FUpdate = true ;
  dialogOperations :: update ( ) ;
  
  assert ( FParent != NULL ) ;
  
  FSaisieParam->setCurrentIndex(getControler()->getObjectScaleType());
  FSaisiePonderation->setChecked(getControler()->getObjectScalePonderation());
  
  switch ( getControler ( ) -> getObjectScaleType ( ) ) {
  case 0 : // sommet
    FAffVecteur -> setText ( "  " ) ; 
    // setIso ( true ); break ;
  case 1 : // droite
    FAffVecteur -> setText ( "  Directing Vector: " ) ; break ;
  case 2 : // plan
    FAffVecteur -> setText ( "  Normal Vector: " ) ; break ;
  default : { }
  }

  bool enabled = (getControler()->getObjectScaleType()!=DistanceToVertex);
  FSaisieVecteur -> getLabelX ( ) -> setEnabled ( enabled ) ;
  FSaisieVecteur -> getLabelY ( ) -> setEnabled ( enabled ) ;
  FSaisieVecteur -> getLabelZ ( ) -> setEnabled ( enabled ) ;
  FSaisieVecteur -> getSaisieX ( ) -> setEnabled ( enabled ) ;
  FSaisieVecteur -> getSaisieY ( ) -> setEnabled ( enabled ) ;
  FSaisieVecteur -> getSaisieZ ( ) -> setEnabled ( enabled ) ;    
  FSaisieVecteur -> getButtonRecuperer ( ) -> setEnabled ( enabled );


  bool not_iso_mode = ! ( getControler()->getObjectScaleIso() ) ;
    
  FSaisieIso ->setChecked ( !not_iso_mode ) ;
  
  FSaisieCoef -> setVisible ( !not_iso_mode ) ;
  FAffCoef -> setVisible ( !not_iso_mode ) ;
  
  FAffCoeffs -> setVisible ( not_iso_mode ) ;
  FAffX -> setVisible ( not_iso_mode ) ;
  FSaisieX -> setVisible ( not_iso_mode ) ;
  FAffY -> setVisible ( not_iso_mode ) ;
  FSaisieY -> setVisible ( not_iso_mode ) ;
  FAffZ -> setVisible ( not_iso_mode ) ;
  FSaisieZ -> setVisible ( not_iso_mode ) ;
	
  FSaisie -> setValueX ( getCx ( ) ) ;
  FSaisie -> setValueY ( getCy ( ) ) ;
  FSaisie -> setValueZ ( getCz ( ) ) ;
  
  FSaisieVecteur -> setValueX ( getVx ( ) ) ;
  FSaisieVecteur -> setValueY ( getVy ( ) ) ;
  FSaisieVecteur -> setValueZ ( getVz ( ) ) ;
  
  if (!not_iso_mode)
     FSaisieCoef->setValuef(getControler() -> getObjectScaleIsoCoef());
  else
  {
     FSaisieX -> setValuef(getKx()) ;
     FSaisieY -> setValuef(getKy()) ;
     FSaisieZ -> setValuef(getKz()) ;
  }
  
  FParent -> repaint ( ) ;    
  FUpdate = false ;
}

// Accesseurs
//-----------
void dialogHomothetie :: setDistanceType ( TScaleType type )
{
  assert ( FParent != NULL ) ;
  if ( type == getControler ( ) -> getObjectScaleType ( ) )
    return;
  
  getControler ( ) -> setObjectScaleType ( type ) ;
  update ( ) ;
}

void dialogHomothetie :: setIso ( bool b )
{
  assert ( FParent != NULL ) ;
  if ( b == getControler ( ) -> getObjectScaleIso ( ) )
    return;
  
  getControler ( ) -> setObjectScaleIso ( b ) ;
  update ( ) ;
}

// SLOTS
//------
void dialogHomothetie :: callbackCx ( )
{
  if ( ! FUpdate )
    setCx ( FSaisie -> getValueX ( ) ) ; 
}

void dialogHomothetie :: callbackCy ( )
{
  if ( ! FUpdate )
    setCy ( FSaisie -> getValueY ( ) ) ;
}

void dialogHomothetie :: callbackCz ( )
{
  if ( ! FUpdate )
    setCz ( FSaisie -> getValueZ ( ) ) ; 
}

void dialogHomothetie :: callbackVx ( )
{ 
 if ( ! FUpdate )
   setVx ( FSaisieVecteur -> getValueX ( ) ) ;
}

void dialogHomothetie :: callbackVy ( )
{
if ( ! FUpdate )
  setVy ( FSaisieVecteur -> getValueY ( ) ) ; 
}

void dialogHomothetie :: callbackVz ( )
{ 
  if ( ! FUpdate )
    setVz ( FSaisieVecteur -> getValueZ ( ) ) ;
}

void dialogHomothetie :: callbackKx ( )
{ 
  if ( ! FUpdate )
    setKx ( FSaisieX -> valuef ( ) ) ;
}

void dialogHomothetie :: callbackKy ( )
{
  if ( ! FUpdate ) 
    setKy( FSaisieY -> valuef ( ) ) ; 
}

void dialogHomothetie :: callbackKz ( )
{
  if ( ! FUpdate )
    setKz ( FSaisieZ -> valuef ( ) ) ;
}

void dialogHomothetie :: callbackToggleIsoButton ( )
{
  if ( ! FUpdate )
    setIso ( FSaisieIso ->  isChecked ( ) ) ;
}

void dialogHomothetie :: callbackIsoCoef ( )
{
  if ( ! FUpdate )
    setIsoCoef ( FSaisieCoef -> valuef ( ) ) ;
}

void dialogHomothetie :: callbackDistance( )
{
  if ( ! FUpdate )
     setDistanceType ( ( TDistanceType ) (FSaisieParam -> currentIndex ( ) ) );
}

void dialogHomothetie :: callbackTakeCenter ( )
{
  assert ( FParent != NULL ) ;
  
  CVertex center;
  if ( getControler ( ) -> takeCenter ( center ) )
    setC ( center ) ;
}

void dialogHomothetie :: callbackTakeVector ( )
{
  assert ( FParent != NULL ) ;

  CVertex vector;
  if ( getControler ( ) -> takeVector ( vector ) )
    setV ( vector ) ;
}

void dialogHomothetie :: callbackTogglePonderation ( )
{
  if ( ! FUpdate )
    {
      getControler ( ) -> toggleObjectScalePonderation ( ) ;
      FParent -> repaint ( ) ;
    }
}

// Accesseurs en lecture
//----------------------
TCoordinate dialogHomothetie :: getCx ( ) const
{
  assert ( FParent != NULL ) ; 
  return getControler ( ) -> getObjectScaleCenter ( ) . getX ( ) ; 
}

TCoordinate dialogHomothetie :: getCy ( ) const
{
  assert ( FParent != NULL ) ;
  return getControler ( ) -> getObjectScaleCenter ( ) . getY ( ) ; 
}

TCoordinate dialogHomothetie :: getCz ( ) const
{
  assert ( FParent != NULL ) ;
  return getControler ( ) -> getObjectScaleCenter ( ) . getZ ( ) ; 
}

TCoordinate dialogHomothetie :: getVx ( ) const
{ 
  assert ( FParent != NULL ) ;
  return getControler ( ) -> getObjectScaleVector ( ) . getX ( ) ; 
}

TCoordinate dialogHomothetie :: getVy ( ) const
{
  assert ( FParent != NULL ) ; 
  return getControler ( ) -> getObjectScaleVector ( ) . getY ( ) ; 
}

TCoordinate dialogHomothetie :: getVz ( ) const
{ 
  assert ( FParent != NULL ) ; 
  return getControler ( ) -> getObjectScaleVector ( ) . getZ ( ) ; 
}

TCoordinate dialogHomothetie :: getKx ( ) const
{
  assert ( FParent != NULL ) ; 
  return getControler ( ) -> getObjectScaleCoef ( 0 ) ; 
}

TCoordinate dialogHomothetie :: getKy ( ) const
{
  assert ( FParent != NULL ) ;
  return getControler ( ) -> getObjectScaleCoef ( 1 ) ; 
}

TCoordinate dialogHomothetie :: getKz ( ) const
{
  assert ( FParent != NULL ) ; 
  return getControler ( ) -> getObjectScaleCoef ( 2 ) ; 
}

// Accesseurs en ecriture
//-----------------------
void dialogHomothetie :: setCx ( TCoordinate k )
{ 
  setC ( CVertex ( k , getCy ( ) , getCz ( ) ) ) ; 
}

void dialogHomothetie :: setCy ( TCoordinate k )
{
  setC ( CVertex ( getCx ( ) , k , getCz ( ) ) ) ; 
}

void dialogHomothetie :: setCz ( TCoordinate k )
{
  setC ( CVertex ( getCx ( ) , getCy ( ) , k ) ) ; 
}

void dialogHomothetie :: setVx ( TCoordinate k )
{
  setV ( CVertex ( k , getVy ( ) , getVz ( ) ) ) ; 
}

void dialogHomothetie :: setVy ( TCoordinate k )
{
  setV ( CVertex ( getVx ( ) , k , getVz ( ) ) ) ; 
}

void dialogHomothetie :: setVz ( TCoordinate k )
{
  setV ( CVertex ( getVx ( ) , getVy ( ) , k ) ) ; 
}

void dialogHomothetie :: setKx ( TCoordinate k )
{
  setK ( CVertex ( k , getKy ( ) , getKz ( ) ) ) ; 
}

void dialogHomothetie :: setKy ( TCoordinate k )
{
  setK ( CVertex ( getKx ( ) , k , getKz ( ) ) ) ; 
}

void dialogHomothetie :: setKz ( TCoordinate k )
{
  setK ( CVertex ( getKx ( ) , getKy ( ) , k ) ) ; 
}

void dialogHomothetie :: setC ( const CVertex & C )
{
  assert ( FParent != NULL ) ;
  if ( C == getControler ( ) -> getObjectScaleCenter ( ) )
    return;
  
  getControler ( ) -> setObjectScaleCenter ( C ) ;
  update ( ) ;
}

void dialogHomothetie :: setV (const CVertex & V )
{
  assert ( FParent != NULL ) ;
  if ( V == getControler ( ) -> getObjectScaleVector ( ) )
    return;

  getControler ( ) -> setObjectScaleVector ( V ) ;
  update ( ) ;
}

void dialogHomothetie :: setK ( const CVertex & K )
{
  assert ( FParent != NULL ) ;
  if ( K == getControler ( ) -> getObjectScaleCoef ( ) )
    return;
  
  getControler ( ) -> setObjectScaleCoef ( K ) ;
  update ( ) ;
}

void dialogHomothetie :: setIsoCoef ( TCoordinate x )
{
  assert ( FParent != NULL ) ;
  if ( x == getControler ( ) -> getObjectScaleIsoCoef ( ) )
    return;
  
  getControler ( ) -> setObjectScaleIsoCoef ( x ) ;
  update ( ) ;
}
