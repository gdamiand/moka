/*
 * lib-controler : Un contrôleur générique de scène 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler
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
#include "controler-types.hh"
#include "precompile-grid.hh"
#include <cassert>
//******************************************************************************
CPrecompileGrid::CPrecompileGrid( CParameterGrid * AParameterGrid ) :
  FParameterGrid(AParameterGrid)
{
  assert(FParameterGrid!=NULL);
  FParameterGrid->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileGrid::CPrecompileGrid( const CPrecompileGrid & APrecompile ) :
  CPrecompile   (APrecompile),
  FParameterGrid(static_cast<CParameterGrid*>
		 (APrecompile.FParameterGrid->copy()))
{
  assert(FParameterGrid!=NULL);
  FParameterGrid->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileGrid::~CPrecompileGrid()
{
  FParameterGrid->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompileGrid::copy() const
{ return new CPrecompileGrid(*this); }
//******************************************************************************
void CPrecompileGrid::setParameter(CParameter * AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_GRID:
      setGrid(static_cast<CParameterGrid *>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompileGrid::getParameter() const
{ return FParameterGrid; }
//******************************************************************************
void CPrecompileGrid::setGrid(CParameterGrid * AGrid)
{
  assert(AGrid != NULL);
  AGrid->addPrecompileToUpdate(this);
  FParameterGrid->removePrecompileToUpdate(this);
  FParameterGrid = AGrid;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileGrid::getType() const
{ return PRECOMPILE_GRID; }
//******************************************************************************
void CPrecompileGrid::drawModel()
{
  int  size= FParameterGrid->getLGGrid();
  bool xy  = FParameterGrid->getDisplayGridXY();
  bool xz  = FParameterGrid->getDisplayGridXZ();
  bool yz  = FParameterGrid->getDisplayGridYZ();
  bool px  = FParameterGrid->getDisplayPositiveGridX();
  bool py  = FParameterGrid->getDisplayPositiveGridY();
  bool pz  = FParameterGrid->getDisplayPositiveGridZ();
  int  xmin= px ? 0 : -size;
  int  ymin= py ? 0 : -size;
  int  zmin= pz ? 0 : -size;

  glLineWidth(FParameterGrid->getLWGrid());

  glBegin(GL_LINES);

  glColor3fv(FParameterGrid->getCLGrid());

  for (int i=-size; i<=+size; ++i)
    {
      if (xy)
	{
	  /* X direction: */
	  if (i>=0 || !py) { glVertex3f(xmin, i, 0); glVertex3f(size, i, 0); }
	  /* Y direction: */
	  if (i>=0 || !px) { glVertex3f(i, ymin, 0); glVertex3f(i, size, 0); }
	}
      if (xz)
	{
	  /* X direction: */
	  if (i>=0 || !pz) { glVertex3f(xmin, 0, i); glVertex3f(size, 0, i); }
	  /* Z direction: */
	  if (i>=0 || !px) { glVertex3f(i, 0, zmin); glVertex3f(i, 0, size); }
	}
      if (yz)
	{
	  /* Y direction: */
	  if (i>=0 || !pz) { glVertex3f(0, ymin, i); glVertex3f(0, size, i); }
	  /* Z direction: */
	  if (i>=0 || !py) { glVertex3f(0, i, zmin); glVertex3f(0, i, size); }
	}
    }

  glEnd();
}
//******************************************************************************
