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
// Ce fichier est une partie de "controler.hh".
//******************************************************************************
public:

//@ Méthodes permettant de récupérer un paramètre
//@{

// Les paramètres de view : 1 par CView
CParameterAimedPosition* getParameterAimedPosition(TViewId AViewId) const;
CParameterDrawing*       getParameterDrawing	  (TViewId AViewId) const;
CParameterEyePosition*   getParameterEyePosition  (TViewId AViewId) const;

// Les paramètres de précompile : 1 par CPrecompile
//   mais un seul précompile de même type par CView

// Méthode "générique"
CParameter* getParameterPrecompile(TViewId AViewId, TPrecompile APrecompile) const;

// Méthodes spécialisées
CParameterAimedPoint* getParameterAimedPoint(TViewId AViewId) const;
CParameterAxis*       getParameterAxis      (TViewId AViewId) const;
CParameterGrid*       getParameterGrid      (TViewId AViewId) const;

//@}

//@ Méthodes permettant de sauver / lire les paramètres
//@{
void saveAllParameters	(TViewId AViewId = 0);
void loadAllParameters	(TViewId AViewId = 0);
void reinitAllParameters(TViewId AViewId = 0);

private:
void saveOnePrecompileParameter(TViewId, TPrecompile, std::ostream&);
void loadOnePrecompileParameter(TViewId, TPrecompile, std::istream&);

//@}
