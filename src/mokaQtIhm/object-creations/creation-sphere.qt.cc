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

#include "creation-sphere.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QLabel>

/*******************************************************************/
/* CLASSE creationSphere                                           */
/*******************************************************************/
// Constructeur
//-------------

CreationSphere :: CreationSphere(Window * parent ,
                                 GMap3d :: CControlerGMap * controler) :
      CreationObjet(parent , HTML::decode("Create sphere") ,
                    controler)
{
   // Creation des widgets
   FAffCar = new QLabel(*new QString("  Subdivisions: ") , this) ;
   FAffPar = new QLabel(*HTML::decode("Parallels: ") , this) ;

   FSaisiePar = new QSpinBox(this) ;
   FSaisiePar -> setMinimum(1);
   FSaisiePar -> setMaximum(1000);
   FSaisiePar -> setAccelerated(true);
   
   FAffMer = new QLabel("Meridians : " , this) ;

   FSaisieMer = new QSpinBox(this) ;
   FSaisieMer -> setMinimum(3);
   FSaisieMer -> setMaximum(1000);
   FSaisieMer -> setAccelerated(true);
         
   addWidget(FAffCar);
   addWidget(FAffPar);
   addWidget(FSaisiePar);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));
   addWidget(FAffMer);
   addWidget(FSaisieMer);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));

   // Creation des boutons
   FChoix = new BoutonsCreation(this) ;

   //*** Creation de la boite de positonnement ******
   FPosit = new BoitePositionnement(this , parent , OBJECT_SPHERE) ;

   update() ;

   // Mise en place des ecoutes
   QObject::connect(FSaisiePar , SIGNAL(valueChanged(int)) , this,
                    SLOT(callbackParallels())) ;
   QObject::connect(FSaisieMer , SIGNAL(valueChanged(int)) , this,
                    SLOT(callbackMeridians())) ;
}

// Destructeur
//------------
CreationSphere :: ~CreationSphere() { }

// Methode de mise a jour
//-----------------------
void CreationSphere :: update()
{
   CreationObjet :: update() ;

   FSaisieMer -> setValue(FControler -> getParameterCreation()
                          -> getSphereNbMeridians()) ;
   FSaisiePar -> setValue(FControler -> getParameterCreation()
                          -> getSphereNbParallels()) ;
}

// Slots
//------
void CreationSphere :: callbackMeridians()
{
   setMeridians(FSaisieMer -> value()) ;
}

void CreationSphere :: callbackParallels()
{
   setParallels(FSaisiePar -> value()) ;
}

// Accesseurs
//-----------
void CreationSphere :: setMeridians(int nb)
{
   FControler -> getParameterCreation() -> setSphereNbMeridians(nb) ;
   update() ;
   repaint() ;
}

void CreationSphere :: setParallels(int nb)
{
   FControler -> getParameterCreation() -> setSphereNbParallels(nb) ;
   update() ;
   repaint() ;
}
