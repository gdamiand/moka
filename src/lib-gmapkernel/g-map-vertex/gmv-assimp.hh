/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 * Copyright (C) 2012, Sebastien Horna, CNRS, Xlim-SIC,
 *               sebastien.horna@univ-poitiers.fr,
 *
 * This file is part of lib-gmapvertex
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
 * Authors : Sébastien Horna and Abdoulaye Diakité (2014)
 *
 */

//******************************************************************************
// Ce fichier fait partie de "g-map-vertex.hh".
//******************************************************************************

public:

#ifdef WITH_ASSIMP

/**
 * Importe une 3Gmap à partir d'un fichier au format lisible par assimp.
 *
 * @param AFilename le nom de fichier à charger (obj, off, ...)
 * @return Un brin de la carte chargée si le chargement s'est bien
 * déroulé, NULL sinon
 */
CDart* importWithAssimp(const char* AFilename);

#endif // WITH_ASSIMP
