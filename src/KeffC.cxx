#include <cmath>
#include "KeffC.hxx"
#include "NusseltCavity.hxx"

namespace KeffCavity
{
    //////////////////////////////////////////////////////////////////////////////////////
    /// Cavity
    //////////////////////////////////////////////////////////////////////////////////////

    Cavity::Cavity(ScreenFlow screenFlow,
                   double maxXDimension,
                   double maxYDimension,
                   double jambHeight,
                   const CavitySide & side1,
                   const CavitySide & side2,
                   double pressure,
                   const GravityVector & gravity,
                   RadiationCalculation radiationMethod,
                   const Gases::CGas & gasC) :
        screenFlow(screenFlow),
        maxXDimension(maxXDimension),
        maxYDimension(maxYDimension),
        jambHeight(jambHeight),
        area(area),
        side1(side1),
        side2(side2),
        gravity(gravity),
        radiationMethod(radiationMethod),
        gas(gasC),
        cavityHeatFlow(heatFlowDirection(screenFlow, gravity)),
        thicknessInHeatFlowDirection(calcThicknessInHeatFlowDirection(screenFlow))
    {
        gas.setTemperatureAndPressure(0.5 * (side1.temperature + side2.temperature), pressure);
    }

    Cavity::GravitySector Cavity::gravityDirection(const GravityVector & gravity) const
    {
        GravitySector result{GravitySector::NegativeY};
        if((gravity.x <= gravity.z) && (gravity.x >= -gravity.z) && (gravity.y <= gravity.z)
           && (gravity.y >= -gravity.z))
        {
            result = GravitySector::PositiveZ;
        }
        if((gravity.x >= gravity.z) && (gravity.x <= -gravity.z) && (gravity.y >= gravity.z)
           && (gravity.y <= -gravity.z))
        {
            result = GravitySector::NegativeZ;
        }
        if((gravity.y < gravity.x) && (gravity.y > -gravity.x) && (gravity.z < gravity.x)
           && (gravity.z > -gravity.x))
        {
            result = GravitySector::PositiveX;
        }
        if((gravity.y > gravity.x) && (gravity.y < -gravity.x) && (gravity.z > gravity.x)
           && (gravity.z < -gravity.x))
        {
            result = GravitySector::NegativeX;
        }
        if((gravity.x < gravity.y) && (gravity.x > -gravity.y) && (gravity.z < gravity.y)
           && (gravity.z > -gravity.y))
        {
            result = GravitySector::PositiveY;
        }
        return result;
    }

    CavityHeatFlow Cavity::heatFlowDirection(ScreenFlow screenFlow, const GravityVector & gravity)
    {
        // Map that will convert screen and gravity flow into cavity flow
        std::map<std::pair<GravitySector, ScreenFlow>, CavityHeatFlow> cavityHeatFlow{
          {{GravitySector::PositiveZ, ScreenFlow::Right}, CavityHeatFlow::Horizontal},
          {{GravitySector::PositiveZ, ScreenFlow::Left}, CavityHeatFlow::Horizontal},
          {{GravitySector::PositiveZ, ScreenFlow::Up}, CavityHeatFlow::Horizontal},
          {{GravitySector::PositiveZ, ScreenFlow::Down}, CavityHeatFlow::Horizontal},
          {{GravitySector::NegativeZ, ScreenFlow::Right}, CavityHeatFlow::Horizontal},
          {{GravitySector::NegativeZ, ScreenFlow::Left}, CavityHeatFlow::Horizontal},
          {{GravitySector::NegativeZ, ScreenFlow::Up}, CavityHeatFlow::Horizontal},
          {{GravitySector::NegativeZ, ScreenFlow::Down}, CavityHeatFlow::Horizontal},
          {{GravitySector::PositiveX, ScreenFlow::Right}, CavityHeatFlow::Downward},
          {{GravitySector::PositiveX, ScreenFlow::Left}, CavityHeatFlow ::Upward},
          {{GravitySector::PositiveX, ScreenFlow::Up}, CavityHeatFlow::Horizontal},
          {{GravitySector::PositiveX, ScreenFlow::Down}, CavityHeatFlow::Horizontal},
          {{GravitySector::NegativeX, ScreenFlow::Right}, CavityHeatFlow::Upward},
          {{GravitySector::NegativeX, ScreenFlow::Left}, CavityHeatFlow::Downward},
          {{GravitySector::NegativeX, ScreenFlow::Up}, CavityHeatFlow::Horizontal},
          {{GravitySector::NegativeX, ScreenFlow::Down}, CavityHeatFlow::Horizontal},
          {{GravitySector::PositiveY, ScreenFlow::Right}, CavityHeatFlow::Horizontal},
          {{GravitySector::PositiveY, ScreenFlow::Left}, CavityHeatFlow::Horizontal},
          {{GravitySector::PositiveY, ScreenFlow::Up}, CavityHeatFlow::Downward},
          {{GravitySector::PositiveY, ScreenFlow::Down}, CavityHeatFlow::Upward},
          {{GravitySector::NegativeY, ScreenFlow::Right}, CavityHeatFlow::Horizontal},
          {{GravitySector::NegativeY, ScreenFlow::Left}, CavityHeatFlow::Horizontal},
          {{GravitySector::NegativeY, ScreenFlow::Up}, CavityHeatFlow::Upward},
          {{GravitySector::NegativeY, ScreenFlow::Down}, CavityHeatFlow::Downward}};
        const auto gDirection = gravityDirection(gravity);

        return cavityHeatFlow.at({gDirection, screenFlow});
    }

