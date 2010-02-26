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
// Export XFig. Il y a deux formats "principaux" :
//   - sauveCarteXfig pour sauver la carte. Suivant les paramètres,
//     sauve les brins orientés (comme si on avait une carte), les faces
//     pleines, les coutures, la grille, les sommets, et inverse l'axe Y.
//   - sauveCarteIntervoxelXfig qui sauve au format intervoxel (donc avec
//     les faces pleines, les arêtes et les sommets comme si on avait des
//     surfels, pointels et lignels.
public:

void initConfig();

void sauveCarteXfig(TViewId, std::ofstream &, bool AArrow, bool AFace,
		    bool ASews, bool AGrid, bool AVertices, bool AInvert = true);

void sauveCarteIntervoxelXfig(TViewId, std::ofstream &, bool AInvert = true);

// Export the actual map in the fout stream, with intervoxels elements
private:
void transfoMmPixel(CVertex & v2d);
void sauveEntete(std::ofstream &os, int nbCoul);
void debutComposante(std::ofstream &os,
		     const CVertex & min, const CVertex & max);
int computeProf(const CVertex & V);
int computeProf(const CVertex & V1, const CVertex & V2);
void sauvePoint(std::ofstream &os, const CVertex & v, int coul, int larg, int AProf=-1);
void sauveLine(std::ofstream &os, const CVertex & p1, const CVertex & p2,
	       int coul, int larg, bool arrow, int AProf = -1);
void sauveTriangle(std::ofstream & os, int coulFill, int larg,
		   CVertex & PC, CVertex & P1, CVertex & P2);
void sauveFace(std::ofstream &os, CVertex *p, int nbPts,
	       int coulFill, int larg, int AProf);

void sauveDartVertex(std::ofstream &os, const CVertex & v, const CVertex & v2);

void treatSews(std::ofstream & /*fout*/,
	       CDart* /*dart*/, int /*mark*/, int /*mark2*/);

void treatFace(std::ofstream& /*fout*/, CDart* /*dart*/,
	       int /*mark*/, int /*mark2*/ );

void treatFilledFace(std::ofstream& /*fout*/, CDart* /*dart*/,
		     int /*mark*/, int /*mark2*/ );

void treatDartWithArrow(std::ofstream& /*fout*/, CDart* /*dart*/,
			int /*mark*/, int /*mark2*/, int AProf=-1);

void treatDartNoArrow(std::ofstream& /*fout*/, CDart* /*dart*/,
		      int /*mark*/, int /*mark2*/ );

void saveGridIncludingBox(std::ofstream & /*fout*/);


// Projet the 3d vertex into the 2d plane, with the Open GL
// matrix. The z of the out parameter v2d is the depth of the point
  void transfo3dTo2d( const CVertex &, CVertex & /*v2d*/);

// this method is used for compute the 2d including box of an
// object. pts is alpha 3d point, min and max was 2d points, in-out
// parameters. Project the 3d pts in 2d, and compare this with min
// and max, and eventually update them if necessary.
// If init is true, min=max=the project in 2d of pts
void updateMinMax2d(const CVertex& /*pts*/, CVertex& /*min*/,
		    CVertex & /*max*/, bool init=false);

// compute the 2d including box of the projection of the darts in the orbit.
void getIncludingBoxForDarts(CDart*, TOrbit, CVertex& /*min*/,
			     CVertex& /*max*/);

// save the grid in fout in xfig format.
void treatGrid(std::ofstream& /*fout*/);

void treatPointel(std::ofstream& /*os*/, CDart* /*d*/, int /*mark*/);
void treatLignel(std::ofstream& /*os*/, CDart* /*d*/, int /*mark*/);
//******************************************************************************
