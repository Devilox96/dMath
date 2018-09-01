/*
 * There is no norm of a quaternion because there is a difference
 * between Russian definition of the norm [srqt(abs(Quat))] and English 
 * [norm = abs], so, use abs to make a norm which you like.
 *
 * Some functions:
 *
 * double operator*(const dVector3D& VectP)  -      Dot product
 * double Abs_2()                            -      Abs * Abs
 * double Greatest()                         -      Get the greatest coordinate
 * double Least()                            -      Get the least coordinate
 * */
//----------------------------------//
#ifndef DVECTORS_H
#define DVECTORS_H
//----------------------------------//
#include <iostream>
#include <cmath>
#include <vector>
#include <initializer_list>
#include <tuple>
//----------------------------------//
template <typename VecT>
struct dVector3D {
    VecT x, y, z;

	dVector3D <VecT>() : x(0), y(0), z(0) {}
    dVector3D <VecT>(VecT xP, VecT yP, VecT zP) : x(xP), y(yP), z(zP) {}
	~dVector3D <VecT>() = default;

    //---Addition---//
    dVector3D <VecT> operator+() {
        return *this;
    }
	void operator+=(const dVector3D <VecT>& VectP) {
        x += VectP.x;
        y += VectP.y;
        z += VectP.z;
    }
    dVector3D <VecT> operator+(const dVector3D <VecT>& VecP) {
        return dVector3D <VecT>(x + VecP.x, y + VecP.y, z + VecP.z);
    }
    //---Addition---//

    //---Subtraction---//
	dVector3D <VecT> operator-() {
        return dVector3D <VecT>(-x, -y, -z);
    }
	void operator-=(const dVector3D <VecT>& VectP) {
        x -= VectP.x;
        y -= VectP.y;
        z -= VectP.z;
    }
    dVector3D <VecT> operator-(const dVector3D <VecT>& VecP) {
        return dVector3D <VecT>(x - VecP.x, y - VecP.y, z - VecP.z);
    }
    //---Subtraction---//

    //---Multiplying---//
    double operator*(const dVector3D <VecT>& VectP) {
        return x * VectP.x + y * VectP.y + z * VectP.z;
    }
    template <typename T> dVector3D <VecT> operator*(const T NumP) {
        return dVector3D <VecT>(x * NumP, y * NumP, z * NumP);
    }
    template <typename T> void operator*=(const T NumP) {
        x *= NumP;
        y *= NumP;
        z *= NumP;
    }
    template <typename T> friend dVector3D operator*(const dVector3D& VecP, T NumP);
    //---Multiplying---//

    //---Division---//
    template <typename T> dVector3D <VecT> operator/(const T NumP) {
        return dVector3D <VecT>(x / NumP, y / NumP, z / NumP);
    }
    template <typename T> void operator/=(const T NumP) {
        x /= NumP;
        y /= NumP;
        z /= NumP;
    }
    template <typename T> friend dVector3D operator/(const dVector3D& VecP, T NumP);
    //---Division---//

    //---Equality---//
	dVector3D <VecT>& operator=(const dVector3D <VecT>& VectP) = default;
	bool operator==(const dVector3D <VecT>& VectP) {
        return std::tie(x, y, z) == std::tie(VectP.x, VectP.y, VectP.z);
	}
	bool operator!=(const dVector3D <VecT>& VectP) {
        return !(std::tie(x, y, z) == std::tie(VectP.x, VectP.y, VectP.z));
	}
    //---Equality---//

    //---Absolute value---//
	double Abs() {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }
    double Abs_2() {
        return pow(x, 2) + pow(y, 2) + pow(z, 2);
    }
    //---Absolute value---//

	dVector3D <double> Norm() {
        dVector3D <double> TempL(0.0, 0.0, 0.0);
        double AbsL;

        AbsL = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

        TempL.x = x / AbsL;
        TempL.y = y / AbsL;
        TempL.z = z / AbsL;

        return TempL;
	}
    VecT Greatest() {
        if (x >= y  && x >= z) {
            return x;
        } else if (y >= x  && y >= z) {
            return y;
        } else {
            return z;
        }
	}
    VecT Least() {
        if (x <= y && x <= z) {
            return x;
        } else if (y <= x && y <= z) {
            return y;
        } else {
            return z;
        }
    }
};
//----------------------------------//
template <typename VecT>
struct dVector2D {
    VecT x, y;

