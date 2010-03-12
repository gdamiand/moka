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

#include "creation-options.qt.hh"
#include "window.qt.hh"

#include <QtGui/QPixmap>
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <Qt3Support/Q3Accel>

static const char * const coordLabels [ 3 ] =
{
   " X" , " Y" , " Z"
} ;

//********************************************************
// Constructeur                                          *
//********************************************************
BoitePositionnement ::  BoitePositionnement(CreationObjet * lien ,
      Window * parent , int objet) :
      QDialog(parent),
      FLien(lien),
      FModifie(true),
      FNumObject(objet)
{
   //******* Construction de la boite *******
   QBoxLayout * total = new QBoxLayout(QBoxLayout :: TopToBottom, this) ;

   //--------------- Centre ------------------------
   QWidget * widCentre = new QWidget(this) ;
   QBoxLayout * boxCentre = new QBoxLayout(QBoxLayout :: LeftToRight,
                                           widCentre);

   FTitreCentre = new QLabel(" Centre : " , widCentre) ;
   boxCentre -> addWidget(FTitreCentre) ;

   for (int i = 0 ; i < 3 ; i++)
   {
      FAffCentre [ i ] = new QLabel(coordLabels [ i ] , widCentre) ;
      boxCentre -> addWidget(FAffCentre [ i ] , Qt::AlignHCenter) ;
      FSaisieCentre [ i ] = new FloatSpinBox(0.5, -1000 , 1000);
      boxCentre -> addWidget(FSaisieCentre [ i ] , Qt::AlignHCenter) ;
   }
   total -> addWidget(widCentre) ;

   //--------------- Facteur ------------------------
   QWidget * widFacteur = new QWidget(this) ;
   QBoxLayout * boxFacteur = new QBoxLayout(QBoxLayout :: LeftToRight,
         widFacteur);
   FTitreFacteur = new QLabel(" Growing coefficient: " , widFacteur) ;
   boxFacteur -> addWidget(FTitreFacteur) ;

   FSaisieFacteur = new FloatSpinBox(0.2, -1000 , 1000);
   boxFacteur -> addWidget(FSaisieFacteur , Qt::AlignHCenter) ;

   total -> addWidget(widFacteur) ;

   //--------------- Proportions ----------------------
   QWidget * widProportions = new QWidget(this) ;
   QBoxLayout * boxProportions = new QBoxLayout(QBoxLayout :: LeftToRight,
         widProportions) ;

   FTitreProportions = new QLabel(" Proportions: " , widProportions) ;
   boxProportions -> addWidget(FTitreProportions) ;

   for (int i = 0 ; i < 3 ; i++)
   {
      FAffProportions [ i ] = new QLabel(coordLabels [ i ] ,
                                         widProportions) ;
      boxProportions -> addWidget(FAffProportions [ i ] , Qt::AlignHCenter) ;

      FSaisieProportions [ i ] = new FloatSpinBox(0.2, -1000 , 1000);
      boxProportions -> addWidget(FSaisieProportions [ i ], 0,
                                  Qt::AlignHCenter) ;
   }

   total -> addWidget(widProportions) ;

   //--------------- Dimensions ------------------------
   QWidget * widDimensions = new QWidget(this) ;
   QBoxLayout * boxDimensions =
      new QBoxLayout(QBoxLayout :: LeftToRight, widDimensions);

   FTitreDimensions = new QLabel(" Dimensions: " , widDimensions) ;
   boxDimensions -> addWidget(FTitreDimensions) ;

   for (int i = 0 ; i < 3 ; i++)
   {
      FAffDimensions [ i ] = new QLabel(coordLabels [ i ] , widDimensions) ;
      boxDimensions->addWidget(FAffDimensions [ i ] , 0, Qt::AlignHCenter) ;

      FSaisieDimensions [ i ] = new FloatSpinBox(0.5, -1000 , 1000);
      boxDimensions->addWidget(FSaisieDimensions[i], 0, Qt::AlignHCenter);
   }

   total -> addWidget(widDimensions) ;

   //--------------- Rotations ------------------------
   QWidget * widRotations = new QWidget(this) ;
   QBoxLayout * boxRotations = new QBoxLayout(QBoxLayout :: LeftToRight,
         widRotations) ;

   FTitreRotations = new QLabel(" Rotations: " , widRotations) ;
   boxRotations -> addWidget(FTitreRotations) ;

   for (int i = 0 ; i < 3 ; i++)
   {
      FAffRotations [ i ] = new QLabel(coordLabels [ i ] , widRotations) ;
      boxRotations -> addWidget(FAffRotations [ i ] , Qt::AlignHCenter) ;

      FSaisieRotations [ i ] =  new FloatSpinBox(5 , -180.0 , 180.0 , 0 , 0.0);
      FSaisieRotations [ i ] -> setWrapping ( true ) ;   
      boxRotations -> addWidget(FSaisieRotations [ i ] , Qt::AlignHCenter) ;
   }
   total -> addWidget(widRotations) ;

//--------------- Pole Nord ------------------------
   QWidget * widPoleNord = new QWidget(this) ;
   QBoxLayout * boxPoleNord = new QBoxLayout(QBoxLayout :: LeftToRight,
         widPoleNord) ;

   FTitrePoleNord = new QLabel(" PoleNord : " , widPoleNord) ;
   boxPoleNord -> addWidget(FTitrePoleNord) ;

   for (int i = 0 ; i < 3 ; i++)
   {
      FAffPoleNord [ i ] = new QLabel(coordLabels [ i ] , widPoleNord) ;
      boxPoleNord -> addWidget(FAffPoleNord [ i ] , Qt::AlignHCenter) ;

      FSaisiePoleNord [ i ] = new FloatSpinBox(0.5, -1000 , 1000);
      boxPoleNord -> addWidget(FSaisiePoleNord [ i ] , Qt::AlignHCenter) ;
   }

   total -> addWidget(widPoleNord) ;

   //--------------- Equateur ------------------------
   QWidget * widEquateur = new QWidget(this) ;
   QBoxLayout * boxEquateur = new QBoxLayout(QBoxLayout :: LeftToRight,
         widEquateur);

   FTitreEquateur = new QLabel(" Equator: " , widEquateur) ;
   boxEquateur -> addWidget(FTitreEquateur) ;

   for (int i = 0 ; i < 3 ; i++)
   {
      FAffEquateur [ i ] = new QLabel(coordLabels [ i ] , widEquateur) ;
      boxEquateur -> addWidget(FAffEquateur [ i ] , Qt::AlignHCenter) ;

      FSaisieEquateur [ i ] = new FloatSpinBox(0.5, -1000 , 1000);
      boxEquateur -> addWidget(FSaisieEquateur [ i ] , Qt::AlignHCenter) ;
   }

   total -> addWidget(widEquateur) ;

   //---------------- Bouton Masquer ----------------
   QWidget * widMasquer = new QWidget(this) ;
   QBoxLayout * boxMasquer = new QBoxLayout(QBoxLayout :: LeftToRight,
         widMasquer) ;

   QIcon p1(":/masquer.png");
   FMasquer = new QPushButton(p1 , "Hide" , widMasquer) ;
   FMasquer -> setFixedSize(110 , 30) ;
   FMasquer -> setShortcut(Qt::CTRL + Qt::Key_P) ;
   FMasquer -> setFocus() ;

   boxMasquer -> addWidget(FMasquer , Qt::AlignHCenter) ;
   total -> addWidget(widMasquer) ;

   //************ Mise en place des ecoutes *************
   for (int i = 0 ; i < 3 ; i++)
   {
      QObject :: connect(FSaisieCentre [ i ] ,
                         SIGNAL(valueChanged(double)),
                         this , SLOT(callbackCenter())) ;
      QObject :: connect(FSaisieProportions [ i ] ,
                         SIGNAL(valueChanged(double)) , this ,
                         SLOT(callbackProportions())) ;
      QObject :: connect(FSaisieDimensions [ i ] ,
                         SIGNAL(valueChanged(double)) , this ,
                         SLOT(callbackDimensions())) ;
      QObject :: connect(FSaisieRotations [ i ] ,
                         SIGNAL(valueChanged(double)) , this ,
                         SLOT(callbackRotations())) ;
      QObject :: connect(FSaisiePoleNord [ i ] ,
                         SIGNAL(valueChanged(double)) , this ,
                         SLOT(callbackFirstVertex())) ;
      QObject :: connect(FSaisieEquateur [ i ] ,
                         SIGNAL(valueChanged(double)) , this ,
                         SLOT(callbackNormalVector())) ;
   }
   QObject :: connect(FSaisieFacteur ,
                      SIGNAL(valueChanged(double)) , this ,
                      SLOT(callbackScale())) ;

   QObject :: connect(FMasquer ,
                      SIGNAL(clicked()) , this ,
                      SLOT(hide())) ;

   Q3Accel * Raccourci = new Q3Accel(this) ;
   Raccourci -> insertItem(QKeySequence(Qt::Key_Space) , 2) ;
   Raccourci -> connectItem(2 , FLien , SLOT(cancel())) ;
}

