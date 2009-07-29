/*
 * lib-mesh : Opérations de maillage et lissage.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-mesh
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
#ifndef INTER_SMOOTH_DIVER_HH
#define INTER_SMOOTH_DIVER_HH
//******************************************************************************
#include "mesh-diver.hh"
//******************************************************************************
class CTransformationMatrix;
//******************************************************************************
/**
 * @name CMesh-InterpolationDiver
 *
 * Classes pour le calcul des plongements des maillages par interpolation
 * (linéaire ou autre).
 *
 * Ces classes mémorisent les bords des objets à mailler.
 * En dimension 1: Les extrémités de l'arête (le plongement de 2 sommets),
 * En dimension 2: Les bords de la grille (le plongement de 4 polylignes),
 * En dimension 3: Les bords du maillage 3d (le plongement de 6 grilles).
 */
// @{
//------------------------------------------------------------------------------
class CMesh1InterpolationDiver : public CMesh1Diver
{
protected:
  CVertex FVertex0, FVertex1;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre d'arêtes du maillage 1d
   * @param AVertex0 Le premier sommet du maillage
   * @param AVertex1 Le dernier sommet du maillage
   */
  CMesh1InterpolationDiver(int ASx,
			   const CVertex& AVertex0,
			   const CVertex& AVertex1);

  /// Destructeur
  virtual ~CMesh1InterpolationDiver();

  // @}
};
//------------------------------------------------------------------------------
class CMesh2InterpolationDiver : public CMesh2Diver
{
protected:
  const CVertex ** FBottomEdge; /* FSx + 1 */
  const CVertex ** FTopEdge   ; /* FSx + 1 */
  const CVertex ** FLeftEdge  ; /* FSy + 1 */
  const CVertex ** FRightEdge ; /* FSy + 1 */

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre de subdivisions du maillage selon la 1ère dimension
   * @param ASy Le nombre de subdivisions du maillage selon la 2ème dimension
   * @param ABottomEdge Un tableau de ASx+1 pointeurs sur sommets correspondant
   *   aux sommets de l'arête du bas    (y<0)
   * @param ATopEdge    Un tableau de ASx+1 pointeurs sur sommets correspondant
   *   aux sommets de l'arête du haut   (y>0)
   * @param ALeftEdge   Un tableau de ASy+1 pointeurs sur sommets correspondant
   *   aux sommets de l'arête de gauche (x<0)
   * @param ARightEdge  Un tableau de ASy+1 pointeurs sur sommets correspondant
   *   aux sommets de l'arête de droite (x>0)
   *
   * @precondition ASx>0
   * @precondition ASy>0
   * @precondition ABottomEdge[ 0 ] == ALeftEdge [ 0 ];
   * @precondition ABottomEdge[ASx] == ARightEdge[ 0 ];
   * @precondition ATopEdge   [ 0 ] == ALeftEdge [ASy];
   * @precondition ATopEdge   [ASx] == ARightEdge[ASy];
   */
  CMesh2InterpolationDiver(int ASx, int ASy,
			   const CVertex** ABottomEdge /* ASx+1 */,
			   const CVertex** ATopEdge    /* ASx+1 */,
			   const CVertex** ALeftEdge   /* ASy+1 */,
			   const CVertex** ARightEdge  /* ASy+1 */);

  /// Destructeur
  virtual ~CMesh2InterpolationDiver();

