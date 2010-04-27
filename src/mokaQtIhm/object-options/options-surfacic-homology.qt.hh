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

#ifndef OPTIONSSURFACICHOMOLOGY_QT_HH
#define OPTIONSSURFACICHOMOLOGY_QT_HH

//--------- Include autres classes developees ----------
#include "controler-gmap.hh"

#ifdef _WINDOWS
#include <windows.h>
#endif

//--------------------- Include QT --------------------

#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>


class Window ;
namespace GMap3d {class CHomology;}

/** 
 * Cette classe implemente la fenetre de dialogue pour le calcul des groupes d'homologies
 */
class OptionsSurfacicHomology : public QDialog
{
  Q_OBJECT

public :

  /**
   * Constructeur.
   * @param parent La fenetre principale
   */
  OptionsSurfacicHomology ( Window * parent ) ;

  /**
   * Destructeur
   */
  ~OptionsSurfacicHomology ( ) ;

  /**
   * Mise a jour de l'affichage
   */
  void update ( ) ;

  virtual void show_impl();

private slots:
  void callbackToggleH0(bool);
  void callbackToggleH1free(bool);
  void callbackToggleH1torsion(bool);
  void callbackToggleH2(bool);

  void closeEvent(QCloseEvent *event);
  
private :
  void restoreAlpha3();
  void removeAlpha3();
  
  // Pointeur sur la fenetre principale
  Window * FParent ;

  bool FUpdate ;  

  QGroupBox *FGenerators, *FInfo ;
  QLabel *Fh0, *Fh1free, *Fh1torsion, *Fh2, *FTime, *FMemory;
  QPushButton *FButtonH0, *FButtonH1free, *FButtonH1torsion, *FButtonH2;
  
  // Toolbox which computes homology.
  GMap3d::CHomology* FHomology;

  int FIndexAlpha3;
} ;

#endif
