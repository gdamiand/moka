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

#include "creation-maillage.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>


/*******************************************************************/
/* CLASSE creationMaillage                                         */
/*******************************************************************/

// Constructeur
//-------------

CreationMaillage :: CreationMaillage(Window * parent , 
                                     GMap3d :: CControlerGMap * controler) :
      CreationObjet(parent , HTML::decode("Create Mesh") ,
                    controler),
      FModifie(true)
{

   //--------- Partie haute -------------
   QWidget * haut = new QWidget ;
   QHBoxLayout * bb = new QHBoxLayout(haut);
   bb -> setSpacing(0);
   bb -> setContentsMargins(0, 0, 0, 0);

   FAffCel =
      new QLabel(*HTML::decode
                 ("  Dimension of the cell to create: ")) ;

   FDimCell = new QComboBox ;
   FDimCell -> addItem(*HTML::decode("1 (edge/polyline)")) ;
   FDimCell -> addItem("2 (face/grid)") ;
   FDimCell -> addItem("3 (mesh/3D volume)") ;

   FAffMai = new QLabel("Mesh dimension: ") ;

   FDimMail = new QComboBox ;
   FDimMail -> addItem("0 (no mesh)") ;
   FDimMail -> addItem(*HTML::decode("1 (edge mesh)")) ;
   FDimMail -> addItem("2 (face mesh)") ;
   FDimMail -> addItem("3 (volume mesh)") ;

   QObject :: connect(FDimCell , SIGNAL(activated(int)) , this ,
                      SLOT(callbackDimCell())) ;
   QObject :: connect(FDimMail , SIGNAL(activated(int)) , this ,
                      SLOT(callbackDimMesh())) ;

   // Subdivisions
   FAffSub = new QLabel("Subdivisions : ") ;

   FAffX = new QLabel("  X : ") ;
   FSaisieX = new QSpinBox ;
   FSaisieX -> setMinimum(1);
   FSaisieX -> setMaximum(1000);
   FSaisieX -> setValue(1) ;
   FSaisieX -> setAccelerated(true);
         
   FAffY = new QLabel("  Y : ") ;
   FSaisieY = new QSpinBox ;
   FSaisieY -> setMinimum(1);
   FSaisieY -> setMaximum(1000);
   FSaisieY -> setValue(1) ;
   FSaisieY -> setAccelerated(true);
   
   FAffZ = new QLabel("  Z : ") ;
   FSaisieZ = new QSpinBox ;
   FSaisieZ -> setMinimum(1);
   FSaisieZ -> setMaximum(1000);
   FSaisieZ -> setValue(1) ;
   FSaisieZ -> setAccelerated(true);
   
   QObject :: connect(FSaisieX , SIGNAL(valueChanged(int)) , this ,
                      SLOT(callbackSubdivisions())) ;
   QObject :: connect(FSaisieY , SIGNAL(valueChanged(int)) , this ,
                      SLOT(callbackSubdivisions())) ;
   QObject :: connect(FSaisieZ , SIGNAL(valueChanged(int)) , this ,
                      SLOT(callbackSubdivisions())) ;

   bb -> addWidget(FAffCel) ;
   bb -> addWidget(FDimCell) ;
   bb -> addSpacing(SPACE_PIX_SIZE);
   bb -> addWidget(FAffMai) ;
   bb -> addWidget(FDimMail) ;
   bb -> addSpacing(SPACE_PIX_SIZE);
   bb -> addWidget(FAffSub) ;
   bb -> addWidget(FAffX) ;
   bb -> addWidget(FSaisieX) ;
   bb -> addWidget(FAffY) ;
   bb -> addWidget(FSaisieY) ;
   bb -> addWidget(FAffZ) ;
   bb -> addWidget(FSaisieZ) ;

   //----------- Partie basse ----------------

   // Faces creees
   QWidget * bas = new QWidget ;
   QHBoxLayout * bh = new QHBoxLayout(bas);
   bh -> setSpacing(0);
   bh -> setContentsMargins(0, 0, 0, 0);

   FAffFac = new QLabel(*HTML::decode("  Faces created: ")) ;
   FGauche = new QCheckBox("Left  ") ;
   FDroite = new QCheckBox("Right  ") ;
   FDevant = new QCheckBox("Front  ") ;
   FDerriere = new QCheckBox(*HTML::decode("Back  ")) ;
   FBas = new QCheckBox("Down  ") ;
   FHaut = new QCheckBox("Up  ") ;

   // Ajout au layout bas
   bh -> addWidget(FAffFac) ;
   bh -> addWidget(FGauche) ;
   bh -> addWidget(FDroite) ;
   bh -> addWidget(FDevant) ;
   bh -> addWidget(FDerriere) ;
   bh -> addWidget(FBas) ;
   bh -> addWidget(FHaut) ;

   FChoix = new BoutonsCreation(this , bh) ;

   //--------- Layout principal ---------
   QWidget * centre = new QWidget ;
   QVBoxLayout * bl = new QVBoxLayout(centre);
   bl -> setSpacing(5);
   bl -> setContentsMargins(0, 0, 0, 0);
   bl -> addWidget(haut) ;
   bl -> addWidget(bas) ;

   addWidget(centre);

   FPosit = new BoitePositionnement(this , parent , OBJECT_MESH) ;

   update() ;
   repaint() ;

   QObject :: connect(FGauche , SIGNAL(clicked()) , this ,
                      SLOT(callbackToggleButton())) ;
   QObject :: connect(FDroite , SIGNAL(clicked()) , this ,
                      SLOT(callbackToggleButton())) ;
   QObject :: connect(FDevant , SIGNAL(clicked()) , this ,
                      SLOT(callbackToggleButton())) ;
   QObject :: connect(FDerriere , SIGNAL(clicked()) , this ,
                      SLOT(callbackToggleButton())) ;
   QObject :: connect(FBas , SIGNAL(clicked()) , this ,
                      SLOT(callbackToggleButton())) ;
   QObject :: connect(FHaut , SIGNAL(clicked()) , this ,
                      SLOT(callbackToggleButton())) ;
}

