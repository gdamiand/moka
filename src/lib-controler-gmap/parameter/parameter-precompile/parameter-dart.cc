/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
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
#include "controler-gmap-types.hh"
#include "math-extension.hh"
#include "parameters-gmap-default-values.hh"
#include "parameter-dart.hh"
#include <cassert>
#include <cstring>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterDart::CParameterDart(int ANbSelectionLevels, int ANbRef) :
  CParameter	    (ANbRef),
  FNbSelectionLevels(ANbSelectionLevels)
{
  assert(FNbSelectionLevels>0);
  
  FCLUnsel   = new float * [FNbSelectionLevels];
  FCLSel     = new float * [FNbSelectionLevels];
  FCLLastSel = new float * [FNbSelectionLevels];
  FCL0Remove  = new float * [FNbSelectionLevels];
  FCL1Remove  = new float * [FNbSelectionLevels];

  for (int i=0; i<FNbSelectionLevels; ++i)
    {
      FCLUnsel[i]   = new float[3];
      FCLSel[i]	    = new float[3];
      FCLLastSel[i] = new float[3];
      FCL0Remove[i]  = new float[3];
      FCL1Remove[i]  = new float[3];
    }

  reinit();
}
//******************************************************************************
CParameterDart::CParameterDart(const CParameterDart & AParam) :
  CParameter        (AParam),
  FNbSelectionLevels(AParam.FNbSelectionLevels),
  FLWDarts	    (AParam.FLWDarts),
  FSAloneDarts	    (AParam.FSAloneDarts)
{
  assert(FNbSelectionLevels>0);
  
  FCLUnsel   = new float * [FNbSelectionLevels];
  FCLSel     = new float * [FNbSelectionLevels];
  FCLLastSel = new float * [FNbSelectionLevels];
  FCL0Remove  = new float * [FNbSelectionLevels];
  FCL1Remove  = new float * [FNbSelectionLevels];

  for (int i=0; i<FNbSelectionLevels; ++i)
    {
      FCLUnsel[i] = new float[3];
      setCLUnsel(i, AParam.getCLUnsel(i));

      FCLSel[i] = new float[3];
      setCLSel(i, AParam.getCLSel(i));

      FCLLastSel[i] = new float[3];
      setCLLastSel(i, AParam.getCLLastSel(i));

      FCL0Remove[i]  = new float[3];
      setCL0Remove(i, AParam.getCL0Remove(i));

      FCL1Remove[i]  = new float[3];
      setCL1Remove(i, AParam.getCL1Remove(i));
    }
}
//******************************************************************************
CParameterDart::~CParameterDart()
{
  for (int i=0; i<FNbSelectionLevels; ++i)
    {
      delete [] FCLUnsel  [i];
      delete [] FCLSel    [i];
      delete [] FCLLastSel[i];
      delete [] FCL0Remove [i];
      delete [] FCL1Remove [i];
    }

  delete [] FCLUnsel  ;
  delete [] FCLSel    ;
  delete [] FCLLastSel;
  delete [] FCL0Remove;
  delete [] FCL1Remove;
}
//******************************************************************************
CParameter * CParameterDart::copy() const
{ return new CParameterDart(*this); }
//******************************************************************************
void CParameterDart::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterDart::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterDart::reinit()
{
  assert (FNbSelectionLevels>0);
  
  FLWDarts     = DEFAULT_DART_LINE_WIDTH;
  FSAloneDarts = DEFAULT_DART_SIZE_ALONE;

  setCLUnsel(0,
	     DEFAULT_DART_COUL_1UNSEL_0,
	     DEFAULT_DART_COUL_1UNSEL_1,
	     DEFAULT_DART_COUL_1UNSEL_2);
  setCLSel(0,
	   DEFAULT_DART_COUL_1SEL_0,
	   DEFAULT_DART_COUL_1SEL_1,
	   DEFAULT_DART_COUL_1SEL_2);
  setCLLastSel(0,
	       DEFAULT_DART_COUL_1LAST_0,
	       DEFAULT_DART_COUL_1LAST_1,
	       DEFAULT_DART_COUL_1LAST_2);
  setCL0Remove(0,
	       DEFAULT_DART_COUL_0REMOVE1_0,
	       DEFAULT_DART_COUL_0REMOVE1_1,
	       DEFAULT_DART_COUL_0REMOVE1_2);
  setCL1Remove(0,
	       DEFAULT_DART_COUL_1REMOVE1_0,
	       DEFAULT_DART_COUL_1REMOVE1_1,
	       DEFAULT_DART_COUL_1REMOVE1_2);
  
  putAllNeedToUpdate();

  if (FNbSelectionLevels==1) return;

  setCLUnsel(1,
	     DEFAULT_DART_COUL_2UNSEL_0,
	     DEFAULT_DART_COUL_2UNSEL_1,
	     DEFAULT_DART_COUL_2UNSEL_2);
  setCLSel(1,
	   DEFAULT_DART_COUL_2SEL_0,
	   DEFAULT_DART_COUL_2SEL_1,
	   DEFAULT_DART_COUL_2SEL_2);
  setCLLastSel(1,
	       DEFAULT_DART_COUL_2LAST_0,
	       DEFAULT_DART_COUL_2LAST_1,
	       DEFAULT_DART_COUL_2LAST_2);
  setCL0Remove(1,
	       DEFAULT_DART_COUL_0REMOVE2_0,
	       DEFAULT_DART_COUL_0REMOVE2_1,
	       DEFAULT_DART_COUL_0REMOVE2_2);
  setCL1Remove(1,
	       DEFAULT_DART_COUL_1REMOVE2_0,
	       DEFAULT_DART_COUL_1REMOVE2_1,
	       DEFAULT_DART_COUL_1REMOVE2_2);

  if (FNbSelectionLevels==2) return;

  setCLUnsel(2,
	     DEFAULT_DART_COUL_3UNSEL_0,
	     DEFAULT_DART_COUL_3UNSEL_1,
	     DEFAULT_DART_COUL_3UNSEL_2);
  setCLSel(2,
	   DEFAULT_DART_COUL_3SEL_0,
	   DEFAULT_DART_COUL_3SEL_1,
	   DEFAULT_DART_COUL_3SEL_2);
  setCLLastSel(2,
	       DEFAULT_DART_COUL_3LAST_0,
	       DEFAULT_DART_COUL_3LAST_1,
	       DEFAULT_DART_COUL_3LAST_2); 
  setCL0Remove(2,
	      DEFAULT_DART_COUL_0REMOVE3_0,
	      DEFAULT_DART_COUL_0REMOVE3_1,
	      DEFAULT_DART_COUL_0REMOVE3_2);
  setCL1Remove(2,
	      DEFAULT_DART_COUL_1REMOVE3_0,
	      DEFAULT_DART_COUL_1REMOVE3_1,
	      DEFAULT_DART_COUL_1REMOVE3_2);
  
  for (int i=3; i<FNbSelectionLevels; ++i)
    {
      setCLUnsel(i,
		 DEFAULT_DART_COUL_nUNSEL_0,
		 DEFAULT_DART_COUL_nUNSEL_1,
		 DEFAULT_DART_COUL_nUNSEL_2);
      setCLSel(i,
	       DEFAULT_DART_COUL_nSEL_0,
	       DEFAULT_DART_COUL_nSEL_1,
	       DEFAULT_DART_COUL_nSEL_2);
      setCLLastSel(i,
		   DEFAULT_DART_COUL_nLAST_0,
		   DEFAULT_DART_COUL_nLAST_1,
		   DEFAULT_DART_COUL_nLAST_2);
      setCL0Remove(i,
		  DEFAULT_DART_COUL_0REMOVEn_0,
		  DEFAULT_DART_COUL_0REMOVEn_1,
		  DEFAULT_DART_COUL_0REMOVEn_2);
      setCL1Remove(i,
		  DEFAULT_DART_COUL_1REMOVEn_0,
		  DEFAULT_DART_COUL_1REMOVEn_1,
		  DEFAULT_DART_COUL_1REMOVEn_2);
    }
}
//******************************************************************************
namespace GMap3d
{
//------------------------------------------------------------------------------
ostream& operator<<(ostream& AStream, const CParameterDart & AParameter)
{
  AStream<<"CParameterDart:"<<endl;
  
  AStream<<"  LineWidth:            "<<AParameter.FLWDarts<<endl;
  
  AStream<<"  SizeAlone:            "<<AParameter.FSAloneDarts<<endl;

  AStream<<"  SelectionLevelNumber: "<<AParameter.FNbSelectionLevels<<endl;

  for (int i=0; i<AParameter.FNbSelectionLevels; ++i)
    {
      AStream<<"  ColorUnsel "<<i<<":         "<<AParameter.FCLUnsel[i][0]<<" "
	     <<AParameter.FCLUnsel[i][1]<<" "<<AParameter.FCLUnsel[i][2]<<endl;
      
      AStream<<"  ColorSel "<<i<<":           "<<AParameter.FCLSel[i][0]<<" "
	     <<AParameter.FCLSel[i][1]<<" "<<AParameter.FCLSel[i][2]<<endl;
      
      AStream<<"  ColorLast "<<i<<":          "<<AParameter.FCLLastSel[i][0]<<" "
	     <<AParameter.FCLLastSel[i][1]<<" "<<AParameter.FCLLastSel[i][2]<<endl;

      AStream<<"  Color0Remove "<<i<<":        "<<AParameter.FCL0Remove[i][0]<<" "
	     <<AParameter.FCL0Remove[i][1]<<" "<<AParameter.FCL0Remove[i][2]<<endl;

      AStream<<"  Color1Remove "<<i<<":        "<<AParameter.FCL1Remove[i][0]<<" "
	     <<AParameter.FCL1Remove[i][1]<<" "<<AParameter.FCL1Remove[i][2]<<endl;
    }
  
  AStream<<endl;
  
  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream, CParameterDart & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterDart:") );

  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth:") );
  AStream>>AParameter.FLWDarts;

  AStream>>tmp; assert ( !strcmp(tmp, "SizeAlone:") );
  AStream>>AParameter.FSAloneDarts;

  int nb, toRead, toIgnore;
  AStream>>tmp; assert ( !strcmp(tmp, "SelectionLevelNumber:") );
  AStream>>nb;

  toRead   = MIN(nb,AParameter.FNbSelectionLevels);
  toIgnore = nb - AParameter.FNbSelectionLevels;

  int i;
  for (i=0; i<toRead; ++i)
    {
      AStream>>tmp; assert( !strcmp(tmp,"ColorUnsel") ); AStream>>tmp;  
      AStream>>AParameter.FCLUnsel[i][0]
	     >>AParameter.FCLUnsel[i][1]>>AParameter.FCLUnsel[i][2];
      
      AStream>>tmp; assert( !strcmp(tmp,"ColorSel") ); AStream>>tmp;
      AStream>>AParameter.FCLSel[i][0]
	     >>AParameter.FCLSel[i][1]>>AParameter.FCLSel[i][2];
      
      AStream>>tmp; assert( !strcmp(tmp,"ColorLast") ); AStream>>tmp;
      AStream>>AParameter.FCLLastSel[i][0]
	     >>AParameter.FCLLastSel[i][1]>>AParameter.FCLLastSel[i][2];

      AStream>>tmp; assert( !strcmp(tmp,"Color0Remove") ); AStream>>tmp;
      AStream>>AParameter.FCL0Remove[i][0]
	     >>AParameter.FCL0Remove[i][1]>>AParameter.FCL0Remove[i][2];

      AStream>>tmp; assert( !strcmp(tmp,"Color1Remove") ); AStream>>tmp;
      AStream>>AParameter.FCL1Remove[i][0]
	     >>AParameter.FCL1Remove[i][1]>>AParameter.FCL1Remove[i][2];
    }

  for (i=0; i<toIgnore; ++i)
    {
      float dummy;
      AStream>>tmp; assert( !strcmp(tmp,"ColorUnsel") ); AStream>>tmp;
      AStream>>dummy>>dummy>>dummy;
      AStream>>tmp; assert( !strcmp(tmp,"ColorSel") ); AStream>>tmp;
      AStream>>dummy>>dummy>>dummy;
      AStream>>tmp; assert( !strcmp(tmp,"ColorLast") ); AStream>>tmp;
      AStream>>dummy>>dummy>>dummy;
      AStream>>tmp; assert( !strcmp(tmp,"Color0Remove") ); AStream>>tmp;
      AStream>>dummy>>dummy>>dummy;
      AStream>>tmp; assert( !strcmp(tmp,"Color1Remove") ); AStream>>tmp;
      AStream>>dummy>>dummy>>dummy;
    }

  AParameter.putAllNeedToUpdate();

  return AStream;
}
//------------------------------------------------------------------------------
} // namespace GMap3d
//******************************************************************************
int CParameterDart::getLWDarts() const
{ return FLWDarts; }

