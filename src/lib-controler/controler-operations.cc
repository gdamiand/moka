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
#include "controler.hh"
#include "operations.hh"
#include "view-precompile.hh"

#include <cassert>
#include <cstring>
using namespace std;
//******************************************************************************
bool CControler::canApplyOperation(const COperation& )
{
  return true;
}
//******************************************************************************
bool CControler::applyOperation(const COperation& )
{
  return false;
}
//******************************************************************************
#ifdef NO_ACCENTED_CHARS
char CControler::unaccent(char AChar) const
{
  static const char* from = "àâÀÂéèëêÉÈËÊîïÎÏôÔùûÙÛ";
  static const char* to   = "aaAAeeeeEEEEiiIIoOuuUU";

  char* pos = strchr(from, AChar);

  if (pos != NULL)
    return * (to - from + pos);

  return AChar;
}
#endif // NO_ACCENTED_CHARS

string CControler::treatAccent(const string& AString) const
{
  string result = AString;
  
#ifdef NO_ACCENTED_CHARS
  for (unsigned int i=0; i<result.length(); ++i)
    result[i] = unaccent(result[i]);

#endif // NO_ACCENTED_CHARS

  return result;
}

string CControler::getMessage() const
{
  return treatAccent(FMessage);
}
//******************************************************************************
void CControler::setMessage(const string & AMessage)
{ FMessage = AMessage; }
//******************************************************************************
void CControler::setMessage(int ANumber, const string & AMessage)
{
  stringstream s;
  s<<ANumber<<AMessage;  
  setMessage(s.str());
}
//******************************************************************************
void CControler::setMessage(int ANumber1, int ANumber2, const string & AMessage)
{
  stringstream s;
  s<<ANumber1<<" "<<ANumber2<<AMessage;
  setMessage(s.str());
}
//******************************************************************************
void CControler::setMessage(const string & AMessage1, int ANumber,
			    const string & AMessage2)
{
  stringstream s;
  s<<AMessage1<<ANumber<<AMessage2;  
  setMessage(s.str());
}
//******************************************************************************
bool CControler::lookAtOrigin(TViewId AView)
{
  getParameterAimedPosition(AView)->setLookAt(0,0);
  getParameterAimedPosition(AView)->setLookAt(1,0);
  getParameterAimedPosition(AView)->setLookAt(2,0);
  return true;
}
//******************************************************************************
bool CControler::lookAtMouseClick(TViewId AView, int x, int y)
{
  CViewPrecompile * view = FViews[AView];
  
  if (view != NULL)
  {
    float res[3];
    view -> unproject(x,y,res);

    CParameterAimedPosition * aimed_pos = getParameterAimedPosition(AView);  
    aimed_pos -> setLookAt(0,res[0]);
    aimed_pos -> setLookAt(1,res[1]);
    aimed_pos -> setLookAt(2,res[2]);
    return true;
  }
  return false;
}
//******************************************************************************
