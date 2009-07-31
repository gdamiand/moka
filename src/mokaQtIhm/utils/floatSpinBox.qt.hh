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

#ifndef FLOATSPINBOX_QT_HH
#define FLOATSPINBOX_QT_HH

//----------------- Include std ------------------
#include <iostream>
using namespace std ;

#include <cstdlib>
#include <string.h>

//------------- Include QT --------------
#include <QtGui/QSpinBox>
#include <QtGui/QValidator> 
#include <QtGui/QLineEdit> 
#include <QtCore/QString>

class FloatSpinBox : public QDoubleSpinBox
{
  Q_OBJECT
  
public:

  FloatSpinBox( double step = 1, double bottom = -1000, double top = 1000, 
                int decimals = 3, float iv = 0.1) ;

  ~FloatSpinBox ( ) ;
  
  float valuef() ;
  
  void setValuef(float v) ;
  
private:
  
  QString mapValueToText( int j ) ;
  int mapTextToValue( bool* ok ) ;
  float sc;                        //scale of the value
  char format[3];                  //text format
} ;

#endif