// Destructeur
//------------
CreationMaillage :: ~CreationMaillage() { }

void CreationMaillage :: update()
{

   FModifie = false ;
   CreationObjet :: update() ;

   FSaisieX->setValue(FControler->getParameterCreation()
         ->getMeshNbSubdivisionsX());
   FSaisieY->setValue(FControler->getParameterCreation()
         ->getMeshNbSubdivisionsY());
   FSaisieZ->setValue(FControler->getParameterCreation()
         ->getMeshNbSubdivisionsZ());

   unsigned char toCreate = 
         FControler -> getParameterCreation()->getMeshCreatedFaces();

   FGauche ->  setChecked((toCreate & (1 << 0)) != 0) ;
   FDroite ->  setChecked((toCreate & (1 << 1)) != 0) ;
   FDevant ->  setChecked((toCreate & (1 << 2)) != 0) ;
   FDerriere -> setChecked((toCreate & (1 << 3)) != 0) ;
   FBas -> setChecked((toCreate & (1 << 4)) != 0) ;
   FHaut ->  setChecked((toCreate & (1 << 5)) != 0) ;

   int dCell = FControler->getParameterCreation()->getMeshDimension();
   int dMail = FControler->getParameterCreation()->getMeshSubdivisionDimension();

   FDimCell -> setCurrentIndex(dCell - 1) ;
   FDimMail -> setCurrentIndex(dMail) ;

   bool b = dCell == 3 && dMail != 3 ;

   FGauche -> setEnabled(b) ;
   FDroite -> setEnabled(b) ;
   FDevant -> setEnabled(b) ;
   FDerriere -> setEnabled(b) ;
   FBas -> setEnabled(b) ;
   FHaut -> setEnabled(b) ;

   FSaisieX->setEnabled(dMail != 0 && dCell > 0);
   FSaisieY->setEnabled(dMail != 0 && dCell > 1);
   FSaisieZ->setEnabled(dMail != 0 && dCell > 2);

   FModifie = true ;
}

// Dialogbox callbacks ---------------------------------------------------------
void CreationMaillage::callbackSubdivisions()
{
   if (FModifie)
      setSubdivisions(getSubdivisions(0),
                      getSubdivisions(1),
                      getSubdivisions(2));
}

void CreationMaillage::callbackToggleButton()
{
   if (FModifie)
      setCreatedFaces(getCreatedFaces());
}

void CreationMaillage::callbackDimCell()
{
   if (FModifie)
      setCellDimension(1 + FDimCell -> currentIndex()) ;
}

void CreationMaillage::callbackDimMesh()
{
   if (FModifie)
      setMeshDimension(FDimMail -> currentIndex()) ;
}

// Accesseurs
//-----------
int CreationMaillage::getSubdivisions(int i) const
{
   switch (i)
   {
      case 0 :
         return FSaisieX -> value() ;
         break ;
      case 1:
         return FSaisieY -> value() ;
         break ;
      case 2 :
         return FSaisieZ -> value() ;
         break ;
   }
   return -1 ;
}

uchar CreationMaillage::getCreatedFaces() const
{
   uchar result = 0;

   if (FGauche -> isChecked())
      result |= (1 << 0) ;
   if (FDroite -> isChecked())
      result |= (1 << 1) ;
   if (FDevant -> isChecked())
      result |= (1 << 2) ;
   if (FDerriere -> isChecked())
      result |= (1 << 3) ;
   if (FBas -> isChecked())
      result |= (1 << 4) ;
   if (FHaut -> isChecked())
      result |= (1 << 5) ;

   return result;
}

void CreationMaillage :: setSubdivisions(int x , int y , int z)
{
   FControler ->getParameterCreation()->setMeshNbSubdivisionsX(x);
   FControler ->getParameterCreation()->setMeshNbSubdivisionsY(y);
   FControler ->getParameterCreation()->setMeshNbSubdivisionsZ(z);

   update() ;
   repaint() ;
}

void CreationMaillage :: setCreatedFaces(uchar c)
{
   FControler -> getParameterCreation()->setMeshCreatedFaces(c);
   update() ;
   repaint() ;
}

void CreationMaillage::setCellDimension(int dim)
{
   FControler -> getParameterCreation()->setMeshDimension(dim);
   update();
   repaint() ;
}

void CreationMaillage :: setMeshDimension(int dim)
{
   FControler->getParameterCreation()->setMeshSubdivisionDimension(dim);
   update() ;
   repaint() ;
}
