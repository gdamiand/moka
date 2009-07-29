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

#ifndef CREATION_MAILLAGE_QT_HH
#define CREATION_MAILLAGE_QT_HH

//--------- Include autres classes developees ----------
class Window ;

//--------------- Include Controler -------------------
#include "creation-objet.qt.hh"

//--------------------- Include QT --------------------
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>

/*******************************************************************/
/* CLASSE creationMaillage                                         */
/*******************************************************************/
class CreationMaillage : public CreationObjet
{
  Q_OBJECT

public:
  
  /**
   * Fabrique le widget
   * @param parent Widget parent
   * @param controler Pointeur sur le controleur
   */
  CreationMaillage ( Window * parent ,
		     GMap3d :: CControlerGMap * controler ) ;

  /**
   * Destructeur
   */
  ~CreationMaillage ( ) ;


  /**
   * Methode de mise a jour
   */
  virtual void update ( ) ;
 
  // Accesseurs

  void setSubdivisions ( int , int , int ) ;
  int getSubdivisions ( int ) const ;
  
  void setCreatedFaces ( uchar ) ;
  uchar getCreatedFaces ( ) const ;
  
  void setCellDimension(int);
  void setMeshDimension(int);
  
private slots:

  void callbackSubdivisions ( ) ;
  void callbackToggleButton ( ) ;
  
  void callbackDimCell ( ) ;
  void callbackDimMesh ( ) ; 

private:

  // SpinBox
  QSpinBox * FSaisieX ;
  QSpinBox * FSaisieY ;
  QSpinBox * FSaisieZ ;

  // Affichages
  QLabel * FAffCel ;
  QLabel * FAffMai ;
  QLabel * FAffX ;
  QLabel * FAffY ;
  QLabel * FAffZ ;
  QLabel * FAffSub ;
  QLabel * FAffFac ;

  //ComboBox
  QComboBox * FDimCell ;
  QComboBox * FDimMail ;

  // Boutons
  QCheckBox * FGauche ;
  QCheckBox * FDroite ;
  QCheckBox * FDevant ;
  QCheckBox * FDerriere ;
  QCheckBox * FBas ;
  QCheckBox * FHaut ;

  // Booleen de modification
  bool FModifie ;

} ;

#endif
