////////////////////////////////////////////////////////////////////////////////
// $Id: ValueList.h 3099 2011-02-10 16:08:47Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class template that encapsulates list i/o.
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
#ifndef VALUE_LIST_H
#define VALUE_LIST_H

#include <iostream>
#include <vector>
#include "Brackets.h"

template<typename T> class ValueList: public std::vector<T>
{
  public:
    ValueList( size_t inSize = 0 ) : std::vector<T>( inSize ) {}

    std::ostream& WriteToStream( std::ostream& ) const;
    std::istream& ReadFromStream( std::istream& );
};

template<typename T>
std::istream&
ValueList<T>::ReadFromStream( std::istream& is )
{
  this->clear();
  is >> std::ws;
  char closingBracket = '\0';
  if( Brackets::IsOpening( is.peek() ) )
    closingBracket = Brackets::ClosingMatch( is.get() );
  else
    is.setstate( std::ios::failbit );
  is >> std::ws;

  T t;
  while( is.peek() != closingBracket && is >> t >> std::ws )
    push_back( t );

  if( is.peek() == closingBracket )
    is.get();
  else
    is.setstate( std::ios::failbit );
  return is;
}

template<typename T>
std::ostream&
ValueList<T>::WriteToStream( std::ostream& os ) const
{
  os << Brackets::OpeningDefault;
  typename ValueList<T>::const_iterator i;
  for( i = this->begin(); i != this->end(); ++i )
    os << *i << " ";
  os << Brackets::ClosingDefault;
  return os;
}

template<typename T>
inline std::ostream&
operator<<( std::ostream& s, const ValueList<T>& v )
{
  return v.WriteToStream( s );
}

template<typename T>
inline std::istream&
operator>>( std::istream& s, ValueList<T>& v )
{
  return v.ReadFromStream( s );
}

#endif // VALUE_LIST_H

