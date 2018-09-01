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
template <typename VecT>
struct dVectorND {
private:
    std :: vector <double> Vec;
public:
    dVectorND <VecT>() = default;
    dVectorND <VecT>(std::initializer_list <double> CoordsP) {
        for (double CoordI : CoordsP) {
            Vec.emplace_back(CoordI);
        }
    }
    explicit dVectorND <VecT>(unsigned long NumP) {
        Vec.resize(NumP, 0);
    }
    ~dVectorND <VecT>() = default;

    void Resize(unsigned long NumP) {
        Vec.resize(NumP);
    }

    //---Addition---//
    dVectorND <VecT> operator+() {
        return *this;
    }
    void operator+=(const dVectorND <VecT>& VectP) {
        if (Vec.size() != VectP.Size()) {
            return;
        }

        for (unsigned long i = 0; i < Vec.size(); i++) {
            Vec[i] += VectP.Vec[i];
        }
    }
    dVectorND <VecT> operator+(const dVectorND <VecT>& VectP) const {
        dVectorND <VecT> TempVecL(Vec.size());

        if (Vec.size() != VectP.Vec.size()) {
            return TempVecL;
        }

        for (unsigned long i = 0; i < Vec.size(); i++) {
            TempVecL.Vec[i] = Vec[i] + VectP.Vec[i];
        }

        return TempVecL;
    }
    //---Addition---//

    //---Subtraction---//
    dVectorND <VecT> operator-() {
        dVectorND <VecT> TempVecL(Vec.size());

        for (unsigned long i = 0; i < Vec.size(); i++) {
            TempVecL.Vec[i] = -Vec[i];
        }

        return TempVecL;
    }
    void operator-=(const dVectorND <VecT>& VectP) {
        if (Vec.size() != VectP.Size()) {
            return;
        }

        for (unsigned long i = 0; i < Vec.size(); i++) {
            Vec[i] -= VectP.Vec[i];
        }
    }
    dVectorND <VecT> operator-(const dVectorND <VecT>& VectP) const {
        dVectorND <VecT> TempVecL(Vec.size());

        if (Vec.size() != VectP.Vec.size()) {
            return TempVecL;
        }

        for (unsigned long i = 0; i < Vec.size(); i++) {
            TempVecL.Vec[i] = Vec[i] - VectP.Vec[i];
        }

        return TempVecL;
    }
    //---Subtraction---//

    //---Multiplying---//
    double operator*(const dVectorND <VecT>& VectP) {
        double ResL = 0.0;

        if (Vec.size() != VectP.Vec.size()) {
            return ResL;
        }

        for (unsigned long i = 0; i < Vec.size(); i++) {
            ResL += Vec[i] * VectP.Vec[i];
        }

        return ResL;
    }
    template <typename T> dVectorND <VecT> operator*(const T NumP) {
        dVectorND <VecT> TempVecL(Vec.size());

        for (unsigned long i = 0; i < Vec.size(); i++) {
            TempVecL.Vec[i] = Vec[i] * NumP;
        }

        return TempVecL;
    }
    template <typename T> void operator*=(const T NumP) {
        for (const double& NumI : Vec) {
            NumI *= NumP;
        }
    }
    template <typename T> friend dVectorND <VecT> operator*(const dVectorND <VecT>& VectP, const T NumP);
    //---Multiplying---//

    //---Division---//
    template <typename T> dVectorND <VecT> operator/(const T NumP) {
        dVectorND <VecT> TempVecL(Vec.size());

        for (unsigned long i = 0; i < Vec.size(); i++) {
            TempVecL.Vec[i] = Vec[i] / NumP;
        }

        return TempVecL;
    }
    template <typename T> void operator/=(const T NumP) {
        for (const double& NumI : Vec) {
            NumI /= NumP;
        }
    }
    template <typename T> friend dVectorND <VecT> operator/(const dVectorND <VecT>& VectP, const T NumP);
    //---Division---//

    //---Equality---//
    dVectorND <VecT>& operator=(const dVectorND <VecT>& VectP) {
        if (Vec.size() != VectP.Size()) {
            return *this;
        }

        Vec = VectP.Vec;

        return *this;
    }
    bool operator==(const dVectorND <VecT>& VectP) {
        if (Vec.size() != VectP.Vec.size()) {
            return true;
        }

        return Vec == VectP.Vec;
    }
    bool operator!=(const dVectorND <VecT>& VectP) {
        if (Vec.size() != VectP.Vec.size()) {
            return true;
        }

        return Vec != VectP.Vec;
    }
    //---Equality---//

    //---Absolute value---//
    double Abs() {
        double ResL = 0.0;

        for (const double& NumI : Vec) {
            ResL += pow(NumI, 2);
        }

        return sqrt(ResL);
    }
    double Abs_2() {
        double ResL = 0.0;

        for (const double& NumI : Vec) {
            ResL += pow(NumI, 2);
        }

        return ResL;
    }
    //---Absolute value---//

    dVectorND <double> Norm() {
        double SumL = 0.0;
        dVectorND <double> VecL(Vec.size());

        for (unsigned long i = 0; i < Vec.size(); i++) {
            SumL += pow(Vec[i], 2);
        }

        double AbsL = sqrt(SumL);

        for (unsigned long i = 0; i < Vec.size(); i++) {
            VecL.Vec[i] = Vec[i] / AbsL;
        }

        return VecL;
    }
    VecT Greatest() {
        VecT GreatestL = Vec[0];

        for (unsigned long i = 1; i < Vec.size(); i++) {
            if (Vec[i] > GreatestL) {
                GreatestL = Vec[i];
            }
        }

        return GreatestL;
    }
    VecT Least() {
        VecT LeastL = Vec[0];

        for (unsigned long i = 1; i < Vec.size(); i++) {
            if (Vec[i] < LeastL) {
                LeastL = Vec[i];
            }
        }

        return LeastL;
    }

    //---Access---//
    VecT& operator[](unsigned long NumP) {
        if (NumP < Vec.size()) {
            return Vec[NumP];
        } else {
            return Vec[0];
        }
    }
    VecT ConstGet(unsigned long NumP) const {
        return Vec[NumP];
    }
    unsigned long Size() const {
        return Vec.size();
    }
    //---Access---//
};
//----------------------------------//
#endif
