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

#include "options-affichage.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>


// Constructeur
//-------------
OptionsAffichage :: OptionsAffichage ( Window * parent ,
				       QTabWidget *  parentTab )
  :
  QWidget ( parentTab ) ,
  FParent ( parent ) ,
  FUpdate ( false ) ,
  FMain ( false )
{
  QVBoxLayout * placement = new QVBoxLayout ( this ) ;

  //--------- Elements ----------------
  FElements = new QGroupBox(*HTML::decode("Elements"));
  QHBoxLayout * elmts_hbl = new QHBoxLayout ( FElements ) ;
  
  QWidget * pElmts = new QWidget ();
  QVBoxLayout * pElmtsLayout = new QVBoxLayout ( pElmts ) ;

  FENormale = new QCheckBox ( "Normal ( Meta-F9 )" ) ;
  pElmtsLayout -> addWidget( FENormale );

  connect ( FENormale , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleNormal ( ) ) ) ;

  FECoutures = new QCheckBox ( "Sews (F9)" ) ;
  pElmtsLayout -> addWidget( FECoutures );

  connect ( FECoutures , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleSews ( ) ) ) ;

  FESommets = new QCheckBox ( "Vertices (F10)" ) ;
  pElmtsLayout -> addWidget( FESommets );

  connect ( FESommets , SIGNAL ( clicked  ( ) ) ,
            this , SLOT ( callbackToggleVertices ( ) ) ) ;
  
  FEFacesPleines = new QCheckBox ( "Faces filled (F11)" );
  pElmtsLayout -> addWidget( FEFacesPleines );
  
  connect ( FEFacesPleines , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleFaces ( ) ) ) ;
  
 // Mise en place d'un layout
 //--------------------------

   // Pour les couleurs aleatoires
   bgroup = new QGroupBox(*HTML::decode("Random colors"));
   QHBoxLayout * grp_hbl = new QHBoxLayout ( bgroup ) ;
   
   QWidget * pAffich = new QWidget ();
   QVBoxLayout * plcmtAffich = new QVBoxLayout ( pAffich ) ;

   FECoulGlobale          = new QRadioButton( "inactive" );
   FECouleursAleatoire    = new QRadioButton( "of faces" );
   FECoulVolumesAleatoire = new QRadioButton( "of volumes" );
   
   FECouleursAleatoire -> setChecked(true);
   
   plcmtAffich -> addWidget( FECoulGlobale );
   plcmtAffich -> addWidget( FECouleursAleatoire );
   plcmtAffich -> addWidget( FECoulVolumesAleatoire );

   connect ( FECoulGlobale, SIGNAL ( toggled ( bool ) ) , 
			       this , SLOT ( callbackToggleFaces ( ) ) );
   
   connect ( FECouleursAleatoire , SIGNAL ( toggled ( bool ) ) ,
             this , SLOT ( callbackFacesCoulAlea( ) ) );
   
   connect ( FECoulVolumesAleatoire, SIGNAL ( toggled ( bool ) ) ,
             this , SLOT ( callbackVolumesCoulAlea( ) ) );
   
   connect ( FECoulGlobale, SIGNAL ( toggled ( bool ) ) ,
             this , SLOT ( callbackCoulFacesGlobale( ) ) );

   // Couleurs aleatoire base sur la topologie ou la geometrie
   bgroup2 = 
         new QGroupBox( *HTML::decode("Calculated from "));
   QVBoxLayout * grp2_hbl = new QVBoxLayout ( bgroup2 ) ;
   
   FECoulTopologie = new QRadioButton( "topology" );
   FECoulGeometry = new QRadioButton( *HTML::decode("geometry") );
   
   grp2_hbl -> addWidget( FECoulTopologie );
   
#ifdef MODULE_ARCHITECTURE
   FECoulSemantique = new QRadioButton( *HTML::decode("semantic") );
   grp2_hbl -> addWidget( FECoulSemantique );
   connect (FECoulSemantique , SIGNAL ( clicked ( ) ) ,
            this , SLOT (callbackCoulSemantique( ) ) ) ; 
