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

#include "options-vue.qt.hh"
#include "window.qt.hh"
#include "options-frame.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

#define SCALE_MAX (1.0)

static QString elementLabs [ 2 ] =
{
  "By barycenter",
  *HTML::decode("By offset"),
};

// Constructeur
//-------------
OptionsVue :: OptionsVue ( Window * parent , OptionsFrame *  parentTab )  
  :
  QWidget ( parentTab ) ,
  FParentTab ( parentTab ) ,
  FParent ( parent ) ,
  FUpdate ( false )
{
  QVBoxLayout * placement = new QVBoxLayout ( this ) ;

  QWidget * WidgetHaut = new QWidget (this ) ;  
  QGridLayout * haut = new QGridLayout ( WidgetHaut ) ;
  
  //-------------------------
  FTypeEclatement=new QGroupBox ( *HTML::decode("Splitted view" ) );
  QVBoxLayout* tet_vbl = new QVBoxLayout(FTypeEclatement);  
  
  for (int i=0;i<2;i++)
  {
    FBurstView[i] = new IdRadioButton(i,elementLabs[i]);
    tet_vbl -> addWidget ( FBurstView[i] );
    
    connect ( FBurstView[i] , SIGNAL (  clicked ( int ) )  ,
              this , SLOT ( callbackBurstView( int ) ) ) ;
  }
  
  haut -> addWidget ( FTypeEclatement, 0, 0 ) ;
  
  //-------- Interactivite -----------------
  FInteractivite = new QGroupBox ( "Interactives modifications" ) ;
  FInteractivite -> setCheckable (true);
  FInteractivite -> setChecked (true) ;
  
  QVBoxLayout* int_vbl = new QVBoxLayout(FInteractivite);  
  FIAppliquer = new QPushButton ( "Apply Modifications" ) ;
  FIAppliquer -> setEnabled ( false ) ;
  FIRecuperer = new QPushButton ( *HTML::decode(
                                   "Recover Values") ) ;
  FIRecuperer -> setEnabled ( false ) ;

  int_vbl -> addWidget(FIAppliquer);
  int_vbl -> addWidget(FIRecuperer);
  
  connect ( FInteractivite , SIGNAL ( clicked ( ) ) ,
		       this , SLOT ( callbackToggleInteractive ( ) ) ) ;

  connect ( FIAppliquer , SIGNAL ( clicked ( ) ) ,
		       this , SLOT ( callbackApply ( ) ) ) ;

  connect ( FIRecuperer , SIGNAL ( clicked () ) ,
		       this , SLOT ( callbackRecover ( ) ) ) ;

  haut -> addWidget ( FInteractivite, 1, 0 ) ;

  //-------------------------
  FPrereglages = new QGroupBox ( *HTML::decode("Presets") ) ;
  QGridLayout * prereglage = new QGridLayout ( FPrereglages ) ;

  FPCompacte = new QPushButton ( "Compacted view (Shift-F8)") ;
  FPSemiEclatee = 
        new QPushButton ( *HTML::decode("Semi-splitted view (Shift-F9)")) ;
  FPMoka=new QPushButton(*HTML::decode(
                          "Splitted view, style \"moKa\" (Shift-F10)")) ;  
  FPTopofil=new QPushButton(*HTML::decode("Splitted View, style "
        "\"Topofil\" (Shift-F11)")) ;
  FPSommets = new QPushButton ( "Vertices" ) ;
  FPAretes = new QPushButton ( *HTML::decode("Edges") ) ;
  FPFaces = new QPushButton ( "Faces"  ) ;
  FPVolumes = new QPushButton ( "Volumes" ) ;
  
  prereglage -> addWidget ( FPCompacte , 1 , 0 ) ;
  prereglage -> addWidget ( FPSemiEclatee , 2 , 0 ) ;
  prereglage -> addWidget ( FPMoka , 3 , 0 ) ;
  prereglage -> addWidget ( FPTopofil , 4 , 0 ) ;
  prereglage -> addWidget ( FPSommets , 1 , 1 ) ;
  prereglage -> addWidget ( FPAretes , 2 , 1 ) ;
  prereglage -> addWidget ( FPFaces , 3 , 1 ) ;
  prereglage -> addWidget ( FPVolumes , 4 , 1 ) ;

  haut -> addWidget ( FPrereglages, 0, 1, 2, 1 ) ;

  //-------------------------
  FParametres = new QGroupBox ( *HTML::decode("Parameters") ) ;
  QGridLayout * parametre = new QGridLayout ( FParametres ) ;

  FAffEclatement = new QLabel ( "Splitting" ) ;
  FAffPosition = new QLabel ( "Position" ) ;
  FAffEpaisseur = new QLabel ( "Thickening" ) ;

  parametre -> addWidget ( FAffEclatement , 1 , 1 ) ;
  parametre -> addWidget ( FAffPosition , 1 , 4 ) ;
  parametre -> addWidget ( FAffEpaisseur , 1 , 6 ) ;

  const char * alpha [] = { "Alpha0 :", "Alpha1 :", "Alpha2 :", "Alpha3 :" };
  const char * couture [] = { "0-Sews" , "1-Sews" , "2-Sews" ,
     "3-Sews" } ;
      
  for (int i = 0 ; i < 4 ; i++ ) 
  {
    // Les alpha
    FParAlpha [i] = new QLabel ( alpha [i] , FParametres ) ;
    FParEclatement [i] = new QSlider ( FParametres ) ;
    FParEclatement [i] -> setMinimum ( 0 ) ;
    FParEclatement [i] -> setMaximum ( 100 ) ;
    FParEclatement [i] -> setSingleStep ( 3 ) ;
    FParEclatement [i] -> setValue ( 0 ) ;
    FParEclatement [i] -> setOrientation ( Qt::Horizontal );

    FParAffEclatement [i] = new QLabel ( 0 , FParametres ) ;
    FParCouture [i] = new QCheckBox ( couture [i] , FParametres ) ;
    
    FParPosition [i] = new QSlider ( FParametres ) ;
    FParPosition [i] -> setMinimum ( 0 ) ;
    FParPosition [i] -> setMaximum ( 100 ) ;
    FParPosition [i] -> setSingleStep ( 3 ) ;
    FParPosition [i] -> setValue ( 0 ) ;
    FParPosition [i] -> setOrientation ( Qt::Horizontal );
    
    FParAffPosition [i] = new QLabel ( 0 , FParametres ) ;
    FParEpaisseur [i] = new QSpinBox ( FParametres ) ;
    FParEpaisseur [i] -> setMinimum ( 1 ) ;
    FParEpaisseur [i] -> setMaximum ( 10 ) ;
    FParEpaisseur [i] -> setAccelerated(true);
          
    parametre -> addWidget ( FParAlpha [i] , i+2 , 0 ) ;
    parametre -> addWidget ( FParEclatement [i] , i+2 , 1 ) ;
    parametre -> addWidget ( FParAffEclatement [i] , i+2 , 2 ) ;
    parametre -> addWidget ( FParCouture [i] , i+2 , 3 ) ;
    parametre -> addWidget ( FParPosition [i] , i+2 , 4 ) ;
    parametre -> addWidget ( FParAffPosition [i] , i+2 , 5 ) ;
    parametre -> addWidget ( FParEpaisseur [i] , i+2 , 6 ) ;
  }

  placement -> addWidget ( WidgetHaut ) ;
  placement -> addWidget ( FParametres ) ;
  
  connect ( FPCompacte , SIGNAL ( clicked () ) ,
            this , SLOT ( callbackPresetting0 ( ) ) ) ;
  
  connect ( FPSemiEclatee , SIGNAL ( clicked () ) ,
            this , SLOT ( callbackPresetting1 ( ) ) ) ;
  
  connect ( FPMoka , SIGNAL ( clicked () ) ,
            this , SLOT ( callbackPresetting2 ( ) ) ) ;

  connect ( FPTopofil , SIGNAL ( clicked () ) ,
            this , SLOT ( callbackPresetting3 ( ) ) ) ;
  
  connect ( FPSommets , SIGNAL ( clicked () ) ,
            this , SLOT ( callbackPresetting4 ( ) ) ) ;
  
  connect ( FPAretes , SIGNAL ( clicked () ) ,
            this , SLOT ( callbackPresetting5 ( ) ) ) ;
  
  connect ( FPFaces , SIGNAL ( clicked () ) ,
            this , SLOT ( callbackPresetting6 ( ) ) ) ;
  
  connect ( FPVolumes , SIGNAL ( clicked () ) ,
            this , SLOT ( callbackPresetting7 ( ) ) ) ;
  
  // -- connection les slider pour les alpha
  QObject :: connect ( FParEclatement [0] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackBurstAlpha0 ( ) ) ) ;
  QObject :: connect ( FParEclatement [1] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackBurstAlpha1 ( ) ) ) ;
  QObject :: connect ( FParEclatement [2] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackBurstAlpha2 ( ) ) ) ;
  QObject :: connect ( FParEclatement [3] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackBurstAlpha3 ( ) ) ) ;

  // -- connection pour les checkbox des coutures
  QObject :: connect ( FParCouture [0] ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackSew0 ( ) ) ) ;
  QObject :: connect ( FParCouture [1] ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackSew1 ( ) ) ) ;
  QObject :: connect ( FParCouture [2] ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackSew2 ( ) ) ) ;
  QObject :: connect ( FParCouture [3] ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackSew3 ( ) ) ) ;

  // -- connection pour les slider pour les positions
  QObject :: connect ( FParPosition [0] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSewposAlpha0 ( ) ) ) ;
  QObject :: connect ( FParPosition [1] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSewposAlpha1 ( ) ) ) ;
  QObject :: connect ( FParPosition [2] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSewposAlpha2 ( ) ) ) ;
  QObject :: connect ( FParPosition [3] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSewposAlpha3 ( ) ) ) ;

  // -- connection pour les spinbox des epaisseur
  QObject :: connect ( FParEpaisseur [0] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSew0Width ( ) ) ) ;
  QObject :: connect ( FParEpaisseur [1] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSew1Width ( ) ) ) ;
  QObject :: connect ( FParEpaisseur [2] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSew2Width ( ) ) ) ;
  QObject :: connect ( FParEpaisseur [3] ,
		       SIGNAL ( valueChanged ( int ) ) ,
		       this ,
		       SLOT ( callbackSew3Width ( ) ) ) ;
}

