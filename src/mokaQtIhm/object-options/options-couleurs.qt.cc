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

#include "options-couleurs.qt.hh"
#include "window.qt.hh"
#include "SpaceWidget.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QPixmap>
#include <QtGui/QColorDialog>
#include <Qt3Support/Q3Accel>

//**********************************************************
// Elements dont on peut modifier la couleur
//**********************************************************
static QString elementLabs [ 27 ] =
{
   "Faces",
   "Background",
   "Unselected darts",
   "Selected darts",
   "Last selected dart",
   "0-removed darts",
   "1-removed darts",
   "Vertices",
   "0-sews",
   "1-sews",
   "2-sews",
   "3-sews",
   "Grid",
   "Ox Axis",
   "Oy Axis",
   "Oz Axis",
   "Origin",
   "Normal vector",
   "Transformation",
   "Preview",
   "Preview2",
} ;

//************************************************************
// Constructeur
//************************************************************
OptionCouleurs :: OptionCouleurs(Window * parent)
      :
      QToolBar("Colors", parent),
      FParent(parent),
      FUpdate(false)
{
   setVisible(false) ;
   parent -> addToolBar(Qt::BottomToolBarArea, this);
   setAllowedAreas(Qt::BottomToolBarArea | Qt::TopToolBarArea);

   // Construction des widgets
   FModifiables = new QComboBox(this) ;
#ifdef MODULE_ARCHITECTURE
   for (int i = 0 ;  i < 26 ; i++)
#else
   for (int i = 0 ;  i < 19 ; i++)
#endif
      FModifiables -> addItem(elementLabs [ i ]) ;

   FCouleur = new QPushButton(this) ;
   FCouleur -> setMinimumSize(30 , 20) ;
   FCouleur -> setMaximumSize(30 , 20) ;

   FEpaisseur = new QSpinBox(this) ;
   FEpaisseur -> setMinimum(1);
   FEpaisseur -> setMaximum(10);
   FEpaisseur -> setAccelerated(true);

   FTransparence = new FloatSpinBox(0.1 , 0.0 , 1.0) ;

   FFin = new QPushButton(QIcon(":/boutonAnnuler.png"), "Close" , this) ;

   // Mise en place des ecoutes
   QObject :: connect(FCouleur , SIGNAL(clicked()) , this ,
                      SLOT(openDialog())) ;
   QObject :: connect(FFin , SIGNAL(clicked()) , this ,
                      SLOT(close())) ;
   QObject :: connect(FEpaisseur , SIGNAL(valueChanged(int)) , this ,
                      SLOT(callbackEpaisseur())) ;
   QObject :: connect(FTransparence , SIGNAL(valueChanged(double)) , this ,
                      SLOT(callbackTransparence())) ;
   QObject :: connect(FModifiables , SIGNAL(activated(int)) , this ,
                      SLOT(changeColor())) ;

   Q3Accel * Raccourci = new Q3Accel(this) ;
   Raccourci -> insertItem(QKeySequence(Qt::Key_Space) , 2) ;
   Raccourci -> connectItem(2 , this , SLOT(close())) ;

   addWidget(new SpaceWidget(15, 1));
   addWidget(FModifiables);
   addWidget(new SpaceWidget(15, 1));
   addWidget(FCouleur);
   addWidget(new SpaceWidget(15, 1));
   addWidget(FEpaisseur);
   addWidget(new SpaceWidget(5, 1));
   addWidget(FTransparence);
   addWidget(new SpaceWidget(15, 1));
   addWidget(FFin);
}

//******************************************************
// Destructeur
//******************************************************
OptionCouleurs :: ~OptionCouleurs() { }

//******************************************************
// SLOTS
//******************************************************
void OptionCouleurs :: openDialog()
{
   QColor choix = QColorDialog :: getColor() ;

   // On regarde si l'utilisateur n'a pas annule
   if (choix . isValid())
   {
      int r , g , b ;
      choix . getRgb(&r , &g , &b) ;
      setR(r) ;
      setG(g) ;
      setB(b) ;
      FParent -> repaint() ;
      colorButton() ;
   }
}

void OptionCouleurs :: close()
{
   setVisible(false) ;
}

void OptionCouleurs :: show()
{
   setVisible(true) ;
   changeColor();
}

void OptionCouleurs :: changeColor()
{
   colorButton() ;
   griseBoutons() ;
}

