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

    //! Flag to show which radiation methodology is used.
    enum class RadiationMethod
    {
        NoRadiation,
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
        Cavity(KeffStandard keffStandard,
                   ScreenFlow screenFlow,
                   double maxXDimension,
                   double maxYDimension,
                   double jambHeight,
                   double area,
                   const CavitySide & side1,
                   const CavitySide & side2,
                   const GravityVector & gravity = {0.0, -1.0, 0.0},
                   Ventilated ventilated = Ventilated::NO,
                   RadiationMethod radiationMethod = RadiationMethod::ISO15099,
                   const Gases::CGas & gas = Gases::CGas());

        //! Calculates cavity dimension in heat flow direction.
        CavityFlowDimensions cavityFlowDimension() const;

        double effectiveConductivity();

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
        GravitySector gravityDirection(const GravityVector & gravity) const;
        CavityHeatFlow heatFlowDirection(ScreenFlow screenFlow, const GravityVector & gravity);

        //! Calculates thickness of cavity in the direction of heat flow
        double calcThicknessInHeatFlowDirection(ScreenFlow screenFlow) const;

        //! Calcualtion of radiation according to ISO15099 standard
        double radKeffISO15099() const;

        KeffStandard keffStandard;
        ScreenFlow screenFlow;
        double maxXDimension;
        double maxYDimension;
        double jambHeight;
        double area;
        CavitySide side1;
        CavitySide side2;
        GravityVector gravity;
        Ventilated ventilated;
        RadiationMethod radiationMethod;
        Gases::CGas gas;
        CavityHeatFlow cavityHeatFlow;
        double thicknessInHeatFlowDirection;

        double calcISO15099Nu();
    };
}   // namespace KeffCavity