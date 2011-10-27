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

#ifndef WINDOW_QT_HH
#define WINDOW_QT_HH

//------------- Include controler ----------------
#include "controler-gmap.hh"
#include "controler.hh"
using namespace GMap3d ;

// Bidouille car ce salete de moc de Qt ne prend pas en compte
// les directives de precompilations
#ifndef MODULE_SPAMOD
namespace GMap3d
{
  typedef int TSpamodViewMode;
}
#endif
//------ Include autres classes developees -------

#include "creation-brin.qt.hh"
#include "creation-polygone.qt.hh"
#include "creation-maillage.qt.hh"
#include "creation-sphere.qt.hh"
#include "creation-cylindre.qt.hh"
#include "creation-pyramide.qt.hh"
#include "creation-tore.qt.hh"

#include "operation-chanfreinage.qt.hh"

#include "dialog-do.qt.hh"
#include "dialog-moveto.qt.hh"
#include "dialog-homothetie.qt.hh"
#include "dialog-translation.qt.hh"
#include "dialog-rotation.qt.hh"

#include "gl-window.qt.hh"
#include "select-bar.qt.hh"
#include "options-frame.qt.hh"
#include "menu-bar.qt.hh"
#include "options-carac.qt.hh"
#include "options-surfacic-homology.qt.hh"
#include "options-volumic-homology.qt.hh"
#include "options-couleurs.qt.hh"

//----------------- Include std ------------------
using namespace std ;
#include <iostream>
#include <cstdlib>

//------------------ Include QT -------------------
#include <QtCore/QDir>
#include <QtGui/QKeyEvent>
class QFileDialog;
         
#define SPACE_PIX_SIZE  15

class Window : public QMainWindow
{
  Q_OBJECT

public:

  /**
   * Constructeur
   */
  Window ( ) ;

  /**
   * Destructeur
   */
  virtual ~Window ( ) ;

  /**
   * Accesseur sur le controleur
   * @return le pointeur sur le controleur
   */
  virtual GMap3d :: CControlerGMap * getControler ( ) const ;

  /**
   * Redessine la fenetre et ses composants
   */
  virtual void repaint ( ) ;
  
  /**
   * Fermeture de la fenetre et enregistrement des parametres.
   */
  virtual void closeEvent ( QCloseEvent * event );

  /**
   * Accesseur sur la fenetre de creation
   * @return Pointeur sur la boite active
   */
  virtual CreationObjet * getCreationActive ( ) const ;

  /**
   * Accesseur sur la fenetre des operations
   * @return Pointeur sur la boite active
   */
  virtual dialogOperations * getOperationActive ( ) const ;

  /**
   * Accesseur sur la fenetre des couleurs
   * @return Pointeur sur la boite active
   */
  virtual OptionCouleurs * getCouleurActive ( ) const;

  /**
   * Accesseur sur la fenetre des caracteristques topologiques
   * @return Pointeur sur la boite active
   */
  virtual OptionsCarac * getOptionsCaracActive ( ) const ;

  /**
   * Accesseur sur la fenetre du calcul de l'homologie surfacique
   * @return Pointeur sur la boite active
   */
  virtual OptionsSurfacicHomology * getOptionsSurfacicHomologyActive() const ;

  /**
   * Accesseur sur la fenetre du calcul de l'homologie volumique
   * @return Pointeur sur la boite active
   */
  virtual OptionsVolumicHomology * getOptionsVolumicHomologyActive() const ;

  /**
   * Accesseur sur le numero de la fenetre active
   * @return Pointeur sur la boite active
   */
  virtual TViewId getCurrentViewId ( ) const ;

  /**
   * Accesseur en ecriture sur la derniere fenetre double cliquee
   * @param vue Vue cliquee
   */
  virtual void setDoubleCliquee ( GLWindow * vue ) ;

  /**
   * Mise a jour de la status barre
   */
  virtual void updateStatusBar ( ) ;
  
  /**
   * Ouvre une boite de dialogue demandant le nom du fichier a ouvrir.
   * Retourne l'adresse complete de ce fichier.
   * Si 'ind_filter' est different de NULL, l'indice du filtre selectionne
   * par l'utilisateur sera ecrit dans le pointeur.
   */
  std::string getOpenFileName ( const QString & caption = QString(), 
                                const QStringList & filters =
                                        QStringList("Tous les fichiers (*)"), 
                                int * ind_filter = NULL );
  
