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

#ifndef DIALOGROTATION_QT_HH
#define DIALOGROTATION_QT_HH

#include "dialog-operations.qt.hh"

class Window ;

/**
 * Implemente la barre d'outil pour l'operation de rotation.
 */
class dialogRotation : public dialogOperations
{
  Q_OBJECT  

public:  
  
  /**
   * Construit la barre d'outil pour l'operation de rotation.
   * @param parent Le Widget parent
   * @param controler Le controleur
   */
  dialogRotation ( Window * parent ,
		   GMap3d :: CControlerGMap * controler ) ;

  /**
   * Destructeur
   */
  ~dialogRotation ( ) ;
 
  /**
   * Methode de mise a jour de l'affichage
   */
  virtual void update ( ) ;

  // Accesseurs
  //-----------
  void setC ( const CVertex & ) ;
  void setV ( const CVertex & ) ;
  
  void setCx ( TCoordinate ) ;  TCoordinate getCx ( ) const ;
  void setCy ( TCoordinate ) ;  TCoordinate getCy ( ) const ;
  void setCz ( TCoordinate ) ;  TCoordinate getCz ( ) const ;
  
  void setVx ( TCoordinate ) ;  TCoordinate getVx ( ) const ;
  void setVy ( TCoordinate ) ;  TCoordinate getVy ( ) const ;
  void setVz ( TCoordinate ) ;  TCoordinate getVz ( ) const ;

  void setA ( TCoordinate ) ;


public slots:
  void callbackCx ( ) ;
  void callbackCy ( ) ;
  void callbackCz ( ) ;

  void callbackVx ( ) ;
  void callbackVy ( ) ;
  void callbackVz ( ) ;

  void callbackA ( ) ;

  void callbackTakeCenter ( ) ;
  void callbackTakeVector ( ) ;

  void callbackReinitAngle ( ) ;

  void callbackTogglePonderation ( ) ; 

  
private:

  QLabel * FAffSaisie ;

  QLabel * FAffAxe ;
  champsOperations * FSaisieAxe ;
  
  QLabel * FAffAngle ;
  FloatSpinBox * FSaisieAngle ;
  
  QPushButton * FReinitAngle ;

  QCheckBox * FPonderation ;

} ;

#endif
