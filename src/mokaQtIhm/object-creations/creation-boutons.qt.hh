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

#ifndef BOUTONS_CREATION_QT_HH
#define BOUTONS_CREATION_QT_HH

//--------- Include autres classes developees ---------
class CreationObjet ;

//----------------- Include Controler -----------------
#include "controler-gmap.hh"

//--------------------- Include QT --------------------
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>

/*******************************************************/
/* CLASSE BoutonsCreation                              */
/*******************************************************/
class BoutonsCreation : public QWidget
{
  Q_OBJECT

public:

  /**
   * Fabrique les 3 boutons et les places
   * @param parent Widget parent
   * @param layout Sera ajoute au layout
   */
  BoutonsCreation ( CreationObjet * parent , QBoxLayout * layout = NULL) ;
  
  /**
   * Destructeur
   */
  ~BoutonsCreation ( ) ;
  
  /**
   * Accesseur sur le bouton Annuler
   * @return Un pointeur sur le bouton annuler
   */
  QPushButton * getAnnuler ( ) ;

  /**
   * Accesseur sur le bouton options
   * @return Un pointeur sur le bouton options
   */
  QPushButton * getOptions ( ) ;

public slots:

  /**
   * Methode qui reinitialise les parametres de creation
   */  
  void reinit ( ) ;

  /**
   * Methode qui clos la phase de creation courante
   */  
  void cancel ( ) ;

  /**
   * Methode qui cree l'objet en creation
   */
  void create ( ) ;

  /**
   * Methode qui rend visible la fenetre des options
   */
  void options ( ) ;
  
private:
  
  // Les boutons
  QPushButton * FReinitialiser ;
  QPushButton * FAnnuler ;
  QPushButton * FCreer ;
  QPushButton * FOptions ;

  // Pointeur sur le widget parent
  CreationObjet * FParent ;
} ;

#endif
