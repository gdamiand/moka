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

#ifndef OPTIONSCOULEURS_QT_HH
#define OPTIONSCOULEURS_QT_HH


//--------- Include autres classes developees ----------
#include "controler-gmap.hh"
#include "floatSpinBox.qt.hh"

//--------------------- Include QT --------------------
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QToolBar>



class Window ;

typedef enum
{
  _FACE = 0,
  _FOND,
  _UNSEL_DART,
  _SEL_DART,
  _LAST_DART,
  _0DELETE_DART,
  _1DELETE_DART,
  _VERTEX,
  _0_SEW,
  _1_SEW,
  _2_SEW,
  _3_SEW,
  _GRID,
  _OX_AXIS,
  _OY_AXIS,
  _OZ_AXIS,
  _AIMED_POINT,
  _NORMAL_VECTOR,
  _OBJECT_TRANSFORMATION,
  _OBJECT_PREVIEW,
  _OBJECT_PREVIEW_BAD,
} TElement;

class OptionCouleurs : public QToolBar
{
  Q_OBJECT

public:

  /**
   * Construit l'objet
   * @param parent Pointeur sur la fenetre mere
   */
  OptionCouleurs ( Window * parent ) ;

  /**
   * Detruit l'objet
   */
  ~OptionCouleurs ( ) ;
  
public slots :

  void openDialog ( ) ;
  void close ( ) ;
  void show ( ) ;

  // Methode qui change la couleur du bouton dialogue
  // en fonction du chois dans le ComboBox
  void changeColor ( ) ;

  void callbackEpaisseur ( ) ;
  void callbackTransparence ( ) ;
  
private :

  // Accesseurs
  void setColor ( int num , float value ) ;
  void setR ( int r ) ;
  void setG ( int g ) ;
  void setB ( int b ) ;
  void colorButton ( ) ;

  // Methode de gisage/degrisage des boutons
  void griseBoutons ( ) ;

  // Pointeur sur la fenetre mere
  Window * FParent ;

  // ComboBox permettent de choisir le type d'objet dont
  // on veut modifier la couleur
  QComboBox * FModifiables ;

  // Bouton ouvrant la boite des couleurs
  QPushButton * FCouleur ;

  // Bouton pour quitter
  QPushButton * FFin ;

  // SpinBox Epaisseur
  QSpinBox * FEpaisseur ;

  // SpinBox Transparence
  FloatSpinBox * FTransparence ;

  // Booleen de modif
  bool FUpdate ;

} ;


#endif