void OptionCouleurs :: colorButton()
{
   float color[3] = { 0.0, 0.0, 0.0 };
   FUpdate = true ;
   TViewId FLastClikedViewId = FParent ->getCurrentViewId() ;
   switch (FModifiables -> currentIndex())
   {
      case _FACE :
         color[0] = FParent->getControler() ->
                    getParameterFace(FLastClikedViewId) -> getCLFace(0) ;
         color[1] = (FParent->getControler()->
                     getParameterFace(FLastClikedViewId)->getCLFace(1));
         color[2] = (FParent->getControler()->
                     getParameterFace(FLastClikedViewId)->getCLFace(2));
         FTransparence -> setValuef(FParent->getControler()->
                                    getParameterFace(FLastClikedViewId)->
                                    getBLFace());
         break;
      case _FOND :
         color[0] = (FParent->getControler()->
                     getParameterDrawing(FLastClikedViewId)->
                     getCLBackground(0));
         color[1] = (FParent->getControler()->
                     getParameterDrawing(FLastClikedViewId)->
                     getCLBackground(1));
         color[2] = (FParent->getControler()->
                     getParameterDrawing(FLastClikedViewId)->
                     getCLBackground(2));
         break;
      case _UNSEL_DART :
         color[0] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCLUnsel(FParent->getControler()->getSelectionLevel(),
                                0));
         color[1] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCLUnsel(FParent->getControler()->getSelectionLevel(),
                                1));
         color[2] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCLUnsel(FParent->getControler()->getSelectionLevel(),
                                2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterDart(FLastClikedViewId)->
                              getLWDarts());
         break;
      case _SEL_DART :
         color[0] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCLSel(FParent->getControler()->getSelectionLevel(),
                              0));
         color[1] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCLSel(FParent->getControler()->getSelectionLevel(),
                              1));
         color[2] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCLSel(FParent->getControler()->getSelectionLevel(),
                              2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterDart(FLastClikedViewId)->
                              getLWDarts());
         break;
      case _LAST_DART :
         color[0] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCLLastSel(FParent->getControler()->getSelectionLevel(),
                                  0));
         color[1] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCLLastSel(FParent->getControler()->getSelectionLevel(),
                                  1));
         color[2] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCLLastSel(FParent->getControler()->getSelectionLevel(),
                                  2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterDart(FLastClikedViewId)->
                              getLWDarts());
         break;
      case _0DELETE_DART :
         color[0] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCL0Remove(FParent->getControler()->getSelectionLevel(),
                                0));
         color[1] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCL0Remove(FParent->getControler()->getSelectionLevel(),
                                1));
         color[2] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCL0Remove(FParent->getControler()->getSelectionLevel(),
                                2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterDart(FLastClikedViewId)->
                              getLWDarts());
         break;
      case _1DELETE_DART :
         color[0] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCL1Remove(FParent->getControler()->getSelectionLevel(),
                                0));
         color[1] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCL1Remove(FParent->getControler()->getSelectionLevel(),
                                1));
         color[2] = (FParent->getControler()->
                     getParameterDart(FLastClikedViewId)->
                     getCL1Remove(FParent->getControler()->getSelectionLevel(),
                                2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterDart(FLastClikedViewId)->
                              getLWDarts());
         break;
      case _VERTEX :
         color[0] = (FParent->getControler()->
                     getParameterVertex(FLastClikedViewId)->
                     getCLVertex(0));
         color[1] = (FParent->getControler()->
                     getParameterVertex(FLastClikedViewId)->
                     getCLVertex(1));
         color[2] = (FParent->getControler()->
                     getParameterVertex(FLastClikedViewId)->
                     getCLVertex(2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterVertex(FLastClikedViewId)->
                              getSVertex());
         break;
      case _0_SEW :
         color[0] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(0, 0));
         color[1] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(0, 1));
         color[2] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(0, 2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterSew(FLastClikedViewId)->
                              getLWSew(0));
         break;
      case _1_SEW :
         color[0] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(1, 0));
         color[1] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(1, 1));
         color[2] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(1, 2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterSew(FLastClikedViewId)->
                              getLWSew(1));
         break;
      case _2_SEW :
         color[0] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(2, 0));
         color[1] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(2, 1));
         color[2] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(2, 2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterSew(FLastClikedViewId)->
                              getLWSew(2));
         break;
      case _3_SEW :
         color[0] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(3, 0));
         color[1] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(3, 1));
         color[2] = (FParent->getControler()->
                     getParameterSew(FLastClikedViewId)->
                     getCLSew(3, 2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterSew(FLastClikedViewId)->
                              getLWSew(3));
         break;
      case _GRID :
         color[0] = (FParent->getControler()->
                     getParameterGrid(FLastClikedViewId)->getCLGrid(0));
         color[1] = (FParent->getControler()->
                     getParameterGrid(FLastClikedViewId)->getCLGrid(1));
         color[2] = (FParent->getControler()->
                     getParameterGrid(FLastClikedViewId)->getCLGrid(2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterGrid(FLastClikedViewId)->
                              getLWGrid());
         break;
      case _OX_AXIS :
         color[0] = (FParent->getControler()->
                     getParameterAxis(FLastClikedViewId)->
                     getCLAxisX(0));
         color[1] = (FParent->getControler()->
                     getParameterAxis(FLastClikedViewId)->
                     getCLAxisX(1));
         color[2] = (FParent->getControler()->
                     getParameterAxis(FLastClikedViewId)->
                     getCLAxisX(2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterAxis(FLastClikedViewId)->
                              getLWAxis());
         break;
      case _OY_AXIS :
         color[0] = (FParent->getControler()->
                     getParameterAxis(FLastClikedViewId)->
                     getCLAxisY(0));
         color[1] = (FParent->getControler()->
                     getParameterAxis(FLastClikedViewId)->
                     getCLAxisY(1));
         color[2] = (FParent->getControler()->
                     getParameterAxis(FLastClikedViewId)->
                     getCLAxisY(2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterAxis(FLastClikedViewId)->
                              getLWAxis());
         break;
      case _OZ_AXIS :
         color[0] = (FParent->getControler()->
                     getParameterAxis(FLastClikedViewId)->
                     getCLAxisZ(0));
         color[1] = (FParent->getControler()->
                     getParameterAxis(FLastClikedViewId)->
                     getCLAxisZ(1));
         color[2] = (FParent->getControler()->
                     getParameterAxis(FLastClikedViewId)->
                     getCLAxisZ(2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterAxis(FLastClikedViewId)->
                              getLWAxis());
         break;
      case _AIMED_POINT :
         color[0] = (FParent->getControler()->
                     getParameterAimedPoint(FLastClikedViewId)->
                     getCLAimedPoint(0));
         color[1] = (FParent->getControler()->
                     getParameterAimedPoint(FLastClikedViewId)->
                     getCLAimedPoint(1));
         color[2] = (FParent->getControler()->
                     getParameterAimedPoint(FLastClikedViewId)->
                     getCLAimedPoint(2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterAimedPoint(FLastClikedViewId)->
                              getLWAimedPoint());
         break;
      case _NORMAL_VECTOR :
         color[0] = (FParent->getControler()->
                     getParameterNormalVector(FLastClikedViewId)->
                     getCLNormalVector(0));
         color[1] = (FParent->getControler()->
                     getParameterNormalVector(FLastClikedViewId)->
                     getCLNormalVector(1));
         color[2] = (FParent->getControler()->
                     getParameterNormalVector(FLastClikedViewId)->
                     getCLNormalVector(2));
         FEpaisseur->setValue(FParent->getControler()->
                              getParameterNormalVector(FLastClikedViewId)->
                              getLWNormalVector());
         break;
      case _OBJECT_TRANSFORMATION :
         color[0] =
            (FParent->getControler()->
             getParameterObjectTransformation(FLastClikedViewId)->
             getCLObjectTransformation(0));
         color[1] =
            (FParent->getControler()->
             getParameterObjectTransformation(FLastClikedViewId)->
             getCLObjectTransformation(1));
         color[2] =
            (FParent->getControler()->
             getParameterObjectTransformation(FLastClikedViewId)->
             getCLObjectTransformation(2));
         FEpaisseur->
         setValue(FParent->getControler()->
                  getParameterObjectTransformation(FLastClikedViewId)->
                  getLWObjectTransformation());
         break;
      case _OBJECT_PREVIEW :
         color[0] =
            (FParent->getControler()->
             getParameterPreview(FLastClikedViewId)->
             getCLPreview(0));
         color[1] =
            (FParent->getControler()->
             getParameterPreview(FLastClikedViewId)->
             getCLPreview(1));
         color[2] =
            (FParent->getControler()->
             getParameterPreview(FLastClikedViewId)->
             getCLPreview(2));
         FEpaisseur->
         setValue(FParent->getControler()->
                  getParameterPreview(FLastClikedViewId)->
                  getLWPreview());
         break;
      case _OBJECT_PREVIEW_BAD :
         color[0] =
            (FParent->getControler()->
             getParameterPreview(FLastClikedViewId)->
             getCLPreviewBad(0));
         color[1] =
            (FParent->getControler()->
             getParameterPreview(FLastClikedViewId)->
             getCLPreviewBad(1));
         color[2] =
            (FParent->getControler()->
             getParameterPreview(FLastClikedViewId)->
             getCLPreviewBad(2));
         FEpaisseur->
         setValue(FParent->getControler()->
                  getParameterPreview(FLastClikedViewId)->
                  getLWPreview());
         break;
   }

   QPixmap * coul = new QPixmap(30 , 30) ;
   coul -> fill(QColor((int)(color[0]* 255) , (int)(color[1]*255) ,
                       (int)(color[2]*255))) ;
   FCouleur -> setIcon(QIcon(*coul));

   FUpdate = false ;
   FParent -> repaint() ;
}

void OptionCouleurs :: setColor(int num , float value)
{
   TViewId FLastClikedViewId = FParent -> getCurrentViewId() ;

   switch (FModifiables -> currentIndex())
   {
      case _FACE :
         FParent -> getControler()->
         getParameterFace(FLastClikedViewId)->setCLFace(num, value);
         break;
      case _FOND :
         FParent -> getControler()->getParameterDrawing(FLastClikedViewId)->
         setCLBackground(num, value);
         break;
      case _UNSEL_DART :
         FParent -> getControler()->getParameterDart(FLastClikedViewId)->
         setCLUnsel(FParent->getControler()->getSelectionLevel(), num, value);
         break;
      case _SEL_DART :
         FParent -> getControler()->getParameterDart(FLastClikedViewId)->
         setCLSel(FParent -> getControler()->getSelectionLevel(), num, value);
         break;
      case _LAST_DART :
         FParent -> getControler()->getParameterDart(FLastClikedViewId)->
         setCLLastSel(FParent -> getControler()->getSelectionLevel(),
                      num, value);
         break;
      case _0DELETE_DART :
         FParent -> getControler()->getParameterDart(FLastClikedViewId)->
         setCL0Remove(FParent -> getControler()->getSelectionLevel(),
                      num, value);
         break;
      case _1DELETE_DART :
         FParent -> getControler()->getParameterDart(FLastClikedViewId)->
         setCL1Remove(FParent -> getControler()->getSelectionLevel(),
                      num, value);
         break;
      case _VERTEX :
         FParent -> getControler()->
         getParameterVertex(FLastClikedViewId)->setCLVertex(num, value);
         break;
      case _0_SEW :
         FParent -> getControler()->
         getParameterSew(FLastClikedViewId)->setCLSew(0, num, value);
         break;
      case _1_SEW :
         FParent -> getControler()->
         getParameterSew(FLastClikedViewId)->setCLSew(1, num, value);
         break;
      case _2_SEW :
         FParent -> getControler()->
         getParameterSew(FLastClikedViewId)->setCLSew(2, num, value);
         break;
      case _3_SEW :
         FParent -> getControler()->
         getParameterSew(FLastClikedViewId)->setCLSew(3, num, value);
         break;
      case _GRID :
         FParent -> getControler()->
         getParameterGrid(FLastClikedViewId)->setCLGrid(num, value);
         break;
      case _OX_AXIS :
         FParent -> getControler()->
         getParameterAxis(FLastClikedViewId)->setCLAxisX(num, value);
         break;
      case _OY_AXIS :
         FParent -> getControler()->
         getParameterAxis(FLastClikedViewId)->setCLAxisY(num, value);
         break;
      case _OZ_AXIS :
         FParent -> getControler()->
         getParameterAxis(FLastClikedViewId)->setCLAxisZ(num, value);
         break;
      case _AIMED_POINT :
         FParent -> getControler()->
         getParameterAimedPoint(FLastClikedViewId)->
         setCLAimedPoint(num, value);
         break;
      case _NORMAL_VECTOR :
         FParent -> getControler()->
         getParameterNormalVector(FLastClikedViewId)->
         setCLNormalVector(num, value);
         break;
      case _OBJECT_TRANSFORMATION :
         FParent -> getControler()->
         getParameterObjectTransformation(FLastClikedViewId)->
         setCLObjectTransformation(num, value);
         break;
      case _OBJECT_PREVIEW :
         FParent -> getControler()->getParameterPreview(FLastClikedViewId)->
         setCLPreview(num, value);
         break;
      case _OBJECT_PREVIEW_BAD :
         FParent -> getControler()->getParameterPreview(FLastClikedViewId)->
         setCLPreviewBad(num, value);
         break;
   }
}

void OptionCouleurs :: setR(int r)
{
   setColor(0 , ((float) r) / 255);
}

void OptionCouleurs :: setG(int g)
{
   setColor(1 , ((float) g) / 255) ;
}

void OptionCouleurs :: setB(int b)
{
   setColor(2 , ((float) b) / 255) ;
}

void OptionCouleurs :: callbackEpaisseur()
{
   if (! FUpdate)
   {
      TViewId FLastClikedViewId = FParent->getCurrentViewId() ;
      int w = FEpaisseur -> value() ;

      switch (FModifiables -> currentIndex())
      {
         case _UNSEL_DART :
         case _SEL_DART :
         case _LAST_DART :
            FParent -> getControler()->
            getParameterDart(FLastClikedViewId)->setLWDarts(w);
            break;
         case _VERTEX :
            FParent -> getControler()->
            getParameterVertex(FLastClikedViewId)->setSVertex(w);
            break;
         case _0_SEW :
            FParent -> getControler()->
            getParameterSew(FLastClikedViewId)->setLWSew(0, w);
            break;
         case _1_SEW :
            FParent -> getControler()->
            getParameterSew(FLastClikedViewId)->setLWSew(1, w);
            break;
         case _2_SEW :
            FParent -> getControler()->
            getParameterSew(FLastClikedViewId)->setLWSew(2, w);
            break;
         case _3_SEW :
            FParent -> getControler()->
            getParameterSew(FLastClikedViewId)->setLWSew(3, w);
            break;
         case _GRID :
            FParent -> getControler()->
            getParameterGrid(FLastClikedViewId)->setLWGrid(w);
            break;
         case _OX_AXIS :
            FParent -> getControler()->
            getParameterAxis(FLastClikedViewId)->setLWAxis(w);
            break;
         case _OY_AXIS :
            FParent -> getControler()->
            getParameterAxis(FLastClikedViewId)->setLWAxis(w);
            break;
         case _OZ_AXIS :
            FParent -> getControler()->
            getParameterAxis(FLastClikedViewId)->setLWAxis(w);
            break;
         case _AIMED_POINT :
            FParent -> getControler()->
            getParameterAimedPoint(FLastClikedViewId)->setLWAimedPoint(w);
            break;
         case _NORMAL_VECTOR :
            FParent -> getControler()->
            getParameterNormalVector(FLastClikedViewId)->
            setLWNormalVector(w);
            break;
         case _OBJECT_TRANSFORMATION :
            FParent -> getControler()->
            getParameterObjectTransformation(FLastClikedViewId)->
            setLWObjectTransformation(w);
            break;
         case _OBJECT_PREVIEW :
         case _OBJECT_PREVIEW_BAD :
            FParent -> getControler()->getParameterPreview(FLastClikedViewId)
            ->setLWPreview(w);
            break;
         default:
            break;
      }
      FParent -> repaint() ;
   }
}

void OptionCouleurs :: callbackTransparence()
{
   if (! FUpdate)
   {
      float a = FTransparence -> valuef() ;
      TViewId FLastClikedViewId = FParent -> getCurrentViewId() ;
      switch (FModifiables -> currentIndex())
      {
         case _FACE :
            FParent -> getControler() ->
            getParameterFace(FLastClikedViewId)->setBLFace(a);
            break;
         default:
            break;
      }
      FParent -> repaint() ;
   }
}

void OptionCouleurs :: griseBoutons()
{
   FEpaisseur -> setEnabled(false) ;
   FTransparence -> setEnabled(false) ;

   switch (FModifiables -> currentIndex())
   {
      case _FACE :
         FTransparence -> setEnabled(true) ;
         break ;
      case _UNSEL_DART :
      case _SEL_DART :
      case _LAST_DART :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _VERTEX :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _0_SEW :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _1_SEW :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _2_SEW :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _3_SEW :

         FEpaisseur -> setEnabled(true) ; break;
      case _GRID :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _OX_AXIS :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _OY_AXIS :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _OZ_AXIS :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _AIMED_POINT :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _NORMAL_VECTOR :
         FEpaisseur -> setEnabled(true) ;
         break;
      case _OBJECT_TRANSFORMATION :

         FEpaisseur -> setEnabled(true) ;
         break;
      case _OBJECT_PREVIEW :
      case _OBJECT_PREVIEW_BAD :
         FEpaisseur -> setEnabled(true) ;
         break;
      default:
         break ;
   }
}
