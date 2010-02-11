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

#ifndef SELECTBAR_QT_HH
#define SELECTBAR_QT_HH

//------------- Include controler ----------------
#include "controler-gmap.hh"
using namespace GMap3d ;

//------ Include autres classes developees --------
class Window ;

//------------------ Include QT -------------------
#include <QtGui/QPushButton>
#include <QtGui/QToolBar>
#include <QtGui/QGroupBox>
#include "IdWidgets.qt.hh"

class SelectBar : public QToolBar
{
  Q_OBJECT

public:
  
  /**
   * Constructeur
   * @param parent Widget parent
   */
  SelectBar ( Window * parent ) ;

  /**
   * Destructeur
   */
  ~SelectBar ( ) ;
  
private slots:

  void callBackClic ( int ind ) ;
  void callbackSelectionLevel ( int id ) ;
  void setSelectionLevel ( int L , bool freeze = false ) ;
  void callbackToggleBlock(bool);
  void callbackToggleSimplificationMode(bool);
  
public slots:

 /**
  * Passe au niveau de selection suivant
  */
 void niveauSuivant ( ) ;

private:

  // Groupe de bouttons
  QGroupBox * FGroupe ; 

  // Pointeur sur le controleur
  GMap3d :: CControlerGMap * FControler ;

  // Pointeur sur la fenetre mere
  Window * FParent ;

  // Boutons de selection de niveau
  IdPushButton ** FNiveaux ;
  
  // Bouton pour blocker le recalcul des coordonnées éclatées
  QPushButton* FBlock;

  // Bouton pour passer un mode simplification, ie les suppression/contraction
  // sont faite sur la carte, et on conserve une copie de la carte initiale
  // pour le plongement.
  QPushButton* FSimplif;
} ;

#endif
