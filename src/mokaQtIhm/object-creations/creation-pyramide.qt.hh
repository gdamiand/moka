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

#ifndef CREATION_PYRAMIDE_QT_HH
#define CREATION_PYRAMIDE_QT_HH

//--------- Include autres classes developees ----------
#include "creation-objet.qt.hh"
class Window ;


//--------------------- Include QT --------------------
#include <QtGui/QCheckBox>

/*******************************************************************/
/* CLASSE CreationPyramide                                         */
/*******************************************************************/
class CreationPyramide : public CreationObjet
{
  Q_OBJECT

public:
  /**
   * Fabrique le widget
   * @param parent Widget parent
   * @param controler Pointeur sur le controleur
   */
  CreationPyramide ( Window * parent ,
		     GMap3d :: CControlerGMap * controler ) ;
  
  /**
   * Destructeur
   */
  ~CreationPyramide ( ) ;

  /**
    * Mise a jour des valeurs
    */
  void update ( ) ;

  // Accesseurs en lecture
  int  getMeridians  () const;
  int  getParallels  () const;
  bool getCreatedBase() const; 

  // Accesseurs en ecriture
  void setMeridians  (int ); 
  void setParallels  (int );
  void setCreatedBase(bool); 


private slots:
  
  void callbackMeridians    ();
  void callbackParallels    ();
  void callbackCheckedButton();
  

private:
  QLabel * FAffSub ;
  QLabel * FAffPar ;
  QLabel * FAffMer ;
  QLabel * FAffFaces ;

  QSpinBox * FSaisiePar ;
  QSpinBox * FSaisieMer ;

  QCheckBox * FBase ;

} ;

#endif

