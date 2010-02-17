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

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#endif // MODULE_SPAMOD

//******************************************************************************
int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(icones);

#ifdef MODULE_SPAMOD
    glutInit(&argc, argv);
#endif // MODULE_SPAMOD

    //    glutInitDisplayMode(AUX_SINGLE | AUX_RGBA | AUX_DEPTH);
 
    QApplication appli(argc, argv);
    Window moka;

    bool fullscreen = false;
    bool help = false;

    for ( int i=1; i<argc; ++i )
    {
        if ( !strcmp(argv[i],"-fullscreen" ) )
            fullscreen=true;
        else if ( !strcmp( argv [ i ] , "-?") || !strcmp( argv [ i ] , "-h") )
            help = true;
        else if (  !strcmp( argv [ i ] , "-i") )
        {
            if (i+1==argc) help=true;
            else moka.getControler()->importOff(argv[++i]);
        }
        else  moka.getControler()->addMap(argv [ i ]);
    }

    if (help)
    {
        std::cout<<"Usage : mokaQt [-h -?] [-fullscreen] [-i offfile1] ... [-i offfilek] "
                 "[mokafile1 ... mokafilek]"<<std::endl
                 <<"   -h -?: help command."<<std::endl
                 <<"   -fullscreen: put moka in fullscreen mode."<<std::endl
                 <<"   -i offfilei: import off file."<<std::endl
                 <<"   mokafilei: load moka file."<<std::endl;
        exit(EXIT_FAILURE);
    }

    if ( fullscreen ) moka . showFullScreen ( ) ;
    else moka.show();

    return appli.exec();
}
//******************************************************************************
