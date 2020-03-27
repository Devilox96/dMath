#ifndef DMATH_DVECTOR_H
#define DMATH_DVECTOR_H
//-----------------------------//
#include <iostream>
#include <algorithm>
#include <numeric>
#include <typeinfo>
#include <type_traits>
#include <complex>
#include <functional>
#include <cmath>


template<typename T, std::size_t M, std::size_t N>
class dMatrix;

//-----------------------------//
template <typename T, std::size_t SizeT>
class dVector;
//-----------------------------//
template <typename T>
using dVector2D = dVector <T, 2>;

template <typename T>
using dVector3D = dVector <T, 3>;

template <typename T>
using dVector4D = dVector <T, 4>;
//-----------------------------//

template <typename T, std::size_t SizeT>
class dVector {
public:
    template <typename ... Args>
    constexpr dVector <T, SizeT>(Args ... tArgs) : mData({tArgs ...}) {}

    constexpr dVector <T, SizeT>() = default;
    constexpr dVector(const dVector <T, SizeT>& tCopy) = default;
    dVector(dVector <T, SizeT>&& tMove) = default;

    ~dVector() = default;

    //----------//

    constexpr dVector <T, SizeT>& operator=(const dVector <T, SizeT>& tCopy) = default;
    constexpr dVector <T, SizeT>& operator=(dVector <T, SizeT>&& tMove) = default;

    //----------//
    dVector <T, SizeT> operator+(const dVector <T, SizeT>& tAdd) const {
        dVector <T, SizeT> Temp;
        std::transform(mData.cbegin(), mData.cend(), tAdd.mData.cbegin(), Temp.mData.begin(), std::plus <T>());
        return Temp;
    }


    template <typename NumberT>
    friend dVector <T, SizeT> operator+(const dVector <T, SizeT>& tVec, const NumberT& tNum) {
        dVector <T, SizeT> Temp;
        std::transform(tVec.mData.cbegin(), tVec.mData.cend(), Temp.mData.begin(),
                       [&tNum](const T& tIter) { return tIter + toT(tNum); });
        return Temp;
    }
    template <typename NumberT>
    friend dVector <T, SizeT> operator+(const NumberT& tNum, const dVector <T, SizeT>& tVec) {
        return tVec + tNum;
    }

