////////////////////////////////////////////////////////////////////////////////
// $Id: Brackets.cpp 3099 2011-02-10 16:08:47Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: This helper class defines what we accept as delimiting
//   single-character symbol pairs for index lists and sub-parameters in a
//   parameter line.
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

#include "Brackets.h"

using namespace std;

const string&
Brackets::BracketPairs()
{ // Bracket pairs that are recognized when reading.
  static string bracketPairs = "{}[]";
  return bracketPairs;
}

