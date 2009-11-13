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
#include "controler-gmap.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
ostringstream* CControlerGMap::saveModel()
{
    updateModelIfNeeded    ();
    updateSelectionIfNeeded();

    ostream* os;
    string filename = getFilename(getNewFileIndex());

    if (FUndoOnFile)
        os = new ofstream(filename.c_str());
    else
        os = new ostringstream;

    assert( os!=NULL );

    int nbLevels = getNbSelectionLevels();
    int* lasts   = new int[nbLevels];

    int i,j;

    // Recherche de l'index des brins "last" dans la liste de tous les brins :
    for (i=0; i<nbLevels; ++i)
        lasts[i] = -1;

    CDynamicCoverageAll it(FMap);

    for (i=0; it.cont(); ++it, ++i)
        for (j=0; j<nbLevels; ++j)
            if (*it == getLastSelectedDart(j))
                lasts[j] = i;

    // On sauve les lasts
    for (i=0; i<nbLevels; ++i)
        (*os) << lasts[i] << " ";

    // Puis les opérations
    (*os) << FLastOperation << " ";

    // Et enfin la carte
    if (! FMap->save(*os, BinaryFormat))
        assert(false);

    delete [] lasts;

    ostringstream * res;

    if (FUndoOnFile)
    {
        os->flush();
        delete os;
        res = new ostringstream;
        (*res) << filename;
    }
    else
        res = static_cast<ostringstream*>(os);

    return res;
}
//------------------------------------------------------------------------------
bool CControlerGMap::loadModel(ostringstream * AStream)
{
    istream * is;

    if (FUndoOnFile)
    {
        is = new ifstream(AStream->str().c_str());
        if (!is->good())
        {
            delete is;
            return false;
        }
    }
    else
        is = new istringstream(AStream->str());

    int nbLevels = getNbSelectionLevels();
    int* lasts   = new int[nbLevels];
    int i,k,n;

    // On charge les last
    for (i=0; i<nbLevels; ++i)
        (*is) >> lasts[i];

    // Puis les opérations
    (*is) >> FLastOperation;

    is->get(); // Pour passer le dernier espace avant le contenu de la carte

    FMap->removeAllDarts();

    setModelChanged    ();
    setSelectionChanged();

    for (i=0; i<nbLevels; ++i)
        unsetLastSelectedDart(i);

    // Et la carte
    if ( FMap->load(*is, BinaryFormat)==NULL )
    {
        FMap->removeAllDarts();
        delete [] lasts;
        delete is;
        return false;
    }

    CDynamicCoverageAll it(FMap);

    for (n=0; it.cont(); ++it)
        ++n;

    for (i=0, it.reinit(); it.cont(); ++it, ++i)
        for (k=0; k<nbLevels; ++k)
            if (lasts[k] == n-i-1)
                selectDart(*it, k); // Ce sera le dernier brin sélectionné

    delete [] lasts;
    delete is;

    assert(FMap->checkTopology());
    assert(FMap->checkEmbeddings(ORBIT_VERTEX, ATTRIBUTE_VERTEX, true));

    return true;
}
//******************************************************************************
