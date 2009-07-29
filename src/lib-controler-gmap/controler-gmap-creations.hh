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

//@ Opérations liées à la création
//@{

void modeCreationReinitPosition();

//@}

//******************************************************************************
//@ Méthodes liées au mode création
//@{

private:

//@name Méthodes permettant de changer de mode
//@{
void modeCreationBegin();

void modeCreationEnd();
//@}

//@name Méthodes pour la gestion de la souris
//@{

/** mode---OperationStart
 *
 *  Méthode appelée lorsqu'une opération débute
 *  (par exemple lorsqu'on appuie sur un bouton de la souris)
 *
 */
void modeCreationPolylineOperationStart();
void modeCreationOperationStart();

/** mode---OperationMove
 *
 *  Méthode appelée pour une opération en cours
 *  (par exemple lorsque la souris bouge)
 *
 */
void modeCreationPolylineOperationMove();
void modeCreationOperationMove();

/** mode---OperationStop
 *
 *  Méthode appelée lorsqu'une une opération est terminée
 *  (par exemple lorsqu'un bouton de la souris est relâché)
 *
 */
void modeCreationPolylineOperationStop();
void modeCreationOperationStop();
//@}

//@}

//******************************************************************************
