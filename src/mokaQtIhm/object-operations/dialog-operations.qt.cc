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

#include "dialog-operations.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QLabel>

/*******************************************************************/
/* CLASSE champsOperations                                         */
/*******************************************************************/

// Constructeur
//-------------
champsOperations :: champsOperations(QToolBar * parent ,
				     QBoxLayout *  Layout,
				     bool WithRecuperer)
      :
      QWidget(parent),
      FParent(parent)
{
   FAffX = new QLabel("  X : ") ;
   FSaisieX = new FloatSpinBox(0.5) ;
   FSaisieX -> setWrapping(true) ;

   FAffY = new QLabel("  Y : ") ;
   FSaisieY = new FloatSpinBox(0.5) ;
   FSaisieY -> setWrapping(true) ;

   FAffZ = new QLabel("  Z : ") ;
   FSaisieZ = new FloatSpinBox(0.5) ;
   FSaisieZ -> setWrapping(true) ;

   if ( WithRecuperer )
     FRecuperer = new QPushButton(*HTML::decode("Recover")) ;
   else
     FRecuperer = NULL;
   
   if (Layout != NULL)
   {
      Layout -> addWidget(FAffX);
      Layout -> addWidget(FSaisieX);
      Layout -> addWidget(FAffY);
      Layout -> addWidget(FSaisieY);
      Layout -> addWidget(FAffZ);
      Layout -> addWidget(FSaisieZ);
      Layout -> addSpacing(8);
      if ( FRecuperer!=NULL ) Layout -> addWidget(FRecuperer);
   }
   else
   {
      parent -> addWidget(FAffX);
      parent -> addWidget(FSaisieX);
      parent -> addWidget(FAffY);
      parent -> addWidget(FSaisieY);
      parent -> addWidget(FAffZ);
      parent -> addWidget(FSaisieZ);
      parent -> addWidget(new SpaceWidget(8, 8));
      if ( FRecuperer!=NULL ) parent -> addWidget(FRecuperer);
   }
}

// Destructeur
//------------
champsOperations:: ~champsOperations()
{}

// Accesseurs en lecture
//----------------------
QPushButton * champsOperations :: getButtonRecuperer()
{
   return FRecuperer ;
}

FloatSpinBox * champsOperations :: getSaisieX()
{
   return FSaisieX ;
}

FloatSpinBox * champsOperations :: getSaisieY()
{
   return FSaisieY ;
}

FloatSpinBox * champsOperations :: getSaisieZ()
{
   return FSaisieZ ;
}

QLabel * champsOperations :: getLabelX()
{
   return FAffX ;
}
QLabel * champsOperations :: getLabelY()
{
   return FAffY ;
}
QLabel * champsOperations :: getLabelZ()
{
   return FAffZ ;
}

float champsOperations :: getValueX() const
{
   return FSaisieX -> valuef() ;
}

float champsOperations :: getValueY() const
{
   return FSaisieY -> valuef() ;
}

float champsOperations :: getValueZ() const
{
   return FSaisieZ -> valuef() ;
}

// Accesseurs en ecriture
//-----------------------
void champsOperations :: setValueX(float X)
{
   FSaisieX -> setValuef(X) ;
}

void champsOperations :: setValueY(float Y)
{
   FSaisieY -> setValuef(Y) ;
}

void champsOperations :: setValueZ(float Z)
{
   FSaisieZ -> setValuef(Z) ;
}

/*******************************************************************/
/* CLASSE dialogOperations                                         */
/*******************************************************************/
// Constructeur
//-------------
dialogOperations :: dialogOperations(Window * parent ,
                                     QString * nomOperation ,
                                     GMap3d :: CControlerGMap * controler ,
                                     Qt :: ToolBarArea dir)
      :
      QToolBar(*nomOperation, parent),
      FControler(controler) ,
      FParent(parent) ,
      FUpdate(false)
{
   setVisible(false);
   parent -> addToolBar(dir, this);
   setAllowedAreas(Qt::BottomToolBarArea | Qt::TopToolBarArea);
}

// Destructeur
//-------------
dialogOperations :: ~dialogOperations()
{
}

// Accesseur sur le controleur
//----------------------------
GMap3d :: CControlerGMap * dialogOperations :: getControler() const
{
   return FControler ;
}

// Methode qui redessine
//----------------------
void dialogOperations :: repaint()
{
   QToolBar :: repaint() ;
   FParent -> repaint() ;
}

// Methode de mise a jour
//-----------------------
void dialogOperations :: update()
{
}

// SLOTS
//-------
void dialogOperations :: callbackClose()
{
   // On retourne en mode selection
   getControler()->setMode(MODE_SELECTION);
   setVisible(false);
}
