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

#ifndef OPTIONSVUE_QT_HH
#define OPTIONSVUE_QT_HH


#include "controler-gmap.hh"
#include "IdWidgets.qt.hh"

#include <QtGui/QSpinBox>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QSlider>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QGroupBox>

class Window ;

class OptionsFrame ;

class OptionsVue : public QWidget
{
  Q_OBJECT

public :

  typedef enum { Compact, SemiBurst, Burst, AltBurst,
		 VertexEmphase, EdgeEmphase,
		 FaceEmphase, VolumeEmphase } Presetting;

  /// Constructeur
  OptionsVue ( Window * parent , 
	       OptionsFrame *  parentTab ) ;

  /// Destructeur
  ~OptionsVue ( ) ;

  /// Mise a jour de l'affichage
  void update ( ) ;


  /// Accesseurs en ecriture:
  void setPresetting ( int /*enum Presetting*/ ) ;
  
  void setAlphaCoefs ( float , float , float , float ) ;
  void setSewposCoefs ( float , float , float , float ) ;
  void setSewsDisplay ( bool , bool , bool , bool ) ;
  
  void setInteractive ( bool ) ; 
  void setAlphaCoef ( int , float ) ; 
  void setSewposCoef ( int , float ) ; 
  void setSewDisplay ( int , bool ) ;
  void setSewWidth  ( int , int ) ; 


  /// Accesseurs en lecture
  bool  getInteractive ( ) const ;
  float getAlphaCoef ( int ) const ;
  float getSewposCoef ( int ) const ;
  bool  getSewDisplay ( int ) const;
  int   getSewWidth ( int) const ;

  
protected :
  void applyValues (  ) ;

public slots :
  void callbackBurstView ( int id );
  
  void callbackBurstAlpha0 ( ) ; 
  void callbackSewposAlpha0 ( ) ; 
  void callbackSew0Width ( ) ;
  
  void callbackBurstAlpha1 ( ) ; 
  void callbackSewposAlpha1 ( ) ;
  void callbackSew1Width ( ) ;
  
  void callbackBurstAlpha2 ( ) ;
  void callbackSewposAlpha2 ( ) ;
  void callbackSew2Width ( ) ;
  
  void callbackBurstAlpha3 ( ) ; 
  void callbackSewposAlpha3 ( ) ;
  void callbackSew3Width ( ) ;
  
  void callbackSew0 ( ) ;
  void callbackSew1 ( ) ;
  void callbackSew2 ( ) ;
  void callbackSew3 ( ) ;

  /**
   * Action a effectuer lorsque l'on coche ou decoche la case
   * "Modifications interactive"
   */
  void callbackToggleInteractive ( ) ;

  /**
   * Action a effectuer lorsque l'on appuie sur le bouton
   * "Appliquer modification"
   */
  void callbackApply ( ) ;

  /**
   * Action a effectuer lorsque l'on appuie sur le bouton
   * "Recuperer valeur"
   */
  void callbackRecover ( ) ;

  void callbackPresetting0 ( ) ;
  void callbackPresetting1 ( ) ;
  void callbackPresetting2 ( ) ;
  void callbackPresetting3 ( ) ;
  void callbackPresetting4 ( ) ;
  void callbackPresetting5 ( ) ;
  void callbackPresetting6 ( ) ;
  void callbackPresetting7 ( ) ;

private :

  void setValueEclatement  ( TCoordinate , int i ) ;
  void setValuePosition  ( float , int i ) ;

  OptionsFrame * FParentTab ;
  Window * FParent ;
  bool FUpdate ;

  QGroupBox * FInteractivite, * FTypeEclatement, * FPrereglages , * FParametres ;

  /* --- Interactivite --- */

  QPushButton * FIAppliquer , * FIRecuperer ;

  /* --- Prereglages --- */
  QPushButton * FPCompacte , * FPSemiEclatee , * FPMoka , * FPTopofil , 
    * FPSommets , * FPAretes , * FPFaces , * FPVolumes ;
    
  IdRadioButton * FBurstView [ 2 ] ;

  /* --- Parametres --- */
  QCheckBox * FParCouture      [4];
  QLabel    * FParAlpha        [4];
  QLabel    * FParAffEclatement[4];
  QLabel    * FParAffPosition  [4];
  QLabel    * FAffEclatement;
  QLabel    * FAffPosition;
  QLabel    * FAffEpaisseur;
  QLabel    * FRegTypeEcla;
  QSlider   * FParEclatement[4];
  QSlider   * FParPosition  [4];
  QSpinBox  * FParEpaisseur [4];
};

#endif
