#include <cmath>
#include <functional>
#include "NusseltCavity.hxx"
#include "KeffC.hxx"

namespace KeffCavity
{
    //////////////////////////////////////////////////////////////////////
    //// INusselt
    //////////////////////////////////////////////////////////////////////

    INusselt::INusselt(const double L,
                       const double H,
                       const double T1,
                       const double T2,
                       Gases::CGas & gasC,
                       const double length) :
        ratio(L / H),
        dT(std::abs(T1 - T2)),
        Tavg((T1 + T2) * 0.5),
        gas(gasC.getGasProperties()),
        Ra(RaCalc(dT, Tavg, length, gasC))
    {}

    double INusselt::RaCalc(double dT, double Tavg, double length, Gases::CGas & gasC)
    {
        auto gas = gasC.getGasProperties();
        return 9.81 * gas.m_Density * gas.m_Density * gas.m_SpecificHeat * dT * length * length
               * length / (Tavg * gas.m_Viscosity * gas.m_ThermalConductivity);
    }

    //////////////////////////////////////////////////////////////////////
    //// NusseltISO15099Downward
    //////////////////////////////////////////////////////////////////////
    NusseltISO15099Downward::NusseltISO15099Downward(
      const double L, const double H, const double T1, const double T2, Gases::CGas & gas) :
        INusselt(L, H, T1, T2, gas, H)
    {}

    double NusseltISO15099Downward::value() const
    {
        return 1.0;
    }

    //////////////////////////////////////////////////////////////////////
    //// NusseltISO15099Upward
    //////////////////////////////////////////////////////////////////////
    NusseltISO15099Upward::NusseltISO15099Upward(
      const double L, const double H, const double T1, const double T2, Gases::CGas & gas) :
        INusselt(L, H, T1, T2, gas, H)
    {}

    double NusseltISO15099Upward::value() const
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

    double NusseltISO15099Upward::pos(const double x) const
    {
        return (x < 0.0) ? 0.0 : x;
    }

    //////////////////////////////////////////////////////////////////////
    //// NusseltISO15099Horizontal
    //////////////////////////////////////////////////////////////////////
    NusseltISO15099Horizontal::NusseltISO15099Horizontal(
      const double L, const double H, const double T1, const double T2, Gases::CGas & gas) :
        INusselt(L, H, T1, T2, gas, L)
    {}

    double NusseltISO15099Horizontal::value() const
    {
        double Nu{0};
        if(ratio > 2.0)
        {
            Nu = Nu1(ratio);
        }
        if(ratio < 1.0 / 5.0)
        {
            Nu = Nu2(ratio);
        }
        if(ratio >= 1.0 / 5.0 && ratio <= 2.0)
        {
            const auto lowPoint = 1.0 / 5.0;
            const auto highPoint = 2.0;
            Nu = LinearInterp(ratio, lowPoint, Nu1(lowPoint), highPoint, Nu2(highPoint));
        }
        return Nu;
    }

    double NusseltISO15099Horizontal::Nu1(const double aRatio) const
    {
        return 1.0
               + std::pow(
                 std::pow(2.756e-6 * Ra * Ra * std::pow(1.0 / aRatio, 8.0), -0.386)
                   + std::pow(0.623 * std::pow(Ra, 1.0 / 5.0) * std::pow(aRatio, 2.0 / 5.0),
                              -0.386),
                 -2.59);
    }

    double NusseltISO15099Horizontal::Nu2(const double aRatio) const
    {
        auto Nu_ct = std::pow(
          1.0 + std::pow(0.104 * std::pow(Ra, 0.293) / (1.0 + std::pow(6310.0 / Ra, 1.36)), 3.0),
          1.0 / 3.0);
        auto Nu_l = 0.242 * pow(Ra * aRatio, 0.273);
        auto Nu_t = 0.0605 * pow(Ra, 1.0 / 3.0);
        Nu_ct = std::max(Nu_ct, Nu_l);
        return std::max(Nu_ct, Nu_t);
    }

    double NusseltISO15099Horizontal::LinearInterp(
      const double x, const double x1, const double y1, const double x2, const double y2) const
    {
        if(x <= x1)
            return y1;
        if(x >= x2)
            return y2;


        return (y1 + (x - x1) * (y2 - y1) / (x2 - x1));
    }

    std::unique_ptr<INusselt> NusseltISO15099Factory::create(const CavityHeatFlow cavityHeatFlow,
                                                             const double L,
                                                             const double H,
                                                             const double T1,
                                                             const double T2,
                                                             Gases::CGas & gas)
    {
        switch(cavityHeatFlow)
        {
            case CavityHeatFlow::Horizontal:
                return std::unique_ptr<NusseltISO15099Horizontal>(new NusseltISO15099Horizontal(L, H, T1, T2, gas));
            case CavityHeatFlow::Upward:
                return std::unique_ptr<NusseltISO15099Upward>(new NusseltISO15099Upward(L, H, T1, T2, gas));
            case CavityHeatFlow::Downward:
                return std::unique_ptr<NusseltISO15099Downward>(new NusseltISO15099Downward(L, H, T1, T2, gas));
        }
        return nullptr;
    }
}   // namespace KeffCavity