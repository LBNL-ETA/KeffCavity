#include "KeffCavity.hxx"

class CavityTest : public KeffCavity::Cavity
{
public:
    CavityTest(KeffCavity::ScreenFlow screenFlow,
               double maxXDimension,
               double maxYDimension,
               double jambHeight,
               double area,
               const KeffCavity::CavitySide & side1,
               const KeffCavity::CavitySide & side2,
               const GravityVector & gravity,
               KeffCavity::RadiationCalculation radiationMethod = KeffCavity::RadiationCalculation::Yes,
               const Gases::CGas & gas = Gases::CGas()) :
            Cavity(screenFlow,
                   maxXDimension,
                   maxYDimension,
                   jambHeight,
                   area,
                   side1,
                   side2,
                   gravity,
                   radiationMethod,
                   gas)
    {}

protected:
    double radKeff() const override {
        return 0;
    }

    double calcNu() override {
        return 0;
    }
};