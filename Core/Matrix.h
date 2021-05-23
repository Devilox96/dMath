//
// Created by skinderev on 24.10.2019.
//

#ifndef DMATH_MATRIX_H
#define DMATH_MATRIX_H

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
#ifndef DMATH_ONLY_MATRIX

template<typename T, std::size_t SizeT>
class Vector;

#endif // DMATH_ONLY_MATRIX

template<typename T, std::size_t M, std::size_t N>
class Matrix;

template<typename T, std::size_t M>
using sMatrix = Matrix<T, M, M>;

enum class Iterate {
    Row, Col, Val
};

template<typename T, std::size_t M, std::size_t N>
class Matrix {
protected:
// TODO make SFINAE

    template<typename ReturnType = void>
    using is_square_t = std::enable_if_t<M == N, ReturnType>;

    template<typename ReturnType = void>
    using is_scalar_t = std::enable_if_t<M == 1 && N == 1, ReturnType>;

#ifndef DMATH_ONLY_MATRIX
    template<typename ReturnType = Vector<T, M * N>>
    using is_vector_t = std::enable_if_t<M == 1 || N == 1, ReturnType>;
#endif // DMATH_ONLY_MATRIX

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
    template<typename Type, typename UnqualifiedType = std::remove_cv_t<Type>>
    struct iteratorCol {
        using iterator_category     = std::forward_iterator_tag;
        using value_type            = UnqualifiedType;
        using difference_type       = std::ptrdiff_t;
        using pointer               = Type*;
        using reference             = Type&;

        constexpr
        explicit iteratorCol() : mPtr(nullptr) {}

        constexpr
        explicit iteratorCol(Type* ptr) : mPtr(ptr) {}

        constexpr
        reference operator*() const { return *mPtr; }

        constexpr
        pointer operator->() { return mPtr; }

        constexpr
        iteratorCol& operator++() {
            mPtr += N;
            return *this;
        }

        constexpr
        iteratorCol operator++(int) {
            auto tmp(*this);
            ++(*this);
            return tmp;
        }

        template<typename Other>
        constexpr
        bool operator==(const iteratorCol<Other>& rhs) { return mPtr == rhs.mPtr; }

        template<typename Other>
        constexpr
        bool operator!=(const iteratorCol<Other>& rhs) { return mPtr != rhs.mPtr; }

        constexpr
        bool operator==(pointer& rhs) { return mPtr == rhs; }

        constexpr
        bool operator!=(pointer& rhs) { return mPtr != rhs; }

        constexpr
        operator iteratorCol<const Type>() const {
            return iteratorCol<const Type>(mPtr);
        }

        constexpr
        explicit operator pointer() const {
            return mPtr;
        }

        friend
        std::ostream& operator<<(std::ostream& tStream, const iteratorCol& iter) {
            return tStream << iter.mPtr;
        }

    private:
        pointer mPtr;
    };

    struct columnView {
        using columnIterator = iteratorCol<T>;
        using constColumnIterator = iteratorCol<const T>;

        constexpr
        explicit columnView(T* ptr) : mPtr(ptr) {}

        constexpr
        const columnView& operator*() const { return *this; }

        constexpr
        columnView *operator->() { return this; }

        constexpr
        columnView& operator++() {
            ++mPtr;
            return *this;
        }

        constexpr
        columnView operator++(int) {
            auto tmp(*this);
            ++(*this);
            return tmp;
        }

        constexpr friend
        bool operator==(const columnView& a, const columnView& b) { return a.mPtr == b.mPtr; };

        constexpr friend
        bool operator!=(const columnView& a, const columnView& b) { return a.mPtr != b.mPtr; };

        constexpr
        constColumnIterator begin() const {
            return constColumnIterator(mPtr);
        }

        constexpr
        columnIterator begin() {
            return columnIterator(mPtr);
        }

        constexpr
        constColumnIterator end() const {
            return constColumnIterator(mPtr + N * M);
        }

        constexpr
        columnIterator end() {
            return columnIterator(mPtr + N * M);
        }

#ifndef DMATH_ONLY_MATRIX

