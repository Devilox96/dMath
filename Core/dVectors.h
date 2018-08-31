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

	dVector3D <VecT> () {
	    x = 0;
	    y = 0;
	    z = 0;
	}
    dVector3D <VecT> (VecT xP, VecT yP, VecT zP) {
        x = xP;
        y = yP;
        z = zP;
    }
	~dVector3D <VecT> () = default;

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
        return dVector3D <VecT> (x + VecP.x, y + VecP.y, z + VecP.z);
    }
    //---Addition---//

    //---Subtraction---//
	dVector3D <VecT> operator-() {
        return dVector3D <VecT> (-x, -y, -z);
    }
	void operator-=(const dVector3D <VecT>& VectP) {
        x -= VectP.x;
        y -= VectP.y;
        z -= VectP.z;
    }
    dVector3D <VecT> operator-(const dVector3D <VecT>& VecP) {
        return dVector3D <VecT> (x - VecP.x, y - VecP.y, z - VecP.z);
    }
    //---Subtraction---//

    //---Multiplying---//
    double operator*(const dVector3D <VecT>& VectP) {
        return x * VectP.x + y * VectP.y + z * VectP.z;
    }
    template <typename T> dVector3D <VecT> operator*(const T NumP) {
        return dVector3D <VecT> (x * NumP, y * NumP, z * NumP);
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
        return dVector3D <VecT> (x / NumP, y / NumP, z / NumP);
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

	dVector3D <VecT> Norm() {
        dVector3D <VecT> TempL;
        double AbsL;

        AbsL = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

        TempL.x = x / AbsL;
        TempL.y = y / AbsL;
        TempL.z = z / AbsL;

        return TempL;
	}
    double Greatest() {
        if (x >= y  && x >= z) {
            return x;
        } else if (y >= x  && y >= z) {
            return y;
        } else {
            return z;
        }
	}
	double Least() {
        if (x <= y && x <= z) {
            return x;
        } else if (y <= x && y <= z) {
            return y;
        } else {
            return z;
        }
    }
};

template <typename T, typename VecT> dVector3D <VecT> operator*(const dVector3D <VecT>& VecP, const T NumP) {
    return dVector3D <VecT> (VecP.x * NumP, VecP.y * NumP, VecP.z * NumP);
}
template <typename T, typename VecT> dVector3D <VecT> operator/(const dVector3D <VecT>& VecP, const T NumP) {
    return dVector3D <VecT> (VecP.x / NumP, VecP.y / NumP, VecP.z / NumP);
}
//----------------------------------//
template <typename VecT> dVector3D <VecT> dVecCrossProd(const dVector3D <VecT>& VectOneP, const dVector3D <VecT>& VectTwoP) {
    dVector3D <VecT> VectL(0.0, 0.0, 0.0);

    VectL.x = VectOneP.y * VectTwoP.z - VectOneP.z * VectTwoP.y;
    VectL.y = VectOneP.z * VectTwoP.x - VectOneP.x * VectTwoP.z;
    VectL.z = VectOneP.x * VectTwoP.y - VectOneP.y * VectTwoP.x;

    return VectL;
}
//----------------------------------//
struct dVector2D {
    double x, y;

    dVector2D();
    dVector2D(double xP, double yP);
    ~dVector2D() = default;

    //---Addition---//
    dVector2D operator+() {
        return *this;
    }
    void operator+=(const dVector2D& VectP);
    dVector2D operator+(const dVector2D& VecP) {
        return dVector2D(x + VecP.x,
                         y + VecP.y);
    }
    //---Addition---//

    //---Subtraction---//
    dVector2D operator-() {
        return dVector2D(-x, -y);
    }
    void operator-=(const dVector2D& VectP);
    dVector2D operator-(const dVector2D& VecP) {
        return dVector2D(x - VecP.x,
                         y - VecP.y);
    }
    //---Subtraction---//

    //---Multiplying---//
    double operator*(const dVector2D& VectP) {
        return x * VectP.x + y * VectP.y;
    }
    template <typename T> dVector2D operator*(const T NumP) {
        return dVector2D(x * NumP, y * NumP);
    }
    template <typename T> void operator*=(const T NumP) {
        x *= NumP;
        y *= NumP;
    }
    template <typename T> friend dVector2D operator*(const dVector2D& VecP, T NumP);
    //---Multiplying---//