    CavityFlowDimensions Cavity::cavityFlowDimension() const
    {
        enum class DimensionAlgorithm
        {
            None,
            swapHL,
            storeJambH,
            swapHLstoreJambH
        };

        const auto smallDimension = 0.01;
        auto H = maxYDimension;
        auto L = maxXDimension;
        if(H <= 0)
        {
            H = smallDimension;
        }
        if(L <= 0)
        {
            L = smallDimension;
        }

        // Map that will convert screen and gravity flow into cavity flow as described in ISO 15099
        std::map<std::pair<GravitySector, ScreenFlow>, DimensionAlgorithm> algorithm{
          {{GravitySector::PositiveZ, ScreenFlow::Right}, DimensionAlgorithm::storeJambH},
          {{GravitySector::PositiveZ, ScreenFlow::Left}, DimensionAlgorithm::storeJambH},
          {{GravitySector::PositiveZ, ScreenFlow::Up}, DimensionAlgorithm::swapHLstoreJambH},
          {{GravitySector::PositiveZ, ScreenFlow::Down}, DimensionAlgorithm::swapHLstoreJambH},
          {{GravitySector::NegativeZ, ScreenFlow::Right}, DimensionAlgorithm::storeJambH},
          {{GravitySector::NegativeZ, ScreenFlow::Left}, DimensionAlgorithm::storeJambH},
          {{GravitySector::NegativeZ, ScreenFlow::Up}, DimensionAlgorithm::swapHLstoreJambH},
          {{GravitySector::NegativeZ, ScreenFlow::Down}, DimensionAlgorithm::swapHLstoreJambH},
          {{GravitySector::PositiveX, ScreenFlow::Right}, DimensionAlgorithm::swapHL},
          {{GravitySector::PositiveX, ScreenFlow::Left}, DimensionAlgorithm::swapHL},
          {{GravitySector::PositiveX, ScreenFlow::Up}, DimensionAlgorithm::swapHL},
          {{GravitySector::PositiveX, ScreenFlow::Down}, DimensionAlgorithm::swapHL},
          {{GravitySector::NegativeX, ScreenFlow::Right}, DimensionAlgorithm::swapHL},
          {{GravitySector::NegativeX, ScreenFlow::Left}, DimensionAlgorithm::swapHL},
          {{GravitySector::NegativeX, ScreenFlow::Up}, DimensionAlgorithm::swapHL},
          {{GravitySector::NegativeX, ScreenFlow::Down}, DimensionAlgorithm::swapHL},
          {{GravitySector::PositiveY, ScreenFlow::Right}, DimensionAlgorithm::None},
          {{GravitySector::PositiveY, ScreenFlow::Left}, DimensionAlgorithm::None},
          {{GravitySector::PositiveY, ScreenFlow::Up}, DimensionAlgorithm::None},
          {{GravitySector::PositiveY, ScreenFlow::Down}, DimensionAlgorithm::None},
          {{GravitySector::NegativeY, ScreenFlow::Right}, DimensionAlgorithm::None},
          {{GravitySector::NegativeY, ScreenFlow::Left}, DimensionAlgorithm::None},
          {{GravitySector::NegativeY, ScreenFlow::Up}, DimensionAlgorithm::None},
          {{GravitySector::NegativeY, ScreenFlow::Down}, DimensionAlgorithm::None}};

        auto gapAlgorithm = algorithm.at({gravityDirection(gravity), screenFlow});

        if(gapAlgorithm == DimensionAlgorithm::swapHL)
        {
            std::swap(H, L);
        }
        if(gapAlgorithm == DimensionAlgorithm::storeJambH)
        {
            H = jambHeight;
        }
        if(gapAlgorithm == DimensionAlgorithm::swapHLstoreJambH)
        {
            L = H;
            H = jambHeight;
        }
        return {L, H};
    }

