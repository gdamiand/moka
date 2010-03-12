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

#include "creation-cylindre.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "SpaceWidget.qt.hh"

#include <QtGui/QLabel>


/*******************************************************************/
/* CLASSE  creationCylindre                                        */
/*******************************************************************/

// Constructeur
//-------------
CreationCylindre :: CreationCylindre(Window * parent ,
                                     GMap3d :: CControlerGMap * controler) :
      CreationObjet(parent , HTML::decode("Create cylinder") ,
                    controler)
{
   // creation des widget
   FAffSub = new QLabel("  Subdivisions: " , this) ;
   FAffPar = new QLabel(*HTML::decode("Parallels: ") , this) ;

   FSaisiePar = new QSpinBox(this) ;
   FSaisiePar -> setMinimum(1);
   FSaisiePar -> setMaximum(1000);
   FSaisiePar -> setAccelerated(true);

   FAffMer = new QLabel(*HTML::decode("Meridians : ") , this) ;
   FSaisieMer = new QSpinBox(this) ;
   FSaisieMer -> setMinimum(3);
   FSaisieMer -> setMaximum(1000);
   FSaisieMer -> setAccelerated(true);

   FAffFaces = new QLabel(*HTML::decode("Faces created: ") ,
                          this) ;
   FFace1 = new QCheckBox("Face 1" , this) ;
   FFace2 = new QCheckBox("Face 2" , this) ;

   addWidget(FAffSub);
   addWidget(FAffPar);
   addWidget(FSaisiePar);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));
   addWidget(FAffMer);
   addWidget(FSaisieMer);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));
   addWidget(FAffFaces);
   addWidget(FFace1);
   addWidget(FFace2);
   addWidget(new SpaceWidget(SPACE_PIX_SIZE, 1));

   // Creation des boutons
   FChoix = new BoutonsCreation(this) ;

   //*** Creation de la boite de positonnement ******
   FPosit = new BoitePositionnement(this , parent , OBJECT_CYLINDER) ;

   update() ;
   repaint() ;

// Mise en place des ecouteurs
   QObject::connect(FSaisiePar , SIGNAL(valueChanged(int)) , this,
                    SLOT(callbackParallels())) ;
   QObject::connect(FSaisieMer , SIGNAL(valueChanged(int)) , this,
                    SLOT(callbackMeridians())) ;
   QObject::connect(FFace1 , SIGNAL(clicked()) , this ,
                    SLOT(callbackCheckedButton1())) ;
   QObject::connect(FFace2 , SIGNAL(clicked()) , this ,
                    SLOT(callbackCheckedButton2())) ;
}

// Destructeur
//------------
CreationCylindre :: ~CreationCylindre() {}

// Methode de mise a jour
//-----------------------
void CreationCylindre :: update()
{
   CreationObjet :: update() ;

   FSaisieMer -> setValue(FControler -> getParameterCreation()
                          -> getCylinderNbMeridians());
   FSaisiePar -> setValue(FControler->getParameterCreation()
                          ->getCylinderNbParallels());

   FFace1 -> setChecked(FControler->getParameterCreation()
                        ->getCylinderClosedUp());
   FFace2 -> setChecked(FControler->getParameterCreation()
                        ->getCylinderClosedDown());
}

// Accesseurs de modification
//----------------------------
void CreationCylindre :: setMeridians(int m)
{
   FControler ->getParameterCreation()->setCylinderNbMeridians(m);
   update();
   repaint() ;
}

void CreationCylindre :: setParallels(int p)
{
   FControler ->getParameterCreation()->setCylinderNbParallels(p);
   update();
   repaint() ;
}

void CreationCylindre :: setClosedBorder1(bool b)
{
   FControler->getParameterCreation()->setCylinderClosedUp(b);
   update() ;
   repaint() ;
}

void CreationCylindre :: setClosedBorder2(bool b)
{
   FControler->getParameterCreation()->setCylinderClosedDown(b);
   update() ;
   repaint() ;

}

// Slots
//------
void CreationCylindre :: callbackMeridians()
{
   setMeridians(FSaisieMer -> value());
}

void CreationCylindre :: callbackParallels()
{
   setParallels(FSaisiePar->value());
}

void CreationCylindre :: callbackCheckedButton1()
{
   setClosedBorder1(FFace1->isChecked());
}

void CreationCylindre :: callbackCheckedButton2()
{
   setClosedBorder2(FFace2->isChecked());
}
