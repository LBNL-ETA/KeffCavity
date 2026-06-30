#pragma once

#include "GravityVector.hxx"
#include <WCEGases.hpp>

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

    //! Flag to select the radiation methodology. Radiation is always included in
    //! effective conductivity; only the method differs.
    //!
    //! Simplified: net-emissivity formula with view-factor correction (the path
    //! implemented today in radKeff()).
    //!
    //! Detailed: full view-factor / enclosure radiation. Not yet implemented in
    //! Windows-CalcEngine; currently falls back to Simplified so radiation is
    //! never silently omitted.
    enum class RadiationCalculation
    {
        Simplified,
        Detailed
    };

    //! Structure to hold data for one side of the cavity
    struct CavitySide
    {
        CavitySide(double temperature, double emissivity);

        double temperature{0};
        double emissivity{0};
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

    //! \brief Class that holds necessary cavity data used in effective conductivity calculations
    class Cavity
    {
    public:
        virtual ~Cavity();

        //! \brief Class to calculate effective properties of cavity
        //!
        //! \param screenFlow Heat flow relative to computer screen
        //! \param maxXDimension Maximal dimension of cavity in x (horizontal) direction
        //! \param maxYDimension Maximal dimension of cavity in y (vertical) direction
        //! \param side1 Surface 1 of frame cavity (temperature and emissivity)
        //! \param side2 Surface 2 of frame cavity (temperature and emissivity)
        //! \param radiationCalculation Flag to turn on/off radiation calculations
        Cavity(ScreenFlow screenFlow,
               double maxXDimension,
               double maxYDimension,
               const CavitySide & side1,
               const CavitySide & side2,
               const Gases::CGas & gas,
               RadiationCalculation radiationCalculation = RadiationCalculation::Simplified);

        double effectiveConductivity();
        double effectiveDiffusionResistanceFactor();
        double nusselt();

    protected:
        //! \brief Information for gravity vector orientation.
        //!
        //! For cavity calculations gravity vector needs to be assigned to one of six cardinal
        //! directions used in calculations
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

        virtual double Nu() = 0;

        ScreenFlow screenFlow;
        double maxXDimension;
        double maxYDimension;
        CavitySide side1;
        CavitySide side2;
        Gases::CGas gas;
        RadiationCalculation radiationMethod;
    };

    //////////////////////////////////////////////////////////////////////////////////////
    /// CavityISO10599
    //////////////////////////////////////////////////////////////////////////////////////

    //! \brief Frame cavity thermal conductivity calculations according to ISO 15099 standard.
    class CavityISO10599 : public Cavity
    {
    public:
        //! \brief Construction of cavity according to ISO 15099 standard
        //!
        //! \param screenFlow Heat flow relative to computer screen
        //! \param maxXDimension Maximal dimension of cavity in x (horizontal) direction
        //! \param maxYDimension Maximal dimension of cavity in y (vertical) direction
        //! \param jambHeight Jamb dimension of frame cavity (jamb is measured direction into
        //! the screen)
        //! \param side1 Surface 1 of frame cavity (temperature and emissivity)
        //! \param side2 Surface 2 of frame cavity (temperature and emissivity)
        //! \param pressure Pressure of gas inside the frame cavity
        //! \param gravity Gravity unit vector
        //! \param radiationCalculation Flag to turn on/off radiation calculations
        //! \param gas Gas that fills frame cavity
        //! \param ventilated Flag that shows if frame cavity is ventilated
        CavityISO10599(ScreenFlow screenFlow,
                       double maxXDimension,
                       double maxYDimension,
                       double jambHeight,
                       const CavitySide & side1,
                       const CavitySide & side2,
                       double pressure = 101325,
                       const FenestrationCommon::GravityVector & gravity = {0.0, -1.0, 0.0},
                       RadiationCalculation radiationCalculation = RadiationCalculation::Simplified,
                       const Gases::CGas & gas = Gases::CGas(),
                       Ventilated ventilated = Ventilated::No);

        //! Calculates cavity dimension in heat flow direction. Dimension L will always be
        //! horizontal compared to heat flow direction, while H will be vertical.
        CavityFlowDimensions cavityFlowDimension() const;

    private:
        double Nu() override;
        double convKeff() override;
        double radKeff() const override;

        //! Calculates sector to which gravity vector is pointing out.
        static GravitySector gravityDirection(const FenestrationCommon::GravityVector & g);
        CavityHeatFlow heatFlowDirection(const FenestrationCommon::GravityVector & g);

        //! Calculates thickness of cavity in the direction of heat flow
        double calcThicknessInHeatFlowDirection() const;


        const double jambHeight;
        FenestrationCommon::GravityVector gravity;
        CavityHeatFlow cavityHeatFlow;
        double thicknessInHeatFlowDirection;
        Ventilated ventilated;
    };

    //////////////////////////////////////////////////////////////////////////////////////
    /// CavityCEN
    //////////////////////////////////////////////////////////////////////////////////////

    //! \brief Frame cavity thermal conductivity calculations according to CEN standard..
    class CavityCEN : public Cavity
    {
    public:
        //! \brief Construction of cavity according to CEN standard
        //!
        //! \param screenFlow Heat flow direction
        //! \param maxXDimension Maximal dimension of cavity in x (horizontal) direction
        //! \param maxYDimension Maximal dimension of cavity in y (vertical) direction
        //! \param area Frame cavity area (relative to the screen)
        //! \param side1 Surface 1 of frame cavity (temperature and emissivity)
        //! \param side2 Surface 2 of frame cavity (temperature and emissivity)
        //! \param gas Gas that fills frame cavity
        //! \param radiationCalculation Flag to turn on/off radiation calculations
        CavityCEN(ScreenFlow screenFlow,
                  double maxXDimension,
                  double maxYDimension,
                  double area,
                  const CavitySide & side1,
                  const CavitySide & side2,
                  const Gases::CGas & gas = Gases::CGas(),
                  RadiationCalculation radiationCalculation = RadiationCalculation::Simplified);

    private:
        double convKeff() override;

        double radKeff() const override;

        double Nu() override;

        double d;
        double b;
    };

}   // namespace KeffCavity
