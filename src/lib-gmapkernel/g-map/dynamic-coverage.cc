/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-gmapkernel
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

//******************************************************************************
#include "dynamic-coverage.hh"
//******************************************************************************
// Instanciation des templates :

namespace GMap3d
{
  template class CTemplateDynamicCoverage1<0>;
  template class CTemplateDynamicCoverage1<1>;
  template class CTemplateDynamicCoverage1<2>;
  template class CTemplateDynamicCoverage1<3>;
  template class CTemplateDynamicCoverage2b<0,1>;
  template class CTemplateDynamicCoverage2a<0,2>;
  template class CTemplateDynamicCoverage2a<0,3>;
  template class CTemplateDynamicCoverage2b<1,2>;
  template class CTemplateDynamicCoverage2a<1,3>;
  template class CTemplateDynamicCoverage2b<2,3>;
  template class CTemplateDynamicCoverage3a<0,1,3>;
  template class CTemplateDynamicCoverage3a<2,3,0>;
  template class CTemplateDynamicCoverage3b<0,1,2>;
  template class CTemplateDynamicCoverage3b<1,2,3>;

  template class CTemplateBasicDynamicCoverage3b<0,1,2>;
  template class CTemplateBasicDynamicCoverage3b<1,2,3>;

  template class CTemplateMultiDynamicCoverage3b<0,1,2>;
  template class CTemplateMultiDynamicCoverage3b<1,2,3>;
} // namespace GMap3d
//******************************************************************************
