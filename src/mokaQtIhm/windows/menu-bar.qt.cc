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

#include "menu-bar.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QPixmap>

//*****************************************
// Constructeur
//*****************************************
MenuBar :: MenuBar(Window * parent)
      :
      QMenuBar(parent),
      FParent(parent)
{
   QIcon pf0(":/nouveau.png");
   QIcon pf1(":/charger.png") ;
   QIcon pf2(":/ajouter.png") ;
   QIcon pf3(":/sauver.png") ;
   QIcon pf7(":/file-import-48x48.png") ;
   QIcon pf4(":/file-export-48x48.png") ;
   QIcon pf5(":/quitter.png") ;
   QIcon pf6(":/etage.png") ;

   QIcon pe1(":/undo.png") ;
   QIcon pe2(":/redo.png") ;
   QIcon pe3(":/vider.png") ;
   QIcon pe4(":/boutonOptions.png") ;

   QIcon po1(":/aide.png") ;
   QIcon po2(":/grille.png") ;

   QIcon pc1(":/polyligne.png") ;
   QIcon pc2(":/polygone.png") ;
   QIcon pc3(":/maillage.png") ;
   QIcon pc4(":/sphere.png") ;
   QIcon pc5(":/cylindre.png") ;
   QIcon pc6(":/cone.png") ;
   QIcon pc7(":/tore.png") ;

   QIcon pop1(":/homothetie.png") ;
   QIcon pop2(":/translation.png") ;
   QIcon pop3(":/rotation.png") ;

   QIcon pv1(":/addvue.png") ;
   QIcon pv2(":/deletevue.png") ;
   QIcon pv3(":/groupervues.png") ;
   QIcon pv4(":/degroupervues.png") ;

#ifdef MODULE_ARCHITECTURE
   QString* str_piece = HTML::decode("Pi&egrave;ces");
   QString* str_fenetre = HTML::decode("Fen&ecirc;tres");
#endif // MODULE_ARCHITECTURE

   // Creation du menu Fichier
   //-------------------------
   fichier = new QMenu("&Fichier");
   fichier_act = addMenu(fichier);
   
   fichier -> addAction(pf0 , "&Nouveau" , FParent ,
                        SLOT(callbackNettoyage()) , Qt::CTRL + Qt::Key_N) ;
   
   charger_act = fichier -> addAction(pf1 , "&Charger" , FParent ,
                                      SLOT(callbackLoad()) ,
                                      Qt::CTRL + Qt::Key_O) ;
   
   ajouter_act = fichier -> addAction(pf2 , "&Ajouter" , FParent ,
                                      SLOT(callbackAdd()) ,
                                           Qt::CTRL + Qt::Key_A) ;

   sauver_act = fichier -> addAction(pf3 , "&Sauver" , FParent ,
                                     SLOT(callbackSave()) ,
                                     Qt::CTRL + Qt::Key_S) ;
   
   fichier -> addSeparator() ;
   
   importer_act = fichier -> addAction(pf7 , "Importer" , FParent ,
                                       SLOT(callbackImport())) ;
   
   exporter_act = fichier -> addAction(pf4 , "Exporter" , FParent ,
                                       SLOT(callbackExport())) ;
   
   fichier -> addSeparator() ;
   
   fichier -> addAction("Chercher motif" , FParent ,
                        SLOT(callbackFindMotif())) ;
   
   fichier -> addAction("Compter tout les motifs" , FParent ,
                        SLOT(callbackCountMotifs())) ;
   
   fichier -> addSeparator() ;

#ifdef MODULE_ARCHITECTURE
   ajout_etage_act = fichier ->
                     addAction(pf6 , *HTML::decode("&Ajouter &eacute;tage 3D"),
                               FParent, SLOT(callbackEtage3D()) ,
                               Qt::CTRL + Qt::Key_T) ;
   
   ajout_plan2D_act = fichier -> addAction(pf6, "Ajouter un plan 2D", FParent,
                                           SLOT(callbackAjoutPlan2D()),
                                           Qt::CTRL + Qt::Key_Y);
   
   fichier -> addSeparator() ;
   ajout_etage_act -> setVisible(false);
   ajout_plan2D_act -> setVisible(false);
#endif

   fichier -> addAction(pf5 , "Quitter" , FParent ,
                        SLOT(callbackQuit()) , Qt::CTRL + Qt::Key_Q) ;

   // Creation du menu Edition
   //-------------------------
   edition = new QMenu("&Edition");
   edition_act = addMenu(edition);

   edition -> addAction(pe1 , "&Annuler" , FParent ,
                        SLOT(callbackUndo()) , Qt::Key_U) ;
   edition -> addAction(pe2 , "&Refaire" , FParent ,
                        SLOT(callbackRedo()) , Qt::SHIFT + Qt::Key_U) ;

   edition -> addSeparator() ;

   edition -> addAction(pe3 , "&Vider la pile" , FParent ,
                        SLOT(callbackEmptyUndoRedo()) , Qt::ALT + Qt::Key_U);

   edition -> addSeparator() ;

   edition -> addAction(pe4 , "&Configurations" , FParent ,
                        SLOT(callbackConfigUndoRedo()) , Qt::CTRL + Qt::Key_U);

   // Creation du menu Options
   //-------------------------
   options = new QMenu("&Options");
   options_act = addMenu(options);

   options -> addAction(po1 , "&Aide succinte" , FParent ,
                        SLOT(help()) , Qt::Key_F1) ;

   options -> addSeparator() ;

   options ->  addAction("&Affichage / Placages" , FParent ,
                         SLOT(affich()) , Qt::Key_F2) ;

   options->addAction(*HTML::decode("&Vue &eacute;clat&eacute;e"), FParent,
                      SLOT(view()) , Qt::Key_F3) ;

   options -> addSeparator() ;

   //options ->  addAction ( "&Placages" , FParent ,
//     SLOT (  plating ( ) ) , Qt::Key_F4 ) ;

   options ->  addAction(*HTML::
                         decode("Pond&eacute;ration des &d&eacute;formations"),
                                FParent, SLOT(pondDef()) , Qt::Key_F4) ;
   options ->  addAction("&Extrusion/Epaississement" , FParent ,
                         SLOT(extrusion()) , Qt::Key_F5) ;
#ifdef MODULE_MESH
   options ->  addAction("&Interpolations/Lissages" , FParent ,
                         SLOT(interpol()) , Qt::Key_F6) ;
#endif

   options ->  addAction(*HTML::decode("D&eacute;placement cam&eacute;ra"),
                         FParent ,
                         SLOT(affichDivers()) , Qt::Key_F7) ;

   options -> addSeparator() ;

   options ->  addAction(*HTML::decode("Co&uleurs") , FParent ,
                         SLOT(couleurs()) , Qt::Key_F8) ;

   options -> addSeparator() ;

   options->addAction(*HTML::decode("Caract&eacute;ristiques &topologiques"),
                      FParent,
                      SLOT(caracTopo()), *new QKeySequence("ALT+c")) ;

   // Creation du menu creations
   //---------------------------
   creations = new QMenu(*HTML::decode("C&r&eacute;er"));
   creations_act = addMenu(creations);

   creations -> addAction(pc1 , "&Brin/Polyligne/Face" , FParent ,
                          SLOT(callbackPolyline()) ,
                          *new QKeySequence("CTRL+x,CTRL+1")) ;
   creations -> addAction(pc2 , *HTML::decode("&Polygone r&eacute;gulier"),
                          FParent ,
                          SLOT(callbackPolygon()) ,
                          *new QKeySequence("CTRL+x,CTRL+2")) ;
   creations -> addAction(pc3 , "&Maillage" , FParent ,
                          SLOT(callbackMeshCreation()) ,
                          *new QKeySequence("CTRL+x,CTRL+3")) ;
   creations -> addAction(pc4 , *HTML::decode("&Sph&egrave;re") , FParent ,
                          SLOT(callbackSphere()) ,
                          *new QKeySequence("CTRL+x,CTRL+4")) ;
   creations -> addAction(pc5 , "&Cylindre" , FParent ,
                          SLOT(callbackCylinder()) ,
                          *new QKeySequence("CTRL+x,CTRL+5")) ;
   creations -> addAction(pc6 , *HTML::decode("P&yramide/C&ocirc;ne") ,
                          FParent ,
                          SLOT(callbackPyramide()),
                          *new QKeySequence("CTRL+x,CTRL+6")) ;
   creations -> addAction(pc7 , "&Tore" , FParent ,
                          SLOT(callbackTorus()) ,
                          *new QKeySequence("CTRL+x,CTRL+7")) ;

   creations -> addSeparator() ;
   
   creations -> addAction("&Randomize darts" , FParent ,
                          SLOT(callbackRandomizeDarts()));
   
   // Creation du menu Operations
   //----------------------------
   operations = new QMenu(*HTML::decode("O&p&eacute;rations"));
   operations_act = addMenu(operations);

   // -- translation/rotation/homothetie
   operations ->  addAction(pop2 , "&Translation" , FParent ,
                            SLOT(callbackTranslateWindow()) , Qt::Key_T) ;
   operations ->  addAction(pop3 , "&Rotation" , FParent ,
                            SLOT(callbackRotateWindow()) , Qt::Key_R) ;
   operations ->  addAction(pop1 , *HTML::decode("&Homoth&eacute;tie") ,
                            FParent ,
                            SLOT(callbackScaleWindow()) , Qt::Key_H) ;
   operations ->  addAction("&Move to" , FParent ,
                            SLOT(callbackMovetoWindow()),
			    Qt::SHIFT + Qt::Key_T) ;

   operations -> addSeparator() ;

   //-- coudre
   QMenu * coudre = operations -> addMenu("&Coudre") ;

   QMenu * intuitif = coudre ->  addMenu("&Intuitivement") ;
   intuitif ->  addAction("&Topologiquement" , FParent ,
                          SLOT(callbackIntuitiveTopoSew()) ,
                          Qt::SHIFT + Qt::Key_C) ;
   intuitif ->  addAction(*HTML::decode("&G&eacute;om&eacute;triquement") ,
                          FParent ,
                          SLOT(callbackIntuitiveGeoSew()) , Qt::Key_C) ;

   coudre -> addSeparator() ;

   coudre ->  addAction("&Sommets" , FParent , SLOT(callbackTopoSew0()));

   QMenu * coudreAretes = coudre ->  addMenu(*HTML::decode("&Ar&ecirc;tes"));
   coudreAretes ->  addAction("&Topologiquement" , FParent ,
                              SLOT(callbackTopoSew1())) ;
   coudreAretes->addAction(*HTML::decode("&G&eacute;om&eacute;triquement"),
                           FParent ,
                           SLOT(callbackGeoSew1())) ;

   QMenu * coudreFaces = coudre ->  addMenu("&Faces") ;
   coudreFaces ->  addAction("&Topologiquement" , FParent ,
                             SLOT(callbackTopoSew2())) ;
   coudreFaces ->  addAction(*HTML::decode("&G&eacute;om&eacute;triquement"),
                             FParent ,
                             SLOT(callbackGeoSew2())) ;

   QMenu * coudreVolumes = coudre ->  addMenu("&Volumes") ;
   coudreVolumes ->  addAction("&Topologiquement" , FParent ,
                               SLOT(callbackTopoSew3())) ;
   coudreVolumes->addAction(*HTML::decode("&G&eacute;om&eacute;triquement") ,
                            FParent ,
                            SLOT(callbackGeoSew3())) ;

   coudre -> addSeparator() ;

   QMenu * coudre1bord = coudre ->  addMenu("1-&bord") ;
   coudre1bord ->  addAction("&Topologiquement" , FParent ,
                             SLOT(callbackTopoSew1borders())) ;
   coudre1bord->addAction(*HTML::decode("&G&eacute;om&eacute;triquement") ,
                          FParent ,
                          SLOT(callbackGeoSew1borders())) ;

   QMenu * coudre2bords = coudre ->  addMenu("2-b&ords") ;
   coudre2bords ->  addAction("&Topologiquement" , FParent ,
                              SLOT(callbackTopoSew2borders()), 
                                   Qt::SHIFT + Qt::Key_B) ;
   coudre2bords ->  addAction(*HTML::decode("&G&eacute;om&eacute;triquement"),
                              FParent ,
                              SLOT(callbackGeoSew2borders()) , Qt::Key_B) ;

   coudre -> addSeparator() ;

   QMenu * inverserCouture =
      coudre->addMenu(*HTML::decode("Inverser la derni&egrave;re &couture"));
   inverserCouture ->  addAction("&Orientation" , FParent ,
                                 SLOT(callbackInvertLastOperation()) , 
                                      Qt::Key_X) ;
   inverserCouture ->  addAction("&Sens" , FParent ,
                                 SLOT(callbackInvertLastOperationGeometry()),
                                 Qt::SHIFT + Qt::Key_X) ;

   // -- Decoudre
   QMenu * decoudre = operations->addMenu(*HTML::decode("&D&eacute;coudre"));

   decoudre -> addAction("&Intuitivement" , FParent ,
                         SLOT(callbackIntuitiveUnsew()) , Qt::Key_D) ;

   decoudre -> addSeparator() ;

   decoudre -> addAction("&Sommets" , FParent ,
                         SLOT(callbackUnsew0())) ;
   decoudre -> addAction("&Aretes" , FParent ,
                         SLOT(callbackUnsew1())) ;
   decoudre -> addAction("&Faces" , FParent ,
                         SLOT(callbackUnsew2())) ;
   decoudre -> addAction("&Volumes" , FParent ,
                         SLOT(callbackUnsew3())) ;

   // -- Placage
   QMenu * placage = operations -> addMenu("&Placage") ;

   placage -> addAction("&Intuitif" , FParent ,
                        SLOT(callbackIntuitivePlate()) , Qt::Key_P) ;

   placage -> addSeparator() ;

   placage -> addAction("&Sommets" , FParent , SLOT(callbackPlate0()));
   placage -> addAction("&Aretes" , FParent , SLOT(callbackPlate1()));
   placage -> addAction("&Faces" , FParent , SLOT(callbackPlate2()));

   placage -> addSeparator() ;

   placage -> addAction("1-bord" , FParent ,
                        SLOT(callbackPlate1borders())) ;
   placage -> addAction("2-bords" , FParent ,
                        SLOT(callbackPlate2borders())) ;

   operations -> addSeparator() ;

   // -- Fusionner
   QMenu * fusionner = operations -> addMenu("&Fusionner   ") ;

   fusionner -> addAction("&Intuitivement" , FParent ,
                          SLOT(callbackIntuitiveMerge()) , Qt::Key_F) ;

   fusionner -> addSeparator() ;

   fusionner -> addAction("&Aretes", FParent, SLOT(callbackMergeEdges()));
   fusionner -> addAction("&Faces", FParent, SLOT(callbackMergeFaces()));
   fusionner -> addAction("&Volumes", FParent, SLOT(callbackMergeVolumes()));

   fusionner -> addSeparator() ;

   fusionner -> addAction(*HTML::decode("Aretes align&eacute;es") , FParent ,
                          SLOT(callbackMergeColinearEdges()))  ;
   fusionner -> addAction("Faces coplanaires" , FParent ,
                          SLOT(callbackMergeCoplanarFaces())) ;
         
   fusionner -> addSeparator() ;

   fusionner -> addAction(*HTML::decode("Faces sans d&eacute;connexion") ,
                          FParent ,
                          SLOT(callbackMergeFacesWithoutDisconnection()));
   
   fusionner -> addAction(*HTML::decode("Volumes en gardant des boules") ,
                           FParent ,
                           SLOT(callbackRemoveMarkedFacesButKeepBalls()));
   
   
   fusionner->addAction(*HTML::decode("D&eacute;caler les ar&ecirc;tes"
                                      " incidentes au sommet"), FParent ,
                        SLOT(callbackShiftAllEdgesIncidentToVertex()));
   fusionner->addAction(*HTML::decode("Supprimer les ar&ecirc;tes pendantes"),
                        FParent, SLOT(callbackRemoveDanglingEdges()));

   // -- Contracter
   QMenu * contracter = operations -> addMenu("&Contracter") ;

   contracter -> addAction("&Intuitivement" , FParent ,
                           SLOT(callbackIntuitiveContract()) , 
                                Qt::SHIFT + Qt::Key_F);

   contracter -> addSeparator() ;

   contracter -> addAction("&Aretes" , FParent ,
                           SLOT(callbackContractEdges())) ;
   contracter -> addAction("&Faces" ,  FParent ,
                           SLOT(callbackContractFaces())) ;
   contracter -> addAction("&Volumes" , FParent ,
                           SLOT(callbackContractVolumes())) ;

   contracter -> addSeparator() ;


   contracter->addAction(*HTML::decode("Ar&ecirc;tes de &longueur nulle"),
                         FParent , SLOT(callbackDelNullEdges())) ;
   contracter -> addAction("Faces &plates" ,  FParent ,
                           SLOT(callbackDelFlatFaces())) ;
   contracter -> addAction("V&olumes plats" , FParent ,
                           SLOT(callbackDelFlatVolumes())) ;

   // -- Inserer
   QMenu * inserer = operations -> addMenu(*HTML::decode("&Ins&eacute;rer"));

   inserer -> addAction("&Sommets" , FParent ,
                        SLOT(callbackInsertVertex())) ;
   inserer -> addAction(*HTML::decode("&Ar&ecirc;tes") , FParent ,
                        SLOT(callbackInsertEdge())) ;
   inserer -> addAction("&Faces" , FParent ,
                        SLOT(callbackInsertFace())) ;

   // -- Fermer
   QMenu * fermer = operations -> addMenu("&Fermer") ;

   fermer -> addAction("&Intuitivement" , FParent ,
                       SLOT(callbackIntuitiveStopUp())) ;

   fermer -> addSeparator() ;

   fermer -> addAction(*HTML::decode("&Ar&ecirc;tes") , FParent ,
                       SLOT(callbackStopUp0())) ;
   fermer -> addAction("&Faces" , FParent , SLOT(callbackStopUp1())) ;
   fermer -> addAction("&Volumes" , FParent , SLOT(callbackStopUp2())) ;
   fermer -> addAction("&Composantes connexes" , FParent ,
                       SLOT(callbackStopUp3())) ;

   // -- Trianguler
   QMenu * trianguler = operations -> addMenu("&Trianguler") ;

   trianguler -> addAction(*HTML::decode("&Selon mode de s&eacute;lection") ,
                           FParent ,
                           SLOT(callbackContextTriangulate()), 
                           Qt::ALT + Qt::Key_T);

   trianguler -> addSeparator() ;

   trianguler -> addAction(*HTML::decode("&Ar&ecirc;tes") , FParent ,
                           SLOT(callbackTriangulateEdges())) ;
   trianguler -> addAction("&Faces" , FParent ,
                           SLOT(callbackTriangulateFaces())) ;
   trianguler -> addAction("&Volumes" , FParent ,
                           SLOT(callbackTriangulateVolumes())) ;
   trianguler -> addSeparator() ;

   trianguler -> addAction(*HTML::decode("Trianguler faces"
                                         " &g&eacute;om&eacute;triquement") ,
                                         FParent ,
                           SLOT(callbackTriangulateGeoFaces())) ;

   trianguler -> addAction(*HTML::decode("&Tourner l'ar&ecirc;te"
					 " s&eacute;lectionn&eacute;e") ,
			   FParent , SLOT(callbackSwapEdge())) ;

   // -- Quadranguler
   QMenu * quadranguler = operations -> addMenu("&Quadranguler") ;

   QMenu * faces = quadranguler -> addMenu("&Faces") ;
   faces -> addAction("&Avec insertion de Sommets" , FParent ,
                      SLOT(callbackQuadrangulateFaces()), Qt::ALT+Qt::Key_Q);
   faces -> addAction("&Sans insertion de Sommets" , FParent ,
                      SLOT(callbackBasicQuadrangulateFaces())) ;

   // -- Calculer Dual
   QMenu * calculerDual = operations -> addMenu("Calculer &dual") ;
   calculerDual -> addAction("2D" , FParent , SLOT(callbackDual2d())) ;
   calculerDual -> addAction("3D" , FParent , SLOT(callbackDual3d())) ;

   operations -> addSeparator() ;

   // -- Extruder
   QMenu * extruder = operations -> addMenu("&Extruder") ;

   ///Extruder / selon normale
   QMenu * normale = extruder -> addMenu("Selon &normale") ;
   normale -> addAction("&Intuitivement" , FParent ,
                        SLOT(callbackIntuitiveExtrudeByNormal()), Qt::Key_N);

   normale -> addSeparator() ;

   normale -> addAction("&Sommets" , FParent ,
                        SLOT(callbackExtrude0ByNormal())) ;
   normale -> addAction(*HTML::decode("&Ar&ecirc;tes") , FParent ,
                        SLOT(callbackExtrude1ByNormal())) ;
   normale -> addAction("&Faces" , FParent ,
                        SLOT(callbackExtrude2ByNormal())) ;

   ///Extruder / selon chemin
   QMenu * chemin = extruder -> addMenu("Selon &chemin") ;
   chemin -> addAction(*HTML::decode("&Ar&ecirc;tes") , FParent ,
                       SLOT(callbackExtrude1ByPath()) , Qt::SHIFT+Qt::Key_E);
   chemin -> addAction("&Faces" , FParent ,
                       SLOT(callbackExtrude2ByPath()) , Qt::Key_E) ;

   ///Extruder / selon axe
   QMenu * axe = extruder -> addMenu("Selon &axe") ;

   axe -> addAction(*HTML::decode("&Ar&ecirc;tes") , FParent ,
                    SLOT(callbackExtrude1ByRevolution()) ,
                    Qt::CTRL + Qt::SHIFT + Qt::Key_E) ;
   axe -> addAction("&Faces" , FParent ,
                    SLOT(callbackExtrude2ByRevolution()) ,
                    Qt::ALT + Qt::SHIFT + Qt::Key_E) ;

   ///Extruder / epaissir
   extruder -> addAction("Epaissir", FParent, SLOT(callbackThicken())) ;

   // -- Interpoler
#ifdef MODULE_MESH
   QMenu * interpoler = operations -> addMenu("&Interpoler") ;

   interpoler -> addAction(*HTML::decode("&Ar&ecirc;tes") , FParent ,
                           SLOT(callbackMeshEdge())) ;


   QMenu * carres = interpoler -> addMenu(*HTML::decode("&Carr&eacute;s"));
   carres -> addAction("0 -> 1" , FParent ,
                       SLOT(callbackMeshSquare01())) ;
   carres -> addAction("0 -> 2" , FParent ,
                       SLOT(callbackMeshSquare02())) ;
   carres -> addAction("1 -> 2" , FParent ,
                       SLOT(callbackMeshSquare12())) ;

   QMenu * cubes = interpoler -> addMenu("C&ubes") ;
   cubes -> addAction("0 -> 1" , FParent ,
                      SLOT(callbackMeshCube01())) ;
   cubes -> addAction("0 -> 2" , FParent ,
                      SLOT(callbackMeshCube02())) ;
   cubes -> addAction("0 -> 3" , FParent ,
                      SLOT(callbackMeshCube03())) ;
   cubes -> addAction("1 -> 2" , FParent ,
                      SLOT(callbackMeshCube12())) ;
   cubes -> addAction("1 -> 3" , FParent ,
                      SLOT(callbackMeshCube13())) ;
   cubes -> addAction("2 -> 3" , FParent ,
                      SLOT(callbackMeshCube23())) ;

   // -- Lisser
   QMenu * lisser = operations -> addMenu("&Lisser") ;

   lisser -> addAction(*HTML::decode("&Ar&ecirc;tes") , FParent ,
                       SLOT(callbackSmooth1())) ;
   lisser -> addAction(*HTML::decode("&Carr&eacute;s") , FParent ,
                       SLOT(callbackSmooth2())) ;
   lisser -> addAction("C&ubes" , FParent ,
                       SLOT(callbackSmooth3())) ;
#endif // MODULE_MESH

   // -- Arrondir
#ifdef MODULE_ROUNDING
   QMenu * arrondir = operations -> addMenu("&Arrondir") ;

   arrondir -> addAction("&Sommets" , FParent ,
                         SLOT(callbackRoundVertices())) ;
   arrondir -> addAction(*HTML::decode("Ar&ecirc;tes 2D") , FParent ,
                         SLOT(callbackRoundEdges2d())) ;
   arrondir -> addAction(*HTML::decode("Ar&ecirc;tes 3D") , FParent ,
                         SLOT(callbackRoundEdges3d())) ;

   arrondir -> addSeparator() ;

   arrondir -> addAction(*HTML::decode("Sommets &creus&eacute;s") , FParent ,
                         SLOT(callbackRoundVerticesDig()) , Qt::Key_A) ;

   arrondir -> addAction(*HTML::decode("Ar&ecirc;tes 3D creus&eacute;es") ,
                         FParent ,
                         SLOT(callbackRoundEdges3dDig()), Qt::SHIFT+Qt::Key_A);

   arrondir -> addSeparator() ;

   arrondir -> addAction(*HTML::decode("Ar&ecirc;tes 2D avec setback") ,
                         FParent ,
                         SLOT(callbackRoundEdges2dWithSetback())) ;
   arrondir -> addAction(*HTML::decode("Ar&ecirc;tes 3D avec setback") ,
                         FParent ,
                         SLOT(callbackRoundEdges3dWithSetback())) ;
   arrondir->addAction(*HTML::decode("Ar&ecirc;tes 3D creus&eacute;es "
                                     "avec setback") , FParent ,
                       SLOT(callbackRoundEdges3dDigWithSetback()) ,
                       Qt::CTRL + Qt::SHIFT + Qt::Key_A) ;

   arrondir -> addSeparator() ;

   arrondir -> addAction("Coefficients variables" , FParent ,
                         SLOT(chanf())) ;
#endif // MODULE_ROUNDING

   // -- Triangulation
#ifdef MODULE_TRIANGULATION
   QMenu * triangulation = operations -> addMenu("Trian&gulation") ;

   triangulation -> addAction("Triangulation de &Delaunay" , FParent ,
                              SLOT(callbackDelaunayTriangulation())) ;

   triangulation -> addSeparator() ;

   /*triangulation -> addAction ( "Trianguler faces &topologiquement" , 
   FParent ,SLOT (  callbackTriangulateTopoFaces ( ) ) ) ; */
   triangulation -> addAction("&Marquer faces non uniformes" , FParent ,
                              SLOT(callbackMarkNonUniformFaces())) ;
   triangulation -> addAction(*HTML::decode("&Optimiser positions des"
                              " ar&ecirc;tes") , FParent ,
                              SLOT(callbackOptimizeSelectedEdges())) ;
   triangulation -> addAction(*HTML::decode("triangulation de D&elaunay"
                              " optimis&eacute;e") , FParent ,
                              SLOT(callbackOptimizedDelaunayTriangulation()));
#endif // MODULE_TRIANGULATION

   // -- Co-raffiner
#ifdef MODULE_COREFINEMENT
   QMenu * coraffiner = operations -> addMenu("Co-ra&ffiner") ;

   coraffiner -> addAction("&Maillages 2D - balayage" , FParent ,
                           SLOT(callbackCorefine2dSegmentsSweeping()) ,
                           Qt::Key_AsciiCircum) ;
   coraffiner -> addAction("Maillages 2D - &propogation" , FParent ,
                           SLOT(callbackCorefine2dSegmentsPropagation()) ,
                           Qt::Key_Backslash) ;
   coraffiner->
   addAction(*HTML::decode("&Op&eacute;rations bool&eacute;ennes 2D"),
             FParent , SLOT(callbackBooleanOperations2d())) ;

   coraffiner -> addSeparator() ;

   coraffiner -> addAction("M&aillages 3D" , FParent ,
                           SLOT(callbackCorefine3dFaces())) ;
   coraffiner -> addAction("Fa&ces 3D" , FParent ,
                           SLOT(callbackCorefineMarked3dFaces())) ;
   coraffiner -> addAction(*HTML::decode("&Tous les maillages 3D"
                                         " s&eacute;lectionn&eacute;s") , 
                                         FParent ,
                           SLOT(callbackCorefineMarked3dMeshesWith())) ;
   coraffiner->
   addAction(*HTML::decode("Op&eacute;rations &bool&eacute;ennes 3D") ,
             FParent, SLOT(callbackBooleanOperations3d()), Qt::Key_At);
#endif // MODULE_COREFINEMENT

   //- Geologie
#ifdef MODULE_GEOLOGY
   geologie = new QMenu(*HTML::decode("&G&eacute;ologie"));
   geologie_act = addMenu(geologie);

   geologie -> addAction(*HTML::decode("&Cr&eacute;er une zone d'incertitude"),
                         FParent ,
                         SLOT(callbackUncertainZone())) ;
   geologie -> addAction(*HTML::decode("Cr&eacute;er les &intersections les"
                                       " plus proches") , FParent ,
                         SLOT(callbackCreateNearestIntersections())) ;
   geologie -> addAction("&Etendre bords" , FParent ,
                         SLOT(callbackExtendSelectedBorders())) ;
   geologie -> addAction(*HTML::decode("Etendre &bords jusqu'&agrave; une"
                                       " surface") , FParent ,
                         SLOT(callbackExtendSelectedBordersToSurface())) ;

   geologie -> addSeparator() ;

   geologie -> addAction(*HTML::decode("&Lisser la s&eacute;lection") ,
                         FParent ,
                         SLOT(callbackSmoothSelection())) ;
   geologie -> addAction(*HTML::decode("&Relacher la s&eacute;lection") ,
                         FParent ,
                         SLOT(callbackRelaxSelection())) ;
   geologie -> addAction("&Simplifier maillage" , FParent ,
                         SLOT(callbackSimplifyMesh())) ;
   geologie -> addAction("Rece&ntrer" , FParent ,
                         SLOT(callbackCenterSelection())) ;

   geologie->addAction(*HTML::decode("&Plaquer les sommets de la"
                                     " cr&eacute;ation courante sur les "
                                     "faces de la s&eacute;lection suivante"),
                       FParent , SLOT(callbackPlateVerticesOnFaces())) ;
   geologie -> addAction(*HTML::decode("Plaquer les sommets de la"
                                       " cr&eacute;ation courante sur les"
                                       " &ar&ecirc;tes de la"
                                       " s&eacute;lection suivante"), FParent,
                         SLOT(callbackPlateVerticesOnEdges())) ;
   geologie->addAction(*HTML::decode("Pla&quer les sommets de la"
                                     " cr&eacute;ation courante sur les somm"
                                     "ets de la s&eacute;lection suivante"),
                       FParent ,  SLOT(callbackPlateVerticesOnVertices())) ;

   geologie -> addSeparator() ;

   geologie -> addAction(*HTML::decode("S&eacute;lectionner to&us les"
                                       " macros-sommets") , FParent ,
                         SLOT(callbackSelectMacroVertices())) ;
   geologie -> addAction(*HTML::decode("S&eacute;lectionner toutes les"
                                       " macros-ar&ecirc;&tes") , FParent ,
                         SLOT(callbackSelectAllMacroEdges())) ;
   geologie -> addAction(*HTML::decode("S&eacute;lectionner les macros-ar"
                                       "&ecirc;tes d&eacute;si&gn&eacute;es"),
                         FParent , SLOT(callbackSelectMacroEdges())) ;
   geologie->addAction(*HTML::decode("S&eacute;lectionner les "
                                     "macr&os-faces d&eacute;sign&eacute;es"),
                       FParent , SLOT(callbackSelectMacroFaces())) ;
   geologie -> addAction(*HTML::decode("S&eacute;lectionner les bor&ds entre "
                                       "les brins s&eacute;lectionn&eacute;s"),
                         FParent,
                         SLOT(callbackSelectBordersBetweenSelectedDarts())) ;

   geologie -> addSeparator() ;
   geologie -> addAction("Simplifier le maillage" , FParent ,
                         SLOT(callbackMeshSimplification())) ;
   geologie -> addAction("Raffiner les trous" , FParent ,
                         SLOT(callbackHolesRefinement())) ;
   geologie -> addAction("Trianguler les trous" , FParent ,
                         SLOT(callbackTriangulateHoles())) ;
   geologie -> addAction("Boucher les trous" , FParent ,
                         SLOT(callbackFillHolesOnMeshes())) ;

   geologie -> addSeparator() ;
   geologie -> addAction(*HTML::decode("D&eacute;terminer les failles") ,
                         FParent , SLOT(callbackFailleDetection())) ;
   geologie -> addAction("Extraire les failles" , FParent ,
                         SLOT(callbackExtractDetectedFailles())) ;
   geologie -> addAction("Extension des failles" , FParent ,
                         SLOT(callbackExtensionFailleSurface())) ;

   geologie -> addSeparator() ;
   geologie -> addAction("I&mporter surface GR2D" , FParent ,
                         SLOT(callbackImportGr2d())) ;
   geologie -> addAction("Importer surface &XYZ" , FParent ,
                         SLOT(callbackImportXyz())) ;
   geologie -> addAction("Importer sur&face TS" , FParent ,
                         SLOT(callbackImportTs())) ;
   geologie -> addAction("Importer nuage de poin&ts" , FParent ,
                         SLOT(callbackImportCloud())) ;
   geologie -> addAction("Importer surface VRML" , FParent ,
                         SLOT(callbackImportVrml())) ;
   geologie -> addAction("Exporter surface TS" , FParent ,
                         SLOT(callbackExportTs())) ;
#endif // MODULE_GEOLOGY

   // Creation du menu Materialiser
   //------------------------------
   materialiser = new QMenu(*HTML::decode("&Mat&eacute;rialiser"));
   materialiser_act = addMenu(materialiser);

   materialiser -> addAction("&Barycentre" , FParent ,
                             SLOT(callbackMaterializeBarycenter())) ;
   materialiser -> addAction("&Axe" , FParent ,
                             SLOT(callbackMaterializeAxe())) ;
   materialiser -> addAction("&Plan" , FParent ,
                             SLOT(callbackMaterializePlane())) ;
   materialiser -> addAction("&Vecteur normal" , FParent ,
                             SLOT(callbackMaterializeNormalVector())) ;
   materialiser -> addAction("Plan &normal" , FParent ,
                             SLOT(callbackMaterializeOrthoPlane())) ;

   materialiser -> addSeparator() ;

   materialiser -> addAction(*HTML::decode("&Rep&egrave;re") , FParent ,
                             SLOT(callbackMaterializeReferential())) ;

   // Creation du menu Selection
   //---------------------------
   selection = new QMenu(*HTML::decode("&S&eacute;lection"));
   selection_act = addMenu(selection);

   selection -> addAction(*HTML::decode("R&eacute;initialiser") , FParent ,
                          SLOT(callbackReinitSelection()) ,
                          Qt::Key_Escape) ;
   selection -> addAction("Inverser" , FParent ,
                          SLOT(callbackNegateSelection()) ,
                          Qt::Key_Minus) ;
   selection -> addAction("Echanger" , FParent ,
                          SLOT(callbackSwapSelections()) ,
                          Qt::Key_Asterisk) ;

   selection -> addSeparator() ;

   selection -> addAction("Dupliquer" , FParent ,
                          SLOT(callbackDuplicateSelection()) ,
                          Qt::Key_Enter) ;

   selection -> addAction("Supprimer" , FParent ,
                          SLOT(callbackDeleteSelection()) ,
                          Qt::Key_Delete) ;

#ifdef MODULE_ARCHITECTURE
   supprimer1_act = selection -> addAction("Supprimer" , FParent ,
                                           SLOT(callbackDeleteSelection()) ,
                                                 Qt::Key_Delete) ;

   supprimer2_act = selection -> addAction("Supprimer" , FParent ,
                                           SLOT(callbackDeleteSelection())) ;

   supprimer2_act -> setVisible(false);
#endif // MODULE_ARCHITECTURE

   selection -> addSeparator() ;

   QMenu * etendre = selection -> addMenu("Etendre   ") ;
   QMenu * etendre1pas = etendre -> addMenu("d'1 pas   ") ;

   etendre1pas -> addAction("En avant" , FParent ,
                            SLOT(callbackGoForward1()) ,
                            Qt::Key_I) ;
   etendre1pas -> addAction(*HTML::decode("En arri&egrave;re") , FParent ,
                            SLOT(callbackGoBackward1()) ,
                            Qt::Key_K) ;
   etendre1pas -> addAction("A gauche" , FParent ,
                            SLOT(callbackGoLeft1()) ,
                            Qt::Key_J) ;
   etendre1pas -> addAction("A droite" , FParent ,
                            SLOT(callbackGoRight1()) ,
                            Qt::Key_L) ;

   QMenu * etendreRepet =
      etendre->addMenu(*HTML::decode("R&eacute;p&eacute;titivement"));

   etendreRepet -> addAction("En avant" , FParent ,
                             SLOT(callbackGoForward1Rep()) ,
                             Qt::CTRL + Qt::Key_I) ;
   etendreRepet -> addAction(*HTML::decode("En arri&egrave;re") , FParent ,
                             SLOT(callbackGoBackward1Rep()) ,
                             Qt::CTRL + Qt::Key_K) ;
   etendreRepet -> addAction("A gauche" , FParent ,
                             SLOT(callbackGoLeft1Rep()) ,
                             Qt::CTRL + Qt::Key_J) ;
   etendreRepet -> addAction("A droite" , FParent ,
                             SLOT(callbackGoRight1Rep()) ,
                             Qt::CTRL + Qt::Key_L) ;

   QMenu * reduire = selection->addMenu(*HTML::decode("R&eacute;duire"));
   QMenu * reduire1pas = reduire -> addMenu("d'un pas") ;

   reduire1pas -> addAction("En avant" , FParent ,
                            SLOT(callbackGoForward0()) ,
                            Qt::SHIFT + Qt::Key_I) ;
   reduire1pas -> addAction(*HTML::decode("En arri&egrave;re") , FParent ,
                            SLOT(callbackGoBackward0()) ,
                            Qt::SHIFT + Qt::Key_K) ;
   reduire1pas -> addAction("A gauche" , FParent ,
                            SLOT(callbackGoLeft0()) ,
                            Qt::SHIFT + Qt::Key_J) ;
   reduire1pas -> addAction("A droite" , FParent ,
                            SLOT(callbackGoRight0()) ,
                            Qt::SHIFT + Qt::Key_L) ;

   QMenu * reduireRepet =
      reduire->addMenu(*HTML::decode("R&eacute;p&eacute;titivement")) ;

   reduireRepet -> addAction("En avant" , FParent ,
                             SLOT(callbackGoForward0Rep()) ,
                             Qt::CTRL + Qt::SHIFT + Qt::Key_I) ;
   reduireRepet -> addAction(*HTML::decode("En arri&egrave;re") , FParent ,
                             SLOT(callbackGoBackward0Rep()) ,
                             Qt::CTRL + Qt::SHIFT + Qt::Key_K) ;
   reduireRepet -> addAction("A gauche" , FParent ,
                             SLOT(callbackGoLeft0Rep()) ,
                             Qt::CTRL + Qt::SHIFT + Qt::Key_J) ;
   reduireRepet -> addAction("A droite" , FParent ,
                             SLOT(callbackGoRight0Rep()) ,
                             Qt::CTRL + Qt::SHIFT + Qt::Key_L) ;

   selection -> addSeparator() ;

   QMenu * marquerInterieur =
      selection->addMenu(*HTML::decode("Marquer int&eacute;rieur"));

   marquerInterieur -> addAction("2D" , FParent ,
                                 SLOT(callbackMarkInterior2())) ;
   marquerInterieur -> addAction("3D" , FParent ,
                                 SLOT(callbackMarkInterior3())) ;

   selection -> addSeparator() ;

   QMenu * selectBords = selection->
                         addMenu(*HTML::decode("S&eacute;lectionner bords")) ;

   selectBords -> addAction("Intuitivement" , FParent ,
                            SLOT(callbackIntuitiveSelectBorders()) ,
                            Qt::ALT + Qt::Key_B) ;

   selectBords -> addSeparator() ;

   selectBords -> addAction("0-bord" , FParent ,
                            SLOT(callbackSelect0borders())) ;
   selectBords -> addAction("1-bord" , FParent ,
                            SLOT(callbackSelect1borders())) ;
   selectBords -> addAction("2-bords" , FParent ,
                            SLOT(callbackSelect2borders())) ;
   selectBords -> addAction("3-bords" , FParent ,
                            SLOT(callbackSelect3borders())) ;

   selection->addAction(*HTML::decode("S&eacute;lectionner faces non planes"),
                        FParent ,
                        SLOT(callbackSelectNonPlanarFaces()));

   selection -> addSeparator();

   selection -> addAction("Des cellules incidentes" , FParent ,
                          SLOT(callbackSelectIncidents()) ,
                          Qt::Key_S) ;

   QMenu * avancer = selection -> addMenu("Avancer") ;

   avancer -> addAction("Par Alpha0" , FParent ,
                        SLOT(callbackGoAlpha0()), Qt::ALT + Qt::Key_0) ;
   avancer -> addAction("Par Alpha1" , FParent ,
                        SLOT(callbackGoAlpha1()), Qt::ALT + Qt::Key_1) ;
   avancer -> addAction("Par Alpha2" , FParent ,
                        SLOT(callbackGoAlpha2()), Qt::ALT + Qt::Key_2) ;
   avancer -> addAction("Par Alpha3" , FParent ,
                        SLOT(callbackGoAlpha3()), Qt::ALT + Qt::Key_3) ;

   // Creation du menu Vues
   //----------------------
   vues = new QMenu("&Vues");
   vues_act = addMenu(vues);

   QMenu * arranger = vues -> addMenu("&Organiser en") ;
   arranger -> addAction("&Cascade" , FParent ,
                         SLOT(cascade())) ;
   arranger -> addAction("&Mosaique" ,  FParent ,
                         SLOT(tile()) , Qt::Key_M) ;

   vues -> addAction(pv2 , "&Supprimer" , FParent ,
                     SLOT(deleteView()) , Qt::Key_Backspace) ;

   QMenu * basculer = vues -> addMenu("&Basculer vers") ;
   basculer -> addAction("&Une vue 3D" , FParent ,
                         SLOT(basculeView3D()) , Qt::Key_0);
   basculer -> addAction("Une vue &XY" , FParent ,
                         SLOT(basculeViewXY()) , Qt::Key_1);
   basculer -> addAction("Une vue &YZ" , FParent ,
                         SLOT(basculeViewYZ()) , Qt::Key_2);
   basculer -> addAction("Une vue &ZX" , FParent ,
                         SLOT(basculeViewZX()) , Qt::Key_3);
   basculer -> addSeparator() ;
   basculer -> addAction("Une multi-vues" , FParent ,
                         SLOT(basculeViewMulti()) , Qt::Key_4);

   vues -> addSeparator() ;

   QMenu * ajouter = vues -> addMenu("&Ajouter") ;
   ajouter -> addAction(pv1 , "&Vue 3D" , FParent ,
                        SLOT(addView3D()) , Qt::CTRL + Qt::Key_0);
   ajouter -> addAction("Vue &XY" , FParent ,
                        SLOT(addViewXY()) , Qt::CTRL + Qt::Key_1) ;
   ajouter -> addAction("Vue &YZ" , FParent ,
                        SLOT(addViewYZ()) , Qt::CTRL + Qt::Key_2) ;
   ajouter -> addAction("Vue &ZX" , FParent ,
                        SLOT(addViewZX()) , Qt::CTRL + Qt::Key_3) ;
   ajouter -> addSeparator() ;
   ajouter -> addAction("Multi vues" , FParent ,
                        SLOT(addViewMulti()) , Qt::CTRL + Qt::Key_4) ;

   vues -> addSeparator() ;

   QMenu * grouper = vues -> addMenu("&Grouper") ;
   grouper -> addAction(pv3 , "&Toutes les vues avec la vue courante" ,
                        FParent , SLOT(OperationGroupAllGeneral()) ,
                        Qt::Key_G) ;

   grouper -> addSeparator() ;

   grouper -> addAction(*HTML::decode("&Les pr&eacute;compiles de toutes"
                                      " les vues"), FParent ,
                        SLOT(OperationGroupAllPrecomp())) ;

   grouper -> addAction("La position de l'&oeil de toutes les vues" ,
                        FParent , SLOT(OperationGroupAllEyePos())) ;

   grouper -> addAction(*HTML::decode("Le point &regard&eacute; de toutes"
                                      " les vues") ,
                        FParent , SLOT(OperationGroupAllAimedPos())) ;

   grouper -> addAction(*HTML::decode("Les param&egrave;tres d'&affichages"
                                      " de toutes les vues") ,
                        FParent , SLOT(OperationGroupAllDrawing())) ;

   grouper -> addSeparator() ;

   grouper->addAction(*HTML::decode("La vue courante et la derni&egrave;re"
                                    " vue double-cliqu&eacute;e"),
                      FParent , SLOT(OperationGroupGeneral()) ,
                      Qt::ALT + Qt::Key_G) ;

   grouper -> addSeparator() ;

   grouper->addAction(*HTML::decode("&Les pr&eacute;compiles de ces 2 vues"),
                      FParent, SLOT(OperationGroupPrecomp()));

   grouper -> addAction("La &position de l'oeil de ces 2 vues" ,
                        FParent , SLOT(OperationGroupEyePos()));

   grouper->addAction(*HTML::decode("Le point &regard&eacute; de ces 2 vues"),
                      FParent, SLOT(OperationGroupAimedPos()));

   grouper->addAction(*HTML::decode("Les param&egrave;tres d'&affichages "
                                    "de ces 2 vues") ,
                      FParent , SLOT(OperationGroupDrawing()));

   QMenu * degrouper = vues->addMenu(*HTML::decode("&D&eacute;grouper"));

   degrouper -> addAction(pv4 , "&Toutes les vues" , FParent ,
                          SLOT(OperationUngroupAllGeneral()) ,
                          Qt::SHIFT + Qt::Key_G) ;

   degrouper -> addSeparator() ;

   degrouper->addAction(*HTML::decode("&Les pr&eacute;compiles de toutes les"
                                      " vues"), FParent ,
                                      SLOT(OperationUngroupAllPrecompiles()));

   degrouper -> addAction("La &position de l'oeil de toutes les vues" ,
                          FParent , SLOT(OperationUngroupAllEyePos()));

   degrouper -> addAction(*HTML::decode("Le point &regard&eacute; de "
                                        "toutes les vues"), FParent ,
                                        SLOT(OperationUngroupAllAimedPos()));

   degrouper->addAction(*HTML::decode("Les param&egrave;tres d'&affichages de"
                                      " toutes les vues"), FParent ,
                                      SLOT(OperationUngroupAllDrawing()));

   degrouper -> addSeparator() ;

   degrouper -> addAction("La vue &courante des autres vues" ,
                          FParent , SLOT(OperationUngroupGeneral()) ,
                          *new QKeySequence("SHIFT+ALT+Key_G")) ;

   degrouper -> addSeparator() ;

   degrouper->addAction(*HTML::decode("Les pr&eacute;compiles de &cette vue") ,
                        FParent, SLOT(OperationUngroupPrecompiles()));

   degrouper -> addAction("La position de l'oeil de cette &vue" ,
                          FParent , SLOT(OperationUngroupEyePos()));

   degrouper->addAction(*HTML::decode("Le point regar&d&eacute; de cette vue") ,
                        FParent, SLOT(OperationUngroupAimedPos()));

   degrouper->addAction(*HTML::decode("Les param&egrave;tres d'affichage "
                                      "de cette v&ue"), FParent ,
                                      SLOT(OperationUngroupDrawing()));

   QMenu * pointDeMire = vues->addMenu("&Point de mire") ;

   pointDeMire -> addAction("&Origine" , FParent ,
                            SLOT(mireOrigine()) , Qt::Key_O) ;
   pointDeMire->addAction(*HTML::decode("&Sommet incident &agrave; \"last\""),
                          FParent, SLOT(mireSommet()), Qt::Key_Exclam);

   pointDeMire -> addSeparator();

#ifdef MODULE_ARCHITECTURE
   pointDeMire -> addAction(*HTML::decode("Brin s&eacute;lectionn&eacute;"
                                          " pr&eacute;c&eacute;dent"),
                            FParent, SLOT(callbackFocusPreviousSelection()));

   pointDeMire->
   addAction(*HTML::decode("Brin s&eacute;lectionn&eacute; suivant") ,
             FParent , SLOT(callbackFocusNextSelection()));

   pointDeMire -> addSeparator();
#endif // MODULE_ARCHITECTURE

   QMenu* centre = pointDeMire ->
                   addMenu(*HTML::decode("&Centre bo&icirc;te englobante de"));

   centre -> addAction(*HTML::decode("&Face incidente &agrave; \"last\""),
                       FParent ,
                       SLOT(mireFace())) ;
   centre -> addAction(*HTML::decode("&Volume incident &agrave; \"last\"") ,
                       FParent ,
                       SLOT(mireVolume())) ;
   centre->addAction(*HTML::decode("&Composante connexe incidente &agrave; "
                                   "\"last\"") , FParent ,
                                   SLOT(mireCompConnexe())) ;
   centre -> addSeparator() ;
   centre -> addAction(*HTML::decode("Tous les brins marqu&eacute;s") ,
                       FParent ,
                       SLOT(mireBrinsMarques()) , Qt::SHIFT + Qt::Key_O) ;
   centre -> addAction("Tous les brins" , FParent ,
                       SLOT(mireBrins()) , Qt::CTRL + Qt::SHIFT + Qt::Key_O) ;

   //- Spamod
#ifdef MODULE_SPAMOD
   QMenu * visualisation = vues -> addMenu("&Visualisation   ") ;

   visualisation -> addAction(*HTML::decode("&Vue Eclat&eacute;e") , FParent,
                              SLOT(callbackBackToBurstView())) ;
   visualisation -> addSeparator() ;
   visualisation -> addAction("Vue &Topologique" , FParent ,
                              SLOT(callbackTopologicalView())) ;
   visualisation -> addAction("Vue &Euclidienne" , FParent ,
                              SLOT(callbackEuclidianView())) ;
   visualisation -> addAction("Vue &Analytique" , FParent ,
                              SLOT(callbackAnalyticView())) ;
   visualisation -> addAction("Vue &Voxels" , FParent ,
                              SLOT(callbackVoxelView())) ;
   visualisation -> addAction("Vue &K2" , FParent ,
                              SLOT(callbackK2View())) ;
   visualisation -> addSeparator() ;
   visualisation -> addAction("&Jongler entre les vues" , FParent ,
                              SLOT(callbackSpamodSwitchToNextView())) ;
#endif // MODULE_SPAMOD

   /**************************************************************************/
   /**************************************************************************/
   /**************************************************************************/
#ifdef MODULE_ARCHITECTURE
   architecture = new QMenu("&Architecture");
   architecture_act = addMenu(architecture);

   architecture -> addAction("Import d'un objet Nff 3D", FParent,
                             SLOT(callbackImportNff3D()));

   QMenu * orientation = architecture -> addMenu("Orientation   ") ;

   orientation -> addAction("Affichage", FParent,
                            SLOT(callbackAfficheOrientation()));
   orientation -> addAction("Test", FParent,
                            SLOT(callbackTestOrientation()));
   orientation-> addSeparator() ;
   orientation -> addAction(*HTML::decode("D&eacute;finir / brin"), FParent,
                            SLOT(callbackDefineOrientation()));
   orientation -> addAction("Inverser", FParent,
                            SLOT(callbackReverseOrientation()));
   orientation -> addAction("Inverser sur CC", FParent,
                            SLOT(callbackReverseOrientationCC()));
   architecture -> addSeparator() ;
   
   /*********************************************************************/
   architecture-> 
         addAction(*HTML::decode("Test de coh&eacute;rence architecture"),
                    FParent, SLOT(callbackFonctionTest()));
   architecture -> addSeparator() ;

   architecture-> addAction("Ajout de la topologie", FParent,
                            SLOT(callbackArchiTopo()));
   architecture -> addSeparator() ;
   
   /*********************************************************************/
   QMenu * areteFictive = architecture ->
                          addMenu(*HTML::decode("Ar&ecirc;te Fictive   ")) ;

   areteFictive -> addAction(*HTML::decode("Cr&eacute;ation"), FParent,
                             SLOT(callbackAreteFictive()));
   areteFictive -> addAction(*HTML::decode("D&eacute;tection"), FParent,
                             SLOT(callbackDetecteFictive()));
   areteFictive -> addAction("Affectation", FParent,
                             SLOT(callbackAffectFictive()));
   areteFictive -> addAction(*HTML::decode("S&eacute;lection"), FParent,
                             SLOT(callbackSelectionFictive()));

   architecture -> addSeparator() ;
   
   /*********************************************************************/
   QMenu * ameublement = architecture -> addMenu("Ameublement   ") ;
   ameublement -> addAction("Save", FParent,
                            SLOT(callbackSaveAmeublement()));
   ameublement -> addAction("Load", FParent,
                            SLOT(callbackLoadAmeublement()));
   ameublement-> addSeparator() ;
   ameublement -> addAction("Plan 2D", FParent,
                            SLOT(callbackAmeublement2D()));
   ameublement -> addAction("Selection 2D", FParent,
                            SLOT(callbackAmeublementSel2D()));
   ameublement -> addAction("Plan 2D convexe", FParent,
                            SLOT(callbackAmeublementConvexe2D()));
   ameublement-> addSeparator() ;
   ameublement -> addAction("Plan 3D", FParent,
                            SLOT(callbackAmeublement3D()));
   ameublement -> addAction("Selection 3D", FParent,
                            SLOT(callbackAmeublementSel3D()));
   ameublement -> addAction("Plan 3D convexe", FParent,
                            SLOT(callbackAmeublementConvexe3D()));
   
   /*********************************************************************/
   QMenu * convexification = architecture -> addMenu("Convexification   ") ;

   convexification -> addAction("Face E1", FParent,
                                SLOT(callbackConvexificationE1()));
   convexification -> addAction("Face E2", FParent,
                                SLOT(callbackConvexificationE2()));
   convexification -> addAction("Face E3", FParent,
                                SLOT(callbackConvexificationE3()));
   convexification-> addSeparator() ;
   convexification -> addAction("Murs", FParent,
                                SLOT(callbackConvexificationMur()));
   convexification -> addAction(*str_piece, FParent,
                                SLOT(callbackConvexificationPiece()));

   /*********************************************************************/
   QMenu * triangulationArchi = architecture -> addMenu("Triangulation   ") ;

   triangulationArchi -> addAction("Face", FParent,
                                   SLOT(callbackTriangulation()));
   triangulationArchi -> addAction("Face rapide", FParent,
                                   SLOT(callbackTriangulationRapide()));
   triangulationArchi -> addAction("Delaunay", FParent,
                                   SLOT(callbackDelaunayTriangulation()));
   triangulationArchi -> addAction("Murs", FParent,
                                   SLOT(callbackTriangulationMur()));
   triangulationArchi -> addAction(*str_piece, FParent,
                                   SLOT(callbackTriangulationPiece()));
   architecture -> addSeparator() ;
   
   /*********************************************************************/
   QMenu * operationArchi = 
         architecture -> addMenu(*HTML::decode("Op&eacute;ration"));

   operationArchi -> addAction("Couture", FParent,
                               SLOT(callbackOperationCouture()));

   /*-------------------------------------------------------------------*/
   QMenu * operationSuppr = operationArchi -> addMenu("Suppression") ;

   operationSuppr->addAction(*HTML::decode("Suppression control&eacute;e"),
                             FParent,
                             SLOT(callbackOperationSuppression()));
   operationSuppr ->
   addAction(*HTML::decode("Suppression ar&ecirc;te (no control)"),
             FParent, SLOT(callbackOperationSuppressionSel2D()));
   operationSuppr -> addAction("Suppression face (no control)", FParent,
                               SLOT(callbackOperationSuppressionSel3D()));
   operationSuppr -> addSeparator() ;

   QMenu * operationSuppr2D = operationSuppr -> addMenu("Suppression 2D") ;

   operationSuppr2D -> addAction("Suppression decoupe", FParent,
                                 SLOT(callbackOperationSuppressionCut2D()));
   operationSuppr2D->addAction("Suppression doublons", FParent,
                               SLOT(callbackOperationSuppressionDoublons2D()));
   operationSuppr2D -> addAction("Suppression Transparent", FParent,
                                 SLOT(callbackOperationSuppressionTransp2D()));

   QMenu * operationSuppr3D = operationSuppr -> addMenu("Suppression 3D") ;

   operationSuppr3D -> addAction("Suppression decoupe", FParent,
                                 SLOT(callbackOperationSuppressionCut3D()));
   operationSuppr3D->addAction("Suppression doublons", FParent,
                               SLOT(callbackOperationSuppressionDoublons3D()));
   operationSuppr3D -> addAction("Suppression Transparent", FParent,
                                 SLOT(callbackOperationSuppressionTransp3D()));

   /*-------------------------------------------------------------------*/
   QMenu * operationPorte = operationArchi -> addMenu("Porte") ;

   operationPorte -> addAction(*HTML::decode("Cr&eacute;ation"), FParent,
                               SLOT(callbackOperationPorte()));
   operationPorte -> addAction("Epaissir", FParent,
                               SLOT(callbackEpaissirPorte()));
   operationPorte -> addAction("Insertion", FParent,
                               SLOT(callbackInsertPorte2D()));
   operationPorte -> addSeparator() ;
   operationPorte -> addAction("Insertion en 3D", FParent,
                               SLOT(callbackInsertPorte3D()));

   /*-------------------------------------------------------------------*/
   QMenu* operationFenetre = operationArchi->
         addMenu(*HTML::decode("Fen&ecirc;tre")) ;
   operationFenetre -> addAction(*HTML::decode("Creacute;ation"), FParent,
                                 SLOT(callbackOperationFenetre()));
   operationFenetre -> addAction("Epaissir", FParent,
                                 SLOT(callbackEpaissirFenetre()));
   operationFenetre -> addAction("Insertion", FParent,
                                 SLOT(callbackInsertFenetre2D()));
   operationFenetre -> addSeparator() ;
   operationFenetre -> addAction("Insertion 3D", FParent,
                                 SLOT(callbackInsertFenetre3D()));

   QMenu * operationMur = operationArchi -> addMenu("Mur") ;

   operationMur -> addAction(*HTML::decode("Cr&eacute;ation d'une ligne mur"),
                              FParent, SLOT(callbackOperationMur()));
   operationMur->addAction(*HTML::decode("Cr&eacute;ation d'un mur epais 2D"),
                            FParent, SLOT(callbackCreateMurEpais2D()));
   operationMur -> addAction("Epaissir un mur 2D", FParent,
                             SLOT(callbackEpaissirMur()));
   operationMur -> addSeparator() ;
   operationMur->addAction(*HTML::decode("Cr&eacute;ation d'un mur epais 3D"),
                            FParent,SLOT(callbackCreateMurEpais3D()));

   operationArchi -> addSeparator() ;

   operationArchi ->
   addAction(*HTML::decode("Donn&eacute;e des brins selectionn&eacute;s"),
             FParent, SLOT(callbackOperationAffichage()));

   /*********************************************************************/
   /*********************************************************************/
   QMenu * operationSimplificationArchi = architecture ->
                                          addMenu("Simplification") ;

   operationSimplificationArchi -> addAction("Suppression doublons", FParent,
         SLOT(callbackSuppressionDoublons()));

   operationSimplificationArchi -> addSeparator() ;
   operationSimplificationArchi -> addAction("Suppression sommets deg 2",
         FParent,
         SLOT(callbackSuppressionSommetDeg2()));
   operationSimplificationArchi -> addAction("Suppression Aretes deg 2",
         FParent,
         SLOT(callbackSuppressionAreteDeg2()));
   operationSimplificationArchi -> addAction("Suppression Faces deg 2",
         FParent,
         SLOT(callbackSuppressionFaceDeg2()));
   operationSimplificationArchi -> addSeparator() ;
   operationSimplificationArchi -> addAction("Simplification Totale",
         FParent,
         SLOT(callbackSimplification()));

   /*********************************************************************/
   architecture -> addSeparator() ;

   QMenu * operationEditionArchi = architecture -> addMenu("Edition   ") ;

   operationEditionArchi -> addAction("Suppr Mur 2D", FParent,
                                      SLOT(callbackSuppressionFaceMur2D()));

   operationEditionArchi ->
   addAction("Suppr ouverture -> Mur 2D", FParent,
             SLOT(callbackSuppressionFaceOuvertureMur2D()));
   operationEditionArchi ->
   addAction("Suppr ouverture -> piece 2D", FParent,
             SLOT(callbackSuppressionFaceOuverturePiece2D()));

   operationEditionArchi -> addSeparator() ;
   operationEditionArchi -> addAction("Suppr Mur 3D", FParent,
                                      SLOT(callbackSuppressionVolumeMur3D()));

   operationEditionArchi -> addSeparator() ;
   operationEditionArchi -> addAction("Translation 2D coef 0.5", FParent,
                                      SLOT(callbackTranslation2D_1()));
   operationEditionArchi -> addAction("Translation 2D coef 1", FParent,
                                      SLOT(callbackTranslation2D_2()));
   operationEditionArchi -> addSeparator() ;
   operationEditionArchi -> addAction("Translation 3D coef 0.5", FParent,
                                      SLOT(callbackTranslation3D_1()));
   operationEditionArchi -> addAction("Translation 3D coef 1", FParent,
                                      SLOT(callbackTranslation3D_2()));
   operationEditionArchi -> addSeparator() ;
   operationEditionArchi -> addAction("Toit droit", FParent,
                                      SLOT(callbackConstructionToit()));
   operationEditionArchi -> addAction("Toit Biseau", FParent,
                                      SLOT(callbackConstructionToitBiso()));
   operationEditionArchi -> addAction("Toit Arrondi", FParent,
                                      SLOT(callbackConstructionToitArrondi()));
   operationEditionArchi -> addSeparator() ;
   operationEditionArchi -> addAction("Biseauter toit", FParent,
                                      SLOT(callbackBisoToit()));
   operationEditionArchi -> addAction("Arrondir toit", FParent,
                                      SLOT(callbackArrondirToit()));

   operationEditionArchi -> addSeparator() ;
   operationEditionArchi -> addAction("Arrondi 3D", FParent,
                                      SLOT(callbackArrondi3D()));

   QMenu * extrusionArchi = architecture -> addMenu("Extrusion") ;

   extrusionArchi -> addAction("Extrusion des Murs", FParent,
                               SLOT(callbackExtrudeMur()));
   extrusionArchi -> addAction("Extrusion des Portes", FParent,
                               SLOT(callbackExtrudePorte()));
   extrusionArchi -> addAction(*HTML::decode("Extrusion des Fen&ecirc;tre"),
                                FParent, SLOT(callbackExtrudeFenetre()));

   QMenu * extrusionEscalier = architecture->addMenu("Extrusion escalier");

   extrusionEscalier -> addAction("Extrusion escalier Droit", FParent,
                                  SLOT(callbackExtrudeEscalierDroit()));
   extrusionEscalier ->
   addAction(*HTML::decode("Extrusion escalier collima&ccedil;on"),
             FParent, SLOT(callbackExtrudeEscalierCollimacon()));

   extrusionArchi -> addAction("Extrusion du sol", FParent,
                               SLOT(callbackExtrudeSol()));
   extrusionArchi -> addAction("Extrusion du plafond", FParent,
                               SLOT(callbackExtrudePlafond()));
   extrusionArchi -> addSeparator() ;
   extrusionArchi->addAction(*HTML::decode("Extrusion de la s&eacute;lection"),
                             FParent, SLOT(callbackExtrudeSelect()));
   extrusionArchi -> addAction("Extrusion totale", FParent,
                               SLOT(callbackExtrudeAll()));

   architecture -> addSeparator() ;

   /*********************************************************************/
   QMenu * semantique = architecture ->
                        addMenu(*HTML::decode("S&eacute;mantique   ")) ;

   semantique -> addAction(*HTML::decode("Affiche s&eacute;mantique"), 
                            FParent, SLOT(callbackAfficheSemantique()));
   semantique -> addAction(*HTML::decode("Brin sans s&eacute;mantique"), 
                            FParent, SLOT(callbackAfficheSansSem()));

   semantique -> addSeparator() ;

   QMenu * selectionSemantique = 
         semantique -> addMenu(*HTML::decode("S&eacute;lection des objets"));

   selectionSemantique -> addAction("Murs", FParent,
                                    SLOT(callbackSelectionMur()));
   selectionSemantique -> addAction(*str_piece, FParent,
                                    SLOT(callbackSelectionPiece()));
   selectionSemantique -> addAction("Portes", FParent,
                                    SLOT(callbackSelectionPorte()));
   selectionSemantique -> addAction(*str_fenetre, FParent,
                                    SLOT(callbackSelectionFenetre()));
   selectionSemantique -> addAction("Sol", FParent,
                                    SLOT(callbackSelectionSol()));
   selectionSemantique -> addAction("Plafond", FParent,
                                    SLOT(callbackSelectionPlafond()));
   selectionSemantique -> addAction("Escalier", FParent,
                                    SLOT(callbackSelectionEscalier()));
   selectionSemantique -> addAction("Contour d'escaliers", FParent,
                                    SLOT(callbackSelectionContourEscalier()));
   selectionSemantique -> addAction("Facade", FParent,
                                    SLOT(callbackSelectionContour()));

   QMenu * affectSemantique = semantique ->
                              addMenu("Affectation des objets   ") ;

   affectSemantique -> addAction("Murs", FParent,
                                 SLOT(callbackAffectMur()));
   affectSemantique -> addAction(*str_piece, FParent,
                                 SLOT(callbackAffectPiece()));
   affectSemantique -> addAction("Portes", FParent,
                                 SLOT(callbackAffectPorte()));
   affectSemantique -> addAction(*str_fenetre, FParent,
                                 SLOT(callbackAffectFenetre()));
   affectSemantique -> addAction("Escalier", FParent,
                                 SLOT(callbackAffectEscalier()));
   affectSemantique -> addAction("Contour d'escaliers", FParent,
                                 SLOT(callbackAffectContourEscalier()));
   affectSemantique -> addAction("Facade", FParent,
                                 SLOT(callbackAffectContour()));

   semantique -> addSeparator() ;

//QMenu * semAuto = architecture -> addMenu ( "Sem Auto   " ) ;

   QMenu * findSem = semantique ->
                     addMenu("Recherche sur un type d'objets   ") ;

   findSem -> addAction("Murs", FParent,
                        SLOT(callbackFindMur()));
   findSem -> addAction(*str_piece, FParent,
                        SLOT(callbackFindPiece()));
   findSem -> addAction("Portes", FParent,
                        SLOT(callbackFindPorte()));
   findSem -> addAction(*str_fenetre, FParent,
                        SLOT(callbackFindFenetre()));
   findSem -> addAction("Escalier", FParent,
                        SLOT(callbackFindEscalier()));
   findSem -> addAction("Facade", FParent,
                        SLOT(callbackFindFacade()));

   QMenu * deduceSem = semantique ->addMenu(*HTML::decode("Deduction de "
         "semantique &agrave; partir de"));

   deduceSem -> addAction(*HTML::decode("Porte / Fen&ecirc;tre / facade"),
                          FParent,
                          SLOT(callbackFindSemPorteFenetre()));
   semantique->addSeparator();
   semantique->addAction(*HTML::decode("Effacer toute la s&eacute;mantique"),
                         FParent,
                         SLOT(callbackEraseSem()));
   semantique->addAction(*HTML::decode("Effacer la s&eacute;mantique de la"
                                       " s&eacute;lection"), FParent,
                                       SLOT(callbackEraseSemSel()));

   /*********************************************************************/
   QMenu * heuristique = architecture -> addMenu("Heuristique") ;

   heuristique -> addAction("Calcul de surface", FParent,
                            SLOT(callbackCalculSurface()));
   heuristique -> addAction(*HTML::decode("Calcul de perim&egrave;tre"),
                            FParent, SLOT(callbackCalculPerimetre()));
   heuristique -> addSeparator() ;
   heuristique ->
   addAction(*HTML::decode("Calcul de surface des pi&egrave;ces"),
             FParent, SLOT(callbackCalculSurfacePiece()));
   heuristique ->
   addAction(*HTML::decode("Calcul de perim&egrave;tre des pi&egrave;ce"),
             FParent, SLOT(callbackCalculPerimetrePiece()));
   heuristique -> addSeparator() ;
   heuristique -> addAction("Calcul de surface des murs", FParent,
                            SLOT(callbackCalculSurfaceMur()));
   heuristique->addAction(*HTML::decode("Calcul de perim&egrave;tre des murs"),
                          FParent, SLOT(callbackCalculPerimetreMur()));
   heuristique -> addSeparator() ;
   heuristique -> addAction("Calcul de rapport S/P mur", FParent,
                            SLOT(callbackCalculRapportMur()));
   heuristique->addAction(*HTML::decode("Calcul de rapport S/P pi&egrave;ce"),
                          FParent, SLOT(callbackCalculRapportPiece()));
   architecture -> addSeparator() ;

   /*********************************************************************/
   QMenu * renduArchi = architecture -> addMenu("Rendu") ;

   renduArchi -> addAction(*HTML::decode("Position cam&eacute;ra"), FParent,
                           SLOT(callbackPositionCamera()));
   renduArchi -> addAction("rayTracing V1 (lilian)", FParent,
                           SLOT(callbackCheminOrigine()));

   renduArchi -> addAction("rayTracing V2 (DI)", FParent,
                           SLOT(callbackRayTracingDI()));

   renduArchi -> addSeparator() ;
   renduArchi -> addAction("Build structA(DI)", FParent,
                           SLOT(callbackBuildStructADI()));
   renduArchi -> addSeparator() ;
   renduArchi -> addAction("Save Ameublement XML", FParent,
                           SLOT(callbackSaveAmeublementRendu()));

   renduArchi->addAction(*HTML::decode("Exporter &agrave; l'&eacute;cran"),
                         FParent, SLOT(callbackExportRenduSortie()));

   architecture -> addSeparator() ;

   /*********************************************************************/
   QMenu * selectionArchi = architecture ->
                            addMenu(*HTML::decode("S&eacute;lection")) ;

   selectionArchi -> addAction("Alpha 1 libre", FParent,
                               SLOT(callbackSelectionAlpha1()));
   selectionArchi -> addAction("Alpha 2 libre", FParent,
                               SLOT(callbackSelectionAlpha2()));
   selectionArchi -> addAction("Alpha 3 libre", FParent,
                               SLOT(callbackSelectionAlpha3()));

   selectionArchi -> addSeparator() ;

   selectionArchi->addAction(*HTML::decode("Ar&ecirc;tes pendantes"), FParent,
                             SLOT(callbackSelectionPendante()));
   selectionArchi-> 
         addAction(*HTML::decode("Ar&ecirc;tes non-orient&eacute;es"), FParent,
                               SLOT(callbackSelectionOrientation()));
   selectionArchi -> addSeparator() ;
   selectionArchi -> addAction("Sem face mauvaise", FParent,
                               SLOT(callbackSelectionFaceSemFaux()));
   selectionArchi -> addAction("Sem volume mauvaise", FParent,
                               SLOT(callbackSelectionVolumeSemFaux()));
   selectionArchi ->
   addAction(*HTML::decode("Incoh&eacute;rence voisinage"), FParent,
             SLOT(callbackSelectionIncoherenceVoisinage()));

   selectionArchi -> addSeparator() ;

   selectionArchi -> addAction(*HTML::decode("Ar&ecirc;tes confondues"), 
                                FParent, SLOT(callbackSelectionConfondu()));
   selectionArchi -> addAction("Intersection", FParent,
                               SLOT(callbackSelectionIntersection()));
   selectionArchi -> addSeparator() ;
   selectionArchi -> addAction("Faces non-convexes", FParent,
                               SLOT(callbackSelectionNonConvexe()));
   selectionArchi -> addAction("Faces convexes ?", FParent,
                               SLOT(callbackIsConvexe()));
   selectionArchi -> addSeparator() ;

   QMenu* selectionTransp =
      selectionArchi->addMenu(*HTML::decode("El&eacute;ments transparents"));
   
   selectionTransp -> addAction(*HTML::decode("S&eacute;lection transparent"),
                                FParent,
                                SLOT(callbackSelectionTransparent()));
   selectionTransp -> addAction(*HTML::decode("D&eacute;tection transparent"),
                                FParent,
                                SLOT(callbackIsTransparent()));
   selectionTransp ->
   addAction(*HTML::decode("Affectation transparent s&eacute;lection"),
             FParent, SLOT(callbackAffectTranspSel()));
   selectionTransp -> addAction("Affectation transparent 2D",
                                FParent,
                                SLOT(callbackAffectTransp2D()));
   selectionTransp -> addAction("Affectation transparent 3D",
                                FParent,
                                SLOT(callbackAffectTransp3D()));

   selectionArchi -> addSeparator() ;
   
   /*------------------------------------------------------------------------*/
   QMenu * selectionVolume = 
         selectionArchi->addMenu(*HTML::decode("S&eacute;lection de volume"));

   selectionVolume -> addAction("Mur", FParent,
                                SLOT(callbackSelectionMur()));
   selectionVolume -> addAction(*HTML::decode("Pi&egrave;ce"), FParent,
                                SLOT(callbackSelectionPiece()));
   selectionVolume -> addAction("Porte", FParent,
                                SLOT(callbackSelectionPorte()));
   selectionVolume -> addAction(*HTML::decode("Fen&ecirc;tre"), FParent,
                                SLOT(callbackSelectionFenetre()));
   selectionVolume -> addAction("Sol", FParent,
                                SLOT(callbackSelectionSol()));
   selectionVolume -> addAction("Plafond", FParent,
                                SLOT(callbackSelectionPlafond()));
   selectionVolume -> addAction("Escalier", FParent,
                                SLOT(callbackSelectionEscalier()));
   selectionVolume -> addAction("Contour escalier", FParent,
                                SLOT(callbackSelectionContourEscalier()));
   selectionVolume -> addAction("Facade", FParent,
                                SLOT(callbackSelectionContour()));

   QMenu * selectionFace = selectionArchi ->
                           addMenu(*HTML::decode("S&eacute;lection de face"));

   selectionFace -> addAction("Ouverture", FParent,
                              SLOT(callbackSelectionOuverture()));
   selectionFace -> addAction("Brique", FParent,
                              SLOT(callbackSelectionBrique()));
   selectionFace -> addAction("Moquette", FParent,
                              SLOT(callbackSelectionMoquette()));
   selectionFace -> addAction("Plaque plafond", FParent,
                              SLOT(callbackSelectionPlaque()));
   selectionFace -> addAction("Tapisserie", FParent,
                              SLOT(callbackSelectionTapisserie()));
   selectionFace -> addAction("Facade", FParent,
                              SLOT(callbackSelectionFacade()));
   selectionFace -> addAction("Marche", FParent,
                              SLOT(callbackSelectionMarche()));
   selectionFace -> addAction("Int marche", FParent,
                              SLOT(callbackSelectionIntMarche()));
   selectionFace -> addSeparator() ;
   selectionFace -> addAction("Doublons", FParent,
                              SLOT(callbackSelectionDoublons3D()));

   QMenu * selectionArete = selectionArchi->
         addMenu(*HTML::decode("S&eacute;lection des ar&ecirc;tes")) ;

   selectionArete -> addAction("Ouverture", FParent,
                               SLOT(callbackSelectionOuverture2D()));
   selectionArete -> addAction("Brique", FParent,
                               SLOT(callbackSelectionBrique2D()));
   selectionArete -> addAction("Tapisserie", FParent,
                               SLOT(callbackSelectionTapisserie2D()));
   selectionArete -> addAction("Facade", FParent,
                               SLOT(callbackSelectionFacade2D()));
   selectionArete -> addAction("Marche", FParent,
                               SLOT(callbackSelectionMarche2D()));
   selectionArete -> addAction("Int marche", FParent,
                               SLOT(callbackSelectionIntMarche2D()));
   selectionArete -> addSeparator() ;
   selectionArete -> addAction("Doublons", FParent,
                               SLOT(callbackSelectionDoublons2D()));

   selectionArchi -> addSeparator() ;

   QMenu* selectionErreur =
      selectionArchi->addMenu(*HTML::decode("D&eacute;tection des erreurs"));

   selectionErreur->addAction(*HTML::decode("S&eacute;lection des autres..."),
                              FParent, SLOT(callbackSelectionAutre()));
   selectionErreur -> addAction("Voisinage 2D", FParent,
                                SLOT(callbackDetectErrorSem2D()));
   selectionErreur -> addAction("Voisinage 3D", FParent,
                                SLOT(callbackDetectErrorSem3D()));
   selectionArchi -> addSeparator() ;
   selectionArchi -> addAction(*HTML::decode("Ar&ecirc;tes fictives"), FParent,
                               SLOT(callbackSelectionFictive()));
   selectionArchi->
   addAction(*HTML::decode("Ar&ecirc;tes d&eacute;coup&eacute;s"), FParent,
             SLOT(callbackSelectionCut()));
   selectionArchi -> addSeparator() ;
   selectionArchi -> addAction(*HTML::decode("De-s&eacute;lection"), FParent,
                               SLOT(callbackDeSelectionBrin()));
#endif
}

//******************************************
// Destructeur
//******************************************
MenuBar :: ~MenuBar() { }

#ifdef MODULE_ARCHITECTURE

// Active ou desactive le mode architecture.
void MenuBar::switchModeArchi(bool active)
{
   creations_act -> setVisible(!active);
   geologie_act -> setVisible(!active);
   materialiser_act -> setVisible(!active);
   architecture_act -> setVisible(active);
   ajout_etage_act -> setVisible(active);
   ajout_plan2D_act -> setVisible(active);
   exporter_act -> setVisible(!active);

   supprimer1_act -> setVisible(!active);
   supprimer2_act -> setVisible(active);

   disconnect(charger_act, 0, 0, 0);
   disconnect(sauver_act, 0, 0, 0);

   if (active)
   {
      connect(charger_act, SIGNAL(triggered()), FParent,
              SLOT(callbackLoadArchi()));
      connect(sauver_act, SIGNAL(triggered()), FParent,
              SLOT(callbackSaveArchi()));
   }
   else
   {
      connect(charger_act, SIGNAL(triggered()), FParent, SLOT(callbackLoad()));
      connect(sauver_act, SIGNAL(triggered()), FParent, SLOT(callbackSave()));
   }
}

#endif