    void operator+=(const dVector <T, SizeT>& tAdd) {
        std::transform(mData.cbegin(), mData.cend(), tAdd.mData.cbegin(), mData.begin(), std::plus <T>());
    }
    template <typename NumberT>
    void operator+=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(),
                       [&tNum](const T& tIter) { return tIter + toT(tNum); });
    }


    //----------//
    dVector <T, SizeT> operator-() const {
        dVector <T, SizeT> Temp;
        std::transform(mData.cbegin(), mData.cend(), Temp.mData.begin(), std::negate <T>());
        return Temp;
    }


    dVector <T, SizeT> operator-(const dVector <T, SizeT>& tOther) const {
        dVector <T, SizeT> Temp;
        std::transform(mData.cbegin(), mData.cend(), tOther.mData.cbegin(), Temp.mData.begin(), std::minus <T>());
        return Temp;
    }

    template <typename NumberT>
    friend dVector <T, SizeT> operator-(const dVector <T, SizeT>& tVec, const NumberT& tNum) {
        dVector <T, SizeT> Temp;
        std::transform(tVec.mData.cbegin(), tVec.mData.cend(), Temp.mData.begin(),
                       [&tNum](const T& tIter) { return tIter - toT(tNum); });

        return Temp;
    }

    void operator-=(const dVector <T, SizeT>& tOther) {
        std::transform(mData.cbegin(), mData.cend(), tOther.mData.cbegin(), mData.begin(), std::minus <T>());
    }
    template <typename NumberT>
    void operator-=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(),
                       [&tNum](const T& tIter) { return tIter - toT(tNum); });
    }

    //----------//
    friend T dot(const dVector <T, SizeT>& tFirst, const dVector <T, SizeT>& tSecond) {
        return std::inner_product(tFirst.mData.cbegin(), tFirst.mData.cend(), tSecond.mData.cbegin(), toT(0));
    }

    T operator*(const dVector <T, SizeT>& tOther) const {
        return dot(*this, tOther);
    }

    template <typename NumberT>
    friend dVector <T, SizeT> operator*(const dVector <T, SizeT>& tVec, const NumberT&  tNum) {
        dVector <T, SizeT> Temp;
        std::transform(tVec.mData.cbegin(), tVec.mData.cend(), Temp.mData.begin(),
                       [&tNum](const T& tIter) { return tIter * toT(tNum); });

        return Temp;
    }
    template <typename NumberT>
    friend dVector <T, SizeT> operator*(const NumberT& tNum, const dVector <T, SizeT>& tVec) {
        return tVec * tNum;
    }

    template <typename NumberT>
    void operator*=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), [&tNum](const T& tIter) { return tIter * toT(tNum); });
    }

    void operator*=(const dVector<T, 1>& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), [&tNum](const T& tIter) { return tIter * toT(tNum[0]); });
    }


    //----------//
    template <typename NumberT>
    friend dVector <T, SizeT> operator/(const dVector <T, SizeT>& tVec, const NumberT& tNum) {
        dVector <T, SizeT> Temp;
        std::transform(tVec.mData.cbegin(), tVec.mData.cend(), Temp.mData.begin(),
                       [&tNum](const T& tIter) { return tIter / toT(tNum); });

        return Temp;
    }

    template <typename NumberT>
    void operator/=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), [&tNum](const T& tIter) { return tIter / toT(tNum); });
    }

    //----------//

    bool operator==(const dVector <T, SizeT>& tOther) const {
        return mData == tOther.mData;
    }
    bool operator!=(const dVector <T, SizeT>& tOther) const {
        return mData != tOther.mData;
    }

    //----------//

    dVector <T, SizeT + 1> addForward(const T& tValue) const {
        dVector <T, SizeT + 1> Temp;
        Temp[0] = tValue;
        std::copy(mData.cbegin(), mData.cend(), (Temp.mData.begin() + 1));
        return Temp;
    }

    dVector <T, SizeT - 1> delForward() const{
        dVector <T, SizeT - 1> Temp;
        std::copy((mData.cbegin() + 1), mData.cend(), Temp.mData.begin());
        return Temp;
    }

    //----------//

    [[nodiscard]] double abs() const {
        return sqrt(std::inner_product(mData.cbegin(), mData.cend(), mData.cbegin(), double(0)));
    }

    [[nodiscard]] float absf() const {
        return sqrtf(std::inner_product(mData.cbegin(), mData.cend(), mData.cbegin(), float(0)));
    }

    T abs2() const {
        return std::inner_product(mData.cbegin(), mData.cend(), mData.cbegin(), toT(0));
    }

    //----------//

    static constexpr dVector <T, SizeT> null() {
        dVector <T, SizeT> Temp;
        Temp.fill(toT(0));
        return Temp;
    }

    //----------//

    dVector <double, SizeT> norm() const {
        dVector <double, SizeT> Temp;
        double s = abs();
        std::transform(mData.cbegin(), mData.cend(), Temp.mData.begin(),
                       [&s](const T& tIter) { return tIter / s; });
        return Temp;
    }

    dVector <float, SizeT> normf() const {
        dVector <float, SizeT> Temp;
        float s = absf();
        std::transform(mData.cbegin(), mData.cend(), Temp.mData.begin(),
                       [&s](const T& tIter) { return tIter / s; });
        return Temp;
    }

    //----------//
    constexpr T max() const {
        return *std::max_element(mData.begin(), mData.end());
    }
    constexpr T min() const {
        return *std::min_element(mData.begin(), mData.end());
    }

    //----------//

    void fill(const T& tValue) {
        std::fill(mData.begin(), mData.end(), tValue);
    }

    constexpr T &operator[](std::size_t tInd) noexcept {
        return mData[tInd];
    }

    constexpr const T &operator[](std::size_t tInd) const noexcept {
        return mData[tInd];
    }

    constexpr T &at(std::size_t tInd) {
        return mData.at(tInd);
    }

    constexpr const T &at(std::size_t tInd) const {
        return mData.at(tInd);
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept  {
        return SizeT;
    }

    constexpr const T* cbegin() const noexcept {
        return mData.cbegin();
    }

    constexpr T* begin() noexcept {
        return mData.begin();
    }

    constexpr const T* cend() const noexcept {
        return mData.cend();
    }

    constexpr T* end() noexcept {
        return mData.end();
    }

    void swap(dVector<T, SizeT>& tOther) {
        mData.swap(tOther.data);
    }

    //----------//

    friend std::ostream &operator<<(std::ostream &tStream, const dVector <T, SizeT> &tVec) {
        auto i = tVec.cbegin();
        tStream << "dVector<" << SizeT << ">(" << *i++;
        for (; i != tVec.cend(); ++i) {
            tStream << ", " << *i;
        }
        return tStream << ')';
    }

    friend std::istream& operator>>(std::istream& tStream, dVector <T, SizeT>& tVec) {
        for(T& item : tVec.mData) {
            tStream >> item;
        }
        return tStream;
    }

    template <typename Q = T>
    constexpr operator std::enable_if_t <SizeT == 1, Q> () const {
        return mData[0];
    }

    operator dMatrix <T, 1, SizeT> () const {
        dMatrix <T, 1, SizeT> Temp;
        std::copy(mData.cbegin(), mData.cend(), Temp.mData.begin()->begin());
        return Temp;
    }
    operator std::enable_if <SizeT != 1, dMatrix <T, SizeT, 1> > () const {
        dMatrix <T, SizeT, 1> Temp;
        std::copy(mData.cbegin(), mData.cend(), Temp.mData.begin()->begin());
        return Temp;
    }


protected:
    std::array <T, SizeT> mData;

    template <typename NumberT>
    static constexpr T toT(const NumberT& tNum) {
        if constexpr (std::is_arithmetic_v <NumberT>) {
            return static_cast<T>(tNum);
        } else if (std::is_same_v <NumberT, T>) {
            return tNum[0];
        } else if (std::is_same_v <NumberT, dVector<T, 1> >) {
            return tNum[0];
        } else if (std::is_same_v <NumberT, dMatrix<T, 1, 1> >) {
            return tNum[0];
        }
    }

    template <typename, std::size_t, std::size_t>
    friend class dMatrix;
};

#endif // DMATH_DVECTOR_H
