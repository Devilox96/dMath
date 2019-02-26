#ifndef DQUATERNION_H
#define DQUATERNION_H
//-----------------------------//
#include <iostream>
#include <cmath>
#include <tuple>
//-----------------------------//
template <typename T>
class dQuaternion {
    T w, x, y, z;

    dQuaternion <T>() : w(0), x(0), y(0), z(0) {}
    dQuaternion <T>(T wP, T xP, T yP, T zP) : w(wP), x(xP), y(yP), z(zP) {}
    dQuaternion <T>(const dQuaternion <T>& CopyQuatP) : x(CopyQuatP.x), y(CopyQuatP.y), z(CopyQuatP.z), w(CopyQuatP.w) {}
    ~dQuaternion <T>() = default;

    //----------//

    dQuaternion <T>& operator=(const dQuaternion <T>& QuatP) = default;

    //----------//

    void operator+=(const dQuaternion <T>& QuatP) {
        w += QuatP.w;
        x += QuatP.x;
        y += QuatP.y;
        z += QuatP.z;
    }
    dQuaternion <T> operator+(const dQuaternion <T>& QuatP) const {
        return dQuaternion <T>(w + QuatP.w, x + QuatP.x, y + QuatP.y, z + QuatP.z);
    }

    //----------//

    dQuaternion <T> operator-() const {
        return dQuaternion <T>(-w, -x, -y, -z);
    }
    void operator-=(const dQuaternion <T>& QuatP) {
        w -= QuatP.w;
        x -= QuatP.x;
        y -= QuatP.y;
        z -= QuatP.z;
    }
    dQuaternion <T> operator-(const dQuaternion <T>& QuatP) const {
        return dQuaternion <T>(w - QuatP.w, x - QuatP.x, y - QuatP.y, z - QuatP.z);
    }

    //----------//

    dQuaternion <T> operator*(const dQuaternion <T>& QuatP) const {
        dQuaternion <T> QuatL(0.0, 0.0, 0.0, 0.0);

        QuatL.w = w * QuatP.w - x * QuatP.x - y * QuatP.y - z * QuatP.z;
        QuatL.x = w * QuatP.x + x * QuatP.w + y * QuatP.z - z * QuatP.y;
        QuatL.y = w * QuatP.y + y * QuatP.w + z * QuatP.x - x * QuatP.z;
        QuatL.z = w * QuatP.z + z * QuatP.w + x * QuatP.y - y * QuatP.x;

        return QuatL;
    }
    void operator*=(const dQuaternion <T>& QuatP) {
        dQuaternion <T> QuatL(0.0, 0.0, 0.0, 0.0);

        QuatL.w = w * QuatP.w - x * QuatP.x - y * QuatP.y - z * QuatP.z;
        QuatL.x = w * QuatP.x + x * QuatP.w + y * QuatP.z - z * QuatP.y;
        QuatL.y = w * QuatP.y + y * QuatP.w + z * QuatP.x - x * QuatP.z;
        QuatL.z = w * QuatP.z + z * QuatP.w + x * QuatP.y - y * QuatP.x;

        w = QuatL.w;
        x = QuatL.x;
        y = QuatL.y;
        z = QuatL.z;
    }
    template <typename NumberT> void operator*=(const NumberT NumP) {
        w *= NumP;
        x *= NumP;
        y *= NumP;
        z *= NumP;
    }
    template <typename NumberT> friend dQuaternion <T> operator*(const dQuaternion <T>& QuatP, NumberT NumberP) {
        return dQuaternion <T>(QuatP.w * NumberP, QuatP.x * NumberP, QuatP.y * NumberP, QuatP.z * NumberP);
    }
    template <typename NumberT> friend dQuaternion <T> operator*(NumberT NumberP, const dQuaternion <T>& QuatP) {
        return dQuaternion <T>(QuatP.w * NumberP, QuatP.x * NumberP, QuatP.y * NumberP, QuatP.z * NumberP);
    }

    //----------//

    template <typename NumberT> void operator/=(const NumberT NumP) {
        w /= NumP;
        x /= NumP;
        y /= NumP;
        z /= NumP;
    }
    template <typename NumberT> friend dQuaternion <T> operator/(const dQuaternion <T>& QuatP, NumberT NumberP) {
        return dQuaternion <T>(QuatP.w / NumberP, QuatP.x / NumberP, QuatP.y / NumberP, QuatP.z / NumberP);
    }

    //----------//

    bool operator==(const dQuaternion <T>& QuatP) const {
        return std::tie(w, x, y, z) == std::tie(QuatP.w, QuatP.x, QuatP.y, QuatP.z);
    }
    bool operator!=(const dQuaternion <T>& QuatP) const {
        return !(std::tie(w, x, y, z) == std::tie(QuatP.w, QuatP.x, QuatP.y, QuatP.z));
    }

    //----------//

    dQuaternion <T> Conjugation() const {
        return dQuaternion <T>(w, -x, -y, -z);
    }
    void Reciprocal() {
        dQuaternion <T> QuatL;
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

    //----------//

    double Abs() const {
        return sqrt(pow(w, 2) + pow(x, 2) + pow(y, 2) + pow(z, 2));
    }
    float Absf() const {
        return sqrtf(powf(w, 2) + powf(x, 2) + powf(y, 2) + powf(z, 2));
    }
    T Abs2() const {
        return w * w + x * x + y * y + z * z;
    }
};
//-----------------------------//
#endif
