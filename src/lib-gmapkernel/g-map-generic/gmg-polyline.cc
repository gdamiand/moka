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
#include "g-map-generic.hh"
using namespace GMap3d;
//******************************************************************************
bool CGMapGeneric::isIsolatedPolyline(CDart * ADart)
{
  assert(ADart!=NULL);

  for (CDynamicCoverage01 it(this,ADart); it.cont(); ++it)
    if (!isFree2(*it) || !isFree3(*it))
      return false;

  return true;
}
//******************************************************************************
bool CGMapGeneric::isClosedPolyline(CDart * ADart)
{
  assert(ADart!=NULL);

  for (CDynamicCoverage01 it(this,ADart); it.cont(); ++it)
    if (isFree0(*it) || isFree1(*it))
      return false;

  return true;
}
//******************************************************************************
int CGMapGeneric::getNbPolylineVertices(CDart * ADart)
{
  assert(ADart!=NULL);

  // Méthode utilisée: le nombre de sommets d'une face est égal à son nombre de
  // brins divisé par 2.
  // Lorsqu'un brin n'est pas 1-cousu, il compte double.

  int n=0;

  for (CDynamicCoverage01 it(this, ADart); it.cont(); ++it)
    {
      ++n;

      if (isFree1(*it))
	++n;
    }

  assert(n%2 == 0);

  return n/2;
}
//******************************************************************************
int CGMapGeneric::getNbPolylineEdges(CDart * ADart,
				     bool AConsider0FreeDartsAsEdges)
{
  assert(ADart!=NULL);

  // Méthode utilisée: le nombre d'arêtes d'une face est égal à son nombre de
  // brins divisé par 2.
  // Lorsqu'un brin n'est pas 0-cousu, soit il compte double
  // (si AConsider0FreeDartsAsEdges vaut 'vrai') soit il ne compte pas
  // (si AConsider0FreeDartsAsEdges vaut 'faux').

  int n=0;

  for (CDynamicCoverage01 it(this, ADart); it.cont(); ++it)
    {
      ++n;

      if (isFree0(*it))
	n += AConsider0FreeDartsAsEdges ? +1 : -1;
    }

  assert(n%2 == 0);

  return n/2;
}
//******************************************************************************
CDart * CGMapGeneric:: prolongPolyline(CDart * ADart)
{
  assert(ADart==NULL || isIsolatedPolyline(ADart));

  // Cas particulier 1: ADart==NULL
  if (ADart==NULL)
    return addMapDart();

  // Cas particulier 2: La polyligne n'alpha qu'un seul brin.
  // On la prolonge pour créer une arête.
  if (isFree0(ADart))
    {
      CDart * dart2= addMapDart();
      linkAlpha0(ADart,dart2);
      return dart2;
    }

  // Cas général:
  CDart * dart2= addMapDart();
  CDart * dart3= addMapDart();

  linkAlpha1(ADart,dart2);
  linkAlpha0(dart2,dart3);

  return dart3;
}
//******************************************************************************
CDart * CGMapGeneric::shortenPolyline(CDart * ADart)
{
  assert(ADart!=NULL);
  assert(isFree1(ADart));
  assert(isIsolatedPolyline(ADart));

  // Cas particulier 1: La polyligne est réduite à un brin. On la détruit.
  if (isFree0(ADart))
    {
      delMapDart(ADart);
      return NULL;
    }

  CDart * dart0= alpha0(ADart);

  // Cas particulier 2: La polyligne est une arête (2 brins)
  if (isFree1(dart0))
    {
      unsew0(ADart);
      delMapDart(ADart);
      return dart0;
    }

  // Cas général:
  CDart * dart01= alpha1(dart0);

  // Inutile de 0-découdre dart0 et ADart car ils vont être détruits.
  unsew1(dart01);

  delMapDart(ADart);
  delMapDart(dart0);

  return dart01;
}
//******************************************************************************
CDart * CGMapGeneric::closePolyline(CDart * ADart)
{
  assert(ADart!=NULL);
  assert(isIsolatedPolyline(ADart));

  // D'abord, on cherche les deux bouts de la polyligne:
  CDart * end1= NULL, * end2= NULL;

  CDynamicCoverage01 it(this, ADart);

  for (; it.cont(); ++it)
    if (isFree1(*it))
      {
	if (end1==NULL)
	  end1=*it;
	else
	  end2=*it;
      }

  if (end2==NULL || // la polyligne n'alpha qu'un seul brin ou est déjà fermée
      end2==alpha0(end1)) // la polyligne est une arête
    return NULL;

  // Fermeture:
  CDart * dart1= addMapDart();
  CDart * dart2= addMapDart();

  linkAlpha1(dart1,end1);
  linkAlpha1(dart2,end2);

  linkAlpha0(dart1,dart2);

  return dart1;
}
//******************************************************************************
