#ifndef DVECTORND_H
#define DVECTORND_H
//-----------------------------//
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>
#include <typeinfo>

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


    dVector(const T& tVal) {
        fill(tVal);
    }

    constexpr
    dVector() = default;
    constexpr
    dVector(const dVector& tCopy) = default;
    constexpr
    dVector(dVector&& tMove) noexcept = default;

    ~dVector() = default;

    //----------//

    constexpr
    dVector& operator=(const dVector& tCopy) = default;
    constexpr
    dVector& operator=(dVector&& tMove) noexcept = default;

    // Addition --->
    constexpr
    dVector operator+() const {
        return *this;
    }

    constexpr
    dVector operator+(const dVector& tOther) const {
        dVector Temp;
        std::transform(cbegin(), cend(), tOther.begin(), Temp.begin(), std::plus<T>());
        return Temp;
    }

    constexpr friend
    dVector operator+(const dVector& tVec, const T& tScalar) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), RightUnaryPlus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend dVector operator+(const T& tScalar, const dVector& tVec) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryPlus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend dVector plusLambda(const T& tScalar, const dVector& tVec) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), [&tScalar](const T& val) { return tScalar + val; });
        return Temp;
    }

    constexpr
    friend dVector plusFunctor(const T& tScalar, const dVector& tVec) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryPlus<T>(tScalar));
        return Temp;
    }


    constexpr
    friend dVector plusForSub(const T& tScalar, const dVector& tVec) {
        dVector Temp;
        for (std::size_t i = 0; i < tVec.size(); ++i) {
            Temp[i] = tScalar + tVec[i] ;
        }
        return Temp;
    }

    constexpr
    friend dVector plusForPtr(const T& tScalar, const dVector& tVec) {
        dVector Temp;
        auto r = Temp.begin();
        auto f = tVec.cbegin();
        auto l = tVec.cend();
        for (;f != l; ++f, ++r) {
             *r = tScalar + *f;
        }
        return Temp;
    }

    constexpr
    dVector& operator+=(const dVector& tAdd) {
        std::transform(cbegin(), cend(), tAdd.cbegin(), begin(), std::plus<T>());
        return *this;
    }

    constexpr
    dVector& operator+=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryPlus<T>(tScalar));
        return *this;
    }
    // <--- Addition

    // Subtraction --->
    constexpr
    dVector operator-() const {
        dVector Temp;
        std::transform(cbegin(), cend(), Temp.begin(), std::negate<T>());
        return Temp;
    }

    constexpr
    dVector operator-(const dVector& tOther) const {
        dVector Temp;
        std::transform(cbegin(), cend(), tOther.begin(), Temp.begin(), std::minus<T>());
        return Temp;
    }

    constexpr
    friend dVector operator-(const dVector& tVec, const T& tScalar) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), RightUnaryMinus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend dVector operator-(const T& tScalar, const dVector& tVec) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryMinus<T>(tScalar));
        return Temp;
    }

    constexpr
    dVector& operator-=(const dVector& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::minus<T>());

        return *this;
    }

    constexpr
    dVector& operator-=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryMinus<T>(tScalar));

        return *this;
    }
    // <--- Subtraction

    // Multiplication --->
    friend value_type dot(const dVector& lhs, const dVector& rhs) {
        return std::inner_product(lhs.cbegin(), lhs.cend(), rhs.cbegin(), T(0));
    }

    constexpr
    dVector operator*(const dVector& tOther) const {
        dVector Temp;
        std::transform(cbegin(), cend(), tOther.begin(), Temp.begin(), std::multiplies<T>());
        return Temp;
    }


    friend dVector operator*(const dVector& tVec, const T& tScalar) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), RightUnaryMultiplies<T>(tScalar));
        return Temp;
    }

    friend dVector operator*(const T& tScalar, const dVector& tVec) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryMultiplies<T>(tScalar));
        return Temp;
    }

    constexpr
    dVector& operator*=(const dVector& tOther) {
        std::transform(cbegin(), cend(), tOther.begin(), begin(), std::multiplies<T>());
        return *this;
    }

    constexpr
    dVector& operator*=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryMultiplies<T>(tScalar));
        return *this;
    }
    // <--- Multiplication

    // Division --->
    constexpr
    dVector operator/(const dVector& tOther) const {
        dVector Temp;
        std::transform(cbegin(), cend(), tOther.begin(), Temp.begin(), std::divides<T>());
        return Temp;
    }


    friend dVector operator/(const dVector& tVec, const T& tScalar) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), RightUnaryDivides<T>(tScalar));
        return Temp;
    }

    friend dVector operator/(const T& tScalar, const dVector& tVec) {
        dVector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryDivides<T>(tScalar));
        return Temp;
    }

    constexpr
    dVector& operator/=(const dVector& tOther) {
        std::transform(cbegin(), cend(), tOther.begin(), begin(), std::divides<T>());
        return *this;
    }


    constexpr
    dVector& operator/=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryDivides<T>(tScalar));
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
        return std::sqrt(std::inner_product(cbegin(), cend(), cbegin(), double(0)));
    }

    [[nodiscard]] constexpr
    float absf() const {
        return std::sqrt(std::inner_product(cbegin(), cend(), cbegin(), float(0)));
    }

    [[nodiscard]] constexpr
    value_type abs2() const {
        return std::inner_product(cbegin(), cend(), cbegin(), T(0));
    }

    //----------//
    constexpr
    static dVector zero() {
        return dVector{T(0)};
    }

    //----------//

    constexpr
    dVector<double, SizeT> norm() const {
        dVector<double, SizeT> Temp;
        std::transform(cbegin(), cend(), Temp.begin(), RightUnaryDivides<double>(abs()));
        return Temp;
    }

    constexpr
    dVector<float, SizeT> normf() const {
        dVector<float, SizeT> Temp;
        std::transform(cbegin(), cend(), Temp.begin(), RightUnaryDivides<float>(absf()));
        return Temp;
    }

    //----------//

    constexpr
    value_type max() const {
        return *std::max_element(begin(), end());
    }
    constexpr
    value_type min() const {
        return *std::min_element(begin(), end());
    }
    constexpr
    value_type sum() const {
        return std::accumulate(begin(), end(), T(0));
    }
    //----------//

    constexpr
    void fill(const T& tValue) {
        std::fill(begin(), end(), tValue);
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
        tStream << "dVector<" << typeid(T).name() << ", " << SizeT << ">({" << *i++;
        for (; i != tVec.cend(); ++i) {
            tStream << ", " << *i;
        }
        return tStream << "})";
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
