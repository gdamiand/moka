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
 * Copyright (C) 2009, Víctor Manuel Soto Francés
 */

//******************************************************************************
#ifndef GEOMA_HH
#define GEOMA_HH
//******************************************************************************
namespace nklein {
namespace nklein_priv {
//*****************************************************************************
template<const unsigned int N>
class GeomGradeTable
{
private:
  static int gradeTable[ 1U << (N) ];

protected:
  GeomGradeTable()
  {
    for ( unsigned ii=0; ii < ( 1U << (N) ); ++ii )
    {
      unsigned int iiBits = 0;

      char* ptr = (char*)&ii;

      for ( unsigned int kk=0; kk < sizeof(unsigned int); ++kk )
      {
        static const unsigned int lut[] = {
          0, 1, 1, 2, 1, 2, 2, 3,
          1, 2, 2, 3, 2, 3, 3, 4
        };
        iiBits += lut[ ( ptr[kk] >> 0 ) & 0x0F ];
        iiBits += lut[ ( ptr[kk] >> 4 ) & 0x0F ];
      }

      gradeTable[ii] = iiBits;
    }
  }

public:
  static inline unsigned int GetGrade( unsigned int index )
  {
    if ( index >= ( 1U << (N) ) ) {
      throw std::out_of_range( "index" );
    }
    return gradeTable[index];
  }
};
//*****************************************************************************
template<const unsigned int P, const unsigned int Q=0>
class GeomMultTable : public GeomGradeTable<P+Q> {
private:
  static int multTable[ 1U << (P+Q) ][ 1U << (P+Q) ];

protected:
  GeomMultTable()
  {
    for ( unsigned ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
      unsigned int iiInitialTopBits
          = nklein_priv::GeomGradeTable< P+Q >::GetGrade(ii);

      for ( unsigned jj=0; jj < ( 1U << (P+Q) ); ++jj ) {
        unsigned int iiTopBits = iiInitialTopBits;

        int sign = 1;

        for ( unsigned int kk=0; kk < (P+Q); ++kk ) {
          unsigned int bit = ( 1U << kk );

          if ( ( ii & bit ) != 0 ) {
            --iiTopBits;
          }

          if ( ( jj & bit ) != 0 ) {
            sign *= ( iiTopBits & 1 ) ? -1 : 1;

            if ( ( ii & bit ) != 0 ) {

              if ( kk >= P ) {
                sign *= -1;
              }

            }
          }
        }

        multTable[ii][jj] = sign;
      }
    }
  }

public:
  static inline bool IsPositive( unsigned int ii, unsigned int jj )
  {
    if ( ii >= ( 1U << (P+Q) ) ) {
      throw std::out_of_range( "first index" );
    }
    if ( jj >= ( 1U << (P+Q) ) ) {
      throw std::out_of_range( "second index" );
    }
    return ( multTable[ii][jj] >= 0 );
  }
};
//*****************************************************************************
}; // end namespace nklein_priv
//*****************************************************************************
// * Inicio clase * //
template <class Type, const unsigned int P, const unsigned int Q=0>
class GeometricAlgebra
    : public nklein_priv::GeomMultTable<P,Q>
{
protected:
  Type coef[ 1U << (P+Q) ];

public:
  GeometricAlgebra( bool init = true )
  {
    if ( init ) {
      for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
        coef[ii] = 0;
      }
    }
  }

  GeometricAlgebra( const GeometricAlgebra<Type,P,Q>& b )
  { *this = b; }

