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

#ifndef OPTIONSEXTRUSION_QT_HH
#define OPTIONSEXTRUSION_QT_HH

//--------- Include autres classes developees ----------
#include "controler-gmap.hh"
#include "floatSpinBox.qt.hh"

//--------------------- Include QT --------------------
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>



class Window ;

/**
 * Implemente la table "extrusion/epaississement"
 */
class OptionsExtrusion : public QWidget
{
  Q_OBJECT

public:

  /**
   * Constructeur
   * Construit la table "extrusion/Epaississement"
   * @param parent La fenetre principale
   * @param parentTab La tabdialog d'options
   */ 
  OptionsExtrusion ( Window * parent , QTabWidget *  parentTab ) ;

  /**
   * Destructeur
   * Detruit la table "Extrusion/Epaississement"
   */
  ~OptionsExtrusion ( ) ;

  /**
   * Met a jour l'affichage de la table
   */
  void update ( ) ;

  // Accesseurs en ecriture
  void setAdjacentSews(bool);
  void setInitialPosition(int);
  void setInitialDirection(int);
  void setDirection(bool);
  void setScale(bool);
  void setPonderation(bool);
  void setRevolutionNbEdges(int);
  void setRevolutionAngle(TCoordinate);
  void setExtrusionCoef(TCoordinate);


public slots :

  void callbackInitialPositionCells ( ) ;
  void callbackInitialDirectCells  ( ) ;

  void callbackAdjacentSews();

  void callbackDirectCells();
  void callbackScaleCells();
  void callbackPonderateSection();

  void callbackRevolutionNbEdges();
  void callbackRevolutionAngle();
  void callbackExtrusionCoef();

private:

  // Pointeur sur la fenetre principale
  Window * FParent ;
  bool FUpdate ;

  QCheckBox * FCoudre, * FOrienter , * FRedimensionner , * FPonderer ;
  QLabel * FAffCoef , * FAffOrientation , * FAffPlacement , * FNbSubdivisions , * FAffAngle ;
  FloatSpinBox * FSaisieCoef , * FSaisieAngle ;
  QSpinBox * FSaisieNbSubdivisions ;
  QComboBox * FChoixOrientation , * FChoixPlacement ;
  QGroupBox * FAffRevolutionChemin , * FAffVecteur , * FAffChemin , * FAffRevolution ;

} ;

#endif
