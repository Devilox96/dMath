#ifndef DVECTOR3D_H
#define DVECTOR3D_H
//-----------------------------//
#include <iostream>
#include <cmath>
#include <tuple>
//-----------------------------//
template <typename T>
class dVector3D {
public:
    dVector3D <T>() : x(0), y(0), z(0) {}
    dVector3D <T>(T xP, T yP, T zP) : x(xP), y(yP), z(zP) {}
    dVector3D <T>(const dVector3D <T>& CopyVectorP) : x(CopyVectorP.x), y(CopyVectorP.y), z(CopyVectorP.z) {}
    ~dVector3D <T>() = default;

    //----------//

    dVector3D <T>& operator=(const dVector3D <T>& VectorP) = default;

    //----------//

    void operator+=(const dVector3D <T>& VectorP) {
        x += VectorP.x;
        y += VectorP.y;
        z += VectorP.z;
    }
    dVector3D <T> operator+(const dVector3D <T>& VectorP) const {
        return dVector3D <T>(x + VectorP.x, y + VectorP.y, z + VectorP.z);
    }

    //----------//

    dVector3D <T> operator-() const {
        return dVector3D <T>(-x, -y, -z);
    }
    void operator-=(const dVector3D <T>& VectorP) {
        x -= VectorP.x;
        y -= VectorP.y;
        z -= VectorP.z;
    }
    dVector3D <T> operator-(const dVector3D <T>& VectorP) const {
        return dVector3D <T>(x - VectorP.x, y - VectorP.y, z - VectorP.z);
    }

    //----------//

    T operator*(const dVector3D <T>& VectorP) const {
        return x * VectorP.x + y * VectorP.y + z * VectorP.z;
    }
    template <typename NumberT> void operator*=(const NumberT NumberP) {
        x *= NumberP;
        y *= NumberP;
        z *= NumberP;
    }
    template <typename NumberT> friend dVector3D <T> operator*(const dVector3D <T>& VectorP, NumberT NumberP) {
        return dVector3D <T>(VectorP.x * NumberP, VectorP.y * NumberP, VectorP.z * NumberP);
    }
    template <typename NumberT> friend dVector3D <T> operator*(NumberT NumberP, const dVector3D <T>& VectorP) {
        return dVector3D <T>(VectorP.x * NumberP, VectorP.y * NumberP, VectorP.z * NumberP);
    }

    //----------//

    template <typename NumberT> void operator/=(const NumberT NumberP) {
        x /= NumberP;
        y /= NumberP;
        z /= NumberP;
    }
    template <typename NumberT> friend dVector3D <T> operator/(const dVector3D <T>& VectorP, NumberT NumberP) {
        return dVector3D <T>(VectorP.x / NumberP, VectorP.y / NumberP, VectorP.z / NumberP);
    }

    //----------//

    bool operator==(const dVector3D <T>& VectorP) const {
        return std::tie(x, y, z) == std::tie(VectorP.x, VectorP.y, VectorP.z);
    }
    bool operator!=(const dVector3D <T>& VectorP) const {
        return !(std::tie(x, y, z) == std::tie(VectorP.x, VectorP.y, VectorP.z));
    }

    //----------//

    friend std::ostream& operator<<(std::ostream& StreamP, const dVector3D <T>& VectorP) {
        StreamP << VectorP.x << " " << VectorP.y << " " << VectorP.z;
        return StreamP;
    }

    //----------//

    double Abs() const {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }
    float Absf() const {
        return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
    }
    T Abs2() const {
        return x * x + y * y + z * z;
    }

    //----------//

    dVector3D <double> Norm() const {
        dVector3D <double> TempL(0.0, 0.0, 0.0);
        double AbsL;

        AbsL = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

        TempL.x = x / AbsL;
        TempL.y = y / AbsL;
        TempL.z = z / AbsL;

        return TempL;
    }
    dVector3D <float> Normf() const {
        dVector3D <float> TempL(0.0, 0.0, 0.0);
        float AbsL;

        AbsL = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));

        TempL.x = x / AbsL;
        TempL.y = y / AbsL;
        TempL.z = z / AbsL;

        return TempL;
    }

    //----------//

    T Greatest() const {
        if (x >= y  && x >= z) {
            return x;
        } else if (y >= x  && y >= z) {
            return y;
        } else {
            return z;
        }
    }
    T Least() const {
        if (x <= y && x <= z) {
            return x;
        } else if (y <= x && y <= z) {
            return y;
        } else {
            return z;
        }
    }

    //----------//

    T& GetX() {
        return x;
    }
    T ConstGetX() const {
        return x;
    }

    T& GetY() {
        return y;
    }
    T ConstGetY() const {
        return y;
    }

    T& GetZ() {
        return z;
    }
    T ConstGetZ() const {
        return z;
    }
private:
    T x, y, z;
};
//-----------------------------//
#endif
