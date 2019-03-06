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

    //! \brief Class that holds necessary cavity data used in effective conductivity calculations
    class Cavity
    {
    public:
        virtual ~Cavity();
        Cavity(
          ScreenFlow screenFlow,      //!< Heat flow relative to computer screen
          double maxXDimension,       //!< Maximal dimension of cavity in x (horizontal) direction
          double maxYDimension,       //!< Maximal dimension of cavity in y (vertical) direction
          const CavitySide & side1,   //!< Surface 1 of frame cavity (temperature and emissivity)
          const CavitySide & side2,   //!< Surface 2 of frame cavity (temperature and emissivity)
          RadiationCalculation radiationCalculation =
            RadiationCalculation::Yes   //!< Flag to turn on/off radiation calculations
        );

        double effectiveConductivity();

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

    //! \brief Frame cavity thermal conductivity calculations according to ISO 15099 standard.
    class CavityISO10599 : public Cavity
    {
    public:
        CavityISO10599(
          ScreenFlow screenFlow,   //!< Heat flow relative to computer screen
          double maxXDimension,    //!< Maximal dimension of cavity in x (horizontal) direction
          double maxYDimension,    //!< Maximal dimension of cavity in y (vertical) direction
          double jambHeight,   //!< Jamb dimension of frame cavity (jamb is measured direction into
                               //!< the screen)
          const CavitySide & side1,   //!< Surface 1 of frame cavity (temperature and emissivity)
          const CavitySide & side2,   //!< Surface 2 of frame cavity (temperature and emissivity)
          double pressure = 101325,   //!< Pressure of gas inside the frame cavity
          const GravityVector & gravity = {0.0, -1.0, 0.0},   //!< Gravity unit vector
          RadiationCalculation radiationCalculation =
            RadiationCalculation::Yes,               //!< Flag to turn on/off radiation calculations
          const Gases::CGas & gas = Gases::CGas(),   //!< Gas that fills frame cavity
          Ventilated ventilated =
            Ventilated::No   //!< Flag that shows if frame cavity is ventilated
        );

        //! Calculates cavity dimension in heat flow direction. Dimension L will always be
        //! horizontal compared to heat flow direction, while H will be vertical.
        CavityFlowDimensions cavityFlowDimension() const;

    private:
        double calcNu();
        double convKeff() override;
        double radKeff() const override;

        //! Calculates sector to which gravity vector is pointing out.
        static GravitySector gravityDirection(const GravityVector & g);
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

    //! \brief Frame cavity thermal conductivity calculations according to CEN standard..
    class CavityCEN : public Cavity
    {
    public:
        CavityCEN(
          ScreenFlow screenFlow,      //!< Heat flow direction
          double maxXDimension,       //!< Maximal dimension of cavity in x (horizontal) direction
          double maxYDimension,       //!< Maximal dimension of cavity in y (vertical) direction
          double area,                //!< Frame cavity area (relative to the screen)
          const CavitySide & side1,   //!< Surface 1 of frame cavity (temperature and emissivity)
          const CavitySide & side2,   //!< Surface 2 of frame cavity (temperature and emissivity)
          RadiationCalculation
            radiationCalculation   //!< Flag to turn on/off radiation calculations
        );

    private:
        double convKeff() override;

        double radKeff() const override;

        const double area;
        double d;
        double b;
    };

}   // namespace KeffCavity
