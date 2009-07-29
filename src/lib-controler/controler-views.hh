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
virtual void viewUpdate(TViewId AViewId);
void         viewInit  (TViewId AViewId);
void         viewDraw  (TViewId AViewId);

TViewId viewCreate     (TView AViewType);
TViewId viewCreateShare(TView AViewType, TViewId AView);
void    viewDestroy    (TViewId AViewId);

void viewAddPrecompile        (TViewId AViewId, TPrecompile APrecompileType);
void viewRemovePrecompile     (TViewId AViewId, TPrecompile APrecompileType);
void viewAddOrRemovePrecompile(TViewId AViewId, TPrecompile APrecompileType);

bool viewIsEnabledPrecompile(TViewId AViewId, TPrecompile APrecom);
void viewEnablePrecompile   (TViewId AViewId, TPrecompile APrecom);
void viewDisablePrecompile  (TViewId AViewId, TPrecompile APrecom);
void viewTogglePrecompile   (TViewId AViewId, TPrecompile APrecom);

void viewEnablePreviewPrecompile (TViewId AViewId);
void viewDisablePreviewPrecompile(TViewId AViewId);

void viewEnableObjectTransformationPrecompile (TViewId AViewId);
void viewDisableObjectTransformationPrecompile(TViewId AViewId);

/// Groupe tout les précompiles et les paramètres de AViewId1 et AViewId2
void viewGroup(TViewId AViewId1, TViewId AViewId2);

/// Groupe tout les précompiles et les paramètres de toue les vues avec AViewId
void viewGroupAll(TViewId AViewId);

/// Décroupe les précompiles et les paramètres de AViewId
///      (ceux non groupés sont inchangés)
void viewUngroup(TViewId AViewId);

/// Décroupe tout les précompiles et les paramètres de toute les vues
void viewUngroupAll();

/// Groupe tout les précompiles de AViewId1 et AViewId2
void viewGroupPrecompiles(TViewId AViewId1, TViewId AViewId2);

/// Groupe tout les précompiles de toute les vues avec AViewId
void viewGroupAllPrecompiles(TViewId AViewId);

/// Décroupe tout les précompiles de AViewId
void viewUngroupPrecompiles(TViewId AViewId);

/// Décroupe tout les précompiles de toute les vues
void viewUngroupAllPrecompiles();

/// Le précompile de AViewId2 est supprimé (s'il existe).
/// Le précompile de AViewId1 est partagé.
void viewGroupPrecompile(TViewId AViewId1, TViewId AViewId2,
			 TPrecompile APrecompileType);

/// Le précompile de AViewId est dégroupé (s'il existe).
void viewUngroupPrecompile(TViewId AViewId, TPrecompile APrecompileType);

/// Le paramètre de AViewId2 est supprimé (s'il existe).
/// Le paramètre de AViewId1 est partagé.
void viewGroupParameter(TViewId AViewId1, TViewId AViewId2,
			TParameter AParameterType);

/// Les paramètres AParameterType de toute les vues sont regroupés.
void viewGroupAllParameter(TViewId AViewId, TParameter AParameterType);

/// Le paramètre de AViewId est dégroupé (s'il existe).
void viewUngroupParameter(TViewId AViewId, TParameter AParameterType);

/// Les paramètres AParameterType de toute les vues sont dégroupés.
void viewUngroupAllParameter(TParameter AParameterType);

void viewEnable	  (TViewId AViewId);
void viewDisable  (TViewId AViewId);
bool viewIsEnabled(TViewId AViewId);

protected:
std::vector<CViewPrecompile *> FViews;
int                            FLastPrecompile;
//******************************************************************************
