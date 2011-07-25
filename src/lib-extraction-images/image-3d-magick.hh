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
#ifndef MODULE_EXTRACTION_IMAGE_WITHOUT_MAGICK
//******************************************************************************
#ifndef IMAGE_3D_MAGICK_HH
#define IMAGE_3D_MAGICK_HH

#include "image-3d.hh"
#include "inline-macro.hh"

#include <string>
#include <Magick++.h>
//******************************************************************************

/** Cette classe permet de lire une image 3d (représentée par un ensemble de
 *  d'images 2d correspondant à des coupes) en utilisant Image Magick.
 */

namespace GMap3d
{

  class CImage3dMagick: public CImage3d
  {
  public:
    CImage3dMagick(const std::string & AFilename,
		   int FirstPlane=0, int NbPlaneToRead=0, unsigned int Lg=3);

    ~CImage3dMagick();

    bool sameVoxel(unsigned int Ax1, unsigned int Ay1, bool Az1,
		   unsigned int Ax2, unsigned int Ay2, bool Az2) const;

    /// Méthode pour comparer si le voxel se trouvant aux coordonnées (Ax,Ay,Az)
    /// est de la même couleur que AColor.
    /// AColor contient 3 octets représentant Rouge, Vert, Bleu (dans cet ordre)
    // (l'octet de poid fort n'est pas utilisé)
    bool sameVoxel(unsigned int Ax, unsigned int Ay, bool Az,
		   int ARed, int AGreen, int ABlue, int AAlpha ) const;

  protected:
    bool readData();
    void freePlane(bool AActuPlane);
    void pushPlane();

  private:
    void computePrefix(const std::string & AFileName);

    Magick::Image * FPrevPlane;
    Magick::Image * FActuPlane;

    std::string FPrefix;
    // Longueur de la chaîne de caractères FPrefix :
    unsigned int  FPrefixLongueur;
    // Nombre de chiffres pour coder les plans dans le nom des fichiers :
    unsigned int FLg;
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("image-3d-magick.icc")
//******************************************************************************
#endif // IMAGE_3D_MAGICK_HH
//******************************************************************************
#endif // MODULE_EXTRACTION_IMAGE_WITHOUT_MAGICK
//******************************************************************************
