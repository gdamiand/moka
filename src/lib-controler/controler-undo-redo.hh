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
// Ce fichier est une partie de "controler.hh".
//******************************************************************************
//@{ Undo - Redo

public:

// Accesseurs pour le nombre maximum d'undo possibles.
// Si -1 : pas de limite...
// Attention, le set vide eventuellement la pile des redo.
int  getNbMaxUndos() const;
void setNbMaxUndos(int ANumber);

// Accesseurs pour savoir si on fait les undo-redo sur fichier
// Attention, le set vide tout les undo-redo.
bool getUndoOnFile() const;
void setUndoOnFile(bool ABool);

/** undo
 *
 *  Termine l'opération en cours (si nécessaire) et passe en mode MODE_SELECTION
 *  avant d'effectuer l'undo.
 */
virtual bool undo(int AStep = 1);
virtual bool redo(int AStep = 1);

/** emptyUndoRedo
 *
 * Pour vider la pile des undo / redo
 */
bool emptyUndoRedo();

protected:

// Les méthodes appelées avant et après les opérations.
// A redéfinir pour ajouter des pré ou post traitements.
virtual void undoRedoPreSave();
virtual void undoRedoPostSaveOk();
virtual void undoRedoPostSaveFailed();

//******************************************************************************

/** @name Méthodes de sauvegarde dans la pile d'undo/redo :
 */
//@{
void emptyUndoList();
void emptyRedoList();

void basicPreSave();
void basicPostSaveOk();
void basicPostSaveFailed();

std::string getUndoRedoStatus() const;

std::string getFilename(int AIndex) const;
int         getNewFileIndex();

virtual std::ostringstream* saveModel();
virtual bool                loadModel(std::ostringstream* AStream);

bool existsFile(const std::string& AFilename);
void recupLastUndoFromFile();
void saveLastUndoOnFile();
void recupFromFiles();

bool basicUndo(int AStep = 1);
bool basicRedo(int AStep = 1);
//@}

protected:
bool                           FUndoOnFile;
int                            FNbMaxUndos;
int                            FLastFileIndex;
std::list<std::ostringstream*> FUndos;
std::list<std::ostringstream*> FRedos;
std::ostringstream*            FActu;

//@}

//******************************************************************************
