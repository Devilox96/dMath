#include "gtest/gtest.h"

#include <sstream>
#include <functional>
#include "../Matrix.h"
#ifndef DMATH_ONLY_MATRIX
    #include "../Vector.h"
#endif // DMATH_ONLY_MATRIX
namespace tests {
using dmath::Matrix;
using dmath::sMatrix;

class TestMatrix : public ::testing::Test {
protected:
    void SetUp() final {
    }

    void TearDown() final {
    }

    using Type = double;
    using doubleSMatrix2 = sMatrix<Type, 2>;
};

TEST_F(TestMatrix, Constructor) {
    {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D, doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D({{{10., 10.}, {10., 10.}}});
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
        EXPECT_EQ(doubleSMatrix2::zero() + double2D, doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D + 5., doubleSMatrix2(15.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(5. + double2D, doubleSMatrix2(15.));
    }
}

TEST_F(TestMatrix, AdditionAssignment) {
    {
        doubleSMatrix2 double2D(10.);
        double2D += double2D;
        EXPECT_EQ(double2D, doubleSMatrix2(20.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D += 5.;
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
        EXPECT_EQ(doubleSMatrix2::zero() - double2D, doubleSMatrix2(-10.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(double2D - 5., doubleSMatrix2(5.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(5. - double2D, doubleSMatrix2(-5.));
    }
}

TEST_F(TestMatrix, SubtractionAssignment) {
    {
        doubleSMatrix2 double2D(10.);
        doubleSMatrix2 double2D_2(5.);
        double2D -= double2D_2;
        EXPECT_EQ(double2D, doubleSMatrix2(5.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D -= doubleSMatrix2::zero();
        EXPECT_EQ(double2D, doubleSMatrix2(10.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D -= 5.;
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
        EXPECT_EQ(double2D * 5., doubleSMatrix2(50.));
    } {
        doubleSMatrix2 double2D(10.);
        EXPECT_EQ(5. * double2D, doubleSMatrix2(50.));
    }
}

TEST_F(TestMatrix, MultiplicationAssignment) {
    {
        doubleSMatrix2 double2D(10.);
        double2D *= double2D;
        EXPECT_EQ(double2D, doubleSMatrix2(100.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D *= doubleSMatrix2::zero();
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
        doubleSMatrix2 double2D_2(5.);
        double2D /= double2D_2;
        EXPECT_EQ(double2D, doubleSMatrix2(2.));
    } {
        doubleSMatrix2 double2D(10.);
        double2D /= 5;
        EXPECT_EQ(double2D, doubleSMatrix2(2.));
    }
}

TEST_F(TestMatrix, Dot) {
    {
        auto double2D = doubleSMatrix2::identity();
        doubleSMatrix2 double2D_2(10.);
        EXPECT_EQ(dot(double2D, double2D_2), doubleSMatrix2(10.));
    } {
        auto double2D = doubleSMatrix2::identity();
        doubleSMatrix2 double2D_2(10.);
        EXPECT_EQ(double2D.dot(double2D_2), doubleSMatrix2(10.));
    } {
        auto double2D = doubleSMatrix2::identity();
        doubleSMatrix2 double2D_2(10.);
        EXPECT_EQ(double2D_2.dot(double2D), doubleSMatrix2(10.));
    }
}

TEST_F(TestMatrix, Identity) {
    {
        doubleSMatrix2 double2D({{{1., 0.}, {0., 1.}}});
        EXPECT_EQ(double2D, doubleSMatrix2::identity());
    }
}

TEST_F(TestMatrix, Zero) {
    {
        doubleSMatrix2 double2D(0.);
        EXPECT_EQ(double2D, doubleSMatrix2::zero());
    }
}


TEST_F(TestMatrix, Print) {
    std::stringstream out;
    out << dmath::Matrix<int, 2, 2>(10);
    EXPECT_EQ(out.str(), "Matrix <i, 2, 2>({\n10 10 \n10 10 \n})");
}

TEST_F(TestMatrix, Iterators) {
    doubleSMatrix2 double2D(10.);
    const auto& c_double2D = double2D;
    {
        EXPECT_EQ(double2D.begin(), double2D.cbegin());
        EXPECT_EQ(double2D.cbegin(), c_double2D.begin());
        EXPECT_EQ(c_double2D.begin(), c_double2D.cbegin());
    } {
        EXPECT_EQ(double2D.end(), double2D.cend());
        EXPECT_EQ(double2D.cend(), c_double2D.end());
        EXPECT_EQ(c_double2D.end(), c_double2D.cend());
    } {
        EXPECT_EQ(double2D.rbegin(), double2D.crbegin());
        EXPECT_EQ(double2D.crbegin(), c_double2D.rbegin());
        EXPECT_EQ(c_double2D.rbegin(), c_double2D.crbegin());

        EXPECT_EQ(double2D.rbegin().base(), double2D.end());
    } {
        EXPECT_EQ(double2D.rend(), double2D.crend());
        EXPECT_EQ(double2D.crend(), c_double2D.rend());
        EXPECT_EQ(c_double2D.rend(), c_double2D.crend());

        EXPECT_EQ(double2D.rend().base(), double2D.begin());
    }
}

TEST_F(TestMatrix, Iterate) {
    {
        doubleSMatrix2 double2D(10.);
        for (auto item : double2D) {
            EXPECT_EQ(item, 10);
        }
    } {
        doubleSMatrix2 double2D(10.);
        for (auto item = double2D.begin<dmath::Iterate::Val>(); item != double2D.end<dmath::Iterate::Val>(); ++item) {
            EXPECT_EQ(*item, 10.);
        }
    }

}

TEST_F(TestMatrix, IterateRow) {
    doubleSMatrix2 double2D(10.);
    {
        for (auto iter = double2D.begin<dmath::Iterate::Row>(); iter != double2D.end<dmath::Iterate::Row>(); ++iter) {
            for (auto item : *iter) {
                EXPECT_EQ(item, 10.);
            }
        }
    }
}

TEST_F(TestMatrix, IterateCol) {
    doubleSMatrix2 double2D(10.);
    {
        for (auto iter = double2D.begin<dmath::Iterate::Col>(); iter != double2D.end<dmath::Iterate::Col>(); ++iter) {
            for (auto item : *iter) {
                EXPECT_EQ(item, 10.);
            }
        }
    }
}

#ifndef DMATH_ONLY_MATRIX
TEST_F(TestMatrix, RowToVector) {
    doubleSMatrix2 double2D(10.);
    for (auto iter = double2D.begin<dmath::Iterate::Row>(); iter != double2D.end<dmath::Iterate::Row>(); ++iter) {
        EXPECT_EQ((static_cast<dmath::Vector<double, 2>>(*iter)), (dmath::Vector<double, 2>(10)));
    }
}

TEST_F(TestMatrix, ColToVector) {
    doubleSMatrix2 double2D(10.);
    for (auto iter = double2D.begin<dmath::Iterate::Col>(); iter != double2D.end<dmath::Iterate::Col>(); ++iter) {
        EXPECT_EQ((static_cast<dmath::Vector<double, 2>>(*iter)), (dmath::Vector<double, 2>(10)));
    }
}
} // namespace tests
#endif // DMATH_ONLY_MATRIX
