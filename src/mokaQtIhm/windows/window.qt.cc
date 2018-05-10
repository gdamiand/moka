/*
 * Moka : Un modeleur de 3-G-cartes.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of Moka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "window.qt.hh"
#include "HtmlEntities.hh"
//#include "icones.hh"
#include "gl-multi-window.qt.hh"

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <QtGui/QPixmap>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMdiSubWindow>
//#include <Qt3Support/Q3Accel>


//***************************************
// Constructeur
//***************************************
Window :: Window() :
      QMainWindow(),
      LINK_COLOR(QColor(53, 200, 255)),
      FControler(new CControlerGMap),
      FCreationActive(NULL),
      FOperationActive(NULL),
      FOptionsFrame(NULL),
      FOperationChanfreinage(NULL),
      FOptionsCarac(NULL),
      FOptionsSurfacicHomology(NULL),
      FOptionsVolumicHomology(NULL),
      FDialogDo(NULL) ,
      FDoubleCliquee(NULL),
      FCouleurs(NULL),
      color_active(false),
      current_dir("."),
      FileDialog(NULL)
{
   setWindowTitle("MOKA");
   resize(800 , 600) ;

   setWindowIcon(QIcon(":/moka-logo.png")) ;

   // Creation des boites a outil
   creationBoitesOutils() ;

   // Creation du menu
   main_menu = new MenuBar(this) ;
   setMenuBar(main_menu);

   // Creation du workspace
   FWorkspace = new QMdiArea(this) ;
   setCentralWidget(FWorkspace) ;
   connect(FWorkspace, SIGNAL(subWindowActivated(QMdiSubWindow *)),
           this, SLOT(subWindowActivated(QMdiSubWindow *)));

   // Affectation du mode du controleur
   FControler -> setMode(MODE_SELECTION) ;
   FControler -> setUndoOnFile(true) ;

   // Fenetre mere

   FVueMere = new GLWindow(VIEW_XYZ , FWorkspace , this , FSelection) ;

   string str = "Main window: " + FVueMere->getViewTypeString();
   FVueMere -> setWindowTitle(*HTML::decode(&str));
   FVueMere -> setMinimumSize(200 , 200) ;
   FVueMere -> showMaximized() ;

   // Création des racourcis clavier : todo passer tout en QT4
//   Q3Accel* Raccourci = new Q3Accel(this) ;

//   Raccourci -> insertItem(QKeySequence("Alt+F9") , 8) ;
//   Raccourci -> connectItem(8 , this ,
//                            SLOT(callbackToggleNormal())) ;

//   Raccourci -> insertItem(QKeySequence(Qt::Key_F9) , 9) ;
//   Raccourci -> connectItem(9 , this ,
//                            SLOT(callbackToggleSews())) ;

//   Raccourci -> insertItem(QKeySequence(Qt::Key_F10) , 10) ;
//   Raccourci -> connectItem(10 , this ,
//                            SLOT(callbackToggleVertices())) ;

//   Raccourci -> insertItem(QKeySequence(Qt::Key_F11) , 11) ;
//   Raccourci -> connectItem(11 , this ,
//                            SLOT(callbackToggleFaces())) ;

//   Raccourci -> insertItem(QKeySequence(Qt::Key_F12) , 12) ;
//   Raccourci -> connectItem(12 , this ,
//                            SLOT(callbackToggleGrille())) ;

//   Raccourci -> insertItem(QKeySequence("Alt+F11") , 14) ;
//   Raccourci -> connectItem(14 , this ,
//                            SLOT(callbackTournerButton())) ;

//   Raccourci -> insertItem(QKeySequence(Qt::Key_Space) , 1) ;
//   Raccourci -> connectItem(1 , this ,
//                            SLOT(callbackHideAllWindow())) ;

//   Raccourci -> insertItem(QKeySequence(Qt::Key_Up) , 15) ;
//   Raccourci -> connectItem(15, this, SLOT(callbackKeyUp()));

//   Raccourci -> insertItem(QKeySequence(Qt::Key_Down) , 16) ;
//   Raccourci -> connectItem(16, this, SLOT(callbackKeyDown()));

//   Raccourci -> insertItem(QKeySequence(Qt::Key_Left) , 17) ;
//   Raccourci -> connectItem(17, this, SLOT(callbackKeyLeft()));

//   Raccourci -> insertItem(QKeySequence(Qt::Key_Right) , 18) ;
//   Raccourci -> connectItem(18, this, SLOT(callbackKeyRight()));

//   Raccourci -> insertItem(QKeySequence(Qt::CTRL + Qt::Key_Up) , 19) ;
//   Raccourci -> connectItem(19, this, SLOT(callbackKeyCtrlUp()));

//   Raccourci -> insertItem(QKeySequence(Qt::CTRL + Qt::Key_Down) , 20) ;
//   Raccourci -> connectItem(20, this, SLOT(callbackKeyCtrlDown()));

//   Raccourci -> insertItem(QKeySequence(Qt::CTRL + Qt::Key_Left) , 21) ;
//   Raccourci -> connectItem(21, this, SLOT(callbackKeyCtrlLeft()));

//   Raccourci -> insertItem(QKeySequence(Qt::CTRL + Qt::Key_Right) , 22) ;
//   Raccourci -> connectItem(22, this, SLOT(callbackKeyCtrlRight()));

//   Raccourci -> insertItem(QKeySequence(Qt::SHIFT + Qt::Key_Up) , 23) ;
//   Raccourci -> connectItem(23, this, SLOT(callbackKeyShiftUp()));

//   Raccourci -> insertItem(QKeySequence(Qt::SHIFT + Qt::Key_Down) , 24) ;
//   Raccourci -> connectItem(24, this, SLOT(callbackKeyShiftDown()));

//   Raccourci -> insertItem(QKeySequence(Qt::SHIFT + Qt::Key_Left) , 25) ;
//   Raccourci -> connectItem(25, this, SLOT(callbackKeyShiftLeft()));

//   Raccourci -> insertItem(QKeySequence(Qt::SHIFT + Qt::Key_Right) , 26);
//   Raccourci -> connectItem(26, this, SLOT(callbackKeyShiftRight()));

//   Raccourci -> insertItem(QKeySequence(Qt::SHIFT + Qt::Key_F8) , 27) ;
//   Raccourci -> connectItem(27 , this , SLOT(callbackVueCompacte())) ;

//   Raccourci -> insertItem(QKeySequence(Qt::SHIFT + Qt::Key_F9) , 28) ;
//   Raccourci -> connectItem(28 , this , SLOT(callbackVueSemiEclatee()));

//   Raccourci -> insertItem(QKeySequence(Qt::SHIFT + Qt::Key_F10) , 29) ;
//   Raccourci -> connectItem(29 , this , SLOT(callbackVueMoka())) ;

//   Raccourci -> insertItem(QKeySequence(Qt::SHIFT + Qt::Key_F11) , 30) ;
//   Raccourci -> connectItem(30 , this , SLOT(callbackVueTopoFil())) ;

   // Creation des Toolbars
   FCreationBrin      = new CreationBrin(this , FControler);
   FCreationCylindre  = new CreationCylindre(this , FControler);
   FCreationMaillage  = new CreationMaillage(this , FControler);
   FCreationPolygone  = new CreationPolygone(this , FControler);
   FCreationPyramide  = new CreationPyramide(this , FControler);
   FCreationSphere    = new CreationSphere(this , FControler);
   FCreationTore      = new CreationTore(this , FControler);
   FDialogRotation    = new dialogRotation(this , FControler);
   FDialogHomothetie  = new dialogHomothetie(this , FControler);
   FDialogTranslation = new dialogTranslation(this , FControler);
   FDialogMoveto      = new dialogMoveto(this , FControler);
   FCouleurs          = new OptionCouleurs(this) ;

   updateStatusBar() ;
}

//***************************************
// Destructeur
//***************************************
Window :: ~Window()
{ }

//***************************************
// Accesseur sur le controleur
//***************************************
GMap3d :: CControlerGMap * Window :: getControler() const
{
   return FControler ;
}

void Window :: setDoubleCliquee(GLWindow * vue)
{
   string str;

   if (FDoubleCliquee != NULL)
   {
      if (FDoubleCliquee == FVueMere)
      {
         str = "M&egrave;re : " + FVueMere->getViewTypeString();
         FVueMere->setWindowTitle(*HTML::decode(&str));
      }

      else
      {
         str = FDoubleCliquee -> getViewTypeString();
         FDoubleCliquee->setWindowTitle(*HTML::decode(&str));
      }
   }

   FDoubleCliquee = vue ;

   if (FDoubleCliquee == FVueMere)
   {
      str = "M&egrave;re : " + FVueMere->getViewTypeString()
            + " : Double-Click";
      FVueMere->setWindowTitle(*HTML::decode(&str));
   }
   else
   {
      str = FDoubleCliquee->getViewTypeString() + " : Double-Click";
      FDoubleCliquee->setWindowTitle(*HTML::decode(&str));
   }
}
//***************************************
// Methode qui demande a toutes les fenetres de se redessiner
//***************************************
void Window::repaint()
{
   static bool is_repainting = false;   // Evite les recursions infinies.

   if (!is_repainting)
   {
      is_repainting = true;
      QMainWindow::repaint() ;      // Appel de la methode de la classe mere

      // Appel de la methode sur toutes les vues ouvertes
      QList<QMdiSubWindow*> vues = FWorkspace -> subWindowList() ;
      for (int i = 0 ; i < int (vues . count()) ; i++)
        ((GLWindow *) vues.at(i)->widget())->updateGL(); //paintGL(); //update(); //repaint() ;

      updateStatusBar();
      is_repainting = false;
   }
}

void Window::closeEvent(QCloseEvent *)
{
   FControler -> saveAllParameters(getCurrentViewId());
}

void Window::subWindowActivated(QMdiSubWindow * w)
{
   if (w == NULL) return;

   CreationObjet * boite = getCreationActive() ;
   if (boite != NULL)
      boite -> update() ;

   dialogOperations * op = getOperationActive() ;
   if (op != NULL)
      op -> update() ;

   OptionCouleurs * col = getCouleurActive();
   if (col != NULL)
      col->changeColor();

   if (FOptionsFrame != NULL && FOptionsFrame -> isVisible())
      FOptionsFrame -> update() ;

   OptionsCarac * carac = getOptionsCaracActive() ;
   if (carac != NULL) carac -> update() ;
}

//*****************************************
// Gestion de la barre de menu
//*****************************************
void Window::updateStatusBar()
{
   int darts    = getControler()->getNbDarts();
   int selected = getControler()->getNbSelectedDarts();
   int vertices = getControler()->getNbVertices();

   QString texte = "Darts: " ;
   texte += QString :: number(darts) ;
   texte += ";  Selected: " ;
   texte += QString :: number(selected) ;
   texte += ";  Vertices: " ;
   texte += QString :: number(vertices) ;
   texte += ";  " ;
   texte += QString::fromUtf8(getControler()->getMessage().c_str()) ;
   statusBar() -> showMessage(texte) ;
}

//*****************************************
// Accesseur sur les boites de dialogues
//*****************************************
CreationObjet * Window :: getCreationActive() const
{
   return FCreationActive ;
}

dialogOperations * Window :: getOperationActive() const
{
   return FOperationActive ;
}

OptionCouleurs * Window :: getCouleurActive() const
{
   return FCouleurs ;
}

OptionsCarac * Window :: getOptionsCaracActive() const
{
   return FOptionsCarac ;
}

OptionsSurfacicHomology * Window::getOptionsSurfacicHomologyActive() const
{
   return FOptionsSurfacicHomology ;
}

OptionsVolumicHomology * Window::getOptionsVolumicHomologyActive() const
{
   return FOptionsVolumicHomology ;
}

//*****************************************
// Methode qui bascule l'affichage
//*****************************************
void Window :: bascule(TView type)
{
   assert(FWorkspace->activeSubWindow()->widget() != NULL);

   bool find = false;
   int  actu = 0;
   int  first;

   // On cherche tout d'abord la vue active.
   QList<QMdiSubWindow*> vues = FWorkspace -> subWindowList() ;

   while (!find && actu < int(vues.count()))
   {
      if ((GLWindow*)FWorkspace->activeSubWindow()->widget() == (GLWindow*)vues.at(actu)->widget())
         find = true;
      else
         ++actu;
   }

   assert(find);

   // Puis on part de cette vue active et on cherche la suivante de meme type.
   find  = false;
   first = actu;
   actu  = (actu + 1) % vues.count();

   while (!find && actu != first)
   {
      if (((GLWindow*)vues.at(actu))->getViewType() == type)
         find = true;
      else
         actu = (actu + 1) % vues.count();
   }

   if (find)
   {
      // Soit on active la vue
      ((GLWindow*)vues.at(actu))->setFocus();
   }
   else
   {
      if (((GLWindow*)FWorkspace->activeSubWindow()->widget())->getViewType() != type)
      {
         // Soit on cree une vue avec le bon type.
         switch (type)
         {
            case VIEW_XYZ :
               addView3D() ;
               break ;
            case VIEW_XY :
               addViewXY() ;
               break ;
            case VIEW_YZ :
               addViewYZ() ;
               break ;
            case VIEW_XZ :
               addViewZX() ;
               break ;
            case VIEW_ISO :
               addViewMulti() ;
               break ;
         }
      }
   }
}

//*****************************************
// Methode qui fabrique les barres d'outils
//*****************************************
void Window :: creationBoitesOutils()
{
   QIcon pf0(":/nouveau.png") ;
   QIcon pf1(":/charger.png") ;
   QIcon pf2(":/sauver.png") ;
   QIcon pf5(":/file-import-48x48.png") ;
   QIcon pf3(":/file-export-48x48.png") ;
   QIcon pf4(":/ajouter.png") ;

   QIcon pe1(":/undo.png") ;
   QIcon pe2(":/redo.png") ;
   QIcon pe3(":/vider.png") ;

   QIcon pc1(":/polyligne.png") ;
   QIcon pc2(":/polygone.png") ;
   QIcon pc3(":/maillage.png") ;
   QIcon pc4(":/sphere.png") ;
   QIcon pc5(":/cylindre.png") ;
   QIcon pc6(":/cone.png") ;
   QIcon pc7(":/tore.png") ;

   QIcon pv1(":/addvue.png") ;
   QIcon pv2(":/deletevue.png") ;
   QIcon pv3(":/groupervues.png") ;
   QIcon pv4(":/degroupervues.png") ;

   QIcon pop1(":/homothetie.png") ;
   QIcon pop2(":/translation.png") ;
   QIcon pop3(":/rotation.png") ;

   QIcon psel1(":/sel_prev.png");
   QIcon psel2(":/sel_next.png");
   QIcon psel3(":/suppression.png");
   QIcon psel4(":/suppression_forcee.png");

   // Creation de la boite a outil fichier
   fichier = new QToolBar("File" , this) ;
   fichier -> addAction(pf0, "New", this, SLOT(callbackNettoyage()));
   charger_act = fichier -> addAction(pf1, "Load", this,
                                      SLOT(callbackLoad()));
   fichier -> addAction(pf4, "Add", this, SLOT(callbackAdd()));
   sauver_act = fichier -> addAction(pf2, "Save", this,
                                     SLOT(callbackSave()));
   exporter_act = fichier -> addAction(pf3, "Export", this,
                                       SLOT(callbackExport()));
   importer_act = fichier -> addAction(pf5, "Import", this,
                                       SLOT(callbackImport()));
   addToolBar(fichier);

   // Creation de la boite a outil edition
   edition = new QToolBar("Undo/Redo", this) ;
   edition -> addAction(pe1, "Undo", this, SLOT(callbackUndo()));
   edition -> addAction(pe2, "Redo", this, SLOT(callbackRedo()));
   edition -> addSeparator() ;
   edition -> addAction(pe3, "Empty the stack", this,
                        SLOT(callbackEmptyUndoRedo()));
   addToolBar(edition);

   // Creation de la boite a outil creations
   creation = new QToolBar("Creation", this) ;
   creation -> addAction(pc1, "Create polyline/dart/face",
                         this, SLOT(callbackPolyline()));
   creation -> addAction(pc2, "Create polygon", this, SLOT(callbackPolygon()));
   creation -> addAction(pc3, "Create mesh",
			 this, SLOT(callbackMeshCreation()));
   creation -> addAction(pc4, "Create sphere",this, SLOT(callbackSphere()));
   creation -> addAction(pc5, "Create cylinder",
			 this,SLOT(callbackCylinder()));
   creation -> addAction(pc6, "Create cone/pyramid",
			 this, SLOT(callbackPyramide()));
   creation -> addAction(pc7, "Create torus", this,SLOT(callbackTorus()));
   addToolBar(creation);

   // Creation de la boite a outils operations
   operations = new QToolBar("Operations", this) ;
   operations -> addAction(pop2, "Translation", this,
                           SLOT(callbackTranslateWindow()));
   operations -> addAction(pop3, "Rotation", this,
                           SLOT(callbackRotateWindow()));
   operations -> addAction(pop1, "Scale", this,
                           SLOT(callbackScaleWindow()));
   addToolBar(operations);


   // Creation de la boite a outils vues
   vues = new QToolBar("View", this) ;
   vues -> addAction(pv1, "Add a 3D view", this, SLOT(addView3D()));
   vues -> addAction(pv2, "Remove the view", this, SLOT(deleteView()));
   vues -> addSeparator() ;
   vues -> addAction(pv3, "Group all the views", this,
                     SLOT(OperationGroupAllGeneral()));
   vues -> addAction(pv4, "Ungroup all the views", this,
                     SLOT(OperationUngroupAllGeneral()));
   addToolBar(vues);

   // Creation de la boite a outils selection
   FSelection = new SelectBar(this) ;
   addToolBar(Qt::LeftToolBarArea, FSelection);
}

// Ouvre une boite de dialogue demandant le nom du fichier a ouvrir.
// Retourne l'adresse complete de ce fichier.
// Si 'ind_filter' est different de NULL, l'indice du filtre selectionne
// par l'utilisateur sera ecrit dans le pointeur.
std::string Window::getOpenFileName(const QString & caption,
                                    const QStringList & filters,
                                    int * ind_filter)
{
   QString filename;
   QFileDialog open_dialog(0, caption, current_dir.path(), filters.join(";;"));

   if (ind_filter != NULL) *ind_filter = -1;
   open_dialog.setAcceptMode(QFileDialog::AcceptOpen);
   open_dialog.setFileMode(QFileDialog::ExistingFile);

   if (open_dialog.exec())
   {
      filename = open_dialog.selectedFiles().at(0);
      current_dir = open_dialog.directory();

      if (ind_filter != NULL)
      {
         for ((*ind_filter) = 0;
               filters.at(*ind_filter) != open_dialog.selectedNameFilter();
               (*ind_filter)++);
      }
      return filename.toStdString();
   }
   return std::string("");
}

void Window::filterSelected(const QString & selected)
{
   QRegExp rxfilter(".*\\(.*\\.(.*)\\).*");
   int pos = rxfilter.indexIn(selected);
   if (pos > -1)
   {
      FileDialog->setDefaultSuffix(rxfilter.cap(1));
   }
}

// Ouvre une boite de dialogue demandant le nom du fichier a ouvrir.
// Retourne l'adresse complete de ce fichier.
// Si 'ind_filter' est different de NULL, l'indice du filtre selectionne
// par l'utilisateur sera ecrit dans le pointeur.
std::string Window::getSaveFileName(const QString & caption,
                                    const QStringList & filters,
                                    int * ind_filter)
{
   assert(FileDialog == NULL);

   QString filename;
   FileDialog = new QFileDialog(0, caption, current_dir.path(), filters.join(";;"));

   connect(FileDialog, SIGNAL(filterSelected(const QString &)),
           this, SLOT(filterSelected(const QString &)));

   if (ind_filter != NULL) *ind_filter = -1;
   FileDialog->setAcceptMode(QFileDialog::AcceptSave);
   FileDialog->setFileMode(QFileDialog::AnyFile);
   filterSelected(filters.at(0));

   if (FileDialog->exec())
   {
      filename = FileDialog->selectedFiles().at(0);
      current_dir = FileDialog->directory();

      if (ind_filter != NULL)
      {
         for ((*ind_filter) = 0;
               filters.at(*ind_filter) != FileDialog->selectedNameFilter();
               (*ind_filter)++);
      }

      delete FileDialog; FileDialog = NULL;
      return filename.toStdString();
   }
   delete FileDialog; FileDialog = NULL;
   return std::string("");
}

//------------------------------------------------------------------------------
bool Window :: getVerticesDisplay() const
{
   return getControler()->viewIsEnabledPrecompile(getCurrentViewId(),
          PRECOMPILE_VERTEX);
}
void Window :: setVerticesDisplay(bool b)
{
   if (getVerticesDisplay() != b)
   {
      getControler()->viewTogglePrecompile(getCurrentViewId(),
                                           PRECOMPILE_VERTEX);
   }
}
//------------------------------------------------------------------------------
bool Window :: getNormalDisplay() const
{
   return getControler()->viewIsEnabledPrecompile(getCurrentViewId(),
          PRECOMPILE_NORMAL_VECTOR);
}
void Window :: setNormalDisplay(bool b)
{
   if (getNormalDisplay() != b)
   {
      getControler()->viewTogglePrecompile(getCurrentViewId(),
                                           PRECOMPILE_NORMAL_VECTOR);
   }
}
//------------------------------------------------------------------------------
bool Window :: getFacesDisplay() const
{
   return getControler()->viewIsEnabledPrecompile(getCurrentViewId(),
          PRECOMPILE_FACE);
}
void Window :: setFacesDisplay(bool b)
{
   if (getFacesDisplay() != b)
   {
      getControler()->viewTogglePrecompile(getCurrentViewId(), PRECOMPILE_FACE);

      toggleDarts(b);
   }
}

bool Window::getFacesCoulAlea() const
{
   return getControler()->getParameterFace(getCurrentViewId())
          ->getRandomCLFace();
}

void Window::setFacesCoulAlea()
{
   if (!getFacesCoulAlea())
   {
      getControler()->getParameterFace(getCurrentViewId())
      ->setRandomCLFace(true);
      getControler()->getParameterFace(getCurrentViewId())
      ->setRandomCLVolume(false);

      toggleDarts(true);
   }
}

bool Window::getVolumesCoulAlea() const
{
   return getControler()->getParameterFace(getCurrentViewId())
          ->getRandomCLVolume();
}

void Window::setVolumesCoulAlea()
{
   if (!getVolumesCoulAlea())
   {
      getControler()->getParameterFace(getCurrentViewId())
      ->setRandomCLVolume(true);
      getControler()->getParameterFace(getCurrentViewId())
      ->setRandomCLFace(false);

      toggleDarts(true);
   }
}

bool Window::getCoulFacesGlobale() const
{
   return
      !getControler()->getParameterFace(getCurrentViewId())
      ->getRandomCLFace() &&
      !getControler()->getParameterFace(getCurrentViewId())
      ->getRandomCLVolume();
}

void Window::setCoulFacesGlobale()
{
   if (!getCoulFacesGlobale())
   {
      getControler()->getParameterFace(getCurrentViewId())
      ->setRandomCLFace(false);
      getControler()->getParameterFace(getCurrentViewId())
      ->setRandomCLVolume(false);

      toggleDarts(true);
   }
}

bool Window::getCoulGeometry() const
{
   return getControler()->getParameterFace(getCurrentViewId())
          ->getRandomColorGeometry();
}

void Window::setCoulGeometry()
{
   if (!getCoulGeometry())
   {
      getControler()->getParameterFace(getCurrentViewId())
      ->setRandomColorGeometry(true);

      toggleDarts(true);
   }
}

bool Window::getCoulTopologie() const
{
   return (!getControler()->getParameterFace(getCurrentViewId())
           ->getRandomColorGeometry());
}

void Window::setCoulTopologie()
{
   if (!getCoulTopologie())
   {
      getControler()->getParameterFace(getCurrentViewId())
      ->setRandomColorGeometry(false);
      toggleDarts(true);
   }
}
//------------------------------------------------------------------------------
bool Window :: getSewsDisplay() const
{
   return getControler()->viewIsEnabledPrecompile(getCurrentViewId(),
          PRECOMPILE_SEW);
}
void Window :: setSewsDisplay(bool b)
{
   if (getSewsDisplay() != b)
   {
      getControler()->viewTogglePrecompile(getCurrentViewId(), PRECOMPILE_SEW);

      toggleDarts(b);
   }
}
//------------------------------------------------------------------------------
bool Window :: getGridDisplay() const
{
   return getControler()->viewIsEnabledPrecompile(getCurrentViewId(),
          PRECOMPILE_GRID);
}
void Window :: setGridDisplay(bool b)
{
   if (getGridDisplay() != b)
   {
      getControler()->viewTogglePrecompile(getCurrentViewId(), PRECOMPILE_GRID);
   }
}
//------------------------------------------------------------------------------
bool Window :: getAxisDisplay() const
{
   return getControler()->viewIsEnabledPrecompile(getCurrentViewId(),
          PRECOMPILE_AXIS);
}
void Window :: setAxisDisplay(bool b)
{
   if (getAxisDisplay() != b)
   {
      getControler()->viewTogglePrecompile(getCurrentViewId(), PRECOMPILE_AXIS);
      getControler()->viewTogglePrecompile(getCurrentViewId(), PRECOMPILE_AIMED_POINT);
   }
}

TViewId Window :: getCurrentViewId() const
{
   assert(FWorkspace->activeSubWindow()->widget() != NULL);

   return ((GLWindow *) FWorkspace -> activeSubWindow()->widget())
          -> getCliquedViewId() ;
}

void Window :: toggleDarts(bool b)
{
#ifdef MODULE_SPAMOD
   // Si on est en vue spamod et qu'on affiche les coutures ou les faces
   // pleines, alors on affiche egalement les brins.
   if (getParameterSpamod()->getViewMode() != SPAMOD_NONE &&
         getControler()->viewIsEnabledPrecompile(getCurrentViewId(),
                                                 PRECOMPILE_SPAMOD))
   {
      if (b)
         getControler()->
         viewEnablePrecompile(getCurrentViewId(), PRECOMPILE_DART);
      else if (!getFacesDisplay() && !getSewsDisplay())
         getControler()->
         viewDisablePrecompile(getCurrentViewId(), PRECOMPILE_DART);
   }
#endif // MODULE_SPAMOD
}
//***************************************
//***************************************
//              CALLBACKS
//***************************************
//***************************************
void Window :: callbackKeyUp()
{
   int view = getCurrentViewId();

   switch (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType())
   {
      case VIEW_XYZ :
      {
         FControler -> moveEye(view, true);
         repaint();
         break;
      }
      case VIEW_XY  : FControler -> moveEyeY(view, true); repaint(); break;
      case VIEW_XZ  :
      case VIEW_YZ  : FControler -> moveEyeZ(view, true); repaint(); break;
   }
}

void Window :: callbackKeyDown()
{
   int view = getCurrentViewId();

   switch (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType())
   {
      case VIEW_XYZ :
      {
         FControler -> moveEye(view, false);
         repaint();
         break;
      }
      case VIEW_XY  : FControler -> moveEyeY(view, false); repaint(); break;
      case VIEW_XZ  :
      case VIEW_YZ  : FControler -> moveEyeZ(view, false); repaint(); break;
   }
}

void Window :: callbackKeyLeft()
{
   int view = getCurrentViewId();

   switch (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType())
   {
      case VIEW_XYZ :
      {
         FControler -> horizontalRotationEye(view, true);
         repaint();
         break;
      }
      case VIEW_XY  :
      case VIEW_XZ  : FControler -> moveEyeX(view, false); repaint(); break;
      case VIEW_YZ  : FControler -> moveEyeY(view, false); repaint(); break;
   }
}

void Window :: callbackKeyRight()
{
   int view = getCurrentViewId();

   switch (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType())
   {
      case VIEW_XYZ :
      {
         FControler -> horizontalRotationEye(view, false);
         repaint();
         break;
      }
      case VIEW_XY  :
      case VIEW_XZ  : FControler -> moveEyeX(view, true); repaint(); break;
      case VIEW_YZ  : FControler -> moveEyeY(view, true); repaint(); break;
   }
}

void Window :: callbackKeyCtrlUp()
{
   if (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType() == VIEW_XYZ)
   {
      FControler -> verticalRotationEye(getCurrentViewId(), true);
      repaint();
   }
}

void Window :: callbackKeyCtrlDown()
{
   if (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType() == VIEW_XYZ)
   {
      FControler -> verticalRotationEye(getCurrentViewId(), false);
      repaint();
   }
}

void Window :: callbackKeyCtrlLeft()
{
   if (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType() == VIEW_XYZ)
   {
      FControler -> moveEyeLateral(getCurrentViewId(), false);
      repaint();
   }
}

void Window :: callbackKeyCtrlRight()
{
   if (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType() == VIEW_XYZ)
   {
      FControler -> moveEyeLateral(getCurrentViewId(), true);
      repaint();
   }
}

void Window :: callbackKeyShiftUp()
{
   if (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType() == VIEW_XYZ)
   {
      TViewId AView = getCurrentViewId();
      float coeff =
         FControler -> getParameterEyePosition(AView) -> getPasAvancement();
      FControler -> moveEye(AView, true, coeff * 5);
      repaint();
   }
}

void Window :: callbackKeyShiftDown()
{
   if (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType() == VIEW_XYZ)
   {
      TViewId AView = getCurrentViewId();
      float coeff =
         FControler -> getParameterEyePosition(AView) -> getPasAvancement();
      FControler -> moveEye(AView, false, coeff * 5);
      repaint();
   }
}

void Window :: callbackKeyShiftLeft()
{
   if (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType() == VIEW_XYZ)
   {
      FControler -> horizontalRotationEye(getCurrentViewId(), true, 90);
      repaint();
   }
}

void Window :: callbackKeyShiftRight()
{
   if (((GLWindow *) FWorkspace -> activeSubWindow()->widget()) -> getViewType() == VIEW_XYZ)
   {
      FControler -> horizontalRotationEye(getCurrentViewId(), false, 90);
      repaint();
   }
}

void Window :: callbackVueCompacte()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> getOptionsVue() -> callbackPresetting0();
}

void Window :: callbackVueSemiEclatee()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> getOptionsVue() -> callbackPresetting1();
}

void Window :: callbackVueMoka()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> getOptionsVue() -> callbackPresetting2();
}

void Window :: callbackVueTopoFil()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> getOptionsVue() -> callbackPresetting3();
}
//***************************************
// Slots Menu Fichier
//***************************************

//------------ Nouveau  --------------
void Window::callbackNettoyage()
{
   if (getControler()->empty()) repaint();
   else updateStatusBar();
}
//------------ Chargement --------------
void Window :: callbackLoad()
{
   // Ouverture d'un QFileDialog
   QStringList type_load;
   type_load << "Moka file (*.moka)";

   std::string filename = getOpenFileName("Load", type_load) ;

   // Si l'utilisateur n'a pas annule -> appel au controleur
   if (filename != "")
   {
      if (getControler() -> loadMap(filename.c_str())) repaint() ;
      else updateStatusBar() ;
   }
}
//------------ Ajout --------------
void Window::callbackAdd()
{
   int type;
   QStringList type_load;

   type_load << "Moka file (*.moka)";

   std::string filename = getOpenFileName("Add a file",
                                          type_load, &type) ;

   if (filename != "")
   {
      int res = 0;

      switch (type)
      {
         case 0 :
         case 1 : res = getControler()->addMap(filename.c_str()); break;
         default : break;
      }

      if (res) repaint() ;
      else updateStatusBar() ;
   }
}
//------------ Sauvegarde --------------
void Window::callbackSave()
{
   int type;
   QStringList type_save;

   type_save << "ASCII format (*.moka)" << "Binary format (*.moka)";
   std::string filename = getSaveFileName("Save", type_save, &type);

   if (filename != "")
   {
      int res = 0;

      switch (type)
      { 
         case 0: res = getControler()->saveMapAscii(filename.c_str()); break;
         case 1: res = getControler()->saveMapBinary(filename.c_str()); break;
         default : break;
      }

      if (res) repaint() ;
      else updateStatusBar() ;
   }
}
//------------ Importer --------------
void Window::callbackImport()
{
   int type;
   QStringList type_load;

#ifdef WITH_ASSIMP
   type_load << "Assimp (*.*)" << "Mesh (*.mesh)";
#else
   type_load << "Off (*.off)" << "Mesh (*.mesh)";
#endif // WITH_ASSIMP

   std::string filename = getOpenFileName("Import a file",
                                          type_load, &type) ;

   if (filename != "")
   {
      int res = 0;

      switch (type)
      {
         case 0 : 
#ifdef WITH_ASSIMP
           res = getControler()->importWithAssimp(filename.c_str()); break;
#else
           res = getControler()->importOff(filename.c_str()); break;
#endif // WITH_ASSIMP
         case 1 : res = getControler()->importTetmesh(filename.c_str()); break;
         default : break;
      }

      if (res) repaint() ;
      else updateStatusBar() ;
   }
}
//------------ Exporter --------------
void Window :: callbackExport()
{
   int type;
   QStringList type_save;

   type_save << "XFig G-map (*.fig)"
   << "XFig map (*.fig)"
   << "XFig Intervoxel (*.fig)"
   << "Pov (*.pov)"
   << "Off2D (*.off)"
   << "Off3D (*.off)";
   std::string filename = getSaveFileName("Export", type_save, &type);

   // Si l'utilisateur n'a pas annule on exporte selon son choix
   if (filename != "")
   {
      switch (type)
      {
         case 0 :
            FControler -> exportXfig(filename.c_str() , getCurrentViewId() ,
                                     false, getFacesDisplay() ,
                                     getSewsDisplay() , false ,
                                     getVerticesDisplay()) ;
            break ;

         case 1 :
            FControler -> exportXfig(filename.c_str() , getCurrentViewId() ,
                                     true, getFacesDisplay() ,
                                     getSewsDisplay() , false ,
                                     getVerticesDisplay()) ;
            break ;

         case 2 :
            FControler -> exportXfigAsIntervoxel(filename.c_str(),
                                                 getCurrentViewId());
            break ;

         case 3 :
            FControler -> exportPov(filename.c_str()) ;
            break ;
         case 4 :
            FControler -> exportOff(filename.c_str()) ;
            break ;
         case 5 :
            FControler -> exportOff3D(filename.c_str()) ;
            break ;
      }
      updateStatusBar() ;
   }
}

void Window::callbackFindMotif()
{
   int type;
   QStringList type_load;

   type_load << "Moka file (*.moka)" << "Off (*.off)";

   std::string filename = getOpenFileName("Search a sub-map",
                                          type_load, &type) ;

   if (filename != "")
   {
      int res = 0;

      switch (type)
      {
         case 0 : res = getControler()->findMotifMoka(filename.c_str()); break;
         case 1 : res = getControler()->findMotifOff(filename.c_str()); break;
         default : break;
      }

      if (res) repaint() ;
      else updateStatusBar() ;
   }
}

void Window::callbackCountMotifs()
{
   int type;
   QStringList type_load;

   type_load << "Moka file (*.moka)" << "Off (*.off)";

   std::string filename = getOpenFileName("Count all the sub-maps",
                                          type_load, &type) ;

   if (filename != "")
   {
      int res = 0;

      switch (type)
      {
         case 0 :
            res = getControler()->countNumberOfMotifsMoka(filename.c_str());
            break;
         case 1 :
            res = getControler()->countNumberOfMotifsOff(filename.c_str());
            break;
         default : break;
      }

      if (res) repaint() ;
      else updateStatusBar() ;
   }
}

//------------ Quitter --------------
void Window::callbackQuit()
{
   this -> close() ;
}

//***************************************
// Slots Menu Edition
//***************************************

//----------- Annuler --------------
void Window :: callbackUndo()
{
   if (getControler()  -> undo()) repaint() ;
   else updateStatusBar() ;
}
//----------- Refaire --------------
void Window :: callbackRedo()
{
   if (getControler() -> redo()) repaint() ;
   else updateStatusBar() ;
}
//---------- Vider la pile ----------
void Window :: callbackEmptyUndoRedo()
{
   getControler() -> emptyUndoRedo() ;
   updateStatusBar() ;
}
//---------- Configuration ----------
void Window :: callbackConfigUndoRedo()
{
   FDialogDo = new DialogDo(this) ;
   FDialogDo -> show() ;
}
//***************************************
// Slots Menu Options
//***************************************
void Window :: help()
{
}
void Window :: view()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> setOption(3) ;
   FOptionsFrame -> show() ;
}

void Window :: plating()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> setOption(4) ;
   FOptionsFrame -> show() ;
}

void Window :: pondDef()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> setOption(5) ;
   FOptionsFrame -> show() ;
}

void Window :: extrusion()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> setOption(6) ;
   FOptionsFrame -> show() ;
}

void Window :: interpol()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> setOption(7) ;
   FOptionsFrame -> show() ;
}

void Window :: affich()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> setOption(2) ;
   FOptionsFrame -> show() ;
}

void Window :: affichDivers()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;

   FOptionsFrame -> setOption(8) ;
   FOptionsFrame -> show() ;
}

void Window :: caracTopo()
{
   if (FOptionsCarac == NULL)
      FOptionsCarac = new OptionsCarac(this) ;

   FOptionsCarac -> show_impl() ;
}

void Window::callbackComputeSurfacicHomology()
{
  if (FOptionsSurfacicHomology == NULL)
    FOptionsSurfacicHomology = new OptionsSurfacicHomology(this) ;
  
  FOptionsSurfacicHomology -> show_impl() ;
  repaint();
}

void Window::callbackComputeVolumicHomology()
{
  if (FOptionsVolumicHomology == NULL)
    FOptionsVolumicHomology = new OptionsVolumicHomology(this) ;
  
  FOptionsVolumicHomology -> show_impl() ;
  repaint();
}

void Window :: callback2Manifold()
{
  getControler()->countNonLocalDegreeTwoEdges();
  updateStatusBar();
}

void Window :: couleurs()
{
   resetActiveOperations();
   color_active = true;
   FCouleurs -> show();
   repaint();
}

// Pour afficher les coutures
void Window :: callbackToggleSews()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;
   FOptionsFrame -> callbackToggleSews() ;
}

// Pour afficher les normales
void Window :: callbackToggleNormal()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;
   FOptionsFrame -> callbackToggleNormal() ;
}

// Pour afficher les sommets
void Window :: callbackToggleVertices()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;
   FOptionsFrame -> callbackToggleVertices() ;
}

// Pour afficher les faces
void Window :: callbackToggleFaces()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this);
   FOptionsFrame -> callbackToggleFaces() ;
}

void Window :: callbackTournerButton()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this);
   FOptionsFrame ->callbackTournerButton();
}

void Window :: callbackToggleGrille()
{
   if (FOptionsFrame == NULL)
      FOptionsFrame = new OptionsFrame(this) ;
   FOptionsFrame -> callbackToggleGrille() ;
}

void Window :: chanf()
{
   if (FOperationChanfreinage == NULL)
      FOperationChanfreinage = new OperationChanfreinage(this) ;
   FOperationChanfreinage -> show() ;
}

void Window :: resetActiveOperations()
{
   // Si il y a une fenetre de creation ou d'operation
   // deja ouverte, on la ferme
   if (FCreationActive != NULL)
   {
      FCreationActive -> cancel() ;
      FCreationActive = NULL ;
   }
   else if (FOperationActive != NULL)
   {
      FOperationActive -> close() ;
      FOperationActive = NULL ;
   }
   else if (color_active)
   {
      FCouleurs -> close();
      color_active = false;
   }
   else if (getOptionsSurfacicHomologyActive() )
     {
       FOptionsSurfacicHomology->close();
       delete FOptionsSurfacicHomology;
       FOptionsSurfacicHomology = NULL;
     }
   else if (getOptionsVolumicHomologyActive() )
     {
       FOptionsVolumicHomology->close();
       delete FOptionsVolumicHomology;
       FOptionsSurfacicHomology = NULL;
     }   
}

// Slots Menu Creer
//-----------------
void Window :: callbackPolyline()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_CREATION_POLYLINE) ;
   FCreationActive = FCreationBrin ;
   FCreationActive -> setVisible(true);
   updateStatusBar();
   repaint();
}

void Window :: callbackPolygon()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_CREATION_POLYGON) ;
   FCreationActive = FCreationPolygone ;
   FCreationActive -> setVisible(true);
   updateStatusBar();
   repaint();
}

void Window :: callbackMeshCreation()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_CREATION_MESH) ;
   FCreationActive = FCreationMaillage ;
   FCreationActive -> setVisible(true);
   updateStatusBar();
   repaint();
}

void Window :: callbackSphere()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_CREATION_SPHERE) ;
   FCreationActive = FCreationSphere ;
   FCreationActive -> setVisible(true);
   updateStatusBar();
   repaint();
}

void Window :: callbackCylinder()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_CREATION_CYLINDER) ;
   FCreationActive = FCreationCylindre ;
   FCreationActive -> setVisible(true);
   updateStatusBar();
   repaint();
}

void Window :: callbackPyramide()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_CREATION_PYRAMID) ;
   FCreationActive = FCreationPyramide ;
   FCreationActive -> setVisible(true);
   updateStatusBar();
   repaint();
}

void Window :: callbackTorus()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_CREATION_TORUS) ;
   FCreationActive =  FCreationTore ;
   FCreationActive -> setVisible(true);
   updateStatusBar();
   repaint();
}

void Window :: callbackRandomizeDarts()
{
   getControler()->randomizeDarts();
   updateStatusBar();
}

//***************************************
// Slots Menu Operations
//***************************************
// -- translation/rotation/homothetie
void Window :: callbackTranslateWindow()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_TRANSLATION);
   FOperationActive =  FDialogTranslation ;
   FOperationActive -> update();
   FOperationActive -> setVisible(true);
   updateStatusBar() ;
   repaint();
}

void Window :: callbackRotateWindow()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_ROTATION);
   FOperationActive =  FDialogRotation ;
   FOperationActive -> update();
   FOperationActive -> setVisible(true);
   updateStatusBar() ;
   repaint();
}

void Window :: callbackScaleWindow()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_SCALE);
   FOperationActive =  FDialogHomothetie ;
   FOperationActive -> update();
   FOperationActive -> setVisible(true);
   updateStatusBar() ;
   repaint();
}

void Window :: callbackMovetoWindow()
{
   resetActiveOperations();
   getControler() -> setMode(MODE_SELECTION);
   FOperationActive =  FDialogMoveto ;
   FOperationActive -> update();
   FOperationActive -> setVisible(true);
   updateStatusBar() ;
   repaint();
}

// -- coudre
void Window :: callbackIntuitiveTopoSew()
{
   if (getControler() -> intuitiveTopoSew()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackIntuitiveGeoSew()
{
   if (getControler() -> intuitiveGeoSew()) repaint() ;
   else updateStatusBar() ;
}

void Window ::  callbackTopoSew0()
{
   if (getControler() -> topoSew(0)) repaint() ;
   else updateStatusBar() ;
}

void Window ::  callbackTopoSew1()
{
   if (getControler() -> topoSew(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackGeoSew1()
{
   if (getControler() -> geoSew(1)) repaint() ;
   else updateStatusBar() ;
}

void Window ::callbackTopoSew2()
{
   if (getControler() -> topoSew(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackGeoSew2()
{
   if (getControler() -> geoSew(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackTopoSew3()
{
   if (getControler() -> topoSew(3)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackGeoSew3()
{
   if (getControler() -> geoSew(3)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackTopoSew1borders()
{
   if (getControler() -> topoSewBorders(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackGeoSew1borders()
{
   if (getControler() -> geoSewBorders(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackTopoSew2borders()
{
   if (getControler() -> topoSewBorders(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackGeoSew2borders()
{
   if (getControler() -> geoSewBorders(2)) repaint();
   else updateStatusBar() ;
}

void Window :: callbackInvertLastOperation()
{
   if (getControler() -> invertLastOperation()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackInvertLastOperationGeometry()
{
   if (getControler() -> invertLastOperationGeometry()) repaint() ;
   else updateStatusBar() ;
}

// -- decoudre
void Window :: callbackIntuitiveUnsew()
{
   if (getControler() -> intuitiveUnsew()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackUnsew0()
{
   if (getControler() -> unsew(0)) repaint();
   else updateStatusBar() ;
}

void Window :: callbackUnsew1()
{
   if (getControler() -> unsew(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackUnsew2()
{
   if (getControler() -> unsew(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackUnsew3()
{
   if (getControler() -> unsew(3)) repaint() ;
   else updateStatusBar() ;
}

// -- placage
void Window :: callbackIntuitivePlate()
{
   if (getControler()  -> intuitivePlate()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackPlate0()
{
   if (getControler() -> plate(0)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackPlate1()
{
   if (getControler() -> plate(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackPlate2()
{
   if (getControler() -> plate(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackPlate1borders()
{
   if (getControler() -> plateBorders(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackPlate2borders()
{
   if (getControler() -> plateBorders(2)) repaint() ;
   else updateStatusBar() ;
}

// -- Fusionner
void Window :: callbackIntuitiveMerge()
{
   if (getControler() -> intuitiveMerge()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMergeEdges()
{
   if (getControler() -> merge(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMergeFaces()
{
   if (getControler() -> merge(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMergeVolumes()
{
   if (getControler() -> merge(3)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMergeColinearEdges()
{
   if (getControler() -> mergeColinearEdges()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMergeCoplanarFaces()
{
   if (getControler() -> mergeCoplanarFaces()) repaint() ;
   else updateStatusBar() ;
}

/// Faces sans deconnexion
void Window :: callbackMergeFacesWithoutDisconnection()
{
   if (getControler()->removeMarkedEdgesWithoutDisconnection()) repaint();
   else updateStatusBar() ;
}

void Window :: callbackRemoveMarkedFacesButKeepBalls()
{
   if (getControler()->removeMarkedFacesButKeepBalls()) repaint();
   else updateStatusBar() ;
}

/// Decaler les aretes incidentes au sommet
void Window :: callbackShiftAllEdgesIncidentToVertex()
{
   if (getControler() -> shiftAllEdgesIncidentToVertex()) repaint() ;
   else updateStatusBar() ;
}

/// Supprimer les aretes pendantes
void Window :: callbackRemoveDanglingEdges()
{
   if (getControler() -> removeDanglingEdges()) repaint() ;
   else updateStatusBar() ;
}

void Window::callbackSimplify2DObject()
{
   if (getControler() -> simplify2DObject()) repaint() ;
   else updateStatusBar() ;
}

void Window::callbackSimplify3DObject()
{
   if (getControler() -> simplify3DObject()) repaint() ;
   else updateStatusBar() ;
}

// -- Contracter
void Window :: callbackIntuitiveContract()
{
   if (getControler() -> contextContract()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackContractEdges()
{
   if (getControler() -> contract(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackContractFaces()
{
   if (getControler() -> contract(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackContractVolumes()
{
   if (getControler() -> contract(3)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackDelNullEdges()
{
   if (getControler() -> delNullEdges()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackDelFlatFaces()
{
   if (getControler() -> delFlatFaces()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackDelFlatVolumes()
{
   if (getControler() -> delFlatVolumes()) repaint() ;
   else updateStatusBar() ;
}

// -- Inserer
void Window :: callbackInsertVertex()
{
   if (getControler() -> insertVertex()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackInsertEdge()
{
   if (getControler() -> insertEdge()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackInsertFace()
{
   if (getControler() -> insertFace()) repaint() ;
   else updateStatusBar() ;
}

// -- Fermer
void Window :: callbackIntuitiveStopUp()
{
   if (getControler() -> intuitiveStopUp()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackStopUp0()
{
   if (getControler() -> stopUp(0)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackStopUp1()
{
   if (getControler() -> stopUp(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackStopUp2()
{
   if (getControler() -> stopUp(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackStopUp3()
{
   if (getControler() -> stopUp(3)) repaint() ;
   else updateStatusBar() ;
}

// -- Trianguler
void Window :: callbackContextTriangulate()
{
   if (getControler() -> contextTriangulate()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackTriangulateEdges()
{
   if (getControler() -> triangulate(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackTriangulateFaces()
{
   if (getControler() -> triangulate(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackTriangulateVolumes()
{
   if (getControler() -> triangulate(3)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackSwapEdge()
{
  if (getControler() -> swapEdge()) repaint() ;
  else updateStatusBar() ;
}

void Window :: callbackTriangulateGeoFaces()
{
  if (getControler() -> triangulateGeoFaces()) repaint() ;
  else updateStatusBar() ;
}


// -- Quadranguler
void Window :: callbackQuadrangulateFaces()
{
   if (getControler() -> quadrangulateFaces()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackBasicQuadrangulateFaces()
{
   if (getControler() -> basicQuadrangulateFaces()) repaint() ;
   else updateStatusBar() ;
}

// -- Calculer Dual
void Window :: callbackDual2d()
{
   if (getControler() -> dual2d()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackDual3d()
{
   if (getControler() -> dual3d()) repaint() ;
   else updateStatusBar() ;
}

// -- Extruder
void Window :: callbackIntuitiveExtrudeByNormal()
{
   if (getControler() -> intuitiveExtrudeByNormal()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackExtrude0ByNormal()
{
   if (getControler() -> extrudeByNormal(0)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackExtrude1ByNormal()
{
   if (getControler() ->extrudeByNormal(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackExtrude2ByNormal()
{
   if (getControler() -> extrudeByNormal(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackExtrude1ByPath()
{
   if (getControler() -> extrudeByPath(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackExtrude2ByPath()
{
   if (getControler() -> extrudeByPath(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackExtrude1ByRevolution()
{
   if (getControler() -> extrudeByRevolution(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackExtrude2ByRevolution()
{
   if (getControler() -> extrudeByRevolution(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackThicken()
{
   if (getControler() -> thicken()) repaint() ;
   else updateStatusBar() ;
}

//--------------------------------------------------------------------
// -- Interpoler
void Window::callbackMeshEdge()
{
#ifdef MODULE_MESH
   if (getControler() -> meshEdges()) repaint() ;
   else updateStatusBar() ;
#endif //MODULE_MESH
}
void Window::callbackMeshSquare01()
{
#ifdef MODULE_MESH
   callbackMeshSquare(0 , 1) ;
#endif //MODULE_MESH
}

void Window::callbackMeshSquare02()
{
#ifdef MODULE_MESH
   callbackMeshSquare(0 , 2) ;
#endif //MODULE_MESH
}

void Window::callbackMeshSquare12()
{
#ifdef MODULE_MESH
   callbackMeshSquare(1 , 2) ;
#endif //MODULE_MESH
}

void Window::callbackMeshSquare(int i , int j)
{
#ifdef MODULE_MESH
   if (getControler() -> meshSquares(i , j)) repaint() ;
   else updateStatusBar() ;
#endif //MODULE_MESH
}

void Window::callbackMeshCube01()
{
#ifdef MODULE_MESH
   callbackMeshCube(0 , 1) ;
#endif //MODULE_MESH
}

void Window::callbackMeshCube02()
{
#ifdef MODULE_MESH
   callbackMeshCube(0 , 2) ;
#endif //MODULE_MESH
}

void Window::callbackMeshCube03()
{
#ifdef MODULE_MESH
   callbackMeshCube(0 , 3) ;
#endif //MODULE_MESH
}

void Window::callbackMeshCube12()
{
#ifdef MODULE_MESH
   callbackMeshCube(1 , 2) ;
#endif //MODULE_MESH
}

void Window::callbackMeshCube13()
{
#ifdef MODULE_MESH
   callbackMeshCube(1 , 3) ;
#endif //MODULE_MESH
}

void Window::callbackMeshCube23()
{
#ifdef MODULE_MESH
   callbackMeshCube(2 , 3) ;
#endif //MODULE_MESH
}

void Window::callbackMeshCube(int i , int j)
{
#ifdef MODULE_MESH
   if (getControler() -> meshCubes(i , j)) repaint() ;
   else updateStatusBar() ;
#endif //MODULE_MESH
}
// -- Lisser
void Window :: callbackSmooth1()
{
#ifdef MODULE_MESH
   if (getControler() -> smoothEdges()) repaint() ;
   else updateStatusBar();
#endif //MODULE_MESH
}

void Window :: callbackSmooth2()
{
#ifdef MODULE_MESH
   if (getControler() -> smoothSquares()) repaint() ;
   else updateStatusBar();
#endif //MODULE_MESH
}

void Window :: callbackSmooth3()
{
#ifdef MODULE_MESH
   if (getControler() -> smoothCubes()) repaint() ;
   else updateStatusBar();
#endif //MODULE_MESH
}
//------------------------------------------------------------------------------
// -- Arrondir
void Window::callbackRoundVertices()
{
#ifdef MODULE_ROUNDING
   roundVertices(false);
#endif // MODULE_ROUNDING
}

void Window::callbackRoundVerticesDig()
{
#ifdef MODULE_ROUNDING
   roundVertices(true);
#endif // MODULE_ROUNDING
}

void Window::roundVertices(bool ADig)
{
#ifdef MODULE_ROUNDING
   if (getControler() -> roundVertices(ADig)) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_ROUNDING
}

void Window::callbackRoundEdges2d()
{
#ifdef MODULE_ROUNDING
   roundEdges(false, false, false);
#endif // MODULE_ROUNDING
}

void Window::callbackRoundEdges2dDig()
{
#ifdef MODULE_ROUNDING
   roundEdges(false, true, false);
#endif // MODULE_ROUNDING
}

void Window::callbackRoundEdges3d()
{
#ifdef MODULE_ROUNDING
   roundEdges(true, false, false);
#endif // MODULE_ROUNDING
}

void Window::callbackRoundEdges3dDig()
{
#ifdef MODULE_ROUNDING
   roundEdges(true, true, false);
#endif // MODULE_ROUNDING
}

void Window::callbackRoundEdges2dWithSetback()
{
#ifdef MODULE_ROUNDING
   roundEdges(false, false, true);
#endif // MODULE_ROUNDING
}

void Window::callbackRoundEdges2dDigWithSetback()
{
#ifdef MODULE_ROUNDING
   roundEdges(false, true, true);
#endif // MODULE_ROUNDING
}

void Window::callbackRoundEdges3dWithSetback()
{
#ifdef MODULE_ROUNDING
   roundEdges(true, false, true);
#endif // MODULE_ROUNDING
}

void Window::callbackRoundEdges3dDigWithSetback()
{
#ifdef MODULE_ROUNDING
   roundEdges(true, true, true);
#endif // MODULE_ROUNDING
}

void Window::roundEdges(bool A3D, bool ADig, bool ASetback)
{
#ifdef MODULE_ROUNDING
   if (getControler() -> roundEdges(A3D, ADig, ASetback)) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_ROUNDING
}
//#endif // MODULE_ROUNDING
//------------------------------------------------------------------------------
// -- Triangulation
void Window :: callbackTriangulateTopoFaces()
{
#ifdef MODULE_TRIANGULATION
   if (getControler() -> triangulateTopoFaces()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_TRIANGULATION
}

void Window :: callbackMarkNonUniformFaces()
{
#ifdef MODULE_TRIANGULATION
   if (getControler() -> markNonUniformFaces()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_TRIANGULATION
}

void Window :: callbackOptimizeSelectedEdges()
{
#ifdef MODULE_TRIANGULATION
   if (getControler() -> optimizeSelectedEdges()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_TRIANGULATION
}

void Window :: callbackDelaunayTriangulation()
{
#ifdef MODULE_TRIANGULATION
   if (getControler() -> createDelaunayTriangulation()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_TRIANGULATION
}

void Window :: callbackOptimizedDelaunayTriangulation()
{
#ifdef MODULE_TRIANGULATION
   if (getControler() -> createOptimizedDelaunayTriangulation()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_TRIANGULATION
}
//------------------------------------------------------------------------------
// -- Co-raffiner
void Window :: callbackCorefine2dSegmentsSweeping()
{
#ifdef MODULE_COREFINEMENT
   if (getControler() ->corefine2dSegmentsSweeping()) repaint() ;
   else updateStatusBar() ;
#endif // COREFINEMENT
}

void Window :: callbackCorefine2dSegmentsPropagation()
{
#ifdef MODULE_COREFINEMENT
   if (getControler() ->corefine2dSegmentsPropagation()) repaint() ;
   else updateStatusBar() ;
#endif // COREFINEMENT
}

void Window :: callbackBooleanOperations2d()
{
#ifdef MODULE_COREFINEMENT
   if (getControler() ->booleanOperations2d()) repaint() ;
   else updateStatusBar() ;
#endif // COREFINEMENT
}

void Window :: callbackCorefine3dMeshes()
{
#ifdef MODULE_COREFINEMENT
   if (getControler() ->corefine3dMeshes()) repaint() ;
   else updateStatusBar() ;
#endif // COREFINEMENT
}

void Window :: callbackCorefine3dFaces()
{
#ifdef MODULE_COREFINEMENT
   if (getControler() ->corefine3dFaces()) repaint() ;
   else updateStatusBar() ;
#endif // COREFINEMENT
}

void Window :: callbackCorefineMarked3dFaces()
{
#ifdef MODULE_COREFINEMENT
   if (getControler() ->corefineMarked3dFaces()) repaint() ;
   else updateStatusBar() ;
#endif // COREFINEMENT
}

void Window :: callbackCorefineMarked3dMeshesWith()
{
#ifdef MODULE_COREFINEMENT
   if (getControler() ->corefineMarked3dMeshesWith()) repaint() ;
   else updateStatusBar() ;
#endif // COREFINEMENT
}

void Window :: callbackBooleanOperations3d()
{
#ifdef MODULE_COREFINEMENT
   if (getControler() ->booleanOperations3d()) repaint() ;
   else updateStatusBar() ;
#endif // COREFINEMENT
}
//------------------------------------------------------------------------------
//- GEOLOGIE
void Window :: callbackUncertainZone()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->createUncertainZone()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackCreateNearestIntersections()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->createNearestIntersections()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackExtendSelectedBorders()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->extendSelectedBorders()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackExtendSelectedBordersToSurface()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->extendSelectedBordersToSurface()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackSmoothSelection()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->smoothSelection()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackRelaxSelection()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->relaxSelection()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackSimplifyMesh()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->simplifyMesh()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackCenterSelection()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->centerSelection()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackPlateVerticesOnFaces()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->plateVerticesOnFaces()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackPlateVerticesOnEdges()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->plateVerticesOnEdges()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackPlateVerticesOnVertices()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->plateVerticesOnVertices()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackSelectMacroVertices()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->selectMacroVertices()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackSelectAllMacroEdges()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->selectAllMacroEdges()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackSelectMacroEdges()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->selectMacroEdges()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackSelectMacroFaces()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->selectMacroFaces()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackSelectBordersBetweenSelectedDarts()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->selectBordersBetweenSelectedDarts()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackHolesRefinement()
{
#ifdef MODULE_GEOLOGY
   // Si il y a une fenetre de creation ou d'operation
   // deja ouverte, on la ferme
   if (FCreationActive != NULL)
   {
      FCreationActive -> cancel() ;
      FCreationActive = NULL ;
   }
   else if (FOperationActive != NULL)
   {
      FOperationActive -> close() ;
      FOperationActive = NULL ;
   }
   else if (FMeshActive != NULL)
   {
      FMeshActive -> reject() ;
      FMeshActive = NULL ;
   }

   FMeshActive = new MeshHolesGestion(this , getControler()) ;
   FMeshActive -> show() ;
   updateStatusBar();
#endif // MODULE_GEOLOGY
}

void Window :: callbackTriangulateHoles()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->triangulateHoles()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackFillHolesOnMeshes()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->fillHolesOnMeshes()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackFailleDetection()
{
#ifdef MODULE_GEOLOGY
   // Si il y a une fenetre de creation ou d'operation
   // deja ouverte, on la ferme
   if (FCreationActive != NULL)
   {
      FCreationActive -> cancel() ;
      FCreationActive = NULL ;
   }
   else if (FOperationActive != NULL)
   {
      FOperationActive -> close() ;
      FOperationActive = NULL ;
   }
   else if (FMeshActive != NULL)
   {
      FMeshActive -> reject() ;
      FMeshActive = NULL ;
   }

   FMeshActive = new MeshFailleGestion(this , getControler()) ;
   FMeshActive -> show() ;
   updateStatusBar();
#endif // MODULE_GEOLOGY
}

void Window :: callbackExtractDetectedFailles()
{
#ifdef MODULE_GEOLOGY
   if (getControler() ->extractDetectedFailles()) repaint() ;
   else updateStatusBar() ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackExtensionFailleSurface()
{
#ifdef MODULE_GEOLOGY
   // Si il y a une fenetre de creation ou d'operation
   // deja ouverte, on la ferme
   if (FCreationActive != NULL)
   {
      FCreationActive -> cancel() ;
      FCreationActive = NULL ;
   }
   else if (FOperationActive != NULL)
   {
      FOperationActive -> close() ;
      FOperationActive = NULL ;
   }
   else if (FMeshActive != NULL)
   {
      FMeshActive -> reject() ;
      FMeshActive = NULL ;
   }

   FMeshActive = new MeshExtension(this , getControler()) ;
   FMeshActive -> show() ;
   updateStatusBar();
#endif // MODULE_GEOLOGY
}
/*
void Window :: callbackGetIntersectionSurface ( )
{
#ifdef MODULE_GEOLOGY
  if (getControler ( ) ->getIntersectionSurface ( ) ) repaint ( ) ;
  else updateStatusBar ( ) ;
#endif // MODULE_GEOLOGY
}*/

