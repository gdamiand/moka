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
#include "operations.hh"
using namespace std;
//******************************************************************************
COperation::COperation() :
  FOperation(OPERATION_NONE),
  FDimension(-1)
{}
//******************************************************************************
COperation::COperation(TOperation AOperationType, int ADimension) :
  FOperation(AOperationType),
  FSubOperation(SUB_OPERATION_NONE),
  FDimension(ADimension)
{}
//******************************************************************************
COperation::COperation(TOperation AOperationType,
		       TSubOperation ASubOperationType, int ADimension) :
  FOperation(AOperationType),
  FSubOperation(ASubOperationType),
  FDimension(ADimension)
{}
//------------------------------------------------------------------------------
COperation::COperation(const COperation & AOperation) :
  FOperation   (AOperation.FOperation),
  FSubOperation(AOperation.FSubOperation),
  FDimension   (AOperation.FDimension)
{}
//------------------------------------------------------------------------------
COperation::~COperation()
{}
//******************************************************************************
TOperation COperation::getType() const
{ return FOperation; }
//------------------------------------------------------------------------------
void COperation::setType(TOperation AOperation)
{ FOperation = AOperation; }
//******************************************************************************
TSubOperation COperation::getSubType() const
{ return FSubOperation; }
//------------------------------------------------------------------------------
void COperation::setSubType(TSubOperation ASubOperation)
{ FSubOperation = ASubOperation; }
//******************************************************************************  
int COperation::getDimension() const
{ return FDimension; }
//------------------------------------------------------------------------------
void COperation::setDimension(int ADimension)
{ FDimension = ADimension; }
//******************************************************************************
void COperation::set(TOperation AOperationType, int ADimension)
{
  FOperation    = AOperationType;
  FSubOperation = SUB_OPERATION_NONE;
  FDimension    = ADimension;
}
//******************************************************************************
void COperation::set(TOperation AOperationType,
		     TSubOperation ASubOperationType, int ADimension)
{
  FOperation    = AOperationType;
  FSubOperation = ASubOperationType;
  FDimension    = ADimension;
}
//******************************************************************************
COperation& COperation::operator=(const COperation& AOperation)
{
  FOperation    = AOperation.FOperation;
  FSubOperation = AOperation.FSubOperation;
  FDimension    = AOperation.FDimension;
  return *this;
}
//******************************************************************************
ostream& operator<<(ostream& AStream, const COperation& AOperation)
{
  AStream<<AOperation.FOperation   <<" "
	 <<AOperation.FSubOperation<<" "
	 <<AOperation.FDimension;
  return AStream;
}
//******************************************************************************
istream& operator>>(istream& AStream, COperation& AOperation)
{
  int op, subOp;

  AStream>>op>>subOp>>AOperation.FDimension;
  AOperation.FOperation = (TOperation) op;
  AOperation.FSubOperation = (TSubOperation) subOp;
  return AStream;
}
//******************************************************************************
