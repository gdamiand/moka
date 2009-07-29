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

#ifndef OPTIONSPONDERATION_QT_HH
#define OPTIONSPONDERATION_QT_HH

//--------- Include autres classes developees ----------
#include "controler-gmap.hh"
#include "floatSpinBox.qt.hh"

//--------------------- Include QT --------------------
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>



class Window ;

/**
 * Implemente la table pour l'option "Ponderation des deformations"
 */
class OptionsPonderation : public QWidget 
{
  Q_OBJECT
public :

/**
* Constructeur
* Construit la table pour l'option "Ponderation des deformations"
* @param parent La fenetre principale
* @param parentTab La tabdialog des options
*/
  OptionsPonderation ( Window * parent , QTabWidget *  parentTab ) ;

/**
* Destructeur
* Detruit la table
*/
  ~OptionsPonderation ( ) ;

/**
* Met a jour l'affichage
*/
  void update ( ) ;

// Accesseurs
  void setDistanceType ( GMap3d :: TPonderationType ) ;
  void setFunctionType ( TFunctionType ) ;
  void setCenter ( const CVertex & ) ;
  void setVector ( const CVertex & ) ;
  void setEtalonnageDistances ( bool ) ;
  void setMin1 ( TCoordinate ) ;
  void setMax1 ( TCoordinate ) ;
  void setEtalonnageResultats ( bool ) ;
  void setMin2 ( TCoordinate ) ;
  void setMax2 ( TCoordinate ) ;


public slots:
  void callbackDistance ( ) ;
  void callbackFunction ( ) ;

  void callbackVertexChanged ( ) ;
  void callbackVectorChanged ( ) ;

  void callbackTakeCenter ( ) ;
  void callbackTakeVector ( ) ;

  void callbackToggleEtalonnageDistances ( ) ;
  void callbackToggleEtalonnageResultats ( ) ;

  void callbackMin1Changed ( ) ;
  void callbackMax1Changed ( ) ;

  void callbackMin2Changed ( ) ;
  void callbackMax2Changed ( ) ;

private :

// Pointeur sur la fenetre principale
  Window * FParent ;
  bool FUpdate ;

  QComboBox * FSaisieDistance , * FSaisieFonction ;

  QLabel * FAffSommet , * FAffVecteur , * FAffMinDistance , * FAffMaxDistance ,
    * FAffMinResultat , * FAffMaxResultat ,
    * FAffCoefSommet [3] , * FAffCoefVecteur [3] ;

  QGroupBox * FAffEtalonnageDistance, * FAffDistance , * FAffEtalonnageResultat ; 


  FloatSpinBox * FSaisieMinDistance , * FSaisieMaxDistance ,
    * FSaisieMinResultat , * FSaisieMaxResultat ,
    * FSaisieCoefSommet [3] , * FSaisieCoefVecteur [3] ;
  
  QPushButton * FRecupSommet, * FRecupVecteur ;

} ;

#endif
