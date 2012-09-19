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
// Ce fichier fait partie de "g-map-generic.hh".
//******************************************************************************

public:

/**
 * Ces méthodes permettent de composer les involutions pour accéder
 * facilement aux brins de la carte. Les indices sont donnés selon
 * l'ordre dans lequel ils sont appliqués.
 * Exemple: alpha123(d) signifie alpha3(alpha2(alpha1(d))).
 *
 * @param ADart Un pointeur sur un brin quelconque
 * @return Un pointeur sur un brin
 */
CDart* alpha01     (CDart* ADart) const;
CDart* alpha010    (CDart* ADart) const;
CDart* alpha0101   (CDart* ADart) const;
CDart* alpha0101210(CDart* ADart) const;
CDart* alpha01010  (CDart* ADart) const;
CDart* alpha01021  (CDart* ADart) const;
CDart* alpha0102101(CDart* ADart) const;
CDart* alpha010212 (CDart* ADart) const;
CDart* alpha012    (CDart* ADart) const;
CDart* alpha0121   (CDart* ADart) const;
CDart* alpha0123   (CDart* ADart) const;
CDart* alpha012321 (CDart* ADart) const;
CDart* alpha02     (CDart* ADart) const;
CDart* alpha020    (CDart* ADart) const;
CDart* alpha021    (CDart* ADart) const;
CDart* alpha0210   (CDart* ADart) const;
CDart* alpha02101  (CDart* ADart) const;
CDart* alpha02121  (CDart* ADart) const;
CDart* alpha023    (CDart* ADart) const;
CDart* alpha03     (CDart* ADart) const;
CDart* alpha030    (CDart* ADart) const;
CDart* alpha10     (CDart* ADart) const;
CDart* alpha101    (CDart* ADart) const;
CDart* alpha1010   (CDart* ADart) const;
CDart* alpha10101  (CDart* ADart) const;
CDart* alpha1010101(CDart* ADart) const;
CDart* alpha1012   (CDart* ADart) const;
CDart* alpha10121  (CDart* ADart) const;
CDart* alpha101201 (CDart* ADart) const;
CDart* alpha1012101(CDart* ADart) const;
CDart* alpha101232 (CDart* ADart) const;
CDart* alpha102    (CDart* ADart) const;
CDart* alpha1021   (CDart* ADart) const;
CDart* alpha12     (CDart* ADart) const;
CDart* alpha121    (CDart* ADart) const;
CDart* alpha1210   (CDart* ADart) const;
CDart* alpha12101  (CDart* ADart) const;
CDart* alpha1212   (CDart* ADart) const;
CDart* alpha123    (CDart* ADart) const;
CDart* alpha1232   (CDart* ADart) const;
CDart* alpha12321  (CDart* ADart) const;
CDart* alpha123210 (CDart* ADart) const;
CDart* alpha13     (CDart* ADart) const;
CDart* alpha20     (CDart* ADart) const;
CDart* alpha201    (CDart* ADart) const;
CDart* alpha21     (CDart* ADart) const;
CDart* alpha210    (CDart* ADart) const;
CDart* alpha2101   (CDart* ADart) const;
CDart* alpha21012  (CDart* ADart) const;
CDart* alpha210123 (CDart* ADart) const;
CDart* alpha212    (CDart* ADart) const;
CDart* alpha2121   (CDart* ADart) const;
CDart* alpha23     (CDart* ADart) const;
CDart* alpha231    (CDart* ADart) const;
CDart* alpha2310   (CDart* ADart) const;
CDart* alpha232    (CDart* ADart) const;
CDart* alpha2321   (CDart* ADart) const;
CDart* alpha232101 (CDart* ADart) const;
CDart* alpha30     (CDart* ADart) const;
CDart* alpha301    (CDart* ADart) const;
CDart* alpha3012   (CDart* ADart) const;
CDart* alpha30120  (CDart* ADart) const;
CDart* alpha302    (CDart* ADart) const;
CDart* alpha31     (CDart* ADart) const;
CDart* alpha312    (CDart* ADart) const;
CDart* alpha32     (CDart* ADart) const;
CDart* alpha3201   (CDart* ADart) const;
CDart* alpha321    (CDart* ADart) const;
CDart* alpha321012 (CDart* ADart) const;
CDart* alpha323    (CDart* ADart) const;

/**
 * Retourne le champ directInfo demandé du brin ADart après l'avoir
 * retypé en pointeur sur un brin.
 *
 * @param ADart Un brin de la carte
 * @param ADirectInfoIndex Un indice de champ directInfo
 * @return Un brin de la carte
 */
CDart* getDirectInfoAsDart(CDart* ADart, int ADirectInfoIndex);

//******************************************************************************
