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
#ifndef DART_VERTEX_HH
#define DART_VERTEX_HH
//******************************************************************************
#include "dart.hh"
#include "attribute-vertex.hh"

#include <bitset>
#include <iostream>
//******************************************************************************
namespace GMap3d
{
  class CDartVertex: public CDart
  {
  public:
    // @name Constructeurs / destructeur
    // @{

    /**
     * Constructeur par défaut
     */
    CDartVertex();

    /**
     * Constructeur par copie
     */
    CDartVertex(const CDartVertex& ADart);

    /**
     * Constructeur avec initialisation des marques
     *
     * @param AMarks Les marques telles qu'elles doivent être positionnées
     * @return Une instance de la classe
     */
    CDartVertex(const std::bitset<NB_MARKS>& AMarks);

    /**
     * Constructeur avec initialisation des marques et du sommet éclaté
     *
     * @param AMarks Les marques telles qu'elles doivent être positionnées
     * @param AVertex Un sommet correspondant au plongement éclaté
     * @return Une instance de la classe
     */
    CDartVertex(const std::bitset<NB_MARKS>& AMarks, const CVertex& AVertex);

    /**
     * Constructeur avec lecture du brin dans un flot
     *
     * @param AStream Un flot dans lequel lire le contenu du brin
     * @param AFormat Le type de format dans lequel le flot est codé
     * @return Une instance de la classe
     */
    CDartVertex(std::istream& AStream, TSaveFormat AFormat);

    /// Destructeur
    ~CDartVertex();

    // @}
    // @name Accesseur au sommet éclaté
    // @{

    CVertex & getBurstVertex();
    void      setBurstVertex(const CVertex & AVertex);

    // @}
    // @name Méthodes de sauvegarde et de chargement
    // @{

    /**
     *  Sauvegarde le contenu du brin dans le flot AStream en fonction du format
     *  demandé.
     *  Remarque: Lorsque cette méthode est appelée, on suppose que tous les
     *  brins ont été numérotés et que ces numéros ont été placés dans un champ
     *  'directInfo' dont l'indice est passé en paramètre.
     *
     *  @param AStream Un flot où sauvegarder le brin
     *  @param AFormat Le type de format dans lequel doit se faire la sauvegarde
     *  @param ADirectInfoIndex Un indice indiquant où sont stockés les numéros
     *   des brins
     *  @return 'vrai' si tout s'est bien passé
     */
    bool save(std::ostream& AStream,
	      TSaveFormat AFormat, int ADirectInfoIndex) const;

    /**
     * Charge le contenu d'un brin dans le flot AStream en fonction du format
     * demandé.
     * Remarque: À l'issue de cette méthode, les champs alphaI du brin ne sont
     * pas valides: ils correspondent à des numéros de brins et non pas à des
     * pointeurs sur des brins. Ces champs doivent être encore modifiés par la
     * méthode globale de chargement, située dans la classe GmapV.
     *
     * @param AStream Un flot dans lequel lire le contenu du brin
     * @param AFormat Le type de format dans lequel le flot est codé
     * @return 'vrai' si tout s'est bien passé
     */
    bool load(std::istream& AStream, TSaveFormat AFormat);

    // @}

  private:
    // @name Attributs privés
    // @{

    /// Plongement éclaté du brin
    CVertex FBurstVertex;

    // @}
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("dart-vertex.icc")
//******************************************************************************
#endif // DART_VERTEX_HH
//******************************************************************************
