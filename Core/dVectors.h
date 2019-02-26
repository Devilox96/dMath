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
 */
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
    dVector3D <VecT> operator+(const dVector3D <VecT>& VectP) const {
        return dVector3D <VecT>(x + VectP.x, y + VectP.y, z + VectP.z);
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
    dVector3D <VecT> operator-(const dVector3D <VecT>& VectP) const {
        return dVector3D <VecT>(x - VectP.x, y - VectP.y, z - VectP.z);
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
    dVector2D <VecT> operator+(const dVector2D <VecT>& VecP) const {
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
    dVector2D <VecT> operator-(const dVector2D <VecT>& VecP) const {
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
template <typename T>
class dVectorND {
public:
    explicit dVectorND <T>(unsigned long SizeP) : Data(new T[SizeP]), Size(SizeP) {}
    dVectorND <T>(const std::initializer_list <T>& ArgsP) : Data(new T[ArgsP.size()]), Size(ArgsP.size()) {
        std::copy(ArgsP.begin(), ArgsP.end(), Data);
    }
    dVectorND <T>(const dVectorND <T>& CopyVectorP) : Data(new T[CopyVectorP.Size]), Size(CopyVectorP.Size) {
        std::copy(CopyVectorP.Data, CopyVectorP.Data + Size, Data);
    }
    dVectorND <T>(dVectorND <T>&& MoveVectorP) noexcept : Data(std::exchange(MoveVectorP.Data, nullptr)), Size(MoveVectorP.Size) {}
    ~dVectorND <T>() {
        delete[] Data;
    }

    //----------//

    dVectorND <T>& operator=(const dVectorND <T>& CopyVectorP) {
        *this = dVectorND <T>(CopyVectorP);
        return *this;
    }
    dVectorND <T>& operator=(dVectorND <T>&& MoveVectorP) noexcept {
        std::swap(Data, MoveVectorP.Data);
        return *this;
    }

    //----------//

    void operator+=(const dVectorND <T>& VectorP) {
        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;

            return;
        }

        for (unsigned long i = 0; i < Size; i++) {
            Data[i] += VectorP.Data[i];
        }
    }
    dVectorND <T> operator+(const dVectorND <T>& VectorP) const {
        dVectorND <T> TempVecL(Size);

        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
        } else {
            for (unsigned long i = 0; i < Size; i++) {
                TempVecL.Vec[i] = Data[i] + VectorP.Data[i];
            }
        }

        return TempVecL;
    }

    //----------//

    dVectorND <T> operator-() {
        dVectorND <T> TempVecL(Size);

        for (unsigned long i = 0; i < Size; i++) {
            TempVecL.Vec[i] = -Data[i];
        }

        return TempVecL;
    }
    void operator-=(const dVectorND <T>& VectorP) {
        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;

            return;
        }

        for (unsigned long i = 0; i < Size; i++) {
            Data[i] -= VectorP.Data[i];
        }
    }
    dVectorND <T> operator-(const dVectorND <T>& VectorP) const {
        dVectorND <T> TempVecL(Size);

        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
        } else {
            for (unsigned long i = 0; i < Size; i++) {
                TempVecL.Vec[i] = Data[i] - VectorP.Data[i];
            }
        }

        return TempVecL;
    }

    //----------//

    T operator*(const dVectorND <T>& VectorP) const {
        T ResultL = 0;

        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
        } else {
            for (unsigned long i = 0; i < Size; i++) {
                ResultL += Data[i] * VectorP.Data[i];
            }
        }

        return ResultL;
    }
    template <typename NumberT> void operator*=(const NumberT NumberP) {
        for (unsigned long i = 0; i < Size; i++) {
            Data[i] *= NumberP;
        }
    }
    template <typename NumberT> friend dVectorND <T> operator*(const dVectorND <T>& VectorP, NumberT NumberP) {
        dVectorND <T> TempVecL(VectorP.Size);

        for (unsigned long i = 0; i < VectorP.Size; i++) {
            TempVecL.Data[i] = VectorP.Data[i] * NumberP;
        }

        return TempVecL;
    }
    template <typename NumberT> friend dVectorND <T> operator*(NumberT NumberP, const dVectorND <T>& VectorP) {
        dVectorND <T> TempVecL(VectorP.Size);

        for (unsigned long i = 0; i < VectorP.Size; i++) {
            TempVecL.Data[i] = VectorP.Data[i] * NumberP;
        }

        return TempVecL;
    }

    //----------//

    template <typename NumberT> void operator/=(const NumberT NumberP) {
        for (unsigned long i = 0; i < Size; i++) {
            Data[i] /= NumberP;
        }
    }
    template <typename NumberT> friend dVectorND <T> operator/(const dVectorND <T>& VectorP, NumberT NumberP) {
        dVectorND <T> TempVecL(VectorP.Size);

        for (unsigned long i = 0; i < VectorP.Size; i++) {
            TempVecL.Data[i] = VectorP.Data[i] / NumberP;
        }

        return TempVecL;
    }

    //----------//

    bool operator==(const dVectorND <T>& VectorP) {
        if (Size != VectorP.Size) {
            return false;
        }

        return std::equal(std::begin(Data), std::end(Data)), std::begin(VectorP.Data);
    }
    bool operator!=(const dVectorND <T>& VectorP) {
        if (Size != VectorP.Size) {
            return true;
        }

        return !std::equal(std::begin(Data), std::end(Data)), std::begin(VectorP.Data);
    }

    //----------//

    T& operator[](unsigned long NumP) {
        if (NumP < Size && NumP >= 0) {
            return Data[NumP];
        } else {
            std::cout << "Error (dVectorND): out of range!" << std::endl;
            return Data[0];
        }
    }
    T operator[](unsigned long NumP) const {
        if (NumP < Size && NumP >= 0) {
            return Data[NumP];
        } else {
            std::cout << "Error (dVectorND): out of range!" << std::endl;
            return Data[0];
        }
    }

    //----------//

    friend std::ostream& operator<<(std::ostream& StreamP, const dVectorND <T>& VectorP) {
        for (unsigned long i = 0; i < VectorP.Size; i++) {
            StreamP << VectorP.Data[i] << " ";
        }

        return StreamP;
    }

    //----------//

    double Abs() {
        double ResL = 0.0;

        for (unsigned long i = 0; i < Size; i++) {
            ResL += pow(Data[i], 2);
        }

        return sqrt(ResL);
    }
    float Absf() {
        float ResL = 0.0;

        for (unsigned long i = 0; i < Size; i++) {
            ResL += powf(Data[i], 2);
        }

        return sqrtf(ResL);
    }
    T Abs_2() {
        double ResL = 0.0;

        for (unsigned long i = 0; i < Size; i++) {
            ResL += pow(Data[i], 2);
        }

        return ResL;
    }

    //----------//

    dVectorND <double> Norm() {
        double SumL = 0.0;
        dVectorND <double> VecL(Size);

        for (unsigned long i = 0; i < Size; i++) {
            SumL += pow(Data[i], 2);
        }

        double AbsL = sqrt(SumL);

        for (unsigned long i = 0; i < Size; i++) {
            VecL.Data[i] = Data[i] / AbsL;
        }

        return VecL;
    }
    dVectorND <float> Normf() {
        float SumL = 0.0;
        dVectorND <float> VecL(Size);

        for (unsigned long i = 0; i < Size; i++) {
            SumL += powf(Data[i], 2);
        }

        float AbsL = sqrtf(SumL);

        for (unsigned long i = 0; i < Size; i++) {
            VecL.Data[i] = Data[i] / AbsL;
        }

        return VecL;
    }

    //----------//

    T Greatest() {
        T GreatestL = Data[0];

        for (unsigned long i = 1; i < Size; i++) {
            if (Data[i] > GreatestL) {
                GreatestL = Data[i];
            }
        }

        return GreatestL;
    }
    T Least() {
        T LeastL = Data[0];

        for (unsigned long i = 1; i < Size; i++) {
            if (Data[i] < LeastL) {
                LeastL = Data[i];
            }
        }

        return LeastL;
    }

    //----------//

    unsigned long GetSize() const {
        return Size;
    }
private:
    T* Data;
    unsigned long Size;
};
//----------------------------------//
#endif
