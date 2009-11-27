/*
 * lib-controler : Un contrôleur générique de scène 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//******************************************************************************
#ifndef PRECOMPILE_HH
#define PRECOMPILE_HH
//******************************************************************************
#include "controler-types.hh"
#include "parameter.hh"

#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

//******************************************************************************
#define PLOT( a ) \
	(glVertex3f((a)->getX(), (a)->getY(), (a)->getZ()))

#define LINE(a,b) (PLOT(a), PLOT(b))
//******************************************************************************

/** La classe CPrecompile définie l'interface de ce que sait faite une precompile :
 *  - des méthodes d'affichage (affichage et mise à jour)
 *  - des méthodes pour fixer et tester si la mise à jour est nécessaire
 *
 * Un objet CPrecompile est en quelque sorte une liste openGL correspondant à un
 * objet particulier. Par exemple une liste de brin, de sommets, pour la grille...
 * Chaqu'un de ces objets appartient à certains paramètres précompile et lorsque
 * un de ces paramètres est mis à jour, il positionne alors le flag FToUpdate.
 * Chaque classe de précompile doit avoir un identifiant différent.
 */

//******************************************************************************
class CView;

class CPrecompile
{
public:
  //@name Constructeurs et destructeur.
  //@{
  /** Constructeur par défaut
   */
  CPrecompile( unsigned int ANb=0 );

  /** Constructeur par recopie
   */
  CPrecompile( const CPrecompile & );

  /** Destructeur
   */
  virtual ~CPrecompile();

  /** Equivalent d'un constructeur par recopie, mais en méthode virtuelle pour
   *   le polymorphisme.
   */
  virtual CPrecompile * copy() const = 0;
  //@}

  //@name Méthodes virtuelles pures à implémenter dans les classes filles.
  //@{

  //@name Méthodes pour l'affichage
  //@{

  /// @return le type du precompile.
  virtual TPrecompile getType() const = 0;

  /// Cette méthode affiche l'objet correspondant.
  void draw();

  /// Cette méthode met à jour les structures internes correspondant à l'objet
  /// lorsque cela est nécessaire (test sur FToUpdate et appel à updateModel).
  void update();

  /// Cette méthode est appelée pour la sélection à la position x,y.
  /// Par défaut, elle ne fait rien. Elle est surchargé que pour les précompiles
  /// voulant sélectionner quelque chose.
  virtual void pick(int, int, CView*);

  //@}

  /** setParameter
   *
   *  Cette méthode affecte le paramètre AParameter à ce précompile uniquement
   *  lorsque le précompile possède un paramètre de type AParameter->getType().
   *  Sinon ne fait rien.
   */
  virtual void setParameter(CParameter * AParameter) = 0;

  /** getParameter
   *
   *  Cette méthode retourne le paramètre précompile associé à ce précompile,
   */
  virtual CParameter* getParameter() const = 0;

  //@name Méthodes pour gérer la mise à jour
  //@{

  /** needToUpdate
   *
   *  Cette méthode retourne si l'objet correspondant doit être mis à jour.
   *  Le flag correspondant est positionné automatiquement par les classes
   *  CParameterPrecompile qui ont cette classe dans leur liste.
   */
  bool needToUpdate();

  /** setToUpdate
   *
   *  Positionne le flag FToUpdate.
   */
  void setToUpdate();

  /** unsetToUpdate
   *
   *  Enleve le flag FToUpdate.
   */
  void unsetToUpdate();

  //@}

  //@name Méthodes pour activer ou désactiver le précompile
  //@{

  /** enable
   *
   *  Cette méthode active le précompile, et positionne le flag FToUpdate.
   */
  void enable();

  /** disable
   *
   *  Cette méthode désactive le précompile, vide la liste opengl et enleve le
   *  flag FToUpdate. Tant que le précompile est "disable", il n'est plus mis
   *  a jour.
   */
  void disable();

  /** isEnabled
   *
   *  Cette méthode teste si le précompile est actif ou non.
   */
  bool isEnabled();

  //@}

  //@name Méthodes pour compter le nombre de vue totale contenant ce précompile
  //@{
  unsigned int getNbView();
  void         incNbView(unsigned int ADec=1);
  void         decNbView(unsigned int ADec=1);
  //@}

  //@name Méthodes pour compter le nombre de vue active contenant ce précompile
  //@{
  unsigned int getNbEnabledView();
  void         incNbEnabledView(unsigned int ADec=1);
  void         decNbEnabledView(unsigned int ADec=1);
  //@}

protected:

  /** compileModel
   *
   *  Cette méthode met à jour les structures internes correspondant à l'objet.
   *  Appelée par update quand le flag FToUpdate est vrai.
   */
  void compileModel();

  /** updateModel
   *
   *  Cette méthode met à jour les structures internes correspondant à l'objet.
   *  Appelée par update quand le flag FToUpdate est vrai.
   */
  virtual void drawModel() = 0;

private:
  /// Flag pour tester si l'objet doît être mis à jour
  bool FToUpdate;

  /// Flag pour tester si le précompile est actif ou non.
  bool FEnable;

  /// Nombre de vues totales dans lequel est ce precompile
  unsigned int FNbView;

  /// Nombre de vues actives dans lequel est ce precompile
  unsigned int FNbEnabledView;

  /// La liste compilée OpenGL
  GLint FCompiledList;
};

//******************************************************************************
#endif // PRECOMPILE_HH
//******************************************************************************
