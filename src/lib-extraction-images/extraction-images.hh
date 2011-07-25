/*
 * lib-extraction-images : Extraction de cartes à partir d'images 2D et 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-extraction-images
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
#ifdef MODULE_EXTRACTION_IMAGE
//******************************************************************************
#ifndef EXTRACTION_IMAGES_HH
#define EXTRACTION_IMAGES_HH
//******************************************************************************
#include "inline-macro.hh"
#include <string>

class CVertex;

#ifndef MODULE_EXTRACTION_IMAGE_WITHOUT_MAGICK
#include <Magick++.h>
#endif

namespace GMap3d
{
  class CDart;
  class CDartVertex;
  class CGMapVertex;
  class CImage3d;

  /** Fonctions permettant d'extraire une carte à partir d'une image 2D ou 3D.
   *  Ce fichier est inclus dans le fichier "g-map-vertex.hh"
   *
   *  @author Guillaume Damiand
   */

  class CExtractionImage
  {
  public:
    //@{

    CExtractionImage(CGMapVertex * AMap);

    ~CExtractionImage();

    //@}

    //@{
    /** extract2dImage
     *
     *  @param AFilename le nom de l'image que l'on veut extraire en G-carte
     *  @return 'vrai' si l'extraction s'est bien déroulé
     *
     *  Construit la carte représentant les frontières des régions de l'image.
     */
    bool extract2dImage(const std::string & AFilename);

    /** extract3dImage
     *
     *  @param AFilename le nom de l'image que l'on veut extraire en G-carte.
     *         ce nom peut contenir un % à l'endroit ou se trouve le numéro de
     *         plan.
     *  @param FirstPlane le numéro du premier plan
     *  @param NbPlaneToRead le nombre de plan à lire. Si 0 on lit tout les
     *         plans consécutifs à partir de FirstPlane.
     *  @param Level le niveau de la carte que l'on veut extraire (-1<=Level<=3)
     *  @param Lg le nombre de chiffre à la place du %
     *  @param DestroyBorder pour détruire le bord initial à la fin de l'algo ou
     *         pas.
     *  @param KeepFictiveEdges pour garder les arêtes fictives ou pas
     *  @param ShiftFictiveEdges pour décaler les arêtes fictives ou pas
     *  @return 'vrai' si l'extraction s'est bien déroulé
     *
     *  Construit la carte représentant les frontières des régions de l'image.
     */
    bool extract3dImage(const std::string & AFilename, int FirstPlane,
			int NbPlaneToRead, int Level, int Lg,
			bool DestroyBorder,
			bool KeepFictiveEdges, bool ShiftFictiveEdges);

    /** extract3dImageCEA
     *
     *  @param AFilename le nom de l'image que l'on veut extraire en G-carte.
     *         ce nom peut contenir un % à l'endroit ou se trouve le numéro de
     *         plan.
     *  @param FirstPlane le numéro du premier plan
     *  @param NbPlaneToRead le nombre de plan à lire. Si 0 on lit tous les
     *         plans consécutifs à partir de FirstPlane.
     *  @param Level le niveau de la carte que l'on veut extraire (-1<=Level<=3)
     *  @param DestroyBorder pour détruire le bord initial à la fin de l'algo ou
     *         pas.
     *  @param KeepFictiveEdges pour garder les arêtes fictives ou pas
     *  @param ShiftFictiveEdges pour décaler les arêtes fictives ou pas
     *  @return 'vrai' si l'extraction s'est bien déroulé
     *
     *  Construit la carte représentant les frontières des régions de l'image.
     */
    bool extract3dImageCEA(const std::string & AFilename, int FirstPlane,
			   int NbPlaneToRead, int Level,
			   bool DestroyBorder, bool KeepFictiveEdges,
			   bool ShiftFictiveEdges );

    /** extract3dImageWithPrecodes
     *
     * @see extract3dImage
     *
     *  Construit la carte représentant les frontières des régions de l'image
     *  mais en utilisant l'algorithme à base de précodes. Même signature que
     *  extract3dImage.
     */
    bool extract3dImageWithPrecodes(const std::string & AFilename,
				    int FirstPlane,
				    int NbPlaneToRead, int Level, int Lg,
				    bool DestroyBorder,
				    bool KeepFictiveEdges,
				    bool ShiftFictiveEdges );

    /** extract3dImageCEAWithPrecodes
     *
     * @see extract3dImageCEA
     *
     *  Construit la carte représentant les frontières des régions de l'image
     *  mais en utilisant l'algorithme à base de précodes. Même signature que
     *  extract3dImage.
     */
    bool extract3dImageCEAWithPrecodes(const std::string & AFilename,
				       int FirstPlane,
				       int NbPlaneToRead, int Level,
				       bool DestroyBorder,
				       bool KeepFictiveEdges,
				       bool ShiftFictiveEdges );

    /** 
     *  @param AFilename le nom de l'image que l'on veut extraire en G-carte.
     *         ce nom peut contenir un % à l'endroit ou se trouve le numéro de
     *         plan.
     *  @param FirstPlane le numéro du premier plan
     *  @param NbPlaneToRead le nombre de plan à lire. Si 0 on lit tous les
     *         plans consécutifs à partir de FirstPlane.
     *  @param Lg le nombre de chiffre à la place du %
     *  @param ARed la composante rouge de la région que l'on veut extraire
     *  @param AGreen la composante rouge de la région que l'on veut extraire
     *  @param ABlue la composante rouge de la région que l'on veut extraire
     *  @param AAlpha la composante rouge de la région que l'on veut extraire
     *  @return 'vrai' si l'extraction s'est bien déroulé
     *
     *  Extrait les voxels (sous formes de cubes) ayant pour couleur
     *  (ARed, AGreen, ABlue, AAlpha). Les autres voxels ne sont pas
     *  représentés dans la carte.
     */
    bool extractOneRegionVoxels(const std::string & AFilename,
				int FirstPlane, 
				int NbPlaneToRead,
				int Lg,
				int ARed, int AGreen, int ABlue, int AAlpha );

    //@}

  private:
    //@{
#ifndef MODULE_EXTRACTION_IMAGE_WITHOUT_MAGICK

    CDart* computeUpFromLast2d(CDart* ADart);

    CDart* createBorder2d(unsigned int rows);
    CDartVertex* createFaceForExtract2d(unsigned int x, unsigned int y);

    bool samePixelActuLeft2d(Magick::Image & image,
			     unsigned int x, unsigned int y);
    bool samePixelActuUp2d  (Magick::Image & image,
			     unsigned int x, unsigned int y);

    void mergeImage2dFaces(CDart* ADart);
    void mergeImage2dEdges(CDart* ADart);

    bool isDegre2Vertex2d(CDart* ADart);

#endif // MODULE_EXTRACTION_IMAGE_WITHOUT_MAGICK

    bool areEdgesAlign(CDart* ADart, CDart* ADart2);

    /** extract3dImage
     *
     *  @param AImage l'image dont on veut extraire la G-carte.
     *  @param Level le niveau de la carte que l'on veut extraire (-1<=Level<=3)
     *  @param DestroyBorder pour détruire le bord initial à la fin de l'algo ou
     *         pas.
     *  @param KeepFictiveEdges pour garder les arêtes fictives ou pas
     *  @param ShiftFictiveEdges pour décaler les arêtes fictives ou pas
     *  @return 'vrai' si l'extraction s'est bien déroulé
     *
     *  Construit la carte représentant les frontières des régions de l'image.
     */
    bool extract3dImage(CImage3d * AImage, int ALevel = 3,
			bool ADestroyBorder = true,
			bool AKeepFictiveEdges = true,
			bool AShiftFictiveEdges = true);

    /** 
     *  @param AImage l'image dont on veut extraire la G-carte.
     *  @param ARed la composante rouge de la région que l'on veut extraire
     *  @param AGreen la composante rouge de la région que l'on veut extraire
     *  @param ABlue la composante rouge de la région que l'on veut extraire
     *  @param AAlpha la composante rouge de la région que l'on veut extraire
     *  @return 'vrai' si l'extraction s'est bien déroulé
     *
     *  Extrait les voxels (sous formes de cubes) ayant pour couleur
     *  (ARed, AGreen, ABlue, AAlpha). Les autres voxels ne sont pas
     *  représentés dans la carte.
     */
    bool extractOneRegionVoxels(CImage3d * AImage,
				int ARed, int AGreen, int ABlue, int AAlpha );

    CDart* computeUpFromLast3d    (CDart* ADart);
    CDart* computeBehindFromLast3d(CDart* ADart);

    CDart*  createBorder3d(unsigned int columns, unsigned int rows);
    CDartVertex* createFaceForExtract3d();
    CDartVertex* createFaceForExtract3d( const CVertex & AVertex );
    CDartVertex* createCubeForExtract3d( const CVertex & AVertex );
    void    sew3Cube(CDart* ADart, CDart* TheLastDart);

    void mergeImage3dVolumes(CDart* ADart);
    void mergeImage3dFaces  (CDart* ADart);
    void mergeImage3dEdges  (CDart* ADart);

    bool   isDegre2Edge3d           (CDart* ADart);
    bool   isDegre2Vertex3d         (CDart* ADart);
    CDart* isRealDegre2Vertex3d     (CDart* ADart, int AMark);
    bool   areFacesColinear         (CDart* ADart);
    bool   faceMergingWillDisconnect(CDart* ADart);

    bool extract3dImageWithPrecodes(CImage3d * AImage, int ALevel = 3,
				    bool ADestroyBorder = true,
				    bool AKeepFictiveEdges = true,
				    bool AShiftFictiveEdges = true);

    CDart* newEdge();
    void   closeFace( CDart* d1, CDart* d2,CDart* d3 );
    void   closeFace( CDart* d1, CDart* d2 );
    void   closeFace( CDart* d );

    void shiftFictiveEdgesForFacesXy( CDart* last, CDart* up, CDart* behind,
				      int FictiveMark );
    void shiftFictiveEdgesForFacesXz( CDart* last, CDart* up, CDart* behind,
				      int FictiveMark );
    void shiftFictiveEdgesForFacesYz( CDart* last, CDart* up, CDart* behind,
				      int FictiveMark );

    // Pour extraire la carte de niveau 1
    CDart* oneSameNeighboorV( CDart* d1, CDart* fo1, CDart* fo2,
			      const CVertex & AVertex );
    CDart* twoSameNeighboorV( CDart* d1, CDart* d5 , CDart* fo ,
			      const CVertex & AVertex );

    void precodeL1( CDart* &last, CDart* up, CDart* behind,
		    const CVertex & AVertex );
    void precodeL2( CDart* &last, CDart* up, CDart* behind,
		    const CVertex & AVertex );
    void precodeL3( CDart* &last, CDart* up, CDart* behind,
		    const CVertex & AVertex );
    void precodeL4( CDart* &last, CDart* up, CDart* behind,
		    const CVertex & AVertex );
    void precodeL5( CDart* &last, CDart* up, CDart* behind,
		    const CVertex & AVertex );
    void precodeL6( CDart* &last, CDart* up, CDart* behind,
		    const CVertex & AVertex );
    void precodeL7( CDart* &last, CDart* up, CDart* behind,
		    const CVertex & AVertex );
    void precodeL8( CDart* &last, CDart* up, CDart* behind,
		    const CVertex & AVertex );

    void subcasePrecodeL1( CImage3d & image,
			   unsigned int x, unsigned int y,
			   CDart* &last, CDart* up, CDart* behind,
			   int level, const CVertex & AVertex, int FictiveMark,
			   bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeL2( CImage3d & image,
			   unsigned int x, unsigned int y,
			   CDart* &last, CDart* up, CDart* behind,
			   int level, const CVertex & AVertex, int FictiveMark,
			   bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeL3( CImage3d & image,
			   unsigned int x, unsigned int y,
			   CDart* &last, CDart* up, CDart* behind,
			   int level, const CVertex & AVertex, int FictiveMark,
			   bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeL4( CImage3d & image,
			   unsigned int x, unsigned int y,
			   CDart* &last, CDart* up, CDart* behind,
			   int level, const CVertex & AVertex, int FictiveMark,
			   bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeL5( CImage3d & image,
			   unsigned int x, unsigned int y,
			   CDart* &last, CDart* up, CDart* behind,
			   int level, const CVertex & AVertex, int FictiveMark,
			   bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeL6( CImage3d & image,
			   unsigned int x, unsigned int y,
			   CDart* &last, CDart* up, CDart* behind,
			   int level, const CVertex & AVertex, int FictiveMark,
			   bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeL7( CImage3d & image,
			   unsigned int x, unsigned int y,
			   CDart* &last, CDart* up, CDart* behind,
			   int level, const CVertex & AVertex, int FictiveMark,
			   bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeL8( CImage3d & image,
			   unsigned int x, unsigned int y,
			   CDart* &last, CDart* up, CDart* behind,
			   int level, const CVertex & AVertex, int FictiveMark,
			   bool keepFictiveEdges, bool shiftFictiveEdges );

    // Pour extraire la carte de niveau 2
    CDart* oneSameNeighboorFc ( const CVertex & AVertex,
				CDart* d1, CDart* fo1, CDart* fo2,
				int fusionCase );
    CDart* twoSameNeighboorFc1( const CVertex & AVertex,
				CDart* d1, CDart* d5, CDart* fo );
    CDart* twoSameNeighboorFc2( const CVertex & AVertex,
				CDart* d1, CDart* d5, CDart* fo );
    CDart* twoSameNeighboorFc3( const CVertex & AVertex,
				CDart* d1, CDart* d5, CDart* fo );

    void precodeFc1 ( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc2 ( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc3 ( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc4 ( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc5 ( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc6 ( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc7 ( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc8 ( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc9 ( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc10( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc11( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc12( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc13( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc14( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc15( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc16( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc17( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );
    void precodeFc18( CDart* &last, CDart* up, CDart* behind,
		      const CVertex & AVertex );

    void subcasePrecodeFc1 ( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc2 ( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc3 ( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc4 ( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc5 ( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc6 ( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc7 ( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc8 ( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc9 ( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc10( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc11( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc12( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc13( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc14( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc15( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc16( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc17( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeFc18( CImage3d & image,
			     unsigned int x, unsigned int y,
			     CDart* &last, CDart* up, CDart* behind,
			     int level, const CVertex & AVertex,
			     int FictiveMark,
			     bool keepFictiveEdges, bool shiftFictiveEdges );

    // Pour extraire la carte de niveau 3
    CDart* oneSameNeighboorF  (CDart* d1, CDart* fo1, CDart* fo2,
			       const CVertex & AVertex );
    CDart* twoSameNeighboorF1 (CDart* d1, CDart* d5,  CDart* fo,
			       const CVertex & AVertex );
    CDart* twoSameNeighboorF2 (CDart* d1, CDart* d5,  CDart* fo,
			       const CVertex & AVertex );
    CDart* threeSameNeighboorF(CDart* d1, CDart* d5,  CDart* fo,
			       const CVertex & AVertex );

    void precodesF1F4F7F10( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodesF2F5F8F11( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodesF3F6F9F12( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodesF13F19   ( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodesF14F20   ( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodesF15F21   ( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodesF16F22   ( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodesF17F23   ( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodesF18F24   ( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodeF25       ( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodeF26       ( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );
    void precodeF27       ( CDart* &last, CDart* up, CDart* behind,
			    const CVertex & AVertex );


    void subcasePrecodesF1F4F7F10( CImage3d & image,
				   unsigned int x, unsigned int y,
				   CDart* &last, CDart* up, CDart* behind,
				   int level, const CVertex & AVertex,
				   int FictiveMark,
				   bool keepFictiveEdges,
				   bool shiftFictiveEdges );
    void subcasePrecodesF2F5F8F11( CImage3d & image,
				   unsigned int x, unsigned int y,
				   CDart* &last, CDart* up, CDart* behind,
				   int level, const CVertex & AVertex,
				   int FictiveMark,
				   bool keepFictiveEdges,
				   bool shiftFictiveEdges );
    void subcasePrecodesF3F6F9F12( CImage3d & image,
				   unsigned int x, unsigned int y,
				   CDart* &last, CDart* up, CDart* behind,
				   int level, const CVertex & AVertex,
				   int FictiveMark,
				   bool keepFictiveEdges,
				   bool shiftFictiveEdges );
    void subcasePrecodesF13F19( CImage3d & image,
				unsigned int x, unsigned int y,
				CDart* &last, CDart* up, CDart* behind,
				int level, const CVertex & AVertex,
				int FictiveMark,
				bool keepFictiveEdges,
				bool shiftFictiveEdges );
    void subcasePrecodesF14F20( CImage3d & image,
				unsigned int x, unsigned int y,
				CDart* &last, CDart* up, CDart* behind,
				int level, const CVertex & AVertex,
				int FictiveMark,
				bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodesF15F21( CImage3d & image,
				unsigned int x, unsigned int y,
				CDart* &last, CDart* up, CDart* behind,
				int level, const CVertex & AVertex,
				int FictiveMark,
				bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodesF16F22( CImage3d & image,
				unsigned int x, unsigned int y,
				CDart* &last, CDart* up, CDart* behind,
				int level, const CVertex & AVertex,
				int FictiveMark,
				bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodesF17F23( CImage3d & image,
				unsigned int x, unsigned int y,
				CDart* &last, CDart* up, CDart* behind,
				int level, const CVertex & AVertex,
				int FictiveMark,
				bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodesF18F24( CImage3d & image,
				unsigned int x, unsigned int y,
				CDart* &last, CDart* up, CDart* behind,
				int level, const CVertex & AVertex,
				int FictiveMark,
				bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeF25( CImage3d & image,
			    unsigned int x, unsigned int y,
			    CDart* &last, CDart* up, CDart* behind,
			    int level, const CVertex & AVertex, int FictiveMark,
			    bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeF26( CImage3d & image,
			    unsigned int x, unsigned int y,
			    CDart* &last, CDart* up, CDart* behind,
			    int level, const CVertex & AVertex, int FictiveMark,
			    bool keepFictiveEdges, bool shiftFictiveEdges );
    void subcasePrecodeF27( CImage3d & image,
			    unsigned int x, unsigned int y,
			    CDart* &last, CDart* up, CDart* behind,
			    int level, const CVertex & AVertex, int FictiveMark,
			    bool keepFictiveEdges, bool shiftFictiveEdges );

    CGMapVertex * FMap;
    bool FKeepFictiveEdges;
    bool FShiftFictiveEdges;
    //@}
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("extraction-images.icc")
//******************************************************************************
#endif // EXTRACTION_IMAGES_HH
//******************************************************************************
#endif // MODULE_EXTRACTION_IMAGE
//******************************************************************************
