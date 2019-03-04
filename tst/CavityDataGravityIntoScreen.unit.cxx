#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>

#include "KeffCavity.hxx"

using KeffCavity::KeffStandard;
using KeffCavity::CavityData;
using KeffCavity::Ventilated;
using KeffCavity::ScreenFlow;
using KeffCavity::CavitySide;

class TestCavityDataGravityIntoScreen : public testing::Test
{
protected:
    const KeffStandard standard{KeffStandard::ISO15099};
    const double maxXDimension{1.2};
    const double maxYDimension{0.8};
    const double jambHeight{1.5};
    const double area{1.0};
    GravityVector gravity{0.0, 0.0, -1.0};
    const CavitySide side1{10, 0.9};
    const CavitySide side2{20, 0.9};

    void SetUp() override
    {}
};

TEST_F(TestCavityDataGravityIntoScreen, TestCavityFlowLeft)
{
    ScreenFlow flow{ScreenFlow::Left};

    CavityData cavity(
      standard, flow, maxXDimension, maxYDimension, jambHeight, area, side1, side2, gravity);

    auto cavityFlow = cavity.cavityFlowDimension();
    EXPECT_EQ(cavityFlow.L, maxXDimension);
    EXPECT_EQ(cavityFlow.H, jambHeight);
}

TEST_F(TestCavityDataGravityIntoScreen, TestCavityFlowRight)
{
    ScreenFlow flow{ScreenFlow::Right};

    CavityData cavity(
      standard, flow, maxXDimension, maxYDimension, jambHeight, area, side1, side2, gravity);

    auto cavityFlow = cavity.cavityFlowDimension();
    EXPECT_EQ(cavityFlow.L, maxXDimension);
    EXPECT_EQ(cavityFlow.H, jambHeight);
}

TEST_F(TestCavityDataGravityIntoScreen, TestCavityFlowUpward)
{
    ScreenFlow flow{ScreenFlow::Up};

    CavityData cavity(
      standard, flow, maxXDimension, maxYDimension, jambHeight, area, side1, side2, gravity);

    auto cavityFlow = cavity.cavityFlowDimension();
    EXPECT_EQ(cavityFlow.L, maxYDimension);
    EXPECT_EQ(cavityFlow.H, jambHeight);
}

TEST_F(TestCavityDataGravityIntoScreen, TestCavityFlowDownward)
{
    ScreenFlow flow{ScreenFlow::Down};

    CavityData cavity(
      standard, flow, maxXDimension, maxYDimension, jambHeight, area, side1, side2, gravity);

    auto cavityFlow = cavity.cavityFlowDimension();
    EXPECT_EQ(cavityFlow.L, maxYDimension);
    EXPECT_EQ(cavityFlow.H, jambHeight);
}
#pragma clang diagnostic pop