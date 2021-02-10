#ifndef DVECTORND_H
#define DVECTORND_H
//-----------------------------//
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>

#include "util.h"
//-----------------------------//
template <typename T, std::size_t SizeT>
class dVector;
//-----------------------------//
template <typename T>
using dVector2D = dVector<T, 2>;

template <typename T>
using dVector3D = dVector<T, 3>;

template <typename T>
using dVector4D = dVector<T, 4>;
//-----------------------------//
template <typename T, std::size_t SizeT>
class dVector {
public:
    typedef T 	    			                    value_type;
    typedef value_type*			                    pointer;
    typedef const value_type*                       const_pointer;
    typedef value_type&                   	        reference;
    typedef const value_type&             	        const_reference;
    typedef value_type*          		            iterator;
    typedef const value_type*			            const_iterator;
    typedef std::size_t                    	        size_type;
    typedef std::ptrdiff_t                   	    difference_type;
    typedef std::reverse_iterator<iterator>	        reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
public:
    dVector(const std::array<T, SizeT>& tArr): mData(tArr) {}


    dVector(const_reference tVal) {
        fill(tVal);
    }

    dVector() = default;
    dVector(const dVector& tCopy) = default;
    dVector(dVector&& tMove) noexcept = default;

    ~dVector() = default;

    //----------//

    dVector& operator=(const dVector& tCopy) = default;
    constexpr dVector& operator=(dVector&& tMove) noexcept = default;

    // Addition --->
    constexpr
    dVector operator+() const {
        return *this;
    }

    constexpr
    dVector operator+(const dVector& tAdd) const {
        dVector Temp(*this);
        Temp += tAdd;
        return Temp;
    }
    template <typename NumberT>
    constexpr friend
    dVector operator+(const dVector& tVec, const NumberT& tNum) {
        dVector Temp(tVec);
        Temp += tNum;
        return Temp;
    }
    template <typename NumberT>
    constexpr
    friend dVector operator+(const NumberT& tNum, const dVector& tVec) {
        return tVec + tNum;
    }

