

#include <gtest/gtest.h>
#include <memory>

#include "KeffCavity.hxx"

class TestEffectivePropertiesISO15099 : public testing::Test
{
protected:
    void SetUp() override
    {}
};

TEST_F(TestEffectivePropertiesISO15099, TestHorizontalFlowGravityNegativeYStandardRatio)
{
    Gases::CGas gas;
    const double MaxXDimension = 0.0756;
    const double MaxYDimension = 0.0675;
    const double JambHeight = 1;
    const double T1 = 275.1543;
    const double e1 = 0.9;
    const double T2 = 278.7715;
    const double e2 = 0.9;
    const KeffCavity::RadiationCalculation radCal = KeffCavity::RadiationCalculation::Simplified;
    const FenestrationCommon::GravityVector g{0, -1, 0};
    const double pressure = 101325;
    KeffCavity::CavityISO10599 cavity(KeffCavity::ScreenFlow::Right,
                                      MaxXDimension,
                                      MaxYDimension,
                                      JambHeight,
                                      {T1, e1},
                                      {T2, e2},
                                      pressure,
                                      g,
                                      radCal,
                                      gas);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 0.377186, 1e-6);

    EXPECT_NEAR(cavity.nusselt(), 6.527696, 1e-6);

    const double diffusionResistanceFactor{cavity.effectiveDiffusionResistanceFactor()};

    EXPECT_NEAR(diffusionResistanceFactor,  0.030883, 1e-6);
}

TEST_F(TestEffectivePropertiesISO15099, TestHorizontalFlowGravityIntoScreenStandardRatio)
{
    Gases::CGas gas;
    const double MaxXDimension = 0.0756;
    const double MaxYDimension = 0.0675;
    const double JambHeight = 1;
    const double T1 = 274.8213;
    const double e1 = 0.9;
    const double T2 = 279.0344;
    const double e2 = 0.9;
    const KeffCavity::RadiationCalculation radCal = KeffCavity::RadiationCalculation::Simplified;
    const FenestrationCommon::GravityVector g{0, 0, -1};
    const double pressure = 101325;
    KeffCavity::CavityISO10599 cavity(KeffCavity::ScreenFlow::Right,
                                      MaxXDimension,
                                      MaxYDimension,
                                      JambHeight,
                                      {T1, e1},
                                      {T2, e2},
                                      pressure,
                                      g,
                                      radCal,
                                      gas);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 0.314463, 1e-6);

    EXPECT_NEAR(cavity.nusselt(), 3.957320, 1e-6);

    const double diffusionResistanceFactor{cavity.effectiveDiffusionResistanceFactor()};

    EXPECT_NEAR(diffusionResistanceFactor, 0.084050, 1e-6);
}

TEST_F(TestEffectivePropertiesISO15099, TestHorizontalFlowGravityIntoScreenStandardRatioVentilated)
{
    Gases::CGas gas;
    const double MaxXDimension = 0.0756;
    const double MaxYDimension = 0.0675;
    const double JambHeight = 1;
    const double T1 = 274.8213;
    const double e1 = 0.9;
    const double T2 = 279.0344;
    const double e2 = 0.9;
    const KeffCavity::RadiationCalculation radCal = KeffCavity::RadiationCalculation::Simplified;
    const FenestrationCommon::GravityVector g{0, 0, -1};
    const double pressure = 101325;
    const KeffCavity::Ventilated ventilated = KeffCavity::Ventilated::Yes;
    KeffCavity::CavityISO10599 cavity(KeffCavity::ScreenFlow::Right,
                                      MaxXDimension,
                                      MaxYDimension,
                                      JambHeight,
                                      {T1, e1},
                                      {T2, e2},
                                      pressure,
                                      g,
                                      radCal,
                                      gas,
                                      ventilated);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 0.628926, 1e-6);

    EXPECT_NEAR(cavity.nusselt(), 3.957320, 1e-6);

    const double diffusionResistanceFactor{cavity.effectiveDiffusionResistanceFactor()};

    EXPECT_NEAR(diffusionResistanceFactor, 0.084050, 1e-6);
}

TEST_F(TestEffectivePropertiesISO15099, TestHorizontalFlowGravityIntoScreenRatioLessThanHalf)
{
    Gases::CGas gas;
    const double MaxXDimension = 0.2756;
    const double MaxYDimension = 0.0703;
    const double JambHeight = 1;
    const double T1 = 271.4768;
    const double e1 = 0.9;
    const double T2 = 275.7409;
    const double e2 = 0.9;
    const KeffCavity::RadiationCalculation radCal = KeffCavity::RadiationCalculation::Simplified;
    const FenestrationCommon::GravityVector g{0, 0, -1};
    const double pressure = 101325;
    KeffCavity::CavityISO10599 cavity(KeffCavity::ScreenFlow::Right,
                                      MaxXDimension,
                                      MaxYDimension,
                                      JambHeight,
                                      {T1, e1},
                                      {T2, e2},
                                      pressure,
                                      g,
                                      radCal,
                                      gas);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 1.039951, 1e-6);

    EXPECT_NEAR(cavity.nusselt(), 16.574408, 1e-6);

    const double diffusionResistanceFactor{cavity.effectiveDiffusionResistanceFactor()};

    EXPECT_NEAR(diffusionResistanceFactor, 0.004901, 1e-6);
}

TEST_F(TestEffectivePropertiesISO15099, TestHorizontalFlowGravityIntoScreenRatioMoreThanFive)
{
    Gases::CGas gas;
    const double MaxXDimension = 0.0161;
    const double MaxYDimension = 0.1932;
    const double JambHeight = 1;
    const double T1 = 272.5621;
    const double e1 = 0.9;
    const double T2 = 276.1058;
    const double e2 = 0.9;
    const KeffCavity::RadiationCalculation radCal = KeffCavity::RadiationCalculation::Simplified;
    const FenestrationCommon::GravityVector g{0, 0, -1};
    const double pressure = 101325;
    KeffCavity::CavityISO10599 cavity(KeffCavity::ScreenFlow::Right,
                                      MaxXDimension,
                                      MaxYDimension,
                                      JambHeight,
                                      {T1, e1},
                                      {T2, e2},
                                      pressure,
                                      g,
                                      radCal,
                                      gas);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 0.083861, 1e-6);

    EXPECT_NEAR(cavity.nusselt(), 1.001951, 1e-6);

    const double diffusionResistanceFactor{cavity.effectiveDiffusionResistanceFactor()};

    EXPECT_NEAR(diffusionResistanceFactor, 1.334508, 1e-6);
}


