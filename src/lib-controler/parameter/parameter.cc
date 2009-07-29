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
#include "parameter.hh"
#include "precompile.hh"
#include "controler-types.hh"
#include <cassert>
using namespace std;
//******************************************************************************
CParameter::CParameter(unsigned int ANbRef) :
  FNbRef(ANbRef)
{
}
//******************************************************************************
CParameter::CParameter(const CParameter & AParameter) :
  FNbRef(0)
{
}
//******************************************************************************
CParameter::~CParameter()
{
}
//******************************************************************************
void CParameter::incNbRef(unsigned int AValue)
{ FNbRef += AValue; }
//******************************************************************************
void CParameter::decNbRef(unsigned int AValue)
{
  assert(FNbRef >= AValue);

  FNbRef -= AValue;
  if ( FNbRef==0 ) delete this;
}
//******************************************************************************
unsigned int CParameter::getNbRef() const
{ return FNbRef; }
//******************************************************************************
void CParameter::addPrecompileToUpdate(CPrecompile * APrecompile)
{
  assert(APrecompile != NULL);

  FListPrecompile.push_back(APrecompile);
  incNbRef();
}
//******************************************************************************
void CParameter::removePrecompileToUpdate(CPrecompile * APrecompile)
{
  assert(APrecompile != NULL);
  bool found = false;

  list<CPrecompile *>::iterator it= FListPrecompile.begin();
  while(!found && it!=FListPrecompile.end())
    {
      if (*it == APrecompile)
	{
	  FListPrecompile.erase(it);
	  found = true;
	}
      else
	++it;
    }

  if (found) decNbRef();
}
//******************************************************************************
void CParameter::putAllNeedToUpdate()
{
  list<CPrecompile *>::iterator it = FListPrecompile.begin();
  while( it!=FListPrecompile.end() )
    {
      (*it)->setToUpdate();
      ++it;
    }
}
//******************************************************************************
