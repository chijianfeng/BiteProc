////////////////////////////////////////////////////////////////////////////////
// $Id: SignalProperties.h 3272 2011-05-09 15:04:30Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents properties of a GenericSignal.
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
#ifndef SIGNAL_PROPERTIES_H
#define SIGNAL_PROPERTIES_H

#include <iostream>
#include "SignalType.h"
#include "PhysicalUnit.h"
#include "LabelIndex.h"
#include "ValueList.h"
#include "EncodedString.h"

class SignalProperties
{
  public:
    SignalProperties()
      : mType( SignalType::defaultType ),
        mValueUnits( 1 )
      { InitMembers( 0, 0 ); }

    SignalProperties(
      size_t inChannels,
      size_t inElements,
      SignalType::Type inType = SignalType::defaultType )
      : mType( inType ),
        mValueUnits( 1 )
      { InitMembers( inChannels, inElements ); }

    SignalProperties(
      size_t inChannels,
      size_t inElements,
      SignalType inType )
      : mType( inType ),
        mValueUnits( 1 )
      { InitMembers( inChannels, inElements ); }

    virtual ~SignalProperties()
      {}

    // Accessors
    SignalProperties& SetChannels( size_t ch )
                      { mChannelLabels.Resize( ch ); return *this; }
    int               Channels() const
                      { return mChannelLabels.Size(); }

    SignalProperties& SetElements( size_t el )
                      { mElementLabels.Resize( el ); return *this; }
    int               Elements() const
                      { return mElementLabels.Size(); }

    SignalProperties& SetType( const SignalType& ty )
                      { mType = ty; return *this; }
    const SignalType& Type() const
                      { return mType; }

    SignalProperties& SetName( const std::string& s )
                      { mName = s; return *this; }
    const std::string& Name() const
                      { return mName; }

    LabelIndex&       ChannelLabels()
                      { return mChannelLabels; }
    const LabelIndex& ChannelLabels() const
                      { return mChannelLabels; }
    PhysicalUnit&     ChannelUnit()
                      { return mChannelUnit; }
    const PhysicalUnit& ChannelUnit() const
                      { return mChannelUnit; }
    // Convert a string-valued channel address into a numeric index.
    // The address may be a label or a value in physical units.
    double            ChannelIndex( const std::string& address ) const
                      { return AddressToIndex( address, ChannelLabels(), ChannelUnit() ); }

    LabelIndex&       ElementLabels()
                      { return mElementLabels; }
    const LabelIndex& ElementLabels() const
                      { return mElementLabels; }
    PhysicalUnit&     ElementUnit()
                      { return mElementUnit; }
    const PhysicalUnit& ElementUnit() const
                      { return mElementUnit; }
    // Convert a string-valued element address into a numeric index.
    // The address may be a label or a value in physical units.
    double            ElementIndex( const std::string& address ) const
                      { return AddressToIndex( address, ElementLabels(), ElementUnit() ); }

    PhysicalUnit&     ValueUnit( size_t ch = 0 );
    const PhysicalUnit& ValueUnit( size_t ch = 0 ) const
                      { return mValueUnits.size() <= ch
                        ? *mValueUnits.rbegin() : mValueUnits[ ch ]; }

    // The SamplingRate property is constructed from the ElementUnit gain if the ElementUnit's symbol is "s",
    // from the UpdateRate property when Elements is 1, and is zero otherwise.
    // SamplingRate in Hz
    double SamplingRate() const;
    // The UpdateRate property represents how often a signal is updated in the online system.
#ifdef TODO
# error Add update rate to the SignalProperties stream representation.
#endif // TODO
    // UpdateRate in Hz
    double            UpdateRate() const
                      { return mUpdateRate; }
    SignalProperties& SetUpdateRate( double r )
                      { mUpdateRate = r; return *this; }

    bool IsEmpty() const
                      { return Channels() == 0 || Elements() == 0; }

    bool operator==( const SignalProperties& sp ) const
                      { return Type() == sp.Type() && Elements() == sp.Elements() && Channels() == sp.Channels(); }
    bool operator!=( const SignalProperties& sp ) const
                      { return !( *this == sp ); }
    bool Accommodates( const SignalProperties& sp ) const;

    // Stream i/o
    std::ostream& WriteToStream( std::ostream& ) const;
    std::istream& ReadFromStream( std::istream& );

  private:
    void   InitMembers( int numChannels, int numElements );
    double AddressToIndex( const std::string&, const LabelIndex&, const PhysicalUnit& ) const;

    EncodedString                      mName;
    LabelIndex                         mChannelLabels,
                                       mElementLabels;
    SignalType                         mType;
    PhysicalUnit                       mChannelUnit,
                                       mElementUnit;
    ValueList<PhysicalUnit>            mValueUnits;
    double                             mUpdateRate;
};

inline
std::ostream& operator<<( std::ostream& os, const SignalProperties& s )
{ return s.WriteToStream( os ); }

inline
std::istream& operator>>( std::istream& is, SignalProperties& s )
{ return s.ReadFromStream( is );
}
#endif // SIGNAL_PROPERTIES_H