//*******************************************
// Destructeur                              *
//*******************************************
BoitePositionnement :: ~BoitePositionnement() { }
//*******************************************
// Methode de mise a jour                   *
//*******************************************
void BoitePositionnement :: update()
{
   FModifie = false ;
   CParameterObjectPosition* p =
      FLien -> getControler() ->
      getParameterObjectPosition(FNumObject) ;

   FSaisieFacteur -> setValuef(p -> getScale()) ;

   FSaisieProportions[0]->setValuef(p->getProportionX());
   FSaisieProportions[1]->setValuef(p->getProportionY());
   FSaisieProportions[2]->setValuef(p->getProportionZ());

   FSaisieDimensions[0]->setValuef(p->getDimensionX());
   FSaisieDimensions[1]->setValuef(p->getDimensionY());
   FSaisieDimensions[2]->setValuef(p->getDimensionZ());

   FSaisieRotations[0]->setValue((int)p->getRotationX());
   FSaisieRotations[1]->setValue((int)p->getRotationY());
   FSaisieRotations[2]->setValue((int)p->getRotationZ());

   FSaisieCentre[0]->setValuef(p->getCenterX());
   FSaisieCentre[1]->setValuef(p->getCenterY());
   FSaisieCentre[2]->setValuef(p->getCenterZ());

   FSaisiePoleNord[0]->setValuef(p->getFirstVertexX());
   FSaisiePoleNord[1]->setValuef(p->getFirstVertexY());
   FSaisiePoleNord[2]->setValuef(p->getFirstVertexZ());

   FSaisieEquateur[0]->setValuef(p->getNormalVectorX());
   FSaisieEquateur[1]->setValuef(p->getNormalVectorY());
   FSaisieEquateur[2]->setValuef(p->getNormalVectorZ());

   FModifie = true ;
}
//*******************************************************
// Slots                                                *
//*******************************************************
void BoitePositionnement :: callbackScale()
{
   if (FModifie)
      setScale(getScale()) ;
}

