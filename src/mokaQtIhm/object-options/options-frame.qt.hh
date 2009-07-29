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

#ifndef OPTIONSFRAME_QT_HH
#define OPTIONSFRAME_QT_HH

//--------- Include autres classes developees ----------
#include "controler-gmap.hh"
#include "options-vue.qt.hh"
#include "options-ponderation.qt.hh"
#include "options-extrusion.qt.hh"
#include "options-interpolation.qt.hh"
#include "options-affichage.qt.hh"
#include "options-divers.qt.hh"


//--------------------- Include QT --------------------
#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>



class Window ;

/**
* Implemente la boite de dialogue contenant toutes les options
*/
class OptionsFrame : public QDialog
{
  Q_OBJECT

public :

  /**
   * Constructeur
   * @param parent La fenetre principale
   */
  OptionsFrame ( Window * parent ) ;

  /**
   * Destructeur
   */
  ~OptionsFrame ( ) ;

  void update ( ) ;

  /**
   * Permet de changer la fenetre activee
   */
  void setOption ( int indice ) ;
  
  
  /**
   * Accesseur sur la methode apply values de FAffichage
   */
  void applyValues() ;

  void setFVisible ( bool b ) ;
  
  OptionsAffichage * getOptionsAffichage() const;
  
  OptionsVue * getOptionsVue() const;

public slots:

  void callbackShowVue ( ) ;
  void callbackShowCouture ( ) ;
  void callbackShowPonderation ( ) ;
  void callbackShowExtrusion ( ) ;
  void callbackShowInterpolation ( ) ;
  void callbackShowAffichage ( ) ;
  void callbackShowDivers ( ) ;
  // Pour afficher les coutures
  void callbackToggleSews ( ) ;
  // Pour afficher les normales
  void callbackToggleNormal     ();
  // Pour afficher les sommets
  void callbackToggleVertices   ();
  // Pour afficher les faces
  void callbackToggleFaces      ();

  void callbackTournerButton( );

  void callbackToggleGrille ( ) ;

  void callbackButtonOkPressed ( ) ;
  
  void updateTab(int num_tab);

private :

  bool FVisible ;
  
  QPushButton * valid_btn;
  QTabWidget * main_tab;

  // Les differentes options presentes

  OptionsPonderation * FPonderation ;
  OptionsInterpolation * FInterpolation ;
  OptionsExtrusion * FExtrusion ;
  OptionsVue * FVue ;
  OptionsDivers * FDivers ;
  OptionsAffichage * FAffichage ;
} ;

#endif
