/*
  Copyright 2019 Equinor.

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

#ifndef VALVE_HPP_HEADER_INCLUDED
#define VALVE_HPP_HEADER_INCLUDED

#include <map>
#include <utility>
#include <vector>
#include <string>
#include <optional>

#include <opm/input/eclipse/Schedule/MSW/icd.hpp>
#include <opm/input/eclipse/Deck/UDAValue.hpp>
#include <opm/input/eclipse/Schedule/SummaryState.hpp>


namespace Opm {

    class DeckRecord;
    class DeckKeyword;
    class Segment;

    struct ValveUDAEval {
        const SummaryState& summary_state;
        const std::string& well_name;
        const size_t segment_number;

        ValveUDAEval(const SummaryState& summary_state_,
                     const std::string& well_name_,
                     const size_t segment_number_);

        double value(const UDAValue& value, const double udq_default = 0.0) const;
    };


    class Valve {
    public:

        Valve();
        explicit Valve(const DeckRecord& record, const double udq_default = 0.0);
        Valve(double conFlowCoeff,
              double conCrossA,
              double conMaxCrossA,
              double pipeAddLength,
              double pipeDiam,
              double pipeRough,
              double pipeCrossA,
              ICDStatus stat);

        static Valve serializationTestObject();

        // the function will return a map
        // [
        //     "WELL1" : [<seg1, valv1>, <seg2, valv2> ...]
        //     ....
        static std::map<std::string, std::vector<std::pair<int, Valve> > > fromWSEGVALV(const DeckKeyword& keyword, const double udq_default = 0.0);

        // parameters for constriction pressure loss
        double conFlowCoefficient() const;
        double conCrossArea(const std::optional<const ValveUDAEval>& uda_eval = std::nullopt) const;
        inline double conCrossAreaValue() const { return m_con_cross_area_value; }
        double conMaxCrossArea() const;
        double pipeDiameter() const;
        double pipeRoughness() const;
        double pipeCrossArea() const;

        // parameters for pressure loss along the pipe
        double pipeAdditionalLength() const;

        // Status: OPEN or SHUT
        ICDStatus status() const;
        int ecl_status() const;

        void setConMaxCrossArea(const double area);

        void setPipeAdditionalLength(const double length);
        void setPipeDiameter(const double dia);
        void setPipeRoughness(const double rou);
        void setPipeCrossArea(const double area);

        bool operator==(const Valve& data) const;

        template<class Serializer>
        void serializeOp(Serializer& serializer)
        {
            serializer(m_con_flow_coeff);
            serializer(m_con_cross_area);
            serializer(m_con_cross_area_value);
            serializer(m_con_max_cross_area);
            serializer(m_pipe_additional_length);
            serializer(m_pipe_diameter);
            serializer(m_pipe_roughness);
            serializer(m_pipe_cross_area);
            serializer(m_status);
            serializer(m_udq_default);
        }

    private:
        double m_con_flow_coeff;
        UDAValue m_con_cross_area;
        mutable double m_con_cross_area_value;
        double m_con_max_cross_area;

        double m_pipe_additional_length;
        double m_pipe_diameter;
        double m_pipe_roughness;
        double m_pipe_cross_area;
        ICDStatus m_status;

        double m_udq_default{0.0};
    };

}

#endif
