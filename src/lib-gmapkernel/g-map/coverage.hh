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
// Generic coverage class following different orbits in gmaps.
//******************************************************************************
#ifndef COVERAGE_HH
#define COVERAGE_HH
//******************************************************************************
#include "kernel-types.hh"
#include "inline-macro.hh"
//******************************************************************************
#define COVERAGE0          (0)
#define COVERAGE1          (1)
#define COVERAGE2A         (2)
#define COVERAGE2B         (3)
#define COVERAGE3A         (4)
#define COVERAGE3B         (5)
#define COVERAGE4          (6)

#define COVERAGE_BORDER    (7)
#define COVERAGE_ALL       (8)

#define COVERAGE_REAL_FACE (9)
//******************************************************************************
namespace GMap3d
{
  class CDart;
  class CGMapBasic;

  /**
   * Classe abstraite pour la définition des itérateurs permettant de parcourir
   * les brins d'une carte.
   *
   * @author MOKA TEAM
   */
  class CCoverage
  {
  public:
    // @name Constructeur et destructeur.
    // @{

    /// Constructeur.
    CCoverage(CGMapBasic* AMap);

    /// Destructeur virtuel.
    virtual ~CCoverage();

    // @}

    /// Accesseur à l'objet G-Carte.
    CGMapBasic* getMap() const;

    // @name Méthodes virtuelles pures implémentées dans les classes filles
    // @{

    /// Réinitialisation du parcours.
    virtual void reinit() = 0;

    /// Itérateur préfixe.
    virtual void operator++() = 0;

    /// Itérateur postfixe
    virtual CDart* operator++(int) = 0;

    /// Drapeau d'arrêt du parcours.
    virtual bool cont() = 0;

    /// Déreférencement du pointeur de parcours en pointeur de brin.
    virtual CDart* operator*() = 0;

    /// Type du dernier appel à ++.
    virtual TOperationState prevOperationType();

    /// Identificateur du type de parcours.
    virtual int type() const = 0;

    // @}

  protected:
    // @name Champs privés
    // @{

    /// La G-Carte sur laquelle va s'effectuer les parcours.
    CGMapBasic* FMap;

    // @}
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("coverage.icc")
//******************************************************************************
#endif // COVERAGE_HH
//******************************************************************************
