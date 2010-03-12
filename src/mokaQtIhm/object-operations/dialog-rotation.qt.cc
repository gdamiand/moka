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

#include "dialog-rotation.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QPixmap>
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QGridLayout>

// Constructeur
//-------------
dialogRotation :: dialogRotation(Window * parent ,
                                 GMap3d :: CControlerGMap * controler)
      :
      dialogOperations(parent , new QString("Rotation") ,
                       controler , Qt::BottomToolBarArea)
{

   /**** Partie haute ****/
   QWidget * haut = new QWidget ;
   QHBoxLayout * bb = new QHBoxLayout(haut) ;
   bb -> setSpacing(0);
   bb -> setContentsMargins(0, 0, 0, 0);

   FAffSaisie = new QLabel("  Center: ") ;
   FAffAngle = new QLabel("Angle: ") ;
   FSaisieAngle = new FloatSpinBox(5 , -180.0 , 180.0 , 0 , 5.0);
   FSaisieAngle -> setWrapping(true) ;
   QIcon p3(":/boutonReinit.png") ;
   FReinitAngle =
      new QPushButton(p3, *HTML::decode("Reset angle")) ;

   // Ajout du layout haut
   FSaisie = new champsOperations(this , bb) ;
   bb -> addSpacing(SPACE_PIX_SIZE) ;
   bb -> addWidget(FAffAngle) ;
   bb -> addWidget(FSaisieAngle) ;
   bb -> addSpacing(SPACE_PIX_SIZE) ;
   bb -> addWidget(FReinitAngle) ;
   bb -> addStretch(1);

   /*** Partie basse ***/
   QWidget * bas = new QWidget ;
   QHBoxLayout * bh = new QHBoxLayout(bas) ;
   bh -> setSpacing(0);
   bh -> setContentsMargins(0, 0, 0, 0);

   FAffAxe = new QLabel(" Axis Vector: ") ;
   FPonderation = new QCheckBox
   (*HTML::decode("Weighted Rotation")) ;
   QIcon p2(":/boutonAnnuler.png") ;
   FClose = new QPushButton(p2 , "Close") ;
   FClose -> setShortcut(Qt :: Key_Space) ;

   // Ajout au layout bas
   FSaisieAxe = new champsOperations(this, bh) ;
   bh -> addSpacing(SPACE_PIX_SIZE) ;
   bh -> addWidget(FPonderation) ;
   bh -> addSpacing(SPACE_PIX_SIZE) ;
   bh -> addWidget(FClose) ;
   bh -> addStretch(1);

   // Ajout au layout principal
   QWidget * centre = new QWidget ;
   QGridLayout * gl = new QGridLayout(centre);

   gl -> setHorizontalSpacing(0);
   gl -> setVerticalSpacing(2);
   gl -> setContentsMargins(0, 0, 0, 0);
   gl -> addWidget(FAffSaisie, 0, 0, Qt::AlignRight) ;
   gl -> addWidget(FAffAxe, 1, 0, Qt::AlignRight) ;
   gl -> addWidget(haut, 0, 1) ;
   gl -> addWidget(bas, 1, 1) ;

   addWidget(centre);

   update() ;

   connect(FSaisie -> getSaisieX() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackCx())) ;

   connect(FSaisie -> getSaisieY() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackCy())) ;

   connect(FSaisie -> getSaisieZ() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackCz())) ;

   connect(FSaisie ->  getButtonRecuperer() , SIGNAL(clicked()) ,
           this, SLOT(callbackTakeCenter())) ;

   connect(FSaisieAngle , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackA())) ;

   connect(FReinitAngle , SIGNAL(clicked()) ,
           this , SLOT(callbackReinitAngle())) ;

   connect(FSaisieAxe -> getSaisieX() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackVx())) ;

   connect(FSaisieAxe -> getSaisieY() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackVy())) ;

   connect(FSaisieAxe -> getSaisieZ() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackVz())) ;

   connect(FSaisieAxe ->  getButtonRecuperer() , SIGNAL(clicked()) ,
           this, SLOT(callbackTakeVector())) ;

   connect(FPonderation , SIGNAL(clicked()) ,
           this , SLOT(callbackTogglePonderation())) ;

   connect(FClose , SIGNAL(clicked()) ,
           this , SLOT(callbackClose())) ;
}