    //---Division---//
    template <typename T> dVector2D operator/(const T NumP) {
        return dVector2D(x / NumP, y / NumP);
    }
    template <typename T> void operator/=(const T NumP) {
        x /= NumP;
        y /= NumP;
    }
    template <typename T> friend dVector2D operator/(const dVector2D& VecP, T NumP);
    //---Division---//

    //---Equality---//
    dVector2D& operator=(const dVector2D& VectP) = default;
    bool operator==(const dVector2D& VectP);
    bool operator!=(const dVector2D& VectP);
    //---Equality---//

    //---Absolute value---//
    double Abs() {
        return sqrt(pow(x, 2) + pow(y, 2));
    }
    double Abs_2() {
        return pow(x, 2) + pow(y, 2);
    }
    //---Absolute value---//

    dVector2D Norm();
    double Greatest();
    double Least();
};

template <typename T> dVector2D operator*(const dVector2D& VecP, const T NumP) {
    return dVector2D(VecP.x * NumP, VecP.y * NumP);
}
template <typename T> dVector2D operator/(const dVector2D& VecP, const T NumP) {
    return dVector2D(VecP.x / NumP, VecP.y / NumP);
}
//----------------------------------//
struct dQuaternion {
	double w, x, y, z;

    dQuaternion();
	dQuaternion(double wP, double xP, double yP, double zP);
	~dQuaternion() = default;

    //---Addition---//
    dQuaternion operator+() {
        return *this;
    }
	void operator+=(const dQuaternion& QuatP);
    dQuaternion operator+(const dQuaternion& QuatP) {
        return dQuaternion(w + QuatP.w, x + QuatP.x, y + QuatP.y, z + QuatP.z);
    }
    //---Addition---//

    //---Subtraction---//
    dQuaternion operator-() {
        return dQuaternion(-w, -x, -y, -z);
    }
    void operator-=(const dQuaternion& QuatP);
	dQuaternion operator-(const dQuaternion& QuatP) {
        return dQuaternion(w - QuatP.w, x - QuatP.x, y - QuatP.y, z - QuatP.z);
    }
    //---Subtraction---//

    //---Multiplying---//
	dQuaternion operator*(const dQuaternion& QuatP);
    template <typename T> dQuaternion operator*(const T NumP) {
        return dQuaternion(w * NumP, x * NumP, y * NumP, z * NumP);
    }
	void operator*=(const dQuaternion& QuatP);
    template <typename T> void operator*=(const T NumP) {
        w *= NumP;
        x *= NumP;
        y *= NumP;
        z *= NumP;
    }
    template <typename T> friend dQuaternion operator*(const dQuaternion& QuatP, T NumP);
    //---Multiplying---//

    //---Division---//
	template <typename T> dQuaternion operator/(const T NumP) {
        return dQuaternion(w / NumP, x / NumP, y / NumP, z / NumP);
    }
    template <typename T> void operator/=(const T NumP) {
        w /= NumP;
        x /= NumP;
        y /= NumP;
        z /= NumP;
    }
    template <typename T> friend dQuaternion operator/(const dQuaternion& QuatP, T NumP);
    //---Division---//

    //---Equality---//
	dQuaternion& operator=(const dQuaternion& QuatP) = default;
	bool operator==(const dQuaternion& QuatP);
	bool operator!=(const dQuaternion& QuatP);
    //---Equality---//

	dQuaternion Conjugation() {
        return dQuaternion(w, -x, -y, -z);
    }
	void Reciprocal();
	double Abs() {
        return sqrt(pow(w, 2) + pow(x, 2) + pow(y, 2) + pow(z, 2));
    }
};

template <typename T> dQuaternion operator*(const dQuaternion& QuatP, const T NumP) {
    return dQuaternion(QuatP.w * NumP, QuatP.x * NumP, QuatP.y * NumP, QuatP.z * NumP);
}
template <typename T> dQuaternion operator/(const dQuaternion& QuatP, const T NumP) {
    return dQuaternion(QuatP.w / NumP, QuatP.x / NumP, QuatP.y / NumP, QuatP.z / NumP);
}
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
