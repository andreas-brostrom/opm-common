/*
  Copyright 2024 Norce.

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
#ifndef OPM_PARSER_CO2STORECONFIG_HPP
#define	OPM_PARSER_CO2STORECONFIG_HPP

#include <cstddef>
#include <vector>
#include <string>

namespace Opm {

class Deck;

  class Co2StoreConfig {
  public:

    enum class SaltMixingType {
        NONE,        // Pure water
        MICHAELIDES, // MICHAELIDES 1971 (default)
    };
    
    enum class LiquidMixingType {
        NONE,     // Pure water
        IDEAL,    // Ideal mixing
        DUANSUN,  // Add heat of dissolution for CO2 according to Fig. 6 in Duan and Sun 2003. (kJ/kg) (default)
    };

    enum class GasMixingType {
        NONE,   // Pure co2 (default)
        IDEAL,  // Ideal mixing
    };

    Co2StoreConfig();

    explicit Co2StoreConfig(const Deck& deck);

    template<class Serializer>
    void serializeOp(Serializer& serializer)
    {
       serializer(brine_type);
       serializer(liquid_type);
       serializer(gas_type);
    }
    bool operator==(const Co2StoreConfig& other) const;

    SaltMixingType brine_type;
    LiquidMixingType liquid_type;
    GasMixingType gas_type;

  private:

    SaltMixingType string2enumSalt(const std::string& input) const;
    LiquidMixingType string2enumLiquid(const std::string& input) const;
    GasMixingType string2enumGas(const std::string& input) const; 
  };
}

#endif // OPM_PARSER_CO2STORECONFIG_HPP
