

#include <gtest/gtest.h>
#include <memory>

#include "KeffCavity.hxx"

class TestNusseltISO15099Horizontal : public testing::Test
{
protected:
    void SetUp() override
    {}
};

TEST_F(TestNusseltISO15099Horizontal, TestISO15099HorizontalRatioLessThanHalf)
{
    Gases::CGas gas;
    const double L = 3.0;
    const double H = 1.0;
    const double T1 = 271.0;
    const double T2 = 274.0;
    const double pressure = 101325;
    gas.setTemperatureAndPressure((T1 + T2) * 0.5, pressure);
    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Horizontal>(
            new KeffCavity::NusseltISO15099Horizontal(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 100.927666, 1e-6);
}

TEST_F(TestNusseltISO15099Horizontal, TestISO15099HorizontalRatioGreaterThanFive)
{
    Gases::CGas gas;
    const double L = 1.0;
    const double H = 6.0;
    const double T1 = 271.0;
    const double T2 = 274.0;
    const double pressure = 101325;
    gas.setTemperatureAndPressure((T1 + T2) * 0.5, pressure);
    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Horizontal>(
            new KeffCavity::NusseltISO15099Horizontal(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 46.080553, 1e-6);
}

TEST_F(TestNusseltISO15099Horizontal, TestISO15099HorizontalRatioInLinearInterpolation)
{
    Gases::CGas gas;
    const double L = 1.0;
    const double H = 2.0;
    const double T1 = 271.0;
    const double T2 = 274.0;
    const double pressure = 101325;
    gas.setTemperatureAndPressure((T1 + T2) * 0.5, pressure);
    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Horizontal>(
            new KeffCavity::NusseltISO15099Horizontal(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 45.881166, 1e-6);
}

TEST_F(TestNusseltISO15099Horizontal, TestISO15099HorizontalRatioLessThanHalf_ThermExample)
{
    Gases::CGas gas;
    const double L = 0.2756;
    const double H = 0.0703;
    const double T1 = 271.665;
    const double T2 = 275.531;
    const double pressure = 101325;
    gas.setTemperatureAndPressure((T1 + T2) * 0.5, pressure);
    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Horizontal>(
            new KeffCavity::NusseltISO15099Horizontal(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 21.772090, 1e-6);
}

TEST_F(TestNusseltISO15099Horizontal, TestISO15099HorizontalRatioGreaterThanFive_ThermExample)
{
    Gases::CGas gas;
    const double L = 0.0161;
    const double H = 0.1932;
    const double T1 = 272.562;
    const double T2 = 276.106;
    const double pressure = 101325;
    gas.setTemperatureAndPressure((T1 + T2) * 0.5, pressure);
    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Horizontal>(
            new KeffCavity::NusseltISO15099Horizontal(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 1.001952, 1e-6);
}

TEST_F(TestNusseltISO15099Horizontal, TestISO15099HorizontalRatioInLinearInterpolation_Therm)
{
    Gases::CGas gas;
    const double L = 0.0756;
    const double H = 0.0675;
    const double T1 = 275.154;
    const double T2 = 278.772;
    const double pressure = 101325;
    gas.setTemperatureAndPressure((T1 + T2) * 0.5, pressure);
    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Horizontal>(
            new KeffCavity::NusseltISO15099Horizontal(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 5.611770, 1e-6);
}

