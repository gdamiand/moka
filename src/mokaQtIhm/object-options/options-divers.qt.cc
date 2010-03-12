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

#include "options-extrusion.qt.hh"
#include "window.qt.hh"
#include "HtmlEntities.hh"

#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

// Constructeur
//-------------
OptionsDivers :: OptionsDivers ( Window * parent , QTabWidget *  parentTab ) 
  :
  QWidget ( parentTab )  ,
  FParent ( parent ) ,
  FUpdate ( false )
{
  
  QVBoxLayout * glo_hbl = new QVBoxLayout(this) ;
  
  QGroupBox * gb_dep = 
        new QGroupBox(*HTML::decode("Camera moves"));
  QVBoxLayout * dep_vbl = new QVBoxLayout(gb_dep);
  
  
  QWidget * wid_rotation = new QWidget;
  QHBoxLayout * rot_hbl = new QHBoxLayout(wid_rotation) ;
  
  saisie_angle = new FloatSpinBox (5,0,360,2,4) ;
  saisie_angle -> setWrapping ( true ) ;

  rot_hbl -> addWidget (new QLabel ("Rotation angle: "), 0, Qt::AlignLeft);
  rot_hbl -> addWidget (saisie_angle, 1, Qt::AlignLeft);
  
  
  QWidget * wid_pas = new QWidget;
  QHBoxLayout * pas_hbl = new QHBoxLayout(wid_pas) ;
  
  saisie_pas = new FloatSpinBox (1,0,200,2,4) ;
  
  pas_hbl -> addWidget (new QLabel ("Step: "), 0, Qt::AlignLeft);
  pas_hbl -> addWidget (saisie_pas, 1, Qt::AlignLeft);
  
  
  dep_vbl -> addWidget(wid_rotation);
  dep_vbl -> addWidget(wid_pas);
  
  glo_hbl -> addWidget(gb_dep);
  glo_hbl -> addStretch(2);

  
  connect(saisie_angle, SIGNAL(valueChanged(double)),
          this, SLOT(callbackChangeAngle(double)));
  
  connect(saisie_pas, SIGNAL(valueChanged(double)),
          this, SLOT(callbackChangePas(double)));
}


// Destructeur
//------------
OptionsDivers ::  ~OptionsDivers ( ) 
{
}

// Mise a jour de l'affichage
//---------------------------
void OptionsDivers :: update ( ) 
{
  FUpdate = true ;

  CParameterEyePosition * eye = FParent -> getParameterEyePosition();
  saisie_angle -> setValuef(eye -> getAngleRotation());
  saisie_pas -> setValuef(eye -> getPasAvancement());

  FUpdate = false ;
}


void OptionsDivers :: callbackChangeAngle(double val)
{
  if (!FUpdate)
    FParent -> getParameterEyePosition() -> setAngleRotation(val);
}


void OptionsDivers :: callbackChangePas(double val)
{
  if (!FUpdate)
    FParent -> getParameterEyePosition() -> setPasAvancement(val);
}