// Destructeur
//------------
dialogRotation :: ~dialogRotation()
{}
// Mise a jour de l'affichage
//---------------------------
void dialogRotation :: update()
{
   FUpdate = true ;

   dialogOperations :: update() ;

   assert(FParent != NULL) ;

   FPonderation -> setChecked(getControler()
                              -> getObjectRotationPonderation()) ;

   CVertex V = getControler() -> getObjectRotationAxeVertex() ;
   FSaisie -> setValueX(V.getX()) ;
   FSaisie -> setValueY(V.getY()) ;
   FSaisie -> setValueZ(V.getZ()) ;

   V = getControler()->getObjectRotationAxeVector();

   FSaisieAxe -> setValueX(V.getX()) ;
   FSaisieAxe -> setValueY(V.getY()) ;
   FSaisieAxe -> setValueZ(V.getZ()) ;

   FSaisieAngle -> setValuef(getControler() ->
                             getObjectRotationAngle()) ;

   FParent -> repaint() ;
   FUpdate = false ;
}

void dialogRotation :: callbackCx()
{
   if (! FUpdate)
      setCx(FSaisie -> getValueX()) ;
}

void dialogRotation :: callbackCy()
{
   if (! FUpdate)
      setCy(FSaisie -> getValueY()) ;
}

void dialogRotation :: callbackCz()
{
   if (! FUpdate)
      setCz(FSaisie -> getValueZ()) ;
}

void dialogRotation :: callbackVx()
{
   if (! FUpdate)
      setVx(FSaisieAxe -> getValueX())  ;
}

void dialogRotation :: callbackVy()
{
   if (! FUpdate)
      setVy(FSaisieAxe -> getValueY()) ;
}

void dialogRotation :: callbackVz()
{
   if (! FUpdate)
      setVz(FSaisieAxe -> getValueZ()) ;
}

void dialogRotation :: callbackA()
{
   if (! FUpdate)
      setA(FSaisieAngle -> valuef()) ;
}

void dialogRotation :: callbackTakeCenter()
{
   assert(FParent != NULL) ;
   CVertex center;
   if (getControler() -> takeCenter(center))
      setC(center) ;
}

void dialogRotation :: callbackTakeVector()
{
   assert(FParent != NULL) ;
   CVertex vector;
   if (getControler() -> takeVector(vector))
      setV(vector) ;
}

void dialogRotation :: callbackReinitAngle()
{
   if (! FUpdate)
      setA(0) ;
}

void dialogRotation :: callbackTogglePonderation()
{
   if (! FUpdate)
   {
      getControler() -> toggleObjectRotationPonderation() ;
      FParent -> repaint() ;
   }
}

// Accesseurs en lecture
//----------------------
TCoordinate dialogRotation :: getCx() const
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectRotationAxeVertex() . getX() ;
}

TCoordinate dialogRotation :: getCy() const
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectRotationAxeVertex() . getY() ;
}

TCoordinate dialogRotation :: getCz() const
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectRotationAxeVertex() . getZ() ;
}

TCoordinate dialogRotation :: getVx() const
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectRotationAxeVector() . getX() ;
}

TCoordinate dialogRotation :: getVy() const
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectRotationAxeVector() . getY() ;
}

TCoordinate dialogRotation :: getVz() const
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectRotationAxeVector() . getZ() ;
}

// Accesseurs en ecriture
//------------------------
void dialogRotation :: setCx(TCoordinate k)
{
   setC(CVertex(k , getCy() , getCz())) ;
}

void dialogRotation :: setCy(TCoordinate k)
{
   setC(CVertex(getCx() , k , getCz())) ;
}

void dialogRotation :: setCz(TCoordinate k)
{
   setC(CVertex(getCz() , getCy() , k)) ;
}

void dialogRotation :: setVx(TCoordinate k)
{
   setV(CVertex(k , getVy() , getVz())) ;
}

void dialogRotation :: setVy(TCoordinate k)
{
   setV(CVertex(getVx() , k , getVz())) ;
}

void dialogRotation :: setVz(TCoordinate k)
{
   setV(CVertex(getVx() , getVy() , k)) ;
}

void dialogRotation :: setC(const CVertex & C)
{
   assert(FParent != NULL) ;
   if (C == getControler() -> getObjectRotationAxeVertex())
      return;

   getControler() -> setObjectRotationAxeVertex(C) ;
   update() ;
}

void dialogRotation :: setV(const CVertex & V)
{
   assert(FParent != NULL) ;
   if (V == getControler() -> getObjectRotationAxeVector())
      return;

   getControler() -> setObjectRotationAxeVector(V) ;
   update() ;
}

void dialogRotation :: setA(TCoordinate A)
{
   assert(FParent != NULL) ;

   A = limit(A) ;

   if (A == getControler() -> getObjectRotationAngle())
      return;

   getControler() -> setObjectRotationAngle(A) ;
   update() ;
}
