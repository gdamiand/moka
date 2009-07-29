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

#ifndef CREATION_CYLINDRE_QT_HH
#define CREATION_CYLINDRE_QT_HH

//--------- Include autres classes developees ----------
class Window ;

//--------------- Include Controler --------------------
#include "creation-objet.qt.hh"

//--------------------- Include QT --------------------
#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include <QtGui/QCheckBox>

/*******************************************************************/
/* CLASSE creationCylindre                                         */
/*******************************************************************/
class CreationCylindre : public CreationObjet
{
  Q_OBJECT

public:
  /**
   * Fabrique le widget
   * @param parent Widget parent
   * @param controler Pointeur sur le controleur
   */
  CreationCylindre ( Window * parent ,
		     GMap3d :: CControlerGMap * controler ) ;

  /**
   * Destructeur
   */
  ~CreationCylindre ( ) ;

  /**
   * Mise a jour des valeurs
   */
  void update ( ) ;

  


private slots:

  void callbackMeridians ( ) ;
  void callbackParallels ( ) ;
  void callbackCheckedButton1();
  void callbackCheckedButton2();


private:

  // Accesseurs de modification
  void setMeridians ( int nb ) ;
  void setParallels ( int nb ) ;
  void setClosedBorder1(bool); 
  void setClosedBorder2(bool); 

  // Affichages 
  QLabel * FAffSub ;
  QLabel * FAffPar ;
  QLabel * FAffMer ;
  QLabel * FAffFaces ;

  QSpinBox * FSaisiePar ;
  QSpinBox * FSaisieMer ;

  QCheckBox * FFace1 ;
  QCheckBox * FFace2 ;

} ;

#endif
