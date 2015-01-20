/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
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
 *
 * Author(s): Víctor Manuel Soto Francés
 * Copyright (C) 2009, Víctor Manuel Soto Francés
 */

//******************************************************************************
#ifndef MULTIVECTOR_HH
#define MULTIVECTOR_HH
//******************************************************************************
#include <stdexcept>
#include "geoma.hh"
#include "geomaData.hh"
#include "vertex.hh"
#include "dart.hh"
#include "attribute-vertex.hh"
#include "inline-macro.hh"
//******************************************************************************
enum {
  E0 = 0,
  E1 = ( 1 << 0 ),
  E2 = ( 1 << 1 ),
  E3 = ( 1 << 2 )
}; // R3
enum{
  e0 = ( 1 << 0 ),
  e1 = ( 1 << 1 ),
  e2 = ( 1 << 2 ),
  e3 = ( 1 << 3 )
}; // R4

class CMultivector
{
private:
  nklein::GeometricAlgebra< double, 4 > MD; //homogeneus model
  double l,a,v; //! length,area,volume
public:
  /** Get */
  nklein::GeometricAlgebra< double, 4 > getMD() const;
  /** Set */
  void setMD(const nklein::GeometricAlgebra< double, 4 >& MVector);
public:
  /** constructores */
  CMultivector();

  CMultivector(const CMultivector &AMv);

  /** Contruct a multivector from a dart and a vertex
     */
  //CMultivector(const GMap3d::CDart *,GMap3d::CVertex *AVertex=NULL);

private:
  double computeLAV(void);
  nklein::GeometricAlgebra<double, 4 > getPoint(const GMap3d::CDart * dart);
};
//******************************************************************************
#include INCLUDE_INLINE("multivector.icc")
//******************************************************************************
#endif // MULTIVECTOR_HH
//******************************************************************************