#endif

   grp2_hbl -> addWidget( FECoulGeometry );
   
   FECoulTopologie -> setChecked(true);
   
   connect (FECoulTopologie , SIGNAL ( clicked ( ) ) ,
           this , SLOT (callbackCoulTopologie( ) ) ) ;   
   
   connect (FECoulGeometry , SIGNAL ( clicked ( ) ) ,
		       this , SLOT (callbackCoulGeometry( ) ) ) ;
   
   grp_hbl -> addWidget(pAffich);
   grp_hbl -> addWidget(bgroup2);


   elmts_hbl -> addWidget( pElmts, 0, Qt::AlignLeft );
   elmts_hbl -> addWidget( bgroup, 0, Qt::AlignLeft );
   elmts_hbl -> addSpacing( 40 );

   
   QWidget* widget = new QWidget ;
   QHBoxLayout* wid_hbl = new QHBoxLayout(widget); 
   
  //-------- Grille -------------
  FGrille = new QGroupBox ( "Grid (F12)" ) ;
  FGrille -> setCheckable ( true );
  
  QVBoxLayout* gri_vbl = new QVBoxLayout(FGrille);

  QWidget * pTaille = new QWidget ;
  QHBoxLayout * plcmtTaille = new QHBoxLayout ( pTaille ) ;
  plcmtTaille -> setSpacing ( 0 );

  FSaisieTaille = new QSpinBox;
  FSaisieTaille -> setMinimum ( 0 );
  FSaisieTaille -> setMaximum ( 1000 );
  FSaisieTaille -> setAccelerated(true);
  
  plcmtTaille -> addWidget ( new QLabel ( "Size: " ), 0, Qt::AlignLeft ) ;
  plcmtTaille -> addWidget ( FSaisieTaille, 1, Qt::AlignLeft ) ;
  gri_vbl -> addWidget( pTaille );

  QWidget * pPlan = new QWidget ( FGrille ) ;
  QGridLayout * plcmtPlan = new QGridLayout ( pPlan );
  
  FPlanXY  = new QCheckBox ( "Plane XY" ) ;
  FValOx = new QCheckBox ( "Positive values over Ox" ) ;

  FPlanXZ = new QCheckBox ( "Plan XZ" ) ;
  FValOy = new QCheckBox ( "Positive values over Oy" ) ;

  FPlanYZ = new QCheckBox ( "Plan YZ" ) ;
  FValOz = new QCheckBox ( "Positive values over Oz" ) ;

  plcmtPlan -> addWidget ( FPlanXY , 0 , 0 ) ;
  plcmtPlan -> addWidget ( FValOx , 0 , 1 ) ;
  plcmtPlan -> addWidget ( FPlanXZ , 1 , 0 ) ;
  plcmtPlan -> addWidget ( FValOy , 1 , 1 ) ;
  plcmtPlan -> addWidget ( FPlanYZ , 2 , 0 ) ;
  plcmtPlan -> addWidget ( FValOz , 2 , 1 ) ;
  
  gri_vbl -> addWidget( pPlan );
  
  //-------- Placage -------------
  FPlacage = new QGroupBox ( "Placage" ) ;
  QVBoxLayout * plc_vbl = new QVBoxLayout(FPlacage); 
  
  FOrientation = new QCheckBox ( "Orientation    " ) ;
  FTaille = new QCheckBox ( "Size    " ) ;
  FPosition = new QCheckBox ( "Position    " ) ;
  
  plc_vbl -> setSpacing ( 0 ) ;
  plc_vbl -> addWidget ( FOrientation ) ;
  plc_vbl -> addWidget ( FTaille ) ;
  plc_vbl -> addWidget ( FPosition ) ;
  
  // Placement des widget
  FAffAxes   = new QCheckBox ( "Display axis (F12)" ) ;
  
  wid_hbl -> addWidget ( FGrille ) ;
  wid_hbl -> addWidget ( FPlacage ) ;

  placement -> addWidget ( FElements ) ;
  placement -> addWidget ( widget ) ;
  placement -> addWidget ( FAffAxes );


  connect ( FGrille , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleGrid ( ) ) ) ;
  
  connect ( FAffAxes , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleAxis ( ) ) ) ;

  connect ( FSaisieTaille , SIGNAL ( valueChanged (int ) ) ,
            this , SLOT ( callbackSize ( ) ) ) ;

  connect ( FPlanXY , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleXy ( ) ) ) ;

  connect ( FPlanXZ , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleXz ( ) ) ) ;
  
  connect ( FPlanYZ , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleYz ( ) ) ) ;

  connect ( FValOx , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackTogglePx ( ) ) ) ;
  
  connect ( FValOy , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackTogglePy ( ) ) ) ;
  
  connect ( FValOz , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackTogglePz ( ) ) ) ;
  
  connect ( FOrientation , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleOrientation ( )  ) ) ;
  
  connect ( FTaille , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackToggleScale ( )  ) ) ;
  
  connect ( FPosition , SIGNAL ( clicked ( ) ) ,
            this , SLOT ( callbackTogglePosition ( )  ) ) ;
}