  // @}
};
//------------------------------------------------------------------------------
class CMesh3InterpolationDiver : public CMesh3Diver
{
protected:
  const CVertex *** FLeftFace  ; /* (FSy+1)*(FSz+1) */
  const CVertex *** FRightFace ; /* (FSy+1)*(FSz+1) */
  const CVertex *** FFrontFace ; /* (FSz+1)*(FSx+1) */
  const CVertex *** FBackFace  ; /* (FSz+1)*(FSx+1) */
  const CVertex *** FBottomFace; /* (FSx+1)*(FSy+1) */
  const CVertex *** FTopFace   ; /* (FSx+1)*(FSy+1) */

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre de subdivisions du maillage selon la 1ère dimension
   * @param ASy Le nombre de subdivisions du maillage selon la 2ème dimension
   * @param ASz Le nombre de subdivisions du maillage selon la 3ème dimension
   * @param FLeftFace   Un tableau de (FSy+1)*(FSz+1) pointeurs sur sommets
   *   correspondant aux sommets de la face de gauche   (x<0)
   * @param FRightFace  Un tableau de (FSy+1)*(FSz+1) pointeurs sur sommets
   *   correspondant aux sommets de la face de droite   (x>0)
   * @param FFrontFace  Un tableau de (FSz+1)*(FSx+1) pointeurs sur sommets
   *   correspondant aux sommets de la face de devant   (y<0)
   * @param FBackFace   Un tableau de (FSz+1)*(FSx+1) pointeurs sur sommets
   *   correspondant aux sommets de la face de derrière (y>0)
   * @param FBottomFace Un tableau de (FSx+1)*(FSy+1) pointeurs sur sommets
   *   correspondant aux sommets de la face du bas      (z<0)
   * @param FTopFace    Un tableau de (FSx+1)*(FSy+1) pointeurs sur sommets
   *   correspondant aux sommets de la face du haut     (z>0)
   *
   * @precondition ASx>0
   * @precondition ASy>0
   * @precondition ASz>0

   * @precondition Quels que soient i, j et k tels que
   * @precondition 0 <= i <= ASx, 0 <= j <= ASy et 0 <= k <= ASz, on alpha:
   *
   * @precondition ABottomFace[ i ][ 0 ] == AFrontFace[ 0 ][ i ]
   * @precondition ABottomFace[ i ][ASy] == ABackFace [ 0 ][ i ]
   * @precondition ATopFace   [ i ][ 0 ] == AFrontFace[ASz][ i ]
   * @precondition ATopFace   [ i ][ASy] == ABackFace [ASz][ i ]
   *
   * @precondition ABottomFace[ 0 ][ j ] == ALeftFace [ j ][ 0 ]
   * @precondition ABottomFace[ASx][ j ] == ARightFace[ j ][ 0 ]
   * @precondition ATopFace   [ 0 ][ j ] == ALeftFace [ j ][ASz]
   * @precondition ATopFace   [ASx][ j ] == ARightFace[ j ][ASz]
   *
   * @precondition AFrontFace [ k ][ 0 ] == ALeftFace [ 0 ][ k ]
   * @precondition AFrontFace [ k ][ASx] == ARightFace[ 0 ][ k ]
   * @precondition ABackFace  [ k ][ 0 ] == ALeftFace [ASy][ k ]
   * @precondition ABackFace  [ k ][ASx] == ARightFace[ASy][ k ]
   */
  CMesh3InterpolationDiver(int ASx, int ASy, int ASz,
			   const CVertex*** ALeftFace   /* (ASy+1)*(ASz+1) */,
			   const CVertex*** ARightFace  /* (ASy+1)*(ASz+1) */,
			   const CVertex*** AFrontFace  /* (ASx+1)*(ASz+1) */,
			   const CVertex*** ABackFace   /* (ASx+1)*(ASz+1) */,
			   const CVertex*** ABottomFace /* (ASx+1)*(ASy+1) */,
			   const CVertex*** ATopFace    /* (ASx+1)*(ASy+1) */);

  /// Destructeur
  virtual ~CMesh3InterpolationDiver();

  // @}
};

// @}
//******************************************************************************
/**
 * @name CMesh-LinearDiver
 *
 * Classes pour le calcul des plongements des maillages par interpolation
 * linéaire.
 */
// @{
//******************************************************************************
class CMesh1LinearDiver : public CMesh1InterpolationDiver
{
public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre d'arêtes du maillage 1d
   * @param AVertex0 Le premier sommet du maillage
   * @param AVertex1 Le dernier sommet du maillage
   */
  CMesh1LinearDiver(int ASx,
		    const CVertex& AVertex0,
		    const CVertex& AVertex1);