  GeometricAlgebra<Type,P,Q>&
  operator =( const GeometricAlgebra<Type,P,Q>& b )
  {
    for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
      coef[ii] = b.coef[ii];
    }
    return *this;
  }

  Type& operator [] ( const unsigned int index )
  {
    if ( index >= ( 1U << (P+Q) ) ) {
      throw std::out_of_range( "index" );
    }
    return coef[ index ];
  }

  const Type& operator [] ( const unsigned int index ) const
  {
    if ( index >= ( 1U << (P+Q) ) ) {
      throw std::out_of_range( "index" );
    }
    return coef[ index ];
  }

  GeometricAlgebra<Type,P,Q> GradeInvolution( void ) const
  {
    GeometricAlgebra<Type,P,Q> a(false);
    Type zero = 0;

    for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
      if ( ( nklein_priv::GeomGradeTable< P+Q >::GetGrade( ii ) & 1 )
           != 0 ) {
        a[ii] = zero - coef[ii];
      } else {
        a[ii] = coef[ii];
      }
    }

    return a;
  }

  GeometricAlgebra<Type,P,Q> Reversion( void ) const
  {
    GeometricAlgebra<Type,P,Q> a(false);
    Type zero = 0;

    for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
      if ( ( nklein_priv::GeomGradeTable< P+Q >::GetGrade( ii ) & 2 )
           != 0 ) {
        a[ii] = zero - coef[ii];
      } else {
        a[ii] = coef[ii];
      }
    }

    return a;
  }

  GeometricAlgebra<Type,P,Q> Conjugation( void ) const
  {
    GeometricAlgebra<Type,P,Q> a(false);
    Type zero = 0;

    for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
      switch ( nklein_priv::GeomGradeTable< P+Q >::GetGrade( ii ) & 3 ) {
        case 0:
        case 3:
          a[ii] = coef[ii];
          break;
        case 1:
        case 2:
          a[ii] = zero - coef[ii];
          break;
      }
    }

    return a;
  }

  /** Ampliacion VICTOR */
  GeometricAlgebra<Type,P,Q> Grade( unsigned int kk ) const
  {
    GeometricAlgebra<Type,P,Q> a(true);
    if ( kk > (P+Q) ) {
      throw std::out_of_range( "grade kk" );
    }

    for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
      if (  nklein_priv::GeomGradeTable< P+Q >::GetGrade( ii ) == kk  )
        a[ii] = coef[ii];
    }

    return a;
  }
};
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator + ( const GeometricAlgebra<Type,P,Q>& a,
             const Type& b )
{
  GeometricAlgebra<Type,P,Q> c = a;

  c[0] = a[0] + b;

  return c;
}
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator + ( const Type& a,
             const GeometricAlgebra<Type,P,Q>& b )
{
  GeometricAlgebra<Type,P,Q> c = b;

  c[0] = a + b[0];

  return c;
}
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator + ( const GeometricAlgebra<Type,P,Q>& a,
             const GeometricAlgebra<Type,P,Q>& b )
{
  GeometricAlgebra<Type,P,Q> c(false);

  for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
    c[ii] = a[ii] + b[ii];
  }

  return c;
}
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator - ( const GeometricAlgebra<Type,P,Q>& a,
             const Type& b )
{
  GeometricAlgebra<Type,P,Q> c = a;

  c[0] = a[0] - b;

  return c;
}
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator - ( const GeometricAlgebra<Type,P,Q>& a,
             const GeometricAlgebra<Type,P,Q>& b )
{
  GeometricAlgebra<Type,P,Q> c(false);

  for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
    c[ii] = a[ii] - b[ii];
  }

  return c;
}
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator - ( const GeometricAlgebra<Type,P,Q>& a )
{
		GeometricAlgebra<Type,P,Q> b(false);
		Type zero = 0;

		for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
				b[ii] = zero - a[ii];
		}

		return b;
}
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator * ( const GeometricAlgebra<Type,P,Q>& a,
             const Type& b )
{
  GeometricAlgebra<Type,P,Q> c;

  for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
    c[ ii ] = a[ ii ] * b;
  }

  return c;
}
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator * ( const Type& a,
             const GeometricAlgebra<Type,P,Q>& b )
{
  GeometricAlgebra<Type,P,Q> c;

  for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
    c[ ii ] = a * b[ ii ];
  }

  return c;
}
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator * ( const GeometricAlgebra<Type,P,Q>& a,
             const GeometricAlgebra<Type,P,Q>& b )
{
  GeometricAlgebra<Type,P,Q> c;

  for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
    for ( unsigned int jj=0; jj < ( 1U << (P+Q) ); ++jj ) {
      unsigned int index = ( ii ^ jj );
      if ( c.IsPositive( ii, jj ) ) {
        c[ index ] = c[ index ] + ( a[ ii ] * b[ jj ] );
      } else {
        c[ index ] = c[ index ] - ( a[ ii ] * b[ jj ] );
      }
    }
  }

  return c;
}
//*****************************************************************************
template <class Type, const unsigned int P, const unsigned int Q>
inline
GeometricAlgebra<Type,P,Q>
operator ^ ( const GeometricAlgebra<Type,P,Q>& a,
             const GeometricAlgebra<Type,P,Q>& b )
{
  GeometricAlgebra<Type,P,Q> c;

  for ( unsigned int ii=0; ii < ( 1U << (P+Q) ); ++ii ) {
    for ( unsigned int jj=0; jj < ( 1U << (P+Q) ); ++jj ) {
      unsigned int mask = ( ii & jj );
      if ( mask == 0 ) {
        unsigned int index = ( ii ^ jj );
        if ( c.IsPositive( ii, jj ) ) {
          c[ index ] = c[ index ] + ( a[ ii ] * b[ jj ] );
        } else {
          c[ index ] = c[ index ] - ( a[ ii ] * b[ jj ] );
        }
      }
    }
  }

  return c;
}
//*****************************************************************************
}; // end namespace nklein
//******************************************************************************
#endif // GEOMA_HH
//******************************************************************************
