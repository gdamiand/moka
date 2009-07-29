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

#ifndef OPTIONSINTERPOLATION_QT_HH
#define OPTIONSINTERPOLATION_QT_HH

//--------- Include autres classes developees ----------
#include "controler-gmap.hh"

//--------------------- Include QT --------------------
#include <QtGui/QSpinBox>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>


class Window ;


/**
* Implemente l'option "Interpolations/Lissages"
*/
class OptionsInterpolation : public QWidget
{
  Q_OBJECT

public:

  /** 
   * Constructeur
   * Construit la table "Interpolations/Lissages"
   * @param parent La fenetre principale
   * @param parentTab La tabdialog contenant toutes les options
   */
  OptionsInterpolation ( Window * parent , QTabWidget *  parentTab ) ;

  /**
   * Destructeur
   * Detruit la table
   */
  ~OptionsInterpolation ( ) ;

  /**
   * Met a jour l'affichage
   */
  void update ( ) ;

  // Accesseurs
  void setSizeX(int);
  void setSizeY(int);
  void setSizeZ(int);

  void setSmoothMethod(int); int getSmoothMethod() const;
  const CTransformationMatrix & getSmoothMatrix() const;
  
  void setInterpolationMerges(bool); bool getInterpolationMerges() const;
  void setInterpolationSews  (bool); bool getInterpolationSews  () const;

  void setSmoothMerges(bool); bool getSmoothMerges() const;
  void setSmoothSews  (bool); bool getSmoothSews  () const;


public slots:
  void callbackSizeX();
  void callbackSizeY();
  void callbackSizeZ();

  void callbackInterpolationMerges();
  void callbackInterpolationSews();

  void callbackSmoothMethod();

  void callbackSmoothMerges();
  void callbackSmoothSews();

 

private:

  // Pointeur su la fenetre principale
  Window * FParent ;
  bool FUpdate ;

  QLabel * FNbDiv [3] , * FAffMethode ;
  QSpinBox * FSaisieNbDiv [3] ;
  
  QCheckBox * FFusionInterpolation , * FCoudreInterpolation , 
    * FFusionLissage , * FCoudreLissage ;
  
  QComboBox * FSaisieMethode ;

  QGroupBox * FNbSubdivision, * FAffInterpolation , * FAffLissage ;
} ;

#endif
