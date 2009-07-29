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

#include "dialog-do.qt.hh"
#include "window.qt.hh"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>

//**********************************************
// Constructeur
//**********************************************

DialogDo :: DialogDo ( Window * parent )
  :
  QDialog ( parent ),
  FParent ( parent )
{
  // Construction des widgets
  QVBoxLayout * placement = new QVBoxLayout ( this ) ;
  placement -> setContentsMargins ( 20, 20, 20, 20 );
  placement -> setSpacing ( 5 );  
  
  QWidget * widhaut = new QWidget ( this ) ;
  QHBoxLayout * haut = new QHBoxLayout ( widhaut ) ;
  FMax = new QLabel ( "Nombre max" , widhaut ) ;
  
  FNombre = new QSpinBox ( widhaut ) ;
  FNombre -> setMinimum ( 0 );
  FNombre -> setMaximum ( 999 );
  FNombre ->setAccelerated(true);
  
  haut -> addWidget ( FMax ) ;
  haut -> addWidget ( FNombre ) ;  
  FFichiers = new QCheckBox ( "Fichiers" , this ) ;
  FFichiers -> setChecked ( parent -> getControler() -> getUndoOnFile() ) ;
  placement -> addWidget ( widhaut ) ;
  placement -> addWidget ( FFichiers ) ;
  setWindowTitle ( "Undo / Redo" ) ;

  // Initialisation
  FNombre -> setValue ( 100 ) ;

  // Mise en place des ecoutes
  QObject :: connect ( FNombre , SIGNAL ( valueChanged ( int ) ) , this ,
		       SLOT ( callbackNbUndos ( ) ) ) ;
  QObject :: connect ( FFichiers , SIGNAL ( clicked ( ) ) , this ,
		       SLOT ( callbackToggleOnFile ( ) ) ) ;
  
  FClose = new QPushButton ( "fermer" , this ) ;
  placement -> addWidget ( FClose ) ;
  FClose -> setFocus ( ) ;
  QObject :: connect ( FClose ,
		       SIGNAL ( clicked ( ) ) ,
		       this ,
		       SLOT ( close ( ) ) ) ;

}


//***********************************************
// Destructeur
//***********************************************

DialogDo :: ~DialogDo ( ) { }


//***********************************************
// Slots
//***********************************************

void DialogDo :: callbackToggleOnFile ( )
{
  FParent -> getControler ( ) -> setUndoOnFile ( FFichiers -> isChecked ( ) ) ;
}
void DialogDo :: callbackNbUndos ( )
{
  FParent -> getControler ( ) -> setNbMaxUndos ( FNombre -> value ( ) ) ;
}

