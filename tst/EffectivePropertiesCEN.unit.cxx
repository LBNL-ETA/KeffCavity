

#include <gtest/gtest.h>
#include <memory>

#include "KeffCavity.hxx"

class TestEffectivePropertiesCEN : public testing::Test
{
protected:
    void SetUp() override
    {}
};

TEST_F(TestEffectivePropertiesCEN, TestHorizontalFlow)
{
    Gases::CGas gas;
    const double MaxXDimension = 0.0756;
    const double MaxYDimension = 0.0675;
    const double area = 0.005103;
    const double T1 = 274.7788;
    const double e1 = 0.9;
    const double T2 = 279.068;
    const double e2 = 0.9;
    const KeffCavity::RadiationCalculation radCal = KeffCavity::RadiationCalculation::Simplified;
    KeffCavity::CavityCEN cavity(KeffCavity::ScreenFlow::Right,
                                 MaxXDimension,
                                 MaxYDimension,
                                 area,
                                 {T1, e1},
                                 {T2, e2},
                                 Gases::CGas(),
                                 radCal);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 0.307709, 1e-6);

    EXPECT_NEAR(cavity.nusselt(), 3.725335, 1e-6);

    const double diffusionResistanceFactor{cavity.effectiveDiffusionResistanceFactor()};

    EXPECT_NEAR(diffusionResistanceFactor, 0.097322, 1e-6);
}

TEST_F(TestEffectivePropertiesCEN, TestUpwardFlow)
{
    Gases::CGas gas;
    const double MaxXDimension = 0.2246;
    const double MaxYDimension = 0.0734;
    const double area = 0.0164856389;
    const double T1 = 271.7405;
    const double e1 = 0.9;
    const double T2 = 274.8692;
    const double e2 = 0.9;
    const KeffCavity::RadiationCalculation radCal = KeffCavity::RadiationCalculation::Simplified;
    KeffCavity::CavityCEN cavity(KeffCavity::ScreenFlow::Up,
                                 MaxXDimension,
                                 MaxYDimension,
                                 area,
                                 {T1, e1},
                                 {T2, e2},
                                 Gases::CGas(),
                                 radCal);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 0.324369, 1e-6);

    EXPECT_NEAR(cavity.nusselt(), 3.255883, 1e-6);

    const double diffusionResistanceFactor{cavity.effectiveDiffusionResistanceFactor()};

    EXPECT_NEAR(diffusionResistanceFactor, 0.127410, 1e-6);
}