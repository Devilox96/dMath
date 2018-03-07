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
//----------------------------------//
struct dVector3D {
	double x, y, z;

	dVector3D();
	dVector3D(double xP, double yP, double zP);
	~dVector3D() = default;

    //---Addition---//
    dVector3D operator+() {
        return *this;
    }
	void operator+=(const dVector3D& VectP);
    dVector3D operator+(const dVector3D& VecP) {
        return dVector3D(this -> x + VecP.x,
                         this -> y + VecP.y,
                         this -> z + VecP.z);
    }
    //---Addition---//

    //---Subtraction---//
	dVector3D operator-() {
        return dVector3D(-this -> x, -this -> y, -this -> z);
    }
	void operator-=(const dVector3D& VectP);
    dVector3D operator-(const dVector3D& VecP) {
        return dVector3D(this -> x - VecP.x,
                         this -> y - VecP.y,
                         this -> z - VecP.z);
    }
    //---Subtraction---//

    //---Multiplying---//
    double operator*(const dVector3D& VectP) {
        return this -> x * VectP.x + this -> y * VectP.y + this -> z * VectP.z;
    }
    template <typename T> dVector3D operator*(const T NumP) {
        return dVector3D(this -> x * NumP, this -> y * NumP, this -> z * NumP);
    }
    template <typename T> void operator*=(const T NumP) {
        this -> x *= NumP;
        this -> y *= NumP;
        this -> z *= NumP;
    }
    template <typename T> friend dVector3D operator*(const dVector3D& VecP, const T NumP);
    //---Multiplying---//

    //---Division---//
    template <typename T> dVector3D operator/(const T NumP) {
        return dVector3D(this -> x / NumP, this -> y / NumP, this -> z / NumP);
    }
    template <typename T> void operator/=(const T NumP) {
        this -> x /= NumP;
        this -> y /= NumP;
        this -> z /= NumP;
    }
    template <typename T> friend dVector3D operator/(const dVector3D& VecP, const T NumP);
    //---Division---//

    //---Equality---//
	dVector3D& operator=(const dVector3D& VectP);
	bool operator==(const dVector3D& VectP);
	bool operator!=(const dVector3D& VectP);
    //---Equality---//

    //---Absolute value---//
	double Abs() {
        return sqrt(pow(this -> x, 2) + pow(this -> y, 2) + pow(this -> z, 2));
    }
    double Abs_2() {
        return pow(this -> x, 2) + pow(this -> y, 2) + pow(this -> z, 2);
    }
    //---Absolute value---//

	dVector3D Norm();
    double Greatest();
	double Least();
};

template <typename T> dVector3D operator*(const dVector3D& VecP, const T NumP) {
    return dVector3D(VecP.x * NumP, VecP.y * NumP, VecP.z * NumP);
}
template <typename T> dVector3D operator/(const dVector3D& VecP, const T NumP) {
    return dVector3D(VecP.x / NumP, VecP.y / NumP, VecP.z / NumP);
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
        return dVector2D(this -> x + VecP.x,
                         this -> y + VecP.y);
    }
    //---Addition---//

    //---Subtraction---//
    dVector2D operator-() {
        return dVector2D(-this -> x, -this -> y);
    }
    void operator-=(const dVector2D& VectP);
    dVector2D operator-(const dVector2D& VecP) {
        return dVector2D(this -> x - VecP.x,
                         this -> y - VecP.y);
    }
    //---Subtraction---//

    //---Multiplying---//
    double operator*(const dVector2D& VectP) {
        return this -> x * VectP.x + this -> y * VectP.y;
    }
    template <typename T> dVector2D operator*(const T NumP) {
        return dVector2D(this -> x * NumP, this -> y * NumP);
    }
    template <typename T> void operator*=(const T NumP) {
        this -> x *= NumP;
        this -> y *= NumP;
    }
    template <typename T> friend dVector2D operator*(const dVector2D& VecP, const T NumP);
    //---Multiplying---//

    //---Division---//
    template <typename T> dVector2D operator/(const T NumP) {
        return dVector2D(this -> x / NumP, this -> y / NumP);
    }
    template <typename T> void operator/=(const T NumP) {
        this -> x /= NumP;
        this -> y /= NumP;
    }
    template <typename T> friend dVector2D operator/(const dVector2D& VecP, const T NumP);
    //---Division---//

    //---Equality---//
    dVector2D& operator=(const dVector2D& VectP);
    bool operator==(const dVector2D& VectP);
    bool operator!=(const dVector2D& VectP);
    //---Equality---//

    //---Absolute value---//
    double Abs() {
        return sqrt(pow(this -> x, 2) + pow(this -> y, 2));
    }
    double Abs_2() {
        return pow(this -> x, 2) + pow(this -> y, 2);
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
        return dQuaternion(this -> w + QuatP.w, this -> x + QuatP.x, this -> y + QuatP.y, this -> z + QuatP.z);
    }
    //---Addition---//

    //---Subtraction---//
    dQuaternion operator-() {
        return dQuaternion(-this -> w, -this -> x, -this -> y, -this -> z);
    }
    void operator-=(const dQuaternion& QuatP);
	dQuaternion operator-(const dQuaternion& QuatP) {
        return dQuaternion(this -> w - QuatP.w, this -> x - QuatP.x, this -> y - QuatP.y, this -> z - QuatP.z);
    }
    //---Subtraction---//

    //---Multiplying---//
	dQuaternion operator*(const dQuaternion& QuatP);
    template <typename T> dQuaternion operator*(const T NumP) {
        return dQuaternion(this -> w * NumP, this -> x * NumP, this -> y * NumP, this -> z * NumP);
    }
	void operator*=(const dQuaternion& QuatP);
    template <typename T> void operator*=(const T NumP) {
        this -> w *= NumP;
        this -> x *= NumP;
        this -> y *= NumP;
        this -> z *= NumP;
    }
    template <typename T> friend dQuaternion operator*(const dQuaternion& QuatP, const T NumP);
    //---Multiplying---//

    //---Division---//
	template <typename T> dQuaternion operator/(const T NumP) {
        return dQuaternion(this -> w / NumP, this -> x / NumP, this -> y / NumP, this -> z / NumP);
    }
    template <typename T> void operator/=(const T NumP) {
        this -> w /= NumP;
        this -> x /= NumP;
        this -> y /= NumP;
        this -> z /= NumP;
    }
    template <typename T> friend dQuaternion operator/(const dQuaternion& QuatP, const T NumP);
    //---Division---//

    //---Equality---//
	dQuaternion& operator=(const dQuaternion& QuatP);
	bool operator==(const dQuaternion& QuatP);
	bool operator!=(const dQuaternion& QuatP);
    //---Equality---//

	dQuaternion Conjugation() {
        return dQuaternion(this -> w, -(this -> x), -(this -> y), -(this -> z));
    }
	void Reciprocal();
	double Abs() {
        return sqrt(pow(this -> w, 2) + pow(this -> x, 2) + pow(this -> y, 2) + pow(this -> z, 2));
    }
};

