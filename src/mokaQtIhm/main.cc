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

//******************************************************************************
#include <QtGui/QApplication>
#include "window.qt.hh"
#include <cstring>

#ifdef MODULE_SPAMOD
#ifdef _WINDOWS
#include <windows.h>
#endif
#include <GL/glut.h>
#endif // MODULE_SPAMOD

//******************************************************************************
int main(int argc, char** argv)
{
  Q_INIT_RESOURCE(icones);
  
#ifdef MODULE_SPAMOD
  glutInit(&argc, argv);
#endif // MODULE_SPAMOD
  
  QApplication appli(argc, argv);
  Window moka ;

  if ( argc == 2 )
  {
    if ( strcmp ( argv [ 1 ] , "-FULLSCREEN" ) == 0 )
	    moka . showFullScreen ( ) ;
    else
	  {
	    cout << "Option inconnue : " << argv [ 1 ] << endl ;
	    return -1 ;
	  }
  }
  else moka.show();
  
  return appli.exec();
}
//******************************************************************************