void Window :: callbackMeshSimplification()
{
#ifdef MODULE_GEOLOGY
   // Si il y a une fenetre de creation ou d'operation
   // deja ouverte, on la ferme
   if (FCreationActive != NULL)
   {
      FCreationActive -> cancel() ;
      FCreationActive = NULL ;
   }
   else if (FOperationActive != NULL)
   {
      FOperationActive -> close() ;
      FOperationActive = NULL ;
   }
   else if (FMeshActive != NULL)
   {
      FMeshActive -> reject() ;
      FMeshActive = NULL ;
   }

   FMeshActive = new MeshSimplification(this , getControler()) ;
   FMeshActive -> show() ;
   updateStatusBar();
#endif // MODULE_GEOLOGY
}

/*void Window :: callbackCollapseEdge ( )
{
#ifdef MODULE_GEOLOGY
  if (getControler ( ) ->collapseEdge ( ) ) repaint ( ) ;
  else updateStatusBar ( ) ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackRemoveVertex ( )
{
#ifdef MODULE_GEOLOGY
  if (getControler ( ) ->removeVertex ( ) ) repaint ( ) ;
  else updateStatusBar ( ) ;
#endif // MODULE_GEOLOGY
}
*/

void Window :: callbackImportGr2d()
{
#ifdef MODULE_GEOLOGY
   char* filename = getOpenFileName("Importer Gr2D") ;
   if (filename != NULL)
   {
      if (getControler() -> importGr2d(filename)) repaint() ;
      else updateStatusBar() ;
   }
#endif // MODULE_GEOLOGY
}

