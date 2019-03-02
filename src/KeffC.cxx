#include "KeffC.hxx"
#include "NusseltCavity.hxx"

namespace KeffCavity
{
    //////////////////////////////////////////////////////////////////////////////////////
    /// CavityData
    //////////////////////////////////////////////////////////////////////////////////////

    CavityData::CavityData(Ventilated ventilated,
                           ScreenFlow screenFlow,
                           double maxXDimension,
                           double maxYDimension,
                           double jambHeight,
                           double area,
                           const CavitySide & side1,
                           const CavitySide & side2,
                           const GravityVector & gravity,
                           const Gases::CGas & gas) :
        ventilated(ventilated),
        screenFlow(screenFlow),
        maxXDimension(maxXDimension),
        maxYDimension(maxYDimension),
        jambHeight(jambHeight),
        area(area),
        side1(side1),
        side2(side2),
        gravity(gravity),
        gas(gas),
        cavityHeatFlow(heatFlowDirection(screenFlow, gravity))
    {}

    CavityData::GravitySector CavityData::gravityDirection(const GravityVector & gravity)
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

    CavityHeatFlow CavityData::heatFlowDirection(ScreenFlow screenFlow,
                                                 const GravityVector & gravity)
    {
        // Map that will convert screen and gravity flow into cavity flow
        std::map<std::pair<GravitySector, ScreenFlow>, CavityHeatFlow> convertFlow{
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

        return convertFlow.at({gDirection, screenFlow});
    }

    CavityFlowDimensions CavityData::cavityFlowDimension()
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

    CavityHeatFlow CavityData::getCavityHeatFlow() const
    {
        return cavityHeatFlow;
    }

    double CavityData::keff(KeffStandard standard)
    {
        double Nu{0};
        switch(standard)
        {
            case KeffStandard::NFRC:
                break;
            case KeffStandard::CENISO:
                break;
            case KeffStandard::USERDIMENSION:
                break;
            case KeffStandard::ISO15099:
                Nu = calcISO15099Nu();
                break;
        }
        return Nu * gas.getGasProperties().m_ThermalConductivity;
    }

    double CavityData::calcISO15099Nu()
    {
        const auto flowDimension = cavityFlowDimension();
        std::unique_ptr<INusselt> nu{NusseltISO15099Factory::create(cavityHeatFlow,
                                                                    flowDimension.L,
                                                                    flowDimension.H,
                                                                    side1.temperature,
                                                                    side2.temperature,
                                                                    gas)};
        return nu->value();
    }

    //////////////////////////////////////////////////////////////////////////////////////
    /// KeffCavity
    //////////////////////////////////////////////////////////////////////////////////////

    KeffCavity::KeffCavity(const CavityData & cavity,
                           KeffStandard standard,
                           RadiationCalculation radiationCalculation,
                           RadiationMethod radiationMethod) :
        cavity(cavity),
        standard(standard),
        radiationCalculation(radiationCalculation),
        radiationMethod(radiationMethod)
    {}

    double KeffCavity::effectiveConductivity()
    {
        return cavity.keff(standard);
    }
}   // namespace KeffCavity