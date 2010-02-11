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

#include "select-bar.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QVBoxLayout>
#include <QtGui/QToolTip>

#define NB_SELECTION_BOUTONS 16

//*****************************
// Constantes
//*****************************
static const char * const mokaLabels [ NB_SELECTION_BOUTONS ] =
{
   "B",
   "0", "1", "2", "3",
   "01*", "02*", "03*",
   "12", "13", "23",
   "S", "A*", "F*", "V*", "C"
} ;

static const TOrbit mokaOrbit [ NB_SELECTION_BOUTONS ] =
{
   ORBIT_SELF,
   ORBIT_0, ORBIT_1, ORBIT_2, ORBIT_3,
   ORBIT_01 , ORBIT_02 , ORBIT_03,
   ORBIT_12 , ORBIT_13 , ORBIT_23,
   ORBIT_123, ORBIT_023, ORBIT_013, ORBIT_012, ORBIT_0123
} ;

static const char * const mokaTooltips [ NB_SELECTION_BOUTONS ] =
{
   "[----: Brin]",
   "[0---]", "[-1--]", "[--2-]", "[---3]",
   "[01--: Face/Volume (ou demi-face)]",
   "[0-2-: Ar&ecirc;te/Volume]",
   "[0--3: Ar&ecirc;te/Face (ou demi-ar&ecirc;te)]",
   "[-12-: Sommet/Volume]", "[-1-3: Sommet/Face]",
   "[--23: Sommet/Ar&ecirc;te (ou demi-sommet)]",
   "[-123: Sommet]", "[0-23: Ar&ecirc;te]", "[01-3: Face]",
   "[012-: Volume]",
   "[0123: Composante connexe]"
} ;

//******************************
// Constructeur
//******************************

SelectBar :: SelectBar(Window * parent)
      :
      QToolBar(*HTML::decode("S&eacute;lection") , parent),
      FControler(parent -> getControler()),
      FParent(parent)
{
   setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);

   // Mise en place d'un layout
   //--------------------------
   QWidget * total = new QWidget(this) ;
   QVBoxLayout * bl = new QVBoxLayout(total) ;
   bl -> setSpacing(0);

   // Creation des boutons
   //---------------------
   FGroupe = new QGroupBox(total) ;
   QVBoxLayout * gpl = new QVBoxLayout(FGroupe) ;
   IdRadioButton * rb_tab[NB_SELECTION_BOUTONS];

   for (int i = 0 ; i < NB_SELECTION_BOUTONS ; i++)
   {
      rb_tab[i] = new IdRadioButton(i, mokaLabels[i]) ;
      gpl -> addWidget(rb_tab[i]);

      connect(rb_tab[i], SIGNAL(clicked(int)),
              this, SLOT(callBackClic(int))) ;
   }

   rb_tab[0] -> setChecked(true);

   // Mise en place des tooltips
   for (int i = 0 ; i < NB_SELECTION_BOUTONS ; i++)
      rb_tab[i] -> setToolTip(*HTML::decode(mokaTooltips [ i ])) ;

   // Boutons de niveau de selection (1, 2 ou 3)
   //-------------------------------------------

   // Disposition
   QWidget * sel = new QWidget ;
   sel -> setToolTip(*HTML::decode("Niveau de s&eacute;lection")) ;

   QHBoxLayout * bh = new QHBoxLayout(sel) ;
   bh -> setSpacing(0);

   // Allocation
   int nb_selection_lev = FControler -> getNbSelectionLevels();
   FNiveaux = new IdPushButton * [ nb_selection_lev ] ;

   // Creation
   for (int i = 0;i < nb_selection_lev;i++)
   {
      FNiveaux [ i ] = new IdPushButton(i, QString::number(i + 1)) ;
      bh -> addWidget(FNiveaux [ i ]) ;

      connect(FNiveaux[i], SIGNAL(clicked(int)),
              this, SLOT(callbackSelectionLevel(int))) ;
   }

   FNiveaux [ 0 ] -> setEnabled(false) ;
  
   // Le bouton block
   FBlock = new QPushButton("Block");
   FBlock->setCheckable(true);
   connect(FBlock, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleBlock(bool))); 
   
   // Le bouton Simplif
   FSimplif = new QPushButton("Simplif");
   FSimplif->setCheckable(true);
   connect(FSimplif, SIGNAL(toggled(bool)), this, 
           SLOT(callbackToggleSimplificationMode(bool))); 
   
   // Ajout au layout principal
   bl -> addWidget(FGroupe) ;
   bl -> addWidget(sel) ;
   bl -> addWidget(FBlock) ;
   bl -> addWidget(FSimplif) ;

   total -> setMaximumWidth(90);
   addWidget(total);
}

//**********************************
// Destructeur
//**********************************
SelectBar :: ~SelectBar() { }
//*********************************
// Slot
//*********************************
void SelectBar::callBackClic(int ind)
{
   FControler->setSelectionOrbit(mokaOrbit [ ind ]) ;
   FParent->repaint() ;
}

static bool freezed = false ;

void SelectBar::callbackSelectionLevel(int id)
{
   if ( !freezed )
      setSelectionLevel(id) ;
}

void SelectBar :: setSelectionLevel(int L , bool freeze)
{
   if (! freeze)
   {
      FControler -> setSelectionLevel(L) ;
      FParent -> repaint() ;
   }

   freezed = true;

   for (int i = 0 ; i < FControler -> getNbSelectionLevels() ; i++)
   {
      if (FControler -> getSelectionLevel() == i)
         FNiveaux [ i ] -> setEnabled(false) ;
      else
         FNiveaux [ i ] -> setEnabled(true) ;
   }

   freezed = false;
}

void SelectBar :: niveauSuivant()
{
   for (int i = 0 ; i < FControler -> getNbSelectionLevels() ; i++)
   {
      if (! FNiveaux [ i ] -> isEnabled())
      {
         setSelectionLevel((i + 1) % FControler->getNbSelectionLevels()) ;
         return ;
      }
   }
}

void SelectBar::callbackToggleBlock( bool ABlock )
{
   FControler->toggleModelBlocked();
   if ( !ABlock ) FParent->repaint(); 
}      

void SelectBar::callbackToggleSimplificationMode( bool ABlock )
{
   FControler->toggleModeSimplification();
   if ( !ABlock ) FParent->repaint(); 
}      




