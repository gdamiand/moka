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

   // Creation du menu Fichier
   //-------------------------
   fichier = new QMenu("&File");
   fichier_act = addMenu(fichier);
   
   fichier -> addAction(pf0 , "&New" , FParent ,
                        SLOT(callbackNettoyage()) , Qt::CTRL + Qt::Key_N) ;
   
   charger_act = fichier -> addAction(pf1 , "&Load" , FParent ,
                                      SLOT(callbackLoad()) ,
                                      Qt::CTRL + Qt::Key_O) ;
   
   ajouter_act = fichier -> addAction(pf2 , "&Add" , FParent ,
                                      SLOT(callbackAdd()) ,
                                           Qt::CTRL + Qt::Key_A) ;

   sauver_act = fichier -> addAction(pf3 , "&Save" , FParent ,
                                     SLOT(callbackSave()) ,
                                     Qt::CTRL + Qt::Key_S) ;
   
   fichier -> addSeparator() ;
   
   importer_act = fichier -> addAction(pf7 , "&Import" , FParent ,
                                       SLOT(callbackImport())) ;
   
   exporter_act = fichier -> addAction(pf4 , "&Export" , FParent ,
                                       SLOT(callbackExport())) ;
   
   fichier -> addSeparator() ;
   
   fichier -> addAction("Search sub-map" , FParent ,
                        SLOT(callbackFindMotif())) ;
   
   fichier -> addAction("Count all the sub-maps" , FParent ,
                        SLOT(callbackCountMotifs())) ;
   
   fichier -> addSeparator() ;

   fichier -> addAction(pf5 , "&Quit" , FParent ,
                        SLOT(callbackQuit()) , Qt::CTRL + Qt::Key_Q) ;

   // Creation du menu Edition
   //-------------------------
   edition = new QMenu("&Edition");
   edition_act = addMenu(edition);

   edition -> addAction(pe1 , "&Undo" , FParent ,
                        SLOT(callbackUndo()) , Qt::Key_U) ;
   edition -> addAction(pe2 , "&Redo" , FParent ,
                        SLOT(callbackRedo()) , Qt::SHIFT + Qt::Key_U) ;

   edition -> addSeparator() ;

   edition -> addAction(pe3 , "&Empty the stack" , FParent ,
                        SLOT(callbackEmptyUndoRedo()) , Qt::ALT + Qt::Key_U);

   edition -> addSeparator() ;

   edition -> addAction(pe4 , "&Configuration" , FParent ,
                        SLOT(callbackConfigUndoRedo()) , Qt::CTRL + Qt::Key_U);

   // Creation du menu Options
   //-------------------------
   options = new QMenu("&Options");
   options_act = addMenu(options);

   options -> addAction(po1 , "Hel&p" , FParent ,
                        SLOT(help()) , Qt::Key_F1) ;

   options -> addSeparator() ;

   options ->  addAction("&Drawing/Plating" , FParent ,
                         SLOT(affich()) , Qt::Key_F2) ;

   options->addAction("&Burst view", FParent,
                      SLOT(view()) , Qt::Key_F3) ;

   options -> addSeparator() ;

   //options ->  addAction ( "&Placages" , FParent ,
//     SLOT (  plating ( ) ) , Qt::Key_F4 ) ;

   options ->  addAction("Deformation &weighting",
			 FParent, SLOT(pondDef()) , Qt::Key_F4) ;
   options ->  addAction("&Extrusion/Thickening" , FParent ,
                         SLOT(extrusion()) , Qt::Key_F5) ;
#ifdef MODULE_MESH
   options ->  addAction("&Interpolation/Smoothing" , FParent ,
                         SLOT(interpol()) , Qt::Key_F6) ;
