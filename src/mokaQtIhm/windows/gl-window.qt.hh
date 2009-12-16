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

#ifndef GLWINDOW_QT_HH
#define GLWINDOW_QT_HH

//------ Include autres classes developees -------
#include "creation-objet.qt.hh"
#include "dialog-operations.qt.hh"
#include "options-carac.qt.hh"
#include "select-bar.qt.hh"

//------------- Include controler ----------------
#include "controler.hh"
#include "controler-gmap.hh"

//------------------ Include QT -------------------
#include <QtGui/QWorkspace>

class Window ;

class GLWindow : public QGLWidget
{
  Q_OBJECT
  
public:

  /**
   * @brief Constructeur pour une vue non partagee
   * @param AViewType Type de vue a creer
   * @param parent Widget parent
   * @param owner Fenetre mere
   * @param selection Pointeur sur la barre de selection
   */
  GLWindow ( TView AViewType , QWorkspace * parent , Window * owner ,
	     SelectBar * selection ) ;

  /**
   * @brief Constructeur pour une vue partagee
   * @param AViewType Type de vue a creer
   * @param parent Widget parent
   * @param owner Fenetre mere
   * @param share Vue avec laquelle elle partage le contexte OpenGL
   * @param selection Pointeur sur la barre de selection
   */
  GLWindow ( TView AViewType , QWorkspace * parent , 
	     Window * owner , GLWindow * share , SelectBar * selection  ) ;
  
  /**
   * Destructeur
   */
  virtual ~GLWindow ( ) ;

  /**
   * Accesseur sur le type de vue
   * @return Le type de la vue
   */
  virtual TView getViewType ( ) const ;

  /**
   * Accesseur sur l'identifiant de la vue
   * @return L'identifiant de la vue
   */
  virtual TViewId getViewId ( ) const ;

  /**
   * Accesseur sur le nom du type de la vue
   * @return   La chaine caracterisant la vue
   */
  virtual string getViewTypeString() const;

  /**
   * Recuperation de l'evenement fermeture
   * @param e Evenement
   */
  virtual void closeEvent ( QCloseEvent * e ) ;

  /**
   * Accesseur sur l'identifiant de la vue cliquee
   * @return L'identifiant de la vue 
   */
  virtual TViewId getCliquedViewId ( ) const ; 

  /**
   * Accesseur sur l'identifiant de la vue double-cliquee
   * @return L'identifiant de la vue 
   */
  virtual TViewId getDoubleCliquedViewId ( ) const ;
  
  /**
   * Dessin de la scene
   */
  virtual void paintGL ( ) ;

protected:

  /**
   * Initialisation d'OpenGL
   */
  virtual void initializeGL ( ) ;

  /**
   * Initialisation de la vue dans le controleur
   */
  virtual void creation ( ) ;

  /**
   * Gestion de l'appui d'un des boutons de la souris
   * @param e Evenement bouton enfonce
   */
  virtual void mousePressEvent ( QMouseEvent * e ) ;

  /**
   * Gestion de relachement d'un des boutons de la souris
   * @param e Evenement bouton lache
   */
  virtual void mouseReleaseEvent ( QMouseEvent * e ) ;

  /**
   * Gestion des mouvements de souris
   * @param e Evenement souris bougee
   */
  virtual void mouseMoveEvent ( QMouseEvent * e ) ;

  /**
   * Gestion du reajustement de la taille de la fenetre
   * @param W width
   * @param H height
   */
  virtual void resizeGL ( int W , int H ) ; 

  /**
   * Gestion du double clicage
   * @param e Evenementdouble clic
   */
  virtual void mouseDoubleClickEvent ( QMouseEvent * e ) ;
  
  // Identifiant associe a cette fenetre (cf. controler-views.hh):
  TViewId FViewId;
  TView   FViewType;

  // Fenetre paratagee
  GLWindow * FShared ;

  // indique si la fenetre est partagee
  bool isShared; 

  // Fenetre principale contenant la vue
  Window * FOwner ;

  // Pointeur sur la barre de selection
  SelectBar * FSelection ;
  
  // Paramètre utiles pour l'affichage du rectangle de sélection
  int FStartX, FStartY, FCurX, FCurY;
  bool FDragMode;
} ;

#endif
