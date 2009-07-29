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
#include "transformation-matrix.hh"
//******************************************************************************
#include INCLUDE_NON_INLINE("transformation-matrix.icc")
//******************************************************************************
CTransformationMatrix::~CTransformationMatrix()
{
}
//******************************************************************************
const CTransformationMatrix COONS     ( 0,  0,  0,  0,
			  0,  0, -1,  1,
			  0,  0,  1,  0,
			  0,  0,  0,  0);

const CTransformationMatrix HERMITE   (-1,  2, -1,  0,
			  3, -5,  1,  1,
			 -3,  4,  0,  0,
			  1, -1,  0,  0);

const CTransformationMatrix BEZIER    (-1,  3, -3,  1,
			  3, -6,  3,  0,
			 -3,  3,  0,  0,
			  1,  0,  0,  0);

const CTransformationMatrix OVERHAUSER(-0.5,  1  , -0.5,  0,
			  1.5, -2.5,  0  ,  1,
			 -1.5,  2  ,  0.5,  0,
			  0.5, -0.5,  0  ,  0);

const CTransformationMatrix SPLINE    (-1/6.0,  0.5, -0.5,  1/6.0,
			  0.5  , -1  ,  0  ,  2/3.0,
			 -0.5  ,  0.5,  0.5,  1/6.0,
			  1/6.0,  0  ,  0  ,  0    );
//******************************************************************************