void CParameterDart::setLWDarts(int AValue)
{
  if (FLWDarts!=AValue)
    {
      putAllNeedToUpdate();
      FLWDarts= AValue;
    }
}
//******************************************************************************
int CParameterDart::getSAloneDarts() const
{ return FSAloneDarts; }

void CParameterDart::setSAloneDarts(int AValue)
{
  if (FSAloneDarts!=AValue)
    {
      putAllNeedToUpdate();
      FSAloneDarts= AValue;
    }
}
//******************************************************************************
float CParameterDart::getCLUnsel(int ALevel, int AIndice) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  return FCLUnsel[ALevel][AIndice];
}

void CParameterDart::setCLUnsel(int ALevel, int AIndice, float AValue)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  if (FCLUnsel[ALevel][AIndice]!=AValue)
    {
      putAllNeedToUpdate();
      FCLUnsel[ALevel][AIndice]= AValue;
    }
}

const float * CParameterDart::getCLUnsel(int ALevel) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  return FCLUnsel[ALevel];
}

void CParameterDart::setCLUnsel(int ALevel,
				float AValue0, float AValue1, float AValue2)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  if ( FCLUnsel[ALevel][0]!=AValue0 ||
       FCLUnsel[ALevel][1]!=AValue1 ||
       FCLUnsel[ALevel][2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLUnsel[ALevel][0]= AValue0;
      FCLUnsel[ALevel][1]= AValue1;
      FCLUnsel[ALevel][2]= AValue2;
    }
}
void CParameterDart::setCLUnsel(int ALevel,const float ATab[3])
{ setCLUnsel(ALevel,ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
float CParameterDart::getCLSel(int ALevel, int AIndice) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  return FCLSel[ALevel][AIndice];
}
void CParameterDart::setCLSel(int ALevel, int AIndice, float AValue)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  if (FCLSel[ALevel][AIndice]!=AValue)
    {
      putAllNeedToUpdate();
      FCLSel[ALevel][AIndice]= AValue;
    }
}
const float * CParameterDart::getCLSel(int ALevel) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  return FCLSel[ALevel];
}
void CParameterDart::setCLSel(int ALevel,
			      float AValue0, float AValue1, float AValue2)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  if ( FCLSel[ALevel][0]!=AValue0 ||
       FCLSel[ALevel][1]!=AValue1 ||
       FCLSel[ALevel][2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLSel[ALevel][0]= AValue0;
      FCLSel[ALevel][1]= AValue1;
      FCLSel[ALevel][2]= AValue2;
    }
}
void CParameterDart::setCLSel(int ALevel,const float ATab[3])
{ setCLSel(ALevel,ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
float CParameterDart::getCLLastSel(int ALevel, int AIndice) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  return FCLLastSel[ALevel][AIndice];
}
void CParameterDart::setCLLastSel(int ALevel, int AIndice, float AValue)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  if (FCLLastSel[ALevel][AIndice]!=AValue)
    {
      putAllNeedToUpdate();
      FCLLastSel[ALevel][AIndice]= AValue;
    }
}
const float * CParameterDart::getCLLastSel(int ALevel) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  return FCLLastSel[ALevel];
}
void CParameterDart::setCLLastSel(int ALevel,
				  float AValue0, float AValue1, float AValue2)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  if ( FCLLastSel[ALevel][0]!=AValue0 ||
       FCLLastSel[ALevel][1]!=AValue1 ||
       FCLLastSel[ALevel][2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLLastSel[ALevel][0]= AValue0;
      FCLLastSel[ALevel][1]= AValue1;
      FCLLastSel[ALevel][2]= AValue2;
    }
}
void CParameterDart::setCLLastSel(int ALevel,const float ATab[3])
{ setCLLastSel(ALevel,ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
float CParameterDart::getCL0Remove(int ALevel, int AIndice) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  return FCL0Remove[ALevel][AIndice];
}

void CParameterDart::setCL0Remove(int ALevel, int AIndice, float AValue)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  if (FCL0Remove[ALevel][AIndice]!=AValue)
    {
      putAllNeedToUpdate();
      FCL0Remove[ALevel][AIndice]= AValue;
    }
}