// Destructeur
//------------
OptionsAffichage :: ~OptionsAffichage ( )
{}


#ifdef MODULE_ARCHITECTURE
void OptionsAffichage :: hideSemantiqueColoration ( )
{
  if (FParent -> getParameterArchi() -> isModeArchi())
  {
    FECoulSemantique -> setVisible(true);
    FECoulSemantique -> setChecked ( FParent -> getCoulSemantique() );

  }
  else
  {
    if (FECoulSemantique -> isChecked())
    {    
      callbackCoulTopologie();
      update();
      FParent -> repaint ( ) ;
    }
    FECoulSemantique -> setVisible(false);
  }
}
#endif // MODULE_ARCHITECTURE

// Mise a jour de l'affichage
//----------------------------
void OptionsAffichage :: update ( )
{
  FUpdate = true ;

  FESommets      -> setChecked ( FParent -> getVerticesDisplay ( ) ) ;
  FECoutures     -> setChecked ( FParent -> getSewsDisplay ( ) ) ;
  FENormale      -> setChecked ( FParent -> getNormalDisplay ( ) ) ;
  FEFacesPleines -> setChecked ( FParent -> getFacesDisplay ( ) ) ;

  bgroup                 -> setEnabled ( FParent -> getFacesDisplay () );
  FECoulGlobale          -> setChecked ( FParent -> getCoulFacesGlobale() );
  FECouleursAleatoire    -> setChecked ( FParent -> getFacesCoulAlea() );
  FECoulVolumesAleatoire -> setChecked ( FParent -> getVolumesCoulAlea() );

  bgroup2         -> setEnabled ( FParent -> getFacesDisplay () &&
				  !FParent -> getCoulFacesGlobale() );

  FECoulTopologie -> setChecked (
#ifdef MODULE_ARCHITECTURE
				 (!(FParent -> getCoulSemantique()))&&
#endif
				 (FParent -> getCoulTopologie()) );
  
  FECoulGeometry  -> setChecked (
#ifdef MODULE_ARCHITECTURE
				 (!(FParent -> getCoulSemantique()))&&
#endif
				 (FParent -> getCoulGeometry()) );
  
#ifdef MODULE_ARCHITECTURE
  FECoulSemantique -> setChecked ( FParent -> getCoulSemantique() );
#endif

  FGrille -> setChecked ( FParent -> getGridDisplay ( ) ) ;

  FSaisieTaille -> setEnabled ( FParent -> getGridDisplay ( ) ) ;

  FAffAxes -> setChecked ( FParent -> getAxisDisplay ( ) ) ;

  FSaisieTaille -> setValue( FParent -> getParameterGrid ( ) -> getLGGrid ( ) ) ;

  FPlanXY -> setChecked ( FParent -> getParameterGrid ( ) -> getDisplayGridXY ( ) ) ;
  FPlanXZ -> setChecked ( FParent -> getParameterGrid ( ) -> getDisplayGridXZ ( ) ) ;
  FPlanYZ -> setChecked ( FParent -> getParameterGrid ( ) -> getDisplayGridYZ ( ) ) ;

  FValOx -> setChecked ( FParent -> getParameterGrid ( ) -> getDisplayPositiveGridX ( ) ) ;
  FValOy -> setChecked ( FParent -> getParameterGrid ( ) -> getDisplayPositiveGridY ( ) ) ;
  FValOz -> setChecked ( FParent -> getParameterGrid ( ) -> getDisplayPositiveGridZ ( ) ) ;
  
  CParameterOperations * p = FParent -> getControler ( ) -> getParameterOperations ( ) ;
  
  setOrientation ( p -> getRotateCells ( ) ) ;
  setScale ( p -> getScaleCells ( ) ) ;
  setPosition ( p -> getTranslateCells ( ) ) ;

  FParent -> repaint ( ) ;

  FUpdate = false ;
  FMain = false ;
}


