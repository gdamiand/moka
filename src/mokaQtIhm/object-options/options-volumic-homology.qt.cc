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

#include "options-volumic-homology.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"
#include "compute-homology.hh"

#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <Qt3Support/Q3Accel>

#ifndef _WINDOWS
#include "chrono.hh"
#endif

// Constructeur
//-------------
OptionsVolumicHomology :: OptionsVolumicHomology(Window * parent) :
      QDialog(parent) ,
      FParent(parent) ,
      FUpdate(false),
      FHomology(NULL)
{
   // Modification du titre de la boite de dialogue
   setWindowTitle("Homology") ;

   // Definition du raccourci

   Q3Accel * Raccourci = new Q3Accel(this) ;
   Raccourci -> insertItem(QKeySequence(Qt :: Key_Space) , 9) ;
   Raccourci -> connectItem(9 , this , SLOT(close()))  ;

   QVBoxLayout * placement = new QVBoxLayout(this) ;

   //-- global --
   FGenerators = new QGroupBox("Generators") ;
   QGridLayout * global = new QGridLayout(FGenerators) ;

   Fh0        = new QLabel("") ;
   Fh1free    = new QLabel("") ;
   Fh1torsion = new QLabel("") ;
   Fh2free    = new QLabel("") ;
   Fh2torsion = new QLabel("") ;
   Fh3        = new QLabel("") ;
      
   FButtonH0 = new QPushButton("H0:");
   FButtonH1free = new QPushButton("free:");
   FButtonH1torsion = new QPushButton("torsion:");
   FButtonH2free = new QPushButton("free:");
   FButtonH2torsion = new QPushButton("torsion:");
   FButtonH3 = new QPushButton("H3:");
      
   FButtonH0->setCheckable(true);
   connect(FButtonH0, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH0(bool))); 

   FButtonH1free->setCheckable(true);
   connect(FButtonH1free, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH1free(bool))); 

   FButtonH1torsion->setCheckable(true);
   connect(FButtonH1torsion, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH1torsion(bool))); 

   FButtonH2free->setCheckable(true);
   connect(FButtonH2free, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH2free(bool))); 

   FButtonH2torsion->setCheckable(true);
   connect(FButtonH2torsion, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH2torsion(bool))); 

   FButtonH3->setCheckable(true);
   connect(FButtonH3, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH3(bool))); 

   global->addWidget(FButtonH0 , 1 , 0) ;
   global->addWidget(Fh0 , 1 , 1) ;
   
   global->addWidget(new QLabel("H1:") , 2 , 0) ;

   global->addWidget(FButtonH1free , 2 , 1) ;
   global->addWidget(Fh1free , 2 , 2) ;

   global->addWidget(FButtonH1torsion , 2 , 3) ;
   global->addWidget(Fh1torsion , 2 , 4) ;
      
   global->addWidget(new QLabel("H2:") , 3 , 0) ;

   global->addWidget(FButtonH2free , 3 , 1) ;
   global->addWidget(Fh2free , 3 , 2) ;   
   
   global->addWidget(FButtonH2torsion , 3 , 3) ;
   global->addWidget(Fh2torsion , 3 , 4) ;   

   global->addWidget(FButtonH3 , 4 , 0) ;
   global->addWidget(Fh3 , 4 , 1) ;   

   //-- volume --
   FInfo = new QGroupBox("Performances") ;
   QGridLayout * pointed = new QGridLayout(FInfo) ;

   FTime   = new QLabel("") ;
   FMemory = new QLabel("") ;

   pointed->addWidget(new QLabel("time:") , 1 , 0);
   pointed->addWidget(FTime , 1 , 1) ;
   
   pointed->addWidget(new QLabel("memory:") , 2 , 0);
   pointed->addWidget(FMemory , 2 , 1) ;   

   // placement dans le layout principal
   placement -> addWidget(FGenerators) ;
   placement -> addWidget(FInfo) ;
}

// Destructeur
//------------
OptionsVolumicHomology :: ~OptionsVolumicHomology()
{ delete FHomology; }

void OptionsVolumicHomology::update()
{
  delete FHomology;
  
  std::ostringstream os;
   
#ifndef _WINDOWS
  CChrono c; c.setMode(PUS_MODE); c.start();
#endif

  FHomology = new GMap3d::CHomology(FParent->getControler()->getMap(),
				    FParent->getControler()->
				    getSelectionMark());
  if ( FHomology!=NULL && FHomology->computeVolumicHomology() )
    FParent->getControler()->setMessage("Homology computed.");
  else
    FParent->getControler()->
      setMessage("Impossible to compute Homology: not enough memory.");

#ifndef _WINDOWS
   c.stop();
#endif

   // Ho
   os.str("-");
   os<<FHomology->getH0FreeGenerators();
   Fh0->setText(os.str().c_str()) ;
   
   // H1free
   os.str("");
   os<<FHomology->getH1FreeGenerators();
   Fh1free->setText(os.str().c_str()) ;  
   
   // H1torsion
   os.str("");
   os<<FHomology->getH1TorsionGenerators();
   Fh1torsion->setText(os.str().c_str()) ;

   // H2free
   os.str("-");
   os<<FHomology->getH2FreeGenerators();
   Fh2free->setText(os.str().c_str()) ;

   // H2torsion
   os.str("");
   os<<FHomology->getH2TorsionGenerators();
   Fh2torsion->setText(os.str().c_str()) ;

   // H3
   os.str("-");
   os<<FHomology->getH3FreeGenerators();
   Fh3->setText(os.str().c_str()) ;
   
   // Memory
   os.str("");
   unsigned long s = FHomology->size();
   std::string unit(" bytes");
   if ( s>8000 )
     {
       s/=1024;
       unit = " kilo-bytes";
       if ( s>8000 )
	 {
	   s/=1024;
	   unit = " mega-bytes";	  
	 }
     }
   
   os<<s<<unit;
   FMemory->setText(os.str().c_str()) ;

  // Time
   os.str("");
#ifndef _WINDOWS
   os<<c.getTime("");
#endif
   FTime->setText(os.str().c_str()) ;
   
   FButtonH0->setChecked(FHomology->getShowH0());
   FButtonH1free->setChecked(FHomology->getShowH1free());
   FButtonH1torsion->setChecked(FHomology->getShowH1torsion());
   FButtonH2free->setChecked(FHomology->getShowH2free());
   FButtonH2torsion->setChecked(FHomology->getShowH2torsion());
   FButtonH3->setChecked(FHomology->getShowH3());
   
   FParent->getControler()->setSelectionChanged();
   FParent -> repaint() ;
}

void OptionsVolumicHomology::show_impl()
{
   show() ;
   update() ;
}

void OptionsVolumicHomology::callbackToggleH0(bool ADraw)
{
  FHomology->setShowH0(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}

void OptionsVolumicHomology::callbackToggleH1free(bool ADraw)
{
  FHomology->setShowH1free(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}

void OptionsVolumicHomology::callbackToggleH1torsion(bool ADraw)
{
  FHomology->setShowH1torsion(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}

void OptionsVolumicHomology::callbackToggleH2free(bool ADraw)
{
  FHomology->setShowH2free(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}

void OptionsVolumicHomology::callbackToggleH2torsion(bool ADraw)
{
  FHomology->setShowH2torsion(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}

void OptionsVolumicHomology::callbackToggleH3(bool ADraw)
{
  FHomology->setShowH3(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}


