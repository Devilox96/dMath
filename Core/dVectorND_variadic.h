#ifndef DVECTORND_VARIADIC_H
#define DVECTORND_VARIADIC_H
//-----------------------------//
#include <algorithm>
#include <numeric>
#include <functional>
//-----------------------------//
template <typename T, size_t SizeT>
class dVectorND_variadic {
public:
    template <typename ... Args>
    explicit dVectorND_variadic <T, SizeT>(Args ... tArgs) : mData({tArgs ...}), mSize(SizeT) {}
    dVectorND_variadic(const dVectorND_variadic <T, SizeT>& tVecToCopy) : mData(tVecToCopy.mData), mSize(SizeT) {}
    dVectorND_variadic(dVectorND_variadic <T, SizeT>&& tVecToMove) noexcept : mData(std::move(tVecToMove.mData)), mSize(SizeT) {}

    ~dVectorND_variadic() = default;

    //----------//

    dVectorND_variadic <T, SizeT>& operator=(const dVectorND_variadic <T, SizeT>& tVecToCopy) {
        *this = dVectorND_variadic <T, SizeT>(tVecToCopy);
        return *this;
    }
    dVectorND_variadic <T, SizeT>& operator=(dVectorND_variadic <T, SizeT>&& tVecToMove) noexcept {
        *this = dVectorND_variadic <T, SizeT>(tVecToMove);
        return *this;
    }

    //----------//

