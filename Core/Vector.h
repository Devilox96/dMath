#ifndef DMATH_VECTOR_H
#define DMATH_VECTOR_H
//-----------------------------//
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <typeinfo>
//-----------------------------//
#include "util.h"
//-----------------------------//
namespace dmath {

#ifndef DMATH_ONLY_VECTOR

template<typename T, std::size_t M, std::size_t N>
class Matrix;

#endif // DMATH_ONLY_VECTOR

template<typename T, std::size_t SizeT>
class Vector;

//-----------------------------//
template<typename T>
using Vector2D = Vector<T, 2>;
template<typename T>
using Vector3D = Vector<T, 3>;
template<typename T>
using Vector4D = Vector<T, 4>;

//-----------------------------//
template<typename T, std::size_t SizeT>
class Vector {
public:
    using value_type             = T;
    using pointer                = value_type*;
    using const_pointer          = const pointer;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = value_type*;
    using const_iterator         = const value_type*;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reverse_iterator       = std::reverse_iterator<iterator> ;
    using const_reverse_iterator = std::reverse_iterator<const_iterator> ;
public:
    using constIterator          = const_iterator;
    using reverseIterator        = reverse_iterator;
    using constReverseIterator   = const_reverse_iterator;

public:
    constexpr
    explicit Vector(const std::array<T, SizeT>& tArray) : mData(tArray) {}

    constexpr
    explicit Vector(std::array<T, SizeT>&& tArray) : mData(std::move(tArray)) {}

    constexpr
    explicit Vector(const T& tValue) {
        fill(tValue);
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
        std::transform(cbegin(), cend(), tOther.cbegin(), Temp.begin(), std::plus<T>());
        return Temp;
    }

