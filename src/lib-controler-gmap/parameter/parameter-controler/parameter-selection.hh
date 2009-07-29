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
#ifndef PARAMETER_SELECTION_HH
#define PARAMETER_SELECTION_HH
//******************************************************************************
#include "parameter.hh"
#include "kernel-types.hh"
//******************************************************************************
namespace GMap3d
{
  class CControlerGMap;
  class CDart;

  class CParameterSelection : public CParameter
  {
  public:
    //@{

    CParameterSelection(CControlerGMap* AMap,
			int ANbSelectionLevels,
			int ANbRef = 0);
    
    CParameterSelection(const CParameterSelection &);
    virtual ~CParameterSelection();
    virtual CParameter * copy() const;
    //@}

    //@{

    virtual void save(std::ostream &);
    virtual void load(std::istream &);
    virtual void reinit();

    //@}

    //@{

    // Pour récupérer les informations de sélection
    int    getNbSelectionLevels() const;

    int    getSelectionLevel    ()             const;
    int    getNextSelectionLevel(int AIndex=1) const;
    void   setSelectionLevel    (int ALevel);

    bool   getHalfSelection() const;
    void   setHalfSelection(bool ABool);

    // Pour récupérer les marques de sélections
    int    getSelectionMark    ()             const;
    int    getSelectionMark    (int ALevel)   const;
    int    getNextSelectionMark(int AIndex=1) const;

    // Accéder à l'orbite de selection.
    TOrbit getSelectionOrbit    () const;
    void   setSelectionOrbit    (TOrbit AOrbit);
    void   setNextSelectionOrbit();

    // Pour récupérer les dernier brins sélectionnés
    CDart* getLastSelectedDart    ()             const;
    CDart* getLastSelectedDart    (int ALevel)   const;
    CDart* getNextLastSelectedDart(int AIndex=1) const;

    // Sélection des dernier brins
    void setLastSelectedDart(CDart* ALastDart);
    void setLastSelectedDart(int ALevel, CDart* ALastDart);

    // Déselection des dernier brins
    void unsetLastSelectedDart    ();
    void unsetLastSelectedDart    (int ALevel);
    void unsetNextLastSelectedDart(int AIndex=1);
    void unsetAllLastSelectedDarts();

    // Pour échanger les niveaux de sélections.
    void swapLevels(int ALevel1, int ALevel2);

    ///@return la valeur de FChanged.
    bool getChanged() const;

    /// Met à vrai la variable FChanged et répercute aux précompiles concernés.
    void setChanged();

    /// Met à faux la variable FChanged sans repercution sur les précompiles.
    void unsetChanged();

    /// Identificateur du type de paramètre.
    virtual int getType() const;
    
    // Pour sélectionner ou déséllectionner une orbite selon son état actuel.
    // Utilisé par le picking.
    void toggleOrbitSelection    (CDart* ADart);

    //@}

  private:

    //@{

    CControlerGMap* FControlerGMapV;

    int     FNbLevels;
    int     FCurrentLevel;
    int*    FMarks;
    CDart** FLasts;

    TOrbit FSelectionOrbit;
    bool   FHalfSelection;

    /// Booléen indiquant si la sélection à changé depuis la dernière mise à jour.
    bool FChanged;
    
    //@}
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_SELECTION_HH
//******************************************************************************
