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
#include "g-map-vertex.hh"
#include "controler.hh"
#include "view-precompile.hh"
#include <cassert>

using namespace GMap3d;


//******************************************************************************

// Fonction modulo avec des nombres flottants.
// Le resultat est toujours positif.

static float fpmod(float div,float mod)
{
  float res = fmod(div,mod);
  return (res < 0 ? res + mod : res);
}


//******************************************************************************

//effectue un produit matriciel

static CVertex multMatrixVector(const float mat[3][3], float x, float y, float z)
{  
  return CVertex(
    mat[0][0] * x + mat[0][1] * y + mat[0][2] * z,
    mat[1][0] * x + mat[1][1] * y + mat[1][2] * z,
    mat[2][0] * x + mat[2][1] * y + mat[2][2] * z);
}

//******************************************************************************

//calcule la matrice de rotation autour de l'axe axe avec un angle 'angle'

static void createRotationMatrix(float mat[3][3], float angle, const CVertex & axe)
{ 
  float cos_a = cosf(angle);
  float sin_a = sinf(angle);
  float inv_cos = 1. - cos_a;
  

  mat[0][0] = cos_a + (inv_cos * axe.getX() * axe.getX());
  mat[0][1] = (inv_cos * axe.getX() * axe.getY()) - (sin_a * axe.getZ());
  mat[0][2] = (inv_cos * axe.getX() * axe.getZ()) + (sin_a * axe.getY());
  
  mat[1][0] = (inv_cos * axe.getX() * axe.getY()) + (sin_a * axe.getZ());
  mat[1][1] = cos_a + (inv_cos * axe.getY() * axe.getY());
  mat[1][2] = (inv_cos * axe.getY() * axe.getZ()) - (sin_a * axe.getX());;
  
  mat[2][0] = (inv_cos * axe.getX() * axe.getZ()) - (sin_a * axe.getY());
  mat[2][1] = (inv_cos * axe.getY() * axe.getZ()) + (sin_a * axe.getX());
  mat[2][2] = cos_a + (inv_cos * axe.getZ() * axe.getZ());
}

//******************************************************************************

// Calcule les coordonnees de l'oeil.

void CControler::calculEyePosition(TViewId AView, CParameterEyePosition * eye_pos, CParameterAimedPosition * pt_view)
{
  float alpha_rad = rad(fpmod(eye_pos -> getRotationAlpha() - 90.0, 360));
  float beta_rad  = rad(fpmod(eye_pos -> getRotationBeta()  + 90.0, 360));
  float distance  = eye_pos -> getDistance();
  
  eye_coord.setX(pt_view -> getLookAt(0) + distance * cosf(alpha_rad) * sinf(beta_rad));
  eye_coord.setY(pt_view -> getLookAt(1) + distance * sinf(alpha_rad) * sinf(beta_rad));
  eye_coord.setZ(pt_view -> getLookAt(2) + distance * cosf(beta_rad));
}

//******************************************************************************

// Calcule les coordonnees de l'oeil.

CVertex CControler::calculEyePosition(TViewId AView)
{
  CParameterEyePosition * eye_pos = getParameterEyePosition(AView);  
  CParameterAimedPosition * pt_view = getParameterAimedPosition(AView);
  calculEyePosition(AView,eye_pos,pt_view);
  return CVertex(eye_coord);
}



//******************************************************************************

// Retourne un accesseur sur les coordonnees de l'oeil
// precedement calculees avec les fonctions horizontalRotationEye(),
// verticalRotationEye(), moveEye() ou moveEyeLateral().

const CVertex & CControler::getEyePosition() const
{
  return eye_coord;
}


//******************************************************************************
//                             Operations 3D
//******************************************************************************

// Effectue une rotation horizontale de la camera de 'alpha' degres.

void CControler::horizontalRotationEye(TViewId AView,bool positive,float alpha)
{
  CParameterAimedPosition * pt_view = getParameterAimedPosition(AView);  
  CParameterEyePosition * eye_pos = getParameterEyePosition(AView);  
  calculEyePosition(AView,eye_pos,pt_view);
  
  if (alpha == 0) alpha = eye_pos -> getAngleRotation();
  if (!positive) alpha = -alpha;
  
  // on ramene la camera à l'origine
  CVertex cam_y(pt_view -> getLookAt(0) - eye_coord.getX(),
                pt_view -> getLookAt(1) - eye_coord.getY(),
                pt_view -> getLookAt(2) - eye_coord.getZ());

  // on tourne autour de l'axe Oz
  float rotation_mat[3][3];
  createRotationMatrix(rotation_mat, rad(alpha), OZ);
  CVertex coord_pt_view = multMatrixVector(rotation_mat, cam_y.getX(),cam_y.getY(),cam_y.getZ() );

  // on remet la camera a son emplacement
  coord_pt_view += eye_coord;
  pt_view -> setLookAt(0,coord_pt_view.getX());
  pt_view -> setLookAt(1,coord_pt_view.getY());
  pt_view -> setLookAt(2,coord_pt_view.getZ());

  // on fait tourner la camera autour de la cible pour la remettre dans l'axe de visee original
  eye_pos -> setRotationAlpha(fpmod(eye_pos -> getRotationAlpha() + alpha,360));
}




// Effectue une rotation verticale de la camera de 'beta' degres.

