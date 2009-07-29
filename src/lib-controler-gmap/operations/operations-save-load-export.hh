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
// Ce fichier fait partie de "controler-gmap-operations.hh".
//******************************************************************************

public:

bool empty();

bool addMap ( const char * AFilename );
bool loadMap( const char * AFilename );

bool saveMapAscii ( const char * AFilename );
bool saveMapBinary( const char * AFilename );

bool exportXfig		   ( const char * AFilename, TViewId AViewId,
			     bool AArrow, bool AFace, bool ASews, bool AGrid,
			     bool AVertices );
bool exportPov 		   ( const char * AFilename );
bool exportXfigAsIntervoxel( const char * AFilename, TViewId AViewId );

#ifdef MODULE_EXTRACTION_IMAGE
bool extractImage2D( const std::string & AFilename );

bool extractImage3D( const std::string & AFilename,
		     int AFirstPlane, int ANbPlane,
		     int ALevel, int NbChiffres, bool ADestroyBorder,
		     bool AKeepFictiveEdges, bool AShiftFictiveEdges,
		     bool AWithPrecodes, bool ACEAFormat );

bool extractOneRegionVoxels( const std::string & AFilename,
			     int AFirstPlane, int ANbPlane,
			     int NbChiffres,
			     int ARed, int AGreen, int ABlue, int AAlpha );

#endif // MODULE_EXTRACTION_IMAGE

// Import et export au format off (format basique de type graphe, en 2D les
// sommets puis les arêtes, en 3D les sommets puis les faces)
bool importOff( const char* AFilename );
bool exportOff( const char* AFilename ); // La dimension est celle de la carte
bool exportOff3D( const char* AFilename ); // La dimension est toujours 3
//******************************************************************************
