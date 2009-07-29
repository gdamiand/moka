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

#ifndef DIALOGHOMOTHETIE_QT_HH
#define DIALOGHOMOTHETIE_QT_HH

//--------- Include autres classes developees ----------
#include "dialog-operations.qt.hh"

//--------------------- Include QT --------------------
#include <QtGui/QComboBox>
#include <QtGui/QLabel>

class Window ;

/**
 * Implemente la barre d'outil pour l'operation "homothetie"
 */
class dialogHomothetie : public dialogOperations
{
  Q_OBJECT  

public:  
  
  /**
   * Construit la barre d'outil pour l'operation de homothetie
   * @param parent Le Widget parent
   * @param controler Le controleur
   */
  dialogHomothetie ( Window * parent ,
		     GMap3d :: CControlerGMap * controler ) ;

  /**
   * Destructeur
   */
  ~dialogHomothetie ( ) ;
 
  /**
   * Methode de mise a jour de l'affichage
   */
  virtual void update ( ) ;


  // Accesseurs
  //-----------
  void setDistanceType ( GMap3d :: TScaleType ) ;
  void setIso ( bool ) ;

  void setC ( const CVertex & /*center*/ ) ;
  void setV ( const CVertex & /*vector*/ ) ;
  void setK ( const CVertex & /*coefs */ ) ;

  void setCx ( TCoordinate ) ; TCoordinate getCx ( ) const ;
  void setCy ( TCoordinate ) ; TCoordinate getCy ( ) const ;
  void setCz ( TCoordinate ) ; TCoordinate getCz ( ) const ;

  void setVx ( TCoordinate ) ; TCoordinate getVx ( ) const ;
  void setVy ( TCoordinate ) ; TCoordinate getVy ( ) const ;
  void setVz ( TCoordinate ) ; TCoordinate getVz ( ) const ;

  void setKx ( TCoordinate ) ; TCoordinate getKx ( ) const ;
  void setKy ( TCoordinate ) ; TCoordinate getKy ( ) const ;
  void setKz ( TCoordinate ) ; TCoordinate getKz ( ) const ;

  void setIsoCoef ( TCoordinate ) ;

 public slots:
 
  void callbackCx ( ) ;
  void callbackCy ( ) ;
  void callbackCz ( ) ;
  
  void callbackVx ( ) ;
  void callbackVy ( ) ;
  void callbackVz ( ) ;

  void callbackKx ( ) ;
  void callbackKy ( ) ;
  void callbackKz ( ) ;

  void callbackIsoCoef ( ) ;

  void callbackDistance ( );
  void callbackToggleIsoButton ( ) ;
  
  void callbackTakeCenter ( ) ;
  void callbackTakeVector ( ) ;
  
  void callbackTogglePonderation ( ) ;  


private:

  QLabel * FAffParam , * FAffSommet , * FAffVecteur , * FAffCoef , * FAffCoeffs ;
  QLabel * FAffX , * FAffY , * FAffZ ;
  QComboBox * FSaisieParam ;
  champsOperations * FSaisieVecteur ;
  QCheckBox * FSaisieIso , * FSaisiePonderation ;
  FloatSpinBox * FSaisieCoef , * FSaisieX , * FSaisieY , * FSaisieZ ;
  

} ;

#endif