    dVector2D <VecT>() : x(0), y(0) {}
    dVector2D <VecT>(VecT xP, VecT yP) : x(xP), y(yP) {}
    ~dVector2D <VecT>() = default;

    //---Addition---//
    dVector2D <VecT> operator+() {
        return *this;
    }
    void operator+=(const dVector2D <VecT>& VectP) {
        x += VectP.x;
        y += VectP.y;
    }
    dVector2D <VecT> operator+(const dVector2D <VecT>& VecP) {
        return dVector2D <VecT>(x + VecP.x, y + VecP.y);
    }
    //---Addition---//

    //---Subtraction---//
    dVector2D <VecT> operator-() {
        return dVector2D <VecT>(-x, -y);
    }
    void operator-=(const dVector2D <VecT>& VectP) {
        x -= VectP.x;
        y -= VectP.y;
    }
    dVector2D <VecT> operator-(const dVector2D <VecT>& VecP) {
        return dVector2D <VecT>(x - VecP.x, y - VecP.y);
    }
    //---Subtraction---//

    //---Multiplying---//
    double operator*(const dVector2D <VecT>& VectP) {
        return x * VectP.x + y * VectP.y;
    }
    template <typename T> dVector2D <VecT> operator*(const T NumP) {
        return dVector2D <VecT>(x * NumP, y * NumP);
    }
    template <typename T> void operator*=(const T NumP) {
        x *= NumP;
        y *= NumP;
    }
    template <typename T> friend dVector2D <VecT> operator*(const dVector2D <VecT>& VecP, T NumP);
    //---Multiplying---//

    //---Division---//
    template <typename T> dVector2D <VecT> operator/(const T NumP) {
        return dVector2D <VecT>(x / NumP, y / NumP);
    }
    template <typename T> void operator/=(const T NumP) {
        x /= NumP;
        y /= NumP;
    }
    template <typename T> friend dVector2D <VecT> operator/(const dVector2D <VecT>& VecP, T NumP);
    //---Division---//

    //---Equality---//
    dVector2D <VecT>& operator=(const dVector2D <VecT>& VectP) = default;
    bool operator==(const dVector2D <VecT>& VectP) {
        return std::tie(x, y) == std::tie(VectP.x, VectP.y);
    }
    bool operator!=(const dVector2D <VecT>& VectP) {
        return !(std::tie(x, y) == std::tie(VectP.x, VectP.y));
    }
    //---Equality---//

    //---Absolute value---//
    double Abs() {
        return sqrt(pow(x, 2) + pow(y, 2));
    }
    double Abs_2() {
        return pow(x, 2) + pow(y, 2);
    }
    //---Absolute value---//

    dVector2D <double> Norm() {
        dVector2D <double> TempL(0.0, 0.0);
        double AbsL;

        AbsL = sqrt(pow(x, 2) + pow(y, 2));

        TempL.x = x / AbsL;
        TempL.y = y / AbsL;

        return TempL;
    }
    VecT Greatest() {
        if (x >= y) {
            return x;
        } else {
            return y;
        }
    }
    VecT Least() {
        if (x <= y) {
            return x;
        } else {
            return y;
        }
    }
};
//----------------------------------//
template <typename QuatT>
struct dQuaternion {
    QuatT w, x, y, z;

    dQuaternion <QuatT>() : w(0), x(0), y(0), z(0) {}
    dQuaternion <QuatT>(QuatT wP, QuatT xP, QuatT yP, QuatT zP) : w(wP), x(xP), y(yP), z(zP) {}
	~dQuaternion <QuatT>() = default;

    //---Addition---//
    dQuaternion <QuatT> operator+() {
        return *this;
    }
    void operator+=(const dQuaternion <QuatT>& QuatP) {
        w += QuatP.w;
        x += QuatP.x;
        y += QuatP.y;
        z += QuatP.z;
    }
    dQuaternion <QuatT> operator+(const dQuaternion <QuatT>& QuatP) {
        return dQuaternion <QuatT>(w + QuatP.w, x + QuatP.x, y + QuatP.y, z + QuatP.z);
    }
    //---Addition---//

