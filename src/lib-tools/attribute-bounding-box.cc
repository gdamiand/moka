/*
 * lib-tools : Divers outils autour de lib-gmapkernel.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-tools
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "attribute-bounding-box.hh"
using namespace std;
//******************************************************************************
CAttributeBoundingBox::CAttributeBoundingBox()
  : CAttribute(), CBoundingBox()
{
}
//******************************************************************************
CAttributeBoundingBox::CAttributeBoundingBox(const CBoundingBox & ABB)
  : CAttribute(), CBoundingBox(ABB)
{
}
//******************************************************************************
CAttributeBoundingBox::CAttributeBoundingBox(const CAttributeBoundingBox & AOther)
  : CAttribute(AOther), CBoundingBox(AOther)
{
}
//******************************************************************************
CAttributeBoundingBox::~CAttributeBoundingBox()
{
}
//******************************************************************************
TAttributeId CAttributeBoundingBox::getType() const
{
 return ATTRIBUTE_COREFINE_BBOX;
}
//******************************************************************************
CAttribute * CAttributeBoundingBox::copy() const
{
  return new CAttributeBoundingBox(*this);
}
//******************************************************************************
void CAttributeBoundingBox::destroy()
{
  delete this;
}
//******************************************************************************
void CAttributeBoundingBox::save(ostream & AStream) const
{
}
//******************************************************************************
void CAttributeBoundingBox::load(istream & AStream)
{
}
//******************************************************************************
