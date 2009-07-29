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
// Ce fichier fait partie de "g-map-vertex.hh".
//******************************************************************************

public:

/// Sélection de la méthode de calcul des sommets éclatés :
void         setBurstMethod(TBurstMethod AMethod);
TBurstMethod getBurstMethod() const;

/// Met à jour le plongement éclaté de tous les brins de la carte.
void updateAllBurstDarts();

/**
 * Met à jour le plongement éclaté des brins marqués avec la marque
 * AMarkNumber.
 * Si AUpdateIncidentVolumes vaut 'vrai', c'est le plongement éclaté de tous
 * les brins des volumes incidents à un sommet dont un brin au moins est
 * marqué avec la marque AMarkNumber qui est mis à jour.
 *
 * @param AMarkNumber Un numéro de marque
 * @param AUpdateIncidentVolumes Un booléen
 */
void updateMarkedBurstDarts(int AMarkNumber,
			    bool AUpdateIncidentVolumes = true);

protected:

void updateMarkedDartsMethodScale(int AMarkNumber, bool AUpdateIncidentVolumes);
void updateMarkedDartsMethodShift(int AMarkNumber, bool AUpdateIncidentVolumes);

private:
void updateFaceMethodShift(int AMarkNumber, int AMarkI, CDart* ADart,
			   const CVertex& ABaryVol, int ADirectIndex);

//******************************************************************************
