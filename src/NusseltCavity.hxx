#pragma once

#include "WCEGases.hpp"

namespace KeffCavity
{
    //! \brief Interface for Nusselt number calculations
    class INusselt
    {
    public:
        INusselt(
          const double L, const double H, const double T1, const double T2, Gases::CGas & gas);

        virtual double value() const = 0;

    protected:
        const double ratio;
        const double dT;
        const double Tavg;
        const Gases::GasProperties gas;
        const double Ra;
    };

    //! \brief Nusselt number for downward heat flow calculations
    class NusseltDownward : public INusselt
    {
    public:
        NusseltDownward(
          const double L, const double H, const double T1, const double T2, Gases::CGas & gas);

        double value() const override;
    };

    //! \brief Nusselt number for upward heat flow calculations
    class NusseltUpward : public INusselt
    {
    public:
        NusseltUpward(
          const double L, const double H, const double T1, const double T2, Gases::CGas & gas);

        double value() const override;

    private:
        double pos(const double x) const;
    };
}   // namespace KeffCavity