  /**
   * Ouvre une boite de dialogue demandant le nom du fichier a ouvrir.
   * Retourne l'adresse complete de ce fichier.
   * Si 'ind_filter' est different de NULL, l'indice du filtre selectionne
   * par l'utilisateur sera ecrit dans le pointeur.
   */
  std::string getSaveFileName ( const QString & caption = QString(), 
                                const QStringList & filters =
                                        QStringList("Tous les fichiers (*)"), 
                                int * ind_filter = NULL);

  const QColor LINK_COLOR;
  
private :

  void resetActiveOperations();

public slots:

  /**
   *  Pour detecter le changement de fenetre active dans le workspace.
   */
  void windowActivated( QWidget * w );
  
  void callbackKeyUp();
  void callbackKeyDown();
  void callbackKeyLeft();
  void callbackKeyRight();
  
  void callbackKeyCtrlUp();
  void callbackKeyCtrlDown();
  void callbackKeyCtrlLeft();
  void callbackKeyCtrlRight();
  
  void callbackKeyShiftUp();
  void callbackKeyShiftDown();
  void callbackKeyShiftLeft();
  void callbackKeyShiftRight();
  
  void callbackVueCompacte();
  void callbackVueSemiEclatee();
  void callbackVueMoka();
  void callbackVueTopoFil();
  
  // Slots Menu Fichier
  //*******************
  void callbackLoad ( ) ;
  void callbackAdd ( ) ;
  void callbackSave ( ) ;
  void callbackImport ( ) ;
  void callbackExport ( ) ;
  void callbackQuit ( ) ;
  
  void callbackFindMotif();
  void callbackCountMotifs();
  
  void callbackNettoyage ();
 
  void filterSelected ( const QString & selected );

  // Slots Menu Edition
  //*******************
  void callbackUndo ( ) ;
  void callbackRedo ( ) ;
  void callbackEmptyUndoRedo ( ) ;
  void callbackConfigUndoRedo ( ) ;

  // Slots Menu Options
  //*******************

  void help ( ) ;
  //  void grid ( ) ;
  void view ( ) ;
  void plating ( ) ;
  void pondDef ( ) ;
  void extrusion ( ) ;
  void interpol ( ) ;
  void affich ( ) ;

  void caracTopo ( ) ;
  void callback2Manifold ( ) ;
  void couleurs ( ) ;
  void affichDivers ( );
  
  void callbackToggleSews();
  void callbackToggleNormal();
  void callbackToggleVertices();
  void callbackToggleFaces();
  void callbackTournerButton();
  void callbackToggleGrille();

  // A deplacer dans Operation
  void chanf ( ) ;

  // Slots Menu Creer
  //*****************
  void callbackPolyline ( ) ;
  void callbackPolygon ( ) ;
  void callbackMeshCreation ( ) ;
  void callbackSphere ( ) ;
  void callbackCylinder ( ) ;
  void callbackPyramide ( ) ;
  void callbackTorus ( ) ;
  void callbackRandomizeDarts();
  
  // Slots Menu Operations
  //**********************
  // -- translation/rotation/homothetie
  void callbackTranslateWindow ( ) ;
  void callbackRotateWindow ( ) ;
  void callbackScaleWindow ( ) ;
  void callbackMovetoWindow ( ) ;

  // -- coudre
  void callbackIntuitiveTopoSew ( ) ;
  void callbackIntuitiveGeoSew  ( ) ;

  void callbackTopoSew0 ( ) ;

  void callbackTopoSew1 ( ) ;
  void callbackGeoSew1 ( ) ;

  void callbackTopoSew2 ( ) ;
  void callbackGeoSew2  ( ) ;

  void callbackTopoSew3 ( ) ;
  void callbackGeoSew3 ( ) ;

  void callbackTopoSew1borders ( ) ;
  void callbackGeoSew1borders ( ) ;

  void callbackTopoSew2borders ( ) ;
  void callbackGeoSew2borders ( ) ;

  void callbackInvertLastOperation ( ) ;
  void callbackInvertLastOperationGeometry ( ) ;

