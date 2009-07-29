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
// Ce fichier fait partie de "controler-operations.hh".
//******************************************************************************

private:
  
CVertex eye_coord;  
  
void calculEyePosition(TViewId AView, CParameterEyePosition * eye_pos,
		       CParameterAimedPosition * pt_view);

public:
  
// Calcule les coordonnees de l'oeil.
CVertex calculEyePosition(TViewId AView);


// Retourne un accesseur sur les coordonnees de l'oeil
// precedement calculees avec les fonctions horizontalRotationEye(),
// verticalRotationEye(), moveEye() ou moveEyeLateral().
const CVertex & getEyePosition() const;


// -------------------------------------
//            Operations 3D
// -------------------------------------

// Effectue une rotation horizontale de la camera de 'alpha' degres.
void horizontalRotationEye(TViewId AView,bool positive = true,float alpha = 0);

// Effectue une rotation veticale de la camera de 'beta' degres.
void verticalRotationEye(TViewId AView,bool positive = true,float beta = 0);

// Deplace la camera dans la direction de visee.
// 'coeff' indique la longueur du deplacement :
// Un nombre positif fait avancer, un nomber negatif fait reculer.
void moveEye(TViewId AView,bool positive = true,float coeff = 0);

// Deplacement latéral de la caméra :
// 'coeff' indique la longueur du deplacement :
// Un nombre positif fait avancer sur la droite, un nomber negatif sur la gauche.
void moveEyeLateral(TViewId AView,bool positive = true,float coeff = 0);


// -------------------------------------
//            Operations 2D
// -------------------------------------

// Deplace le point de vue selon un axe.
void moveEyeX(TViewId AView,bool positive = true,float coeff = 0);
void moveEyeY(TViewId AView,bool positive = true,float coeff = 0);
void moveEyeZ(TViewId AView,bool positive = true,float coeff = 0);


//******************************************************************************
