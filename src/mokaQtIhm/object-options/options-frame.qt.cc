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

#include "options-frame.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <Qt3Support/Q3Accel>
#include <QtGui/QVBoxLayout>


// Constructeur
//-------------
OptionsFrame :: OptionsFrame ( Window * parent ) 
  : QDialog ( parent ) ,
    FVisible ( false )
{
  valid_btn = new QPushButton("OK");
  valid_btn->setMaximumSize(75,50);
  connect(valid_btn, SIGNAL(clicked()),
          this, SLOT(callbackButtonOkPressed()));
  
  main_tab = new QTabWidget(this);  
    
  FAffichage = new OptionsAffichage ( parent , main_tab ) ;
  main_tab -> addTab ( FAffichage , "&Display") ;
  
  FVue = new OptionsVue ( parent , this ) ;  
  main_tab -> addTab ( FVue , "&Splitted View" ) ;

  FPonderation = new OptionsPonderation ( parent , main_tab) ;
  main_tab -> addTab ( FPonderation , "Weights of &deformations") ;

  FExtrusion = new OptionsExtrusion ( parent , main_tab ) ;
  main_tab -> addTab ( FExtrusion , "&Extrusions/thickening" ) ;

#ifdef MODULE_MESH
  FInterpolation = new OptionsInterpolation ( parent , main_tab ) ;
  main_tab -> addTab ( FInterpolation , "&Interpolations/Smoothing" ) ;
#endif //MODULE_MESH

  FDivers = new OptionsDivers ( parent , main_tab ) ;
  main_tab -> addTab ( FDivers , "&Misc" ) ;

  // Definition des raccourcis
  Q3Accel * Raccourci = new Q3Accel ( this ) ;

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F2 ) , 2 ) ;
  Raccourci -> connectItem ( 2 , this , SLOT ( callbackShowAffichage ( ) ) ) ;

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F3 ) , 3 ) ;
  Raccourci -> connectItem ( 3 , this , SLOT ( callbackShowVue ( ) ) ) ;

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F4 ) , 4 ) ;
  Raccourci -> connectItem ( 4 , this , SLOT ( callbackShowPonderation ( ) ) ) ;

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F5 ) , 5 ) ;
  Raccourci -> connectItem ( 5 , this , SLOT ( callbackShowExtrusion ( ) ) ) ;

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F6 ) , 6 ) ;
  Raccourci -> connectItem ( 6 , this , SLOT ( callbackShowInterpolation()));

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F7 ) , 7 ) ;
  Raccourci->connectItem(7, this, SLOT(callbackShowDivers()));
  
//  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F8 ) , 15 ) ;
//  Raccourci -> connectItem ( 15 , this , SLOT ( callbackShowDivers ( ) ) ) ;

  Raccourci -> insertItem ( QKeySequence ( "Alt+F9" ) , 8 ) ;
  Raccourci -> connectItem ( 8 , this , SLOT ( callbackToggleNormal ( ) ) ) ;

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F9 ) , 9 ) ;
  Raccourci -> connectItem ( 9 , this , SLOT ( callbackToggleSews ( ) ) ) ;

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F10 ) , 10 ) ;
  Raccourci -> connectItem ( 10 , this , SLOT ( callbackToggleVertices ( ) ) ) ;

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F11 ) , 11 ) ;
  Raccourci -> connectItem ( 11 , this , SLOT ( callbackToggleFaces ( ) ) ) ;

  Raccourci -> insertItem ( QKeySequence ( Qt::Key_F12 ) , 12 ) ;
  Raccourci -> connectItem ( 12 , this , SLOT ( callbackToggleGrille ( ) ) ) ;

  Raccourci->insertItem ( QKeySequence ( Qt :: Key_Space ) , 13 ) ;
  Raccourci->connectItem(13, parent, SLOT (callbackHideAllWindow()));

  Raccourci -> insertItem ( QKeySequence ( "Alt+F11" ) , 14 ) ;
  Raccourci -> connectItem ( 14 , this , SLOT (callbackTournerButton() ))  ;
  
  connect(main_tab,SIGNAL(currentChanged(int)),this,SLOT(updateTab(int)));
  
  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(main_tab);
  layout->addWidget(valid_btn);
  layout->setAlignment(valid_btn, Qt::AlignCenter);
  
  setLayout(layout);
  setWindowTitle ( "Options" ) ;
}


// Destructeur
//------------
OptionsFrame ::  ~OptionsFrame ( )
{
}

void OptionsFrame :: update ( ) {
   FAffichage -> update ( ) ;
}

