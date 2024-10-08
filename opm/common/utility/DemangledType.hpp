/*
  Copyright 2022 Equinor ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPM_DEMANGLEDTYPE_HPP
#define OPM_DEMANGLEDTYPE_HPP

#include <string>
#include <typeinfo>

namespace Opm {

//! \brief Returns demangled name of symbol.
//! \details Non-demangled name is return if demangling is not supported
std::string demangle(const char* mangled_symbol);

//! \brief Returns demangled information of a type.
//! \details Non-demangled type-id symbol is returned if demangling is not supported
template<typename T>
std::string getDemangledType() {
    return demangle(typeid(T).name());
}

}
#endif //OPM_DEMANGLEDTYPE_HPP
