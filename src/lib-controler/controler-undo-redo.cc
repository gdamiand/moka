/*
 * lib-controler : Un contrôleur générique de scène 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler
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
#include "controler.hh"
using namespace std;
//******************************************************************************
bool CControler::getUndoOnFile() const
{
    return FUndoOnFile;
}
//******************************************************************************
void CControler::setUndoOnFile(bool ABool)
{
    if (ABool!=FUndoOnFile)
    {
        emptyUndoRedo();
        FUndoOnFile = ABool;
    }
}
//******************************************************************************
int CControler::getNbMaxUndos() const
{
    return FNbMaxUndos;

}
//******************************************************************************
void CControler::setNbMaxUndos(int ANumber)
{
    assert(ANumber==-1 || ANumber>=0);

    if ( ANumber!=-1 && ANumber<FNbMaxUndos )
    {
        if (FActu != NULL)
        {
            FRedos.push_front(FActu);
            --FLastFileIndex;
            FActu = NULL;
        }
        emptyRedoList();

        int nbToRemove = FUndos.size()-ANumber;
        for (int i=0; i<nbToRemove; ++i)
        {
            if (FUndoOnFile) remove(FUndos.back()->str().c_str());
            delete FUndos.back();
            FUndos.pop_back();
        }
    }

    FNbMaxUndos = ANumber;
    saveLastUndoOnFile();
}
//******************************************************************************
bool CControler::undo(int AStep)
{
    if (!canApplyOperation(COperation(OPERATION_UNDO_REDO)))
        return false;

    if (!basicUndo(AStep))
        return false;

    setMessage(string("Undo [") + getUndoRedoStatus() + "]");
    return true;
}
//******************************************************************************
bool CControler::redo(int AStep)
{
    if (!canApplyOperation(COperation(OPERATION_UNDO_REDO)))
        return false;

    if (!basicRedo(AStep))
        return false;

    setMessage(string("Redo [") + getUndoRedoStatus() + "]");
    return true;
}
//******************************************************************************
void CControler::emptyUndoList()
{
    while (!FUndos.empty())
    {
        if (FUndoOnFile) remove(FUndos.front()->str().c_str());
        delete FUndos.front();
        FUndos.pop_front();
    }

    FLastFileIndex = 0;
    saveLastUndoOnFile();
}
//******************************************************************************
void CControler::emptyRedoList()
{
    while (!FRedos.empty())
    {
        if (FUndoOnFile) remove(FRedos.front()->str().c_str());
        delete FRedos.front();
        FRedos.pop_front();
    }
}
//******************************************************************************
bool CControler::emptyUndoRedo()
{
    if (FActu != NULL)
    {
        FRedos.push_front(FActu);
        FActu = NULL;
    }

    emptyRedoList();
    emptyUndoList();

    setMessage("Undo/redo stack is now empty");
    return true;
}
//******************************************************************************
string CControler::getFilename(int AIndex) const
{
    assert(AIndex >= 0);

    stringstream s;
    s<<FConfigDirectory<<"/.moka-undo-"<<AIndex;
    return s.str();
}
//******************************************************************************
int CControler::getNewFileIndex()
{
    ++FLastFileIndex;

    if ( FNbMaxUndos!=-1 )
    {
        while (FUndos.size() > (unsigned int)FNbMaxUndos)
        {
            ostringstream* os = FUndos.back();
            FUndos.pop_back();
            if (FUndoOnFile) remove(os->str().c_str());
            delete os;
        }
    }

    return FLastFileIndex;
}
//******************************************************************************
ostringstream* CControler::saveModel()
{
    return NULL;
}
//------------------------------------------------------------------------------
bool CControler::loadModel(ostringstream * /* AStream */)
{
    return true;
}
//******************************************************************************
bool CControler::basicUndo(int AStep)
{
    if (AStep == 0)
        return true;

    if (AStep < 0)
        return basicRedo(-AStep);

    if (FUndos.empty())
        return false;

    // Premier undo :
    if (FActu == NULL)
        FActu = saveModel();

    // Sauvegarde de l'état actuel dans la pile des redos :
    FRedos.push_front(FActu);

    // Déplacement de AStep-1 éléments de la pile des undos vers celle des redos :
    for (int i = 1; i < AStep && 1 < FUndos.size(); ++i)
    {
        FRedos.push_front(FUndos.front());
        FUndos.pop_front();
        --FLastFileIndex;
    }

    // Lecture de la carte dans la pile des undos :
    assert(! FUndos.empty());
    FActu = FUndos.front();
    FUndos.pop_front();
    --FLastFileIndex;
    loadModel(FActu);

    saveLastUndoOnFile();

    return true;
}
//------------------------------------------------------------------------------
bool CControler::basicRedo(int AStep)
{
    if (AStep == 0)
        return true;

    if (AStep < 0)
        return basicUndo(-AStep);

    if (FRedos.empty())
        return false;

    assert(FActu != NULL);

    // Sauvegarde de l'état actuel dans la pile des undos :
    FUndos.push_front(FActu);

    // Déplacement de AStep-1 éléments de la pile des redos vers celle des undos :
    for (int i = 1; i < AStep && 1 < FUndos.size(); ++i)
    {
        FUndos.push_front(FRedos.front());
        ++FLastFileIndex;
        FRedos.pop_front();
    }

    // Lecture de la carte dans la pile des undos :
    assert(! FRedos.empty());
    FActu = FRedos.front();
    FRedos.pop_front();
    loadModel(FActu);
    ++FLastFileIndex;

    saveLastUndoOnFile();

    return true;
}
//******************************************************************************
void CControler::undoRedoPreSave()
{
    basicPreSave();
}
//------------------------------------------------------------------------------
void CControler::undoRedoPostSaveOk()
{
    basicPostSaveOk();
    FLastOperation = FCurrentOperation;
}
//------------------------------------------------------------------------------
void CControler::undoRedoPostSaveFailed()
{
    basicPostSaveFailed();
}
//******************************************************************************
void CControler::basicPreSave()
{
    if (FNbMaxUndos==0) return;

    if ( FActu!=NULL )
    {
        if (FUndoOnFile) remove(FActu->str().c_str());
        delete FActu;
        --FLastFileIndex;
    }

    FActu = saveModel();
}
//------------------------------------------------------------------------------
void CControler::basicPostSaveOk()
{
    if (FNbMaxUndos==0) return;

    assert( FActu!=NULL );

    // Sauvegarde de FActu :
    FUndos.push_front(FActu);
    FActu = NULL;

    emptyRedoList();
    saveLastUndoOnFile();
}
//------------------------------------------------------------------------------
void CControler::basicPostSaveFailed()
{
    if (FNbMaxUndos==0) return;

    assert(FActu != NULL);
}
//******************************************************************************
string CControler::getUndoRedoStatus() const
{
    stringstream s;
    s<< FUndos.size()<<" * "<<FRedos.size();
    return s.str();
}
//******************************************************************************
bool CControler::existsFile(const string & AFilename)
{
    FILE * f;
    f=fopen(AFilename.c_str(), "r");

    if (f==NULL)
        return false;

    fclose(f);
    return true;
}
//******************************************************************************
void CControler::saveLastUndoOnFile()
{
    if (!FUndoOnFile) return;

    stringstream s;
    s<<FConfigDirectory<<"/.moka-last-undo-file";

    ofstream os(s.str().c_str());
    if (os.is_open())
    {
        os<<FLastFileIndex<<endl;
        os.close();
    }
}
//******************************************************************************
void CControler::recupLastUndoFromFile()
{
    if (!FUndoOnFile) return;

    stringstream s;
    s<<FConfigDirectory<<"/.moka-last-undo-file";

    ifstream is(s.str().c_str());;
    if (is.is_open())
    {
        is>>FLastFileIndex;
        is.close();
    }
    else
    {
        FLastFileIndex = 0;
    }
}
//******************************************************************************
void CControler::recupFromFiles()
{
    recupLastUndoFromFile();

    if (!FUndoOnFile || FNbMaxUndos==0 || FLastFileIndex==0) return;

    string filename;
    int nbFilesRecup = 0;

    int current = FLastFileIndex;
    bool undoFull = false;
    ostringstream * os;

    filename = getFilename(current);
    while (existsFile(filename) && !undoFull)
    {
        os = new ostringstream;
        (*os)<<filename.c_str();

        FUndos.push_back(os);

        ++nbFilesRecup;
        if ( nbFilesRecup==FNbMaxUndos )
            undoFull = true;

        --current;
        filename = getFilename(current);
    }
}
//******************************************************************************