void Window :: callbackImportXyz()
{
#ifdef MODULE_GEOLOGY
   char* filename = getOpenFileName("Importer XYZ") ;
   if (filename != NULL)
   {
      if (getControler() -> importXyz(filename)) repaint() ;
      else updateStatusBar() ;
   }
#endif // MODULE_GEOLOGY

}

void Window :: callbackImportTs()
{
#ifdef MODULE_GEOLOGY

   char* filename = getOpenFileName("Importer TS") ;
   if (filename != NULL)
   {
      if (getControler() -> importTs(filename)) repaint() ;
      else updateStatusBar() ;
   }
#endif // MODULE_GEOLOGY
}

void Window :: callbackImportCloud()
{
#ifdef MODULE_GEOLOGY
   char* filename = getOpenFileName("Importer Nuage de point") ;
   if (filename != NULL)
   {
      if (getControler() -> importCloud(filename)) repaint() ;
      else updateStatusBar() ;
   }
#endif // MODULE_GEOLOGY
}

void Window :: callbackImportVrml()
{
#ifdef MODULE_GEOLOGY

   char* filename = getOpenFileName("Importer VRML") ;

   if (filename != NULL)
   {
      if (getControler() -> importVrml(filename)) repaint() ;
      else updateStatusBar() ;
   }
#endif // MODULE_GEOLOGY
}

