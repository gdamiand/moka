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
#ifndef VIEW_PRECOMPILE_HH
#define VIEW_PRECOMPILE_HH
//******************************************************************************
#include "controler-types.hh"
#include "view.hh"
#include "vertex.hh"

#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <list>
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

//******************************************************************************

/** La classe CViewPrecompile définie l'interface de ce que sait faite une
 *  view precompile :
 *  - des méthodes d'ajout et de suppression de precompile
 */

class CParameter;
class CPrecompile;
class CParameterEyePosition;
class CParameterAimedPosition;
class CParameterDrawing;

  //****************************************************************************
class CViewPrecompile : public CView
{
public:
  //@name Constructeur et destructeur.
  //@{
  /// Constructeur
  CViewPrecompile(CParameterEyePosition  * AEyePosition,
		  CParameterAimedPosition* AAimedPosition,
		  CParameterDrawing 	 * ADrawing);

  /// Destructeur
  virtual ~CViewPrecompile();
  //@}

  //@{

  /// Cette méthode met à jour les structures internes s'il y en a.
  virtual void update();

  /**
   *  Cette méthode initialise la scène : loadIdentity pour la MODELVIEW
   *  matrice et effacement de l'écran. Cette méthode n'est pas appelée
   *  automatiquement pour permettre par exemple de superposer des
   *  vue. C'est donc a l'utilisateur de l'appeler avant un drawScene.
   */
  virtual void initScene();
  
  /** 
   *  Cette méthode affiche la scène, en ayant préalablement positionné les
   *  paramètres de visualisation (appel à setScenePosition puis à display)
   */
  virtual void drawScene();

  /// Cette méthode est appelée pour la sélection à la position x,y.
  virtual void pick(int AX, int AY);
  //@}

  //@{

  /** findPrecompile
   *
   *  @param APrecompileType
   *  @return Un pointeur sur le precompile de ce type
   *
   *  Retourne le precompile de type APrecompileType associé à la vue.
   *  Retourne NULL si ce precompile n'existe pas.
   */
  CPrecompile* findPrecompile(TPrecompile APrecompileType);

  /** addPrecompile
   *
   *  @param APrecompile
   *
   *  Ajoute un precompile * dans la liste des precompiles
   */
  void addPrecompile(CPrecompile* APrecompile);

  /** removePrecompile
   *
   *  @param APrecompileType
   *  @return Un precompile
   *
   *  Détache le précompile de type APrecompileType associé à la vue,
   *  et le retourne (ou retourne NULL si la vue n'utilise pas de precompile
   *  de ce type).
   */
  CPrecompile* removePrecompile(TPrecompile APrecompileType);

  /** removePrecompile
   *
   *  @return Un precompile
   *  @param APrecompile
   *
   *  Enlève un precompile * dans la liste des precompiles
   */
  CPrecompile* removePrecompile(CPrecompile* APrecompile);

  //@}

  //@{

  /** getEyePosition
   *
   *  Retourne la position de l'oeil de la vue.
   */
  CParameterEyePosition & getEyePosition() const;

  /** getAimedPosition
   *
   *  Retourne la position du point de mire de la vue (point observé).
   */
  CParameterAimedPosition & getAimedPosition() const;

  /** getLookAt
   *
   *  Retourne la valeur du point de mire.
   */
  CVertex getLookAt();

  /** getEyeDirection
   *
   *  Retourne le vecteur indiquant la direction de l'oeil.
   *  Ce vecteur est calculé avec OpenGL.
   */
  CVertex getEyeDirection();

  /** unproject
   *
   *  @param Ax coordonnées 2D
   *  @param Ay coordonnées 2D
   *  @param ARes paramètre résultat, tableau de 3 float
   *
   *  Calcule le point 3D dont la projection sur "l'écran" est (x,y).
   */
  void unproject(float Ax, float Ay, float ARes[]);

  /** project
   *
   *  @param Ax coordonnées 3D
   *  @param Ay coordonnées 3D
   *  @param Az coordonnées 3D
   *  @param ARes paramètre résultat, tableau de 3 float
   *
   *  Calcule le point 2D étant le projeté sur "l'écran" de (x,y,z).
   */
  void project(float Ax, float Ay, float Az, float ARes[]);

  //@}

  //@{

  CParameterEyePosition*   getParameterEyePosition() const;
  CParameterAimedPosition* getParameterAimedPosition() const;
  CParameterDrawing*       getParameterDrawing() const;

  void setParameterEyePosition  (CParameterEyePosition* AParam);
  void setParameterAimedPosition(CParameterAimedPosition* AParam);
  void setParameterDrawing      (CParameterDrawing* AParam);

  CParameter* getParameter(TParameter AParameterType) const;
  void        setParameter(CParameter* AParameter);

  void groupParameter  (CViewPrecompile* AView, TParameter AParameterType);
  void ungroupParameter(TParameter AParameterType);

  //@}

  //@name Méthodes pour activer ou désactiver la vue
  //@{

  /** enable
   *
   *  Cette méthode active la vue.
   */
  virtual void enable();

  /** disable
   *
   *  Cette méthode désactive la vue.
   */
  virtual void disable();

  //@}

protected:

  /** setScenePositionInitialize
   *
   *  Cette méthode doit être appelée au début de setScenePosition.
   */
  void setScenePositionInitialize();

  /** setScenePosition
   *
   *  Cette méthode positionne les différents paramètres de visualisation
   * (position de la caméra, direction...)
   */
  virtual void setScenePosition() = 0;

  /** setScenePositionFinalize
   *
   *  Cette méthode doit être appelée à la fin de setScenePosition.
   */
  void setScenePositionFinalize();

  /** display
   *
   *  Cette méthode affiche la scène sans aucun positionnement. Elle est
   *  appelée par draw.
   */
  virtual void display();

  /// La position de l'oeil
  CParameterEyePosition   * FParameterEyePosition;

  /// La position du point que l'on regarde
  CParameterAimedPosition * FParameterAimedPosition;

  /// Les paramètres pour le dessin (couleur du fond, des lumières...)
  CParameterDrawing       * FParameterDrawing;

  /// Aspect ratio de la fenêtre (largeur / hauteur) :
  float FRatio;

private:
  /// Matrices OpenGL : (doivent être mises à jour dans setScenePosition)
  GLint FViewport[4];
  GLdouble FModelViewMatrix[16], FProjectionMatrix[16];

  /// La liste des précompilés à afficher
  std::list<CPrecompile*> FListPrecompile;
};

//******************************************************************************
#endif // VIEW_PRECOMPILE_HH
//******************************************************************************