#endif

   options ->  addAction("M&ove camera", FParent ,
                         SLOT(affichDivers()) , Qt::Key_F7) ;

   options -> addSeparator() ;

   options ->  addAction("&Colors", FParent ,
                         SLOT(couleurs()) , Qt::Key_F8) ;

   options -> addSeparator() ;

   options->addAction("&Topological characteristics", FParent,
                      SLOT(caracTopo()), *new QKeySequence("ALT+c"));

   options->addAction("Compute &surfacic homology generators", FParent,
		      SLOT(callbackComputeSurfacicHomology()),
		      *new QKeySequence("ALT+SHIFT+h"));
   
   options->addAction("Compute volumic &homology generators", FParent,
		      SLOT(callbackComputeVolumicHomology()),
		      *new QKeySequence("ALT+h"));
   
   options->addAction("Is 2D quasi-&manifold",
                      FParent,
                      SLOT(callback2Manifold())) ;

   // Creation du menu creations
   //---------------------------
   creations = new QMenu("C&reate");
   creations_act = addMenu(creations);

   creations -> addAction(pc1 , "&Dart/Polyline/Face" , FParent ,
                          SLOT(callbackPolyline()) ,
                          *new QKeySequence("CTRL+x,CTRL+1")) ;
   creations -> addAction(pc2 , "Regular &polygon",
                          FParent ,
                          SLOT(callbackPolygon()) ,
                          *new QKeySequence("CTRL+x,CTRL+2")) ;
   creations -> addAction(pc3 , "&Mesh" , FParent ,
                          SLOT(callbackMeshCreation()) ,
                          *new QKeySequence("CTRL+x,CTRL+3")) ;
   creations -> addAction(pc4 , "&Sphere", FParent ,
                          SLOT(callbackSphere()) ,
                          *new QKeySequence("CTRL+x,CTRL+4")) ;
   creations -> addAction(pc5 , "&Cylinder" , FParent ,
                          SLOT(callbackCylinder()) ,
                          *new QKeySequence("CTRL+x,CTRL+5")) ;
   creations -> addAction(pc6 , "P&yramid/Cone" ,
                          FParent ,
                          SLOT(callbackPyramide()),
                          *new QKeySequence("CTRL+x,CTRL+6")) ;
   creations -> addAction(pc7 , "&Torus" , FParent ,
                          SLOT(callbackTorus()) ,
                          *new QKeySequence("CTRL+x,CTRL+7")) ;

   creations -> addSeparator() ;
   
   creations -> addAction("&Randomize darts" , FParent ,
                          SLOT(callbackRandomizeDarts()));
   
   // Creation du menu Operations
   //----------------------------
   operations = new QMenu(*HTML::decode("O&perations"));
   operations_act = addMenu(operations);

   // -- translation/rotation/homothetie
   operations ->  addAction(pop2 , "&Translation" , FParent ,
                            SLOT(callbackTranslateWindow()) , Qt::Key_T) ;
   operations ->  addAction(pop3 , "&Rotation" , FParent ,
                            SLOT(callbackRotateWindow()) , Qt::Key_R) ;
   operations ->  addAction(pop1 , "&Homothethy" ,
                            FParent ,
                            SLOT(callbackScaleWindow()) , Qt::Key_H) ;
   operations ->  addAction("M&ove to" , FParent ,
                            SLOT(callbackMovetoWindow()),
			    Qt::SHIFT + Qt::Key_T) ;

   operations -> addSeparator() ;

   //-- coudre
   QMenu * coudre = operations -> addMenu("&Sew") ;

   QMenu * intuitif = coudre ->  addMenu("&Intuitively") ;
   intuitif ->  addAction("&Topologically" , FParent ,
                          SLOT(callbackIntuitiveTopoSew()) ,
                          Qt::SHIFT + Qt::Key_C) ;
   intuitif ->  addAction("&Geometrically" , FParent ,
                          SLOT(callbackIntuitiveGeoSew()) , Qt::Key_C) ;

   coudre -> addSeparator() ;

   coudre ->  addAction("&Vertices" , FParent , SLOT(callbackTopoSew0()));

   QMenu * coudreAretes = coudre ->  addMenu("&Edges");
   coudreAretes ->  addAction("&Topologically" , FParent ,
                              SLOT(callbackTopoSew1())) ;
   coudreAretes->addAction("&Geometrically", FParent ,
                           SLOT(callbackGeoSew1())) ;

   QMenu * coudreFaces = coudre ->  addMenu("&Faces") ;
   coudreFaces ->  addAction("&Topologically" , FParent ,
                             SLOT(callbackTopoSew2())) ;
   coudreFaces ->  addAction("&Geometrically", FParent ,
                             SLOT(callbackGeoSew2())) ;

   QMenu * coudreVolumes = coudre ->  addMenu("V&olumes") ;
   coudreVolumes ->  addAction("&Topologically" , FParent ,
                               SLOT(callbackTopoSew3())) ;
   coudreVolumes->addAction("&Geometrically", FParent ,
                            SLOT(callbackGeoSew3())) ;

   coudre -> addSeparator() ;

   QMenu * coudre1bord = coudre ->  addMenu("&1-border") ;
   coudre1bord ->  addAction("&Topologically" , FParent ,
                             SLOT(callbackTopoSew1borders())) ;
   coudre1bord->addAction("&Geometrically" , FParent ,
                          SLOT(callbackGeoSew1borders())) ;

   QMenu * coudre2bords = coudre ->  addMenu("&2-border") ;
   coudre2bords ->  addAction("&Topologically" , FParent ,
                              SLOT(callbackTopoSew2borders()), 
                                   Qt::SHIFT + Qt::Key_B) ;
   coudre2bords ->  addAction("&Geometrically", FParent ,
                              SLOT(callbackGeoSew2borders()) , Qt::Key_B) ;

   coudre -> addSeparator() ;

   QMenu * inverserCouture =
      coudre->addMenu("&Reverse the last sewing");
   inverserCouture ->  addAction("&Orientation" , FParent ,
                                 SLOT(callbackInvertLastOperation()) , 
                                      Qt::Key_X) ;
   inverserCouture ->  addAction("&Direction" , FParent ,
                                 SLOT(callbackInvertLastOperationGeometry()),
                                 Qt::SHIFT + Qt::Key_X) ;

   // -- Decoudre
   QMenu * decoudre = operations->addMenu(*HTML::decode("&Unsew"));

   decoudre -> addAction("&Intuitively" , FParent ,
                         SLOT(callbackIntuitiveUnsew()) , Qt::Key_D) ;

   decoudre -> addSeparator() ;

   decoudre -> addAction("&Vertices" , FParent ,
                         SLOT(callbackUnsew0())) ;
   decoudre -> addAction("&Edges" , FParent ,
                         SLOT(callbackUnsew1())) ;
   decoudre -> addAction("&Faces" , FParent ,
                         SLOT(callbackUnsew2())) ;
   decoudre -> addAction("V&olumes" , FParent ,
                         SLOT(callbackUnsew3())) ;

   // -- Placage
   QMenu * placage = operations -> addMenu("&Plate") ;

   placage -> addAction("&Intuitively" , FParent ,
                        SLOT(callbackIntuitivePlate()) , Qt::Key_P) ;

   placage -> addSeparator() ;

   placage -> addAction("&Vertices" , FParent , SLOT(callbackPlate0()));
   placage -> addAction("&Edges" , FParent , SLOT(callbackPlate1()));
   placage -> addAction("&Faces" , FParent , SLOT(callbackPlate2()));

   placage -> addSeparator() ;

   placage -> addAction("&1-border" , FParent ,
                        SLOT(callbackPlate1borders())) ;
   placage -> addAction("&2-border" , FParent ,
                        SLOT(callbackPlate2borders())) ;

   operations -> addSeparator() ;

   // -- Fusionner
   QMenu * fusionner = operations -> addMenu("&Merge") ;

   fusionner -> addAction("&Intuitively" , FParent ,
                          SLOT(callbackIntuitiveMerge()) , Qt::Key_F) ;

   fusionner -> addSeparator() ;

   fusionner -> addAction("&Edges", FParent, SLOT(callbackMergeEdges()));
   fusionner -> addAction("&Faces", FParent, SLOT(callbackMergeFaces()));
   fusionner -> addAction("&Volumes", FParent, SLOT(callbackMergeVolumes()));

   fusionner -> addSeparator() ;

   fusionner -> addAction("&Aligned edges", FParent ,
                          SLOT(callbackMergeColinearEdges()))  ;
   fusionner -> addAction("&Coplanar faces" , FParent ,
                          SLOT(callbackMergeCoplanarFaces())) ;
         
   fusionner -> addSeparator() ;

   fusionner -> addAction("Faces without &disconnection" , FParent ,
                          SLOT(callbackMergeFacesWithoutDisconnection()));
   
   fusionner -> addAction("Volumes by keeping balls", FParent ,
                           SLOT(callbackRemoveMarkedFacesButKeepBalls()));
   
   
   fusionner->addAction("Shift all edges incident to vertex", FParent ,
                        SLOT(callbackShiftAllEdgesIncidentToVertex()));
   fusionner->addAction("Remove dangling edges", FParent,
			SLOT(callbackRemoveDanglingEdges()));

   fusionner -> addAction("Simplify 2D object", FParent ,
                           SLOT(callbackSimplify2DObject()));
   fusionner -> addAction("Simplify 3D object", FParent ,
                           SLOT(callbackSimplify3DObject()));
   
   // -- Contracter
   QMenu * contracter = operations -> addMenu("&Contract") ;

   contracter -> addAction("&Intuitively" , FParent ,
                           SLOT(callbackIntuitiveContract()) , 
                                Qt::SHIFT + Qt::Key_F);

   contracter -> addSeparator() ;

   contracter -> addAction("&Edges" , FParent ,
                           SLOT(callbackContractEdges())) ;
   contracter -> addAction("&Faces" ,  FParent ,
                           SLOT(callbackContractFaces())) ;
   contracter -> addAction("&Volumes" , FParent ,
                           SLOT(callbackContractVolumes())) ;

   contracter -> addSeparator() ;


   contracter->addAction("&Null length edges", FParent ,
			 SLOT(callbackDelNullEdges())) ;
   contracter -> addAction("Flattened f&aces" ,  FParent ,
                           SLOT(callbackDelFlatFaces())) ;
   contracter -> addAction("Flattened V&olumes" , FParent ,
                           SLOT(callbackDelFlatVolumes())) ;

   // -- Inserer
   QMenu * inserer = operations -> addMenu("&Insert");

   inserer -> addAction("&Vertices" , FParent ,
                        SLOT(callbackInsertVertex())) ;
   inserer -> addAction("&Edges" , FParent ,
                        SLOT(callbackInsertEdge())) ;
   inserer -> addAction("&Faces" , FParent ,
                        SLOT(callbackInsertFace())) ;

   // -- Fermer
   QMenu * fermer = operations -> addMenu("C&lose") ;

   fermer -> addAction("&Intuitively" , FParent ,
                       SLOT(callbackIntuitiveStopUp())) ;

   fermer -> addSeparator() ;

   fermer -> addAction("&Edges" , FParent , SLOT(callbackStopUp0())) ;
   fermer -> addAction("&Faces" , FParent , SLOT(callbackStopUp1())) ;
   fermer -> addAction("&Volumes" , FParent , SLOT(callbackStopUp2())) ;
   fermer -> addAction("&Connected components" , FParent ,
                       SLOT(callbackStopUp3())) ;

   // -- Trianguler
   QMenu * trianguler = operations -> addMenu("Trian&gulate") ;

   trianguler -> addAction("&Depending on selected mode", FParent ,
                           SLOT(callbackContextTriangulate()), 
                           Qt::ALT + Qt::Key_T);

   trianguler -> addSeparator() ;

   trianguler -> addAction("&Edges" , FParent ,
                           SLOT(callbackTriangulateEdges())) ;
   trianguler -> addAction("&Faces" , FParent ,
                           SLOT(callbackTriangulateFaces())) ;
   trianguler -> addAction("&Volumes" , FParent ,
                           SLOT(callbackTriangulateVolumes())) ;
   trianguler -> addSeparator() ;

   trianguler -> addAction("Triangulate faces &geometrically" ,
			   FParent , SLOT(callbackTriangulateGeoFaces())) ;

