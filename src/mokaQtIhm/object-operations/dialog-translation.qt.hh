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

#ifndef DIALOGTRANSLATION_QT_HH
#define DIALOGTRANSLATION_QT_HH

#include "dialog-operations.qt.hh"

class Window ;

/**
 * Implemente la barre d'outil pour l'operation de translation.
 */
class dialogTranslation : public dialogOperations
{

  Q_OBJECT  
public:
  
  /**
   * Construit la barre d'outil pour l'operation de translation
   * @param parent La fenetre principale
   * @param controler Le controleur
   */
  dialogTranslation ( Window * parent ,
		      GMap3d :: CControlerGMap * controler ) ;
  
  /**
   * Destructeur
   */
  ~dialogTranslation ( ) ;
 
  /**
   * Methode de mise a jour de l'affichage
   */
  virtual void update ( ) ;


  // Accesseurs
  //------------
  CVertex getV ( ) ;

  void setV(const CVertex & /*T*/);
  
  void setVx ( TCoordinate ) ;  TCoordinate getVx ( ) const ;
  void setVy ( TCoordinate ) ;  TCoordinate getVy ( ) const ;
  void setVz ( TCoordinate ) ;  TCoordinate getVz ( ) const ;
  
  
private slots:
  void callbackVx ( ) ;
  void callbackVy ( ) ;
  void callbackVz ( ) ;
  
  void callbackTakeVector ( ) ;
  void callbackReinitVector ( ) ;
  
  void callbackTogglePonderation ( ) ;
  

private:

  // Bouton reinitialiser
  QPushButton * FReinit ;
  
  // Checkbox Translation ponderee
  QCheckBox * FSaisieTranslate ;

} ;

#endif