// Destructeur
//------------
OptionsVue :: ~OptionsVue ( ) 
{
}

// Mise a jour de l'affichage
//---------------------------
void OptionsVue :: update ( )
{
   if (FUpdate) return;
   
  FUpdate = true ;

  CParameterSew * p = FParent -> getParameterSew ( ) ;
  
  for ( int i = 0 ; i < 4 ; ++i )
    { 
      // QSlider
      setValueEclatement ( FParent -> getControler ( ) -> 
            getMapBurstCoef ( i ) , i ) ;
      // QSlider
      setValuePosition ( p -> getSewPosCoef ( i ) , i ) ;
      
      FParCouture [ i ] -> setChecked ( p -> getDisplayAlpha ( i ) ) ;
      FParEpaisseur [ i ] -> setValue( p -> getLWSew ( i ) ) ;

      
      FParCouture [ i ] -> setEnabled ( FParent -> getSewsDisplay ( ) );

      FParEpaisseur [ i ] -> setEnabled ( FParent -> getSewsDisplay ( ) 
				    && p -> getDisplayAlpha ( i ) ) ;
      
      FParPosition [ i ] -> setEnabled( FParent -> getSewsDisplay ( )
				   && p -> getDisplayAlpha ( i ) ) ;
      FParAffPosition [ i ] -> setEnabled ( FParent -> getSewsDisplay ( )
					  && p -> getDisplayAlpha ( i ) ) ;

      FBurstView[FParent -> getControler() -> getBurstMethod()] 
            -> setChecked( true ) ;
    }

  if ( getInteractive ( ) ) 
    {
      FParent -> repaint ( ) ;
    }

  FUpdate = false ;
}

