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

/*********************************************/
/* CLASSE dialogTranslation                  */
/*********************************************/
#include "dialog-translation.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QPixmap>

// Constructeur
//-------------
dialogTranslation :: dialogTranslation(Window * parent ,
                                       GMap3d :: CControlerGMap * controler)
      :
      dialogOperations(parent , new QString("Translation") ,
                       controler , Qt::BottomToolBarArea)

{
   QIcon p1(":/boutonReinit.png") ;
   FReinit = new QPushButton(p1 , *HTML::decode("Reset")) ;
   FSaisieTranslate = 
         new QCheckBox(*HTML::decode("Weighted Translation")) ;

   QIcon p2(":/boutonAnnuler.png") ;
   FClose = new QPushButton(p2 , "Close") ;
   FClose -> setShortcut(Qt :: Key_Space) ;

   FSaisie = new champsOperations(this) ;

   addWidget(FReinit);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));
   addWidget(FSaisieTranslate);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));
   addWidget(FClose);

   update() ;

   connect(FSaisie -> getSaisieX() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackVx())) ;

   connect(FSaisie -> getSaisieY() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackVy())) ;

   connect(FSaisie -> getSaisieZ() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackVz())) ;

   connect(FSaisie ->  getButtonRecuperer() , SIGNAL(clicked()) ,
           this, SLOT(callbackTakeVector())) ;

   connect(FReinit , SIGNAL(clicked()) ,
           this , SLOT(callbackReinitVector())) ;

   connect(FSaisieTranslate , SIGNAL(clicked()) ,
           this , SLOT(callbackTogglePonderation())) ;

   connect(FClose , SIGNAL(clicked()) ,
           this , SLOT(callbackClose())) ;
}

// Destructeur
//------------
dialogTranslation :: ~dialogTranslation()
{
}
// Mise a jour de l'affichage
//---------------------------
void dialogTranslation :: update()
{
   FUpdate = true ;
   dialogOperations :: update() ;

   FSaisie->setValueX(getVx());
   FSaisie->setValueY(getVy());
   FSaisie->setValueZ(getVz());

   FSaisieTranslate->setChecked(getControler()
                                -> getObjectTranslationPonderation()) ;

   FParent -> repaint() ;
   FUpdate = false ;
}

// SLOTS
//------
void dialogTranslation :: callbackVx()
{
   if (!FUpdate)
      setVx(FSaisie -> getValueX()) ;
}

void dialogTranslation :: callbackVy()
{
   if (!FUpdate)
      setVy(FSaisie -> getValueY()) ;
}

void dialogTranslation :: callbackVz()
{
   if (!FUpdate)
      setVz(FSaisie -> getValueZ()) ;
}

void dialogTranslation :: callbackTakeVector()
{
   assert(FParent != NULL);
   CVertex vector;

   if (getControler() -> takeVector(vector))
   {
      setV(getV() + vector) ;
      FParent -> repaint() ;
   }
}

void dialogTranslation :: callbackReinitVector()
{
   setV(ORIGIN) ;
}

void dialogTranslation :: callbackTogglePonderation()
{
   getControler() -> toggleObjectTranslationPonderation() ;
   FParent -> repaint() ;
}

// Accesseurs en lecture
//----------------------
TCoordinate dialogTranslation :: getVx() const
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectTranslationVector() . getX() ;
}

TCoordinate dialogTranslation :: getVy() const
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectTranslationVector() . getY() ;
}

TCoordinate dialogTranslation :: getVz() const
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectTranslationVector() . getZ() ;
}

CVertex dialogTranslation :: getV()
{
   assert(FParent != NULL) ;
   return getControler() -> getObjectTranslationVector() ;
}

// Accesseurs en ecriture
//-----------------------
void dialogTranslation :: setVx(TCoordinate k)
{
   setV(CVertex(k , getVy() , getVz())) ;
}

void dialogTranslation :: setVy(TCoordinate k)
{
   setV(CVertex(getVx() , k , getVz())) ;
}
void dialogTranslation :: setVz(TCoordinate k)
{
   setV(CVertex(getVx() , getVy() , k)) ;
}

void dialogTranslation :: setV(const CVertex & V)
{
   assert(FParent != NULL) ;
   if (V == getV())
      return;

   getControler() -> setObjectTranslationVector(V) ;
   update() ;
}