  // -- decoudre
  void callbackIntuitiveUnsew ( ) ;
  void callbackUnsew0 ( ) ;
  void callbackUnsew1 ( ) ;
  void callbackUnsew2 ( ) ;
  void callbackUnsew3 ( ) ;

  // -- placage
  void callbackIntuitivePlate ( ) ;
  void callbackPlate0 ( ) ;
  void callbackPlate1 ( ) ;
  void callbackPlate2 ( ) ;

  void callbackPlate1borders  ( ) ;
  void callbackPlate2borders  ( ) ;

  // -- Fusionner
  void callbackIntuitiveMerge ( ) ;
  void callbackMergeEdges ( ) ;
  void callbackMergeFaces ( ) ;
  void callbackMergeVolumes ( ) ;

  void callbackMergeColinearEdges ( ) ;
  void callbackMergeCoplanarFaces ( ) ;
  
  void callbackMergeFacesWithoutDisconnection ( ) ;
  void callbackRemoveMarkedFacesButKeepBalls ( ) ;
  void callbackShiftAllEdgesIncidentToVertex ( ) ;
  void callbackRemoveDanglingEdges();
  void callbackSimplify2DObject();
  void callbackSimplify3DObject();
  
  // -- Contracter
  void callbackIntuitiveContract ( ) ;

  void callbackContractEdges ( ) ;
  void callbackContractFaces ( ) ;
  void callbackContractVolumes ( ) ;

  void callbackDelNullEdges ( ) ;
  void callbackDelFlatFaces ( ) ;
  void callbackDelFlatVolumes ( ) ;

  // -- Inserer
  void callbackInsertVertex ( ) ;
  void callbackInsertEdge ( ) ;
  void callbackInsertFace ( ) ;

  // -- Fermer
  void callbackIntuitiveStopUp ( ) ;
  void callbackStopUp0 ( ) ;
  void callbackStopUp1 ( ) ;
  void callbackStopUp2 ( ) ;
  void callbackStopUp3 ( ) ;

  // -- Trianguler
  void callbackContextTriangulate ( ) ;
  void callbackTriangulateEdges   ( ) ;
  void callbackTriangulateFaces   ( ) ;
  void callbackTriangulateVolumes ( );
  void callbackSwapEdge           ( );
  void callbackTriangulateGeoFaces( );

  // -- Quadranguler
  void callbackQuadrangulateFaces ( ) ;
  void callbackBasicQuadrangulateFaces ( ) ;

  // -- Calculer Dual
  void callbackDual2d ( ) ;
  void callbackDual3d ( ) ;

  // -- Extruder
  // Selon normale
  void callbackIntuitiveExtrudeByNormal ( ) ;
  void callbackExtrude0ByNormal ( ) ;
  void callbackExtrude1ByNormal ( ) ;
  void callbackExtrude2ByNormal ( ) ;

  // Selon chemin
  void callbackExtrude1ByPath ( ) ;
  void callbackExtrude2ByPath ( ) ;

  // Selon axe
  void callbackExtrude1ByRevolution ( ) ;
  void callbackExtrude2ByRevolution ( ) ;
  
  /// Epaissir
  void callbackThicken ( ) ;

  // -- Interpoler
  void callbackMeshEdge ( ) ;
  void callbackMeshSquare01 ( ) ;
  void callbackMeshSquare02 ( ) ;
  void callbackMeshSquare12 ( ) ;

  void callbackMeshCube01 ( ) ;
  void callbackMeshCube02 ( ) ;
  void callbackMeshCube03 ( ) ;
  void callbackMeshCube12 ( ) ;
  void callbackMeshCube13 ( ) ;
  void callbackMeshCube23 ( ) ;

  // -- Lisser
  void callbackSmooth1 ( ) ;
  void callbackSmooth2 ( ) ;
  void callbackSmooth3 ( ) ;

  // -- Arrondir
  void callbackRoundVertices ( ) ;
  void callbackRoundVerticesDig ( ) ;
  void callbackRoundEdges2d ( ) ;
  void callbackRoundEdges2dDig		       ();
  void callbackRoundEdges3d ( ) ;
  void callbackRoundEdges3dDig ( ) ;
  void callbackRoundEdges2dWithSetback	       ();
  void callbackRoundEdges2dDigWithSetback      ();
  void callbackRoundEdges3dWithSetback ( ) ;
  void callbackRoundEdges3dDigWithSetback ( ) ;

