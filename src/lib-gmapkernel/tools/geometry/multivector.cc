/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
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
 *
 * Author(s): Víctor Manuel Soto Francés
*/

//******************************************************************************
#include "inline-macro.hh"
#include "multivector.hh"
#include INCLUDE_NON_INLINE("multivector.icc")
//******************************************************************************
CMultivector::CMultivector()
{}
//------------------------------------------------------------------------------
CMultivector::CMultivector(const CMultivector & AMv)
{
  MD=AMv.getMD();
  // OJO: compute here the values to compute length,area and volume
  computeLAV();
}
//------------------------------------------------------------------------------
/*CMultivector::CMultivector(const CDart *dart,CVertex *AVertex)
{}*/
//******************************************************************************
/** Compute sense of the face according to the First stored
 */
double CMultivector::computeLAV()
{
  nklein::GeometricAlgebra< double, 4 > plane,Rh,Mh,Th,Sh;
  nklein::GeometricAlgebra< double, 4 > line;
  nklein::GeometricAlgebra< double, 4 > point;
  nklein::GeometricAlgebra< double, 4 > E0h,Ih;//Ih=ID
  nklein::GeometricAlgebra< double, 4 > planeD;

  nklein::GeometricAlgebra< double, 3 > A,C,B,T,N,V;
  nklein::GeometricAlgebra< double, 3 > I;
  double invnorm,volume,Tnorm,Nnorm,Bnorm;

  /*! Definitions */
  Ih[e0|e1|e2|e3]=1.0;
  I[E1|E2|E3]=1.0;
  E0h[e0]=1.0;E0h[e1]=0;E0h[e2]=0;E0h[e3]=0;//e0

  /*! VOLUME */
  volume=MD[0];

  /*! PLANE */
  planeD=MD.Grade(1);
  plane=planeD*Ih;//!< undualize the plane without homogenizing. 2-FLAT...OK
  Rh=(E0h*plane.Grade(3)).Grade(3-1);//!< bivector director
  Th=(Rh*Rh.Reversion()).Grade(0);//!< Inverse of direccion
  invnorm=1/Th[0];
  Th=Rh.Reversion()*invnorm;
  Mh=(E0h*(E0h^plane)).Grade(4-1);//!< MOMENT of the 2-FLAT
  Sh=Mh*Th;//!< SUPPORT VECTOR
  //    std::cout << i<< " planoD=" << planoD <<"  ===> ("<<planoDh  <<"\n";
  //    std::cout << i<< " plano =" << plano  <<"  ===> \n";
  //    std::cout << i<< " plano-direccion =" << Rh  <<"  ===> \n";
  //    std::cout <<"plano-Moment= "<< Mh <<"\n";
  //    std::cout <<"plano-soporte= "<< Sh <<"\n";
  //    std::cout<< std::endl;
  A=0;
  A[E1|E2]=Rh[e1|e2];//!< bivector-director A => R3
  A[E1|E3]=Rh[e1|e3];
  A[E2|E3]=Rh[e2|e3];
  B=0;
  B=-I*A; //!< vector normal to A, cross product equivalent
  Bnorm=sqrt(B[E1]*B[E1]+B[E2]*B[E2]+B[E3]*B[E3]);

  /*! LINE */
  line=MD.Grade(2);
  line=line*Ih;//!< desdualizo el plano sin homogeneizar. 1-FLAT.... OK
  Rh=(E0h*line.Grade(2)).Grade(2-1);//!< vector director
  Th=(Rh*Rh.Reversion()).Grade(0);//!< Inversa de la direccion
  invnorm=1/Th[0];
  Th=Rh.Reversion()*invnorm;
  Mh=(E0h*(E0h^line)).Grade(3-1);//!< MOMENT of the 1-FLAT
  Sh=Mh*Th;//!< SUPPORT VECTOR
  //    std::cout << i<< " ladoD=" << ladoD[ilado]<<"\n";
  //    std::cout << i<< " lado =" << lado[ilado]  <<"  ===> \n";
  //    std::cout << i<< " lado-direccion =" << Rh  <<"  ===> \n";
  //    std::cout <<"lado-Moment= "<< Mh <<"\n";
  //    std::cout <<"lado-soporte= "<< Sh <<"\n";
  //    std::cout<< std::endl;
  T=0;
  T[E1]=Rh[e1];//!< paso la direccion a R3 (dirección del lado)
  T[E2]=Rh[e2];
  T[E3]=Rh[e3];
  Tnorm=sqrt(T[E1]*T[E1]+T[E2]*T[E2]+T[E3]*T[E3]);

  //D normal vector to the face
  //    std::cout<< "** Vector normal hacia la cara **\n";
  C=-A*I;//!< Vector normal a la cara en R3
  N=(T*A).Grade(1);//!< producto escalar del vector B por el bivector A
  //    std::cout<<A<<","<<D<<","<< C;
  //    std::cout<<std::endl;
  Nnorm=sqrt(N[E1]*N[E1]+N[E2]*N[E2]+N[E3]*N[E3]);

  /*! POINT */
  point=MD.Grade(3);
  point=point*Ih; //!< desdualizo el punto 0-FLAT..OK
  if(point[e0]!=0)
  {
    double xx=point[e0];
    point[e0]=1;
    point[e1]=point[e1]/xx;
    point[e2]=point[e2]/xx;
    point[e3]=point[e3]/xx;
  }
  V=0;
  V[E1]=point[e1];
  V[E2]=point[e2];
  V[E3]=point[e3];

  /*! RESULTS */
  this->l=(V*T)[0]/Tnorm;
  this->a=this->l*(V*N)[0]/Nnorm;
  this->v=this->a*(V*B)[0]/Bnorm*volume;//! chages the sense of B thus the scalar product
  /*! use factors: Warning the signs are not included */
  this->a*=0.5;//! 1/2
  this->v*=0.1666666666666667; //! 1/6
}
//******************************************************************************
nklein::GeometricAlgebra< double, 4 > CMultivector::getMD() const
{ return MD; }
//******************************************************************************
void CMultivector::setMD(const nklein::GeometricAlgebra< double, 4 >&  MVector)
{
  MD=MVector;
}
//******************************************************************************