  /// Destructeur
  virtual ~CMesh1LinearDiver();

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /// Calcule le plongement du sommet courant du maillage.
  virtual CVertex computeCurrentVertex() const;

  // @}
};
//------------------------------------------------------------------------------
class CMesh2LinearDiver : public CMesh2InterpolationDiver
{
private:
  const CVertex * FP00, * FP01, * FP10, * FP11;
  const CVertex * FPi0, * FP0j, * FP1j, * FPi1;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre de subdivisions du maillage selon la 1ère dimension
   * @param ASy Le nombre de subdivisions du maillage selon la 2ème dimension
   * @param ABottomEdge Un tableau de ASx+1 pointeurs sur sommets correspondant
   *   aux sommets de l'arête du bas    (y<0)
   * @param ATopEdge    Un tableau de ASx+1 pointeurs sur sommets correspondant
   *   aux sommets de l'arête du haut   (y>0)
   * @param ALeftEdge   Un tableau de ASy+1 pointeurs sur sommets correspondant
   *   aux sommets de l'arête de gauche (x<0)
   * @param ARightEdge  Un tableau de ASy+1 pointeurs sur sommets correspondant
   *   aux sommets de l'arête de droite (x>0)
   *
   * @precondition ASx>0
   * @precondition ASy>0
   * @precondition ABottomEdge[ 0 ] == ALeftEdge [ 0 ];
   * @precondition ABottomEdge[ASx] == ARightEdge[ 0 ];
   * @precondition ATopEdge   [ 0 ] == ALeftEdge [ASy];
   * @precondition ATopEdge   [ASx] == ARightEdge[ASy];
   */
  CMesh2LinearDiver(int ASx, int ASy,
		    const CVertex ** ABottomEdge /* ASx+1 */,
		    const CVertex ** ATopEdge    /* ASx+1 */,
		    const CVertex ** ALeftEdge   /* ASy+1 */,
		    const CVertex ** ARightEdge  /* ASy+1 */);

  /// Destructeur
  virtual ~CMesh2LinearDiver();

  // @}
  // @name Accesseurs
  // @{

  /**
   * @param La position courante sur le maillage selon la 1ère dimension.
   * @precondition 0 <= AIz <= getSz()
   */
  virtual void setIx(int AIx);

  /**
   * @param La position courante sur le maillage selon la 2ème dimension.
   * @precondition 0 <= AIy <= getSy()
   */
  virtual void setIy(int AIy);

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /// Calcule le plongement du sommet courant du maillage.
  virtual CVertex computeCurrentVertex() const;

  // @}
};
//------------------------------------------------------------------------------
class CMesh3LinearDiver : public CMesh3InterpolationDiver
{
private:
  const CVertex * FP000, * FP001, * FP010, * FP011;
  const CVertex * FP100, * FP101, * FP110, * FP111;

  const CVertex * FPi00, * FP0j0, * FP00k;
  const CVertex * FPi11, * FP1j1, * FP11k;

  const CVertex * FPi10, * FP0j1, * FP10k;
  const CVertex * FPi01, * FP1j0, * FP01k;