const float * CParameterDart::getCL0Remove(int ALevel) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  return FCL0Remove[ALevel];
}

void CParameterDart::setCL0Remove(int ALevel,
				float AValue0, float AValue1, float AValue2)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  if ( FCL0Remove[ALevel][0]!=AValue0 ||
       FCL0Remove[ALevel][1]!=AValue1 ||
       FCL0Remove[ALevel][2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCL0Remove[ALevel][0]= AValue0;
      FCL0Remove[ALevel][1]= AValue1;
      FCL0Remove[ALevel][2]= AValue2;
    }
}
void CParameterDart::setCL0Remove(int ALevel,const float ATab[3])
{ setCL0Remove(ALevel,ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
float CParameterDart::getCL1Remove(int ALevel, int AIndice) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  return FCL1Remove[ALevel][AIndice];
}

void CParameterDart::setCL1Remove(int ALevel, int AIndice, float AValue)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  assert(0<=AIndice && AIndice<=2);
  if (FCL1Remove[ALevel][AIndice]!=AValue)
    {
      putAllNeedToUpdate();
      FCL1Remove[ALevel][AIndice]= AValue;
    }
}

const float * CParameterDart::getCL1Remove(int ALevel) const
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  return FCL1Remove[ALevel];
}

void CParameterDart::setCL1Remove(int ALevel,
				float AValue0, float AValue1, float AValue2)
{
  assert(0<=ALevel && ALevel<FNbSelectionLevels);
  if ( FCL1Remove[ALevel][0]!=AValue0 ||
       FCL1Remove[ALevel][1]!=AValue1 ||
       FCL1Remove[ALevel][2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCL1Remove[ALevel][0]= AValue0;
      FCL1Remove[ALevel][1]= AValue1;
      FCL1Remove[ALevel][2]= AValue2;
    }
}
void CParameterDart::setCL1Remove(int ALevel,const float ATab[3])
{ setCL1Remove(ALevel,ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
int CParameterDart::getType() const
{ return PARAMETER_DART; }
//******************************************************************************
