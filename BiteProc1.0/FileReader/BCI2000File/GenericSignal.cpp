////////////////////////////////////////////////////////////////////////////////
// $Id: GenericSignal.cpp 3250 2011-05-05 13:38:39Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: GenericSignal is the BCI2000 type representing filter input and
//              output data.
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2011: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
//#include "PCHIncludes.h"
#pragma hdrstop

#include "GenericSignal.h"

#include "LengthField.h"
#include "defines.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cassert>

#undef DEBUG_GENERIC_SIGNAL

using namespace std;

GenericSignal::GenericSignal()
{
  SetProperties( mProperties );
}

GenericSignal::GenericSignal( size_t inChannels, size_t inElements, SignalType::Type inType )
{
  SetProperties( SignalProperties( inChannels, inElements, inType ) );
}

GenericSignal::GenericSignal( size_t inChannels, size_t inElements, SignalType inType )
{
  SetProperties( SignalProperties( inChannels, inElements, inType ) );
}

GenericSignal::GenericSignal( const SignalProperties& inProperties )
{
  SetProperties( inProperties );
}

const GenericSignal::ValueType&
GenericSignal::Value( size_t inChannel, size_t inElement ) const
{
  return operator()( inChannel, inElement );
}

GenericSignal&
GenericSignal::SetValue( size_t inChannel, size_t inElement, ValueType inValue )
{
  operator()( inChannel, inElement ) = inValue;
  return *this;
}

GenericSignal&
GenericSignal::AssignValues( const GenericSignal& s )
{
  mProperties.SetChannels( s.Channels() );
  mProperties.SetElements( s.Elements() );
  mValues = s.mValues;
  return *this;
}

const GenericSignal::ValueType&
GenericSignal::operator() ( size_t inChannel, size_t inElement ) const
{
#if DEBUG_GENERIC_SIGNAL
   return mValues.at( inChannel ).at( inElement );
#else
   return mValues[ inChannel ][ inElement ];
#endif
}

GenericSignal::ValueType&
GenericSignal::operator() ( size_t inChannel, size_t inElement )
{
#if DEBUG_GENERIC_SIGNAL
   return mValues.at( inChannel ).at( inElement );
#else
   return mValues[ inChannel ][ inElement ];
#endif
}

GenericSignal&
GenericSignal::SetProperties( const SignalProperties& inSp )
{
  mValues.resize( inSp.Channels() );
  for( size_t i = 0; i != mValues.size(); ++i )
    mValues[ i ].resize( inSp.Elements(), ValueType( 0 ) );
  mProperties = inSp;
  return *this;
}

ostream&
GenericSignal::WriteToStream( ostream& os ) const
{
  int indent = os.width();
  os << '\n' << setw( indent ) << ""
     << "SignalProperties { ";
  mProperties.WriteToStream( os );
  os << '\n' << setw( indent ) << ""
     << "}";
  os << setprecision( 7 );
  for( int j = 0; j < Elements(); ++j )
  {
    os << '\n' << setw( indent ) << "";
    for( size_t i = 0; i < mValues.size(); ++i )
    {
      os << setw( 14 )
         << Value( i, j )
         << ' ';
    }
  }
  return os;
}

ostream&
GenericSignal::WriteBinary( ostream& os ) const
{
  Type().WriteBinary( os );
  LengthField<2> channelsField( Channels() ),
                 elementsField( Elements() );
  channelsField.WriteBinary( os );
  elementsField.WriteBinary( os );
  switch( Type() )
  {
    case SignalType::int16:
      for( int i = 0; i < Channels(); ++i )
        for( int j = 0; j < Elements(); ++j )
          PutValueBinary<SignalType::int16>( os, i, j );
      break;

    case SignalType::float24:
      for( int i = 0; i < Channels(); ++i )
        for( int j = 0; j < Elements(); ++j )
          PutValueBinary<SignalType::float24>( os, i, j );
      break;

    case SignalType::float32:
      for( int i = 0; i < Channels(); ++i )
        for( int j = 0; j < Elements(); ++j )
          PutValueBinary<SignalType::float32>( os, i, j );
      break;

    case SignalType::int32:
      for( int i = 0; i < Channels(); ++i )
        for( int j = 0; j < Elements(); ++j )
          PutValueBinary<SignalType::int32>( os, i, j );
      break;

    default:
      os.setstate( os.failbit );
  }
  return os;
}

istream&
GenericSignal::ReadBinary( istream& is )
{
  SignalType     type;
  LengthField<2> channels,
                 elements;
  type.ReadBinary( is );
  channels.ReadBinary( is );
  elements.ReadBinary( is );
  SetProperties( SignalProperties( channels, elements, type ) );
  switch( Type() )
  {
    case SignalType::int16:
      for( int i = 0; i < Channels(); ++i )
        for( int j = 0; j < Elements(); ++j )
          GetValueBinary<SignalType::int16>( is, i, j );
      break;

    case SignalType::float24:
      for( int i = 0; i < Channels(); ++i )
        for( int j = 0; j < Elements(); ++j )
          GetValueBinary<SignalType::float24>( is, i, j );
      break;

    case SignalType::float32:
      for( int i = 0; i < Channels(); ++i )
        for( int j = 0; j < Elements(); ++j )
          GetValueBinary<SignalType::float32>( is, i, j );
      break;

    case SignalType::int32:
      for( int i = 0; i < Channels(); ++i )
        for( int j = 0; j < Elements(); ++j )
          GetValueBinary<SignalType::int32>( is, i, j );
      break;

    default:
      is.setstate( is.failbit );
  }
  return is;
}