        constexpr
        explicit operator Vector<T, M>() const {
            Vector<T, M> Temp;
            std::copy_n(begin(), M, Temp.begin());
            return Temp;
        }

#endif // DMATH_ONLY_MATRIX

    private:
        T* mPtr;
    };

public:
    constexpr
    explicit Matrix(const std::array<std::array<T, N>, M>& tArray) : mData(tArray) {}

    constexpr
    explicit Matrix(std::array<std::array<T, N>, M>&& tArray) : mData(std::move(tArray)) {}


    constexpr
    explicit Matrix(const T& tValue) {
        fill(tValue);
    }

    constexpr
    Matrix() = default;

    constexpr
    Matrix(const Matrix& ) = default;

    constexpr
    Matrix(Matrix&&) noexcept = default;

    ~Matrix() = default;

    constexpr
    Matrix& operator=(const Matrix& ) = default;

    constexpr
    Matrix& operator=(Matrix&&) noexcept = default;

    constexpr
    Matrix operator+() const {
        return *this;
    }

    constexpr friend
    Matrix operator+(const Matrix& tMatrix, const T& tScalar) {
        Matrix Temp;
        std::transform(tMatrix.cbegin(), tMatrix.cend(), Temp.begin(), RightUnaryPlus<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Matrix operator+(const T& tScalar, const Matrix& tMatrix) {
        Matrix Temp;
        std::transform(tMatrix.cbegin(), tMatrix.cend(), Temp.begin(), LeftUnaryPlus<T>{tScalar});
        return Temp;
    }

    constexpr
    Matrix operator+(const Matrix& tOther) const {
        Matrix Temp;
        std::transform(cbegin(), cend(), tOther.cbegin(), Temp.begin(), std::plus<T>());
        return Temp;
    }

    constexpr
    Matrix& operator+=(const Matrix& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::plus<T>());
        return *this;
    }

    constexpr
    Matrix& operator+=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryPlus<T>{tScalar});
        return *this;
    }

    constexpr
    Matrix operator-() const {
        Matrix Temp;
        std::transform(cbegin(), cend(), Temp.begin(), std::negate<T>());
        return Temp;
    }

    constexpr friend
    Matrix operator-(const Matrix& tMatrix, const T& tScalar) {
        Matrix Temp;
        std::transform(tMatrix.cbegin(), tMatrix.cend(), Temp.begin(), RightUnaryMinus<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Matrix operator-(const T& tScalar, const Matrix& tMatrix) {
        Matrix Temp;
        std::transform(tMatrix.cbegin(), tMatrix.cend(), Temp.begin(), LeftUnaryMinus<T>{tScalar});
        return Temp;
    }

    constexpr
    Matrix operator-(const Matrix& tOther) const {
        Matrix Temp;
        std::transform(cbegin(), cend(), tOther.cbegin(), Temp.begin(), std::minus<T>());
        return Temp;
    }

    constexpr
    Matrix& operator-=(const Matrix& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::minus<T>());
        return *this;
    }

    constexpr
    Matrix& operator-=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryMinus<T>{tScalar});
        return *this;
    }


    constexpr
    Matrix operator*(const Matrix& tOther) {
        Matrix Temp;
        std::transform(cbegin(), cend(), tOther.cbegin(), Temp.begin(), std::multiplies<T>());
        return Temp;
    }

    constexpr friend
    Matrix operator*(const Matrix& tMatrix, const T& tScalar) {
        Matrix Temp;
        std::transform(tMatrix.cbegin(), tMatrix.cend(), Temp.begin(), RightUnaryMultiplies<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Matrix operator*(const T& tScalar, const Matrix& tMatrix) {
        Matrix Temp;
        std::transform(tMatrix.cbegin(), tMatrix.cend(), Temp.begin(), LeftUnaryMultiplies<T>{tScalar});
        return Temp;
    }

    constexpr
    Matrix& operator*=(const Matrix& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::multiplies<T>());
        return *this;
    }

    constexpr
    Matrix& operator*=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryMultiplies<T>{tScalar});
        return *this;
    }


    constexpr
    Matrix operator/(const Matrix& tOther) {
        Matrix Temp;
        std::transform(cbegin(), cend(), tOther.cbegin(), Temp.begin(), std::divides<T>());
        return Temp;
    }

    constexpr friend
    Matrix operator/(const Matrix& tMatrix, const T& tScalar) {
        Matrix Temp;
        std::transform(tMatrix.cbegin(), tMatrix.cend(), Temp.begin(), RightUnaryDivides<T>{tScalar});
        return Temp;
    }

    constexpr friend
    Matrix operator/(const T& tScalar, const Matrix& tMatrix) {
        Matrix Temp;
        std::transform(tMatrix.cbegin(), tMatrix.cend(), Temp.begin(), LeftUnaryDivides<T>{tScalar});
        return Temp;
    }

    constexpr
    Matrix& operator/=(const Matrix& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::divides<T>());
        return *this;
    }

    constexpr
    Matrix& operator/=(const T& tScalar) {
        std::transform(cbegin(), cend(), begin(), RightUnaryDivides<T>{tScalar});
        return *this;
    }

    constexpr
    Matrix<T, N, M> transpose() const {
        Matrix<T, N, M> Temp;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < M; ++j) {
                Temp[i][j] = *this[j][i];
            }
        }
        return Temp;
    }

    template<std::size_t K>
    constexpr
    Matrix<T, M, K> dot(const Matrix<T, N, K>& rhs) const {
        Matrix<T, M, K> Temp(static_cast<T>(0));
//      TODO implement fast algorithm.
        for (std::size_t i = 0; i < M; ++i) {
            for (std::size_t j = 0; j < K; ++j) {
                for (std::size_t k = 0; k < N; ++k) {
                    Temp[i][j] += (*this)[i][k] * rhs[k][j];
                }
            }
        }
        return Temp;
    }

    template<std::size_t K>
    constexpr friend
    Matrix<T, M, K> dot(const Matrix& lhs, const Matrix<T, N, K>& rhs) {
        return lhs.dot(rhs);
    }

    constexpr static
    Matrix zero() {
        return Matrix(static_cast<T>(0));
    }

    // only for square tMatrix
    template<typename Q = T>
    constexpr static
    is_square_t<Matrix<Q, M, M>> identity() {
        Matrix<Q, M, M> Temp(static_cast<Q>(0));
        auto iter = Temp.begin();
        for (std::size_t i = 0; i < N; ++i) {
            iter += i + i * N;
            *iter = static_cast<Q>(1);
        }
        return Temp;
    }


    constexpr
    bool operator==(const Matrix& tOther) const {
        return mData == tOther.mData;
    }

    constexpr
    bool operator!=(const Matrix& tOther) const {
        return mData != tOther.mData;
    }

    constexpr
    void fill(const T& tValue) {
        std::fill(begin(), end(), tValue);
    }

    constexpr
    std::array<T, N>& operator[](std::size_t tIndex) noexcept {
        return mData[tIndex];
    }

    constexpr
    const std::array<T, N>& operator[](std::size_t tIndex) const noexcept {
        return mData[tIndex];
    }

    constexpr
    std::array<T, N>& at(std::size_t tIndex) {
        return mData.at(tIndex);
    }

    [[nodiscard]] constexpr
    std::size_t rows() const {
        return M;
    }

    [[nodiscard]] constexpr
    std::size_t cols() const {
        return M;
    }

    constexpr
    constIterator cbegin() const noexcept {
        return begin();
    }

    constexpr
    constIterator begin() const noexcept {
        return const_cast<Matrix&>(*this).begin();
    }

    constexpr
    iterator begin() noexcept {
        return mData.begin()->begin();
    }

    constexpr
    constIterator cend() const noexcept {
        return end();
    }

    constexpr
    constIterator end() const noexcept {
        return const_cast<Matrix&>(*this).end();
    }

    constexpr
    iterator end() noexcept {
        return (mData.end() - 1)->end();
    }

    constexpr
    constReverseIterator crbegin() const noexcept {
        return rbegin();
    }

    constexpr
    constReverseIterator rbegin() const noexcept {
        return const_cast<Matrix&>(*this).rbegin();
    }

    constexpr
    reverseIterator rbegin() noexcept {
        return mData.rbegin()->rbegin();
    }

    constexpr
    constReverseIterator crend() const noexcept {
        return rend();
    }

    constexpr
    constReverseIterator rend() const noexcept {
        return const_cast<Matrix&>(*this).rend();
    }

    constexpr
    reverseIterator rend() noexcept {
        return (mData.rend() - 1)->rend();
    }

    template<Iterate iter>
    constexpr
    auto cbegin() const {
        return begin<iter>();
    }

    template<Iterate iter>
    constexpr
    auto begin() const {
        return const_cast<Matrix&>(*this).begin<iter>();
    }

    template<Iterate iter>
    constexpr
    auto begin() {
        if constexpr (iter == Iterate::Row) {
            return mData.begin();
        } else if constexpr (iter == Iterate::Col) {
            return columnView(begin());
        } else {
            return begin();
        }
    }

    template<Iterate iter>
    constexpr
    auto cend() const {
        return end<iter>();
    }

    template<Iterate iter>
    constexpr
    auto end() const {
        return const_cast<Matrix&>(*this).end<iter>();
    }

    template<Iterate iter>
    constexpr
    auto end() {
        if constexpr (iter == Iterate::Row) {
            return mData.end();
        } else if constexpr (iter == Iterate::Col) {
            return columnView(mData.begin()->end());
        } else {
            return end();
        }
    }

    constexpr
    void swap(Matrix& tOther) noexcept(std::is_nothrow_swappable_v<T>) {
        mData.swap(tOther.mData);
    }

    friend
    std::ostream& operator<<(std::ostream& tStream, const Matrix& tMatrix) {
        tStream << "Matrix <" << typeid(T).name() << ", " << M << ", " << N << ">({" << std::endl;
        for (std::size_t i = 0; i < tMatrix.rows(); ++i) {
            for (std::size_t j = 0; j < tMatrix.cols(); ++j) {
                tStream << tMatrix[i][j] << " ";
            }
            tStream << std::endl;
        }
        return tStream << "})";
    }

