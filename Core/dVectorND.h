#ifndef DVECTORND_H
#define DVECTORND_H
//-----------------------------//
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>
//-----------------------------//
template <typename T, std::size_t SizeT>
class dVectorND;
//-----------------------------//
template <typename T>
using dVector2D = dVectorND <T, 2>;

template <typename T>
using dVector3D = dVectorND <T, 3>;

template <typename T>
using dVector4D = dVectorND <T, 4>;
//-----------------------------//
template <typename T, std::size_t SizeT>
class dVectorND {
public:
    template <typename ... Args>
    explicit dVectorND <T, SizeT>(Args ... tArgs) : mData({tArgs ...}), mSize(SizeT) {}
    dVectorND(const dVectorND <T, SizeT>& tCopy) : mData(tCopy.mData), mSize(SizeT) {}
    dVectorND(dVectorND <T, SizeT>&& tMove) noexcept : mData(std::move(tMove.mData)), mSize(SizeT) {}

    ~dVectorND() = default;

    //----------//

    dVectorND <T, SizeT>& operator=(const dVectorND <T, SizeT>& tCopy) {
        *this = dVectorND <T, SizeT>(tCopy);
        return *this;
    }
    dVectorND <T, SizeT>& operator=(dVectorND <T, SizeT>&& tMove) noexcept {
        *this = std::move(tMove);
        return *this;
    }

    //----------//

