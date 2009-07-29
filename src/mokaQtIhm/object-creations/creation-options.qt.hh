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

#ifndef BOITE_POSITIONNEMENT_QT_HH
#define BOITE_POSITIONNEMENT_QT_HH

//--------- Include autres classes developees --------
#include "controler-gmap.hh"
#include "floatSpinBox.qt.hh"
class Window ;
class CreationObjet ;

//-------------------- Include QT --------------------
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QToolBar>
#include <QtGui/QDialog>

/*****************************************************/
/* CLASSE BoitePositionnement                        */
/*****************************************************/
class BoitePositionnement : public QDialog
{
   Q_OBJECT

public:
   /**
    * Fabrique la fenetre
    * @param lien ToolBar auquel il est lie
    * @param parent Widget parent
    * @param objet Type d'objet en construction
    */
   BoitePositionnement(CreationObjet * lien , Window * parent , int objet) ;

   /**
    * Destructeur
    */
   ~BoitePositionnement() ;

   //****************************************************
   // Accesseurs                                        *
   //****************************************************
   virtual TCoordinate getScale() const;
   virtual CVertex getProportions() const;
   virtual CVertex getDimensions() const;
   virtual CVertex getCenter() const;
   virtual CVertex getRotations() const;
   virtual CVertex getFirstVertex() const;
   virtual CVertex getNormalVector() const;
   virtual void activateYProportions(bool b) ;
   virtual void activateZProportions(bool b) ;

   virtual void setScale(TCoordinate);
   virtual void setProportions(const CVertex &);
   virtual void setDimensions(const CVertex &);
   virtual void setCenter(const CVertex &);
   virtual void setRotations(const CVertex &);
   virtual void setFirstVertex(const CVertex &);
   virtual void setNormalVector(const CVertex &);

   virtual void update() ;

   virtual void show() ;

private slots:

   void callbackScale() ;
   void callbackProportions() ;
   void callbackDimensions() ;
   void callbackCenter() ;
   void callbackRotations() ;
   void callbackFirstVertex() ;
   void callbackNormalVector() ;

private:

   // Les Saisies
   FloatSpinBox * FSaisieCentre [ 3 ] ;
   FloatSpinBox * FSaisieFacteur ;
   FloatSpinBox * FSaisieProportions [ 3 ] ;
   FloatSpinBox * FSaisieDimensions [ 3 ] ;
   FloatSpinBox * FSaisieRotations [ 3 ] ;
   FloatSpinBox * FSaisiePoleNord [ 3 ] ;
   FloatSpinBox * FSaisieEquateur [ 3 ] ;

   // Les affichages
   QLabel * FAffCentre [ 3 ] ;
   QLabel * FAffProportions [ 3 ] ;
   QLabel * FAffDimensions [ 3 ] ;
   QLabel * FAffRotations [ 3 ] ;
   QLabel * FAffPoleNord [ 3 ] ;
   QLabel * FAffEquateur [ 3 ] ;

   QLabel * FTitreCentre ;
   QLabel * FTitreFacteur ;
   QLabel * FTitreProportions ;
   QLabel * FTitreDimensions ;
   QLabel * FTitreRotations ;
   QLabel * FTitrePoleNord ;
   QLabel * FTitreEquateur ;

   // Pointeur sur la toolbar a laquelle il est lie
   CreationObjet * FLien ;

   // Booleen de modification
   bool FModifie ;

   // Identifiant du type de l'objet a construire
   int FNumObject ;

   // Bouton masquer
   QPushButton * FMasquer ;
} ;

#endif
