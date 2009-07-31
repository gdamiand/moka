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
#include "view-precompile.hh"
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef _WINDOWS
#include <direct.h>
#endif

using namespace std;
//******************************************************************************
#define DEFAULT_DIRECTORY ".moka"
//******************************************************************************
CControler::CControler(int ANb, const std::string & AName,
		       const string & ADirectory) :
  // controler-views.hh
  FViews	       (INITIAL_NB_VIEWS),
  FLastPrecompile      (ANb),
  // controler-input-events.hh
  FCurrentMode         (MODE_DEFAULT),
  FCurrentModeOperation(MODE_OPERATION_NONE),
  // controler-undo-redo.hh
  FUndoOnFile	       (true),
  FNbMaxUndos	       (100),
  FLastFileIndex       (0),
  FActu                (NULL)
{
  for (int j=0; j<INITIAL_NB_VIEWS; ++j)
    FViews[j] = NULL;
  
//----------------------------------------------------------------------------
  // Si le répertoire de configuration n'est pas fourni, on le fixe
  // par défaut à $(HOME)
  if (ADirectory=="")
    {
      char* home = getenv("HOME");
      
      // Si la variable PATH existe, on l'analyse:
      if (home!=NULL)
	FConfigDirectory=string(home)+string("/");
    }
 
//----------------------------------------------------------------------------
  FConfigDirectory += AName;
  
  // Création du répertoire FConfigDirectory au cas ou il n'existe pas déjà.
#ifdef _WINDOWS
  _mkdir(FConfigDirectory.c_str());
#else
#if defined(_MSC_VER) || defined(__MINGW32__)
  mkdir(FConfigDirectory.c_str());
#else
  mkdir(FConfigDirectory.c_str(), 0755);
#endif
#endif

//----------------------------------------------------------------------------

  // Récupération éventuelle des undo sur fichiers.
  recupFromFiles();
}
//******************************************************************************
CControler::~CControler()
{
  for (vector<CViewPrecompile *>::iterator it = FViews.begin();
       it!=FViews.end(); ++it)
    if ( (*it)!=NULL )
      delete (*it);

  emptyRedoList();
}
//******************************************************************************

