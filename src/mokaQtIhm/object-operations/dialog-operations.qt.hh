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

#ifndef CHAMPSOPERATIONS_QT_HH
#define CHAMPSOPERATIONS_QT_HH

//--------- Include autres classes developees ----------
#include "floatSpinBox.qt.hh"
#include "controler-gmap.hh"
#include "dialog-types.qt.hh"

//--------------------- Include QT --------------------
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QToolBar>
#include <QtGui/QBoxLayout>


/*******************************************************************/
/* CLASSE champsOperations                                         */
/*******************************************************************/

/**
 * Cette classe implemente 3 QSpinBox ainsi qu'un bouton Recuperer
 */
class champsOperations : public QWidget
{
  Q_OBJECT

public:

  /**
   * Construit 3 champs de saisie X , Y , Z ainsi que le bouton Recuperer
   * @param parent Le Widget parent
   * @param Layout Si NULL, le widget est ajoute a la toolbar parent,
                   sinon il est ajoute au Layout.
   */
  champsOperations ( QToolBar * parent , QBoxLayout * Layout = NULL,
		     bool WithRecuperer = true) ;

  /**
   * Destructeur
   */
  ~champsOperations ( ) ;
  

  // Accesseurs en lecture

  /**
   * Recupere le bouton Recuperer. Cette methode est utile pour poser
   * un SLOT sur le bouton ainsi que pour placer le bouton dans un layout
   * par exemple.
   * @return le pointeur sur le bouton Recuperer
   */
  virtual QPushButton * getButtonRecuperer ( ) ;

  /**
   * @return Le QSpinBox X
   */
  virtual FloatSpinBox * getSaisieX ( ) ;

  /**
   * @return Le QSpinBox Y
   */
  virtual FloatSpinBox * getSaisieY ( ) ;

  /**
   * @return Le QSpinBox Z
   */
  virtual FloatSpinBox * getSaisieZ ( ) ;

  
  /**
   * @return Le QLabel X
   */
  virtual QLabel * getLabelX ( ) ;

  /**
   * @return Le QLabel Y
   */
  virtual QLabel * getLabelY ( ) ;

  /**
   * @return Le Qlabel Z
   */
  virtual QLabel * getLabelZ ( ) ;

  /**
   * @return La valeur se trouvant dans le QSpinBox X
   */
  virtual float getValueX ( ) const ;

  /**
   * @return La valeur se trouvant dans le QSpinBox Y
   */
  virtual float getValueY ( ) const ;

  /**
   * @return La valeur se trouvant dans le QSpinBox Z
   */
  virtual float getValueZ ( ) const ;


  // Accesseurs en ecriture
  /**
   * Ecrit la valeur X dans le QSpinBox X
   */
  virtual void setValueX ( float X ) ;

  /**
   * Ecrit la valeur Y dans le QSpinBox Y
   */
  virtual void setValueY ( float Y ) ;

  /**
   * Ecrit la valeur Z dans le QSpinBox Z
   */
  virtual void setValueZ ( float Z ) ;

private:
  
  // Bouton
  QPushButton * FRecuperer ;
  // Champs de saisies
  FloatSpinBox * FSaisieX , * FSaisieY , * FSaisieZ ;
  QLabel * FAffX ,  * FAffY , * FAffZ ;
  
  // Pointeur sur le widget parent
  QToolBar * FParent ;

} ;
#endif

/*******************************************************************/
/* CLASSE dialogOperations                                         */
/*******************************************************************/

#ifndef DIALOG_OPERATIONS_QT_HH
#define DIALOG_OPERATIONS_QT_HH

class Window ;

/**
 * Cette classe represente une barre d'outil contenant un champsOperation
 * ainsi qu'un bouton de fermeture
 */
class dialogOperations : public QToolBar
{
 Q_OBJECT

public:

 /**
  * Fabrique le widget
  * @param parent Widget parent
  * @param nom Nom du toolbar
  * @param controler Pointeur sur le controleur
  */
 dialogOperations ( Window * parent , QString * nom ,
		    GMap3d :: CControlerGMap * controler ,
		    Qt :: ToolBarArea dir ) ;
  

  /**
   * Destructeur
   */
  ~dialogOperations ( ) ;
 
  
  /**
   * Accesseur sur le controleur
   * @return le pointeur sur le controleur
   */
  GMap3d :: CControlerGMap * getControler ( ) const ;
  
  /**
   * Methode qui demande a la fenetre principale de tout redessiner
   */
  virtual void repaint ( ) ;
  
  
  /**
   * Methode qui met a jour les valeurs affichees
   */
  virtual void update ( ) ;
  
  
public slots:

/**
 * Definit l'action effectue lorsque l'on clique sur le bouton Fermer
 */
virtual void callbackClose ( ) ;

protected: 
  
  // Champs de saisie
  champsOperations * FSaisie ;

  // Bouton fermer
  QPushButton * FClose ;
  
  // Pointeur sur le controleur
  GMap3d :: CControlerGMap * FControler ;

  // Pointeur sur la fenetre principale
  Window * FParent ;

  // Booleen indiquant si le update vient de la souris ou du clavier
  bool FUpdate ;
  
} ;

#endif