    constexpr
    dVector& operator+=(const dVector& tAdd) {
        std::transform(mData.cbegin(), mData.cend(), tAdd.mData.cbegin(), mData.begin(), std::plus<T>());

        return *this;
    }
    template <typename NumberT>
    constexpr
    dVector& operator+=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), RightUnaryPlus<T>(tNum));

        return *this;
    }
    // <--- Addition

    // Subtraction --->
    constexpr
    dVector operator-() const {
        dVector Temp;
        std::transform(mData.cbegin(), mData.cend(), Temp.mData.begin(), std::negate<T>());

        return Temp;
    }

    constexpr
    dVector operator-(const dVector& tOther) const {
        dVector Temp(*this);
        Temp -= tOther;

        return Temp;
    }

    template <typename NumberT>
    constexpr
    friend dVector operator-(const dVector& tVec, const NumberT& tNum) {
        dVector Temp(tVec);
        Temp -= tNum;
        return Temp;
    }

    constexpr
    dVector& operator-=(const dVector& tOther) {
        std::transform(mData.cbegin(), mData.cend(), tOther.mData.cbegin(), mData.begin(), std::minus<T>());

        return *this;
    }
    template <typename NumberT>
    constexpr
    dVector& operator-=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), RightUnaryMinus<T>(tNum));

        return *this;
    }
    // <--- Subtraction

    // Multiplication --->
    friend value_type dot(const dVector& lhs, const dVector& rhs) {
        return std::inner_product(lhs.mData.cbegin(), lhs.mData.cend(), rhs.mData.cbegin(), T(0));
    }

    constexpr
    dVector operator*(const dVector& tOther) const {
        dVector Temp(*this);
        Temp *= tOther;

        return Temp;
    }

    template <typename NumberT>
    friend dVector operator*(const dVector& tVec, const NumberT& tNum) {
        dVector Temp(tVec);
        return Temp *= tNum;
    }
    template <typename NumberT>
    friend dVector operator*(const NumberT& tNum, const dVector& tVec) {
        return tVec * tNum;
    }

    constexpr
    dVector& operator*=(const dVector& tOther) {
        std::transform(mData.cbegin(), mData.cend(), tOther.mData.begin(), mData.begin(), std::multiplies<T>());

        return *this;
    }

    template <typename NumberT>
    constexpr
    dVector& operator*=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), RightUnaryMultiplies<T>(tNum));

        return *this;
    }
    // <--- Multiplication

    // Division --->
    constexpr
    dVector operator/(const dVector& tOther) const {
        dVector Temp(*this);
        Temp /= tOther;

        return Temp;
    }

    template <typename NumberT>
    friend dVector operator/(const NumberT& tNum, const dVector& tVec) {
        dVector Temp;
        std::transform(tVec.mData.cbegin(), tVec.mData.cend(), Temp.mData.begin(), LeftUnaryDivides<T>(tNum));

        return Temp;
    }

    template <typename NumberT>
    friend dVector operator/(const dVector& tVec, const NumberT& tNum) {
        dVector Temp(tVec);
        return Temp /= tNum;
    }

    constexpr
    dVector& operator/=(const dVector& tOther) {
        std::transform(mData.cbegin(), mData.cend(), tOther.mData.begin(), mData.begin(), std::divides<T>());

        return *this;
    }

    template <typename NumberT>
    constexpr
    dVector& operator/=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), RightUnaryDivides<T>(tNum));

        return *this;
    }
    // <--- Division

    //----------//

    constexpr
    bool operator==(const dVector& tOther) const {
        return mData == tOther.mData;
    }
    constexpr
    bool operator!=(const dVector& tOther) const {
        return mData != tOther.mData;
    }

    //----------//

    [[nodiscard]] constexpr
    double abs() const {
        return sqrt(std::inner_product(mData.cbegin(), mData.cend(), mData.cbegin(), double(0)));
    }

    [[nodiscard]] constexpr
    float absf() const {
        return sqrtf(std::inner_product(mData.cbegin(), mData.cend(), mData.cbegin(), float(0)));
    }

    constexpr
    value_type abs2() const {
        return std::inner_product(mData.cbegin(), mData.cend(), mData.cbegin(), T(0));
    }

    //----------//
    constexpr
    static  dVector null() {
        dVector Temp;
        Temp.fill(T(0));
        return Temp;
    }

    template<class Func>
    constexpr dVector apply() {
        dVector Temp;
        Temp.fill(T(0));
        return Temp;
    }

    //----------//

    constexpr
    dVector<double, SizeT> norm() const {
        dVector<double, SizeT> Temp;
        double s = abs();
        std::transform(mData.cbegin(), mData.cend(), Temp.mData.begin(),
                       [&s](const T& tIter) { return tIter / s; });
        return Temp;
    }

    constexpr
    dVector<float, SizeT> normf() const {
        dVector<float, SizeT> Temp;
        float s = absf();
        std::transform(mData.cbegin(), mData.cend(), Temp.mData.begin(),
                       [&s](const T& tIter) { return tIter / s; });
        return Temp;
    }

    //----------//

    constexpr
    value_type max() const {
        return *std::max_element(mData.begin(), mData.end());
    }
    constexpr
    value_type min() const {
        return *std::min_element(mData.begin(), mData.end());
    }
    constexpr
    value_type sum() const {
        return std::accumulate(mData.begin(), mData.end(), T(0));
    }
    //----------//

    constexpr
    void fill(const T& tValue) {
        std::fill(mData.begin(), mData.end(), tValue);
    }

    constexpr
    reference operator[](size_type tInd) noexcept {
        return mData[tInd];
    }

    constexpr
    const_reference operator[](size_type tInd) const noexcept {
        return mData[tInd];
    }

    constexpr
    reference at(size_type tInd) {
        return mData.at(tInd);
    }

    constexpr
    const_reference at(size_type tInd) const {
        return mData.at(tInd);
    }

    [[nodiscard]] constexpr
    size_type size() const noexcept  {
        return SizeT;
    }

    constexpr
    const_iterator cbegin() const noexcept {
        return mData.cbegin();
    }

    constexpr
    const_iterator begin() const noexcept {
        return mData.begin();
    }

    constexpr
    iterator begin() noexcept {
        return mData.begin();
    }

    constexpr
    const_iterator cend() const noexcept {
        return mData.cend();
    }

    constexpr
    const_iterator end() const noexcept {
        return mData.end();
    }

    constexpr
    iterator end() noexcept {
        return mData.end();
    }

    constexpr
    const_iterator crbegin() const noexcept {
        return mData.crbegin();
    }

    constexpr
    const_iterator rbegin() const noexcept {
        return mData.rbegin();
    }

    constexpr
    iterator rbegin() noexcept {
        return mData.rbegin();
    }

    constexpr
    const_iterator crend() const noexcept {
        return mData.crend();
    }

    constexpr
    const_iterator rend() const noexcept {
        return mData.rend();
    }

    constexpr
    iterator rend() noexcept {
        return mData.rend();
    }

    constexpr
    void swap(dVector& tOther) {
        mData.swap(tOther.data);
    }

    //----------//

    friend std::ostream &operator<<(std::ostream &tStream, const dVector &tVec) {
        auto i = tVec.cbegin();
        tStream << "Vector<" << SizeT << ">(" << *i++;
        for (; i != tVec.cend(); ++i) {
            tStream << ", " << *i;
        }
        return tStream << ')';
    }

    friend std::istream& operator>>(std::istream& tStream, dVector& tVec) {
        for(T& item : tVec.mData) {
            tStream >> item;
        }
        return tStream;
    }

protected:
    std::array<T, SizeT> mData;
};

#endif
