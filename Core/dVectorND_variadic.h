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
    dVectorND_variadic() : mSize(SizeT) {}

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
    dVectorND_variadic <T, SizeT> operator+(const dVectorND_variadic <T, SizeT>& tVecToAdd) const {
        dVectorND_variadic <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), tVecToAdd.mData.cbegin(), TempVec.mData.begin(), std::plus <T>());

        return TempVec;
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
    dVectorND_variadic <T, SizeT> operator-(const dVectorND_variadic <T, SizeT>& tVecToAdd) const {
        dVectorND_variadic <T, SizeT> TempVec;
        std::transform(mData.cbegin(), mData.cend(), tVecToAdd.mData.cbegin(), TempVec.mData.begin(), std::minus <T>());

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
    friend dVectorND <T> operator/(const dVectorND_variadic <T, SizeT>& tVecToDiv, NumberT tNumToDivBy) {
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

    friend std::ostream& operator<<(std::ostream& StreamP, const dVectorND_variadic <T, SizeT>& tVecToPrint) {
        for (unsigned long i = 0; i < tVecToPrint.mData.size(); i++) {
            StreamP << tVecToPrint.mData[i] << " ";
        }

        return StreamP;
    }
//    template <typename NumberT>
//    friend dVectorND <T> operator*(const dVectorND <T>& VectorP, NumberT NumberP) {
//        dVectorND <T> TempVecL(VectorP.Size);
//
//        for (unsigned long i = 0; i < VectorP.Size; i++) {
//            TempVecL.Data[i] = VectorP.Data[i] * NumberP;
//        }
//
//        return TempVecL;
//    }
//    template <typename NumberT>
//    friend dVectorND <T> operator*(NumberT NumberP, const dVectorND <T>& VectorP) {
//        dVectorND <T> TempVecL(VectorP.Size);
//
//        for (unsigned long i = 0; i < VectorP.Size; i++) {
//            TempVecL.Data[i] = VectorP.Data[i] * NumberP;
//        }
//
//        return TempVecL;
//    }
private:
    std::array<T, SizeT> mData;
    size_t mSize;
};
//-----------------------------//
#endif
