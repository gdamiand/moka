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
#ifndef PARAMETER_POLYLINE_POSITION_HH
#define PARAMETER_POLYLINE_POSITION_HH
//******************************************************************************
#include "parameter.hh"
#include <vector>
//******************************************************************************
class CVertex;

namespace GMap3d
{
  class CParameterPolylinePosition : public CParameter
  {
  public:

    //@{
    CParameterPolylinePosition(int ANbRef = 0);
    CParameterPolylinePosition(const CParameterPolylinePosition &);
    virtual ~CParameterPolylinePosition();
    virtual CParameter * copy() const;
    //@}

    //@{

    virtual void save(std::ostream &);
    virtual void load(std::istream &);
    virtual void reinit();

    //@}

    //@{

    unsigned int                 getNbVertices() const;
    const CVertex              & getVertex(unsigned int) const;
    const CVertex              & getLastVertex() const;
    const std::vector<CVertex> & getVertices() const;

    void removeLastVertex();
    void addVertex(const CVertex &);
    void setVertex(unsigned int, const CVertex &);
    void setLastVertex(const CVertex &);

    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

  private:
    // Les sommets de la polyline
    std::vector<CVertex> FVertexList;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_POLYLINE_POSITION_HH
//******************************************************************************
