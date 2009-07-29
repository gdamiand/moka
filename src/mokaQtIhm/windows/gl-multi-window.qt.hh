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

#ifndef GLWINDOW_MULTI_QT_HH
#define GLWINDOW_MULTI_QT_HH

//------------- Include controler ----------------
#include "controler.hh"
#include "controler-gmap.hh"

//------ Include autres classes developees -------
#include "creation-objet.qt.hh"
#include "dialog-operations.qt.hh"
#include "gl-window.qt.hh"

#ifdef MODULE_ARCHITECTURE
#include "flap-selection.qt.hh"
#endif //MODULE_ARCHITECTURE
//------------------ Include QT -------------------

class Window ;
class GLWindow ;

class GLMultiWindow : public GLWindow
{

  Q_OBJECT
  
public:

  /**
   * Constructeur
   * @param parent Workspace parent
   * @param owner Fenetre principale
   * @param share Vue avec laquelle est partage le contexte OpenGL
   */
  GLMultiWindow ( QWorkspace * parent , Window * owner , GLWindow * share ,
		  SelectBar * selection ) ;
  
  /**
   * Destructeur
   */
  virtual ~GLMultiWindow ( ) ;

  /**
   * Accesseur sur le type de la vue
   * @return L'identifiant de la vue 0
   */
  TViewId getViewId ( ) const ; 
  
  /**
   * Accesseur sur le nom du type de la vue
   * @return   La chaine caracterisant la vue
   */
  virtual string getViewTypeString() const;

  /**
   * Initialisation de la vue dans le controleur
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

protected:

  /**
   * Initialisation d'OpenGL
   */
  virtual void initializeGL ( ) ;
  
  /**
   * Dessin de la scene
   */
  virtual void paintGL ( ) ;
  
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
   * Gestion de l'appui d'un des boutons de la souris
   * @param e Evenement bouton enfonce
   */
  virtual void mouseDoubleClickEvent ( QMouseEvent * e ) ;

private:
  
  // Methode indiquant dans quel cadre se trouve cette position
  int cadre ( int x , int y ) ;
  
  // Identifiants associes aux 4 vues (cf. controler-views.hh):
  TViewId FViewIds [ 4 ] ;

  // Numero de la vue cliquee : 0=Haut-Gauche, 1=Haut-droit,
  //                            2=Bas-Gauche,  3=Bas-droit
  int FCliqued;
  
  // Numero de la vue double-cliquee : 0=Haut-Gauche, 1=Haut-droit,
  //                                   2=Bas-Gauche,  3=Bas-droit
  int FDoubleCliqued;
} ;

#endif