    //---Subtraction---//
    dQuaternion <QuatT> operator-() {
        return dQuaternion <QuatT>(-w, -x, -y, -z);
    }
    void operator-=(const dQuaternion <QuatT>& QuatP) {
        w -= QuatP.w;
        x -= QuatP.x;
        y -= QuatP.y;
        z -= QuatP.z;
    }
	dQuaternion <QuatT> operator-(const dQuaternion <QuatT>& QuatP) {
        return dQuaternion <QuatT>(w - QuatP.w, x - QuatP.x, y - QuatP.y, z - QuatP.z);
    }
    //---Subtraction---//

    //---Multiplying---//
    dQuaternion <QuatT> operator*(const dQuaternion <QuatT>& QuatP) {
        dQuaternion <QuatT> QuatL(0.0, 0.0, 0.0, 0.0);

        QuatL.w = w * QuatP.w - x * QuatP.x - y * QuatP.y - z * QuatP.z;
        QuatL.x = w * QuatP.x + x * QuatP.w + y * QuatP.z - z * QuatP.y;
        QuatL.y = w * QuatP.y + y * QuatP.w + z * QuatP.x - x * QuatP.z;
        QuatL.z = w * QuatP.z + z * QuatP.w + x * QuatP.y - y * QuatP.x;

        return QuatL;
    }
    template <typename T> dQuaternion <QuatT> operator*(const T NumP) {
        return dQuaternion <QuatT> (w * NumP, x * NumP, y * NumP, z * NumP);
    }
    void operator*=(const dQuaternion <QuatT>& QuatP) {
        dQuaternion <QuatT> QuatL(0.0, 0.0, 0.0, 0.0);

        QuatL.w = w * QuatP.w - x * QuatP.x - y * QuatP.y - z * QuatP.z;
        QuatL.x = w * QuatP.x + x * QuatP.w + y * QuatP.z - z * QuatP.y;
        QuatL.y = w * QuatP.y + y * QuatP.w + z * QuatP.x - x * QuatP.z;
        QuatL.z = w * QuatP.z + z * QuatP.w + x * QuatP.y - y * QuatP.x;

        w = QuatL.w;
        x = QuatL.x;
        y = QuatL.y;
        z = QuatL.z;
    }
    template <typename T> void operator*=(const T NumP) {
        w *= NumP;
        x *= NumP;
        y *= NumP;
        z *= NumP;
    }
    template <typename T> friend dQuaternion <QuatT> operator*(const dQuaternion <QuatT>& QuatP, T NumP);
    //---Multiplying---//

    //---Division---//
	template <typename T> dQuaternion <QuatT> operator/(const T NumP) {
        return dQuaternion <QuatT>(w / NumP, x / NumP, y / NumP, z / NumP);
    }
    template <typename T> void operator/=(const T NumP) {
        w /= NumP;
        x /= NumP;
        y /= NumP;
        z /= NumP;
    }
    template <typename T> friend dQuaternion <QuatT> operator/(const dQuaternion <QuatT>& QuatP, T NumP);
    //---Division---//

    //---Equality---//
	dQuaternion <QuatT>& operator=(const dQuaternion <QuatT>& QuatP) = default;
    bool operator==(const dQuaternion <QuatT>& QuatP) {
        return std::tie(w, x, y, z) == std::tie(QuatP.w, QuatP.x, QuatP.y, QuatP.z);
    }
    bool operator!=(const dQuaternion <QuatT>& QuatP) {
        return !(std::tie(w, x, y, z) == std::tie(QuatP.w, QuatP.x, QuatP.y, QuatP.z));
    }
    //---Equality---//

	dQuaternion <QuatT> Conjugation() {
        return dQuaternion <QuatT>(w, -x, -y, -z);
    }
    void Reciprocal() {
        dQuaternion <QuatT> QuatL;
        double NormL;

        QuatL.w = w;
        QuatL.x = -x;
        QuatL.y = -y;
        QuatL.z = -z;

        NormL = pow(w, 2) + pow(x, 2) + pow(y, 2) + pow(z, 2);

        QuatL.w /= NormL;
        QuatL.x /= NormL;
        QuatL.y /= NormL;
        QuatL.z /= NormL;

        *this = QuatL;
    }
	double Abs() {
        return sqrt(pow(w, 2) + pow(x, 2) + pow(y, 2) + pow(z, 2));
    }
};
//----------------------------------//
struct dVectorND {
private:
    std :: vector <double> Vec;
    unsigned long Num = 0;
public:
    dVectorND() = default;
    dVectorND(std :: initializer_list <double> CoordsP);
    explicit dVectorND(unsigned long NumP);
    ~dVectorND();