// SLOTS
//------

//--- Interactivite ---

//Action a effectuer lorsque l'on coche ou decoche la case
// "Modifications interactive"
void OptionsVue :: callbackToggleInteractive ( )
{
  if ( ! FUpdate )
    setInteractive ( getInteractive ( ) ) ; 
}

void OptionsVue :: callbackApply ( )
{
  if ( ! FUpdate ) applyValues ( ); 
}

void OptionsVue :: callbackRecover ( )
{
  if ( ! FUpdate ) update ( ) ; 
}

void OptionsVue ::callbackBurstView ( int id ) 
{
  if ( ! FUpdate )
    {
      FParent->getControler()->setBurstMethod( id );
      FParent->repaint ( ) ;
    }
}

// --- Parametres ---
void OptionsVue :: callbackBurstAlpha0 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    // Mise a jour de l'affichage txt
    char ValEclatement [8] ; 
    sprintf ( ValEclatement , "%5.2f", getAlphaCoef ( 0 ) ) ;
    FParAffEclatement [ 0 ] -> setText ( ValEclatement ) ;
    
    FParent -> getControler ( ) -> setMapBurstCoef ( 0 , getAlphaCoef ( 0 ) ) ;
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackBurstAlpha1 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    // Mise a jour de l'affichage txt
    char ValEclatement [8] ; 
    sprintf ( ValEclatement , "%5.2f", 
	     ( float ( FParEclatement [ 1 ] -> value ( ) ) ) / 100.0 ) ;
    FParAffEclatement [ 1 ] -> setText ( ValEclatement ) ;

    FParent -> getControler ( ) -> setMapBurstCoef ( 1 , getAlphaCoef ( 1 ) ) ;
    FParent -> repaint ( ) ;
  }  
}

