#pragma once

#include <memory>
#include "WCEGases.hpp"

namespace KeffCavity
{
    enum class CavityHeatFlow;

    //! \brief Interface for Nusselt number calculations
    class INusselt
    {
    public:
        INusselt(
          const double L,       //!< Frame cavity length as described in ISO15099 standard
          const double H,       //!< Frame cavity height as described in ISO15099 standard
          const double T1,      //!< Temperature of surface 1
          const double T2,      //!< Temperature of surface 2
          Gases::CGas & gas,    //!< Cavity gas fill
          const double length   //!< Length in the direction of heat flow (set up by child class)
        );

        virtual double value() const = 0;

    protected:
        double RaCalc(double dT, double Tavg, double length, Gases::CGas & gas);
        const double ratio;
        const double dT;
        const double Tavg;
        const Gases::GasProperties gas;
        const double Ra;
    };

    //! \brief Nusselt number for downward heat flow calculations
    class NusseltISO15099Downward : public INusselt
    {
    public:
        NusseltISO15099Downward(
          const double L, const double H, const double T1, const double T2, Gases::CGas & gas);

        double value() const override;
    };

    //! \brief Nusselt number for upward heat flow calculations
    class NusseltISO15099Upward : public INusselt
    {
    public:
        NusseltISO15099Upward(
          const double L, const double H, const double T1, const double T2, Gases::CGas & gas);

        double value() const override;

    private:
        double pos(const double x) const;
    };

    //! \brief Nusselt number for horizontal heat flow calculations
    class NusseltISO15099Horizontal : public INusselt
    {
    public:
        NusseltISO15099Horizontal(
          const double L, const double H, const double T1, const double T2, Gases::CGas & gas);

        double value() const override;

    private:
        double Nu1(double aRatio) const;
        double Nu2(double aRatio) const;
        double LinearInterp(double x, double x1, double y1, double x2, double y2) const;
    };

    class NusseltISO15099Factory
    {
    public:
        static std::unique_ptr<INusselt> create(CavityHeatFlow cavityHeatFlow,
                                                const double L,
                                                const double H,
                                                const double T1,
                                                const double T2,
                                                Gases::CGas & gas);
    };

}   // namespace KeffCavity