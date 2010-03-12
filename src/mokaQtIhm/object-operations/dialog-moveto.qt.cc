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
/* CLASSE dialogMoveto                       */
/*********************************************/
#include "dialog-moveto.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QPixmap>

// Constructeur
//-------------
dialogMoveto:: dialogMoveto(Window * parent ,
                                       GMap3d :: CControlerGMap * controler)
      :
      dialogOperations(parent , new QString("MoveTo") ,
                       controler , Qt::BottomToolBarArea),
      FSelectedDart(NULL)

{
   QIcon p1(":/boutonReinit.png") ;
   FReinit = new QPushButton(p1 , *HTML::decode("Reset")) ;

   QIcon p2(":/boutonAnnuler.png") ;
   FClose = new QPushButton(p2 , "Close") ;
   FClose -> setShortcut(Qt :: Key_Space) ;

   FSaisie = new champsOperations(this,NULL,false) ;
   
   addWidget(FReinit);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));
   addWidget(FClose);

   update() ;

   connect(FSaisie -> getSaisieX() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackXPos())) ;

   connect(FSaisie -> getSaisieY() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackYPos())) ;

   connect(FSaisie -> getSaisieZ() , SIGNAL(valueChanged(double)) ,
           this , SLOT(callbackZPos())) ;

   connect(FReinit , SIGNAL(clicked()) ,
           this , SLOT(callbackReinitPos())) ;

   connect(FClose , SIGNAL(clicked()) ,
           this , SLOT(callbackClose())) ;
}

// Destructeur
//------------
dialogMoveto :: ~dialogMoveto()
{
}
// Mise a jour de l'affichage
//---------------------------
void dialogMoveto :: update()
{
   FUpdate = true ;
   dialogOperations :: update() ;
   
   if ( FSelectedDart!=getControler()->getLastSelectedDart() )
     {
       FSelectedDart = getControler()->getLastSelectedDart();
       if ( FSelectedDart!=NULL )
	 FInitialPos = getControler()->getVertexPosition();
     }   
   
   if (FSelectedDart!=NULL)
     {
       FSaisie->getSaisieX()->setEnabled(true);
       FSaisie->getSaisieY()->setEnabled(true);
       FSaisie->getSaisieZ()->setEnabled(true);
       
       FSaisie->setValueX(getXPos()); 
       FSaisie->setValueY(getYPos()); 
       FSaisie->setValueZ(getZPos()); 
     }
   else
     {
        FSaisie->getSaisieX()->setEnabled(false);
	FSaisie->getSaisieY()->setEnabled(false);
	FSaisie->getSaisieZ()->setEnabled(false);	
     }
   
   FParent -> repaint() ;

   FUpdate = false ;
}

// SLOTS
//------
void dialogMoveto :: callbackXPos()
{
   if (!FUpdate)
      setXPos(FSaisie -> getValueX()) ;
}

void dialogMoveto :: callbackYPos()
{
   if (!FUpdate)
      setYPos(FSaisie -> getValueY()) ;
}

void dialogMoveto :: callbackZPos()
{
   if (!FUpdate)
      setZPos(FSaisie -> getValueZ()) ;
}

void dialogMoveto :: callbackReinitPos()
{
   setPos(FInitialPos) ;
}

// Accesseurs en lecture
//----------------------
TCoordinate dialogMoveto :: getXPos() const
{
   assert(FParent != NULL) ;
   return getControler()->getVertexPosition().getX() ;
}

TCoordinate dialogMoveto :: getYPos() const
{
   assert(FParent != NULL) ;
   return getControler()->getVertexPosition().getY() ;
}

TCoordinate dialogMoveto :: getZPos() const
{
   assert(FParent != NULL) ;
   return getControler()->getVertexPosition().getZ() ;
}

CVertex dialogMoveto :: getPos()
{
   assert(FParent != NULL) ;
   return getControler()->getVertexPosition() ;
}

// Accesseurs en ecriture
//-----------------------
void dialogMoveto :: setXPos(TCoordinate k)
{
   setPos(CVertex(k , getYPos() , getZPos())) ;
}

void dialogMoveto :: setYPos(TCoordinate k)
{
   setPos(CVertex(getXPos() , k , getZPos())) ;
}
void dialogMoveto :: setZPos(TCoordinate k)
{
   setPos(CVertex(getXPos() , getYPos() , k)) ;
}

void dialogMoveto :: setPos(const CVertex & AVertex)
{
   assert(FParent != NULL) ;
   if (AVertex == getPos())
      return;

   getControler() -> setVertexPosition(AVertex) ;
   update() ;
}