    void operator+=(const dVectorND <T, SizeT>& tAdd) {
        std::transform(mData.cbegin(), mData.cend(), tAdd.mData.cbegin(), mData.begin(), std::plus <T>());
    }
    template <typename NumberT>
    void operator+=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(),
                [&tNum](T tIter) { return tIter + T(tNum); });
    }
    dVectorND <T, SizeT> operator+(const dVectorND <T, SizeT>& tAdd) const {
        dVectorND <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), tAdd.mData.cbegin(), TempVec.mData.begin(), std::plus <T>());

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator+(const dVectorND <T, SizeT>& tVec, const NumberT& tNum) {
        dVectorND <T, SizeT> TempVec;
        std::transform(tVec.mData.cbegin(), tVec.mData.cend(), TempVec.mData.begin(),
                [&tNum](T tIter) { return tIter + T(tNum); });

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator+(NumberT tNum, const dVectorND <T, SizeT>& tVec) {
        return tVec + tNum;
    }

    //----------//

    dVectorND <T, SizeT> operator-() const {
        dVectorND <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), TempVec.mData.begin(), std::negate <T>());

        return TempVec;
    }
    void operator-=(const dVectorND <T, SizeT>& tVec) {
        std::transform(mData.cbegin(), mData.cend(), tVec.mData.cbegin(), mData.begin(), std::minus <T>());
    }
    template <typename NumberT>
    void operator-=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(),
                [&tNum](T tIter) { return tIter - T(tNum); });
    }
    dVectorND <T, SizeT> operator-(const dVectorND <T, SizeT>& tVec) const {
        dVectorND <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), tVec.mData.cbegin(), TempVec.mData.begin(), std::minus <T>());

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator-(const dVectorND <T, SizeT>& tVec, const NumberT& tNum) {
        dVectorND <T, SizeT> TempVec;
        std::transform(tVec.mData.cbegin(), tVec.mData.cend(), TempVec.mData.begin(),
                [&tNum](T tIter) { return tIter - T(tNum); });

        return TempVec;
    }

    //----------//

    T operator*(const dVectorND <T, SizeT> tVec) const {
        return std::inner_product(mData.cbegin(), mData.cend(), tVec.mData.cbegin(), T(0));
    }
    template <typename NumberT>
    void operator*=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), [&tNum](T tIter) { return tIter * T(tNum); });
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator*(const dVectorND <T, SizeT>& tVec, const NumberT&  tNum) {
        dVectorND <T, SizeT> TempVec;
        std::transform(tVec.mData.cbegin(), tVec.mData.cend(), TempVec.mData.begin(),
                [&tNum](T tIter) { return tIter * T(tNum); });

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator*(const NumberT& tNum, const dVectorND <T, SizeT>& tVec) {
        return tVec * tNum;
    }

    //----------//

    template <typename NumberT>
    void operator/=(const NumberT tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), [&tNum](T tIter) { return tIter / T(tNum); });
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator/(const dVectorND <T, SizeT>& tVec, const NumberT& tNum) {
        dVectorND <T, SizeT> TempVec;
        std::transform(tVec.mData.cbegin(), tVec.mData.cend(), TempVec.mData.begin(),
                [&tNum](T tIter) { return tIter / T(tNum); });

        return TempVec;
    }

    //----------//

    bool operator==(const dVectorND <T, SizeT>& tVec) const {
        return mData == tVec.mData;
    }
    bool operator!=(const dVectorND <T, SizeT>& tVec) const {
        return mData != tVec.mData;
    }

    //----------//

    T& operator[](std::size_t tInd) {
        return mData[tInd];
    }
    T operator[](std::size_t tInd) const {
        return mData[tInd];
    }

    //----------//

    friend std::ostream& operator<<(std::ostream& StreamP, const dVectorND <T, SizeT>& tVec) {
        for (std::size_t i = 0; i < tVec.mData.size(); i++) {
            StreamP << tVec.mData[i] << " ";
        }

        return StreamP;
    }
    friend std::istream& operator>>(std::istream& StreamP, dVectorND <T, SizeT>& tVec) {
        for (std::size_t i = 0; i < tVec.mData.size(); i++) {
            StreamP >> tVec.mData[i];
        }

        return StreamP;
    }

    //----------//

    double abs() const {
        double TempRes = 0.0;

        for (std::size_t i = 0; i < mSize; i++) {
            TempRes += mData[i] * mData[i];
        }

        return sqrt(TempRes);
    }
    float absf() const {
        float TempRes = 0.0;

        for (std::size_t i = 0; i < mSize; i++) {
            TempRes += mData[i] * mData[i];
        }

        return sqrtf(TempRes);
    }
    T abs2() const {
        T TempRes = 0.0;

        for (std::size_t i = 0; i < mSize; i++) {
            TempRes += mData[i] * mData[i];
        }

        return TempRes;
    }

    //----------//

    dVectorND <double, SizeT> norm() const {
        dVectorND <double, SizeT> TempVec(*this);
        double AbsValue = abs();

        TempVec /= AbsValue;

        return TempVec;
    }
    dVectorND <float, SizeT> normf() const {
        dVectorND <float, SizeT> TempVec(*this);
        float AbsValue = abs();

        TempVec /= AbsValue;

        return TempVec;
    }

    //----------//

    T max() const {
        return *std::max_element(mData.begin(), mData.end());
    }
    T min() const {
        return *std::min_element(mData.begin(), mData.end());
    }

    //----------//

    std::size_t getSize() const {
        return mSize;
    }

    //----------//

    template <typename EnableT = T>
    std::enable_if_t <SizeT == 3, dVectorND <EnableT, SizeT>> crossProduct(dVectorND <T, 3> tVec) const {
        dVectorND <T, 3> TempVec;

        TempVec.mData[0] = mData[1] * tVec.mData[2] - mData[2] * tVec.mData[1];
        TempVec.mData[1] = mData[2] * tVec.mData[0] - mData[0] * tVec.mData[2];
        TempVec.mData[2] = mData[0] * tVec.mData[1] - mData[1] * tVec.mData[0];

        return TempVec;
    }
    template <typename EnableT = T>
    friend std::enable_if_t <SizeT == 3, dVectorND <EnableT, SizeT>> crossProduct(
            const dVectorND <T, 3> tFirst,
            const dVectorND <T, 3> tSecond) {
        dVectorND <T, 3> TempVec;

        TempVec.mData[0] = tFirst.mData[1] * tSecond.mData[2] - tFirst.mData[2] * tSecond.mData[1];
        TempVec.mData[1] = tFirst.mData[2] * tSecond.mData[0] - tFirst.mData[0] * tSecond.mData[2];
        TempVec.mData[2] = tFirst.mData[0] * tSecond.mData[1] - tFirst.mData[1] * tSecond.mData[0];

        return TempVec;
    }
private:
    dVectorND() : mSize(SizeT) {}

    //----------//

    std::array <T, SizeT> mData;
    std::size_t mSize;
};
//-----------------------------//
#endif
