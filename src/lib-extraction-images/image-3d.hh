/*
 * lib-extraction-images : Extraction de cartes à partir d'images 2D et 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-extraction-images
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
#ifndef IMAGE_3D_HH
#define IMAGE_3D_HH
//******************************************************************************
#include "inline-macro.hh"
#include "alt-stdint.hh"
/*******************************************************************************
 * Cette classe virtuelle pure définit les méthodes de base pour travailler sur
 * une image 3d pour un parcours de type haut-bas, devant-derrière et
 * gauche-droite.
 * On ne conserve que 2 plans à la fois, et on ne peut donc accéder qu'aux
 * voxels d'un de ces 2 plans. D'ou les méthodes getCurrentVoxel pour le plan
 * actuel, et getPrevVoxel pour le dernier plan.
 */

namespace GMap3d
{
  class CImage3d
  {
  public:
    CImage3d(int AActuPlaneIndex);

    virtual ~CImage3d();

    bool isOk() const;

    unsigned int getPlaneActu() const;

    unsigned int getWidth () const;
    unsigned int getHeight() const;

    bool readNextPlane();

    bool lastPlane();

    // Az* == false => FPrevPlane
    // Az* == true  => FActuPlane
    virtual bool sameVoxel(unsigned int Ax1, unsigned int Ay1, bool Az1,
			   unsigned int Ax2, unsigned int Ay2, bool Az2) const =0;

    /// Méthode pour comparer si le voxel se trouvant aux coordonnées (Ax,Ay,Az)
    /// est de la couleur (ARed, AGreen, ABlue, AAlpha).
    virtual bool sameVoxel(unsigned int Ax, unsigned int Ay, bool Az,
			   int ARed, int AGreen, int ABlue, int AAlpha ) const =0;

    /* Les méthodes suivantes testent l'appartenante à la même région de deux
     * voxels. Le premier mot de la méthode désigne la position du premier voxel
     * par rapport au voxel courant. Le deuxième mot (après le -) désigne la
     * position du deuxième voxel par rapport au premier (et donc est soit up,
     * left ou behind). X et y désigne les cordonnées du voxel courant qui est
     * dans le plan courant en z.
     */
    bool sameVoxelActuLeft    (unsigned int x, unsigned int y) const;
    bool sameVoxelActuBehind  (unsigned int x, unsigned int y) const;
    bool sameVoxelActuUp      (unsigned int x, unsigned int y) const;

    bool sameVoxelLeftBehind  (unsigned int x, unsigned int y) const;
    bool sameVoxelLeftUp      (unsigned int x, unsigned int y) const;

    bool sameVoxelBehindLeft  (unsigned int x, unsigned int y) const;
    bool sameVoxelBehindUp    (unsigned int x, unsigned int y) const;

    bool sameVoxelUpLeft      (unsigned int x, unsigned int y) const;
    bool sameVoxelUpBehind    (unsigned int x, unsigned int y) const;

    bool sameVoxelUpbehindLeft(unsigned int x, unsigned int y) const;

    bool sameVoxelLeftbehindUp(unsigned int x, unsigned int y) const;

    bool sameVoxelLeftupBehind(unsigned int x, unsigned int y) const;

  protected:
    virtual bool readData() =0;
    virtual void freePlane(bool AActuPlane);
    virtual void pushPlane();

    unsigned int FWidth, FHeight;

    bool FIsOk; // vrai si on a réussi à créer l'image, faux sinon

    int FNbPlaneToRead;
    int FActuPlaneIndex;

  private:
    bool FNullActuPlane, FNullPrevPlane;
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("image-3d.icc")
//******************************************************************************
#endif // IMAGE_3D_HH
//******************************************************************************