trianguler -> addAction("&Turn selected edge",
			   FParent , SLOT(callbackSwapEdge())) ;

   // -- Quadranguler
   QMenu * quadranguler = operations -> addMenu("&Quadrangulate") ;

   QMenu * faces = quadranguler -> addMenu("&Faces") ;
   faces -> addAction("&With new vertices" , FParent ,
                      SLOT(callbackQuadrangulateFaces()), Qt::ALT+Qt::Key_Q);
   faces -> addAction("Wi&thout new vertices" , FParent ,
                      SLOT(callbackBasicQuadrangulateFaces())) ;

   // -- Calculer Dual
   QMenu * calculerDual = operations -> addMenu("Compute &dual") ;
   calculerDual -> addAction("&2D" , FParent , SLOT(callbackDual2d())) ;
   calculerDual -> addAction("&3D" , FParent , SLOT(callbackDual3d())) ;

   operations -> addSeparator() ;

   // -- Extruder
   QMenu * extruder = operations -> addMenu("&Extrude") ;

   ///Extruder / selon normale
   QMenu * normale = extruder -> addMenu("Along &normal") ;
   normale -> addAction("&Intuitively" , FParent ,
                        SLOT(callbackIntuitiveExtrudeByNormal()), Qt::Key_N);

   normale -> addSeparator() ;

   normale -> addAction("&Vertices" , FParent ,
                        SLOT(callbackExtrude0ByNormal())) ;
   normale -> addAction("&Edges" , FParent ,
                        SLOT(callbackExtrude1ByNormal())) ;
   normale -> addAction("&Faces" , FParent ,
                        SLOT(callbackExtrude2ByNormal())) ;

   ///Extruder / selon chemin
   QMenu * chemin = extruder -> addMenu("Along &path") ;
   chemin -> addAction("&Edges" , FParent ,
                       SLOT(callbackExtrude1ByPath()) , Qt::SHIFT+Qt::Key_E);
   chemin -> addAction("&Faces" , FParent ,
                       SLOT(callbackExtrude2ByPath()) , Qt::Key_E) ;

   ///Extruder / selon axe
   QMenu * axe = extruder -> addMenu("Along &axis") ;

   axe -> addAction("&Edges" , FParent ,
                    SLOT(callbackExtrude1ByRevolution()) ,
                    Qt::CTRL + Qt::SHIFT + Qt::Key_E) ;
   axe -> addAction("&Faces" , FParent ,
                    SLOT(callbackExtrude2ByRevolution()) ,
                    Qt::ALT + Qt::SHIFT + Qt::Key_E) ;

   ///Extruder / epaissir
   extruder -> addAction("&Thick", FParent, SLOT(callbackThicken())) ;

   // -- Interpoler