void OptionsFrame :: setOption ( int indice )
{
  switch (indice) {
  case 2 : case 4 :
    if ( FVisible  &&  main_tab->currentWidget ( ) == FAffichage ) {
      FVisible = false ;
      close ( ) ;
      break ;
    }
    FVisible = true ;
    FAffichage -> update ( ) ;
    main_tab->setCurrentWidget ( FAffichage ) ; break ;

  case 3 :
    if ( FVisible  &&  main_tab->currentWidget ( ) == FVue ) {
      FVisible = false ;
      close ( ) ;
      break ;
    }
    FVisible = true ;
    FVue -> update ( ) ;
    main_tab->setCurrentWidget ( FVue ) ; break ;

  case 5 :
    if ( FVisible  &&  main_tab->currentWidget ( ) == FPonderation ){
      FVisible = false ;
      close ( ) ;
      break ;
    }
    FPonderation -> update ( ) ;
    FVisible = true ;
    main_tab->setCurrentWidget ( FPonderation ) ; break ;

  case 6 :
    if ( FVisible  &&  main_tab->currentWidget ( ) == FExtrusion ) {
      FVisible = false ;
      close ( ) ;
      break ;
    }
    FExtrusion -> update ( ) ;
    FVisible = true ;
    main_tab->setCurrentWidget ( FExtrusion ) ; break ;

  case 7 :
    if ( FVisible  &&  main_tab->currentWidget ( ) == FInterpolation ) {
      FVisible = false ;
      close ( ) ;
      break ;
    }
    FInterpolation -> update ( ) ;
    FVisible = true ;
    main_tab->setCurrentWidget ( FInterpolation ) ; break ;
    
  case 8 :
    if ( FVisible  &&  main_tab->currentWidget ( ) == FDivers ) {
      FVisible = false ;
      close ( ) ;
      break ;
    }
    FDivers -> update ( ) ;
    FVisible = true ;
    main_tab->setCurrentWidget ( FDivers ) ; break ;

  default : {}
  }
}

void OptionsFrame :: updateTab(int num_tab)
{
  QWidget * current = main_tab->currentWidget();
  
  if      (current == FAffichage) FAffichage -> update();
  else if (current == FPonderation) FPonderation -> update();
  else if (current == FInterpolation) FInterpolation -> update();
  else if (current == FExtrusion) FExtrusion -> update();
  else if (current == FVue) FVue -> update();
  else if (current == FDivers) FDivers -> update();
}

OptionsAffichage * OptionsFrame :: getOptionsAffichage() const
{
  return FAffichage;
}

OptionsVue * OptionsFrame :: getOptionsVue() const
{
  return FVue;
}

// SLOTS
//------
void OptionsFrame :: callbackShowVue ( )
{
  setOption ( 3 ) ;
}

void OptionsFrame :: callbackShowCouture ( )
{
  setOption ( 4 ) ;
}

void OptionsFrame :: callbackShowPonderation ( )
{
  setOption ( 5 ) ;
}

void OptionsFrame :: callbackShowExtrusion ( )
{
  setOption ( 6 ) ;
}

void OptionsFrame :: callbackShowInterpolation ( )
{
#ifdef MODULE_MESH
  setOption ( 7 ) ;
#endif //MODULE_MESH
}

void OptionsFrame :: callbackShowAffichage ( )
{
  setOption ( 2 ) ;
}

void OptionsFrame :: callbackShowDivers ()
{
  setOption ( 8 ) ;
}

void OptionsFrame :: applyValues()
{
  FAffichage -> applyValues ( ) ;
}

// Pour afficher les coutures
void OptionsFrame :: callbackToggleSews ( )
{
  FAffichage -> setFMain ( ) ;
  FAffichage -> callbackToggleSews ( ) ;
}

// Pour afficher les normales
void OptionsFrame :: callbackToggleNormal ( )
{
  FAffichage -> setFMain ( ) ;
  FAffichage -> callbackToggleNormal ( ) ;
}

// Pour afficher les sommets
void OptionsFrame :: callbackToggleVertices ( )
{
  FAffichage -> setFMain ( ) ;
  FAffichage -> callbackToggleVertices ( ) ;
}

// Pour afficher les faces
void OptionsFrame :: callbackToggleFaces ( )
{
  FAffichage -> setFMain ( ) ;
  FAffichage -> callbackToggleFaces ( ) ;
}

void  OptionsFrame :: callbackTournerButton( )
{
  if ( FAffichage ->getFacesDisplay ( ) )
  {
    if      (FAffichage->getCoulFacesGlobale()) FAffichage->setFacesCoulAlea();
    else if (FAffichage->getFacesCoulAlea())   
       FAffichage->setVolumesCoulAlea();
    else if (FAffichage->getVolumesCoulAlea()) 
       FAffichage->setCoulFacesGlobale();
    else { assert(false); }
  }
}

void OptionsFrame :: setFVisible ( bool b )
{
  FVisible = b ;
}

void OptionsFrame :: callbackToggleGrille ( )
{
  FAffichage -> setFMain ( ) ;
  FAffichage -> callbackToggleGrid ( ) ;
  FAffichage -> setFMain ( ) ;
  FAffichage -> callbackToggleAxis ( ) ;
}

void OptionsFrame :: callbackButtonOkPressed ( )
{
  FVisible = false ;
  done(QDialog::Accepted);
}