void BoitePositionnement :: callbackProportions()
{
   if (FModifie)
      setProportions(getProportions()) ;
}

void BoitePositionnement :: callbackDimensions()
{
   if (FModifie)
      setDimensions(getDimensions()) ;
}

void BoitePositionnement :: callbackCenter()
{
   if (FModifie)
      setCenter(getCenter()) ;
}

void BoitePositionnement :: callbackRotations()
{
   if (FModifie)
      setRotations(getRotations()) ;
}

void BoitePositionnement :: callbackFirstVertex()
{
   if (FModifie)
      setFirstVertex(getFirstVertex()) ;
}

void BoitePositionnement :: callbackNormalVector()
{
   if (FModifie)
      setNormalVector(getNormalVector()) ;
}

//*******************************************************
// Accesseurs en lecture                                *
//*******************************************************
TCoordinate  BoitePositionnement :: getScale() const
{
   return FSaisieFacteur -> valuef() ;
}

CVertex BoitePositionnement :: getProportions() const
{
   return CVertex(FSaisieProportions[0]-> valuef(),
                  FSaisieProportions[1]-> valuef(),
                  FSaisieProportions[2]-> valuef());
}

CVertex BoitePositionnement :: getDimensions() const
{
   return CVertex(FSaisieDimensions[0]-> valuef(),
                  FSaisieDimensions[1]-> valuef(),
                  FSaisieDimensions[2]-> valuef());
}

