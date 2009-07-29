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
#ifndef VIEW_HH
#define VIEW_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */
//******************************************************************************

/** La classe CView définie l'interface de ce que sait faite une view :
 *    - des méthodes d'affichage (affichage, mise à jour, position dans la
 *      scène, dessin du contenu)
 *    - des méthodes de conversion 2D <=> 3D
 *  Chaque classe vue doit avoir un identifiant unique.
 */

  class CView
  {
  public:
    //@name Constructeurs et destructeur.
    //@{
    /// Constructeur par défaut
    CView();

    /// Destructeur
    virtual ~CView();
    //@}

    //@name Méthodes pour l'affichage
    //@{

    /**
     *  Cette méthode initialise la scène (par exemple loadIdentity pour la
     *  MODELVIEW matrice, effacement de l'écran...). Cette méthode n'est pas
     *  appelée automatiquement pour permettre par exemple de superposer des
     *  vue. C'est donc a l'utilisateur de l'appeler avant un drawScene.
     */
    virtual void initScene() = 0;

    /**
     *  Cette méthode affiche la scène, en ayant préalablement positionné les
     *  paramètres de visualisation (appel à setScenePosition)
     */
    virtual void drawScene() = 0;

    /// Cette méthode met à jour les structures internes s'il y en alpha.
    virtual void update() = 0;

    //@}

    //@name Méthodes pour le picking
    //@{

    /// Cette méthode est appelée pour la sélection à la position x,y.
    virtual void pick(int AX, int AY) = 0;

    //@}


    //@name Méthodes pour la conversion 2D <=> 3D
    //@{
    
    /**
     *  Calcule le point 3D dont la projection sur "l'écran" est (x,y).
     *
     *  @param x coordonnées 2D
     *  @param y coordonnées 2D
     *  @param res paramètre résultat, tableau de 3 float
     */
    virtual void unproject(float x, float y, float res[]) = 0;

    /**
     *  Calcule le point 2D étant le projeté sur "l'écran" de (x,y,z).
     *
     *  @param x coordonnées 3D
     *  @param y coordonnées 3D
     *  @param z coordonnées 3D
     *  @param res paramètre résultat, tableau de 2 float
     */
    virtual void project(float x, float y, float z, float res[]) = 0;

    //@}

    //@name Méthodes pour activer ou désactiver la vue
    //@{

    /// Cette méthode active la vue.
    virtual void enable();

    /// Cette méthode désactive la vue.
    virtual void disable();

    /// Cette méthode teste si la vue est active ou non.
    virtual bool isEnabled();

    //@}

  protected:

    //@name Méthodes protected devant être utilisées uniquement par la méthode
    //      drawScene
    //@{

    /**
     *  Cette méthode positionne les différents paramètres de visualisation
     *  (position de la caméra, direction...)
     */
    virtual void setScenePosition() = 0;

    /**
     *  Cette méthode affiche la scène sans aucun positionnement. Elle est
     *  appelée par drawScene.
     */
    virtual void display() = 0;

    //@}

  private:
    /// Flag pour tester si la vue est active ou non.
    bool FEnable;
  };

//******************************************************************************
#endif // VIEW_HH
//******************************************************************************
