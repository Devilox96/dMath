#ifndef DVECTOR2D_H
#define DVECTOR2D_H
//-----------------------------//
#include <iostream>
#include <cmath>
#include <tuple>
//-----------------------------//
template <typename T>
class dVector2D {
public:
    dVector2D <T>() : x(0), y(0) {}
    dVector2D <T>(T xP, T yP) : x(xP), y(yP) {}
    dVector2D <T>(const dVector2D <T>& CopyVectorP) : x(CopyVectorP.x), y(CopyVectorP.y) {}
    ~dVector2D <T>() = default;

    //----------//

    dVector2D <T>& operator=(const dVector2D <T>& VectorP) = default;

    //----------//

    void operator+=(const dVector2D <T>& VectorP) {
        x += VectorP.x;
        y += VectorP.y;
    }
    dVector2D <T> operator+(const dVector2D <T>& VectorP) const {
        return dVector2D <T>(x + VectorP.x, y + VectorP.y);
    }

    //----------//

    dVector2D <T> operator-() {
        return dVector2D <T>(-x, -y);
    }
    void operator-=(const dVector2D <T>& VectorP) {
        x -= VectorP.x;
        y -= VectorP.y;
    }
    dVector2D <T> operator-(const dVector2D <T>& VectorP) const {
        return dVector2D <T>(x - VectorP.x, y - VectorP.y);
    }

    //----------//

    T operator*(const dVector2D <T>& VectorP) const {
        return x * VectorP.x + y * VectorP.y;
    }
    template <typename NumberT> void operator*=(const NumberT NumberP) {
        x *= NumberP;
        y *= NumberP;
    }
    template <typename NumberT> friend dVector2D <T> operator*(const dVector2D <T>& VectorP, NumberT NumberP) {
        return dVector2D <T>(VectorP.x * NumberP, VectorP.y * NumberP);
    }
    template <typename NumberT> friend dVector2D <T> operator*(NumberT NumberP, const dVector2D <T>& VectorP) {
        return dVector2D <T>(VectorP.x * NumberP, VectorP.y * NumberP);
    }

    //----------//

    template <typename NumberT> void operator/=(const NumberT NumberP) {
        x /= NumberP;
        y /= NumberP;
    }
    template <typename NumberT> friend dVector2D <T> operator/(const dVector2D <T>& VectorP, NumberT NumberP) {
        return dVector2D <T>(VectorP.x / NumberP, VectorP.y / NumberP);
    }

    //----------//

    bool operator==(const dVector2D <T>& VectorP) const {
        return std::tie(x, y) == std::tie(VectorP.x, VectorP.y);
    }
    bool operator!=(const dVector2D <T>& VectorP) const {
        return !(std::tie(x, y) == std::tie(VectorP.x, VectorP.y));
    }

    //----------//

    friend std::ostream& operator<<(std::ostream& StreamP, const dVector2D <T>& VectorP) {
        StreamP << VectorP.x << " " << VectorP.y;
        return StreamP;
    }

    //----------//

    double Abs() const {
        return sqrt(pow(x, 2) + pow(y, 2));
    }
    float Absf() const {
        return sqrtf(powf(x, 2) + powf(y, 2));
    }
    T Abs2() const {
        return x * x + y * y;
    }

    //----------//

    dVector2D <double> Norm() const {
        dVector2D <double> TempL(0.0, 0.0);
        double AbsL;

        AbsL = sqrt(pow(x, 2) + pow(y, 2));

        TempL.x = x / AbsL;
        TempL.y = y / AbsL;

        return TempL;
    }
    dVector2D <float> Normf() const {
        dVector2D <float> TempL(0.0, 0.0);
        float AbsL;

        AbsL = sqrtf(powf(x, 2) + powf(y, 2));

        TempL.x = x / AbsL;
        TempL.y = y / AbsL;

        return TempL;
    }

    //----------//

    T Greatest() const {
        if (x >= y) {
            return x;
        } else {
            return y;
        }
    }
    T Least() const {
        if (x <= y) {
            return x;
        } else {
            return y;
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
private:
    T x, y;
};
//-----------------------------//
#endif
