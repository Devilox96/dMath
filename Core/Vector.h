#ifndef DMATH_VECTOR_H
#define DMATH_VECTOR_H
//-----------------------------//
#include <iostream>
#include <algorithm>
#include <numeric>
#include <typeinfo>
#include <type_traits>
#include <complex>
#include <functional>
#include <cmath>
#include <typeinfo>
//-----------------------------//
#include "util.h"
//-----------------------------//
namespace dmath {

#ifndef DMATH_ONLY_VECTOR
    template<typename T, std::size_t M, std::size_t N>
    class Matrix;
#endif // DMATH_ONLY_VECTOR

template <typename T, std::size_t SizeT>
class Vector;

//-----------------------------//
template <typename T>
using Vector2D = Vector<T, 2>;
template <typename T>
using Vector3D = Vector<T, 3>;
template <typename T>
using Vector4D = Vector<T, 4>;
//-----------------------------//
template <typename T, std::size_t SizeT>
class Vector {
public:
    typedef T                                               value_type;
    typedef value_type*                                     pointer;
    typedef const value_type*                               const_pointer;
    typedef value_type&                   	                reference;
    typedef const value_type&             	                const_reference;
    typedef value_type*          		                    iterator;
    typedef const value_type*			                    const_iterator;
    typedef std::size_t                    	                size_type;
    typedef std::ptrdiff_t                   	            difference_type;
    typedef std::reverse_iterator<iterator>	                reverse_iterator;
    typedef std::reverse_iterator<const_iterator>           const_reverse_iterator;
public:
    constexpr
    explicit Vector(const std::array<T, SizeT>& tArr): mData(tArr) {}
    constexpr
    explicit Vector(std::array<T, SizeT>&& tArr): mData(std::move(tArr)) {}

    constexpr
    explicit Vector(const T& tVal) {
        fill(tVal);
    }

    constexpr
    Vector() = default;
    constexpr
    Vector(const Vector&) = default;
    constexpr
    Vector(Vector&&) noexcept = default;

    ~Vector() = default;

    //----------//

    constexpr
    Vector& operator=(const Vector&) = default;
    constexpr
    Vector& operator=(Vector&&) noexcept = default;

    // Addition --->
    constexpr
    Vector operator+() const {
        return *this;
    }

    constexpr
    Vector operator+(const Vector& tOther) const {
        Vector Temp;
        std::transform(cbegin(), cend(), tOther.begin(), Temp.begin(), std::plus<T>());
        return Temp;
    }

