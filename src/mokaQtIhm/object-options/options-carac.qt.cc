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

#include "options-carac.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <Qt3Support/Q3Accel>


// Constructeur
//-------------
OptionsCarac :: OptionsCarac(Window * parent)
      :
      QDialog(parent) ,
      FParent(parent) ,
      FUpdate(false)
{
   // Modification du titre de la boite de dialogue
   setWindowTitle("Topological characteristics") ;

   // Definition du raccourci

   Q3Accel * Raccourci = new Q3Accel(this) ;
   Raccourci -> insertItem(QKeySequence(Qt :: Key_Space) , 9) ;
   Raccourci -> connectItem(9 , this , SLOT(close()))  ;

   QHBoxLayout * placement = new QHBoxLayout(this) ;

   //-- global --
   FGlobal = new QGroupBox("Globals") ;
   QGridLayout * global = new QGridLayout(FGlobal) ;

   global->addWidget(new QLabel("darts:") , 1 , 0) ;
   global->addWidget(new QLabel("vertices:") , 2 , 0) ;
   global->addWidget(new QLabel("edges:") , 3 , 0);
   global->addWidget(new QLabel("faces:") , 4 , 0) ;
   global->addWidget(new QLabel("volumes:") , 5 , 0) ;
   global->addWidget(new QLabel("composants:") , 6 , 0) ;

   for (int i = 0 ; i < 6 ; i++)
   {
      FGlobalCells [i] = new QLabel("") ;
      global -> addWidget(FGlobalCells [ i ] , i + 1 , 1) ;
   }

   global -> addWidget(new QLabel("0-borders:") , 2 , 2) ;
   global -> addWidget(new QLabel("1-borders:") , 3 , 2) ;
   global -> addWidget(new QLabel("2-borders:") , 4 , 2) ;
   global -> addWidget(new QLabel("3-borders:") , 5 , 2) ;

   for (int i = 0 ; i < 4 ; i++)
   {
      FGlobalBorders [i] = new QLabel("") ;
      global -> addWidget(FGlobalBorders [ i ] , i + 2 , 3) ;
   }

   //-- volume --
   FPointed = new QGroupBox("Volume incident to 'last'") ;
   QGridLayout * pointed = new QGridLayout(FPointed) ;

   pointed->addWidget(new QLabel("darts:") , 1 , 0);
   pointed->addWidget(new QLabel("vertices:") , 2 , 0);
   pointed->addWidget(new QLabel("edges:") , 3 , 0);
   pointed->addWidget(new QLabel("faces:") , 4 , 0);

   for (int i = 0 ; i < 4 ; i++)
      FPointedCells [i] = new QLabel("") ;

   for (int i = 0 ; i < 4 ; i++)
      pointed -> addWidget(FPointedCells [ i ] , i + 1 , 1) ;


   pointed -> addWidget(new QLabel("0-borders:") , 1 , 2) ;
   pointed -> addWidget(new QLabel("1-borders:") , 2 , 2) ;
   pointed -> addWidget(new QLabel("2-borders:") , 3 , 2) ;

   for (int i = 0 ; i < 3 ; i++)
      FPointedBorders [i]  = new QLabel("") ;

   for (int i = 0 ; i < 3 ; i++)
      pointed -> addWidget(FPointedBorders [ i ] , i + 1 , 3) ;

   QWidget * WPar = new QWidget ;
   QGridLayout * par = new QGridLayout(WPar) ;

   par->
   addWidget(new QLabel("Euler characteristic:"),1 , 0) ;
   par->
   addWidget(new QLabel("Orientability coefficient:"),2 , 0) ;
   par -> addWidget(new QLabel("Genus:") , 3 , 0) ;

   FEulerLabel = new QLabel("") ;
   par -> addWidget(FEulerLabel , 1 , 1) ;
   FOrientLabel = new QLabel("") ;
   par -> addWidget(FOrientLabel , 2 , 1) ;
   FGenusLabel = new QLabel("") ;
   par -> addWidget(FGenusLabel , 3 , 1) ;
   FNameLabel = new QLabel("") ;

   pointed -> addWidget(WPar);
   pointed -> addWidget(FNameLabel);

   // placement dans le layout principal
   placement -> addWidget(FGlobal) ;
   placement -> addWidget(FPointed) ;
}

// Destructeur
//------------
OptionsCarac :: ~OptionsCarac()
{}

void OptionsCarac::update()
{
   // Calcul des caracteristiques topologiques:
   int nb[4], nb2WhenClosed;
   int nc[5], nd;
   int euler, orient, genus;

   char buffer[64];

   // Caracteristiques generales:
   FParent -> getControler()
   -> getMapGlobalCharacteristics(& nd,
                                  & nc [ 0 ] , & nc [ 1 ] , & nc [ 2 ] ,
                                  & nc [ 3 ] , & nc [ 4 ] , & nb [ 0 ] ,
                                  & nb [ 1 ] , & nb [ 2 ] , & nb [ 3 ]) ;

   sprintf(buffer, "%d", nd) ;
   FGlobalCells [ 0 ] -> setText(buffer) ;

   for (int i = 0 ; i < 4 ; ++i)
   {
      sprintf(buffer , "%d" , nc [ i ]) ;
      FGlobalCells [ i+1 ] -> setText(buffer) ;

      sprintf(buffer , "%d" , nb [ i ]) ;
      FGlobalBorders [ i ] -> setText(buffer) ;
   }

   sprintf(buffer , "%d" , nc [ 4 ]) ;
   FGlobalCells [ 5 ] -> setText(buffer) ;

   // Caracteristiques de l'objet pointe:
   string name;

   bool ok =
      FParent -> getControler()
      -> getSurfaceCharacteristics(& name , & nd ,
                                   & nc [ 0 ] , & nc [ 1 ] , & nc [ 2 ] ,
                                   & nb [ 0 ] , & nb [ 1 ] , & nb [ 2 ] ,
                                   & nb2WhenClosed ,
                                   & euler , & orient , & genus) ;

   if (ok)
   {
      sprintf(buffer , "%d" , nd) ;
      FPointedCells [ 0 ] -> setText(buffer) ;

      for (int i = 0 ; i < 3 ; ++i)
      {
         sprintf(buffer , "%d" , nc [ i ]) ;
         FPointedCells [ i+1 ] -> setText(buffer) ;

         sprintf(buffer , "%d" , nb [ i ]) ;
         FPointedBorders [ i ] -> setText(buffer) ;
      }

      sprintf(buffer , "%d" , euler) ;
      FEulerLabel -> setText(buffer) ;

      sprintf(buffer , "%d" , orient) ;
      FOrientLabel -> setText(buffer) ;

      sprintf(buffer , "%d" , genus) ;
      FGenusLabel -> setText(buffer) ;

      FNameLabel->setText(QString::fromUtf8(name.c_str()));
   }
   else
   {
      FPointedCells [ 0 ] -> setText("") ;

      for (int i = 0 ; i < 3 ; ++i)
      {
         FPointedBorders [ i ] -> setText("") ;
         FPointedCells [ i+1 ] -> setText("") ;
      }

      FEulerLabel -> setText("") ;
      FOrientLabel -> setText("") ;
      FGenusLabel -> setText("") ;
      FNameLabel -> setText("") ;
   }

   FParent -> repaint() ;
}

void OptionsCarac :: show_impl()
{
   show() ;
   update() ;
}