template <typename T> dQuaternion operator*(const dQuaternion& QuatP, const T NumP) {
    return dQuaternion(QuatP.w * NumP, QuatP.x * NumP, QuatP.y * NumP, QuatP.z * NumP);
}
template <typename T> dQuaternion operator/(const dQuaternion& QuatP, const T NumP) {
    return dQuaternion(QuatP.w / NumP, QuatP.x / NumP, QuatP.y / NumP, QuatP.z / NumP);
}
//----------------------------------//
dVector3D dVecCrossProd(const dVector3D& VectOneP, const dVector3D& VectTwoP);
//----------------------------------//
struct dVectorND {
private:
    std :: vector <double> Vec;
    unsigned long Num = 0;
public:
    dVectorND(std :: initializer_list <double> CoordsP);
    explicit dVectorND(unsigned long NumP);
    ~dVectorND();
    
    //---Addition---//
    dVectorND operator+() {
        return *this;
    }
    void operator+=(const dVectorND& VectP);
    dVectorND operator+(const dVectorND& VectP) {
        dVectorND TempVecL(Num);

        if (this -> Num != VectP.Num) {
            return TempVecL;
        }

        for (unsigned long i = 0; i < Num; i++) {
            TempVecL.Vec[i] = this -> Vec[i] + VectP.Vec[i];
        }

        return TempVecL;
    }
    //---Addition---//

    //---Subtraction---//
    dVectorND operator-() {
        dVectorND TempVecL(Num);

        for (unsigned long i = 0; i < Num; i++) {
            TempVecL.Vec[i] = -this -> Vec[i];
        }

        return TempVecL;
    }
    void operator-=(const dVectorND& VectP);
    dVectorND operator-(const dVectorND& VectP) {
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

        if (this -> Num != VectP.Num) {
            return ResL;
        }

        for (unsigned long i = 0; i < Num; i++) {
            ResL += this -> Vec[i] * VectP.Vec[i];
        }

        return ResL;
    }
    template <typename T> dVectorND operator*(const T NumP) {
        dVectorND TempVecL(Num);

        for (unsigned long i = 0; i < Num; i++) {
            TempVecL.Vec[i] = this -> Vec[i] * NumP;
        }

        return TempVecL;
    }
    template <typename T> void operator*=(const T NumP) {
        for (unsigned long i = 0; i < Num; i++) {
            this -> Vec[i] *= NumP;
        }
    }
    template <typename T> friend dVectorND operator*(const dVectorND& VectP, const T NumP);
    //---Multiplying---//

    //---Division---//
    template <typename T> dVectorND operator/(const T NumP) {
        dVectorND TempVecL(Num);

        for (unsigned long i = 0; i < Num; i++) {
            TempVecL.Vec[i] = this -> Vec[i] / NumP;
        }

        return TempVecL;
    }
    template <typename T> void operator/=(const T NumP) {
        for (unsigned long i = 0; i < Num; i++) {
            this -> Vec[i] /= NumP;
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
            ResL += pow(this -> Vec[i], 2);
        }

        return sqrt(ResL);
    }
    double Abs_2() {
        double ResL = 0.0;

        for (unsigned long i = 0; i < Num; i++) {
            ResL += pow(this -> Vec[i], 2);
        }

        return ResL;
    }
    //---Absolute value---//

    dVectorND Norm();
    double Greatest();
    double Least();

    //---Access---//
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
