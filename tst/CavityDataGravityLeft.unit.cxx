

#include <gtest/gtest.h>

#include "KeffCavity.hxx"
#include "Mocks.hxx"

using KeffCavity::Ventilated;
using KeffCavity::ScreenFlow;
using KeffCavity::CavitySide;

class TestCavityDataGravityLeft : public testing::Test
{
protected:
    const double maxXDimension{1.2};
    const double maxYDimension{0.8};
    const double jambHeight{1.5};
    GravityVector gravity{1.0, 0.0, 0.0};
    const CavitySide side1{10, 0.9};
    const CavitySide side2{20, 0.9};

    void SetUp() override
    {}
};

TEST_F(TestCavityDataGravityLeft, TestCavityFlowLeft)
{
    ScreenFlow flow{ScreenFlow::Left};

    CavityTest cavity(flow, maxXDimension, maxYDimension, jambHeight, side1, side2, gravity);

    auto cavityFlow = cavity.cavityFlowDimension();
    EXPECT_EQ(cavityFlow.L, maxYDimension);
    EXPECT_EQ(cavityFlow.H, maxXDimension);
}

TEST_F(TestCavityDataGravityLeft, TestCavityFlowRight)
{
    ScreenFlow flow{ScreenFlow::Right};

    CavityTest cavity(flow, maxXDimension, maxYDimension, jambHeight, side1, side2, gravity);

    auto cavityFlow = cavity.cavityFlowDimension();
    EXPECT_EQ(cavityFlow.L, maxYDimension);
    EXPECT_EQ(cavityFlow.H, maxXDimension);
}

TEST_F(TestCavityDataGravityLeft, TestCavityFlowUpward)
{
    ScreenFlow flow{ScreenFlow::Up};

    CavityTest cavity(flow, maxXDimension, maxYDimension, jambHeight, side1, side2, gravity);

    auto cavityFlow = cavity.cavityFlowDimension();
    EXPECT_EQ(cavityFlow.L, maxYDimension);
    EXPECT_EQ(cavityFlow.H, maxXDimension);
}

TEST_F(TestCavityDataGravityLeft, TestCavityFlowDownward)
{
    ScreenFlow flow{ScreenFlow::Down};

    CavityTest cavity(flow, maxXDimension, maxYDimension, jambHeight, side1, side2, gravity);

    auto cavityFlow = cavity.cavityFlowDimension();
    EXPECT_EQ(cavityFlow.L, maxYDimension);
    EXPECT_EQ(cavityFlow.H, maxXDimension);
}

