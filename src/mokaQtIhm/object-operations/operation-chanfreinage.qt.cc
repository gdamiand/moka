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

#include "operation-chanfreinage.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <Qt3Support/Q3Accel>


// Constructeur
//-------------
OperationChanfreinage :: OperationChanfreinage ( Window * parent ) 
  :
  QDialog ( parent ) ,
  FParent ( parent ) ,
  FUpdate ( false )
{
  // Modifie le titre de la boite de dialogue
  setWindowTitle ( "chamfering" ) ;

  // Definition des raccourcis claviers

  Q3Accel * Raccourci = new Q3Accel ( this ) ;
  Raccourci -> insertItem ( QKeySequence ( Qt :: Key_Space ) , 9 ) ;
  Raccourci -> connectItem ( 9 , FParent ,
			     SLOT ( callbackHideAllWindow ( ) ) )  ;


  QBoxLayout * placement = new QBoxLayout ( QBoxLayout::TopToBottom, this ) ; 
  
  /** Partie haute **/
  QWidget * WidgetHaut = new QWidget ( this ) ;
  QBoxLayout * haut = new QBoxLayout ( QBoxLayout::LeftToRight, WidgetHaut );
  
  FAffSommet = new QLabel ( "Weights of vertices" , WidgetHaut ) ;
 
  FSaisie[0] = new FloatSpinBox ( 0.1 ) ;
  
  FGetSommet = new QPushButton ( "Get" , WidgetHaut ) ;
  FSetSommet = new QPushButton ( "Set" , WidgetHaut ) ;
  FNextSommet = new QPushButton ( ">>" , WidgetHaut ) ;

  haut -> addWidget ( FAffSommet ) ;
  haut -> addWidget ( FSaisie[0] ) ;
  haut -> addWidget ( FGetSommet ) ;
  haut -> addWidget ( FSetSommet ) ;
  haut -> addWidget ( FNextSommet ) ;

  /** Partie basse **/
  QWidget * WidgetBas = new QWidget ( this ) ;
  QBoxLayout * bas = new QBoxLayout ( QBoxLayout::LeftToRight, WidgetBas ) ;

  FAffArete = new QLabel ( *HTML::decode("Weights of edges") ,
                            WidgetBas ) ;
  FSaisie[1] = new FloatSpinBox ( 0.1 ) ;
  FGetArete = new QPushButton ( "Get" , WidgetBas ) ;
  FSetArete = new QPushButton ( "Set" , WidgetBas) ;
  FNextArete = new QPushButton ( " >>" , WidgetBas ) ;

  bas -> addWidget ( FAffArete ) ;
  bas -> addWidget ( FSaisie[1] ) ;
  bas -> addWidget ( FGetArete ) ;
  bas -> addWidget ( FSetArete ) ;
  bas -> addWidget ( FNextArete ) ;

  FClose = new QPushButton ( "Close" , this ) ;
  QObject :: connect ( FClose ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( close ( ) ) ) ;
  FClose -> setFocus ( ) ;
  placement -> addWidget ( WidgetHaut ) ;
  placement -> addWidget ( WidgetBas ) ;
  placement -> addWidget ( FClose ) ;

  QObject :: connect ( FSaisie[0] ,
		       SIGNAL ( valueChanged ( double ) ) ,
		       this ,
		       SLOT ( callbackRounding0Coef ( ) ) ) ;

  QObject :: connect ( FGetSommet ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackGet0 ( ) ) ) ;
  QObject :: connect ( FSetSommet ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackSet0 ( ) ) ) ;

  QObject :: connect ( FNextSommet ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackNext0 ( ) ) ) ;


  QObject :: connect ( FSaisie[1] ,
		       SIGNAL ( valueChanged ( double ) ) ,
		       this ,
		       SLOT ( callbackRounding1Coef ( ) ) ) ;

  QObject :: connect ( FGetArete ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackGet1 ( ) ) ) ;
  QObject :: connect ( FSetArete ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackSet1 ( ) ) ) ;
  QObject :: connect ( FNextArete ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( callbackNext1 ( ) )  ) ;

  update ( ) ;

}

// Destructeur
//------------
OperationChanfreinage :: ~OperationChanfreinage ( ) 
{
}

// Mise a jour de l'affichage
//---------------------------
void OperationChanfreinage::update()
{
  CParameterOperations * p = FParent -> getControler ( )
    -> getParameterOperations();

  FSaisie[0] -> setValuef ( p -> getDefaultRoundingCoefForVertices() );
  FSaisie[1] -> setValuef ( p -> getDefaultRoundingCoefForEdges   () );
}


// SLOTS
//------
void OperationChanfreinage :: callbackRounding0Coef ( )
{
  if (!FUpdate)
    setRoundingCoef(0, FSaisie[0]->valuef());
}

void OperationChanfreinage :: callbackRounding1Coef ( )
{
  if (!FUpdate)
    setRoundingCoef(1, FSaisie[1]->valuef());
}

void OperationChanfreinage :: callbackGet0  ( )
{
  getCoef ( 0 ) ; 
}

void OperationChanfreinage :: callbackGet1 ( )
{
  getCoef ( 1 ) ;
}

void OperationChanfreinage :: callbackSet0 ( )
{
  setCoef ( 0 ) ;
}

void OperationChanfreinage :: callbackSet1 ( )
{
  setCoef ( 1 ) ; 
}

void OperationChanfreinage :: callbackNext0 ( )
{
  nextCoef ( 0 ) ; 
}

void OperationChanfreinage :: callbackNext1 ( )
{
  nextCoef ( 1 ) ;
}


// Accesseurs
//-----------
void OperationChanfreinage :: getCoef ( int ADimension )
{
#ifdef MODULE_ROUNDING
  TCoordinate value;
  bool same = FParent -> 
    getControler ( ) -> 
    modeRoundingGetCoefOfMarkedDarts ( ADimension , value ) ;

  if (same)
    FSaisie [ ADimension ] -> setValuef ( value ) ;

#endif // MODULE_ROUNDING
}

void OperationChanfreinage :: setCoef ( int ADimension )
{
#ifdef MODULE_ROUNDING
  FParent -> getControler ( )
    ->modeRoundingSetCoefOfMarkedDarts ( ADimension,
				       FSaisie [ ADimension ] -> valuef ( ) ) ;
#endif // MODULE_ROUNDING
}

void OperationChanfreinage :: nextCoef ( int ADimension )
{
#ifdef MODULE_ROUNDING
  float result = FParent -> 
    getControler ( ) -> 
    modeRoundingSelectNextCoef ( ADimension ) ;
  FSaisie [ ADimension ] -> setValuef ( result ) ;
  FParent -> repaint ( ) ;
#endif // MODULE_ROUNDING
}

// Accesseur en ecriture
//----------------------

void OperationChanfreinage :: setRoundingCoef ( int i , float k )
{
  CParameterOperations * p = FParent -> 
    getControler ( ) -> getParameterOperations ( ) ;

  switch ( i )
    {
    case 0 :
      p -> setDefaultRoundingCoefForVertices ( k ) ;
      break ;

    case 1 :
      p -> setDefaultRoundingCoefForEdges ( k ) ;
      break ;
      
    default :
      assert ( false ) ;
    }

  update ( ) ;
}
