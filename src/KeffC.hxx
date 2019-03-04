#pragma once

#include "GravityVector.hxx"
#include "WCEGases.hpp"

namespace KeffCavity
{
    //! Effective conductivity calculation standard
    enum class KeffStandard
    {
        NFRC,
        CENISO,
        USERDIMENSION,
        ISO15099
    };

    //! Flag showing whether or not cavity is ventilated
    enum class Ventilated
    {
        NO,
        YES
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

    //! Flag to show whether or not to include radiation in cavity calculations.
    enum class RadiationCalculation
    {
        No,
        Yes
    };

    enum class RadiationMethod
    {
        OriginalImplementation,
        ISO15099
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
        double L; // Cavity length
        double H; // Cavity height
    };

    //////////////////////////////////////////////////////////////////////////////////////
    /// CavityData
    //////////////////////////////////////////////////////////////////////////////////////

    //! Class that holds necessary cavity data used in effective conductivity calculations
    class CavityData
    {
    public:
        CavityData(ScreenFlow screenFlow,
                   double maxXDimension,
                   double maxYDimension,
                   double jambHeight,
                   double area,
                   const CavitySide & side1,
                   const CavitySide & side2,
                   const GravityVector & gravity = {0.0, -1.0, 0.0},
                   Ventilated ventilated = Ventilated::NO,
                   RadiationCalculation radiationCalculation = RadiationCalculation::Yes,
                   const Gases::CGas & gas = Gases::CGas());

        //! Calculates cavity dimension in heat flow direction.
        CavityFlowDimensions cavityFlowDimension();

        double keff(KeffStandard standard);

    private:

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
        GravitySector gravityDirection(const GravityVector & gravity);
        CavityHeatFlow heatFlowDirection(ScreenFlow screenFlow, const GravityVector & gravity);

        ScreenFlow screenFlow;
        double maxXDimension;
        double maxYDimension;
        double jambHeight;
        double area;
        CavitySide side1;
        CavitySide side2;
        GravityVector gravity;
        Ventilated ventilated;
        RadiationCalculation radiationCalculation;
        Gases::CGas gas;
        CavityHeatFlow cavityHeatFlow;

        double calcISO15099Nu();
    };

    //////////////////////////////////////////////////////////////////////////////////////
    /// KeffCavity
    //////////////////////////////////////////////////////////////////////////////////////

    //! Class to calculate effective cavity conductivity
    class KeffCavity
    {
    public:
        KeffCavity(const CavityData & cavity,
                   KeffStandard standard,
                   RadiationCalculation radiationCalculation,
                   RadiationMethod radiationMethod);

        double effectiveConductivity();

    private:

        CavityData cavity;
        KeffStandard standard;
        RadiationCalculation radiationCalculation;
        RadiationMethod radiationMethod;
    };
}   // namespace KeffCavity