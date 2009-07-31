/*
 * lib-corefinement : Opérations de corafinement.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-corefinement
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
#ifndef COREF_API_HH
#define COREF_API_HH

//******************************************************************************
namespace GMap3d
{
  class CGMapVertex;
  class CDart;
}
class CVertex;
//******************************************************************************
class CCorefinementAPI
{
private:
  GMap3d::CGMapVertex * FMap;

public:
  CCorefinementAPI(GMap3d::CGMapVertex * AMap);
  virtual ~CCorefinementAPI();

  void corefine2dMeshesSweeping(GMap3d::CDart * ADart1,
				GMap3d::CDart * ADart2, const CVertex& ANormal);

  void corefine2dMeshesPropagation(GMap3d::CDart * ADart1,
				   GMap3d::CDart * ADart2,
				   const CVertex & ANormal,
				   bool useBBox = false);

  void corefine3dMeshes(GMap3d::CDart * ADart1, GMap3d::CDart * ADart2);
  void corefineMarked3dFaces(int AMark1, int AMark2);
  void corefineMarked3dMeshesWith(int AMark, GMap3d::CDart * ADart);

  void booleanOperations2d(GMap3d::CDart * ADart1, GMap3d::CDart * ADart2,
			   const CVertex & ANormal,
			   int ADifference1Mark = -1,
			   int ADifference2Mark = -1,
			   int AIntersectionMark = -1,
			   int AUnionMark = -1);

  void booleanOperations3d(GMap3d::CDart * ADart1, GMap3d::CDart * ADart2,
			   int ADifference1Mark = -1,
			   int ADifference2Mark = -1,
			   int AIntersectionMark = -1,
			   int AUnionMark = -1);
};
//******************************************************************************
#endif // COREF_API_HH
//******************************************************************************