#ifdef MODULE_MESH
   QMenu * interpoler = operations -> addMenu("&Interpolate") ;

   interpoler -> addAction("&Edges" , FParent ,
                           SLOT(callbackMeshEdge())) ;


   QMenu * carres = interpoler -> addMenu("&Squares");
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
   QMenu * lisser = operations -> addMenu("&Smooth") ;

   lisser -> addAction("&Edges" , FParent ,
                       SLOT(callbackSmooth1())) ;
   lisser -> addAction("&Squares" , FParent ,
                       SLOT(callbackSmooth2())) ;
   lisser -> addAction("C&ubes" , FParent ,
                       SLOT(callbackSmooth3())) ;
#endif // MODULE_MESH

   // -- Arrondir
#ifdef MODULE_ROUNDING
   QMenu * arrondir = operations -> addMenu("&Round") ;

   arrondir -> addAction("&Vertices" , FParent ,
                         SLOT(callbackRoundVertices())) ;
   arrondir -> addAction("Edges &2D" , FParent ,
                         SLOT(callbackRoundEdges2d())) ;
   arrondir -> addAction("Edges &3D" , FParent ,
                         SLOT(callbackRoundEdges3d())) ;

   arrondir -> addSeparator() ;

   arrondir -> addAction("Dug vertices" , FParent ,
                         SLOT(callbackRoundVerticesDig()) , Qt::Key_A) ;

   arrondir -> addAction("3D dug edges" , FParent ,
                         SLOT(callbackRoundEdges3dDig()), Qt::SHIFT+Qt::Key_A);

   arrondir -> addSeparator() ;

   arrondir -> addAction("2D edges with setback" , FParent ,
                         SLOT(callbackRoundEdges2dWithSetback())) ;
   arrondir -> addAction("3D edges with setback" , FParent ,
                         SLOT(callbackRoundEdges3dWithSetback())) ;
   arrondir->addAction("3D dug edges with setback" , FParent ,
                       SLOT(callbackRoundEdges3dDigWithSetback()) ,
                       Qt::CTRL + Qt::SHIFT + Qt::Key_A) ;

   arrondir -> addSeparator() ;

   arrondir -> addAction("Variable coefficients" , FParent ,
                         SLOT(chanf())) ;