    constexpr friend
    Vector operator+(const Vector& tVector, const T& tScalar) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(), RightUnaryPlus<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Vector operator+(const T& tScalar, const Vector& tVector) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(), LeftUnaryPlus<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Vector plusLambda(const T& tScalar, const Vector& tVector) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(),
                       [&tScalar](const T& val) { return tScalar + val; });
        return Temp;
    }

    constexpr friend
    Vector plusFunctor(const T& tScalar, const Vector& tVector) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(), LeftUnaryPlus<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Vector plusForSub(const T& tScalar, const Vector& tVector) {
        Vector Temp;
        for (std::size_t i = 0; i < tVector.size(); ++i) {
            Temp[i] = tScalar + tVector[i];
        }
        return Temp;
    }

    constexpr friend
    Vector plusForPtr(const T& tScalar, const Vector& tVector) {
        Vector Temp;
        auto r = Temp.begin();
        auto f = tVector.cbegin();
        auto l = tVector.cend();
        for (; f != l; ++f, ++r) {
            *r = tScalar + *f;
        }
        return Temp;
    }

    constexpr
    Vector& operator+=(const Vector& tVector) {
        std::transform(cbegin(), cend(), tVector.cbegin(), begin(), std::plus<T>());
        return *this;
    }

    constexpr
    Vector& operator+=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryPlus<T>{tScalar});
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
        std::transform(cbegin(), cend(), tOther.cbegin(), Temp.begin(), std::minus<T>());
        return Temp;
    }

    constexpr friend
    Vector operator-(const Vector& tVector, const T& tScalar) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(), RightUnaryMinus<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Vector operator-(const T& tScalar, const Vector& tVector) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(), LeftUnaryMinus<T>{tScalar});
        return Temp;
    }

    constexpr
    Vector& operator-=(const Vector& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::minus<T>());

        return *this;
    }

    constexpr
    Vector& operator-=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryMinus<T>{tScalar});

        return *this;
    }
    // <--- Subtraction

    // Multiplication --->
    constexpr
    T dot(const Vector& tOther) const {
        return std::inner_product(cbegin(), cend(), tOther.cbegin(), static_cast<T>(0));
    }

    constexpr friend
    T dot(const Vector& lhs, const Vector& rhs) {
        return lhs.dot(rhs);
    }

    constexpr
    Vector operator*(const Vector& tOther) const {
        Vector Temp;
        std::transform(cbegin(), cend(), tOther.cbegin(), Temp.begin(), std::multiplies<T>());
        return Temp;
    }

    constexpr friend
    Vector operator*(const Vector& tVector, const T& tScalar) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(), RightUnaryMultiplies<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Vector operator*(const T& tScalar, const Vector& tVector) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(), LeftUnaryMultiplies<T>{tScalar});
        return Temp;
    }

    constexpr
    Vector& operator*=(const Vector& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::multiplies<T>());
        return *this;
    }

    constexpr
    Vector& operator*=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryMultiplies<T>{tScalar});
        return *this;
    }
    // <--- Multiplication

    // Division --->
    constexpr
    Vector operator/(const Vector& tOther) const {
        Vector Temp;
        std::transform(cbegin(), cend(), tOther.cbegin(), Temp.begin(), std::divides<T>());
        return Temp;
    }
    constexpr friend
    Vector operator/(const Vector& tVector, const T& tScalar) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(), RightUnaryDivides<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Vector operator/(const T& tScalar, const Vector& tVector) {
        Vector Temp;
        std::transform(tVector.cbegin(), tVector.cend(), Temp.begin(), LeftUnaryDivides<T>{tScalar});
        return Temp;
    }

    constexpr
    Vector& operator/=(const Vector& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::divides<T>());
        return *this;
    }

    constexpr
    Vector& operator/=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryDivides<T>{tScalar});
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
        return std::sqrt(std::inner_product(cbegin(), cend(), cbegin(), static_cast<double>(0)));
    }

    [[nodiscard]]
    constexpr
    float absf() const {
        return std::sqrt(std::inner_product(cbegin(), cend(), cbegin(), static_cast<float>(0)));
    }

    [[nodiscard]]
    constexpr
    T abs2() const {
        return std::inner_product(cbegin(), cend(), cbegin(), static_cast<T>(0));
    }

    //----------//

    constexpr static
    Vector zero() {
        return Vector{static_cast<T>(0)};
    }

    constexpr static
    Vector identity() {
        return Vector{static_cast<T>(1)};
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
        return std::accumulate(begin(), end(), static_cast<T>(0));
    }
    //----------//

    constexpr
    void fill(const T& tValue) {
        std::fill(begin(), end(), tValue);
    }

    constexpr
    const T& operator[](std::size_t tIndex) const noexcept {
        return const_cast<Vector&>(*this)[tIndex];
    }

    constexpr
    T& operator[](std::size_t tIndex) noexcept {
        return mData[tIndex];
    }

    constexpr
    const T& at(std::size_t tIndex) const {
        return const_cast<Vector&>(*this).at(tIndex);
    }

    constexpr
    T& at(std::size_t tIndex) {
        return mData.at(tIndex);
    }

    [[nodiscard]]
    constexpr
    std::size_t size() const noexcept {
        return SizeT;
    }

    constexpr
    constIterator cbegin() const noexcept {
        return const_cast<Vector&>(*this).begin();
    }

    constexpr
    constIterator begin() const noexcept {
        return const_cast<Vector&>(*this).begin();
    }

    constexpr
    iterator begin() noexcept {
        return mData.begin();
    }

    constexpr
    constIterator cend() const noexcept {
        return const_cast<Vector&>(*this).end();
    }

    constexpr
    constIterator end() const noexcept {
        return const_cast<Vector&>(*this).end();
    }

    constexpr
    iterator end() noexcept {
        return mData.end();
    }

    constexpr
    constReverseIterator crbegin() const noexcept {
        return const_cast<Vector&>(*this).rbegin();
    }

    constexpr
    constReverseIterator rbegin() const noexcept {
        return const_cast<Vector&>(*this).rbegin();
    }

    constexpr
    reverseIterator rbegin() noexcept {
        return mData.rbegin();
    }

    constexpr
    constReverseIterator crend() const noexcept {
        return const_cast<Vector&>(*this).rend();
    }

    constexpr
    constReverseIterator rend() const noexcept {
        return const_cast<Vector&>(*this).rend();
    }

    constexpr
    reverseIterator rend() noexcept {
        return mData.rend();
    }

    constexpr
    void swap(Vector& tOther) noexcept(std::is_nothrow_swappable_v<T>) {
        mData.swap(tOther.data);
    }

    //----------//

    friend std::ostream& operator<<(std::ostream& tStream, const Vector& tVector) {
        auto i = tVector.cbegin();
        tStream << "Vector<" << typeid(T).name() << ", " << SizeT << ">({" << *i++;
        for (; i != tVector.cend(); ++i) {
            tStream << ", " << *i;
        }
        return tStream << "})";
    }

    friend std::istream& operator>>(std::istream& tStream, Vector& tVector) {
        for (T& item : tVector.mData) {
            tStream >> item;
        }
        return tStream;
    }

    constexpr friend
    Vector<double, SizeT> pow(const Vector<T, SizeT>& tVector, double tDegree) {
        Vector<double, SizeT> Temp;

        for (int i = 0; i < tVector.size(); i++) {
            Temp[i] = pow(tVector[i], tDegree);
        }

        return Temp;
    }

    template<typename Q = T>
    constexpr
    explicit operator std::enable_if_t<SizeT == 1, Q>() const {
        return mData[0];
    }

#ifndef DMATH_ONLY_VECTOR

    constexpr
    explicit operator std::enable_if<SizeT != 1, Matrix<T, 1, SizeT>>()  const {
        Matrix<T, 1, SizeT> Temp;
        std::copy(mData.cbegin(), mData.cend(), Temp.mData.begin());
        return Temp;
    }

    constexpr
    explicit operator std::enable_if<SizeT != 1, Matrix<T, SizeT, 1>>() const {
        Matrix<T, SizeT, 1> Temp;
        std::copy(mData.cbegin(), mData.cend(), Temp.mData.begin());
        return Temp;
    }

#endif // DMATH_ONLY_VECTOR

    constexpr static
    Vector generate(std::function<T(void)> func) {
        Vector Temp;
        std::generate(Temp.begin(), Temp.end(), func);
        return Temp;
    }


protected:
    std::array<T, SizeT> mData;
};

} //dmath

#endif // DMATH_VECTOR_H
