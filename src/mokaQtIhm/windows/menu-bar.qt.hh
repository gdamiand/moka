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

#ifndef MENUBAR_QT_HH
#define MENUBAR_QT_HH

//------ Include autres classes developees --------
class Window ;

//------------------ Include QT -------------------

#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QAction>



class MenuBar : public QMenuBar
{
  Q_OBJECT

public:

  /**
   * Construit le menu, ajoute les differents items
   * @param parent Fenetre principale a laquelle il appartient
   */
  MenuBar ( Window * parent ) ;

  /**
   * Destructeur
   */
  ~MenuBar ( ) ;

private:

  // Pointeur sur la fenetre mere
  Window * FParent ;
  
  
  // Les differents sous-menus.
  
  QMenu * fichier;
  QMenu * edition;
  QMenu * options;
  QMenu * creations;
  QMenu * operations;
  QMenu * geologie;
  QMenu * materialiser;
  QMenu * selection;
  QMenu * vues;
  
  // Les actions associees a chaque sous-menu.
  
  QAction * fichier_act;
  QAction * edition_act;
  QAction * options_act;
  QAction * creations_act;
  QAction * operations_act;
  QAction * geologie_act;
  QAction * materialiser_act;
  QAction * selection_act;
  QAction * vues_act;
  
  // Les actions dynamiques du menu.
  
  QAction * charger_act;
  QAction * ajouter_act;
  QAction * sauver_act;
  QAction * importer_act;
  QAction * exporter_act;
  
  QAction * supprimer1_act;
  QAction * supprimer2_act;
} ;

#endif
