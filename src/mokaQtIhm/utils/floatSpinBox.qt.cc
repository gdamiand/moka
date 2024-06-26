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

#include "floatSpinBox.qt.hh"
#include <cmath>

//*********************************************
// Fonctions auxiliaires
//*********************************************
QString FloatSpinBox :: mapValueToText(int j)
{
  QString str=QString(format).arg(float(j) / sc);
  //str.sprintf(format, float(j) / sc);
  return str;
}

int FloatSpinBox :: mapTextToValue(bool* ok)
{
   *ok = true;
   float f = text().toFloat();
   return int(f*sc);
}

//************************
// Constructeurs
//************************
FloatSpinBox :: FloatSpinBox(double step , double bottom , double top ,
                             int decimals , float iv) : QDoubleSpinBox()
{
   setRange(bottom, top);
   setDecimals(decimals);
   setAccelerated(true);
   setSingleStep(step); ///pow ( 10.0 , decimals ));
   setValuef(iv);
}

FloatSpinBox :: ~FloatSpinBox() {}

float FloatSpinBox::valuef()
{
   return (float)value();
}

void FloatSpinBox::setValuef(float v)
{
   setValue(v) ;
}
