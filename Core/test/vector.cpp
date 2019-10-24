#include "gtest/gtest.h"
#include "../dVector.h"

class TestVector : public ::testing::Test
{
protected:
    void SetUp()
    {
        double2D.fill(10);
        double3D.fill(10);
        double4D.fill(10);
        int645D.fill(10);
        float6D.fill(10);
    }
    void TearDown()
    {

    }

    typedef dVector<double, 2> doubleVector2D;
    doubleVector2D double2D;
    typedef dVector<double, 3> doubleVector3D;
    doubleVector3D double3D;
    typedef dVector<double, 4> doubleVector4D;
    doubleVector4D double4D;
    typedef dVector<std::int64_t, 5> int64Vector5D;
    int64Vector5D int645D;
    typedef dVector<float, 6> floatVector6D;
    floatVector6D float6D;

};

TEST_F(TestVector, Addition) {
    EXPECT_EQ(double2D + double2D, doubleVector2D(20., 20.));

    EXPECT_EQ(10 + double3D, doubleVector3D(20., 20., 20.));

    EXPECT_EQ(double4D + 10, doubleVector4D(20., 20., 20., 20.));

    EXPECT_EQ(float6D + floatVector6D::null(), float6D);

    EXPECT_EQ(int64Vector5D::null() + int645D, int645D);
}

TEST_F(TestVector, AdditionAssignment) {
    double2D += double2D;
    EXPECT_EQ(double2D, doubleVector2D(20., 20.));

    double3D += 10;
    EXPECT_EQ(double3D, doubleVector3D(20., 20., 20.));

    double4D += -10;
    EXPECT_EQ(double4D, doubleVector4D::null());

    float6D += floatVector6D::null();
    EXPECT_EQ(float6D, float6D);

    int64Vector5D temp;
    temp.fill(-10.);
    int645D += temp;
    EXPECT_EQ(int645D, int64Vector5D::null());
}

TEST_F(TestVector, UnaryMinus) {
    EXPECT_EQ(-double2D, doubleVector2D(-10., -10.));

    EXPECT_EQ(-double3D, doubleVector3D(-10., -10., -10.));

    EXPECT_EQ(-int64Vector5D::null(), int64Vector5D::null());
}


TEST_F(TestVector, Subtraction) {
    EXPECT_EQ(double2D - double2D, doubleVector2D::null());

    EXPECT_EQ(double3D - 10, doubleVector3D::null());

    EXPECT_EQ(double4D - 0, double4D);

    EXPECT_EQ(float6D - floatVector6D::null(), float6D);

    EXPECT_EQ(int64Vector5D::null() - int645D, int64Vector5D(-10., -10., -10., -10., -10.));
}

TEST_F(TestVector, SubtractionAssignment) {
    double2D -= double2D;
    EXPECT_EQ(double2D, doubleVector2D::null());

    double3D -= -10;
    EXPECT_EQ(double3D, doubleVector3D(20., 20., 20.));

    double4D -= 10;
    EXPECT_EQ(double4D, doubleVector4D::null());

    float6D -= floatVector6D::null();
    EXPECT_EQ(float6D, float6D);

    int64Vector5D temp;
    temp.fill(10);
    int645D -= temp;
    EXPECT_EQ(int645D, int64Vector5D::null());
}

TEST_F(TestVector, Multiplication) {
    EXPECT_EQ(double2D * double2D, 200);

    EXPECT_EQ(double3D * 10, doubleVector3D(100., 100., 100.));

    EXPECT_EQ(double4D * 0, doubleVector4D::null());

    EXPECT_EQ(float6D * floatVector6D::null(), 0);

    EXPECT_EQ(int64Vector5D::null() * int645D, 0);
}

TEST_F(TestVector, MultiplicationAssignment) {
    double2D *= 10;
    EXPECT_EQ(double2D, doubleVector2D(100., 100.));

    double3D *= 0;
    EXPECT_EQ(double3D, doubleVector3D::null());

    double4D *= 1;
    EXPECT_EQ(double4D, double4D);

    float6D *= -1;
    EXPECT_EQ(float6D, floatVector6D(-10., -10., -10., -10., -10., -10.));

    int645D *= -10;
    EXPECT_EQ(int645D, int64Vector5D(-100., -100., -100., -100., -100.));
}

TEST_F(TestVector, Division) {
    EXPECT_EQ(double2D / -1, doubleVector2D(-10., -10.));

    EXPECT_EQ(double3D / 10, doubleVector3D(1., 1., 1.));

    EXPECT_EQ(double4D / -1, doubleVector4D(-10., -10., -10., -10.));

    EXPECT_EQ(float6D / 1, float6D);

    EXPECT_EQ(int645D / -10, int64Vector5D(-1., -1., -1., -1., -1.));
}


TEST_F(TestVector, DivisionAssignment) {
    double2D /= -1;
    EXPECT_EQ(double2D, doubleVector2D(-10., -10.));

    double3D /= 10;
    EXPECT_EQ(double3D, doubleVector3D(1., 1., 1.));

    double4D /= -1;
    EXPECT_EQ(double4D, doubleVector4D(-10., -10., -10., -10.));

    float6D /= 1;
    EXPECT_EQ(float6D, float6D);

    int645D /= -10;
    EXPECT_EQ(int645D, int64Vector5D(-1., -1., -1., -1., -1.));
}