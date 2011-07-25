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
#ifndef IMAGE_3D_CEA_HH
#define IMAGE_3D_CEA_HH
//******************************************************************************
#include "alt-stdint.hh"
#include "image-3d.hh"
#include "inline-macro.hh"

#include <fstream>
#include <string>
#include <cstring>
//******************************************************************************

/** Cette classe permet de lire une image 3d au format ascii du CEA.
 */

namespace GMap3d
{
  class CImage3dCEA: public CImage3d
  {
  public:
    CImage3dCEA(const std::string & AFilename,
		int AFirstPlane = 0, int ANbPlaneToRead = 0);

    ~CImage3dCEA();

    bool sameVoxel(unsigned int Ax1, unsigned int Ay1, bool Az1,
		   unsigned int Ax2, unsigned int Ay2, bool Az2) const;

    /// Ici, seul la composante rouge est utilisée vue le type d'image.
    bool sameVoxel(unsigned int Ax, unsigned int Ay, bool Az,
		   int ARed, int, int, int) const;

  protected:
    bool readData();
    void freePlane(bool AActuPlane);
    void pushPlane();

  private:
    uint8_t** allocArray();
    void freeArray(uint8_t** AArray);

    std::ifstream FStream;
    std::streampos FStreamPos;
    bool readVoxel(int* Ax, int* Ay, int* Az, uint8_t* AColor);
    void unreadLastVoxel();
    bool findPlaneSize();

    int FXMin, FXMax; unsigned int FXStep;
    int FYMin, FYMax; unsigned int FYStep;

    uint8_t** FPrevPlane;
    uint8_t** FActuPlane;
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("image-3d-cea.icc")
//******************************************************************************
#endif // IMAGE_3D_CEA_HH
//******************************************************************************
