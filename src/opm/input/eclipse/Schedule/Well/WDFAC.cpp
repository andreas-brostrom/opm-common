/*
  Copyright 2023 Equinor.

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

#include <opm/input/eclipse/Schedule/Well/WDFAC.hpp>

#include <opm/io/eclipse/rst/well.hpp>
#include <opm/output/eclipse/VectorItems/well.hpp>

#include <opm/input/eclipse/Schedule/Well/Connection.hpp>
#include <opm/input/eclipse/Schedule/Well/WellConnections.hpp>

#include <opm/input/eclipse/Units/Dimension.hpp>
#include <opm/input/eclipse/Units/UnitSystem.hpp>
#include <opm/input/eclipse/Units/Units.hpp>

#include <opm/input/eclipse/Deck/DeckRecord.hpp>

#include <opm/input/eclipse/Parser/ParserKeywords/W.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <string>

namespace Opm {

    WDFAC::Correlation WDFAC::Correlation::serializationTestObject()
    {
        return { 1.23, 0.456, 0.457 };
    }

    bool WDFAC::Correlation::operator==(const Correlation& other) const
    {
        return (this->coeff_a == other.coeff_a)
            && (this->exponent_b == other.exponent_b)
            && (this->exponent_c == other.exponent_c)
            ;
    }

    // -------------------------------------------------------------------------

    WDFAC WDFAC::serializationTestObject()
    {
        WDFAC result;

        result.m_type = WDFacType::DAKEMODEL;
        result.m_d = 0.458;
        result.m_total_cf = 1.0;
        result.m_corr = Correlation::serializationTestObject();

        return result;
    }

    void WDFAC::updateWDFAC(const DeckRecord& record)
    {
        m_d = record.getItem<ParserKeywords::WDFAC::DFACTOR>().getSIDouble(0);

        m_type = WDFacType::DFACTOR;
    }

    void WDFAC::updateWDFACCOR(const DeckRecord& record)
    {
        this->m_corr.coeff_a = record.getItem<ParserKeywords::WDFACCOR::A>().getSIDouble(0);
        this->m_corr.exponent_b = record.getItem<ParserKeywords::WDFACCOR::B>().getSIDouble(0);
        this->m_corr.exponent_c = record.getItem<ParserKeywords::WDFACCOR::C>().getSIDouble(0);

        m_type = WDFacType::DAKEMODEL;
    }

    void WDFAC::updateWDFACType(const WellConnections& connections)
    {
        const auto non_trivial_dfactor =
            std::any_of(connections.begin(), connections.end(),
                [](const auto& conn) { return conn.dFactor() != 0.0; });

        if (non_trivial_dfactor) {
            // Non-trivial D-factors detected.  Use connection D-factors.
            m_type = WDFacType::CON_DFACTOR;
            this->updateTotalCF(connections);
        }
    }

    void WDFAC::updateTotalCF(const WellConnections& connections)
    {
        this->m_total_cf = std::accumulate(connections.begin(), connections.end(), 0.0,
            [](const double tot_cf, const auto& conn) { return tot_cf + conn.CF(); });
    }

    double WDFAC::getDFactor(const Connection& connection, double mu, double rho, double phi) const
    {
        switch (this->m_type) {
        case WDFacType::NONE:
            return 0.0;

        case WDFacType::DFACTOR:
        {
            if (m_total_cf < 0.0) {
                throw std::invalid_argument { "Total connection factor is not set" };
            }

            return this->m_d * this->m_total_cf / connection.CF();
        }

        case WDFacType::DAKEMODEL:
        {
            const double Kh = connection.Kh();
            const double Ke = connection.Ke();
            const double h = Kh / Ke;
            const double rw = connection.rw();

            const auto k_md = unit::convert::to(Ke, prefix::milli*unit::darcy);
            double beta = m_corr.coeff_a * (std::pow(k_md, m_corr.exponent_b) * std::pow(phi, m_corr.exponent_c));
            double specific_gravity = rho / 1.225; // divide by density of air at standard conditions. 
            return beta * specific_gravity * Ke / (h * mu * rw );
        }
        break;

        case WDFacType::CON_DFACTOR:
        {
            const double d = connection.dFactor();

            // If a negative D-factor is set in COMPDAT, then the individual
            // connection D-factor should be used directly.
            if (d < 0.0) {
                return -d;
            }

            if (this->m_total_cf < 0.0) {
                throw std::invalid_argument { "Total connection factor is not set" };
            }

            // If a positive D-factor is set in COMPDAT, then the connection
            // D-factor is treated as a well-level D-factor and thus scaled
            // with the connection transmissibility factor.
            return d * m_total_cf / connection.CF();
        }

        default:
            break;
        }

        return 0.0;
    }

    bool WDFAC::useDFactor() const
    {
        return m_type != WDFacType::NONE;
    }

    bool WDFAC::operator==(const WDFAC& other) const
    {
        return (this->m_type == other.m_type)
            && (this->m_d == other.m_d)
            && (this->m_total_cf == other.m_total_cf)
            && (this->m_corr == other.m_corr)
            ;
    }
}
