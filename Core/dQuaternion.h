#ifndef DQUATERNION_H
#define DQUATERNION_H
//-----------------------------//
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>
//-----------------------------//
template <typename T>
class dQuaternion {
    dQuaternion() : mData(0, 0, 0, 0) {}
    dQuaternion(T tW, T tX, T tY, T tZ) : mData(tW, tX, tY, tZ) {}
    dQuaternion(const dQuaternion <T>& tCopy) : mData(tCopy.mData) {}
    dQuaternion(dQuaternion <T>&& tMove) noexcept : mData(std::move(tMove.mData)) {}
    
    ~dQuaternion <T>() = default;

    //----------//

    dQuaternion <T>& operator=(const dQuaternion <T>& tCopy) {
        *this = dQuaternion <T>(tCopy);
        return *this;
    }
    dQuaternion <T>& operator=(dQuaternion <T>&& tMove) noexcept {
        *this = std::move(tMove);
        return *this;
    }

    //----------//

    void operator+=(const dQuaternion <T>& tAdd) {
        std::transform(mData.cbegin(), mData.cend(), tAdd.mData.cbegin(), mData.begin(), std::plus <T>());
    }
    template <typename NumberT>
    void operator+=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(),
                       [&tNum](T tIter) { return tIter + T(tNum); });
    }
    dQuaternion <T> operator+(const dQuaternion <T>& tAdd) const {
        dQuaternion <T> Temp;
        std::transform(mData.cbegin(), mData.cend(), tAdd.mData.cbegin(), Temp.mData.begin(), std::plus <T>());

        return Temp;
    }
    template <typename NumberT>
    friend dQuaternion <T> operator+(const dQuaternion <T>& tQuat, const NumberT& tNum) {
        dQuaternion <T> Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(),
                       [&tNum](T tIter) { return tIter + T(tNum); });

        return Temp;
    }
    template <typename NumberT>
    friend dQuaternion <T> operator+(NumberT tNum, const dQuaternion <T>& tQuat) {
        return tQuat + tNum;
    }

    //----------//

    dQuaternion <T> operator-() const {
        dQuaternion <T> Temp;
        std::transform(mData.cbegin(), mData.cend(), Temp.mData.begin(), std::negate <T>());

        return Temp;
    }
    void operator-=(const dQuaternion <T>& tQuat) {
        std::transform(mData.cbegin(), mData.cend(), tQuat.mData.cbegin(), mData.begin(), std::minus <T>());
    }
    template <typename NumberT>
    void operator-=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(),
                       [&tNum](T tIter) { return tIter - T(tNum); });
    }
    dQuaternion <T> operator-(const dQuaternion <T>& tQuat) const {
        dQuaternion <T> Temp;
        std::transform(mData.cbegin(), mData.cend(), tQuat.mData.cbegin(), Temp.mData.begin(), std::minus <T>());

        return Temp;
    }
    template <typename NumberT>
    friend dQuaternion <T> operator-(const dQuaternion <T>& tQuat, const NumberT& tNum) {
        dQuaternion <T> Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(),
                       [&tNum](T tIter) { return tIter - T(tNum); });

        return Temp;
    }

    //----------//

    dQuaternion <T> operator*(const dQuaternion <T>& tQuat) const {
        dQuaternion <T> TempQuat(0.0, 0.0, 0.0, 0.0);

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
    void operator*=(const dQuaternion <T>& tQuat) {
        dQuaternion <T> TempQuat(0.0, 0.0, 0.0, 0.0);

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
    template <typename NumberT>
    void operator*=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), [&tNum](T tIter) { return tIter * T(tNum); });
    }
    template <typename NumberT>
    friend dQuaternion <T> operator*(const dQuaternion <T>& tQuat, const NumberT&  tNum) {
        dQuaternion <T> Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(),
                       [&tNum](T tIter) { return tIter * T(tNum); });

        return Temp;
    }
    template <typename NumberT>
    friend dQuaternion <T> operator*(const NumberT& tNum, const dQuaternion <T>& tQuat) {
        return tQuat * tNum;
    }

    //----------//

    template <typename NumberT>
    void operator/=(const NumberT tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), [&tNum](T tIter) { return tIter / T(tNum); });
    }
    template <typename NumberT>
    friend dQuaternion <T> operator/(const dQuaternion <T>& tQuat, const NumberT& tNum) {
        dQuaternion <T> Temp;
        std::transform(tQuat.mData.cbegin(), tQuat.mData.cend(), Temp.mData.begin(),
                       [&tNum](T tIter) { return tIter / T(tNum); });

        return Temp;
    }

    //----------//

    bool operator==(const dQuaternion <T>& tQuat) const {
        return mData == tQuat.mData;
    }
    bool operator!=(const dQuaternion <T>& tQuat) const {
        return mData != tQuat.mData;
    }

    //----------//

    dQuaternion <T> conjugation() const {
        return dQuaternion <T>(mData[0], -mData[1], -mData[2], -mData[3]);
    }
    void reciprocal() {
        *this = (this -> conjugation()) /= abs2();
    }

    //----------//

    double abs() const {
        double TempRes = 0.0;

        for (std::size_t i = 0; i < 4; i++) {
            TempRes += mData[i] * mData[i];
        }

        return sqrt(TempRes);
    }
    float absf() const {
        float TempRes = 0.0;

        for (std::size_t i = 0; i < 4; i++) {
            TempRes += mData[i] * mData[i];
        }

        return sqrtf(TempRes);
    }
    T abs2() const {
        T TempRes = 0.0;

        for (std::size_t i = 0; i < 4; i++) {
            TempRes += mData[i] * mData[i];
        }

        return TempRes;
    }
private:
    std::array <T, 4> mData;
};
//-----------------------------//
#endif