    void Resize(unsigned long NumP);

    //---Addition---//
    dVectorND operator+() {
        return *this;
    }
    void operator+=(const dVectorND& VectP);
    dVectorND operator+(const dVectorND& VectP) const {
        dVectorND TempVecL(Num);

        if (Num != VectP.Num) {
            return TempVecL;
        }

        for (unsigned long i = 0; i < Num; i++) {
            TempVecL.Vec[i] = Vec[i] + VectP.Vec[i];
        }

        return TempVecL;
    }
    //---Addition---//

    //---Subtraction---//
    dVectorND operator-() {
        dVectorND TempVecL(Num);

        for (unsigned long i = 0; i < Num; i++) {
            TempVecL.Vec[i] = -Vec[i];
        }

        return TempVecL;
    }
    void operator-=(const dVectorND& VectP);
    dVectorND operator-(const dVectorND& VectP) const {
        dVectorND TempVecL(Num);

        if (Num != VectP.Num) {
            return TempVecL;
        }

        for (unsigned long i = 0; i < Num; i++) {
            TempVecL.Vec[i] = Vec[i] - VectP.Vec[i];
        }

        return TempVecL;
    }
    //---Subtraction---//

    //---Multiplying---//
    double operator*(const dVectorND& VectP) {
        double ResL = 0.0;

        if (Num != VectP.Num) {
            return ResL;
        }

        for (unsigned long i = 0; i < Num; i++) {
            ResL += Vec[i] * VectP.Vec[i];
        }

        return ResL;
    }
    template <typename T> dVectorND operator*(const T NumP) {
        dVectorND TempVecL(Num);

        for (unsigned long i = 0; i < Num; i++) {
            TempVecL.Vec[i] = Vec[i] * NumP;
        }

        return TempVecL;
    }
    template <typename T> void operator*=(const T NumP) {
        for (unsigned long i = 0; i < Num; i++) {
            Vec[i] *= NumP;
        }
    }
    template <typename T> friend dVectorND operator*(const dVectorND& VectP, const T NumP);
    //---Multiplying---//

    //---Division---//
    template <typename T> dVectorND operator/(const T NumP) {
        dVectorND TempVecL(Num);

        for (unsigned long i = 0; i < Num; i++) {
            TempVecL.Vec[i] = Vec[i] / NumP;
        }

        return TempVecL;
    }
    template <typename T> void operator/=(const T NumP) {
        for (unsigned long i = 0; i < Num; i++) {
            Vec[i] /= NumP;
        }
    }
    template <typename T> friend dVectorND operator/(const dVectorND& VectP, const T NumP);
    //---Division---//

    //---Equality---//
    dVectorND& operator=(const dVectorND& VectP);
    bool operator==(const dVectorND& VectP);
    bool operator!=(const dVectorND& VectP);
    //---Equality---//

    //---Absolute value---//
    double Abs() {
        double ResL = 0.0;

        for (unsigned long i = 0; i < Num; i++) {
            ResL += pow(Vec[i], 2);
        }

        return sqrt(ResL);
    }
    double Abs_2() {
        double ResL = 0.0;

        for (unsigned long i = 0; i < Num; i++) {
            ResL += pow(Vec[i], 2);
        }

        return ResL;
    }
    //---Absolute value---//

    dVectorND Norm();
    double Greatest();
    double Least();

    //---Access---//
    double operator[](unsigned long NumP) const {
        if (NumP < Vec.size()) {
            return Vec[NumP];
        } else {
            return Vec[0];
        }
    }
    double Get(unsigned long NumP) {
        return Vec[NumP];
    }
    unsigned long Size() const {
        return Num;
    }
    //---Access---//
};

template <typename T> dVectorND operator*(const dVectorND& VectP, const T NumP) {
    dVectorND TempVecL(VectP.Size());

    for (unsigned long i = 0; i < VectP.Size(); i++) {
        TempVecL.Vec[i] = VectP.Vec[i] * NumP;
    }

    return TempVecL;
}
template <typename T> dVectorND operator/(const dVectorND& VectP, const T NumP) {
    dVectorND TempVecL(VectP.Size());

    for (unsigned long i = 0; i < VectP.Size(); i++) {
        TempVecL.Vec[i] = VectP.Vec[i] / NumP;
    }

    return TempVecL;
}
//----------------------------------//
#endif
