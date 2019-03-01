#include <gtest/gtest.h>
#include <memory>

#include "KeffCavity.hxx"

class TestNusseltISO15099Downward : public testing::Test
{
protected:
    void SetUp() override
    {}
};

TEST_F(TestNusseltISO15099Downward, TestISO15099Downward)
{
    Gases::CGas gas;
    const double L = 1.0;
    const double H = 1.0;
    const double T1 = 293.15;
    const double T2 = 293.15;

    std::unique_ptr<KeffCavity::INusselt> nu = std::unique_ptr<KeffCavity::NusseltISO15099Downward>(
      new KeffCavity::NusseltISO15099Downward(L, H, T1, T2, gas));

    const double Nu = nu->value();

    EXPECT_NEAR(Nu, 1.0, 1e-6);
}