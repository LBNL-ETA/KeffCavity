#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>
#include <memory>

#include "KeffCavity.hxx"

class TestKeffCavity : public testing::Test
{
protected:
    void SetUp() override
    {}
};

TEST_F(TestKeffCavity, TestHorizontal)
{
    Gases::CGas gas;
    const KeffCavity::KeffStandard standard{KeffCavity::KeffStandard::ISO15099};
    const double MaxXDimension = 0.2246;
    const double MaxYDimension = 0.0756;
    const double JambHeight = 1;
    const double area = 2;
    const double T1 = 271.0;
    const double e1 = 0.9;
    const double T2 = 274.0;
    const double e2 = 0.9;
    const GravityVector g{0, -1, 0};
    const double pressure = 101325;
    gas.setTemperatureAndPressure((T1 + T2) * 0.5, pressure);
    KeffCavity::Cavity cavity(standard,
                              KeffCavity::ScreenFlow::Right,
                              MaxXDimension,
                              MaxYDimension,
                              JambHeight,
                              area,
                              {T1, e1},
                              {T2, e2},
                              g);

    const double keff = cavity.effectiveConductivity();

    EXPECT_NEAR(keff, 0.961007, 1e-6);
}
#pragma clang diagnostic pop