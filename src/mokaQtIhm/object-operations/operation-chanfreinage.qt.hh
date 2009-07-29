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

#ifndef OPERATIONCHANFREINAGE_QT_HH
#define OPERATIONCHANFREINAGE_QT_HH

//--------- Include autres classes developees ----------
#include "controler-gmap.hh"
#include "floatSpinBox.qt.hh"

//--------------------- Include QT --------------------
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QDialog>


class Window ;

/**
 * Cette classe correspond a la boite de dialogue pour l'operation "Chanfreinage"
 */
class OperationChanfreinage : public QDialog
{
  Q_OBJECT

public :

  /**
   * Constructeur
   * Construit la boite de dialogue "chanfreinage".
   * @param parent La fenetre principale
   */
  OperationChanfreinage ( Window * parent ) ;
	
  /**
   * Destructeur
   * Detruit la boite de dialogue "chanfreinage".
   */		
  ~OperationChanfreinage ( ) ;

  /**
   * Met a jour l'affichage de la boite.
   */
  virtual void update ( ) ;

  void setRoundingCoef ( int , float ) ;

  void getCoef(int ADimension);
  void setCoef(int ADimension);
  void nextCoef(int ADimension);

public slots :
  void callbackRounding0Coef();
  void callbackRounding1Coef();

  void callbackGet0();
  void callbackGet1();


  void callbackSet0();
  void callbackSet1();


  void callbackNext0();
  void callbackNext1();
 
private :

  // Pointeur sur la fenetre principale
  Window * FParent ;

  // Indique si la table est en train d'etre reaffichee
  bool FUpdate ;

  QLabel * FAffSommet , * FAffArete ;
  FloatSpinBox * FSaisie [2] ;
  QPushButton * FGetSommet , * FGetArete, 
    * FSetSommet , * FSetArete ,
    * FNextSommet , * FNextArete ;
  QPushButton * FClose ;
} ;


#endif