  // -- Triangulation
  void callbackTriangulateTopoFaces            ();
  void callbackMarkNonUniformFaces             ();
  void callbackOptimizeSelectedEdges           ();
  void callbackDelaunayTriangulation           ();
  void callbackOptimizedDelaunayTriangulation  ();
  //void callbackCreateBSplineSurface            ();
  //void callbackgetSurfaceIntersection          ();

  // -- Co-raffiner
  void callbackCorefine2dSegmentsSweeping ( ) ;
  void callbackCorefine2dSegmentsPropagation ( ) ;
  void callbackBooleanOperations2d ( ) ;
  void callbackCorefine3dMeshes ( ) ;
  void callbackCorefine3dFaces ( ) ;
  void callbackCorefineMarked3dFaces ( ) ;
  void callbackCorefineMarked3dMeshesWith ( ) ;
  void callbackBooleanOperations3d ( ) ;
  //void callbackCorefineTest ( ) ;

  // -- Homology
  void callbackComputeSurfacicHomology();
  void callbackComputeVolumicHomology();
  
  
  // Slots Menu Geologie
  //********************
  void callbackUncertainZone                     ();
  void callbackCreateNearestIntersections        ();
  void callbackExtendSelectedBorders             ();
  void callbackExtendSelectedBordersToSurface    ();
  void callbackSmoothSelection                   ();
  void callbackRelaxSelection                    ();
  void callbackSimplifyMesh                      ();
  void callbackCenterSelection                   ();
  void callbackPlateVerticesOnFaces              ();
  void callbackPlateVerticesOnEdges              ();
  void callbackPlateVerticesOnVertices           ();
  void callbackSelectMacroVertices               ();
  void callbackSelectAllMacroEdges               ();
  void callbackSelectMacroEdges                  ();
  void callbackSelectMacroFaces                  ();
  void callbackSelectBordersBetweenSelectedDarts ();
  void callbackImportGr2d                        ();
  void callbackImportGr2dOk                      (); //-
  void callbackImportXyz                         ();
  void callbackImportXyzOk                       (); //-
  void callbackImportTs                          ();
  void callbackImportTsOk                        (); //-
  void callbackImportCloud                       ();
  void callbackImportCloudOk                     (); //-
  
  void callbackHolesRefinement                   ();
  void callbackTriangulateHoles                  ();
  void callbackFillHolesOnMeshes                 ();
  void callbackFailleDetection                   ();
  void callbackExtractDetectedFailles            ();
  void callbackExtensionFailleSurface            ();
  //void callbackGetIntersectionSurface            ();
  //void callbackCollapseEdge                      ();
  //void callbackRemoveVertex                      ();
  void callbackMeshSimplification                ();
  void callbackImportVrml                        ();
  void callbackExportTs                          ();

  // Slots Menu Materialiser
  //************************
  void callbackMaterializeBarycenter           ();
  void callbackMaterializeAxe                  ();
  void callbackMaterializePlane                ();
  void callbackMaterializeNormalVector         ();
  void callbackMaterializeOrthoPlane           ();
  void callbackMaterializeReferential          ();

  // Slots Menu Selection
  //*********************
  void callbackReinitSelection ( ) ;
  void callbackNegateSelection ( ) ;
  void callbackSwapSelections ( ) ;

  // Copie / Destruction
  void callbackDuplicateSelection ( ) ;
  void callbackDeleteSelection ( ) ;

  // Etendre d'1 pas
  void callbackGoForward1 ( ) ;
  void callbackGoBackward1 ( ) ;
  void callbackGoLeft1 ( ) ;
  void callbackGoRight1 ( ) ;

  // Etendre Repetitivement
  void callbackGoForward1Rep ( ) ;
  void callbackGoBackward1Rep ( ) ;
  void callbackGoLeft1Rep ( ) ;
  void callbackGoRight1Rep ( ) ;

  // Reduire d'1 pas
  void callbackGoForward0 ( )  ;
  void callbackGoBackward0 ( ) ;
  void callbackGoLeft0 ( ) ;
  void callbackGoRight0 ( ) ;

  // Reduire repetitivement
  void callbackGoForward0Rep ( ) ;
  void callbackGoBackward0Rep ( ) ;
  void callbackGoLeft0Rep ( ) ;
  void callbackGoRight0Rep ( ) ;

