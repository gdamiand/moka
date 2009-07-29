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

#ifndef CREATION_OBJET_QT_HH
#define CREATION_OBJET_QT_HH

//--------- Include autres classes developees ----------
#include "creation-boutons.qt.hh"
#include "creation-options.qt.hh"
class Window ;

/********************************************************/
/* CLASSE CreationObjet                                 */
/********************************************************/

class CreationObjet : public QToolBar
{
  Q_OBJECT
  
public:
  
  /**
   * Fabrique le widget
   * @param parent Widget parent
   * @param nom Nom du toolbar
   * @param controler Pointeur sur le controleur
   */
  CreationObjet ( Window * parent , QString * nom ,
		          GMap3d :: CControlerGMap * controler ) ;

  /**
   * Destructeur
   */
  ~CreationObjet ( ) ;

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

 public slots :

  /**
   * Methode qui cree l'objet en creation
   */
  virtual void create ( ) ;

  /**
   * Methode qui quitte la phase de creation
   */
  virtual void cancel ( ) ;

  /**
   * Methode qui affiche la boite d'options
   */
  virtual void showOptions ( ) ;

protected:
  
  // Boutons
  BoutonsCreation * FChoix ;

  // Pointeur sur le controleur
  GMap3d :: CControlerGMap * FControler ;

  // Pointeur sur le parent
  Window * FParent ;

  // Pointeur sur la boite de positionnement
  BoitePositionnement * FPosit ;
 
  // Booleen indiquant si on est en mode "mise a jour"
  bool FUpdate ;
} ;


#endif