// slots
//------

void OptionsAffichage :: callbackToggleNormal ( )
{
  if ( ! FUpdate )
    {
      setNormalDisplay ( getNormalDisplay ( ) ) ;
      update() ;
    }
}

void OptionsAffichage :: callbackToggleVertices ( )
{
  if ( ! FUpdate ) {
    setVerticesDisplay ( getVerticesDisplay ( ) ) ;
    update() ;
  }
}

void OptionsAffichage :: callbackToggleSews ( )
{
  if ( ! FUpdate ) {
    setSewsDisplay ( getSewsDisplay ( ) ) ;
    update() ;
  }
}

void OptionsAffichage :: callbackToggleFaces ( )
{
  if ( ! FUpdate ) {
    setFacesDisplay ( getFacesDisplay ( ) ) ;
    update() ;
  }
}

void OptionsAffichage :: callbackCoulFacesGlobale ( )
{
  if ( ! FUpdate )
    {
      setCoulFacesGlobale();
      update() ;
    }
}

void OptionsAffichage ::callbackFacesCoulAlea( )
{
  if ( ! FUpdate )
    {
      setFacesCoulAlea();
      update ( ) ;     
    }
}

void OptionsAffichage ::callbackVolumesCoulAlea( )
{
  if ( ! FUpdate )
    {
      setVolumesCoulAlea();
      update ( ) ;     
    }
}

void  OptionsAffichage :: callbackCoulTopologie( )
{
  if ( ! FUpdate )
    {
      setCoulTopologie();
      update ( ) ;     
    }
}


#ifdef MODULE_ARCHITECTURE

void  OptionsAffichage :: callbackCoulSemantique( )
{
  if ( ! FUpdate )
    {
      setCoulSemantique();
      update ( ) ;
    }
}

void OptionsAffichage :: setCoulSemantique()
{
  FParent -> setCoulSemantique () ;
  update ( ) ;
}

bool OptionsAffichage :: getCoulSemantique( ) const
{
  if ( ! FMain )
    return FECoulSemantique -> isChecked ( );
  else
    return  ! (FECoulSemantique -> isChecked ( ) ) ;
}

#endif

void  OptionsAffichage :: callbackCoulGeometry()
{
  if ( ! FUpdate )
    {
      setCoulGeometry();
      update ( ) ;     
    }
}

// Slots pour grille
void OptionsAffichage :: callbackToggleGrid ( )
{
  if ( ! FUpdate )
    {
      setGridDisplay ( getGridDisplay ( ) ) ;
      update ( ) ;
    }
}

void OptionsAffichage :: callbackToggleAxis ( )
{
  if ( ! FUpdate )
    {
      setAxisDisplay ( getAxisDisplay ( ) ) ;
      update ( ) ;
    }
}

void OptionsAffichage :: callbackSize ( )
{
  if ( ! FUpdate )
    {
      setSize ( getSize ( ) ) ;
      update ( ) ;
    }
}

void OptionsAffichage :: callbackToggleXy ( )
{
  if ( ! FUpdate )
    {
      setXy ( getXy ( ) ) ;
      update ( ) ;
    }
}

void OptionsAffichage :: callbackToggleXz ( )
{
  if ( ! FUpdate )
    {
      setXz ( getXz ( ) ) ;
      update ( ) ;
    }
}

void OptionsAffichage :: callbackToggleYz ( )
{
  if ( ! FUpdate )
    {
      setYz ( getYz ( ) ) ;
      update ( ) ;
    }
}

void OptionsAffichage :: callbackTogglePx ( )
{
  if ( ! FUpdate )
    {
      setPx ( getPx ( ) ) ;
      update ( ) ;
    }
}

void OptionsAffichage :: callbackTogglePy ( )
{
  if ( ! FUpdate )
    {
      setPy ( getPy ( ) ) ;
      update ( ) ;
    }
}

void OptionsAffichage :: callbackTogglePz ( )
{
  if ( ! FUpdate )
    {
      setPz ( getPz ( ) ) ;
      update ( ) ;
    }
}

// Slots pour placage

void OptionsAffichage :: callbackToggleOrientation ( ) 
{
  if ( ! FUpdate ) {
    setOrientation ( FOrientation -> isChecked ( ) ) ; 
    update ( ) ;
  }
}

