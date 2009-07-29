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
#include "precompile.hh"
#include "parameter-gmap-vertex.hh"
#include "parameter-aimed-point.hh"
#include "parameter-axis.hh"
#include "parameter-dart.hh"
#include "parameter-face.hh"
#include "parameter-grid.hh"
#include "parameter-normal-vector.hh"
#include "parameter-object-transformation.hh"
#include "parameter-preview.hh"
#include "parameter-sew.hh"
#include "parameter-spamod.hh"
#include "parameter-vertex.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterGMapVertex* CControlerGMap::getParameterGMapVertex() const
{ return FParameterGMapVertex; }
//******************************************************************************
CParameterSelection* CControlerGMap::getParameterSelection() const
{ return FParameterSelection; }
//******************************************************************************
CParameterCreation* CControlerGMap::getParameterCreation() const
{ return FParameterCreation; }
//******************************************************************************
CParameterPolylinePosition* CControlerGMap::getParameterPolylinePosition() const
{ return FParameterPolylinePosition; }
//******************************************************************************
CParameterOperations* CControlerGMap::getParameterOperations() const
{ return FParameterOperations; }
//******************************************************************************
CParameterObjectPosition* CControlerGMap::getParameterObjectPosition(int AIndex) const
{
  assert( 0<=AIndex && AIndex<NB_PREDEFINED_OBJECTS );
  return FParameterObjectPosition[AIndex];
}
//******************************************************************************
CParameterObjectPosition* CControlerGMap::getParameterPolygonPosition() const
{ return FParameterObjectPosition[OBJECT_POLYGON]; }
//******************************************************************************
CParameterObjectPosition* CControlerGMap::getParameterMeshPosition() const
{ return FParameterObjectPosition[OBJECT_MESH]; }
//******************************************************************************
CParameterObjectPosition* CControlerGMap::getParameterSpherePosition() const
{ return FParameterObjectPosition[OBJECT_SPHERE]; }
//******************************************************************************
CParameterObjectPosition* CControlerGMap::getParameterCylinderPosition() const
{ return FParameterObjectPosition[OBJECT_CYLINDER]; }
//******************************************************************************
CParameterObjectPosition* CControlerGMap::getParameterPyramidPosition() const
{ return FParameterObjectPosition[OBJECT_PYRAMID]; }
//******************************************************************************
CParameterObjectPosition* CControlerGMap::getParameterTorusPosition() const
{ return FParameterObjectPosition[OBJECT_TORUS]; }
//******************************************************************************
CParameterObjectPonderation* CControlerGMap::getParameterObjectPonderation() const
{ return FParameterPonderation; }
//******************************************************************************
CParameterDart* CControlerGMap::getParameterDart(TViewId AViewId) const
{ return static_cast<CParameterDart*>
    (getParameterPrecompile(AViewId, PRECOMPILE_DART));
}
//******************************************************************************
CParameterFace* CControlerGMap::getParameterFace(TViewId AViewId) const
{ return static_cast<CParameterFace*>
    (getParameterPrecompile(AViewId, PRECOMPILE_FACE));
}
//******************************************************************************
CParameterPreview* CControlerGMap::getParameterPreview(TViewId AViewId) const
{ return static_cast<CParameterPreview*>
    (getParameterPrecompile(AViewId, PRECOMPILE_PREVIEW));
}
//******************************************************************************
CParameterNormalVector* CControlerGMap::getParameterNormalVector(TViewId AViewId) const
{ return static_cast<CParameterNormalVector*>
    (getParameterPrecompile(AViewId, PRECOMPILE_NORMAL_VECTOR));
}
//******************************************************************************
CParameterObjectTransformation*
CControlerGMap::getParameterObjectTransformation(TViewId AViewId) const
{ return static_cast<CParameterObjectTransformation*>
    (getParameterPrecompile(AViewId, PRECOMPILE_OBJECT_TRANSFORMATION));
}
//******************************************************************************
CParameterSew* CControlerGMap::getParameterSew(TViewId AViewId) const
{ return static_cast<CParameterSew*>
    (getParameterPrecompile(AViewId, PRECOMPILE_SEW));
}
//******************************************************************************
#ifdef MODULE_SPAMOD
CParameterSpamod* CControlerGMap::getParameterSpamod(TViewId AViewId) const
{ return static_cast<CParameterSpamod*>
    (getParameterPrecompile(AViewId, PRECOMPILE_SPAMOD));
}
#endif // MODULE_SPAMOD
//******************************************************************************
CParameterVertex* CControlerGMap::getParameterVertex(TViewId AViewId) const
{ return static_cast<CParameterVertex*>
    (getParameterPrecompile(AViewId, PRECOMPILE_VERTEX));
}
//******************************************************************************