#endif // MODULE_ROUNDING

   // -- Triangulation
#ifdef MODULE_TRIANGULATION
   QMenu * triangulation = operations -> addMenu("Trian&gulation") ;

   triangulation -> addAction("&Delanyay triangulation" , FParent ,
                              SLOT(callbackDelaunayTriangulation())) ;

   triangulation -> addSeparator() ;

   /*triangulation -> addAction ( "Trianguler faces &topologiquement" , 
   FParent ,SLOT (  callbackTriangulateTopoFaces ( ) ) ) ; */
   triangulation -> addAction("&Mark non-uniform faces" , FParent ,
                              SLOT(callbackMarkNonUniformFaces())) ;
   triangulation -> addAction("&Optimize edges positions" , FParent ,
                              SLOT(callbackOptimizeSelectedEdges())) ;
   triangulation -> addAction("D&elaunay triangulation optimized", FParent ,
                              SLOT(callbackOptimizedDelaunayTriangulation()));
#endif // MODULE_TRIANGULATION

   // -- Co-raffiner
#ifdef MODULE_COREFINEMENT
   QMenu * coraffiner = operations -> addMenu("Corefine") ;

   coraffiner -> addAction("2D mesh - &sweeping" , FParent ,
                           SLOT(callbackCorefine2dSegmentsSweeping()) ,
                           Qt::Key_AsciiCircum) ;
   coraffiner -> addAction("2D Mesh - &propogation" , FParent ,
                           SLOT(callbackCorefine2dSegmentsPropagation()) ,
                           Qt::Key_Backslash) ;
   coraffiner->
   addAction(*HTML::decode("2D boolean operations"),
             FParent , SLOT(callbackBooleanOperations2d())) ;

   coraffiner -> addSeparator() ;

   coraffiner -> addAction("3D Mesh" , FParent ,
                           SLOT(callbackCorefine3dFaces())) ;
   coraffiner -> addAction("3D Faces" , FParent ,
                           SLOT(callbackCorefineMarked3dFaces())) ;