  // Marquages divers
  void callbackMarkInterior2 ( ) ;
  void callbackMarkInterior3 ( ) ;

  void callbackIntuitiveSelectBorders ( ) ;
  void callbackSelect0borders ( ) ;
  void callbackSelect1borders ( ) ;
  void callbackSelect2borders ( ) ;
  void callbackSelect3borders ( ) ;
  void callbackSelectNonPlanarFaces ( ) ;
  void callbackSelectIncidents ( ) ;
  
  // Avancer
  void callbackGoAlpha0 ( ) ;  //Par Alpha0
  void callbackGoAlpha1 ( ) ;
  void callbackGoAlpha2 ( ) ;
  void callbackGoAlpha3 ( ) ;

  // Slots Menu Vues
  //****************
  void tile ( ) ;
  void cascade ( ) ;

  void addView3D ( ) ;
  void addViewXY ( ) ;
  void addViewYZ ( ) ;
  void addViewZX ( ) ;
  void addViewMulti ( ) ;

  void basculeView3D ( ) ;
  void basculeViewXY ( ) ;
  void basculeViewYZ ( ) ;
  void basculeViewZX ( ) ;
  void basculeViewMulti ( ) ;

  void deleteView ( ) ;

  void mireOrigine ( ) ;
  void mireFace ( ) ;
  void mireSommet ( ) ;
  void mireVolume ( ) ;
  void mireCompConnexe ( ) ;
  void mireBrinsMarques ( ) ;
  void mireBrins ( ) ;

  void callbackBackToBurstView                 ();
  void callbackTopologicalView                 ();
  void callbackEuclidianView                   ();
  void callbackAnalyticView                    ();
  void callbackVoxelView                       ();
  void callbackK2View                          ();

  bool callbackSpamodViews(GMap3d::TSpamodViewMode);

  void callbackSpamodSwitchToNextView          ();

  void OperationUngroupAllGeneral ( ) ;
  void OperationUngroupAllPrecompiles ( ) ;
  void OperationUngroupAllEyePos ( ) ;
  void OperationUngroupAllAimedPos ( ) ;
  void OperationUngroupAllDrawing ( ) ;
  void OperationUngroupGeneral ( ) ;
  void OperationUngroupPrecompiles ( ) ;
  void OperationUngroupEyePos ( ) ;
  void OperationUngroupAimedPos ( ) ;
  void OperationUngroupDrawing ( ) ;

  void OperationGroupAllGeneral ( ) ;
  void OperationGroupAllPrecomp ( ) ;
  void OperationGroupAllEyePos ( ) ;
  void OperationGroupAllAimedPos ( ) ;
  void OperationGroupAllDrawing ( ) ;
  void OperationGroupGeneral ( ) ;
  void OperationGroupPrecomp ( ) ;
  void OperationGroupEyePos ( ) ;
  void OperationGroupAimedPos ( ) ;
  void OperationGroupDrawing ( ) ;

  // Cache toutes les fenetres ouvertes
  void callbackHideAllWindow ( ) ;

  // Methode qui bascule l'affichage vers une vue de typa souhaite
  void bascule ( TView type ) ;

  // Methode qui construit les boites a outils
  void creationBoitesOutils ( ) ;

  // Methodes utilises pour interpoler
  void callbackMeshSquare (int i , int j) ;
  void callbackMeshCube (int i , int j) ;

  // Methodes utilisees pour Arrondir
  void roundVertices(bool ADig);
  void roundEdges ( bool A3D, bool ADig, bool ASetback);

  /***************************************************************************/
public:
  // Parametres d'affichage
  bool getVerticesDisplay() const;
  void setVerticesDisplay(bool b);

  bool getNormalDisplay() const;
  void setNormalDisplay(bool b);

  bool getFacesDisplay() const;
  void setFacesDisplay(bool b);

  bool getFacesCoulAlea () const;
  void setFacesCoulAlea ();
  
  bool getVolumesCoulAlea () const;
  void setVolumesCoulAlea ();
  
  bool getCoulFacesGlobale () const;
  void setCoulFacesGlobale ();

  bool getCoulTopologie () const;
  void setCoulTopologie ();
  
  bool getCoulGeometry () const;
  void setCoulGeometry ();

  bool getSewsDisplay() const;
  void setSewsDisplay(bool b);

