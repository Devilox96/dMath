#ifndef DVECTORND_H
#define DVECTORND_H
//-----------------------------//
#include <algorithm>
#include <numeric>
#include <functional>
//-----------------------------//
template <typename T, size_t SizeT>
class dVectorND {
public:
    template <typename ... Args>
    explicit dVectorND <T, SizeT>(Args ... tArgs) : mData({tArgs ...}), mSize(SizeT) {}
    dVectorND(const dVectorND <T, SizeT>& tVecToCopy) : mData(tVecToCopy.mData), mSize(SizeT) {}
    dVectorND(dVectorND <T, SizeT>&& tVecToMove) noexcept : mData(std::move(tVecToMove.mData)), mSize(SizeT) {}

    ~dVectorND() = default;

    //----------//

    dVectorND <T, SizeT>& operator=(const dVectorND <T, SizeT>& tVecToCopy) {
        *this = dVectorND <T, SizeT>(tVecToCopy);
        return *this;
    }
    dVectorND <T, SizeT>& operator=(dVectorND <T, SizeT>&& tVecToMove) noexcept {
        *this = dVectorND <T, SizeT>(tVecToMove);
        return *this;
    }

    //----------//

    void operator+=(const dVectorND <T, SizeT>& tVecToAdd) {
        std::transform(mData.cbegin(), mData.cend(), tVecToAdd.mData.cbegin(), mData.begin(), std::plus <T>());
    }
    template <typename NumberT>
    void operator+=(NumberT tNumToSubtract) {
        for (size_t i = 0; i < mSize; i++) {
            mData[i] += tNumToSubtract;
        }
    }
    dVectorND <T, SizeT> operator+(const dVectorND <T, SizeT>& tVecToAdd) const {
        dVectorND <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), tVecToAdd.mData.cbegin(), TempVec.mData.begin(), std::plus <T>());

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator+(const dVectorND <T, SizeT>& tVecToAddTo, NumberT tNumToAdd) {
        dVectorND <T, SizeT> TempVec;

        for (size_t i = 0; i < tVecToAddTo.mSize; i++) {
            TempVec.mData[i] = tVecToAddTo.mData[i] + tNumToAdd;
        }

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator+(NumberT tNumToAdd, const dVectorND <T, SizeT>& tVecToAddTo) {
        return tVecToAddTo + tNumToAdd;
    }

    //----------//

    dVectorND <T, SizeT> operator-() const {
        dVectorND <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), TempVec.mData.begin(), std::negate <T>());

        return TempVec;
    }
    void operator-=(const dVectorND <T, SizeT>& tVecToSubtract) {
        std::transform(mData.cbegin(), mData.cend(), tVecToSubtract.mData.cbegin(), mData.begin(), std::minus <T>());
    }
    template <typename NumberT>
    void operator-=(NumberT tNumToSubtract) {
        for (size_t i = 0; i < mSize; i++) {
            mData[i] -= tNumToSubtract;
        }
    }
    dVectorND <T, SizeT> operator-(const dVectorND <T, SizeT>& tVecToAdd) const {
        dVectorND <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), tVecToAdd.mData.cbegin(), TempVec.mData.begin(), std::minus <T>());

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator-(const dVectorND <T, SizeT>& tVecToSubtractFrom, NumberT tNumToSubtract) {
        dVectorND <T, SizeT> TempVec;

        for (size_t i = 0; i < tVecToSubtractFrom.mSize; i++) {
            TempVec.mData[i] = tVecToSubtractFrom.mData[i] - tNumToSubtract;
        }

        return TempVec;
    }

    //----------//

    T operator*(const dVectorND <T, SizeT> tVecToMultBy) const {
        return std::inner_product(mData.cbegin(), mData.cend(), tVecToMultBy.mData.cbegin(), static_cast <T>(0));
    }
    template <typename NumberT>
    void operator*=(const NumberT tNumToMultBy) {
        for (size_t i = 0; i < mSize; i++) {
            mData[i] *= tNumToMultBy;
        }
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator*(const dVectorND <T, SizeT>& tVecToMult, NumberT tNumToMultBy) {
        dVectorND <T, SizeT> TempVec;

        for (size_t i = 0; i < tVecToMult.mSize; i++) {
            TempVec.mData[i] = tVecToMult.mData[i] * tNumToMultBy;
        }

        return TempVec;
    }
    template <typename NumberT>
    friend dVectorND <T, SizeT> operator*(NumberT tNumToMultBy, const dVectorND <T, SizeT>& tVecToMult) {
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
    friend dVectorND <T, SizeT> operator/(const dVectorND <T, SizeT>& tVecToDiv, NumberT tNumToDivBy) {
        dVectorND <T, SizeT> TempVec;

        for (size_t i = 0; i < tVecToDiv.mSize; i++) {
            TempVec.mData[i] = tVecToDiv.mData[i] / tNumToDivBy;
        }

        return TempVec;
    }

    //----------//

    bool operator==(const dVectorND <T, SizeT>& tVecToComp) const {
        return mData == tVecToComp.mData;
    }
    bool operator!=(const dVectorND <T, SizeT>& tVecToComp) const {
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

    friend std::ostream& operator<<(std::ostream& StreamP, const dVectorND <T, SizeT>& tVecToPrint) {
        for (size_t i = 0; i < tVecToPrint.mData.size(); i++) {
            StreamP << tVecToPrint.mData[i] << " ";
        }

        return StreamP;
    }
    friend std::istream& operator>>(std::istream& StreamP, dVectorND <T, SizeT>& tVecToPrint) {
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

    size_t getSize() const {
        return mSize;
    }
private:
    dVectorND() : mSize(SizeT) {}

    //----------//

    std::array <T, SizeT> mData;
    size_t mSize;
};
//-----------------------------//
#endif
