/*
  Copyright (C) 2017 TNO

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

#ifndef OPM_AQUIFERFETP_HPP
#define OPM_AQUIFERFETP_HPP

/*
  The Aquiferfetp which stands for AquiferFetkovich is a data container object meant to hold the data for the fetkovich aquifer model.
  This includes the logic for parsing as well as the associated tables. It is meant to be used by opm-grid and opm-simulators in order to
  implement the Fetkovich analytical aquifer model in OPM Flow.
*/
#include <vector>


namespace Opm {

class Deck;
class DeckRecord;

class Aquifetp {
    public:

    struct AQUFETP_data{
        AQUFETP_data(const DeckRecord& record);
        bool operator==(const AQUFETP_data& other) const;

        int aquiferID;
        int pvttableID;
        double  J, // Specified Productivity Index
                C_t, // total rock compressibility
                V0, // initial volume of water in aquifer
                d0; // aquifer datum depth
        std::pair<bool, double> p0;
    };

    Aquifetp(const Deck& deck);
    Aquifetp(const std::vector<Aquifetp::AQUFETP_data>& data);
    const std::vector<Aquifetp::AQUFETP_data>& data() const;

    std::size_t size() const;
    std::vector<Aquifetp::AQUFETP_data>::const_iterator begin() const;
    std::vector<Aquifetp::AQUFETP_data>::const_iterator end() const;
    bool operator==(const Aquifetp& other) const;
private:
    std::vector<Aquifetp::AQUFETP_data> m_aqufetp;
};
}


#endif
