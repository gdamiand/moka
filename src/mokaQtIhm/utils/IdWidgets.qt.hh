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

#ifndef _IDWIDGETS_H
#define _IDWIDGETS_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QAction>
#include <QtWidgets/QWidget>


// Des QPushButton avec un identifiant.

class IdPushButton : public QPushButton
{
  Q_OBJECT
          
  public:
  
    IdPushButton ( int id, QWidget * parent = 0 ) ;
    IdPushButton ( int id, const QString & text, QWidget * parent = 0 ) ;
    IdPushButton ( int id, const QIcon & icon, const QString & text, 
                   QWidget * parent = 0 ) ;
    ~IdPushButton ( ) ;
    
  signals:
  
    void clicked(int id);
    
  private slots:
    
    void emitClicked();
    
  private:
    
    int id;
};



// Des QRadioButton avec un identifiant.

class IdRadioButton : public QRadioButton
{
  Q_OBJECT
          
  public:
  
    IdRadioButton ( int id, QWidget * parent = 0 ) ;
    IdRadioButton ( int id, const QString & text, QWidget * parent = 0 ) ;
    ~IdRadioButton ( ) ;
    
  signals:
  
    void clicked(int id);
    
  private slots:
    
    void emitClicked();
    
  private:
    
    int id;
};



// Des QAction avec un identifiant.

class IdAction : public QAction
{
  Q_OBJECT
          
  public:
  
    IdAction ( int id, QObject * parent ) ;
    IdAction ( int id, const QString & text, QObject * parent ) ;
    IdAction ( int id, const QIcon & icon, const QString & text, 
               QObject * parent );
    
    
    ~IdAction ( ) ;
    
  signals:
  
    void clicked(int id);
    
  private slots:
    
    void emitClicked();
    
  private:
    
    int id;
};

#endif