void OptionsAffichage :: callbackToggleScale ( ) 
{ 
  if ( ! FUpdate ) {
    setScale ( FTaille -> isChecked ( ) ) ; 
    update ( ) ;
  }

}

void OptionsAffichage :: callbackTogglePosition ( ) 
{
  if ( ! FUpdate ) {
    setPosition ( FPosition -> isChecked ( ) ) ; 
    update ( ) ;
  }
}


// Accesseurs en lecture
//----------------------

bool  OptionsAffichage :: getVerticesDisplay ( ) const
{
  if ( ! FMain )
    return FESommets -> isChecked ( ) ;
  else
    return ! (FESommets -> isChecked ( ) ) ;
}


bool  OptionsAffichage :: getSewsDisplay ( ) const
{
  if ( ! FMain )
    return FECoutures -> isChecked ( ) ;
  else
    return ! ( FECoutures -> isChecked ( ) );
}

bool  OptionsAffichage :: getNormalDisplay  ( ) const
{
  if ( ! FMain )
    return FENormale -> isChecked ( ) ;
  else
    return ! ( FENormale -> isChecked ( ) ) ;
}

bool  OptionsAffichage :: getFacesDisplay ( ) const
{
  if ( ! FMain )
    return FEFacesPleines -> isChecked ( ) ;
  else
    return ! ( FEFacesPleines -> isChecked ( ) ) ;
}

bool OptionsAffichage :: getFacesCoulAlea ( ) const
{
 if ( ! FMain )
    return FECouleursAleatoire -> isChecked ( );
  else
    return  ! (FECouleursAleatoire -> isChecked ( ) ) ;
}

bool OptionsAffichage :: getVolumesCoulAlea ( ) const
{
  if ( ! FMain )
    return FECoulVolumesAleatoire -> isChecked ( );
  else
    return  ! (FECoulVolumesAleatoire -> isChecked ( ) ) ;
}
bool OptionsAffichage :: getCoulFacesGlobale() const
{
  if ( ! FMain )
    return FECoulGlobale  -> isChecked ( );
  else
    return  ! (FECoulGlobale  -> isChecked ( ) ) ;

}
bool OptionsAffichage :: getCoulTopologie( ) const
{
  if ( ! FMain )
    return FECoulTopologie -> isChecked ( );
  else
    return  ! (FECoulTopologie -> isChecked ( ) ) ;
}


bool  OptionsAffichage :: getCoulGeometry( ) const
{
  if ( ! FMain )
    return FECoulGeometry -> isChecked ( );
  else
    return  ! (FECoulGeometry -> isChecked ( ) ) ;
}

// Grille
bool OptionsAffichage :: getGridDisplay ( ) const
{
  if ( ! FMain )
    return FGrille -> isChecked ( );
  else
    return  ! (FGrille -> isChecked ( ) ) ;
}

bool OptionsAffichage :: getAxisDisplay ( ) const
{
  if ( ! FMain )
    return FAffAxes -> isChecked ( );
  else
    return  ! ( FAffAxes -> isChecked ( ) ) ;
}

int  OptionsAffichage :: getSize ( ) const
{
  return FSaisieTaille -> value ( ) ;
}

bool OptionsAffichage :: getXy ( ) const
{
  return FPlanXY -> isChecked ( );
}

bool OptionsAffichage :: getXz ( ) const
{
  return FPlanXZ -> isChecked ( );
}

bool OptionsAffichage :: getYz ( ) const
{
  return FPlanYZ -> isChecked ( );
}

bool OptionsAffichage :: getPx ( ) const
{
  return FValOx -> isChecked ( );
}

bool OptionsAffichage :: getPy ( ) const
{
  return FValOy -> isChecked ( );
}

bool OptionsAffichage :: getPz ( ) const
{
  return FValOz -> isChecked ( );
}

// Accesseurs en ecriture
//-----------------------
void OptionsAffichage :: setNormalDisplay ( bool b )
{
  FParent -> setNormalDisplay ( b ) ;
  update ( ) ;
}

void OptionsAffichage :: setFacesDisplay ( bool b )
{
  FParent -> setFacesDisplay ( b ) ;
  update ( ) ;
}

void OptionsAffichage :: setCoulFacesGlobale ()
{
  FParent->setCoulFacesGlobale();
  update();
}

