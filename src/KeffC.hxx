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
               const CavitySide & side1,
               const CavitySide & side2,
               RadiationCalculation radiationCalculation = RadiationCalculation::Yes);

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

        //! Calculation of convective part of thermal conductivity
        virtual double convKeff() = 0;

        //! Calculation of radiative part of thermal conductivity
        virtual double radKeff() const = 0;

        ScreenFlow screenFlow;
        double maxXDimension;
        double maxYDimension;
        CavitySide side1;
        CavitySide side2;
        RadiationCalculation radiationMethod;
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

        //! Calculates cavity dimension in heat flow direction.
        CavityFlowDimensions cavityFlowDimension() const;

    private:
        double calcNu();
        double convKeff() override;
        double radKeff() const override;
        //! Calculates sector in which gravity vector is pointing out.
        GravitySector gravityDirection(const GravityVector & g) const;
        CavityHeatFlow heatFlowDirection(const GravityVector & g);

        //! Calculates thickness of cavity in the direction of heat flow
        double calcThicknessInHeatFlowDirection() const;


        const double jambHeight;
        GravityVector gravity;
        Gases::CGas gas;
        CavityHeatFlow cavityHeatFlow;
        double thicknessInHeatFlowDirection;
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
                  const CavitySide & side1,
                  const CavitySide & side2,
                  RadiationCalculation radiationCalculation);

    private:
        double convKeff() override;

        double radKeff() const override;

    private:
        const double area;
        double d;
        double b;
    };

}   // namespace KeffCavity