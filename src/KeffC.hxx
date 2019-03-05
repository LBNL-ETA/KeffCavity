#pragma once

#include "GravityVector.hxx"
#include "WCEGases.hpp"

namespace KeffCavity
{
    //! Flag showing whether or not cavity is ventilated
    enum class Ventilated
    {
        No,
        Yes
    };

    //! Heat flow direction relative to the cavity
    enum class CavityHeatFlow
    {
        Horizontal,
        Upward,
        Downward
    };

    //! Heat flow direction relative to the screen
    enum class ScreenFlow
    {
        Left,
        Right,
        Up,
        Down
    };

    //! Flag to show which radiation methodology is used.
    enum class RadiationCalculation
    {
        No,
        Yes
    };

    //! Structure to hold data for one side of the cavity
    struct CavitySide
    {
        double temperature;
        double emissivity;
    };

    //! Represents cavity dimension in the direction of heat flow
    struct CavityFlowDimensions
    {
        double L;   // Cavity length
        double H;   // Cavity height
    };

    //////////////////////////////////////////////////////////////////////////////////////
    /// Cavity
    //////////////////////////////////////////////////////////////////////////////////////

    //! Class that holds necessary cavity data used in effective conductivity calculations
    class Cavity
    {
    public:
        Cavity(ScreenFlow screenFlow,
               double maxXDimension,
               double maxYDimension,
               double jambHeight,
               const CavitySide & side1,
               const CavitySide & side2,
               double pressure = 101325,
               const GravityVector & gravity = {0.0, -1.0, 0.0},
               RadiationCalculation radiationMethod = RadiationCalculation::Yes,
               const Gases::CGas & gas = Gases::CGas());

        //! Calculates cavity dimension in heat flow direction.
        CavityFlowDimensions cavityFlowDimension() const;

        double effectiveConductivity();

    protected:
        enum class GravitySector
        {
            PositiveX,
            NegativeX,
            PositiveY,
            NegativeY,
            PositiveZ,
            NegativeZ
        };

        //! Calculates sector in which gravity vector is pointing out.
        GravitySector gravityDirection(const GravityVector & gravity) const;
        CavityHeatFlow heatFlowDirection(ScreenFlow screenFlow, const GravityVector & gravity);

        //! Calculates thickness of cavity in the direction of heat flow
        double calcThicknessInHeatFlowDirection(ScreenFlow screenFlow) const;

        //! Calculation of convective part of thermal conductivity
        virtual double convKeff() = 0;

        //! Calculation of radiative part of thermal conductivity
        virtual double radKeff() const = 0;

        ScreenFlow screenFlow;
        double maxXDimension;
        double maxYDimension;
        double jambHeight;
        CavitySide side1;
        CavitySide side2;
        GravityVector gravity;
        RadiationCalculation radiationMethod;
        Gases::CGas gas;
        CavityHeatFlow cavityHeatFlow;
        double thicknessInHeatFlowDirection;
    };

    //////////////////////////////////////////////////////////////////////////////////////
    /// CavityISO10599
    //////////////////////////////////////////////////////////////////////////////////////

    class CavityISO10599 : public Cavity
    {
    public:
        CavityISO10599(ScreenFlow screenFlow,
                       double maxXDimension,
                       double maxYDimension,
                       double jambHeight,
                       const CavitySide & side1,
                       const CavitySide & side2,
                       double pressure = 101325,
                       const GravityVector & gravity = {0.0, -1.0, 0.0},
                       RadiationCalculation radiationCalculation = RadiationCalculation::Yes,
                       const Gases::CGas & gas = Gases::CGas(),
                       Ventilated ventilated = Ventilated::No);

    private:
        double calcNu();
        double convKeff() override;
        double radKeff() const override;

        Ventilated ventilated;
    };

    //////////////////////////////////////////////////////////////////////////////////////
    /// CavityCEN
    //////////////////////////////////////////////////////////////////////////////////////

    class CavityCEN : public Cavity
    {
    public:
        CavityCEN(ScreenFlow screenFlow,
                  double maxXDimension,
                  double maxYDimension,
                  const double area,
                  double jambHeight,
                  const CavitySide & side1,
                  const CavitySide & side2,
                  double pressure,
                  const GravityVector & gravity,
                  RadiationCalculation radiationMethod,
                  const Gases::CGas & gas);

    private:
        double convKeff() override;

        double radKeff() const override;

    private:
        const double area;
        double d;
        double b;
    };

}   // namespace KeffCavity