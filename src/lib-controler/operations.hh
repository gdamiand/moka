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
#ifndef OPERATIONS_HH
#define OPERATIONS_HH
//******************************************************************************
#include "controler-types.hh"
#include <iostream>
//******************************************************************************
class COperation
{
public:
  COperation();
  COperation(TOperation AOperationType, int ADimension = -1);
  COperation(TOperation AOperationType, TSubOperation ASubOperationType,
	     int ADimension);
  
  COperation(const COperation & AOperation);
  ~COperation();
  
  TOperation    getType() const;
  void          setType(TOperation AOperation);
  
  TSubOperation getSubType() const;
  void          setSubType(TSubOperation ASubOperation);
  
  int           getDimension() const;
  void          setDimension(int ADimension);
  
  void          set(TOperation AOperationType, int ADimension = -1);
  void          set(TOperation AOperationType,
		    TSubOperation ASubOperationType, int ADimension = -1);
  
  COperation& operator=(const COperation& AOperation);
  
  friend std::ostream& operator<<(std::ostream& AStream,
				  const COperation& AOperation);
  friend std::istream& operator>>(std::istream& AStream,
				  COperation& AOperation);
  
private:
  TOperation    FOperation;
  TSubOperation FSubOperation;
  int           FDimension;
};
//******************************************************************************
#endif // OPERATIONS_HH
//******************************************************************************