  const CVertex * FP0jk, * FPi0k, * FPij0;
  const CVertex * FP1jk, * FPi1k, * FPij1;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre de subdivisions du maillage selon la 1ère dimension
   * @param ASy Le nombre de subdivisions du maillage selon la 2ème dimension
   * @param ASz Le nombre de subdivisions du maillage selon la 3ème dimension
   * @param FLeftFace   Un tableau de (FSy+1)*(FSz+1) pointeurs sur sommets
   *   correspondant aux sommets de la face de gauche   (x<0)
   * @param FRightFace  Un tableau de (FSy+1)*(FSz+1) pointeurs sur sommets
   *   correspondant aux sommets de la face de droite   (x>0)
   * @param FFrontFace  Un tableau de (FSz+1)*(FSx+1) pointeurs sur sommets
   *   correspondant aux sommets de la face de devant   (y<0)
   * @param FBackFace   Un tableau de (FSz+1)*(FSx+1) pointeurs sur sommets
   *   correspondant aux sommets de la face de derrière (y>0)
   * @param FBottomFace Un tableau de (FSx+1)*(FSy+1) pointeurs sur sommets
   *   correspondant aux sommets de la face du bas      (z<0)
   * @param FTopFace    Un tableau de (FSx+1)*(FSy+1) pointeurs sur sommets
   *   correspondant aux sommets de la face du haut     (z>0)
   *
   * @precondition ASx>0
   * @precondition ASy>0
   * @precondition ASz>0
   *
   * @precondition Quels que soient i, j et k tels que
   * @precondition 0 <= i <= ASx, 0 <= j <= ASy et 0 <= k <= ASz, on alpha:
   *
   * @precondition ABottomFace[ i ][ 0 ] == AFrontFace[ 0 ][ i ]
   * @precondition ABottomFace[ i ][ASy] == ABackFace [ 0 ][ i ]
   * @precondition ATopFace   [ i ][ 0 ] == AFrontFace[ASz][ i ]
   * @precondition ATopFace   [ i ][ASy] == ABackFace [ASz][ i ]
   *
   * @precondition ABottomFace[ 0 ][ j ] == ALeftFace [ j ][ 0 ]
   * @precondition ABottomFace[ASx][ j ] == ARightFace[ j ][ 0 ]
   * @precondition ATopFace   [ 0 ][ j ] == ALeftFace [ j ][ASz]
   * @precondition ATopFace   [ASx][ j ] == ARightFace[ j ][ASz]
   *
   * @precondition AFrontFace [ k ][ 0 ] == ALeftFace [ 0 ][ k ]
   * @precondition AFrontFace [ k ][ASx] == ARightFace[ 0 ][ k ]
   * @precondition ABackFace  [ k ][ 0 ] == ALeftFace [ASy][ k ]
   * @precondition ABackFace  [ k ][ASx] == ARightFace[ASy][ k ]
   */
  CMesh3LinearDiver(int ASx, int ASy, int ASz,
		    const CVertex*** ALeftFace   /* (ASy+1)*(ASz+1) */,
		    const CVertex*** ARightFace  /* (ASy+1)*(ASz+1) */,
		    const CVertex*** AFrontFace  /* (ASx+1)*(ASz+1) */,
		    const CVertex*** ABackFace   /* (ASx+1)*(ASz+1) */,
		    const CVertex*** ABottomFace /* (ASx+1)*(ASy+1) */,
		    const CVertex*** ATopFace    /* (ASx+1)*(ASy+1) */);

  /// Destructeur
  virtual ~CMesh3LinearDiver();

  // @}
  // @name Accesseurs
  // @{

  /**
   * @param La position courante sur le maillage selon la 1ère dimension.
   * @precondition 0 <= AIz <= getSz()
   */
  virtual void setIx(int AIx);

  /**
   * @param La position courante sur le maillage selon la 2ème dimension.
   * @precondition 0 <= AIy <= getSy()
   */
  virtual void setIy(int AIy);

  /**
   * @param La position courante sur le maillage selon la 3ème dimension.
   * @precondition 0 <= AIz <= getSz()
   */
  virtual void setIz(int AIz);

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /// Calcule le plongement du sommet courant du maillage.
  virtual CVertex computeCurrentVertex() const;

  // @}
};

// @}
//******************************************************************************
/**
 * @name CMesh-SmoothingDiver
 *
 * Classes pour le calcul des plongements des maillages par lissage
 * (Hermite, Bézier, ...).
 */
// @{
//******************************************************************************
class CMesh1SmoothingDiver : public CMesh1Diver
{
protected:
  CVector FTX;
  CVector FControl[3];

public:
  // @name Constructeur, initialisateur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre d'arêtes du maillage 1d
   * @param AMethod Une matrice 4*4 (HERMITE, BEZIER, ...)
   * @param AControlPoints Un tableau de 4 points de contrôle
   */
  CMesh1SmoothingDiver(int ASx,
		       const CTransformationMatrix & AMethod,
		       const CVertex * AControlPoints);