CVertex BoitePositionnement :: getCenter() const
{
   return CVertex(FSaisieCentre[0]-> valuef(),
                  FSaisieCentre[1]-> valuef(),
                  FSaisieCentre[2]-> valuef());
}

CVertex BoitePositionnement :: getRotations() const
{
   return CVertex(FSaisieRotations[0]-> value(),
                  FSaisieRotations[1]-> value(),
                  FSaisieRotations[2]-> value());
}

CVertex BoitePositionnement :: getFirstVertex() const
{
   return CVertex(FSaisiePoleNord[0]-> valuef(),
                  FSaisiePoleNord[1]-> valuef(),
                  FSaisiePoleNord[2]-> valuef());
}

CVertex BoitePositionnement :: getNormalVector() const
{
   return CVertex(FSaisieEquateur[0]-> valuef(),
                  FSaisieEquateur[1]-> valuef(),
                  FSaisieEquateur[2]-> valuef());
}

void BoitePositionnement :: activateYProportions(bool b)
{
   FSaisieProportions[1]->setEnabled(b);
   FSaisieDimensions [1]->setEnabled(b);
}

void BoitePositionnement :: activateZProportions(bool b)
{
   FSaisieProportions[2]->setEnabled(b);
   FSaisieDimensions [2]->setEnabled(b);
}

//*******************************************************
// Accesseurs en ecriture                               *
//*******************************************************
void BoitePositionnement :: setScale(TCoordinate s)
{
   FLien->getControler()->getParameterObjectPosition(FNumObject)->setScale(s);
   update() ;
   FLien->repaint();
}

void BoitePositionnement :: setProportions(const CVertex & p)
{
   FLien->getControler()->
         getParameterObjectPosition(FNumObject)->setProportions(p);
   update();
   FLien -> repaint() ;
}

void BoitePositionnement :: setDimensions(const CVertex & d)
{
   FLien->getControler()->
         getParameterObjectPosition(FNumObject)->setDimensions(d);
   update();
   FLien -> repaint() ;
}

void BoitePositionnement :: setRotations(const CVertex & alpha)
{
   FLien->getControler()->
         getParameterObjectPosition(FNumObject)->setRotations(alpha);
   update();
   FLien -> repaint() ;
}

void BoitePositionnement :: setCenter(const CVertex & C)
{
   FLien->getControler()
         ->getParameterObjectPosition(FNumObject)->setCenter(C);
   update();
   FLien -> repaint() ;
}

void BoitePositionnement :: setFirstVertex(const CVertex & S)
{
   FLien->getControler()->
         getParameterObjectPosition(FNumObject)->setFirstVertex(S);
   update();
   FLien -> repaint() ;
}

void BoitePositionnement :: setNormalVector(const CVertex & N)
{
   FLien->getControler()->
         getParameterObjectPosition(FNumObject)->setNormalVector(N);
   update();
   FLien -> repaint() ;
}

void BoitePositionnement :: show()
{
   QDialog :: show() ;
   FMasquer -> setFocus() ;
}
