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

#ifndef DIALOGMOVETO_QT_HH
#define DIALOGMOVETO_QT_HH

#include "dialog-operations.qt.hh"

class Window ;

namespace GMap3d{ class CDart;}

/**
 * Implemente une barre d'outil pour déplacer un sommet à une position
 * absolue.
 */
class dialogMoveto : public dialogOperations
{

  Q_OBJECT  
public:
  
  /**
   * Constructeur.
   * @param parent La fenetre principale
   * @param controler Le controleur
   */
  dialogMoveto ( Window * parent ,
		 GMap3d :: CControlerGMap * controler ) ;
  
  /**
   * Destructeur
   */
  ~dialogMoveto ( ) ;
 
  /**
   * Methode de mise a jour de l'affichage
   */
  virtual void update ( ) ;


  // Accesseurs
  //------------
  CVertex getPos ( ) ;

  void setPos(const CVertex & /*AVertex*/);
  
  void setXPos ( TCoordinate ) ;  TCoordinate getXPos ( ) const ;
  void setYPos ( TCoordinate ) ;  TCoordinate getYPos ( ) const ;
  void setZPos ( TCoordinate ) ;  TCoordinate getZPos ( ) const ;
  
  
private slots:
  void callbackXPos ( ) ;
  void callbackYPos ( ) ;
  void callbackZPos ( ) ;
  
  void callbackReinitPos ( ) ;
  
private:

  // Bouton reinitialiser
  QPushButton * FReinit ;

  // Actual last selected dart
  GMap3d::CDart* FSelectedDart;
  
  // Initial position
  CVertex FInitialPos ;
} ;

#endif
