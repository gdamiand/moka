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

#include "creation-objet.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

//**********************************************
// Constructeur                                *
//**********************************************
CreationObjet :: CreationObjet(Window * parent , QString * nom,
                               GMap3d :: CControlerGMap * controler) :
      QToolBar(*HTML::decode(*nom) , parent),
      FChoix(NULL),
      FControler(controler),
      FParent(parent),
      FPosit(NULL)
{
   setVisible(false);
   parent -> addToolBar(Qt::BottomToolBarArea, this);
   setAllowedAreas(Qt::BottomToolBarArea | Qt::TopToolBarArea);
}
//**********************************************
// Destructeur                                 *
//**********************************************
CreationObjet :: ~CreationObjet() { }
//**********************************************
// Accesseurs                                  *
//**********************************************
GMap3d :: CControlerGMap * CreationObjet :: getControler() const
{
   return FControler ;
}
//**********************************************
// Methode de rafraichissement                 *
//**********************************************
void CreationObjet :: repaint()
{
   QToolBar :: repaint() ;
   FParent -> repaint() ;
}
//**********************************************
// Mise a jour                                 *
//**********************************************
void CreationObjet :: update()
{
   if (FPosit != NULL) FPosit -> update() ;
}
//**********************************************
// Creation                                    *
//**********************************************
void CreationObjet :: create()
{
   if (FChoix != NULL) FChoix -> create() ;
}
//**********************************************
// Fermeture du dialogue                       *
//**********************************************
void CreationObjet :: cancel()
{
   if (FPosit != NULL)
      FPosit -> close() ;

   FControler  -> setMode(MODE_SELECTION) ;
   setVisible(false) ;
}
//**********************************************
// Ouverture de la boite d'options             *
//**********************************************
void CreationObjet :: showOptions()
{
   if (FPosit != NULL)
   {
      if (FPosit -> isVisible())
         FPosit -> hide() ;
      else
         FPosit -> show() ;
   }
}