ostream&
GenericSignal::WriteValueBinary( ostream& os, size_t inChannel, size_t inElement ) const
{
  switch( Type() )
  {
    case SignalType::int16:
      PutValueBinary<SignalType::int16>( os, inChannel, inElement );
      break;

    case SignalType::float24:
      PutValueBinary<SignalType::float24>( os, inChannel, inElement );
      break;

    case SignalType::float32:
      PutValueBinary<SignalType::float32>( os, inChannel, inElement );
      break;

    case SignalType::int32:
      PutValueBinary<SignalType::int32>( os, inChannel, inElement );
      break;

    default:
      os.setstate( os.failbit );
  }
  return os;
}

istream&
GenericSignal::ReadValueBinary( istream& is, size_t inChannel, size_t inElement )
{
  switch( Type() )
  {
    case SignalType::int16:
      GetValueBinary<SignalType::int16>( is, inChannel, inElement );
      break;

    case SignalType::float24:
      GetValueBinary<SignalType::float24>( is, inChannel, inElement );
      break;

    case SignalType::float32:
      GetValueBinary<SignalType::float32>( is, inChannel, inElement );
      break;

    case SignalType::int32:
      GetValueBinary<SignalType::int32>( is, inChannel, inElement );
      break;

    default:
      is.setstate( is.failbit );
  }
  return is;
}

template<>
void
GenericSignal::PutValueBinary<SignalType::int16>( std::ostream& os, size_t inChannel, size_t inElement ) const
{
  int value = static_cast<int>( Value( inChannel, inElement ) );
  os.put( value & 0xff ).put( value >> 8 );
}

template<>
void
GenericSignal::GetValueBinary<SignalType::int16>( std::istream& is, size_t inChannel, size_t inElement )
{
  signed short value = is.get();
  value |= is.get() << 8;
  SetValue( inChannel, inElement, value );
}

template<>
void
GenericSignal::PutValueBinary<SignalType::int32>( std::ostream& os, size_t inChannel, size_t inElement ) const
{
  signed int value = static_cast<signed int>( Value( inChannel, inElement ) );
  PutLittleEndian( os, value );
}

template<>
void
GenericSignal::GetValueBinary<SignalType::int32>( std::istream& is, size_t inChannel, size_t inElement )
{
  signed int value = 0;
  GetLittleEndian( is, value );
  SetValue( inChannel, inElement, value );
}

template<>
void
GenericSignal::PutValueBinary<SignalType::float24>( std::ostream& os, size_t inChannel, size_t inElement ) const
{
  GenericSignal::ValueType value = Value( inChannel, inElement );
  int mantissa,
      exponent;
  if( value == 0.0 )
  {
    mantissa = 0;
    exponent = 1;
  }
  else
  {
    exponent = static_cast<int>( ::ceil( ::log10( ::fabs( value ) ) ) );
    mantissa = static_cast<int>( value / ::pow( 10.0, exponent ) ) * 10000;
    exponent -= 4;
  }
  os.put( mantissa & 0xff ).put( mantissa >> 8 );
  os.put( exponent & 0xff );
}

template<>
void
GenericSignal::GetValueBinary<SignalType::float24>( std::istream& is, size_t inChannel, size_t inElement )
{
  signed short mantissa = is.get();
  mantissa |= is.get() << 8;
  signed char exponent = is.get();
  SetValue( inChannel, inElement, mantissa * ::pow( 10.0, exponent ) );
}

template<>
void
GenericSignal::PutValueBinary<SignalType::float32>( std::ostream& os, size_t inChannel, size_t inElement ) const
{
  assert( numeric_limits<float>::is_iec559 && sizeof( unsigned int ) == sizeof( float ) );
  float floatvalue = static_cast<float>( Value( inChannel, inElement ) );
  unsigned int value = *reinterpret_cast<const uint32*>( &floatvalue );
  PutLittleEndian( os, value );
}

template<>
void
GenericSignal::GetValueBinary<SignalType::float32>( std::istream& is, size_t inChannel, size_t inElement )
{
  assert( numeric_limits<float>::is_iec559 && sizeof( unsigned int ) == sizeof( float ) );
  uint32 value = 0;
  GetLittleEndian( is, value );
  try
  {
    SetValue( inChannel, inElement, *reinterpret_cast<float*>( &value ) );
  }
  catch( ... )
  {
    is.setstate( is.failbit );
  }
}

template<typename T>
void
GenericSignal::PutLittleEndian( std::ostream& os, const T& inValue )
{
  T value = inValue;
  for( size_t i = 0; i < sizeof( T ); ++i )
  {
    os.put( value & 0xff );
    value >>= 8;
  }
}

template<typename T>
void
GenericSignal::GetLittleEndian( std::istream& is, T& outValue )
{
  outValue = 0;
  for( size_t i = 0; i < sizeof( T ); ++i )
    outValue |= is.get() << ( i * 8 );
}

