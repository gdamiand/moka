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

//@{ Pour la translation
void    setObjectTranslationVector(const CVertex & AVector);
CVertex getObjectTranslationVector() const;

void setObjectTranslationPonderation(bool AActive);
bool getObjectTranslationPonderation() const;
void toggleObjectTranslationPonderation();
//@}

//@{ Pour la rotation
void    setObjectRotationAxeVertex(const CVertex & AAxeVertex);
CVertex getObjectRotationAxeVertex() const;

void    setObjectRotationAxeVector(const CVertex & AAxeVector);
CVertex getObjectRotationAxeVector() const;

void  setObjectRotationAngle(float AAngle);
float getObjectRotationAngle() const;

void setObjectRotationPonderation(bool AActive);
bool getObjectRotationPonderation() const;
void toggleObjectRotationPonderation();
//@}

//@{ Pour le scaling
void       setObjectScaleType(TScaleType AScaleType);
TScaleType getObjectScaleType() const;

void    setObjectScaleCenter(const CVertex & ACenter);
CVertex getObjectScaleCenter() const;

void    setObjectScaleVector(const CVertex & AVector);
CVertex getObjectScaleVector() const;

void setObjectScaleIso(bool AValue);
bool getObjectScaleIso() const;
void toggleObjectScaleIso();

void  setObjectScaleIsoCoef(float AValue);
float getObjectScaleIsoCoef() const;

void    setObjectScaleCoef(const CVertex & AValue);
CVertex getObjectScaleCoef() const;

void  setObjectScaleCoef(int ADim, float AValue);
float getObjectScaleCoef(int ADim) const;

void setObjectScalePonderation(bool AActive);
bool getObjectScalePonderation() const;
void toggleObjectScalePonderation();

CVertex getVertexPosition() const;
void    setVertexPosition(const CVertex& AVertex);

//@}

//******************************************************************************

private:

//@name Méthodes pour gérer la pondération
//@{
void addPonderationCoefs();
void delPonderationCoefs();
void updatePonderationCoefs();
//@}

//@name Méthodes permettant de changer de mode
//@{
void modeObjectTransformationBegin(bool APonderation);
void modeTranslationBegin();
void modeRotationBegin();
void modeScaleBegin();

void modeObjectTransformationEnd(bool APonderation);
void modeTranslationEnd();
void modeRotationEnd();
void modeScaleEnd();

//@}

//@name Méthodes pour la gestion de la souris
//@{

/** mode---OperationStart
 *
 *  Méthode appelée lorsqu'une opération débute
 *  (par exemple lorsqu'on appuie sur un bouton de la souris)
 *
 */
void modeTranslationOperationStart();
void modeRotationOperationStart();
void modeScaleOperationStart();

/** mode---OperationMove
 *
 *  Méthode appelée pour une opération en cours
 *  (par exemple lorsque la souris bouge)
 *
 */
void modeTranslationOperationMove();
void modeRotationOperationMove();
void modeScaleOperationMove();

/** mode---OperationStop
 *
 *  Méthode appelée lorsqu'une une opération est terminée
 *  (par exemple lorsqu'un bouton de la souris est relâché)
 *
 */
// Regroupe mode{Translation,Rotation,Scale}OperationStop :
void modeTransformationOperationStop();

//@name Champs privés pour les déformations (translation, rotation, homothétie)
//@{

int FDeformationMark;
int FDirectInfoVertex;
int FDirectInfoPonderation;

bool FDeformationDone;

//@}

//******************************************************************************