void Window :: callbackExportTs()
{
#ifdef MODULE_GEOLOGY

   char* filename = getSaveFileName("Exporter TS") ;

   if (filename != NULL)
   {
      if (getControler() -> exportTs(filename)) repaint() ;
      else updateStatusBar() ;
   }
#endif // MODULE_GEOLOGY
}

void Window :: callbackImportGr2dOk()
{
#ifdef MODULE_GEOLOGY
//   string filename = fileSelection->get_filename ( ) ;
//   if (getControler ( ) ->importGr2d(filename.c_str ( ) )) repaint ( ) ;
   // else updateStatusBar ( ) ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackImportXyzOk()
{
#ifdef MODULE_GEOLOGY
//   string filename = fileSelection->get_filename ( ) ;
//   if (getControler ( ) ->importXyz(filename.c_str ( ) )) repaint ( ) ;
//  else updateStatusBar ( ) ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackImportTsOk()
{
#ifdef MODULE_GEOLOGY
//   string filename = fileSelection->get_filename ( ) ;
//   if (getControler ( ) ->importTs(filename.c_str ( ) )) repaint ( ) ;
//  else updateStatusBar ( ) ;
#endif // MODULE_GEOLOGY
}

void Window :: callbackImportCloudOk()
{
#ifdef MODULE_GEOLOGY
//   string filename = fileSelection->get_filename ( ) ;
//   if (getControler ( ) ->importCloud(filename.c_str ( ) )) repaint ( ) ;
   //else updateStatusBar ( ) ;
#endif // MODULE_GEOLOGY
}

//***************************************
// Slots Menu Materialiser
//***************************************
void Window :: callbackMaterializeBarycenter()
{
   if (getControler() ->materializeBarycenter()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMaterializeAxe()
{
   if (getControler() ->materializeAxe()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMaterializePlane()
{
   if (getControler() ->materializePlane()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMaterializeNormalVector()
{
   if (getControler() ->materializeNormalVector()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMaterializeOrthoPlane()
{
   if (getControler() ->materializeOrthoPlane()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMaterializeReferential()
{
   if (getControler() ->materializeReferential()) repaint() ;
   else updateStatusBar() ;
}

//***************************************
// Slots Menu Selection
//***************************************
void Window :: callbackReinitSelection()
{
   if (getControler() -> unmarkAllCurrentSelectionLevel()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackNegateSelection()
{
   if (getControler() -> negateAllCurrentSelectionLevel()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackSwapSelections()
{
   if (getControler() -> swapCurrentSelectionLevelWithNext()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackDuplicateSelection()
{
   if (getControler() -> duplicateSelection()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackDeleteSelection()
{
   if (getControler() -> deleteSelection()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackGoForward1()
{
   if (getControler() -> goForward1()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackGoBackward1()
{
   if (getControler()  -> goBackward1()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoLeft1()
{
   if (getControler() -> goLeft1()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoRight1()
{
   if (getControler() -> goRight1()) repaint() ;
   else updateStatusBar();
}
void Window :: callbackGoForward1Rep()
{
   if (getControler()  -> goForward1Rep()) repaint()  ;
   else updateStatusBar();
}

void Window :: callbackGoBackward1Rep()
{
   if (getControler() -> goBackward1Rep()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoLeft1Rep()
{
   if (getControler() -> goLeft1Rep()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoRight1Rep()
{
   if (getControler() -> goRight1Rep()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoForward0()
{
   if (getControler() -> goForward0()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoBackward0()
{
   if (getControler() -> goBackward0()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoLeft0()
{
   if (getControler() -> goLeft0()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoRight0()
{
   if (getControler() -> goRight1()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoForward0Rep()
{
   if (getControler() -> goForward0Rep()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoBackward0Rep()
{
   if (getControler() -> goBackward0Rep()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoLeft0Rep()
{
   if (getControler() -> goLeft0Rep()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackGoRight0Rep()
{
   if (getControler() -> goRight0Rep()) repaint() ;
   else updateStatusBar();
}

void Window :: callbackMarkInterior2()
{
   if (getControler() ->markInterior(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackMarkInterior3()
{
   if (getControler() ->markInterior(3)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackIntuitiveSelectBorders()
{
   if (getControler() ->intuitiveSelectBorders()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackSelect0borders()
{
   if (getControler() ->selectBorders(0)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackSelect1borders()
{
   if (getControler() ->selectBorders(1)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackSelect2borders()
{
   if (getControler() ->selectBorders(2)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackSelect3borders()
{
   if (getControler() ->selectBorders(3)) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackSelectNonPlanarFaces()
{
   if (getControler() ->selectNonPlanarFaces()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackSelectIncidents()
{
   if (getControler() ->selectIncidents()) repaint() ;
   else updateStatusBar() ;
}

void Window :: callbackGoAlpha0()
{
   if (getControler() ->goAlpha0()) repaint() ;
   else updateStatusBar() ;
}
void Window :: callbackGoAlpha1()
{
   if (getControler() ->goAlpha1()) repaint() ;
   else updateStatusBar() ;
}
void Window :: callbackGoAlpha2()
{
   if (getControler() ->goAlpha2()) repaint() ;
   else updateStatusBar() ;
}
void Window :: callbackGoAlpha3()
{
   if (getControler() ->goAlpha3()) repaint() ;
   else updateStatusBar() ;
}
//***************************************
// Slots Menu Vues
//***************************************
void Window :: tile()
{
   FWorkspace -> tileSubWindows(); ;
}

void Window :: cascade()
{
   FWorkspace -> cascadeSubWindows();
}

void Window :: addView3D()
{
   GLWindow * vue = new GLWindow(VIEW_XYZ , FWorkspace ,
                                 this , FVueMere , FSelection) ;
   vue -> setMinimumSize(200 , 200) ;
   vue -> show() ;
}

void Window :: addViewMulti()
{
   GLMultiWindow * vue = new GLMultiWindow(FWorkspace , this , FVueMere,
                                           FSelection) ;
   vue -> setMinimumSize(200 , 200) ;
   vue -> show() ;
}

void Window :: addViewXY()
{
   GLWindow * vue = new GLWindow(VIEW_XY , FWorkspace ,
                                 this , FVueMere , FSelection) ;
   vue -> setMinimumSize(200 , 200) ;
   vue -> show() ;
}

void Window :: addViewYZ()
{
   GLWindow * vue = new GLWindow(VIEW_YZ , FWorkspace ,
                                 this , FVueMere , FSelection) ;
   vue -> setMinimumSize(200 , 200) ;
   vue -> show() ;
}

void Window :: addViewZX()
{
   GLWindow * vue = new GLWindow(VIEW_XZ , FWorkspace ,
                                 this , FVueMere , FSelection) ;
   vue -> setMinimumSize(200 , 200) ;
   vue -> show() ;
}

void Window :: basculeView3D()
{
   bascule(VIEW_XYZ) ;
}

void Window :: basculeViewXY()
{
   bascule(VIEW_XY) ;
}

void Window :: basculeViewYZ()
{
   bascule(VIEW_YZ) ;
}

void Window :: basculeViewZX()
{
   bascule(VIEW_XZ) ;
}

void Window :: basculeViewMulti()
{
   bascule(VIEW_ISO) ;
}

void Window :: deleteView()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   active -> closeEvent(NULL) ;
}

void Window :: OperationUngroupAllGeneral()
{
   FControler -> viewUngroupAll() ;
   repaint() ;
}

void Window :: OperationUngroupAllPrecompiles()
{
   FControler -> viewUngroupAllPrecompiles() ;
   repaint() ;
}

void Window :: OperationUngroupAllEyePos()
{
   FControler -> viewUngroupAllParameter(PARAMETER_EYE_POSITION) ;
   repaint() ;
}

void Window :: OperationUngroupAllAimedPos()
{
   FControler -> viewUngroupAllParameter(PARAMETER_AIMED_POSITION) ;
   repaint() ;
}

void Window :: OperationUngroupAllDrawing()
{
   FControler -> viewUngroupAllParameter(PARAMETER_DRAWING) ;
   repaint() ;
}

void Window :: OperationUngroupGeneral()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewUngroup(active -> getCliquedViewId()) ;
   repaint() ;
}

void Window :: OperationUngroupPrecompiles()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewUngroupPrecompiles(active -> getCliquedViewId()) ;
   repaint() ;
}

void Window :: OperationUngroupEyePos()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewUngroupParameter(active -> getCliquedViewId() ,
                                      PARAMETER_EYE_POSITION) ;
   repaint() ;
}

void Window :: OperationUngroupAimedPos()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewUngroupParameter(active -> getCliquedViewId() ,
                                      PARAMETER_AIMED_POSITION) ;
   repaint() ;
}

void Window :: OperationUngroupDrawing()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewUngroupParameter(active -> getCliquedViewId() ,
                                      PARAMETER_DRAWING) ;
   repaint() ;
}

void Window :: OperationGroupAllGeneral()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewGroupAll(active -> getCliquedViewId()) ;
   repaint() ;
}

void Window :: OperationGroupAllPrecomp()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewGroupAllPrecompiles(active -> getCliquedViewId()) ;
   repaint() ;
}

void Window :: OperationGroupAllEyePos()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewGroupAllParameter(active -> getCliquedViewId() ,
                                       PARAMETER_EYE_POSITION) ;
   repaint() ;
}

void Window :: OperationGroupAllAimedPos()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewGroupAllParameter(active -> getCliquedViewId() ,
                                       PARAMETER_AIMED_POSITION) ;
   repaint() ;
}

void Window :: OperationGroupAllDrawing()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewGroupAllParameter(active -> getCliquedViewId() ,
                                       PARAMETER_DRAWING) ;
   repaint() ;
}

void Window :: OperationGroupGeneral()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewGroup(active -> getCliquedViewId(),
                           FDoubleCliquee -> getDoubleCliquedViewId());
   repaint() ;
}

void Window :: OperationGroupPrecomp()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler->viewGroupPrecompiles(active -> getCliquedViewId(),
                                    FDoubleCliquee->getDoubleCliquedViewId());
   repaint() ;
}

void Window :: OperationGroupEyePos()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewGroupParameter(active -> getCliquedViewId() ,
                                    FDoubleCliquee -> getDoubleCliquedViewId(),
                                    PARAMETER_EYE_POSITION) ;
   repaint() ;
}

void Window :: OperationGroupAimedPos()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewGroupParameter(active -> getCliquedViewId() ,
                                    FDoubleCliquee -> getDoubleCliquedViewId(),
                                    PARAMETER_AIMED_POSITION) ;
   repaint() ;
}

void Window :: OperationGroupDrawing()
{
   GLWindow * active = ((GLWindow *) FWorkspace -> activeSubWindow()->widget()) ;
   FControler -> viewGroupParameter(active -> getCliquedViewId() ,
                                    FDoubleCliquee -> getDoubleCliquedViewId(),
                                    PARAMETER_DRAWING) ;
   repaint() ;
}

void Window :: mireOrigine()
{
   if (getControler()->lookAtOrigin(getCurrentViewId()))
      repaint();
   else updateStatusBar();
}

void Window :: mireFace()
{
   if (getControler()->lookAtFaceBary(getCurrentViewId()))
      repaint();
   else updateStatusBar() ;
}

void Window :: mireSommet()
{
   if (getControler()->lookAtLast(getCurrentViewId()))
      repaint();
   else updateStatusBar();
}

void Window :: mireVolume()
{
   if (getControler()->lookAtVolumeBary(getCurrentViewId()))
      repaint();
   else updateStatusBar();
}

void Window :: mireCompConnexe()
{
   if (getControler()->lookAtCcBary(getCurrentViewId()))
      repaint();
   else updateStatusBar();
}

void Window :: mireBrinsMarques()
{
   if (getControler()->lookAtSelectedBary(getCurrentViewId()))
      repaint();
   else updateStatusBar();
}

void Window :: mireBrins()
{
   if (getControler()->lookAtAllBary(getCurrentViewId()))
      repaint() ;
   else updateStatusBar() ;
}

void Window::callbackTopologicalView()
{
#ifdef MODULE_SPAMOD
   callbackSpamodViews(SPAMOD_TOPOLOGICAL);
#endif // MODULE_SPAMOD
}

void Window::callbackEuclidianView()
{
#ifdef MODULE_SPAMOD
   callbackSpamodViews(SPAMOD_EUCLIDIAN);
#endif // MODULE_SPAMOD
}

void Window::callbackAnalyticView()
{
#ifdef MODULE_SPAMOD
   callbackSpamodViews(SPAMOD_ANALYTIC);
#endif // MODULE_SPAMOD
}

void Window::callbackVoxelView()
{
#ifdef MODULE_SPAMOD
   callbackSpamodViews(SPAMOD_VOXEL);
#endif // MODULE_SPAMOD
}

void Window::callbackK2View()
{
#ifdef MODULE_SPAMOD
   callbackSpamodViews(SPAMOD_K2);
#endif // MODULE_SPAMOD
}

// Permet de revenir vers l'affichage d'origine de Moka
void Window::callbackBackToBurstView()
{
#ifdef MODULE_SPAMOD
   callbackSpamodViews(SPAMOD_NONE);
#endif // MODULE_SPAMOD
}

// Methode permettant de basculer vers l'une des visualisations de Spamod
bool Window::callbackSpamodViews(TSpamodViewMode mode)
{
#ifdef MODULE_SPAMOD
   if (mode != SPAMOD_NONE)
   {
      getControler()->viewEnablePrecompile(getCurrentViewId(),
                                           PRECOMPILE_SPAMOD);

      getControler()->viewDisablePrecompile(getCurrentViewId(),
                                            PRECOMPILE_DART);
      getControler()->viewDisablePrecompile(getCurrentViewId(),
                                            PRECOMPILE_FACE);
      getControler()->viewDisablePrecompile(getCurrentViewId(),
                                            PRECOMPILE_SEW);
      getControler()->viewDisablePrecompile(getCurrentViewId(),
                                            PRECOMPILE_VERTEX);
   }
   else
   {
      getControler()->viewDisablePrecompile(getCurrentViewId(),
                                            PRECOMPILE_SPAMOD);

      if (getSewsDisplay())
         getControler()->viewEnablePrecompile(getCurrentViewId(),
                                              PRECOMPILE_SEW);

      if (getVerticesDisplay())
         getControler()->viewEnablePrecompile(getCurrentViewId(),
                                              PRECOMPILE_VERTEX);

      if (getFacesDisplay())
         getControler()->viewEnablePrecompile(getCurrentViewId(),
                                              PRECOMPILE_FACE);

      getControler()->viewEnablePrecompile(getCurrentViewId(),
                                           PRECOMPILE_DART);
   }

   getParameterSpamod()->setViewMode(mode);
   repaint();
#endif // MODULE_SPAMOD

   return true;
}

void Window::callbackSpamodSwitchToNextView()
{
#ifdef MODULE_SPAMOD
   getParameterSpamod()->setNextViewMode();
   callbackSpamodViews(getParameterSpamod()->getViewMode());
#endif // MODULE_SPAMOD
}

void Window :: callbackHideAllWindow()
{
   if (FCreationActive != NULL &&  FCreationActive -> isVisible())
   {
      FCreationActive -> cancel();
      //  FCreationActive = NULL ;
   }
   if (FOperationActive != NULL && FOperationActive -> isVisible())
      FOperationActive -> callbackClose() ;

   if (FOptionsFrame != NULL && FOptionsFrame -> isVisible())
   {
      FOptionsFrame -> setFVisible(false) ;
      FOptionsFrame -> close() ;
   }
   if (FOperationChanfreinage != NULL &&
         FOperationChanfreinage -> isVisible())
      FOperationChanfreinage -> close() ;
   if (FOptionsCarac != NULL && FOptionsCarac -> isVisible())
      FOptionsCarac -> close() ;
   if (FDialogDo != NULL && FDialogDo -> isVisible())
      FDialogDo -> close() ;
   if (FCouleurs != NULL && FCouleurs->isVisible())
      FCouleurs -> close() ;
   if ( FOptionsSurfacicHomology!=NULL )
     {
       FOptionsSurfacicHomology -> close() ;
       delete FOptionsSurfacicHomology;
       FOptionsSurfacicHomology = NULL;
     }
   if ( FOptionsVolumicHomology!=NULL )
     {
       FOptionsVolumicHomology -> close() ;
       delete FOptionsVolumicHomology;
       FOptionsVolumicHomology = NULL;
     }
}

//---------------------------------------------------------------
CParameterAimedPosition* Window::getParameterAimedPosition() const
{ return getControler()->getParameterAimedPosition(getCurrentViewId()); }

CParameterDrawing* Window::getParameterDrawing() const
{ return getControler()->getParameterDrawing(getCurrentViewId()); }

CParameterEyePosition* Window::getParameterEyePosition() const
{ return getControler()->getParameterEyePosition(getCurrentViewId()); }

CParameterAimedPoint* Window::getParameterAimedPoint() const
{ return getControler()->getParameterAimedPoint(getCurrentViewId()); }

CParameterAxis* Window::getParameterAxis() const
{ return getControler()->getParameterAxis(getCurrentViewId()); }

CParameterGrid* Window::getParameterGrid() const
{ return getControler()->getParameterGrid(getCurrentViewId()); }

CParameterSew* Window::getParameterSew() const
{ return getControler()->getParameterSew(getCurrentViewId()); }

CParameterDart* Window::getParameterDart() const
{ return getControler()->getParameterDart(getCurrentViewId()); }

#ifdef MODULE_SPAMOD
CParameterSpamod* Window::getParameterSpamod() const
{ return getControler()->getParameterSpamod(getCurrentViewId()); }
#endif // MODULE_SPAMOD
