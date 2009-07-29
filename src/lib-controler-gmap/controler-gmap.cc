/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
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
#include "controler-gmap.hh"
#include "view-precompile.hh"
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
using namespace GMap3d;
//******************************************************************************
CControlerGMap::CControlerGMap(CGMapVertex* AGMapVertex,
                               const string & ADirectory) :
      CControler(LAST_PRECOMPILE_CONTROLER_GMAP,
                 CONTROLER_GMAP_DIRECTORY, ADirectory),
      // controler-gmap-object-transformation.hh
      FDirectInfoPonderation(-1),
#ifdef MODULE_ROUNDING
      // controler-gmap-rounding.hh
      FRounding(NULL),
#endif // MODULE_ROUNDING
      FDeselectMode(false),
      // controler-gmap-protected-methods.hh
      FNbDarts(0),
      FNbSelectedDarts(0),
      FNbVertices(0),
      // controler-gmap.hh
      FMap(AGMapVertex == NULL ? new CGMapVertex() : AGMapVertex),
      FParameterGMapVertex(new CParameterGMapVertex(FMap, 1)),
      FParameterSelection(new CParameterSelection(this, NB_SELECTION_LEVELS, 
                          1)),
      FParameterCreation(new CParameterCreation(1)),
      FParameterPolylinePosition(new CParameterPolylinePosition(1)),
      FParameterOperations(new CParameterOperations(1)),
      FParameterTranslation(new CParameterObjectTranslation(1)),
      FParameterRotation(new CParameterObjectRotation(1)),
      FParameterScale(new CParameterObjectScale(1)),
      FParameterPonderation(new CParameterObjectPonderation(1))
{
   assert(FMap != NULL);

   for (int i = 0; i < NB_PREDEFINED_OBJECTS; ++i)
   {
      bool rotate = i == OBJECT_SPHERE || i == OBJECT_PYRAMID 
            || i == OBJECT_CYLINDER;
      FParameterObjectPosition[i] = new CParameterObjectPosition(rotate, 1);
   }
}
//******************************************************************************
CControlerGMap::~CControlerGMap()
{
   FParameterGMapVertex->decNbRef();
   FParameterSelection->decNbRef();
   FParameterCreation->decNbRef();
   FParameterPolylinePosition->decNbRef();
   FParameterOperations->decNbRef();
   FParameterTranslation->decNbRef();
   FParameterRotation->decNbRef();
   FParameterScale->decNbRef();
   FParameterPonderation->decNbRef();

   for (int i = 0; i < NB_PREDEFINED_OBJECTS; ++i)
      FParameterObjectPosition[i]->decNbRef();
}
//******************************************************************************
CGMapVertex* CControlerGMap::getMap()
{ return FMap; }
//******************************************************************************