coraffiner -> addAction("&All selected 3D meshes",FParent ,
                           SLOT(callbackCorefineMarked3dMeshesWith())) ;
   coraffiner->addAction("3D boolean operations 3D" , FParent,
			 SLOT(callbackBooleanOperations3d()), Qt::Key_At);
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
   materialiser = new QMenu("&Materialize");
   materialiser_act = addMenu(materialiser);

   materialiser -> addAction("&Barycenter" , FParent ,
                             SLOT(callbackMaterializeBarycenter())) ;
   materialiser -> addAction("&Axis" , FParent ,
                             SLOT(callbackMaterializeAxe())) ;
   materialiser -> addAction("&Plane" , FParent ,
                             SLOT(callbackMaterializePlane())) ;
   materialiser -> addAction("&Normal vector" , FParent ,
                             SLOT(callbackMaterializeNormalVector())) ;
   materialiser -> addAction("Normal &plane" , FParent ,
                             SLOT(callbackMaterializeOrthoPlane())) ;

   materialiser -> addSeparator() ;

   materialiser -> addAction("&Reference point" , FParent ,
                             SLOT(callbackMaterializeReferential())) ;

   // Creation du menu Selection
   //---------------------------
   selection = new QMenu("&Selection");
   selection_act = addMenu(selection);

   selection -> addAction("Reinit" , FParent ,
                          SLOT(callbackReinitSelection()) ,
                          Qt::Key_Escape) ;
   selection -> addAction("Inverse" , FParent ,
                          SLOT(callbackNegateSelection()) ,
                          Qt::Key_Minus) ;
   selection -> addAction("Swap" , FParent ,
                          SLOT(callbackSwapSelections()) ,
                          Qt::Key_Asterisk) ;

   selection -> addSeparator() ;

   selection -> addAction("Duplicate" , FParent ,
                          SLOT(callbackDuplicateSelection()) ,
                          Qt::Key_Enter) ;

   selection -> addAction("Remove" , FParent ,
                          SLOT(callbackDeleteSelection()) ,
                          Qt::Key_Delete) ;

   selection -> addSeparator() ;

   QMenu * etendre = selection -> addMenu("Grow") ;
   QMenu * etendre1pas = etendre -> addMenu("1 step") ;

   etendre1pas -> addAction("Forward" , FParent ,
                            SLOT(callbackGoForward1()) ,
                            Qt::Key_I) ;
   etendre1pas -> addAction(*HTML::decode("Backward") , FParent ,
                            SLOT(callbackGoBackward1()) ,
                            Qt::Key_K) ;
   etendre1pas -> addAction("To the left" , FParent ,
                            SLOT(callbackGoLeft1()) ,
                            Qt::Key_J) ;
   etendre1pas -> addAction("To the right" , FParent ,
                            SLOT(callbackGoRight1()) ,
                            Qt::Key_L) ;

   QMenu * etendreRepet = etendre->addMenu("Repetitively");

   etendreRepet -> addAction("Forward" , FParent ,
                             SLOT(callbackGoForward1Rep()) ,
                             Qt::CTRL + Qt::Key_I) ;
   etendreRepet -> addAction("Backward" , FParent ,
                             SLOT(callbackGoBackward1Rep()) ,
                             Qt::CTRL + Qt::Key_K) ;
   etendreRepet -> addAction("To the left" , FParent ,
                             SLOT(callbackGoLeft1Rep()) ,
                             Qt::CTRL + Qt::Key_J) ;
   etendreRepet -> addAction("To the right" , FParent ,
                             SLOT(callbackGoRight1Rep()) ,
                             Qt::CTRL + Qt::Key_L) ;

   QMenu * reduire = selection->addMenu("Reduce");
   QMenu * reduire1pas = reduire -> addMenu("1 step") ;

   reduire1pas -> addAction("Forward" , FParent ,
                            SLOT(callbackGoForward0()) ,
                            Qt::SHIFT + Qt::Key_I) ;
   reduire1pas -> addAction("Backward" , FParent ,
                            SLOT(callbackGoBackward0()) ,
                            Qt::SHIFT + Qt::Key_K) ;
   reduire1pas -> addAction("To the left" , FParent ,
                            SLOT(callbackGoLeft0()) ,
                            Qt::SHIFT + Qt::Key_J) ;
   reduire1pas -> addAction("To the right" , FParent ,
                            SLOT(callbackGoRight0()) ,
                            Qt::SHIFT + Qt::Key_L) ;

   QMenu * reduireRepet =
      reduire->addMenu("Repetitively") ;

   reduireRepet -> addAction("Forward" , FParent ,
                             SLOT(callbackGoForward0Rep()) ,
                             Qt::CTRL + Qt::SHIFT + Qt::Key_I) ;
   reduireRepet -> addAction("Backward" , FParent ,
                             SLOT(callbackGoBackward0Rep()) ,
                             Qt::CTRL + Qt::SHIFT + Qt::Key_K) ;
   reduireRepet -> addAction("To the left" , FParent ,
                             SLOT(callbackGoLeft0Rep()) ,
                             Qt::CTRL + Qt::SHIFT + Qt::Key_J) ;
   reduireRepet -> addAction("To the right" , FParent ,
                             SLOT(callbackGoRight0Rep()) ,
                             Qt::CTRL + Qt::SHIFT + Qt::Key_L) ;

   selection -> addSeparator() ;

   QMenu * marquerInterieur =
      selection->addMenu(*HTML::decode("Mark interior"));

   marquerInterieur -> addAction("2D" , FParent ,
                                 SLOT(callbackMarkInterior2())) ;
   marquerInterieur -> addAction("3D" , FParent ,
                                 SLOT(callbackMarkInterior3())) ;

   selection -> addSeparator() ;

   QMenu * selectBords = selection->addMenu("Select borders") ;

   selectBords -> addAction("Intuitively" , FParent ,
                            SLOT(callbackIntuitiveSelectBorders()) ,
                            Qt::ALT + Qt::Key_B) ;

   selectBords -> addSeparator() ;

   selectBords -> addAction("0-border" , FParent ,
                            SLOT(callbackSelect0borders())) ;
   selectBords -> addAction("1-border" , FParent ,
                            SLOT(callbackSelect1borders())) ;
   selectBords -> addAction("2-border" , FParent ,
                            SLOT(callbackSelect2borders())) ;
   selectBords -> addAction("3-border" , FParent ,
                            SLOT(callbackSelect3borders())) ;

   selection->addAction("Select non planar faces", FParent ,
                        SLOT(callbackSelectNonPlanarFaces()));

   selection -> addSeparator();

   selection -> addAction("Incident cells" , FParent ,
                          SLOT(callbackSelectIncidents()) ,
                          Qt::Key_S) ;

   QMenu * avancer = selection -> addMenu("Move by") ;

   avancer -> addAction("Alpha0" , FParent ,
                        SLOT(callbackGoAlpha0()), Qt::ALT + Qt::Key_0) ;
   avancer -> addAction("Alpha1" , FParent ,
                        SLOT(callbackGoAlpha1()), Qt::ALT + Qt::Key_1) ;
   avancer -> addAction("Alpha2" , FParent ,
                        SLOT(callbackGoAlpha2()), Qt::ALT + Qt::Key_2) ;
   avancer -> addAction("Alpha3" , FParent ,
                        SLOT(callbackGoAlpha3()), Qt::ALT + Qt::Key_3) ;

   // Creation du menu Vues
   //----------------------
   vues = new QMenu("&Views");
   vues_act = addMenu(vues);

   QMenu * arranger = vues -> addMenu("&Organize") ;
   arranger -> addAction("&Cascade" , FParent ,
                         SLOT(cascade())) ;
   arranger -> addAction("&Mosaic" ,  FParent ,
                         SLOT(tile()) , Qt::Key_M) ;

   vues -> addAction(pv2 , "&Remove" , FParent ,
                     SLOT(deleteView()) , Qt::Key_Backspace) ;

   QMenu * basculer = vues -> addMenu("&Swith to") ;
   basculer -> addAction("&3D view" , FParent ,
                         SLOT(basculeView3D()) , Qt::Key_0);
   basculer -> addAction("&XY view" , FParent ,
                         SLOT(basculeViewXY()) , Qt::Key_1);
   basculer -> addAction("&YZ view" , FParent ,
                         SLOT(basculeViewYZ()) , Qt::Key_2);
   basculer -> addAction("&ZX view" , FParent ,
                         SLOT(basculeViewZX()) , Qt::Key_3);
   basculer -> addSeparator() ;
   basculer -> addAction("&Multi-view" , FParent ,
                         SLOT(basculeViewMulti()) , Qt::Key_4);

   vues -> addSeparator() ;

   QMenu * ajouter = vues -> addMenu("&Add") ;
   ajouter -> addAction(pv1 , "&3D view" , FParent ,
                        SLOT(addView3D()) , Qt::CTRL + Qt::Key_0);
   ajouter -> addAction("&XY view" , FParent ,
                        SLOT(addViewXY()) , Qt::CTRL + Qt::Key_1) ;
   ajouter -> addAction("&YZ view" , FParent ,
                        SLOT(addViewYZ()) , Qt::CTRL + Qt::Key_2) ;
   ajouter -> addAction("&ZX view" , FParent ,
                        SLOT(addViewZX()) , Qt::CTRL + Qt::Key_3) ;
   ajouter -> addSeparator() ;
   ajouter -> addAction("&Multi-view" , FParent ,
                        SLOT(addViewMulti()) , Qt::CTRL + Qt::Key_4) ;

   vues -> addSeparator() ;

   QMenu * grouper = vues -> addMenu("&Group") ;
   grouper -> addAction(pv3 , "&All the views with the current view" ,
                        FParent , SLOT(OperationGroupAllGeneral()) ,
                        Qt::Key_G) ;

   grouper -> addSeparator() ;

   grouper -> addAction("&Precompiles of all the views", FParent ,
                        SLOT(OperationGroupAllPrecomp())) ;

   grouper -> addAction("Camera position of all the views" ,
                        FParent , SLOT(OperationGroupAllEyePos())) ;

   grouper -> addAction("Aimed point of all the views" ,
                        FParent , SLOT(OperationGroupAllAimedPos())) ;

   grouper -> addAction("Drawing parameters of all the views" ,
                        FParent , SLOT(OperationGroupAllDrawing())) ;

   grouper -> addSeparator() ;

   grouper->addAction("Current view and double-clicked view",
                      FParent , SLOT(OperationGroupGeneral()) ,
                      Qt::ALT + Qt::Key_G) ;

   grouper -> addSeparator() ;

   grouper->addAction("&Precompiles of these 2 views",
                      FParent, SLOT(OperationGroupPrecomp()));

   grouper -> addAction("Camera position of these 2 views" ,
                        FParent , SLOT(OperationGroupEyePos()));

   grouper->addAction("Aimed point of these 2 views",
                      FParent, SLOT(OperationGroupAimedPos()));

   grouper->addAction("Drawing parameters of these 2 views" ,
                      FParent , SLOT(OperationGroupDrawing()));

   QMenu * degrouper = vues->addMenu("&Ungroup");

   degrouper -> addAction(pv4 , "&All the views" , FParent ,
                          SLOT(OperationUngroupAllGeneral()) ,
                          Qt::SHIFT + Qt::Key_G) ;

   degrouper -> addSeparator() ;

   degrouper->addAction("&Precompiles of all the views", FParent ,
			SLOT(OperationUngroupAllPrecompiles()));

   degrouper -> addAction("Camera position of all the views" ,
                          FParent , SLOT(OperationUngroupAllEyePos()));

