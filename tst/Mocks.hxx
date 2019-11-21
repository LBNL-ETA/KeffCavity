#include "KeffCavity.hxx"

class CavityTest : public KeffCavity::CavityISO10599
{
public:
    CavityTest(const KeffCavity::ScreenFlow screenFlow,
               const double maxXDimension,
               const double maxYDimension,
               const double jambHeight,
               const KeffCavity::CavitySide & side1,
               const KeffCavity::CavitySide & side2,
               const FenestrationCommon::GravityVector & gravity = {0, -1, 0}) :
        CavityISO10599(screenFlow, maxXDimension, maxYDimension, jambHeight, side1, side2, 101325, gravity)
    {}

protected:
    double radKeff() const override
    {
        return 0;
    }

    double convKeff() override
    {
        return 0;
    }
};