void CControler::verticalRotationEye(TViewId AView,bool positive,float beta)
{  
  CParameterAimedPosition * pt_view = getParameterAimedPosition(AView);  
  CParameterEyePosition * eye_pos = getParameterEyePosition(AView);  
  calculEyePosition(AView,eye_pos,pt_view);
  
  if (beta == 0) beta = eye_pos -> getAngleRotation();
  if (!positive) beta = -beta;
  
    //on ramene la camera à l'origine
  CVertex cam_y(pt_view -> getLookAt(0) - eye_coord.getX(),
                pt_view -> getLookAt(1) - eye_coord.getY(),
                pt_view -> getLookAt(2) - eye_coord.getZ());

  //on tourne autour de l'axe normale a la vue et a la verticale
  CVertex axe_de_rotation = OZ * cam_y;
  axe_de_rotation.normalize();

  float rotation_mat[3][3];
  createRotationMatrix(rotation_mat, rad(-beta), axe_de_rotation);
  CVertex coord_pt_view = multMatrixVector(rotation_mat, cam_y.getX(),cam_y.getY(),cam_y.getZ() );

//on remet la camera a son emplacement
  coord_pt_view += eye_coord;
  pt_view -> setLookAt(0,coord_pt_view.getX());
  pt_view -> setLookAt(1,coord_pt_view.getY());
  pt_view -> setLookAt(2,coord_pt_view.getZ());

//on fait tourner la camera autour de la cible pour la remttre dans l'axe de visee original
  eye_pos -> setRotationBeta(fpmod(eye_pos -> getRotationBeta() + beta,360));
}




// Deplace la camera dans la direction de visee.
// 'coeff' indique la longueur du deplacement :
// Un nombre positif fait avancer, un nomber negatif fait reculer.

void CControler::moveEye(TViewId AView,bool positive,float coeff)
{
  CParameterEyePosition * eye_pos = getParameterEyePosition(AView);  
  CParameterAimedPosition * pt_view = getParameterAimedPosition(AView);
  calculEyePosition(AView,eye_pos,pt_view);
  
  if (coeff == 0) coeff = eye_pos -> getPasAvancement();
  if (!positive) coeff = -coeff;
  
  CVertex direction(pt_view -> getLookAt(0) - eye_coord.getX(),
                    pt_view -> getLookAt(1) - eye_coord.getY(),
                    pt_view -> getLookAt(2) - eye_coord.getZ());
                    
  direction.normalize();
  direction *= coeff;
  eye_coord += direction;
  
  pt_view -> setLookAt(0,pt_view -> getLookAt(0) + direction.getX());
  pt_view -> setLookAt(1,pt_view -> getLookAt(1) + direction.getY());
  pt_view -> setLookAt(2,pt_view -> getLookAt(2) + direction.getZ());
}

//******************************************************************************
//Deplacement lateral de la camera :
// 'coeff' indique la longueur du deplacement :
// Un nombre positif fait avancer sur la droite, un nomber negatif sur la gauche.
void CControler::moveEyeLateral(TViewId AView,bool positive,float coeff)
{
  CParameterEyePosition * eye_pos = getParameterEyePosition(AView);  
  CParameterAimedPosition * pt_view = getParameterAimedPosition(AView);
  calculEyePosition(AView,eye_pos,pt_view);
  
  if (coeff == 0) coeff = eye_pos -> getPasAvancement();
  if (!positive) coeff = -coeff;
  
  CVertex avant(pt_view -> getLookAt(0) - eye_coord.getX(),
                    pt_view -> getLookAt(1) - eye_coord.getY(),
                    pt_view -> getLookAt(2) - eye_coord.getZ());
  
  CVertex direction=avant*OZ;
  direction.normalize();
  direction *= coeff;
  eye_coord += direction;

  pt_view -> setLookAt(0,pt_view -> getLookAt(0) + direction.getX());
  pt_view -> setLookAt(1,pt_view -> getLookAt(1) + direction.getY());
  pt_view -> setLookAt(2,pt_view -> getLookAt(2) + direction.getZ());
}


//******************************************************************************
//                             Operations 2D
//******************************************************************************

void CControler::moveEyeX(TViewId AView,bool positive,float coeff)
{
  CParameterAimedPosition * pt_view = getParameterAimedPosition(AView);
  if (coeff == 0) coeff = getParameterEyePosition(AView) -> getPasAvancement();
  if (!positive) coeff = -coeff;  
  pt_view -> setLookAt(0,pt_view -> getLookAt(0) + coeff);
}


void CControler::moveEyeY(TViewId AView,bool positive,float coeff)
{
  CParameterAimedPosition * pt_view = getParameterAimedPosition(AView);
  if (coeff == 0) coeff = getParameterEyePosition(AView) -> getPasAvancement();
  if (!positive) coeff = -coeff;
  pt_view -> setLookAt(1,pt_view -> getLookAt(1) + coeff);
}



void CControler::moveEyeZ(TViewId AView,bool positive,float coeff)
{
  CParameterAimedPosition * pt_view = getParameterAimedPosition(AView);
  if (coeff == 0) coeff = getParameterEyePosition(AView) -> getPasAvancement();
  if (!positive) coeff = -coeff;
  pt_view -> setLookAt(2,pt_view -> getLookAt(2) + coeff);
}

