#include <sstream>

#include "gtest/gtest.h"
#include "../dVector.h"

class TestVector : public ::testing::Test  {
protected:
    void SetUp() final {
    }
    void TearDown() final {

    }

    typedef dVector<double, 2> doubleVector2D;

};
TEST_F(TestVector, Constructor) {
    {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D, doubleVector2D(10.));
    } {
        doubleVector2D double2D({10., 10.});
        EXPECT_EQ(double2D, doubleVector2D(10.));
    }
}

TEST_F(TestVector, UnaryPlus) {
    {
        doubleVector2D double2D(10.);
        EXPECT_EQ(+double2D, doubleVector2D({10., 10.}));
    } {
        EXPECT_EQ(+doubleVector2D::zero(), doubleVector2D::zero());
    }
}

TEST_F(TestVector, Addition) {
    {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D + double2D, doubleVector2D({20., 20.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D + doubleVector2D::zero(), doubleVector2D({10., 10.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(doubleVector2D::zero() + double2D , doubleVector2D({10., 10.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D + 5, doubleVector2D({15., 15.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(5 + double2D, doubleVector2D({15., 15.}));
    }
}

TEST_F(TestVector, AdditionAssignment) {
    {
        doubleVector2D double2D(10.);
        double2D += double2D;
        EXPECT_EQ(double2D, doubleVector2D({20., 20.}));
    } {
        doubleVector2D double2D(10.);
        double2D += 5;
        EXPECT_EQ(double2D, doubleVector2D({15., 15.}));
    } {
        doubleVector2D double2D(10.);
        double2D += doubleVector2D::zero();
        EXPECT_EQ(double2D, doubleVector2D({10., 10.}));
    } {
        doubleVector2D double2D(10.);
    }
}

TEST_F(TestVector, UnaryMinus) {
    {
        doubleVector2D double2D(10.);
        EXPECT_EQ(-double2D, doubleVector2D({-10., -10.}));
    } {
        EXPECT_EQ(-doubleVector2D::zero(), doubleVector2D::zero());
    }
}


TEST_F(TestVector, Subtraction) {
    {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D - double2D, doubleVector2D::zero());
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D - doubleVector2D::zero(), doubleVector2D({10., 10.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(doubleVector2D::zero() - double2D , doubleVector2D({-10., -10.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D - 5, doubleVector2D({5., 5.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(5 - double2D, doubleVector2D({-5., -5.}));
    }
}

TEST_F(TestVector, SubtractionAssignment) {
    {
        doubleVector2D double2D(10.);
        double2D -= double2D;
        EXPECT_EQ(double2D, doubleVector2D::zero());
    } {
        doubleVector2D double2D(10.);
        double2D -= doubleVector2D::zero();
        EXPECT_EQ(double2D, doubleVector2D({10., 10.}));
    } {
        doubleVector2D double2D(10.);
        double2D -= 5;
        EXPECT_EQ(double2D, doubleVector2D({5., 5.}));
    }
}

TEST_F(TestVector, Multiplication) {
    {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D * double2D, doubleVector2D({100., 100.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D * doubleVector2D::zero(), doubleVector2D({0., 0.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(doubleVector2D::zero() * double2D, doubleVector2D({0., 0.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D * 5, doubleVector2D({50., 50.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(5 * double2D, doubleVector2D({50., 50.}));
    }
}

TEST_F(TestVector, MultiplicationAssignment) {
    {
        doubleVector2D double2D(10.);
        double2D *= double2D;
        EXPECT_EQ(double2D, doubleVector2D({100., 100.}));
    } {
        doubleVector2D double2D(10.);
        double2D *= doubleVector2D ::zero();
        EXPECT_EQ(double2D, doubleVector2D({0., 0.}));
    } {
        doubleVector2D double2D(10.);
        double2D *= 5;
        EXPECT_EQ(double2D, doubleVector2D({50., 50.}));
    }
}

TEST_F(TestVector, Division) {
    {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D / double2D, doubleVector2D({1., 1.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(doubleVector2D::zero() / double2D, doubleVector2D({0., 0.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(double2D / 5, doubleVector2D({2., 2.}));
    } {
        doubleVector2D double2D(10.);
        EXPECT_EQ(5 / double2D, doubleVector2D({0.5, 0.5}));
    }
}


TEST_F(TestVector, DivisionAssignment) {
    {
        doubleVector2D double2D(10.);
        double2D /= double2D;
        EXPECT_EQ(double2D, doubleVector2D({1., 1.}));
    } {
        doubleVector2D double2D(10.);
        double2D /= 5;
        EXPECT_EQ(double2D, doubleVector2D({2., 2.}));
    }
}

TEST_F(TestVector, Print) {
    std::stringstream out;
    out << dVector<int, 2>(10.);

    EXPECT_EQ(out.str(), ("dVector<i, 2>({10, 10})"));
}
