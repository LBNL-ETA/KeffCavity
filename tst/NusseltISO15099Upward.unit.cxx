#include <gtest/gtest.h>
#include <memory>

#include "KeffCavity.hxx"

class TestNusseltISO15099Upward : public testing::Test
{
protected:
    void SetUp() override
    {}
};

TEST_F(TestNusseltISO15099Upward, TestISO15099UpwardRatio1)
{
    Gases::CGas gas;
    const double L = 1.0;
    const double H = 1.0;
    const double T1 = 293.15;
    const double T2 = 293.15;
    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Upward>(
      new KeffCavity::NusseltISO15099Upward(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 1.0, 1e-6);
}

TEST_F(TestNusseltISO15099Upward, TestISO15099UpwardRatioBetween1and5)
{
    Gases::CGas gas;
    const double L = 0.3;
    const double H = 0.1;
    const double T1 = 271.0;
    const double T2 = 274.0;
    const double pressure = 101325;
    gas.setTemperatureAndPressure((T1 + T2) * 0.5, pressure);
    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Upward>(
            new KeffCavity::NusseltISO15099Upward(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 14.774822, 1e-6);
}

TEST_F(TestNusseltISO15099Upward, TestISO15099UpwardRatioGreaterThan5)
{
    Gases::CGas gas;
    const double L = 0.6;
    const double H = 0.1;
    const double T1 = 271.0;
    const double T2 = 274.0;
    const double pressure = 101325;
    gas.setTemperatureAndPressure((T1 + T2) * 0.5, pressure);
    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Upward>(
            new KeffCavity::NusseltISO15099Upward(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 26.831612, 1e-6);
}