  bool getGridDisplay() const;
  void setGridDisplay(bool b);

  bool getAxisDisplay() const;
  void setAxisDisplay(bool b);

  // Pour spamod, affichage ou non des brins
  void toggleDarts(bool b);

  // Methodes pour recuperer les parametres associes a la vue courante
  CParameterAimedPosition*        getParameterAimedPosition() const;
  CParameterDrawing*              getParameterDrawing() const;
  CParameterEyePosition*          getParameterEyePosition() const;
  CParameterAimedPoint*    getParameterAimedPoint() const;
  CParameterAxis*          getParameterAxis() const;
  CParameterGrid*            getParameterGrid() const;
  
  GMap3d::CParameterDart*          getParameterDart() const;
  GMap3d::CParameterFace*     getParameterFace() const;
  GMap3d::CParameterPreview*              getParameterPreview() const;
  GMap3d::CParameterNormalVector*    getParameterNormalVector() const;
  GMap3d::CParameterObjectTransformation* getParameterObjectTransformation()
                                          const;
  GMap3d::CParameterSew*     getParameterSew() const;
  
  GMap3d::CParameterVertex*     getParameterVertex() const;
  
#ifdef MODULE_SPAMOD
  GMap3d::CParameterSpamod*     	  getParameterSpamod		  () const;
#endif // MODULE_SPAMOD

  //************************************************************************/
private:
  // Pointeur sur le controleur
  GMap3d :: CControlerGMap * FControler ;

  // Pointeur sur un workspace
  QWorkspace * FWorkspace ;
  
  // Pointeur sur la fenetre de construction active
  CreationObjet * FCreationActive ;
  
  // Pointeurs sur chacunes des toolbars de construction.
  CreationObjet * FCreationBrin ;
  CreationObjet * FCreationCylindre ;
  CreationObjet * FCreationMaillage ;
  CreationObjet * FCreationPolygone ;
  CreationObjet * FCreationPyramide ;
  CreationObjet * FCreationSphere ;
  CreationObjet * FCreationTore ; 
  
  // Pointeurs sur chacunes des toolbars d'operation.
  dialogOperations * FDialogRotation ;
  dialogOperations * FDialogHomothetie ;
  dialogOperations * FDialogTranslation ;
  dialogOperations * FDialogMoveto ;
   
#ifdef MODULE_GEOLOGY
  // Pointeur sur la fenetre de construction active
  DialogMesh * FMeshActive ;
#endif // MODULE_GEOLOGY
  
  // Pointeur sur l'operation active 
  // Operations : Rotation, translation, homothetie, moveto
  dialogOperations * FOperationActive ;

  // Pointeur sur l'ensemble des actions
  OptionsFrame * FOptionsFrame ;

  // Pointeur sur la boite de dialogue chanfreinage
  OperationChanfreinage * FOperationChanfreinage ;

  // Pointeur sur la boite de dialogue des caracteristiques topologiques
  OptionsCarac * FOptionsCarac ;

  // Pointeur sur la boite de dialogue de calcul de l'homology surfacique
  OptionsSurfacicHomology * FOptionsSurfacicHomology ;

  // Pointeur sur la boite de dialogue de calcul de l'homology volumique
  OptionsVolumicHomology * FOptionsVolumicHomology ;

  // Pointeur sur la boite de dialogue de configuration des Undo/Redo
  DialogDo * FDialogDo ;

  // Vue mere
  GLWindow * FVueMere ;

  // Derniere vue double cliquee
  GLWindow * FDoubleCliquee ;

  // Barre de selection
  SelectBar * FSelection ;

  // Boite des couleurs
  OptionCouleurs * FCouleurs ;
  bool color_active;

  Window * FParent; 
  
  MenuBar * main_menu;
  
  // Les Toolbars.
  QToolBar * fichier;
  QToolBar * edition;
  QToolBar * creation;
  QToolBar * operations;
  QToolBar * vues;
  QToolBar * selection;
  
  // Les actions dynamiques.
  QAction * charger_act;
  QAction * sauver_act;
  QAction * importer_act;
  QAction * exporter_act;
  
  // Le repertoire courant.
  QDir current_dir;
  
  // La boite de dialogue de sélection de fichier en cours.
  QFileDialog* FileDialog;  
};

#endif

