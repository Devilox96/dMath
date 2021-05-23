#include <sstream>

#include "gtest/gtest.h"
#include "../Vector.h"
namespace tests {
using dmath::Vector;

class TestVector : public ::testing::Test  {
protected:

    void SetUp() final {
    }
    void TearDown() final {

    }
    using Type = double;
    using doubleVector2D = Vector<Type, 2>;

};

TEST_F(TestVector, Constructor) {
    {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec, doubleVector2D(10.));
    } {
        doubleVector2D vec({10., 10.});
        EXPECT_EQ(vec, doubleVector2D(10.));
    }
}

TEST_F(TestVector, Types) {
    {
        EXPECT_EQ(dmath::Vector2D<Type>(1.), (Vector<Type, 2>(1.)));
    } {
        EXPECT_EQ(dmath::Vector3D<Type>(1.), (Vector<Type, 3>(1.)));
    } {
        EXPECT_EQ(dmath::Vector4D<Type>(1.), (Vector<Type, 4>(1.)));
    }
}

TEST_F(TestVector, UnaryPlus) {
    {
        doubleVector2D vec(10.);
        EXPECT_EQ(+vec, doubleVector2D(10.));
    } {
        EXPECT_EQ(+doubleVector2D(0.), doubleVector2D(0.));
    }
}

