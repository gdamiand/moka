/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-gmapkernel
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
#include "g-map-generic.hh"
using namespace GMap3d;
//******************************************************************************
void CGMapGeneric::createTopoMesh1(int ASx,
				   CDart * AMesh1Corners[2],
				   CDart * ADart)
{
  /* Le maillage créé est un faisceau.
   * Ce faiseau est composé de m brins "au-dessus" de AExtremity1 et
   * n brins "au-dessous" (voir schéma).
   *
   * Le brin du haut et celui du bas sont éventuellement 3-cousus (le faisceau
   * est alors fermé).
   *
   * Ici, m=2 et n=4.
   *
   *  -----------------------------
   *      | 3	
   *  -----------------------------
   *      | 2	
   *  ============================= AExtremity1
   *      | 3	
   *  -----------------------------
   *      | 2	
   *  -----------------------------
   *      | 3	
   *  -----------------------------
   *      | 2	
   *  -----------------------------
   *
   * Remarque: On peut s'appuyer sur la méthode 'insertVertex', mais le maillage
   * est plus rapide si on crée tous les brins et si on fait toutes les coutures
   * "à la main".
   * Or 'createTopoMesh1' est une méthode de base qui est utilisée par les
   * méthodes de maillage de dimension supérieure.
   * Sa complexité en temps doit donc être optimisée.
   */

  assert(ASx>=0);
  assert(AMesh1Corners!=NULL);

  // On détermine d'abord m,n et closed (booléen indiquant si le faisceau est
  // fermé):
  int m = 0, n = 0;
  bool closed;

  if (ADart==NULL)
    closed = false;
  else
    {
      CDart * last = NULL;
      bool jumped = false;

      for (CDynamicCoverage23 cov(this, ADart); cov.cont(); ++cov)
	{
	  last = *cov;

	  switch (cov.prevOperationType())
	    {
	    case OP_ALPHAI:
	    case OP_ALPHAJ:
	      if (jumped) ++n; else ++m;
	      break;

	    case OP_JUMP:
	      jumped = true;
	      ++n;
	      break;

	    case OP_NONE:
	      break;
	    }
	}

      closed = !isFree2(last) && !isFree3(last);
    }

  // Création du maillage:
  int i, j;
  int perimeter = m + n + 1;
  int length    = ASx==0 ? 1 : 2*ASx;

  CDart *** beam = new CDart ** [length];

  for (j=0; j<length; ++j)
    {
      beam[j] = new CDart * [perimeter];

      for (i=0; i<perimeter; ++i)
	beam[j][i] = addMapDart();
    }

  // 0 et 1-coutures:
  for (i=0; i<perimeter; ++i)
    for (j=1; j<length; j+=2)
      {
	linkAlpha0(beam[j-1][i], beam[j][i]);

	if (j<length-1)
	  linkAlpha1(beam[j][i], beam[j+1][i]);
      }

  // 2 et 3-coutures:
  bool alpha2;

  // Au-dessus de AExtremity1:
  for (i=m, alpha2=true; i>0; --i, alpha2=!alpha2)
    if (alpha2)
      for (j=0; j<length; ++j)
	linkAlpha2(beam[j][i], beam[j][i-1]);
    else
      for (j=0; j<length; ++j)
      	linkAlpha3(beam[j][i], beam[j][i-1]);

  // Au-dessous de AExtremity1:
  for (i=m, alpha2=false; i<perimeter-1; ++i, alpha2=!alpha2)
    if (alpha2)
      for (j=0; j<length; ++j)
	linkAlpha2(beam[j][i], beam[j][i+1]);
    else
      for (j=0; j<length; ++j)
      	linkAlpha3(beam[j][i], beam[j][i+1]);

  // Fermeture:
  if (closed)
    for (j=0; j<length; ++j)
      linkAlpha3(beam[j][0], beam[j][perimeter-1]);

  // Paramètres en sortie:
  AMesh1Corners[0] = beam[       0][m];
  AMesh1Corners[1] = beam[length-1][m];

  // Libération de la mémoire occupée par le tableau:
  for (j=0; j<length; ++j)
    delete [] beam[j];

  delete [] beam;
}
//******************************************************************************
CDart * CGMapGeneric::createTopoMesh1(int ASx, CDart * ADart)
{
  assert(ASx>=0);

  CDart * extremities[2];

  createTopoMesh1(ASx, extremities, ADart);

  return extremities[0];
}
//******************************************************************************
void CGMapGeneric::createTopoMesh2(int ASx, int ASy,
				   CDart * AMesh2Corners[2][2],
				   bool A3Sewed)
{
  assert(ASx>0);
  assert(ASy>0);
  assert(AMesh2Corners!=NULL);

  /* Numérotation des brins de chaque face:
   *
   *     5    4
   *   +---  ---+
   *   |        |
   * 6 |        | 3
   *
   * 7 |        | 2
   *   |        |
   *   +---  ---+
   *     0    1
   */

  CDart **** D = new CDart *** [ASx];
  int x,y,n;

  // Première couche:
  for (x=0; x<ASx; ++x)
    {
      D[x] = new CDart ** [ASy];

      for (y=0; y<ASy; ++y)
	{
	  D[x][y] = new CDart * [8];

	  for (n=0; n<8; ++n)
	    D[x][y][n] = addMapDart();
	}
    }

  for (x=0; x<ASx; ++x)
    for (y=0; y<ASy; ++y)
      {
	linkAlpha0( D[x][y][0] , D[x][y][1] );
	linkAlpha0( D[x][y][2] , D[x][y][3] );
	linkAlpha0( D[x][y][4] , D[x][y][5] );
	linkAlpha0( D[x][y][6] , D[x][y][7] );
	
	linkAlpha1( D[x][y][1] , D[x][y][2] );
	linkAlpha1( D[x][y][3] , D[x][y][4] );
	linkAlpha1( D[x][y][5] , D[x][y][6] );
	linkAlpha1( D[x][y][7] , D[x][y][0] );
	
	if (x>0) topoSew2( D[x][y][7] , D[x-1][y  ][2] );
	if (y>0) topoSew2( D[x][y][0] , D[x  ][y-1][5] );
      }

  // Deuxième couche:
  if (A3Sewed)
    {
      for (x=0; x<ASx; ++x)
	for (y=0; y<ASy; ++y)
	  for (n=0; n<8; ++n)
	    linkAlpha3(D[x][y][n], addMapDart());

      for (x=0; x<ASx; ++x)
	for (y=0; y<ASy; ++y)
	  {
	    linkAlpha0( alpha3( D[x][y][0] ) , alpha3( D[x][y][1] ) );
	    linkAlpha0( alpha3( D[x][y][2] ) , alpha3( D[x][y][3] ) );
	    linkAlpha0( alpha3( D[x][y][4] ) , alpha3( D[x][y][5] ) );
	    linkAlpha0( alpha3( D[x][y][6] ) , alpha3( D[x][y][7] ) );
	
	    linkAlpha1( alpha3( D[x][y][1] ) , alpha3( D[x][y][2] ) );
	    linkAlpha1( alpha3( D[x][y][3] ) , alpha3( D[x][y][4] ) );
	    linkAlpha1( alpha3( D[x][y][5] ) , alpha3( D[x][y][6] ) );
	    linkAlpha1( alpha3( D[x][y][7] ) , alpha3( D[x][y][0] ) );

	    if (x>0) topoSew2( alpha3( D[x  ][y  ][7] ) ,
			       alpha3( D[x-1][y  ][2] ) );
	    if (y>0) topoSew2( alpha3( D[x  ][y  ][0] ) ,
			       alpha3( D[x  ][y-1][5] ) );
	  }
    }

  // Paramètres en sortie:
  AMesh2Corners[0][0] = D[  0  ][  0  ][0];
  AMesh2Corners[1][0] = D[ASx-1][  0  ][1];
  AMesh2Corners[0][1] = D[  0  ][ASy-1][5];
  AMesh2Corners[1][1] = D[ASx-1][ASy-1][4];

  // Libération de la mémoire occupée par le tableau:
  for (x=0; x<ASx; ++x)
    {
      for (y=0; y<ASy; ++y)
	delete [] D[x][y];

      delete [] D[x];
    }

  delete [] D;
}
//******************************************************************************
CDart * CGMapGeneric::createTopoMesh2(int ASx, int ASy, bool A3Sewed)
{
  assert(ASx>0);
  assert(ASy>0);

  CDart * mesh2Corners[2][2];

  createTopoMesh2(ASx,ASy, mesh2Corners, A3Sewed);

  return mesh2Corners[0][0];
}
//******************************************************************************
void CGMapGeneric::createTopoMesh3(int ASx, int ASy, int ASz,
				   CDart * AMesh3Corners[2][2][2])
{
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);
  assert(AMesh3Corners!=NULL);

  /* Numérotation des brins de chaque cube:
   *
   *             +----  ----+           +----  ----+
   *         +   |  29  28  |          /  45  44  /    +
   *        /|   |30      27|         /46      43/    /|
   *      4/ |3     ARRIERE              HAUT      12/ |11
   *             |31      26|       /47      42/
   *    5/   |2  |  24  25  |      /  40  41  /  13/   |10
   *    /    |   +----  ----+     +----  ----+    /    |
   *   +  G. +                                   +  D. +
   *   |    /    +----  ----+     +----  ----+   |    /
   *  6|   /1   /  37  36  /      |  21  20  | 14|   /9
   *           /38      35/       |22      19|
   *  7| /0       BAS                 AVANT    15| /8
   *   |/    /39      34/         |23      18|   |/
   *   +    /  32  33  /          |  16  17  |   +
   *       +----  ----+           +----  ----+
   */

  CDart ***** D = new CDart **** [ASx];
  int x, y, z, n;
  
  for (x=0; x<ASx; ++x)
    {
      D[x] = new CDart *** [ASy];

      for (y=0; y<ASy; ++y)
	{
	  D[x][y] = new CDart ** [ASz];

	  for (z=0; z<ASz; ++z)
	    {
	      D[x][y][z] = new CDart * [48];

	      for (n=0; n<48; ++n)
		D[x][y][z][n] = addMapDart();
	    }
	}
    }

  for (x=0; x<ASx; ++x)
    for (y=0; y<ASy; ++y)
      for (z=0; z<ASz; ++z)
	{
	  linkAlpha0( D[x][y][z][ 0], D[x][y][z][ 1] );
	  linkAlpha0( D[x][y][z][ 2], D[x][y][z][ 3] );
	  linkAlpha0( D[x][y][z][ 4], D[x][y][z][ 5] );
	  linkAlpha0( D[x][y][z][ 6], D[x][y][z][ 7] );
	  linkAlpha0( D[x][y][z][ 8], D[x][y][z][ 9] );
	  linkAlpha0( D[x][y][z][10], D[x][y][z][11] );
	  linkAlpha0( D[x][y][z][12], D[x][y][z][13] );
	  linkAlpha0( D[x][y][z][14], D[x][y][z][15] );
	  linkAlpha0( D[x][y][z][16], D[x][y][z][17] );
	  linkAlpha0( D[x][y][z][18], D[x][y][z][19] );
	  linkAlpha0( D[x][y][z][20], D[x][y][z][21] );
	  linkAlpha0( D[x][y][z][22], D[x][y][z][23] );
	  linkAlpha0( D[x][y][z][24], D[x][y][z][25] );
	  linkAlpha0( D[x][y][z][26], D[x][y][z][27] );
	  linkAlpha0( D[x][y][z][28], D[x][y][z][29] );
	  linkAlpha0( D[x][y][z][30], D[x][y][z][31] );
	  linkAlpha0( D[x][y][z][32], D[x][y][z][33] );
	  linkAlpha0( D[x][y][z][34], D[x][y][z][35] );
	  linkAlpha0( D[x][y][z][36], D[x][y][z][37] );
	  linkAlpha0( D[x][y][z][38], D[x][y][z][39] );
	  linkAlpha0( D[x][y][z][40], D[x][y][z][41] );
	  linkAlpha0( D[x][y][z][42], D[x][y][z][43] );
	  linkAlpha0( D[x][y][z][44], D[x][y][z][45] );
	  linkAlpha0( D[x][y][z][46], D[x][y][z][47] );

	  linkAlpha1( D[x][y][z][ 0], D[x][y][z][ 7] );
	  linkAlpha1( D[x][y][z][ 2], D[x][y][z][ 1] );
	  linkAlpha1( D[x][y][z][ 4], D[x][y][z][ 3] );
	  linkAlpha1( D[x][y][z][ 6], D[x][y][z][ 5] );
	  linkAlpha1( D[x][y][z][ 8], D[x][y][z][15] );
	  linkAlpha1( D[x][y][z][10], D[x][y][z][ 9] );
	  linkAlpha1( D[x][y][z][12], D[x][y][z][11] );
	  linkAlpha1( D[x][y][z][14], D[x][y][z][13] );
	  linkAlpha1( D[x][y][z][16], D[x][y][z][23] );
	  linkAlpha1( D[x][y][z][18], D[x][y][z][17] );
	  linkAlpha1( D[x][y][z][20], D[x][y][z][19] );
	  linkAlpha1( D[x][y][z][22], D[x][y][z][21] );
	  linkAlpha1( D[x][y][z][24], D[x][y][z][31] );
	  linkAlpha1( D[x][y][z][26], D[x][y][z][25] );
	  linkAlpha1( D[x][y][z][28], D[x][y][z][27] );
	  linkAlpha1( D[x][y][z][30], D[x][y][z][29] );
	  linkAlpha1( D[x][y][z][32], D[x][y][z][39] );
	  linkAlpha1( D[x][y][z][34], D[x][y][z][33] );
	  linkAlpha1( D[x][y][z][36], D[x][y][z][35] );
	  linkAlpha1( D[x][y][z][38], D[x][y][z][37] );
	  linkAlpha1( D[x][y][z][40], D[x][y][z][47] );
	  linkAlpha1( D[x][y][z][42], D[x][y][z][41] );
	  linkAlpha1( D[x][y][z][44], D[x][y][z][43] );
	  linkAlpha1( D[x][y][z][46], D[x][y][z][45] );

	  linkAlpha2( D[x][y][z][ 0], D[x][y][z][39] );
	  linkAlpha2( D[x][y][z][ 1], D[x][y][z][38] );
	  linkAlpha2( D[x][y][z][ 2], D[x][y][z][31] );
	  linkAlpha2( D[x][y][z][ 3], D[x][y][z][30] );
	  linkAlpha2( D[x][y][z][ 4], D[x][y][z][46] );
	  linkAlpha2( D[x][y][z][ 5], D[x][y][z][47] );
	  linkAlpha2( D[x][y][z][ 6], D[x][y][z][22] );
	  linkAlpha2( D[x][y][z][ 7], D[x][y][z][23] );
	  linkAlpha2( D[x][y][z][ 8], D[x][y][z][34] );
	  linkAlpha2( D[x][y][z][ 9], D[x][y][z][35] );
	  linkAlpha2( D[x][y][z][10], D[x][y][z][26] );
	  linkAlpha2( D[x][y][z][11], D[x][y][z][27] );
	  linkAlpha2( D[x][y][z][12], D[x][y][z][43] );
	  linkAlpha2( D[x][y][z][13], D[x][y][z][42] );
	  linkAlpha2( D[x][y][z][14], D[x][y][z][19] );
	  linkAlpha2( D[x][y][z][15], D[x][y][z][18] );
	  linkAlpha2( D[x][y][z][16], D[x][y][z][32] );
	  linkAlpha2( D[x][y][z][17], D[x][y][z][33] );
	  linkAlpha2( D[x][y][z][20], D[x][y][z][41] );
	  linkAlpha2( D[x][y][z][21], D[x][y][z][40] );
	  linkAlpha2( D[x][y][z][24], D[x][y][z][37] );
	  linkAlpha2( D[x][y][z][25], D[x][y][z][36] );
	  linkAlpha2( D[x][y][z][28], D[x][y][z][44] );
	  linkAlpha2( D[x][y][z][29], D[x][y][z][45] );

	  if (x>0) topoSew3( D[x][y][z][ 0] , D[x-1][y  ][z  ][ 8] );
	  if (y>0) topoSew3( D[x][y][z][16] , D[x  ][y-1][z  ][24] );
	  if (z>0) topoSew3( D[x][y][z][32] , D[x  ][y  ][z-1][40] );
	}

  AMesh3Corners[0][0][0] = D[  0  ][  0  ][  0  ][32];
  AMesh3Corners[1][0][0] = D[ASx-1][  0  ][  0  ][33];
  AMesh3Corners[0][1][0] = D[  0  ][ASy-1][  0  ][37];
  AMesh3Corners[1][1][0] = D[ASx-1][ASy-1][  0  ][36];
  AMesh3Corners[0][0][1] = D[  0  ][  0  ][ASz-1][40];
  AMesh3Corners[1][0][1] = D[ASx-1][  0  ][ASz-1][41];
  AMesh3Corners[0][1][1] = D[  0  ][ASy-1][ASz-1][45];
  AMesh3Corners[1][1][1] = D[ASx-1][ASy-1][ASz-1][44];

  // Libération du tableau:
  for (x=0; x<ASx; ++x)
    {
      for (y=0; y<ASy; ++y)
	{
	  for (z=0; z<ASz; ++z)
	    delete [] D[x][y][z];

	  delete [] D[x][y];
	}

      delete [] D[x];
    }

  delete [] D;
}
//******************************************************************************
CDart * CGMapGeneric::createTopoMesh3(int ASx, int ASy, int ASz)
{
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);

  CDart * mesh3Corners[2][2][2];

  createTopoMesh3(ASx,ASy,ASz, mesh3Corners);

  return mesh3Corners[0][0][0];
}
//******************************************************************************