    constexpr
    friend Vector operator+(const Vector& tVec, const T& tScalar) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), RightUnaryPlus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend Vector operator+(const T& tScalar, const Vector& tVec) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryPlus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend Vector plusLambda(const T& tScalar, const Vector& tVec) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), [&tScalar](const T& val) { return tScalar + val; });
        return Temp;
    }

    constexpr
    friend Vector plusFunctor(const T& tScalar, const Vector& tVec) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryPlus<T>(tScalar));
        return Temp;
    }


    constexpr
    friend Vector plusForSub(const T& tScalar, const Vector& tVec) {
        Vector Temp;
        for (std::size_t i = 0; i < tVec.size(); ++i) {
            Temp[i] = tScalar + tVec[i] ;
        }
        return Temp;
    }

    constexpr
    friend Vector plusForPtr(const T& tScalar, const Vector& tVec) {
        Vector Temp;
        auto r = Temp.begin();
        auto f = tVec.cbegin();
        auto l = tVec.cend();
        for (;f != l; ++f, ++r) {
             *r = tScalar + *f;
        }
        return Temp;
    }

    constexpr
    Vector& operator+=(const Vector& tAdd) {
        std::transform(cbegin(), cend(), tAdd.cbegin(), begin(), std::plus<T>());
        return *this;
    }

    constexpr
    Vector& operator+=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryPlus<T>(tScalar));
        return *this;
    }
    // <--- Addition

    // Subtraction --->
    constexpr
    Vector operator-() const {
        Vector Temp;
        std::transform(cbegin(), cend(), Temp.begin(), std::negate<T>());
        return Temp;
    }

    constexpr
    Vector operator-(const Vector& tOther) const {
        Vector Temp;
        std::transform(cbegin(), cend(), tOther.begin(), Temp.begin(), std::minus<T>());
        return Temp;
    }

    constexpr
    friend Vector operator-(const Vector& tVec, const T& tScalar) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), RightUnaryMinus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend Vector operator-(const T& tScalar, const Vector& tVec) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryMinus<T>(tScalar));
        return Temp;
    }

    constexpr
    Vector& operator-=(const Vector& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::minus<T>());

        return *this;
    }

    constexpr
    Vector& operator-=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryMinus<T>(tScalar));

        return *this;
    }
    // <--- Subtraction

    // Multiplication --->
    friend T dot(const Vector& lhs, const Vector& rhs) {
        return std::inner_product(lhs.cbegin(), lhs.cend(), rhs.cbegin(), T(0));
    }

    constexpr
    Vector operator*(const Vector& tOther) const {
        Vector Temp;
        std::transform(cbegin(), cend(), tOther.begin(), Temp.begin(), std::multiplies<T>());
        return Temp;
    }


    friend Vector operator*(const Vector& tVec, const T& tScalar) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), RightUnaryMultiplies<T>(tScalar));
        return Temp;
    }

    friend Vector operator*(const T& tScalar, const Vector& tVec) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryMultiplies<T>(tScalar));
        return Temp;
    }

    constexpr
    Vector& operator*=(const Vector& tOther) {
        std::transform(cbegin(), cend(), tOther.begin(), begin(), std::multiplies<T>());
        return *this;
    }

    constexpr
    Vector& operator*=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryMultiplies<T>(tScalar));
        return *this;
    }
    // <--- Multiplication

    // Division --->
    constexpr
    Vector operator/(const Vector& tOther) const {
        Vector Temp;
        std::transform(cbegin(), cend(), tOther.begin(), Temp.begin(), std::divides<T>());
        return Temp;
    }


    friend Vector operator/(const Vector& tVec, const T& tScalar) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), RightUnaryDivides<T>(tScalar));
        return Temp;
    }

    friend Vector operator/(const T& tScalar, const Vector& tVec) {
        Vector Temp;
        std::transform(tVec.cbegin(), tVec.cend(), Temp.begin(), LeftUnaryDivides<T>(tScalar));
        return Temp;
    }

    constexpr
    Vector& operator/=(const Vector& tOther) {
        std::transform(cbegin(), cend(), tOther.begin(), begin(), std::divides<T>());
        return *this;
    }


    constexpr
    Vector& operator/=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryDivides<T>(tScalar));
        return *this;
    }
    // <--- Division

    //----------//

    constexpr
    bool operator==(const Vector& tOther) const {
        return mData == tOther.mData;
    }
    constexpr
    bool operator!=(const Vector& tOther) const {
        return mData != tOther.mData;
    }

    //----------//

    [[nodiscard]]
    constexpr
    double abs() const {
        return std::sqrt(std::inner_product(cbegin(), cend(), cbegin(), double(0)));
    }

    [[nodiscard]]
    constexpr
    float absf() const {
        return std::sqrt(std::inner_product(cbegin(), cend(), cbegin(), float(0)));
    }

    [[nodiscard]]
    constexpr
    T abs2() const {
        return std::inner_product(cbegin(), cend(), cbegin(), T(0));
    }

    //----------//
    constexpr
    static Vector zero() {
        return Vector{T(0)};
    }

    //----------//

    constexpr
    Vector<double, SizeT> norm() const {
        Vector<double, SizeT> Temp;
        std::transform(cbegin(), cend(), Temp.begin(), RightUnaryDivides<double>(abs()));
        return Temp;
    }

    constexpr
    Vector<float, SizeT> normf() const {
        Vector<float, SizeT> Temp;
        std::transform(cbegin(), cend(), Temp.begin(), RightUnaryDivides<float>(absf()));
        return Temp;
    }

    //----------//

    constexpr
    T max() const {
        return *std::max_element(begin(), end());
    }
    constexpr
    T min() const {
        return *std::min_element(begin(), end());
    }
    constexpr
    T sum() const {
        return std::accumulate(begin(), end(), T(0));
    }
    //----------//

    constexpr
    void fill(const T& tValue) {
        std::fill(begin(), end(), tValue);
    }

    constexpr
    T& operator[](std::size_t tInd) noexcept {
        return mData[tInd];
    }

    constexpr
    const T& operator[](std::size_t tInd) const noexcept {
        return mData[tInd];
    }

    constexpr
    T& at(std::size_t tInd) {
        return mData.at(tInd);
    }

    constexpr
    const T& at(std::size_t tInd) const {
        return mData.at(tInd);
    }

    [[nodiscard]]
    constexpr
    std::size_t size() const noexcept  {
        return SizeT;
    }

    constexpr
    const T* cbegin() const noexcept {
        return mData.cbegin();
    }

    constexpr
    const T* begin() const noexcept {
        return mData.begin();
    }

    constexpr
    T* begin() noexcept {
        return mData.begin();
    }

    constexpr
    const T* cend() const noexcept {
        return mData.cend();
    }

    constexpr
    const T* end() const noexcept {
        return mData.end();
    }

    constexpr
    T* end() noexcept {
        return mData.end();
    }

    constexpr
    const T* crbegin() const noexcept {
        return mData.crbegin();
    }

    constexpr
    const T* rbegin() const noexcept {
        return mData.rbegin();
    }

    constexpr
    T* rbegin() noexcept {
        return mData.rbegin();
    }

    constexpr
    const T* crend() const noexcept {
        return mData.crend();
    }

    constexpr
    const T* rend() const noexcept {
        return mData.rend();
    }

    constexpr
    T* rend() noexcept {
        return mData.rend();
    }

    constexpr
    void swap(Vector& tOther) {
        mData.swap(tOther.data);
    }

    //----------//

    friend std::ostream &operator<<(std::ostream &tStream, const Vector &tVec) {
        auto i = tVec.cbegin();
        tStream << "Vector<" << typeid(T).name() << ", " << SizeT << ">({" << *i++;
        for (; i != tVec.cend(); ++i) {
            tStream << ", " << *i;
        }
        return tStream << "})";
    }

    friend std::istream& operator>>(std::istream& tStream, Vector& tVec) {
        for(T& item : tVec.mData) {
            tStream >> item;
        }
        return tStream;
    }

    friend Vector <double, SizeT> pow(const Vector <T, SizeT>& tVec, double tDegree) {
        Vector <double, SizeT> Temp;

        for (int i = 0; i < tVec.size(); i++) {
            Temp[i] = pow(tVec[i], tDegree);
        }

        return Temp;
    }

    template <typename Q = T>
    constexpr explicit operator std::enable_if_t <SizeT == 1, Q> () const {
        return mData[0];
    }

#ifndef DMATH_ONLY_VECTOR
    explicit operator Matrix <T, 1, SizeT> () const {
        Matrix <T, 1, SizeT> Temp;
        std::copy(mData.cbegin(), mData.cend(), Temp.mData.begin());
        return Temp;
    }
    explicit operator std::enable_if <SizeT != 1, Matrix <T, SizeT, 1> > () const {
        Matrix <T, SizeT, 1> Temp;
        std::copy(mData.cbegin(), mData.cend(), Temp.mData.begin());
        return Temp;
    }
#endif // DMATH_ONLY_VECTOR



protected:
    std::array<T, SizeT> mData;
};

} //dmath

#endif // DMATH_VECTOR_H
