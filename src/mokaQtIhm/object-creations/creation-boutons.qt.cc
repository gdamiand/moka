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

#include "creation-boutons.qt.hh"
#include "creation-objet.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

// #include <Qt3Support/Q3Accel>

//***********************************
// Constructeurs                    *
//***********************************
BoutonsCreation::BoutonsCreation(CreationObjet * parent , QBoxLayout * layout)
      :
      QWidget(parent),
      FParent(parent)
{
   // Creation des images

   QIcon p1(":/boutonOK.png") ;
   QIcon p2(":/boutonAnnuler.png") ;
   QIcon p3(":/boutonReinit.png") ;
   QIcon p4(":/boutonOptions.png") ;

   // Creation des boutons
   FReinitialiser = new QPushButton(p3 , *HTML::decode("Reset"),
                                    this) ;
   FAnnuler = new QPushButton(p2 , "Close" , this) ;
   FCreer = new QPushButton(p1 , *HTML::decode("Create") , this) ;
   FOptions = new QPushButton(p4 , "Options" , this) ;

   // Raccourcis claviers

   /*  Q3Accel * Raccourci = new Q3Accel ( this ) ;
       Raccourci -> insertItem ( QKeySequence ( Qt::Key_Space ) , 2 ) ;
       Raccourci -> connectItem ( 2 , this ,
                              SLOT ( cancel ( ) ) ) ;
   */

   FReinitialiser->setShortcut(Qt::Key_Space);
   FOptions->setShortcut(Qt::CTRL + Qt::Key_P);

   /*
   Raccourci -> insertItem ( QKeySequence ( Qt::CTRL + Qt::Key_P ) , 3 ) ;
   Raccourci -> connectItem ( 3 , this ,
                              SLOT ( options ( ) ) ) ;
   */

   // Mise en place des ecoutes
   QObject::connect(FReinitialiser , SIGNAL(clicked()) , this,
                    SLOT(reinit())) ;
   QObject::connect(FAnnuler , SIGNAL(clicked()) , this,
                    SLOT(cancel())) ;
   QObject::connect(FCreer , SIGNAL(clicked()) , this,
                    SLOT(create())) ;
   QObject::connect(FOptions , SIGNAL(clicked()) , this,
                    SLOT(options())) ;

   // Positionnement avec un boxLayout

   if (layout != NULL)
   {
      layout -> addWidget(FReinitialiser) ;
      layout -> addWidget(FAnnuler) ;
      layout -> addWidget(FCreer) ;
      layout -> addWidget(FOptions) ;
   }
   else
   {
      QToolBar* tl = (QToolBar*) parent;
      tl -> addWidget(FReinitialiser) ;
      tl -> addWidget(FAnnuler) ;
      tl -> addWidget(FCreer) ;
      tl -> addWidget(FOptions) ;
   }
}

//***********************************
// Destructeur                      *
//***********************************
BoutonsCreation :: ~BoutonsCreation() { }

//***********************************
// Slots                            *
//***********************************
void BoutonsCreation :: reinit()
{
   FParent -> getControler()
   -> modeCreationReinitPosition() ;

   FParent -> update() ;
   FParent -> repaint() ;
}

void BoutonsCreation :: cancel()
{
   FParent -> cancel() ;
}

void BoutonsCreation :: create()
{
   FParent -> getControler()
   -> createObject() ;
   FParent -> repaint() ;
}

void BoutonsCreation :: options()
{
   FParent -> showOptions() ;
}

//***********************************
// Accesseurs                       *
//***********************************
QPushButton * BoutonsCreation :: getAnnuler()
{
   return FAnnuler ;
}

QPushButton * BoutonsCreation :: getOptions()
{
   return FOptions ;
}
