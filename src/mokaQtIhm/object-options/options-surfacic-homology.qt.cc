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

#include "options-surfacic-homology.qt.hh"
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
OptionsSurfacicHomology :: OptionsSurfacicHomology(Window * parent) :
      QDialog(parent) ,
      FParent(parent) ,
      FUpdate(false),
      FHomology(NULL),
      FIndexAlpha3(-1)
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
   Fh2        = new QLabel("") ;
      
   FButtonH0	    = new QPushButton("H0:");
   FButtonH1free    = new QPushButton("free:");
   FButtonH1torsion = new QPushButton("torsion:");
   FButtonH2	    = new QPushButton("H2:");
      
   FButtonH0->setCheckable(true);
   connect(FButtonH0, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH0(bool))); 

   FButtonH1free->setCheckable(true);
   connect(FButtonH1free, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH1free(bool))); 

   FButtonH1torsion->setCheckable(true);
   connect(FButtonH1torsion, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH1torsion(bool))); 

   FButtonH2->setCheckable(true);
   connect(FButtonH2, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleH2(bool))); 

   global->addWidget(FButtonH0 , 1 , 0) ;
   global->addWidget(Fh0 , 1 , 1) ;
   
   global->addWidget(new QLabel("H1:") , 2 , 0) ;

   global->addWidget(FButtonH1free , 2 , 1) ;
   global->addWidget(Fh1free , 2 , 2) ;

   global->addWidget(FButtonH1torsion , 2 , 3) ;
   global->addWidget(Fh1torsion , 2 , 4) ;
      
   global->addWidget(FButtonH2 , 3 , 0) ;
   global->addWidget(Fh2 , 3 , 1) ;

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
OptionsSurfacicHomology :: ~OptionsSurfacicHomology()
{}

void OptionsSurfacicHomology::closeEvent(QCloseEvent *event)
{   
  restoreAlpha3();
  delete FHomology; FHomology=NULL;
  event->accept();
}

void OptionsSurfacicHomology::restoreAlpha3()
{
  if ( FIndexAlpha3==-1 ) return;

  for (CDynamicCoverageAll it(FParent->getControler()->getMap());
       it.cont(); ++it)
    {      
      (*it)->setAlpha3(FParent->getControler()->getMap()->
		       getDirectInfoAsDart(*it, FIndexAlpha3));
    }

  FParent->getControler()->getMap()->freeDirectInfo(FIndexAlpha3);
  FIndexAlpha3 = -1;
}

void OptionsSurfacicHomology::removeAlpha3()
{
  if ( FIndexAlpha3!=-1 ) return;
  
  FIndexAlpha3 = FParent->getControler()->getMap()->getNewDirectInfo();
  for (CDynamicCoverageAll it(FParent->getControler()->getMap());
       it.cont(); ++it)
    {      
      FParent->getControler()->getMap()->
	setDirectInfo(*it, FIndexAlpha3, (*it)->getAlpha3());
      (*it)->setFree3();
    }
}

void OptionsSurfacicHomology::update()
{
  delete FHomology;
  removeAlpha3();
  
  std::ostringstream os;

#ifndef _WINDOWS
  CChrono c; c.setMode(PUS_MODE); c.start();
#endif

  FHomology = new GMap3d::CHomology(FParent->getControler()->getMap(),
				    FParent->getControler()->
				    getSelectionMark());
  if ( FHomology!=NULL && FHomology->computeSurfacicHomology() )
    FParent->getControler()->setMessage("Surfacic homology computed.");
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
   Fh2->setText(os.str().c_str()) ;
   
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
   FButtonH2->setChecked(FHomology->getShowH2free());
   
   FParent->getControler()->setSelectionChanged();
   FParent -> repaint() ;
}

void OptionsSurfacicHomology::show_impl()
{
   show() ;
   update() ;
}

void OptionsSurfacicHomology::callbackToggleH0(bool ADraw)
{
  FHomology->setShowH0(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}

void OptionsSurfacicHomology::callbackToggleH1free(bool ADraw)
{
  FHomology->setShowH1free(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}

void OptionsSurfacicHomology::callbackToggleH1torsion(bool ADraw)
{
  FHomology->setShowH1torsion(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}

void OptionsSurfacicHomology::callbackToggleH2(bool ADraw)
{
  FHomology->setShowH2free(ADraw);
  FParent->getControler()->setSelectionChanged();
  FParent -> repaint() ;
}
