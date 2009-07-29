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
// Ce fichier est une partie de "controler-gmap.hh".
//******************************************************************************
public:

//@ Méthodes permettant de récupérer un paramètre
//@{

// Les paramètres de controler : 1 par CControler
CParameterGMapVertex*        getParameterGMapVertex() const;
CParameterSelection*         getParameterSelection	  () const;
CParameterCreation*          getParameterCreation	  () const;
CParameterPolylinePosition*  getParameterPolylinePosition () const;
CParameterOperations*        getParameterOperations       () const;
CParameterObjectPosition*    getParameterObjectPosition   (int AIndex) const;
CParameterObjectPosition*    getParameterPolygonPosition  () const;
CParameterObjectPosition*    getParameterMeshPosition	  () const;
CParameterObjectPosition*    getParameterSpherePosition	  () const;
CParameterObjectPosition*    getParameterCylinderPosition () const;
CParameterObjectPosition*    getParameterPyramidPosition  () const;
CParameterObjectPosition*    getParameterTorusPosition	  () const;
//CParameterObjectTranslation* getParameterObjectTranslation() const;
//CParameterObjectRotation*    getParameterObjectRotation	  () const;
//CParameterObjectScale*       getParameterObjectScale	  () const;
CParameterObjectPonderation* getParameterObjectPonderation() const;


// Les paramètres de précompile : 1 par CPrecompile
//   mais un seul précompile de même type par CView

// Méthodes spécialisées
CParameterDart*       		getParameterDart		(TViewId AViewId) const;
CParameterFace* 		getParameterFace		(TViewId AViewId) const;
CParameterPreview* 		getParameterPreview		(TViewId AViewId) const;
CParameterNormalVector* 	getParameterNormalVector	(TViewId AViewId) const;
CParameterObjectTransformation* getParameterObjectTransformation(TViewId AViewId) const;
CParameterSew* 			getParameterSew			(TViewId AViewId) const;
#ifdef MODULE_SPAMOD
CParameterSpamod*     		getParameterSpamod		(TViewId AViewId) const;
#endif // MODULE_SPAMOD
CParameterVertex* 		getParameterVertex		(TViewId AViewId) const;

//@}

