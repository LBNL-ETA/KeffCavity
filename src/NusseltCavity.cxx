#include <cmath>
#include "NusseltCavity.hxx"

namespace KeffCavity
{
    //////////////////////////////////////////////////////////////////////
    //// INusselt
    //////////////////////////////////////////////////////////////////////

    INusselt::INusselt(
      const double L, const double H, const double T1, const double T2, Gases::CGas & gasC) :
        ratio(L / H),
        dT(std::abs(T1 - T2)),
        Tavg((T1 + T2) * 0.5),
        gas(gasC.getGasProperties()),
        Ra(9.81 * gas.m_Density * gas.m_Density * gas.m_SpecificHeat * dT * L * L * L
           / (Tavg * gas.m_Viscosity * gas.m_ThermalConductivity))
    {}

    //////////////////////////////////////////////////////////////////////
    //// NusseltDownward
    //////////////////////////////////////////////////////////////////////
    NusseltDownward::NusseltDownward(
      const double L, const double H, const double T1, const double T2, Gases::CGas & gas) :
        INusselt(L, H, T1, T2, gas)
    {}

    double NusseltDownward::value() const
    {
        return 1.0;
    }

    //////////////////////////////////////////////////////////////////////
    //// NusseltUpward
    //////////////////////////////////////////////////////////////////////
    NusseltUpward::NusseltUpward(
      const double L, const double H, const double T1, const double T2, Gases::CGas & gas) :
        INusselt(L, H, T1, T2, gas)
    {}

    double NusseltUpward::value() const
    {
        double Nu{1.0};
        if((1 < ratio) && (ratio <= 5))
        {
            const auto RaCrit = std::exp((0.721 / ratio) + 7.46);

            const auto Pr = gas.m_PrandlNumber;

            const auto k1 = 1.44 / (1.0 + 0.018 / Pr + 0.00136 / (Pr * Pr));
            const auto k2 = pow(Ra, 1.0 / 3.0) / (75.0 * std::exp(1.5 * std::pow(Pr, -0.5)));

            Nu = 1.0 + pos((1.0 - RaCrit / Ra)) * (k1 + 2.0 * std::pow(k2, 1.0 - std::log(k2)))
                 + pos((pow((Ra / 5830), 1.0 / 3.0) - 1.0))
                     * (1.0 - std::exp(-0.95 * pos(std::pow((Ra / RaCrit), 1.0 / 3.0) - 1.0)));
        }
        if(ratio > 5)
        {
            Nu = 1 + 1.44 * pos((1.0 - 1708.0 / Ra)) + pos(std::pow(Ra / 5830.0, 1.0 / 3.0) - 1.0);
        }
        return Nu;
    }

    double NusseltUpward::pos(const double x) const
    {
        return (x < 0.0) ? 0.0 : x;
    }


}   // namespace KeffCavity