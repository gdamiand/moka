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
#include "view.hh"
#include "precompile.hh"
#include <cassert>
#include <iostream>
//******************************************************************************
CPrecompile::CPrecompile(unsigned int ANb) :
  FToUpdate	(true),
  FEnable	(true),
  FNbView	(ANb),
  FNbEnabledView(0)
{
#ifndef NO_LIST_OPENGL
  FCompiledList = -1;
#endif // NO_LIST_OPENGL
}
//******************************************************************************
CPrecompile::CPrecompile(const CPrecompile & APrecompile) :
  FToUpdate	(APrecompile.FToUpdate),
  FEnable	(APrecompile.FEnable),
  FNbView	(0),
  FNbEnabledView(0)
{
#ifndef NO_LIST_OPENGL
  FCompiledList = -1;
#endif // NO_LIST_OPENGL 
}
//******************************************************************************
CPrecompile::~CPrecompile()
{
#ifndef NO_LIST_OPENGL
  glDeleteLists(FCompiledList, 1);
#endif // NO_LIST_OPENGL 
}
//******************************************************************************
void CPrecompile::draw()
{
  if (isEnabled())
    {
#ifndef NO_LIST_OPENGL
      glCallList(FCompiledList);
#else
      drawModel();
#endif // NO_LIST_OPENGL 
    }
}
//******************************************************************************
void CPrecompile::update()
{
#ifndef NO_LIST_OPENGL  
  if (needToUpdate())
    {
      compileModel ();
      unsetToUpdate();
    }
#endif // NO_LIST_OPENGL 
}
//******************************************************************************
void CPrecompile::pick(int /*x*/, int /*y*/, CView* /*AView*/)
{}
//******************************************************************************
bool CPrecompile::needToUpdate()
{ return FToUpdate; }
//******************************************************************************
void CPrecompile::setToUpdate()
{ if (isEnabled()) FToUpdate = true; }
//******************************************************************************
void CPrecompile::unsetToUpdate()
{ FToUpdate = false; }
//******************************************************************************
unsigned int CPrecompile::getNbView()
{ return FNbView; }
//******************************************************************************
void CPrecompile::incNbView(unsigned int ADec)
{ FNbView+=ADec; }
//******************************************************************************
void CPrecompile::decNbView(unsigned int ADec)
{
  assert( ADec<=FNbView );

  FNbView-=ADec;
  if ( FNbView==0 ) delete this;
}
//******************************************************************************
unsigned int CPrecompile::getNbEnabledView()
{ return FNbEnabledView; }
//******************************************************************************
void CPrecompile::incNbEnabledView(unsigned int ADec)
{
  if ( FNbEnabledView==0 && FEnable==true ) FToUpdate=true;
  FNbEnabledView+=ADec;
}
//******************************************************************************
void CPrecompile::decNbEnabledView(unsigned int ADec)
{
  assert( ADec<=FNbEnabledView );

  FNbEnabledView-=ADec;
  if ( FNbEnabledView==0 && FEnable==true ) FToUpdate=true;
}
//******************************************************************************
bool CPrecompile::isEnabled()
{ return ( FEnable && FNbEnabledView>0 ); }
//******************************************************************************
void CPrecompile::enable()
{
  if (!FEnable)
    {
      if (FNbEnabledView>0) FToUpdate=true;
      FEnable=true;
    }
}
//******************************************************************************
void CPrecompile::disable()
{
  if (FEnable)
    {
      if (FNbEnabledView>0) FToUpdate=true;
      FEnable= false;
    }
}
//******************************************************************************
void CPrecompile::compileModel()
{
#ifndef NO_LIST_OPENGL
  if (FCompiledList == -1)
    // Création du numéro de la liste compilée
    FCompiledList = glGenLists(1);

  glNewList(FCompiledList, GL_COMPILE);
  
  if (isEnabled())
    drawModel();
  
  glEndList();
#endif // NO_LIST_OPENGL
}
//******************************************************************************
