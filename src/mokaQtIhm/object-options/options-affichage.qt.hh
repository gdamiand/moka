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

#ifndef OPTIONSAFFICHAGE_QT_HH
#define OPTIONSAFFICHAGE_QT_HH

//--------- Include autres classes developees ----------
#include "controler-gmap.hh"
#include "options-vue.qt.hh"

//--------------------- Include QT --------------------
#include <QtGui/QSpinBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>

class Window ;

/**
 * Cette classe represente la table affichage
 */
class OptionsAffichage : public QWidget 
{
  Q_OBJECT

public :

  /**
   * Constructeur
   * Construit la table "affichage"
   * @param parent La fenetre principale
   * @param parentTab La tabdialog d'options
   */
  OptionsAffichage ( Window * parent , 
		     QTabWidget *  parentTab ) ;

  /**
   * Destructeur
   * Detruit la table affichage
   */
  ~OptionsAffichage ( ) ;

  /**
   * Met a jour l'affichage de la table
   */
  void update ( ) ;


  void applyValues ( ) ;


  // Accesseurs en lecture

  // Partie elements
  bool getVerticesDisplay ( ) const ;
  bool getSewsDisplay ( ) const ;
  bool getNormalDisplay ( ) const ;
  bool getFacesDisplay ( ) const ;
  bool getFacesCoulAlea ( ) const ;
  bool getVolumesCoulAlea ( ) const;
  bool getCoulFacesGlobale() const;
  bool getCoulTopologie ( ) const;

#ifdef MODULE_ARCHITECTURE
  bool getCoulSemantique( ) const;
  void hideSemantiqueColoration ( );
#endif

  bool getCoulGeometry ( ) const;
  int  getDartWidth ( ) const ;

  // Partie grille
  bool getGridDisplay ( ) const ;
  bool getAxisDisplay() const ;
  int  getSize ( ) const ;
  bool getXy ( ) const ;
  bool getXz ( ) const ;
  bool getYz ( ) const ;
  bool getPx ( ) const ;
  bool getPy ( ) const ;
  bool getPz ( ) const ;


  // Accesseurs en ecriture

  /**
   * Methode a appeler si c'est une autre fenetre
   * qui declenche un evenement gere par cette classe.
   */
  void setFMain ( ) ;

  // Partie Elements
  void setVerticesDisplay ( bool ) ; 
  void setSewsDisplay ( bool ) ;
  void setNormalDisplay ( bool ) ;
  void setFacesDisplay ( bool ) ;
  void setFacesCoulAlea ();
  void setVolumesCoulAlea ();
  void setCoulFacesGlobale();
  void setCoulTopologie();
#ifdef MODULE_ARCHITECTURE
  void setCoulSemantique();
#endif
  void setCoulGeometry();
  void setDartWidth ( int ) ; 

  // Partie grille
  void setGridDisplay ( bool ) ; 
  void setAxisDisplay ( bool ) ;
  void setSize ( int ) ; 
  void setXy ( bool ) ;
  void setXz ( bool ) ; 
  void setYz ( bool ) ;
  void setPx ( bool ) ;
  void setPy ( bool ) ; 
  void setPz ( bool ) ;
   
  // Partie Placages  
  void setOrientation(bool);
  void setScale      (bool);
  void setPosition   (bool);
  
public slots:

  // Partie Elements
  void callbackToggleNormal ( ) ;
  void callbackToggleVertices ( ) ;
  void callbackToggleSews ( ) ;
  void callbackToggleFaces ( ) ;
  void callbackFacesCoulAlea( );
  void callbackVolumesCoulAlea( );
  void callbackCoulFacesGlobale( );
  void callbackCoulTopologie( );
  void callbackCoulGeometry( );
#ifdef MODULE_ARCHITECTURE
  void callbackCoulSemantique( );
 #endif
  // Partie grille
  void callbackToggleGrid ( ) ;
  void callbackToggleAxis ( ) ;
  void callbackSize ( ) ;
  void callbackToggleXy ( ) ;
  void callbackToggleXz ( ) ;
  void callbackToggleYz ( ) ;
  void callbackTogglePx ( ) ;
  void callbackTogglePy ( ) ;
  void callbackTogglePz ( ) ;
  
  // Partie Placages  
  void callbackToggleOrientation();
  void callbackToggleScale      ();
  void callbackTogglePosition   ();

private:

  // La fenetre principale
  Window * FParent ;
 
  bool FUpdate ;

  // Indique si la methode a ete appelee depuis une autre classe
  bool FMain ;

  /* --- Elements --- */
  QGroupBox * FElements ;
  QCheckBox * FENormale , * FECoutures , * FESommets , * FEFacesPleines ;  
  QRadioButton * FECouleursAleatoire , * FECoulVolumesAleatoire , * FECoulGlobale , * FECoulTopologie,* FECoulGeometry;
#ifdef MODULE_ARCHITECTURE
  QRadioButton * FECoulSemantique;
#endif
  QGroupBox * bgroup ,* bgroup2;

  /* --- Grille --- */
  QGroupBox * FGrille ;
  QCheckBox *  FAffAxes , * FPlanXY , * FPlanXZ , * FPlanYZ , 
    * FValOx , * FValOy , * FValOz ;
    
  /* --- Placage --- */
  QGroupBox * FPlacage ;
  QCheckBox * FOrientation , * FTaille , *FPosition ;
  
  QSpinBox * FSaisieTaille ;
} ;

#endif