#ifndef DMATH_ONLY_MATRIX
//    template<typename U = T>
//    is_vector_t < Matrix <U, M, N> > operator+(const Vector <U, M*N>& tOther) const {
//        Matrix <T, M, N> Temp;
//        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
//                       tOther.mData.cbegin(), Temp.mData.begin()->begin(), std::plus<U>());
//        return Temp;
//    }
//
//    template<typename U = T>
//    is_vector_t <void> operator+=(const Vector <U, M*N>& tOther) {
//        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
//                       tOther.mData.cbegin(), mData.begin()->begin(), std::plus<U>());
//    }
//
//    template<typename U = T>
//    is_vector_t <Matrix <U, M, N> > operator-(const Vector <U, M*N>& tOther) const {
//        Matrix <T, M, N> Temp;
//        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
//                       tOther.mData.cbegin(), Temp.mData.begin()->begin(), std::minus<U>());
//        return Temp;
//    }
//
//    template<typename Q = T>
//    is_vector_t <void> operator-=(const Vector <Q, M*N>& tOther) {
//        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
//                       tOther.mData.cbegin(), mData.begin()->begin(), std::minus<Q>());
//    }
//
//
//    operator is_vector_t <Vector <T, M*N> > () const {
//        Vector <T, M*N> Temp;
//        std::copy(mData.cbegin(), mData.cend(), Temp.mData.begin());
//        return Temp;
//    }

    constexpr friend
    Vector<T, N> dot(const Vector<T, M>& vec, const Matrix<T, M, N>& mat) {
        Vector<T, N> Temp(static_cast<T>(0));
// TODO implement fast algorithm.
        for (std::size_t j = 0; j < N; ++j) {
            for (std::size_t k = 0; k < M; ++k) {
                Temp[j] += vec[k] * mat[k][j];
            }
        }
        return Temp;
    }

#endif // DMATH_ONLY_MATRIX


protected:
    std::array<std::array<T, N>, M> mData;
};

} // namespace dmath
#endif //DMATH_MATRIX_H
