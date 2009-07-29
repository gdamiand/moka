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
#ifndef PARAMETER_CREATION_HH
#define PARAMETER_CREATION_HH
//******************************************************************************
#include "parameter.hh"
#include "vector.hh"
//******************************************************************************
//@name Flags pour la création de cubes (maillés en dimension < 3)

//@{
#define CUBE_X1 (1 << 0)
#define CUBE_X2 (1 << 1)
#define CUBE_Y1 (1 << 2)
#define CUBE_Y2 (1 << 3)
#define CUBE_Z1 (1 << 4)
#define CUBE_Z2 (1 << 5)

#define CUBE_ALL (CUBE_X1 | CUBE_X2 | CUBE_Y1 | CUBE_Y2 | CUBE_Z1 | CUBE_Z2)

#define CUBE_RING_X (CUBE_Y1 | CUBE_Y2 | CUBE_Z1 | CUBE_Z2)
#define CUBE_RING_Y (CUBE_X1 | CUBE_X2 | CUBE_Z1 | CUBE_Z2)
#define CUBE_RING_Z (CUBE_X1 | CUBE_X2 | CUBE_Y1 | CUBE_Y2)
//@}

//******************************************************************************
class CVertex;

namespace GMap3d
{
  class CParameterCreation : public CParameter
  {
  public:
    //@{

    CParameterCreation(int ANbRef = 0);
    CParameterCreation(const CParameterCreation &);
    virtual ~CParameterCreation();
    virtual CParameter* copy() const;

    //@}

    //@{

    virtual void save(std::ostream &);
    virtual void load(std::istream &);
    virtual void reinit();
  
    //@}

    //@{ Méthodes pour le polygone

    int  getPolygonNbEdges() const;
    void setPolygonNbEdges(int ANbEdges);

    //@}

    //@{ Méthodes pour les maillages

    int getMeshNbSubdivisionsX() const;
    int getMeshNbSubdivisionsY() const;
    int getMeshNbSubdivisionsZ() const;

    void setMeshNbSubdivisionsX(int ASx);
    void setMeshNbSubdivisionsY(int ASy);
    void setMeshNbSubdivisionsZ(int ASz);

    int  getMeshDimension() const;
    void setMeshDimension(int ADimension);

    int  getMeshSubdivisionDimension() const;
    void setMeshSubdivisionDimension(int ADimension);

    bool getMeshCreatedFacesActive() const;

    unsigned char getMeshCreatedFaces() const;
    bool          getMeshCreatedFace(unsigned char AFace) const;

    void setMeshCreatedFaces(unsigned char AFaces);
    void setMeshCreatedFace(unsigned char AFace, bool AValue = true);

    //@}

    //@{ Méthodes pour le cylindre

    int  getCylinderNbMeridians() const;
    void setCylinderNbMeridians(int ANb);

    int  getCylinderNbParallels() const;
    void setCylinderNbParallels(int ANb);

    bool getCylinderClosedUp() const;
    void setCylinderClosedUp(bool AClosed);

    bool getCylinderClosedDown() const;
    void setCylinderClosedDown(bool AClosed);

    //@}

    //@{ Méthodes pour la pyramide

    int  getPyramidNbMeridians() const;
    void setPyramidNbMeridians(int ANb);

    int  getPyramidNbParallels() const;
    void setPyramidNbParallels(int ANb);

    bool getPyramidClosed() const;
    void setPyramidClosed(bool AClosed);

    //@}

    //@{ Méthodes pour la sphère

    int  getSphereNbMeridians() const;
    void setSphereNbMeridians(int ANb);

    int  getSphereNbParallels() const;
    void setSphereNbParallels(int ANb);

    //@}

    //@{ Méthodes pour le tore

    int  getTorusNbMeridians() const;
    void setTorusNbMeridians(int ANb);

    int  getTorusNbParallels() const;
    void setTorusNbParallels(int ANb);

    float getTorusRadiusRapport() const;
    void  setTorusRadiusRapport(float AValue);

    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

  private:
    // Nombre d'arêtes du polygone
    int FPolygonNbEdges;

    // Maillages
    int FMeshNbSubdivisions[3];
    int FMeshDimension;
    int FMeshSubdivisionDimension;
    unsigned char FCreatedFaces;

    // Cylindre
    int FCylinderNbMeridians; // Nombre de méridiens du cylindre
    int FCylinderNbParallels; // Nombre de paralèlles du cylindre
    bool FCylinderClosedUp;   // Fermeture ou non du bord 1
    bool FCylinderClosedDown; // Fermeture ou non du bord 2

    // Pyramide
    int FPyramidNbMeridians; // Nombre de méridiens de la pyramide
    int FPyramidNbParallels; // Nombre de paralèlles de la pyramide
    bool FPyramidClosed;     // Fermeture ou non de la base

    // Sphère
    int FSphereNbMeridians; // Nombre de méridiens de la sphère
    int FSphereNbParallels; // Nombre de paralèlles de la sphère

    // Tore
    int FTorusNbMeridians;     // Nombre de méridiens du tore
    int FTorusNbParallels;     // Nombre de paralèlles du tore
    float FTorusRadiusRapport; // Rapport entre le petit rayon et le grand
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_CREATION_HH
//******************************************************************************