void OptionsVue :: callbackBurstAlpha2 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    // Mise a jour de l'affichage txt
    char ValEclatement [8] ; 
    sprintf ( ValEclatement , "%5.2f" , 
	     ( float ( FParEclatement [ 2 ] -> value ( ) ) ) / 100.0 ) ;
    FParAffEclatement [ 2 ] -> setText ( ValEclatement ) ;

    FParent -> getControler ( ) -> setMapBurstCoef ( 2 , getAlphaCoef ( 2 ) ) ;
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackBurstAlpha3 ( )
{
  if ( !FUpdate && getInteractive ( ) ) { 
    // Mise a jour de l'affichage txt
    char ValEclatement [8] ; 
    sprintf ( ValEclatement , "%5.2f", 
	     ( float ( FParEclatement [ 3 ] -> value ( ) ) ) / 100.0 ) ;
    FParAffEclatement [ 3 ] -> setText ( ValEclatement ) ;

    FParent -> getControler ( ) -> setMapBurstCoef ( 3 , getAlphaCoef ( 3 ) ) ;
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSewposAlpha0 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewposCoef ( 0 , getSewposCoef ( 0 ) ) ; 
    
    char ValPosition [8] ; 
    sprintf ( ValPosition , "%5.2f", 
	     ( float ( FParPosition [ 0 ] -> value ( ) ) ) / 100.0 ) ;
    FParAffPosition[0] -> setText ( ValPosition ) ;
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSewposAlpha1 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewposCoef ( 1 , getSewposCoef ( 1 ) ) ; 
    char ValPosition [8] ; 
    sprintf ( ValPosition , "%5.2f", 
	     ( float ( FParPosition [ 1 ] -> value ( ) ) ) / 100.0 ) ;
    FParAffPosition [ 1 ] -> setText ( ValPosition ) ;
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSewposAlpha2 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewposCoef ( 2 , getSewposCoef ( 2 ) ) ; 
    char ValPosition [8] ; 
    sprintf ( ValPosition , "%5.2f", 
	     ( float ( FParPosition [ 2 ] -> value ( ) ) ) / 100.0 ) ;
    FParAffPosition [ 2 ] -> setText ( ValPosition ) ;
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSewposAlpha3 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewposCoef ( 3 , getSewposCoef ( 3 ) ) ; 
    char ValPosition [8] ; 
    sprintf ( ValPosition , "%5.2f", 
	     ( float ( FParPosition [ 3 ] -> value ( ) ) ) / 100.0 ) ;
    FParAffPosition [ 3 ] -> setText ( ValPosition ) ;
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSew0 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewDisplay ( 0 , getSewDisplay ( 0 ) ) ; 
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSew1 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewDisplay ( 1 , getSewDisplay ( 1 ) ) ; 
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSew2 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewDisplay ( 2 , getSewDisplay ( 2 ) ) ; 
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSew3 ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewDisplay ( 3 , getSewDisplay ( 3 ) ) ; 
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSew0Width ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewWidth ( 0 , getSewWidth ( 0 ) ) ; 
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSew1Width ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewWidth ( 1 , getSewWidth ( 1 ) ) ; 
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSew2Width ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewWidth ( 2 , getSewWidth ( 2 ) ) ; 
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: callbackSew3Width ( )
{
  if ( !FUpdate && getInteractive ( ) ) {
    setSewWidth ( 3 , getSewWidth ( 3 ) ) ; 
    FParent -> repaint ( ) ;
  }
}

// -- Prereglages --
void OptionsVue :: callbackPresetting0 ( )
{
  if ( ! FUpdate ) 
    setPresetting ( 0 ) ; 
}

void OptionsVue :: callbackPresetting1 ( )
{
  if ( ! FUpdate ) 
    setPresetting ( 1 ) ; 
}

void OptionsVue :: callbackPresetting2 ( )
{
  if ( ! FUpdate )
    setPresetting ( 2 ) ; 
}

void OptionsVue :: callbackPresetting3 ( )
{
  if ( ! FUpdate ) setPresetting ( 3 ) ; 
}

void OptionsVue :: callbackPresetting4 ( )
{
  if ( ! FUpdate ) setPresetting ( 4 ) ; 
}

void OptionsVue :: callbackPresetting5 ( )
{
  if ( ! FUpdate ) setPresetting ( 5 ) ; 
}

void OptionsVue :: callbackPresetting6 ( )
{
  if ( ! FUpdate ) setPresetting ( 6 ) ; 
}

void OptionsVue :: callbackPresetting7 ( )
{
  if ( ! FUpdate ) setPresetting ( 7 ) ; 
}

// Accesseurs en lecture
//----------------------
float OptionsVue :: getAlphaCoef ( int i ) const
{
  assert(i>=0 && i<4); 
  return ( ( (float) FParEclatement [i] -> value ( ) ) / 100.0 ) ; 
}

float OptionsVue :: getSewposCoef ( int i ) const
{
  assert(i>=0 && i<4);
  return ( ( ( float ) FParPosition [i] -> value ( ) ) / 100.0 ) ; 
}

bool  OptionsVue :: getSewDisplay ( int i ) const
{ 
  assert(i>=0 && i<4);
  return FParCouture [i] -> isChecked ( ) ; 
}

int   OptionsVue :: getSewWidth ( int i ) const
{ 
  assert(i>=0 && i<4); 
  return FParEpaisseur [i] -> value ( ) ; 
}


bool  OptionsVue :: getInteractive ( ) const
{
  return FInteractivite -> isChecked ( ) ; 
}

// Accesseurs en ecriture
//-----------------------
void OptionsVue :: setAlphaCoefs ( float v0 , float v1 , float v2 , float v3 )
{
  bool update = FUpdate ;
  FUpdate = true ;
  setAlphaCoef ( 0 ,v0 ) ;
  setAlphaCoef ( 1 ,v1 ) ;
  setAlphaCoef ( 2 ,v2 ) ;
  setAlphaCoef ( 3 ,v3 ) ;
  FUpdate = update ;

  if ( getInteractive ( ) && !FUpdate ) {
    FParent -> repaint ( ) ;
  }
}

void OptionsVue :: setSewposCoefs ( float v0 , float v1 , float v2 , float v3 )
{
  bool update = FUpdate ;
  FUpdate = true ;
  setSewposCoef ( 0 , v0 ) ;
  setSewposCoef ( 1 , v1 ) ;
  setSewposCoef ( 2 , v2 ) ;
  setSewposCoef ( 3 , v3 ) ;
  FUpdate = update ;

  if ( getInteractive () && !FUpdate )
    FParent -> repaint ( ) ;
}

void OptionsVue :: setSewsDisplay ( bool b0 , bool b1 , bool b2 , bool b3 )
{
  bool update = FUpdate ;
  FUpdate = true ;
  setSewDisplay ( 0 , b0 ) ;
  setSewDisplay ( 1 , b1 ) ;
  setSewDisplay ( 2 , b2 ) ;
  setSewDisplay ( 3 , b3 ) ;
  FUpdate = update ;

  if ( getInteractive () && !FUpdate )
    FParent -> repaint ( ) ;
}

void OptionsVue :: setAlphaCoef ( int i , float v )
{
  assert ( i >= 0 ) ;
  assert ( i < 4 ) ;
  assert ( v >= 0 ) ;
  assert ( v <= SCALE_MAX ) ;

  setValueEclatement ( v , i ) ;

  if ( getInteractive ( ) )
    FParent -> getControler ( ) -> setMapBurstCoef ( i , v) ;
}

void OptionsVue :: setSewposCoef ( int i , float v )
{
  assert ( i >= 0 ) ; 
  assert ( i < 4 ) ; 
  assert ( v >= 0 ) ;
  assert ( v <= 1 ) ;

  setValuePosition ( v , i ) ;
  if ( getInteractive ( ) )
    FParent -> getParameterSew ( ) -> setSewPosCoef ( i , v ) ;
}

void OptionsVue :: setSewDisplay ( int i , bool b )
{
  assert ( i >= 0 ) ;
  assert ( i < 4 ) ;

  FParCouture [ i ] -> setChecked ( b ) ;

  FParPosition [ i ] -> setEnabled ( b &&
				   FParent -> getSewsDisplay ( ) ) ;
  FParAffPosition [ i ] -> setEnabled ( b &&
				      FParent -> getSewsDisplay ( ) ) ;
  FParEpaisseur [ i ] -> setEnabled( b && 
				  FParent -> getSewsDisplay ( ) ) ;

  if ( getInteractive ( ) )
    FParent -> getParameterSew ( ) -> setDisplayAlpha ( i , b ) ;

}

void OptionsVue :: setSewWidth ( int i , int w )
{
  assert ( i >= 0 ) ;
  assert ( i <= 3 ) ;
  assert ( w > 0 ) ;

  FParEpaisseur [ i ] -> setValue ( w ) ;

  if ( getInteractive ( ) )
    FParent -> getParameterSew ( ) -> setLWSew ( i , w ) ;
}


void OptionsVue :: setInteractive ( bool b )
{  
  FInteractivite -> setChecked ( b ) ;
  FIAppliquer -> setEnabled ( !b ) ;
  FIRecuperer -> setEnabled ( !b ) ;
  
  if ( b )
    update ( ) ;
}

void OptionsVue :: applyValues ( )
{
  for ( int i = 0 ; i < 4 ; ++i )
    {
      FParent-> getControler ( ) ->setMapBurstCoef ( i , getAlphaCoef ( i ) );
      FParent-> getParameterSew()->setSewPosCoef ( i , getSewposCoef ( i ) );
      FParent-> getParameterSew()->setDisplayAlpha ( i , getSewDisplay ( i ) );
      FParent-> getParameterSew()->setLWSew ( i , getSewWidth ( i ) );
    }

  FParentTab -> applyValues ( ) ;
  FParent -> repaint ( ) ;
}

void OptionsVue :: setPresetting ( int P )
{
  switch ( P )
    {
    case Compact:
      setAlphaCoefs( 1.00 , 1.00 , 1.00 , 1.00 ) ;
      setSewposCoefs ( 1.0 , 0.0 , 1.0 , 1.0 ) ;
      setSewsDisplay ( false , false , false , false ) ;
      break ;
    case SemiBurst:
      setAlphaCoefs( 1.00 , 1.00 , 0.85 , 0.96 ) ;
      setSewposCoefs ( 1.0 , 0.2 , 0.9 , 0.0 ) ;
      setSewsDisplay ( false , true , true , true ) ;
      break ;
    case Burst:
      setAlphaCoefs ( 1.00 , 1.00 , 0.80 , 0.50 );
      setSewposCoefs ( 0.8 , 0.0 , 0.5 , 0.0 ) ;
      setSewsDisplay ( false , true , true , true ) ;
      break ;
    case AltBurst:
      setAlphaCoefs ( 0.90 , 0.90 , 0.90 , 0.90 );
      setSewposCoefs ( 1.0 , 0.2 , 0.5 , 0.6 ) ;
      setSewsDisplay ( true , true , true , true );
      break ;
    case VertexEmphase:
      setAlphaCoefs ( 0.50 , 1.00 , 0.97 , 0.98 );
      setSewposCoefs ( 1.0 , 0.1 , 0.9 , 0.5 ) ;
      setSewsDisplay ( false, true , true , true );
      break ;
    case EdgeEmphase:
      setAlphaCoefs ( 1.00 , 0.50 , 0.97 , 0.98 ) ;
      setSewposCoefs ( 1.0 , 0.1 , 0.9 , 0.5 ) ;
      setSewsDisplay ( true , false , true , true ) ;
      break ;
    case FaceEmphase:
      setAlphaCoefs ( 1.00 , 1.00 , 0.50 , 0.96 ) ;
      setSewposCoefs ( 1.0 , 0.1 , 0.9 , 0.5 ) ;
      setSewsDisplay ( true , true , false, true ) ;
      break ;
    case VolumeEmphase:
      setAlphaCoefs( 1.00 , 1.00 , 0.97 , 0.65 ) ;
      setSewposCoefs ( 1.0 , 0.1 , 0.9 , 0.5) ;
      setSewsDisplay ( true , true , true , false) ;
      break ;
    default: assert ( false ) ; break ;
    }

}

void OptionsVue :: setValueEclatement ( TCoordinate c , int i ) 
{
  FParEclatement [ i ] -> setValue ( int ( c * 100.0 ) ) ;
  char ValEclatement [8] ; 
  sprintf ( ValEclatement , "%5.2f", 
	   ( float ( FParEclatement [ i ] -> value ( ) ) ) / 100.0 ) ;
  FParAffEclatement[i] -> setText ( ValEclatement ) ;
}

void OptionsVue :: setValuePosition ( float f , int i ) 
{
  FParPosition [ i ] -> setValue ( int ( f *100.0) ) ;
  char ValPosition [8] ; 
  sprintf ( ValPosition , "%5.2f", 
	   ( float ( FParPosition [ i ] -> value ( ) ) ) / 100.0 ) ;
  FParAffPosition[i] -> setText ( ValPosition ) ;
}


