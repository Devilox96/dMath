#include <sstream>

#include "gtest/gtest.h"
#include "../dMatrix.h"

class TestMatrix : public ::testing::Test  {
protected:
    void SetUp() final {
    }
    void TearDown() final {

    }

    typedef dSMatrix<double, 2> doubleSMatrix2;

};

TEST_F(TestMatrix, Constructor) {
    {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D, doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D({10., 10., 10., 10.});
        EXPECT_EQ(double2D, doubleSMatrix2(10.));
    }
}

TEST_F(TestMatrix, UnaryPlus) {
    {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(+double2D, doubleSMatrix2(10.));
    } {
        EXPECT_EQ(+doubleSMatrix2::zero(), doubleSMatrix2::zero());
    }
}

TEST_F(TestMatrix, Addition) {
    {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D + double2D, doubleSMatrix2(20.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D + doubleSMatrix2::zero(), doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(doubleSMatrix2::zero() + double2D , doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D + 5, doubleSMatrix2(15.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(5 + double2D, doubleSMatrix2(15.));
    }
}

TEST_F(TestMatrix, AdditionAssignment) {
    {
        doubleSMatrix2 double2D(10.);
        double2D += double2D;
        EXPECT_EQ(double2D, doubleSMatrix2(20.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D += 5;
        EXPECT_EQ(double2D, doubleSMatrix2(15.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D += doubleSMatrix2::zero();
        EXPECT_EQ(double2D, doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D(10.);
    }
}

TEST_F(TestMatrix, UnaryMinus) {
    {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(-double2D, doubleSMatrix2(-10.));
    } {
        EXPECT_EQ(-doubleSMatrix2::zero(), doubleSMatrix2::zero());
    }
}


TEST_F(TestMatrix, Subtraction) {
    {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D - double2D, doubleSMatrix2::zero());
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D - doubleSMatrix2::zero(), doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(doubleSMatrix2::zero() - double2D , doubleSMatrix2(-10.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D - 5, doubleSMatrix2(5.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(5 - double2D, doubleSMatrix2(-5.));
    }
}

TEST_F(TestMatrix, SubtractionAssignment) {
    {
        doubleSMatrix2 double2D(10.);
        double2D -= double2D;
        EXPECT_EQ(double2D, doubleSMatrix2::zero());
    } {
        doubleSMatrix2 double2D(10.);
        double2D -= doubleSMatrix2::zero();
        EXPECT_EQ(double2D, doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D -= 5;
        EXPECT_EQ(double2D, doubleSMatrix2(5.));
    }
}

TEST_F(TestMatrix, Multiplication) {
    {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D * double2D, doubleSMatrix2(100.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D * doubleSMatrix2::zero(), doubleSMatrix2(0.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(doubleSMatrix2::zero() * double2D, doubleSMatrix2(0.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D * 5, doubleSMatrix2(50.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(5 * double2D, doubleSMatrix2(50.));
    }
}

TEST_F(TestMatrix, MultiplicationAssignment) {
    {
        doubleSMatrix2 double2D(10.);
        double2D *= double2D;
        EXPECT_EQ(double2D, doubleSMatrix2(100.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D *= doubleSMatrix2 ::zero();
        EXPECT_EQ(double2D, doubleSMatrix2(0.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D *= 5;
        EXPECT_EQ(double2D, doubleSMatrix2(50.));
    }
}

TEST_F(TestMatrix, Division) {
    {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D / double2D, doubleSMatrix2(1.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(doubleSMatrix2::zero() / double2D, doubleSMatrix2(0.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D / 5, doubleSMatrix2(2.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(5 / double2D, doubleSMatrix2(0.5));
    }
}


TEST_F(TestMatrix, DivisionAssignment) {
    {
        doubleSMatrix2 double2D(10.);
        double2D /= double2D;
        EXPECT_EQ(double2D, doubleSMatrix2(1.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D /= 5;
        EXPECT_EQ(double2D, doubleSMatrix2(2.));
    }
}

TEST_F(TestMatrix, Dot) {
    {
        doubleSMatrix2 double2D({1, 0, 0, 1});
        doubleSMatrix2 double2D_2(10.);
        EXPECT_EQ(dot(double2D, double2D_2), doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D({1, 0, 0, 1});
        doubleSMatrix2 double2D_2(10.);
        EXPECT_EQ(double2D.dot(double2D_2), doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D({1, 0, 0, 1});
        doubleSMatrix2 double2D_2(10.);
        EXPECT_EQ(double2D_2.dot(double2D), doubleSMatrix2(10.));
    }
}

TEST_F(TestMatrix, Identity) {
    {
        doubleSMatrix2 double2D({1, 0, 0, 1});
        EXPECT_EQ(double2D, doubleSMatrix2::identity());
    }
}

TEST_F(TestMatrix, Print) {
    std::stringstream out;
    out << dMatrix<int, 2, 2>(10.);
    EXPECT_EQ(out.str(), "dMatrix <i, 2, 2>({\n10 10 \n10 10 \n})");
}

TEST_F(TestMatrix, IterateRow) {
    doubleSMatrix2 double2D(10.);
//    EXPECT_EQ(out.str(), "dMatrix <i, 2, 2>({\n10 10 \n10 10 \n})");
}