    void operator+=(const dVectorND_variadic <T, SizeT>& tVecToAdd) {
        std::transform(mData.cbegin(), mData.cend(), tVecToAdd.mData.cbegin(), mData.begin(), std::plus <T>());
    }
    template <typename NumberT>
    void operator+=(NumberT tNumToSubtract) {
        for (size_t i = 0; i < mSize; i++) {
            mData[i] += tNumToSubtract;
        }
    }
    dVectorND_variadic <T, SizeT> operator+(const dVectorND_variadic <T, SizeT>& tVecToAdd) const {
        dVectorND_variadic <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), tVecToAdd.mData.cbegin(), TempVec.mData.begin(), std::plus <T>());

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND_variadic <T, SizeT> operator+(const dVectorND_variadic <T, SizeT>& tVecToAddTo, NumberT tNumToAdd) {
        dVectorND_variadic <T, SizeT> TempVec;

        for (size_t i = 0; i < tVecToAddTo.mSize; i++) {
            TempVec.mData[i] = tVecToAddTo.mData[i] + tNumToAdd;
        }

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND_variadic <T, SizeT> operator+(NumberT tNumToAdd, const dVectorND_variadic <T, SizeT>& tVecToAddTo) {
        return tVecToAddTo + tNumToAdd;
    }

    //----------//

    dVectorND_variadic <T, SizeT> operator-() const {
        dVectorND_variadic <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), TempVec.mData.begin(), std::negate <T>());

        return TempVec;
    }
    void operator-=(const dVectorND_variadic <T, SizeT>& tVecToSubtract) {
        std::transform(mData.cbegin(), mData.cend(), tVecToSubtract.mData.cbegin(), mData.begin(), std::minus <T>());
    }
    template <typename NumberT>
    void operator-=(NumberT tNumToSubtract) {
        for (size_t i = 0; i < mSize; i++) {
            mData[i] -= tNumToSubtract;
        }
    }
    dVectorND_variadic <T, SizeT> operator-(const dVectorND_variadic <T, SizeT>& tVecToAdd) const {
        dVectorND_variadic <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), tVecToAdd.mData.cbegin(), TempVec.mData.begin(), std::minus <T>());

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND_variadic <T, SizeT> operator-(const dVectorND_variadic <T, SizeT>& tVecToSubtractFrom, NumberT tNumToSubtract) {
        dVectorND_variadic <T, SizeT> TempVec;

        for (size_t i = 0; i < tVecToSubtractFrom.mSize; i++) {
            TempVec.mData[i] = tVecToSubtractFrom.mData[i] - tNumToSubtract;
        }

        return TempVec;
    }

    //----------//

    T operator*(const dVectorND_variadic <T, SizeT> tVecToMultBy) const {
        return std::inner_product(mData.cbegin(), mData.cend(), tVecToMultBy.mData.cbegin(), static_cast <T>(0));
    }
    template <typename NumberT>
    void operator*=(const NumberT tNumToMultBy) {
        for (size_t i = 0; i < mSize; i++) {
            mData[i] *= tNumToMultBy;
        }
    }
    template <typename NumberT>
    friend dVectorND_variadic <T, SizeT> operator*(const dVectorND_variadic <T, SizeT>& tVecToMult, NumberT tNumToMultBy) {
        dVectorND_variadic <T, SizeT> TempVec;

        for (size_t i = 0; i < tVecToMult.mSize; i++) {
            TempVec.mData[i] = tVecToMult.mData[i] * tNumToMultBy;
        }

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND_variadic <T, SizeT> operator*(NumberT tNumToMultBy, const dVectorND_variadic <T, SizeT>& tVecToMult) {
        return tVecToMult * tNumToMultBy;
    }

    //----------//

    template <typename NumberT>
    void operator/=(const NumberT tNumToDivBy) {
        for (size_t i = 0; i < mSize; i++) {
            mData[i] /= tNumToDivBy;
        }
    }
    template <typename NumberT>
    friend dVectorND_variadic <T, SizeT> operator/(const dVectorND_variadic <T, SizeT>& tVecToDiv, NumberT tNumToDivBy) {
        dVectorND_variadic <T, SizeT> TempVec;

        for (size_t i = 0; i < tVecToDiv.mSize; i++) {
            TempVec.mData[i] = tVecToDiv.mData[i] / tNumToDivBy;
        }

        return TempVec;
    }

    //----------//

    bool operator==(const dVectorND_variadic <T, SizeT>& tVecToComp) const {
        return mData == tVecToComp.mData;
    }
    bool operator!=(const dVectorND_variadic <T, SizeT>& tVecToComp) const {
        return mData != tVecToComp.mData;
    }

    //----------//

    T& operator[](size_t tIndex) {
        return mData[tIndex];
    }
    T operator[](size_t tIndex) const {
        return mData[tIndex];
    }

    //----------//

    friend std::ostream& operator<<(std::ostream& StreamP, const dVectorND_variadic <T, SizeT>& tVecToPrint) {
        for (size_t i = 0; i < tVecToPrint.mData.size(); i++) {
            StreamP << tVecToPrint.mData[i] << " ";
        }

        return StreamP;
    }
    friend std::istream& operator>>(std::istream& StreamP, dVectorND_variadic <T, SizeT>& tVecToPrint) {
        for (size_t i = 0; i < tVecToPrint.mData.size(); i++) {
            StreamP >> tVecToPrint.mData[i];
        }

        return StreamP;
    }

    //----------//

    double abs() const {
        double TempRes = 0.0;

        for (size_t i = 0; i < mSize; i++) {
            TempRes += mData[i] * mData[i];
        }

        return sqrt(TempRes);
    }
    float absf() const {
        float TempRes = 0.0;

        for (size_t i = 0; i < mSize; i++) {
            TempRes += mData[i] * mData[i];
        }

        return sqrtf(TempRes);
    }
    T abs2() const {
        T TempRes = 0.0;

        for (size_t i = 0; i < mSize; i++) {
            TempRes += mData[i] * mData[i];
        }

        return TempRes;
    }

    //----------//

    dVectorND_variadic <double, SizeT> norm() const {
        dVectorND_variadic <double, SizeT> TempVec(*this);
        double AbsValue = abs();

        TempVec /= AbsValue;

        return TempVec;
    }
    dVectorND_variadic <float, SizeT> normf() const {
        dVectorND_variadic <float, SizeT> TempVec(*this);
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

    size_t getSize() const {
        return mSize;
    }
private:
    dVectorND_variadic() : mSize(SizeT) {}

    //----------//

    std::array <T, SizeT> mData;
    size_t mSize;
};
//-----------------------------//
#endif
