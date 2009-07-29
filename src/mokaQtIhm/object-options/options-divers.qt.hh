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

#ifndef OPTIONSDIVERS_QT_HH
#define OPTIONSDIVERS_QT_HH

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


class OptionsDivers : public QWidget
{
  Q_OBJECT

public:


  OptionsDivers ( Window * parent , QTabWidget *  parentTab ) ;

  ~OptionsDivers ( ) ;

  void update ( ) ;


public slots :

  void callbackChangeAngle(double val);
  void callbackChangePas(double val);

private:

  Window * FParent ;
  
  bool FUpdate ;

  FloatSpinBox * saisie_angle , * saisie_pas;

} ;

#endif