  /// Destructeur
  virtual ~CMesh1SmoothingDiver();

  // @}
  // @name Accesseurs
  // @{

  /**
   * @param La position courante sur le maillage selon la 1ère dimension.
   * @precondition 0 <= AIx <= getSx()
   */
  virtual void setIx(int AIx);

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /// Calcule le plongement du sommet courant du maillage.
  /// Le sommet courant est positionné à l'aide de la méthode 'setIx'.
  virtual CVertex computeCurrentVertex() const;

  // @}
};
//------------------------------------------------------------------------------
class CMesh2SmoothingDiver : public CMesh2Diver
{
protected:
  const CTransformationMatrix * FMethod;

  CMesh1SmoothingDiver * FCurbsY[4];
  CMesh1SmoothingDiver * FCurbX;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre de subdivisions selon la première dimension
   * @param ASy Le nombre de subdivisions selon la deuxième dimension
   * @param AMethod Une matrice 4*4 (HERMITE, BEZIER, ...)
   * @param AControlPoints Un tableau de 4*4 points de contrôle
   */
  CMesh2SmoothingDiver(int ASx, int ASy,
		       const CTransformationMatrix& AMethod,
		       const CVertex** AControlPoints);

  /// Destructeur
  virtual ~CMesh2SmoothingDiver();

  // @}
  // @name Accesseurs
  // @{

  /**
   * @param La position courante sur le maillage selon la 1ère dimension.
   * @precondition 0 <= AIx <= getSx()
   */
  virtual void setIx(int AIx);

  /**
   * @param La position courante sur le maillage selon la 2ème dimension.
   * @precondition 0 <= AIy <= getSy()
   */
  virtual void setIy(int AIy);

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /// Calcule le plongement du sommet courant du maillage.
  /// Le sommet courant est positionné à l'aide des méthodes 'setIx' et 'setIy'.
  virtual CVertex computeCurrentVertex() const;

  // @}
};
//------------------------------------------------------------------------------
class CMesh3SmoothingDiver : public CMesh3Diver
{
protected:
  const CTransformationMatrix * FMethod;

  CMesh2SmoothingDiver * FSurfacesYZ[4];
  CMesh1SmoothingDiver * FCurbX;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre de subdivisions selon la première dimension
   * @param ASy Le nombre de subdivisions selon la deuxième dimension
   * @param ASz Le nombre de subdivisions selon la deuxième dimension
   * @param AMethod Une matrice 4*4 (HERMITE, BEZIER, ...)
   * @param AControlPoints Un tableau de 4*4*4 points de contrôle
   */
  CMesh3SmoothingDiver(int ASx, int ASy, int ASz,
		       const CTransformationMatrix& AMethod,
		       const CVertex*** AControlPoints);

  /// Destructeur
  virtual ~CMesh3SmoothingDiver();

  // @}
  // @name Accesseurs
  // @{

  /**
   * @param La position courante sur le maillage selon la 1ère dimension.
   * @precondition 0 <= AIx <= getSx()
   */
  virtual void setIx(int AIx);

  /**
   * @param La position courante sur le maillage selon la 2ème dimension.
   * @precondition 0 <= AIy <= getSy()
   */
  virtual void setIy(int AIy);

  /**
   * @param La position courante sur le maillage selon la 3ème dimension.
   * @precondition 0 <= AIz <= getSz()
   */
  virtual void setIz(int AIz);

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /// Calcule le plongement du sommet courant du maillage.
  /// Le sommet courant est positionné à l'aide des méthodes 'setIx', 'setIy' et
  /// 'setIy'.
  virtual CVertex computeCurrentVertex() const;

  // @}
};
//******************************************************************************
#include INCLUDE_INLINE("inter-smooth-diver.icc")
//******************************************************************************
#endif // INTER_SMOOTH_DIVER_HH
//******************************************************************************
