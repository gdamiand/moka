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

#include "SpaceWidget.qt.hh"


SpaceWidget::SpaceWidget(int width,int height)
  : QWidget()
{
  QBoxLayout* bl = new QBoxLayout(QBoxLayout::LeftToRight, this);
  bl -> setSpacing(0);
  bl -> setContentsMargins(0,0,0,0);
  bl -> addItem(new QSpacerItem(width,height));
  setContentsMargins(0,0,0,0);
}
    

SpaceWidget::~SpaceWidget()
{
}