degrouper -> addAction("Aimed point of all the views", FParent ,
		       SLOT(OperationUngroupAllAimedPos()));

   degrouper->addAction("Drawing parameters of all the views", FParent ,
			SLOT(OperationUngroupAllDrawing()));

   degrouper -> addSeparator() ;

   degrouper -> addAction("Curent view from others" ,
                          FParent , SLOT(OperationUngroupGeneral()) ,
                          *new QKeySequence("SHIFT+ALT+Key_G")) ;

   degrouper -> addSeparator() ;

   degrouper->addAction("Precompiles of this view" ,
                        FParent, SLOT(OperationUngroupPrecompiles()));

   degrouper -> addAction("Camera position of this view" ,
                          FParent , SLOT(OperationUngroupEyePos()));

   degrouper->addAction("Aimed point of this view" ,
                        FParent, SLOT(OperationUngroupAimedPos()));

   degrouper->addAction("Drawing parameters of this view", FParent ,
			SLOT(OperationUngroupDrawing()));

   QMenu * pointDeMire = vues->addMenu("&Aimed point") ;

   pointDeMire -> addAction("&Origin" , FParent ,
                            SLOT(mireOrigine()) , Qt::Key_O) ;
   pointDeMire->addAction("&Vertex incident to 'last'",
                          FParent, SLOT(mireSommet()), Qt::Key_Exclam);

   pointDeMire -> addSeparator();

   QMenu* centre = pointDeMire ->addMenu("&Center of the bounding box of");

   centre -> addAction("&Face incident to 'last'",
                       FParent , SLOT(mireFace())) ;
   centre -> addAction("&Volume incident to 'last'" , FParent ,
                       SLOT(mireVolume())) ;
   centre->addAction("&Connected component incident to 'last'", FParent ,
		  SLOT(mireCompConnexe())) ;
   centre -> addSeparator() ;
   centre -> addAction("All the &marked darts" , FParent ,
                       SLOT(mireBrinsMarques()) , Qt::SHIFT + Qt::Key_O) ;
   centre -> addAction("&All the darts" , FParent ,
                       SLOT(mireBrins()) , Qt::CTRL + Qt::SHIFT + Qt::Key_O) ;

   //- Spamod
#ifdef MODULE_SPAMOD
   QMenu * visualisation = vues -> addMenu("&Spamod mode") ;

   visualisation -> addAction("&Burst view" , FParent,
                              SLOT(callbackBackToBurstView())) ;
   visualisation -> addSeparator() ;
   visualisation -> addAction("&Topological view" , FParent ,
                              SLOT(callbackTopologicalView())) ;
   visualisation -> addAction("&Euclidean view" , FParent ,
                              SLOT(callbackEuclidianView())) ;
   visualisation -> addAction("&Analytic view" , FParent ,
                              SLOT(callbackAnalyticView())) ;
   visualisation -> addAction("&Voxels view" , FParent ,
                              SLOT(callbackVoxelView())) ;
   visualisation -> addAction("&K2 view" , FParent ,
                              SLOT(callbackK2View())) ;
   visualisation -> addSeparator() ;
   visualisation -> addAction("&Switch to the next view" , FParent ,
                              SLOT(callbackSpamodSwitchToNextView())) ;
#endif // MODULE_SPAMOD
}

//******************************************
// Destructeur
//******************************************
MenuBar :: ~MenuBar() { }