    double Cavity::calcThicknessInHeatFlowDirection(ScreenFlow screenFlow) const
    {
        auto result{maxXDimension};

        if(screenFlow == ScreenFlow::Up || screenFlow == ScreenFlow::Down)
        {
            result = maxYDimension;
        }

        return result;
    }

    double Cavity::effectiveConductivity()
    {
        auto Keff = convKeff();
        if(radiationMethod == RadiationCalculation::Yes)
        {
            Keff += radKeff();
        }
        return Keff;
    }

    //////////////////////////////////////////////////////////////////////////////////////
    /// CavityISO10599
    //////////////////////////////////////////////////////////////////////////////////////

    CavityISO10599::CavityISO10599(ScreenFlow screenFlow,
                                   double maxXDimension,
                                   double maxYDimension,
                                   double jambHeight,
                                   const CavitySide & side1,
                                   const CavitySide & side2,
                                   double pressure,
                                   const GravityVector & gravity,
                                   RadiationCalculation radiationCalculation,
                                   const Gases::CGas & gas,
                                   Ventilated ventilated) :
        Cavity(screenFlow,
               maxXDimension,
               maxYDimension,
               jambHeight,
               side1,
               side2,
               pressure,
               gravity,
               radiationCalculation,
               gas),
        ventilated(ventilated)
    {}

    double CavityISO10599::calcNu()
    {
        const auto flowDimension = cavityFlowDimension();
        std::unique_ptr<INusselt> nuCav{NusseltISO15099Factory::create(cavityHeatFlow,
                                                                       flowDimension.L,
                                                                       flowDimension.H,
                                                                       side1.temperature,
                                                                       side2.temperature,
                                                                       gas)};
        auto nu = nuCav->value();
        if(ventilated == Ventilated::Yes)
        {
            nu *= 2;
        }

        return nu;
    }

    double CavityISO10599::radKeff() const
    {
        const auto e1 = std::max(side1.emissivity, 0.001);
        const auto e2 = std::max(side2.emissivity, 0.001);
        const auto Tavg = (side1.temperature + side2.temperature) * 0.5;
        auto ratio = maxXDimension / maxYDimension;
        if(cavityHeatFlow == CavityHeatFlow::Downward || cavityHeatFlow == CavityHeatFlow::Upward)
        {
            ratio = 1 / ratio;
        }
        const auto hr =
          4 * 5.67e-8 * Tavg * Tavg * Tavg
          / (1.0 / e1 + 1.0 / e2 - 2.0
             + 1.0 / (0.5 * (std::pow(1.0 + std::pow(ratio, 2.0), 0.5) - ratio + 1.0)));
        auto keff = hr * thicknessInHeatFlowDirection;
        if(ventilated == Ventilated::Yes)
        {
            keff *= 2;
        }
        return keff;
    }

    double CavityISO10599::convKeff()
    {
        return calcNu() * gas.getGasProperties().m_ThermalConductivity;
    }
}   // namespace KeffCavity