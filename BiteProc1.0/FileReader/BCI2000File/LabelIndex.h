////////////////////////////////////////////////////////////////////////////////
// $Id: LabelIndex.h 3099 2011-02-10 16:08:47Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A helper class that represents string labels for indexing
//   matrix and list parameters, and signals.
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
#ifndef LABEL_INDEX_H
#define LABEL_INDEX_H

#include <iostream>
#include <map>
#include <vector>
#include "EncodedString.h"

class LabelIndex
{
  typedef std::map<EncodedString, size_t>  IndexBase;
  typedef std::vector<IndexBase::key_type> IndexReverse;

 public:
  LabelIndex()
    : mNeedSync( false )
    { Reset(); }
  ~LabelIndex() {}

  // Forward lookup.
  IndexBase::mapped_type operator[]( const std::string& ) const;
  // We need an additional Exists() function because operator[] must return 0
  // for nonexisting labels.
  bool Exists( const std::string& ) const;
  // A reverse lookup operator.
  const std::string& operator[]( size_t ) const;
  std::string& operator[]( size_t );

  bool IsTrivial() const;
  static const std::string& TrivialLabel( size_t );

  // Stream I/O.
  std::ostream& WriteToStream( std::ostream& ) const;
  std::istream& ReadFromStream( std::istream& );

  LabelIndex& Clear()
              { return Resize( 0 ); }
  LabelIndex& Resize( size_t );
  int         Size() const
              { return mReverseIndex.size(); }

 private:
  void Sync() const;
  void Reset()
  {
    mReverseIndex.clear();
    mForwardIndex.clear();
    Resize( 1 );
  }

 private:
  // This is the maintained index.
  IndexReverse      mReverseIndex;
  // This is a cache for the more probable lookup direction.
  mutable bool      mNeedSync;
  mutable IndexBase mForwardIndex;
};

inline
std::ostream& operator<<( std::ostream& os, const LabelIndex& l )
{ return l.WriteToStream( os ); }

inline
std::istream& operator>>( std::istream& is, LabelIndex& l )
{ return l.ReadFromStream( is ); }

#endif // LABEL_INDEX_H

