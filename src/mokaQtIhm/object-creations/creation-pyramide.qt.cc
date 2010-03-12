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

#include "creation-pyramide.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QLabel>

/*******************************************************************/
/* CLASSE  creationPyramide                                        */
/*******************************************************************/
CreationPyramide ::  CreationPyramide(Window * parent ,
                                      GMap3d :: CControlerGMap * controler)
      :
      CreationObjet(parent , HTML::decode("Create cylinder") ,
                    controler)
{
   FAffSub = new QLabel("  Subdivisions: " , this) ;
   FAffPar = new QLabel(*HTML::decode("Parallels: ") , this) ;

   FSaisiePar = new QSpinBox(this) ;
   FSaisiePar -> setMinimum(1);
   FSaisiePar -> setMaximum(1000);
   FSaisiePar -> setAccelerated(true);

   FAffMer = new QLabel(*HTML::decode("Meridians: ") , this) ;

   FSaisieMer = new QSpinBox(this) ;
   FSaisieMer -> setMinimum(3);
   FSaisieMer -> setMaximum(1000);
   FSaisieMer -> setAccelerated(true);
   
   FAffFaces =
      new QLabel(*HTML::decode("Faces created: ") , this) ;
   FBase = new QCheckBox("Basis" , this) ;

   addWidget(FAffSub);
   addWidget(FAffPar);
   addWidget(FSaisiePar);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));
   addWidget(FAffMer);
   addWidget(FSaisieMer);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));
   addWidget(FAffFaces);
   addWidget(FBase);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));

   // Creation des boutons
   FChoix = new BoutonsCreation(this) ;

   //*** Creation de la boite de positonnement ******
   FPosit = new BoitePositionnement(this , parent , OBJECT_PYRAMID) ;

   update() ;

   QObject::connect(FSaisiePar , SIGNAL(valueChanged(int)) , this,
                    SLOT(callbackParallels())) ;
   QObject::connect(FSaisieMer , SIGNAL(valueChanged(int)) , this,
                    SLOT(callbackMeridians())) ;
   QObject::connect(FBase , SIGNAL(stateChanged(int)) , this ,
                    SLOT(callbackCheckedButton())) ;
}

CreationPyramide :: ~CreationPyramide()
{
}

void CreationPyramide :: update()
{
   CreationObjet :: update() ;

   FSaisieMer ->setValue(FControler->getParameterCreation()
                         ->getPyramidNbMeridians());
   FSaisiePar->setValue(FControler->getParameterCreation()
                        ->getPyramidNbParallels());
   FBase -> setChecked(FControler->getParameterCreation()->getPyramidClosed());
}

// Accesseurs en lecture
//----------------------
int CreationPyramide::getMeridians() const
{
   return FSaisieMer -> value();
}

int CreationPyramide::getParallels() const
{
   return FSaisiePar -> value();
}

bool CreationPyramide::getCreatedBase() const
{
   return FBase -> isChecked();
}

// Accesseurs a la modification
//------------------------------
void CreationPyramide::setMeridians(int m)
{
   FSaisieMer -> setValue(m) ;
   FControler -> getParameterCreation() -> setPyramidNbMeridians(m) ;
   repaint() ;
}

void CreationPyramide::setParallels(int p)
{
   FSaisiePar -> setValue(p) ;
   FControler -> getParameterCreation() -> setPyramidNbParallels(p) ;
   repaint() ;
}

void CreationPyramide::setCreatedBase(bool b)
{
   FBase-> setChecked(b) ;
   FControler -> getParameterCreation() -> setPyramidClosed(b) ;
   repaint() ;
}

// Slots
void CreationPyramide :: callbackMeridians()
{
   setMeridians(getMeridians()) ;
}

void CreationPyramide::callbackParallels()
{
   setParallels(getParallels()) ;
}

void CreationPyramide::callbackCheckedButton()
{
   setCreatedBase(getCreatedBase()) ;
}
