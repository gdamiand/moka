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

#ifndef DIALOG_DO_QT_HH
#define DIALOG_DO_QT_HH

//------------- Include controler ----------------
#include "controler-gmap.hh"
using namespace GMap3d ;

//------------------ Include QT -------------------
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QDialog>


class Window ;

/**
 * @class DialogDo
 * @brief Implementation de la boite de dialogues des options de undo/redo
 */
class DialogDo : public QDialog
{
  Q_OBJECT

public:

  /**
   * @brief Construit la fenetre de dialogue
   * @param parent Fenetre mere
   */
  DialogDo ( Window * parent ) ;

  /**
   * Destructeur
   */
  ~DialogDo ( ) ;


private slots :

  // Slots prives
  void callbackToggleOnFile();
  void callbackNbUndos();

private :

  // Pointeur sur le parent
  Window * FParent ;

  // Affichage
  QLabel * FMax ;

  // Taille de la pile
  QSpinBox * FNombre ;

  // Fichiers
  QCheckBox * FFichiers ;
  
  QPushButton * FClose ;

} ;

#endif
