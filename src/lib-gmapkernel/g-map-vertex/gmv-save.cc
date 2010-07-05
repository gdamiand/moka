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
#include "g-map-vertex.hh"
#include "streams.hh"
#include <cstring>
#include <map>
using namespace std;
using namespace GMap3d;
//******************************************************************************
void CGMapVertex::giveNumberToDarts(int ADirectInfoIndex)
{
  CDynamicCoverageAll it(this);

  for (unsigned long int num=0; it.cont(); ++it, ++num)
    (*it)->setDirectInfo(ADirectInfoIndex, (void *) num);
}
//******************************************************************************
TSaveFormat CGMapVertex::getFileFormat(const char * AFilename)
{
  ifstream stream(AFilename);
  char id[256];

  stream.getline(id,256);
  stream.close();

  if (strcmp(id, getFileHeader(BinaryFormat))==0) return BinaryFormat;
  if (strcmp(id, getFileHeader( AsciiFormat))==0) return  AsciiFormat;

  return UnknownFormat;
}
//******************************************************************************
const char * CGMapVertex::getFileHeader(TSaveFormat AFormat)
{
  switch (AFormat)
    {
    case BinaryFormat:
      return "Moka file [binary]";
    case AsciiFormat:
      return "Moka file [ascii]";
    default:
      return NULL;
    }
}
//******************************************************************************
bool CGMapVertex::save(const char * AFilename, TSaveFormat AFormat)
{
  ofstream stream;
  if ( AFormat==BinaryFormat )
    stream.open(AFilename, ios::trunc | ios::binary);
  else
    stream.open(AFilename, ios::trunc);
    
  bool result = save(stream, AFormat);
  stream.close();
  return result;
}
//******************************************************************************
CDart* CGMapVertex::load(const char * AFilename, TSaveFormat AFormat)
{
  ifstream stream;
  if ( AFormat==BinaryFormat )
    stream.open(AFilename, ios::binary);
  else
    stream.open(AFilename);

  CDart* result = load(stream, AFormat);
  stream.close();
  return result;
}
//******************************************************************************
bool CGMapVertex::save(ostream & AStream, TSaveFormat AFormat)
{
  int directInfoIndex = getNewDirectInfo();
  const char * header = getFileHeader(AFormat);
  
  assert(header!=NULL);

  // Numérotation des brins:
  giveNumberToDarts(directInfoIndex);

  // Positionnement du format d'écriture:
  switch (AFormat)
    {
    case AsciiFormat : setAsciiMode (); break;
    case BinaryFormat: setBinaryMode(); break;
    default: cerr << "CGMapVertex.save: Format inconnu!!!" << endl;
    }

  AStream << header << endl;

  // Sauvegarde de l'état général des marques:
  assert(NB_MARKS % 8 == 0);
  bool m[8], u[8];

  for (int i=0; i<NB_MARKS; ++i)
    {
      m[i % 8] = FMaskMarks[i];
      u[i % 8] = FUsedMarks[i];

      if (i % 8 == 7)
	{
	  writeChar(AStream, bool2char(m)); writeSpc(AStream);
	  writeChar(AStream, bool2char(u)); writeSpc(AStream);
	}
    }

  writeRet(AStream);

  // Sauvegarde des brins:
  bool ok = true;
  CDynamicCoverageAll it(this);

  for (; ok && it.cont(); ++it)
    {
      CDartVertex * current = (CDartVertex *) *it;
      ok = current->save(AStream, AFormat, directInfoIndex);
    }
    
  freeDirectInfo(directInfoIndex);
  return ok;
}
//******************************************************************************
CDart* CGMapVertex::load(istream & AStream, TSaveFormat AFormat)
{
  // Positionnement du format de lecture:
  switch (AFormat)
    {
    case AsciiFormat : setAsciiMode (); break;
    case BinaryFormat: setBinaryMode(); break;
    default:
      cerr << "CGMapVertex.load: Format inconnu!!!" << endl;
      return NULL;
    }

  // Lecture et vérification du header:
  {
    char id[256];
    AStream.getline(id, 256);

    if (strcmp(id, getFileHeader(AFormat)) != 0)
      return NULL;
  }

  // Lecture de l'état général des marques du fichier à charger:
  bitset<NB_MARKS> streamUsed;
  bitset<NB_MARKS> streamMask;

  assert(NB_MARKS % 8 == 0);
  bool m[8],u[8];

  int i;

  for (i=0; i<NB_MARKS; ++i)
    {
      if (i % 8 == 0)
	{
	  char2bool(readChar(AStream), m);
	  char2bool(readChar(AStream), u);
	}

      if (u[i % 8] && ! FUsedMarks[i])
	cerr << "CGMapVertex::load: Marque n°" << i
	     << " ignorée lors du chargement." << endl;

      streamUsed.set(i, u[i % 8]);
      streamMask.set(i, m[i % 8]);
    }

  // Lecture des brins du fichier à charger:
  bitset<NB_MARKS> marksToKeep = FUsedMarks & streamUsed;

  int nbLoaded = 0;
  int mark = getNewMark(); // pour savoir quels brins ont été chargés

  negateMaskMark(mark);

  CDartVertex * dart=NULL;
  for (bool cont = true; cont; )
    {
      dart = (CDartVertex *) addMapDart();
      cont = dart->load(AStream, AFormat);

      if (cont)
	{
	  // Mise-à-jour des marques du brin chargé:
	  setMarks(dart, (dart->getMarks() ^ streamMask) & marksToKeep);
	  ++nbLoaded;
	}
      else
	delMapDart(dart);
    }

  negateMaskMark(mark);

  // Alpha-i: Transformation des indices en pointeurs:
  CDynamicCoverageAll it(this);
  if ( it.cont() )
    {
      dart=static_cast<CDartVertex*>(*it);
      
      CDartVertex** table = new CDartVertex* [nbLoaded];
      i = nbLoaded;
      
      for (; it.cont(); ++it)
	if (isMarked(*it, mark))
	  {
	    table[--i] = (CDartVertex *) *it;
	    unsetMark(*it, mark);
	  }

      assert(i==0);
      
      for (; i<nbLoaded; ++i)
	for (int dim=0; dim<=3; ++dim)
	  {
	    int n = (long int) table[i]->getAlpha(dim) -1;
	    
	    if (n==i)
	      table[i]->setFree(dim);
	    else
	      if (n<i)
		linkAlpha(table[i], table[n], dim);
	  }

      delete [] table;
    }
  
  freeMark(mark);

  return dart;
}
//******************************************************************************
CDart* CGMapVertex::load(const char * AFilename)
{
  TSaveFormat format= CGMapVertex::getFileFormat(AFilename);

  if ( format==UnknownFormat ) return NULL;
  return load(AFilename,format);
}
//******************************************************************************
