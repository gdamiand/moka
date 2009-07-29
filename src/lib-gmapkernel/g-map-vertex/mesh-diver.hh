/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-gmapkernel
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
#ifndef MESH_DIVER_HH
#define MESH_DIVER_HH
//******************************************************************************
#include <vector>
#include "vector.hh"
#include "vertex.hh"
//******************************************************************************
/**
 * Ces classes permettent de calculer les plongements de tous types de maillages
 * (polylignes, grilles, maillages 3d).
 *
 * @author MOKA TEAM
 */

class CVertex;
//******************************************************************************
/**
 * @name CMesh-Diver
 *
 * Classes génériques pour le calcul des plongements.
 */
// @{
//******************************************************************************
class CMesh1Diver
{
protected:
  int FSx;
  int FIx; TCoordinate FKx0, FKx1;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre d'arêtes du maillage 1d
   */
  CMesh1Diver(int ASx);

  /// Destructeur
  virtual ~CMesh1Diver();

  //@}

  // @name Accesseurs
  // @{

  /// @return Le nombre de subdivisions du maillage selon la 1ère dimension.
  int getSx() const;

  /**
   * @param La position courante sur le maillage selon la 1ère dimension.
   * @precondition 0 <= AIx <= getSx()
   */
  virtual void setIx(int AIx);

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /**
   * Calcule le plongement du sommet courant du maillage.
   * Le sommet courant est positionné à l'aide de la méthode 'setIx'.
   */
  virtual CVertex computeCurrentVertex() const = 0;

  // @}
};
//------------------------------------------------------------------------------
class CMesh2Diver : public CMesh1Diver
{
protected:
  int FSy;
  int FIy; TCoordinate FKy0, FKy1;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre de subdivisions du maillage selon la 1ère dimension
   * @param ASy Le nombre de subdivisions du maillage selon la 2ème dimension
   */
  CMesh2Diver(int ASx, int ASy);

  /// Destructeur
  virtual ~CMesh2Diver();

  // @}
  // @name Accesseurs
  // @{

  // @return Le nombre de subdivisions du maillage selon la 2ème dimension
  int getSy() const;

  /**
   * @param La position courante sur le maillage selon la 2ème dimension.
   * @precondition 0 <= AIy <= getSy()
   */
  virtual void setIy(int AIy);

  // @}
};
//------------------------------------------------------------------------------
class CMesh3Diver : public CMesh2Diver
{
protected:
  int FSz;
  int FIz; TCoordinate FKz0, FKz1;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param ASx Le nombre de subdivisions du maillage selon la 1ère dimension
   * @param ASy Le nombre de subdivisions du maillage selon la 2ème dimension
   * @param ASz Le nombre de subdivisions du maillage selon la 3ème dimension
   */
  CMesh3Diver(int ASx, int ASy, int ASz);

  /// Destructeur
  virtual ~CMesh3Diver();

  // @}
  // @name Accesseurs
  // @{

  /// @return Le nombre de subdivisions du maillage selon la 3ème dimension
  int getSz() const;

  /**
   * @param La position courante sur le maillage selon la 3ème dimension.
   * @precondition 0 <= AIz <= getSz()
   */
  virtual void setIz(int AIz);

  // @}
};

// @}
//******************************************************************************
/**
 * @name CMesh-BasicDiver
 *
 * Classes pour le calcul des plongements des maillages selon une base.
 */
// @{
//******************************************************************************
class CMesh1BasicDiver : public CMesh1Diver
{
private:
  CVertex FOrigin;
  CVertex FVectorX;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * Le dernier sommet du maillage a pour coordonnées AOrigin + AVectorX.
   *
   * @param ASx Le nombre d'arêtes du maillage 1d
   * @param AOrigin Le sommet origine du maillage
   * @param AVectorX Le vecteur directeur du maillage
   */
  CMesh1BasicDiver(int ASx,
		   const CVertex& AOrigin,
		   const CVertex& AVectorX);

  /// Destructeur
  virtual ~CMesh1BasicDiver();

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /// Calcule le plongement du sommet courant du maillage.
  virtual CVertex computeCurrentVertex() const;

  // @}
};
//------------------------------------------------------------------------------
class CMesh2BasicDiver : public CMesh2Diver
{
private:
  CVertex FOrigin;
  CVertex FVectorX;
  CVertex FVectorY;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * Le dernier sommet du maillage a pour coordonnées
   * AOrigin + AVectorX + AVectorY.
   *
   * @param ASx Le nombre de subdivisions du maillage selon la 1ère dimension
   * @param ASy Le nombre de subdivisions du maillage selon la 2ème dimension
   * @param AOrigin Le sommet origine du maillage
   * @param AVectorX Le vecteur directeur du maillage selon la 1ère dimension
   * @param AVectorY Le vecteur directeur du maillage selon la 2ème dimension
   */
  CMesh2BasicDiver(int ASx, int ASy,
		   const CVertex & AOrigin,
		   const CVertex & AVectorX,
		   const CVertex & AVectorY);

  /// Destructeur
  virtual ~CMesh2BasicDiver();

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /// Calcule le plongement du sommet courant du maillage.
  virtual CVertex computeCurrentVertex() const;

  // @}
};
//------------------------------------------------------------------------------
class CMesh3BasicDiver : public CMesh3Diver
{
private:
  CVertex FOrigin;
  CVertex FVectorX;
  CVertex FVectorY;
  CVertex FVectorZ;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * Le dernier sommet du maillage a pour coordonnées
   * AOrigin + AVectorX + AVectorY + AVectorZ.
   *
   * @param ASx Le nombre de subdivisions du maillage selon la 1ère dimension
   * @param ASy Le nombre de subdivisions du maillage selon la 2ème dimension
   * @param ASz Le nombre de subdivisions du maillage selon la 3ème dimension
   * @param AOrigin Le sommet origine du maillage
   * @param AVectorX Le vecteur directeur du maillage selon la 1ère dimension
   * @param AVectorY Le vecteur directeur du maillage selon la 2ème dimension
   * @param AVectorZ Le vecteur directeur du maillage selon la 3ème dimension
   */
  CMesh3BasicDiver(int ASx, int ASy, int ASz,
		   const CVertex& AOrigin,
		   const CVertex& AVectorX,
		   const CVertex& AVectorY,
		   const CVertex& AVectorZ);

  /// Destructeur
  virtual ~CMesh3BasicDiver();

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
 * @name CMesh1VectorDiver
 *
 * Classe permettant de plonger une polyligne.
 */
// @{
//******************************************************************************
class CMesh1VectorDiver : public CMesh1Diver
{
protected:
  std::vector<CVertex> FVertices;

public:
  // @name Constructeur et destructeur
  // @{

  /**
   * Constructeur
   *
   * @param AVector Le vecteur contenant les sommets du maillage
   */

  CMesh1VectorDiver(const std::vector<CVertex>& AVector);

  /// Destructeur
  virtual ~CMesh1VectorDiver();

  // @}
  // @name Méthode de calcul des plongements
  // @{

  /// Retourne le plongement du sommet courant du maillage.
  virtual CVertex computeCurrentVertex() const;

  // @}
};
//******************************************************************************
//@}
//******************************************************************************
#include INCLUDE_INLINE("mesh-diver.icc")
//******************************************************************************
#endif // MESH_DIVER_HH
//******************************************************************************