TEST_F(TestVector, Addition) {
    {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec + vec, doubleVector2D(20.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec + doubleVector2D(0.), doubleVector2D(10.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(doubleVector2D(0.) + vec , doubleVector2D(10.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec + 5, doubleVector2D(15.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(5 + vec, doubleVector2D(15.));
    }
}

TEST_F(TestVector, AdditionAssignment) {
    {
        doubleVector2D vec(10.);
        vec += vec;
        EXPECT_EQ(vec, doubleVector2D(20.));
    } {
        doubleVector2D vec(10.);
        vec += 5;
        EXPECT_EQ(vec, doubleVector2D(15.));
    } {
        doubleVector2D vec(10.);
        vec += doubleVector2D(0.);
        EXPECT_EQ(vec, doubleVector2D(10.));
    } {
        doubleVector2D vec(10.);
    }
}

TEST_F(TestVector, UnaryMinus) {
    {
        doubleVector2D vec(10.);
        EXPECT_EQ(-vec, doubleVector2D(-10.));
    } {
        EXPECT_EQ(-doubleVector2D(0.), doubleVector2D(0.));
    }
}


TEST_F(TestVector, Subtraction) {
    {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec - vec, doubleVector2D(0.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec - doubleVector2D(0.), doubleVector2D(10.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(doubleVector2D(0.) - vec , doubleVector2D(-10.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec - 5, doubleVector2D(5.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(5 - vec, doubleVector2D(-5.));
    }
}

TEST_F(TestVector, SubtractionAssignment) {
    {
        doubleVector2D vec(10.);
        doubleVector2D vec_2(5.);
        vec -= vec_2;
        EXPECT_EQ(vec, doubleVector2D(5.));
    } {
        doubleVector2D vec(10.);
        vec -= doubleVector2D(0.);
        EXPECT_EQ(vec, doubleVector2D(10.));
    } {
        doubleVector2D vec(10.);
        vec -= 5;
        EXPECT_EQ(vec, doubleVector2D(5.));
    }
}

TEST_F(TestVector, Multiplication) {
    {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec * vec, doubleVector2D(100.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec * doubleVector2D(0.), doubleVector2D(0.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(doubleVector2D(0.) * vec, doubleVector2D(0.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec * 5, doubleVector2D(50.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(5 * vec, doubleVector2D(50.));
    }
}

TEST_F(TestVector, MultiplicationAssignment) {
    {
        doubleVector2D vec(10.);
        vec *= vec;
        EXPECT_EQ(vec, doubleVector2D(100.));
    } {
        doubleVector2D vec(10.);
        vec *= doubleVector2D(0.);
        EXPECT_EQ(vec, doubleVector2D(0.));
    } {
        doubleVector2D vec(10.);
        vec *= 5;
        EXPECT_EQ(vec, doubleVector2D(50.));
    }
}

TEST_F(TestVector, Division) {
    {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec / vec, doubleVector2D(1.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(doubleVector2D(0.) / vec, doubleVector2D(0.));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(vec / 5, doubleVector2D({2., 2.}));
    } {
        doubleVector2D vec(10.);
        EXPECT_EQ(5 / vec, doubleVector2D(0.5));
    }
}


TEST_F(TestVector, DivisionAssignment) {
    {
        doubleVector2D vec(10.);
        doubleVector2D vec_2(5.);
        vec /= vec_2;
        EXPECT_EQ(vec, doubleVector2D(2.));
    } {
        doubleVector2D vec(10.);
        vec /= 5;
        EXPECT_EQ(vec, doubleVector2D(2.));
    }
}

TEST_F(TestVector, Dot) {
    doubleVector2D vec(1.);
    doubleVector2D vec_2(2.);
    {
        EXPECT_EQ(vec.dot(vec_2), 4.);
    } {
        EXPECT_EQ(vec_2.dot(vec), 4.);
    } {
        EXPECT_EQ(dot(vec_2, vec), 4.);
    } {
        EXPECT_EQ(dot(vec, vec_2), 4.);
    }
}

TEST_F(TestVector, Min) {
    {
        Vector<Type, 4> vec({0.,1.,2.,3.});
        EXPECT_EQ(vec.min(), 0.);
    }
}

TEST_F(TestVector, Max) {
    {
        Vector<Type, 4> vec({0.,1.,2.,3.});
        EXPECT_EQ(vec.max(), 3.);
    }
}

TEST_F(TestVector, Sum) {
    {
        Vector<Type, 4> vec({0.,1.,2.,3.});
        EXPECT_EQ(vec.sum(), 6.);
    }
}

TEST_F(TestVector, Abs) {
    {
        doubleVector2D vec({4.,3.});
        EXPECT_EQ(vec.abs(), 5.);
    }
}

TEST_F(TestVector, Absf) {
    {
        doubleVector2D vec({4.,3.});
        EXPECT_EQ(vec.absf(), 5.);
    }
}

TEST_F(TestVector, Abs2) {
    {
        doubleVector2D vec({4.,3.});
        EXPECT_EQ(vec.abs2(), 25.);
    }
}

TEST_F(TestVector, Norm) {
    {
        doubleVector2D vec({4.,3.});
        EXPECT_EQ(vec.norm(), (Vector<double ,2>({4/5., 3/5.})));
    }
}

TEST_F(TestVector, Normf) {
    {
        doubleVector2D vec({4.,3.});
        EXPECT_EQ(vec.normf(), (Vector<float, 2>({4/5., 3/5.})));
    }
}

TEST_F(TestVector, Print) {
    std::stringstream out;
    out << dmath::Vector<int, 2>(10.);

    EXPECT_EQ(out.str(), ("Vector<i, 2>({10, 10})"));
}

TEST_F(TestVector, Iterators) {
    doubleVector2D vec(10.);
    const auto& constVec = vec;
    {
        EXPECT_EQ(vec.begin(), vec.cbegin());
        EXPECT_EQ(vec.cbegin(), constVec.begin());
        EXPECT_EQ(constVec.begin(), constVec.cbegin());
    } {
        EXPECT_EQ(vec.end(), vec.cend());
        EXPECT_EQ(vec.cend(), constVec.end());
        EXPECT_EQ(constVec.end(), constVec.cend());
    } {
        EXPECT_EQ(vec.rbegin(), vec.crbegin());
        EXPECT_EQ(vec.crbegin(), constVec.rbegin());
        EXPECT_EQ(constVec.rbegin(), constVec.crbegin());

        EXPECT_EQ(vec.rbegin().base(), vec.end());
    } {
        EXPECT_EQ(vec.rend(), vec.crend());
        EXPECT_EQ(vec.crend(), constVec.rend());
        EXPECT_EQ(constVec.rend(), constVec.crend());

        EXPECT_EQ(vec.rend().base(), vec.begin());
    }
}

} // namespace tests