void OptionsAffichage :: setFacesCoulAlea ()
{
  FParent->setFacesCoulAlea();
  update();
}

void OptionsAffichage :: setVolumesCoulAlea ()
{
   FParent->setVolumesCoulAlea() ;
   update();
}

void OptionsAffichage :: setCoulTopologie()
 {
   FParent -> setCoulTopologie () ;
   update ( ) ;
}


void OptionsAffichage :: setCoulGeometry()
 {
   FParent -> setCoulGeometry () ;
   
   update ( ) ;
}

void OptionsAffichage :: setVerticesDisplay ( bool b )
{
  FParent -> setVerticesDisplay ( b ) ;
  update ( ) ;
}

void OptionsAffichage :: setSewsDisplay ( bool b )
{
  FParent -> setSewsDisplay ( b ) ;
  update ( ) ;
}


void OptionsAffichage :: setOrientation ( bool b )
{
  FOrientation -> setChecked ( b ) ;
  FParent -> getControler ( ) ->getParameterOperations ( ) -> setRotateCells ( b ) ;
}

void OptionsAffichage :: setScale ( bool b )
{
  FTaille -> setChecked ( b ) ;
  FParent -> getControler ( ) -> getParameterOperations ( ) -> setScaleCells ( b ) ;
}

void OptionsAffichage :: setPosition ( bool b )
{
  FPosition -> setChecked ( b ) ;
  FParent -> getControler ( ) -> getParameterOperations ( ) -> setTranslateCells ( b ) ;
}



void OptionsAffichage :: applyValues ( )
{
  FParent -> setVerticesDisplay ( getVerticesDisplay ( ) ) ;
  FParent -> setSewsDisplay ( getSewsDisplay ( ) ) ;
  FParent -> setNormalDisplay ( getNormalDisplay ( ) ) ;
  FParent -> setFacesDisplay ( getFacesDisplay ( ) ) ;
  FParent -> repaint ( ) ;
}


void OptionsAffichage :: setFMain ( )
{
  FMain = true ;
}

// Grille
void OptionsAffichage :: setGridDisplay ( bool b )
{
  FGrille -> setChecked ( b ) ;
  FSaisieTaille -> setEnabled ( b ) ;
  FPlanXY -> setEnabled ( b ) ;
  FValOx -> setEnabled ( b ) ;
  FPlanXZ -> setEnabled ( b ) ; 
  FValOy -> setEnabled ( b ) ;
  FPlanYZ -> setEnabled ( b ) ;
  FValOz -> setEnabled ( b ) ;
  FParent -> setGridDisplay ( b ) ;
}

void OptionsAffichage :: setAxisDisplay ( bool b )
{
  FAffAxes -> setChecked ( b ) ;
  FParent -> setAxisDisplay ( b ) ;  
}

void OptionsAffichage :: setSize ( int s )
{
  assert ( s >= 0 ) ;

  FSaisieTaille -> setValue ( s ) ;
  FParent -> getParameterGrid ( ) -> setLGGrid ( s ) ;
}

void OptionsAffichage :: setXy ( bool b )
{
  FPlanXY -> setChecked ( b ) ;
  FParent -> getParameterGrid ( ) -> setDisplayGridXY ( b ) ;
}

void OptionsAffichage :: setXz ( bool b )
{
  FPlanXZ -> setChecked ( b ) ;
  FParent -> getParameterGrid ( ) -> setDisplayGridXZ ( b ) ;
}

void OptionsAffichage :: setYz ( bool b )
{
  FPlanYZ -> setChecked ( b ) ;
  FParent -> getParameterGrid ( ) -> setDisplayGridYZ ( b ) ;
}

void OptionsAffichage :: setPx ( bool b )
{
  FValOx -> setChecked ( b ) ;
  FParent -> getParameterGrid ( ) -> setDisplayPositiveGridX ( b ) ;
}

void OptionsAffichage :: setPy ( bool b )
{
  FValOy -> setChecked ( b ) ;
  FParent -> getParameterGrid ( ) -> setDisplayPositiveGridY ( b ) ;
}

void OptionsAffichage :: setPz ( bool b )
{
  FValOz -> setChecked ( b ) ;
  FParent -> getParameterGrid ( ) -> setDisplayPositiveGridZ ( b ) ;
}
