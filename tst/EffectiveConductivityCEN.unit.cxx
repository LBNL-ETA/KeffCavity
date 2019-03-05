#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>
#include <memory>

#include "KeffCavity.hxx"

class TestEffectiveConductivityCEN : public testing::Test
{
protected:
    void SetUp() override
    {}
};

TEST_F(TestEffectiveConductivityCEN, TestHorizontalFlow)
{
    Gases::CGas gas;
    const double MaxXDimension = 0.0756;
    const double MaxYDimension = 0.0675;
    const double area = 0.005103;
    const double JambHeight = 1;
    const double T1 = 274.7788;
    const double e1 = 0.9;
    const double T2 = 279.068;
    const double e2 = 0.9;
    const KeffCavity::RadiationCalculation radCal = KeffCavity::RadiationCalculation::Yes;
    const GravityVector g{0, -1, 0};
    const double pressure = 101325;
    KeffCavity::CavityCEN cavity(KeffCavity::ScreenFlow::Right,
                                 MaxXDimension,
                                 MaxYDimension,
                                 area,
                                 JambHeight,
                                 {T1, e1},
                                 {T2, e2},
                                 pressure,
                                 g,
                                 radCal,
                                 gas);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 0.307709, 1e-6);
}

TEST_F(TestEffectiveConductivityCEN, TestUpwardFlow)
{
    Gases::CGas gas;
    const double MaxXDimension = 0.2246;
    const double MaxYDimension = 0.0734;
    const double area = 0.0164856389;
    const double JambHeight = 1;
    const double T1 = 271.7405;
    const double e1 = 0.9;
    const double T2 = 274.8692;
    const double e2 = 0.9;
    const KeffCavity::RadiationCalculation radCal = KeffCavity::RadiationCalculation::Yes;
    const GravityVector g{0, -1, 0};
    const double pressure = 101325;
    KeffCavity::CavityCEN cavity(KeffCavity::ScreenFlow::Up,
                                 MaxXDimension,
                                 MaxYDimension,
                                 area,
                                 JambHeight,
                                 {T1, e1},
                                 {T2, e2},
                                 pressure,
                                 g,
                                 radCal,
                                 gas);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 0.324369, 1e-6);
}