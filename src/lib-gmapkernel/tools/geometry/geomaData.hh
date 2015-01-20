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
#ifndef GEOMA_DATA_HH
#define GEOMA_DATA_HH
//******************************************************************************
namespace nklein {
  namespace nklein_priv {
    template<const unsigned int P, const unsigned int Q>
    int GeomMultTable< P, Q >::multTable[ 1U << (P+Q) ][ 1U << (P+Q) ];
    
    template<const unsigned int N>
    int GeomGradeTable<N>::gradeTable[ 1U << (N) ];
  };
  
  static const char* GeometricAlgebraVersion
  = "nklein::GeometricAlgebra::version: "
    "v1.2.2007.08.20"                                                        ;
};
//******************************************************************************
#endif // GEOMA_DATA_HH
//******************************************************************************
