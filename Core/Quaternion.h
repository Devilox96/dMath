#ifndef DQUATERNION_H
#define DQUATERNION_H
//-----------------------------//
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>
//-----------------------------//
#include "util.h"

namespace dmath {

template <typename T>
class Quaternion {
public:
    Quaternion() = default;
    explicit Quaternion(T tVal) : mData{tVal, tVal, tVal, tVal} {}
    Quaternion(T tW, T tX, T tY, T tZ) : mData{tW, tX, tY, tZ} {}
    Quaternion(const Quaternion&) = default;
    Quaternion(Quaternion&&) noexcept = default;
    
    ~Quaternion() = default;

    //----------//
    constexpr
    Quaternion& operator=(const Quaternion& tCopy) = default;
    constexpr
    Quaternion& operator=(Quaternion&& tMove) noexcept = default;

    //----------//
    constexpr
    void operator+=(const Quaternion& tAdd) {
        std::transform(mData.cbegin(), mData.cend(), tAdd.mData.cbegin(), mData.begin(), std::plus <T>());
    }
    constexpr
    void operator+=(const T& tScalar) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), RightUnaryPlus<T>(tScalar));
    }
    constexpr
    Quaternion operator+(const Quaternion& tAdd) const {
        Quaternion Temp;
        std::transform(mData.cbegin(), mData.cend(), tAdd.mData.cbegin(), Temp.mData.begin(), std::plus <T>());

        return Temp;
    }
    constexpr
    friend Quaternion operator+(const Quaternion& tQuat, const T& tScalar) {
        Quaternion Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(), RightUnaryPlus<T>(tScalar));

        return Temp;
    }
    constexpr
    friend Quaternion operator+(T tScalar, const Quaternion& tQuat) {
        Quaternion Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(), LeftUnaryPlus<T>(tScalar));

        return Temp;
    }

    //----------//
    constexpr
    Quaternion operator-() const {
        Quaternion Temp;
        std::transform(mData.cbegin(), mData.cend(), Temp.mData.begin(), std::negate <T>());

        return Temp;
    }
    constexpr
    void operator-=(const Quaternion& tQuat) {
        std::transform(mData.cbegin(), mData.cend(), tQuat.mData.cbegin(), mData.begin(), std::minus <T>());
    }
    constexpr
    void operator-=(const T& tScalar) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), RightUnaryMinus<T>(tScalar));
    }
    constexpr
    Quaternion operator-(const Quaternion& tQuat) const {
        Quaternion Temp;
        std::transform(mData.cbegin(), mData.cend(), tQuat.mData.cbegin(), Temp.mData.begin(), std::minus <T>());

        return Temp;
    }
    constexpr
    friend Quaternion operator-(const Quaternion& tQuat, const T& tScalar) {
        Quaternion Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(), RightUnaryMinus<T>(tScalar));

        return Temp;
    }
    constexpr
    friend Quaternion operator-(const T& tScalar, const Quaternion& tQuat) {
        Quaternion Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(), LeftUnaryMinus<T>(tScalar));

        return Temp;
    }

    //----------//
    constexpr
    Quaternion operator*(const Quaternion& tQuat) const {
        Quaternion TempQuat(0.0, 0.0, 0.0, 0.0);

        TempQuat.mData[0] = mData[0] * tQuat.mData[0] - mData[1] * tQuat.mData[1] -
                            mData[2] * tQuat.mData[2] - mData[3] * tQuat.mData[3];
        TempQuat.mData[1] = mData[0] * tQuat.mData[1] + mData[1] * tQuat.mData[0] +
                            mData[2] * tQuat.mData[3] - mData[3] * tQuat.mData[2];
        TempQuat.mData[2] = mData[0] * tQuat.mData[2] + mData[2] * tQuat.mData[0] +
                            mData[3] * tQuat.mData[1] - mData[1] * tQuat.mData[3];
        TempQuat.mData[3] = mData[0] * tQuat.mData[3] + mData[3] * tQuat.mData[0] +
                            mData[1] * tQuat.mData[2] - mData[2] * tQuat.mData[1];

        return TempQuat;
    }
    constexpr
    void operator*=(const Quaternion& tQuat) {
        Quaternion TempQuat(0.0, 0.0, 0.0, 0.0);

        TempQuat.mData[0] = mData[0] * tQuat.mData[0] - mData[1] * tQuat.mData[1] -
                            mData[2] * tQuat.mData[2] - mData[3] * tQuat.mData[3];
        TempQuat.mData[1] = mData[0] * tQuat.mData[1] + mData[1] * tQuat.mData[0] +
                            mData[2] * tQuat.mData[3] - mData[3] * tQuat.mData[2];
        TempQuat.mData[2] = mData[0] * tQuat.mData[2] + mData[2] * tQuat.mData[0] +
                            mData[3] * tQuat.mData[1] - mData[1] * tQuat.mData[3];
        TempQuat.mData[3] = mData[0] * tQuat.mData[3] + mData[3] * tQuat.mData[0] +
                            mData[1] * tQuat.mData[2] - mData[2] * tQuat.mData[1];

        *this = TempQuat;
    }
    constexpr
    void operator*=(const T& tScalar) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), RightUnaryMultiplies<T>(tScalar));
    }
    constexpr
    friend Quaternion operator*(const Quaternion& tQuat, const T&  tScalar) {
        Quaternion Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(), RightUnaryMultiplies<T>(tScalar));

        return Temp;
    }
    constexpr
    friend Quaternion operator*(const T& tScalar, const Quaternion& tQuat) {
        Quaternion Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(), LeftUnaryMultiplies<T>(tScalar));

        return Temp;
    }

    //----------//
    constexpr
    void operator/=(const T tScalar) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), RightUnaryDivides<T>(tScalar));
    }
    constexpr
    friend Quaternion operator/(const Quaternion& tQuat, const T& tScalar) {
        Quaternion Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(), RightUnaryDivides<T>(tScalar));

        return Temp;
    }
    constexpr
    friend Quaternion operator/(const T& tScalar, const Quaternion& tQuat) {
        Quaternion Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(), LeftUnaryDivides<T>(tScalar));

        return Temp;
    }

    //----------//
    constexpr
    bool operator==(const Quaternion& tQuat) const {
        return mData == tQuat.mData;
    }
    constexpr
    bool operator!=(const Quaternion& tQuat) const {
        return mData != tQuat.mData;
    }

    //----------//
    constexpr
    Quaternion conjugation() const {
        return Quaternion(mData[0], -mData[1], -mData[2], -mData[3]);
    }
    void reciprocal() {
        *this = (this -> conjugation()) /= abs2();
    }

    //----------//

    [[nodiscard]]
    double abs() const {
        double TempRes{0};

        for (std::size_t i = 0; i < 4; i++) {
            TempRes += mData[i] * mData[i];
        }

        return sqrt(TempRes);
    }
    [[nodiscard]]
    float absf() const {
        float TempRes{0};

        for (std::size_t i = 0; i < 4; i++) {
            TempRes += mData[i] * mData[i];
        }

        return sqrtf(TempRes);
    }
    [[nodiscard]]
    T abs2() const {
        T TempRes{0};

        for (std::size_t i = 0; i < 4; i++) {
            TempRes += mData[i] * mData[i];
        }

        return TempRes;
    }
private:
    std::array <T, 4> mData;
};

} // dmath
//-----------------------------//
#endif
