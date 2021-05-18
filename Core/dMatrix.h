//
// Created by skinderev on 24.10.2019.
//

#ifndef DMATH_DMATRIX_H
#define DMATH_DMATRIX_H

#include <cstddef>
#include <iostream>
#include <array>
#include <memory>
#include <numeric>
#include <algorithm>
#include <typeinfo>
#include <type_traits>

//#define DMATH_ONLY_MATRIX

#include "util.h"

#ifndef DMATH_ONLY_MATRIX
    #include "dVector.h"
#endif // DMATH_ONLY_MATRIX

template<typename T, std::size_t M, std::size_t N>
class dMatrix;

template <typename T, std::size_t M>
using dSMatrix = dMatrix<T, M, M>;

enum class Iterate {Row, Col, Val};

template<typename T, std::size_t M, std::size_t N>
class dMatrix {
protected:
// TODO make SFINAE

    static constexpr std::size_t SizeOnlyForVector = std::max(M, N);

    template<typename ReturnType = void>
    using is_square_t = std::enable_if_t<M == N, ReturnType>;

    template<typename ReturnType = void>
    using is_scalar_t = std::enable_if_t<M == 1 && N == 1, ReturnType>;

#ifndef DMATH_ONLY_MATRIX
    template<typename ReturnType = dVector<T, M*N>>
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

    template<typename Type, typename UnqualifiedType = std::remove_cv_t<Type>>
    struct iteratorCol {
        using iteratorCategory      = std::forward_iterator_tag;
        using value_type            = UnqualifiedType;
        using difference_type       = std::ptrdiff_t;
        using pointer               = Type*;
        using reference             = Type&;

        constexpr
        explicit iteratorCol() : mPtr(nullptr) {}

        constexpr
        explicit iteratorCol(Type* ptr) : mPtr(ptr) {}

        constexpr
        Type& operator*() const { return *mPtr; }
        constexpr
        Type* operator->() { return mPtr; }

        // Prefix increment
        constexpr
        iteratorCol& operator++() { mPtr += N; return *this; }

        // Postfix increment
        constexpr
        iteratorCol operator++(int) { auto tmp(*this); ++(*this); return tmp; }

        template<typename Other>
        constexpr
        bool operator== (const iteratorCol<Other>& rhs) { return mPtr == rhs.mPtr; }
        template<typename Other>
        constexpr
        bool operator!= (const iteratorCol<Other>& rhs) { return mPtr != rhs.mPtr; }

        bool operator== (Type*& rhs) { return mPtr == rhs; }
        bool operator!= (Type*& rhs) { return mPtr != rhs; }

        operator iteratorCol<const Type>() const {
            return iteratorCol<const Type>(mPtr);
        }

        operator Type*() const {
            return mPtr;
        }

        friend std::ostream &operator<<(std::ostream &tStream, const iteratorCol &iter) {
            return tStream << iter.mPtr;
        }

    private:
        Type* mPtr;
    };

    struct columnView {
        using columnIterator        = iteratorCol<T>;
        using constColumnIterator   = iteratorCol<const T>;

        constexpr
        explicit columnView(T* ptr): mPtr(ptr) {}

        constexpr
        columnView& operator*() const { return *this; }
        constexpr
        columnView* operator->() { return this; }

        // Prefix increment
        constexpr
        columnView& operator++() { ++mPtr; return *this; }

        // Postfix increment
        constexpr
        columnView operator++(int) { auto tmp(*this); ++(*this); return tmp; }

        constexpr friend
        bool operator== (const columnView& a, const columnView& b) { return a.mPtr == b.mPtr; };
        constexpr friend
        bool operator!= (const columnView& a, const columnView& b) { return a.mPtr != b.mPtr; };

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

    private:
        T* mPtr;
    };


public:
    constexpr
    explicit dMatrix(const std::array<std::array<T, N>, M>& tMat) : mData(tMat) {}

    constexpr
    explicit dMatrix(std::array<std::array<T, N>, M>&& tMat) : mData(std::move(tMat)) {}


    constexpr
    explicit dMatrix(const T& tVal) {
        fill(tVal);
    }

    constexpr
    dMatrix() = default;
    constexpr
    dMatrix(const dMatrix& tCopy) = default;
    constexpr
    dMatrix(dMatrix&& tMove) noexcept = default;

    ~dMatrix() = default;

    constexpr
    dMatrix& operator=(const dMatrix& tCopy) = default;

    constexpr
    dMatrix& operator=(dMatrix &&tMove) noexcept = default;

    constexpr
    dMatrix operator+() const {
        return dMatrix(*this);
    }

    constexpr
    friend dMatrix operator+(const dMatrix& tMat, const T& tScalar) {
        dMatrix Temp;
        std::transform(tMat.cbegin(), tMat.cend(), Temp.begin(), RightUnaryPlus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend dMatrix operator+(const T& tScalar, const dMatrix& tMat) {
        dMatrix Temp;
        std::transform(tMat.cbegin(), tMat.cend(), Temp.begin(), LeftUnaryPlus<T>(tScalar));
        return Temp;
    }

    constexpr
    dMatrix operator+(const dMatrix& tOther) const {
        dMatrix Temp;
        std::transform(cbegin(), cend(), tOther.cbegin(), Temp.begin(), std::plus<T>());
        return Temp;
    }

    constexpr
    dMatrix& operator+=(const dMatrix& tOther) {
        std::transform(cbegin(), cend(), tOther.cbegin(), begin(), std::plus<T>());
        return *this;
    }

    constexpr
    dMatrix& operator+=(const T& tScalar) {
        std::transform(cbegin(), cend(),
                       begin(), RightUnaryPlus<T>(tScalar));
        return *this;
    }

    constexpr
    dMatrix operator-() const {
        dMatrix Temp;
        std::transform(cbegin(), cend(), Temp.begin(), std::negate<T>());
        return Temp;
    }

    constexpr
    friend dMatrix operator-(const dMatrix& tMat, const T& tScalar) {
        dMatrix Temp;
        std::transform(tMat.cbegin(), tMat.cend(), Temp.begin(), RightUnaryMinus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend dMatrix operator-(const T& tScalar, const dMatrix& tMat) {
        dMatrix Temp;
        std::transform(tMat.cbegin(), tMat.cend(), Temp.begin(), LeftUnaryMinus<T>(tScalar));
        return Temp;
    }

    constexpr
    dMatrix operator-(const dMatrix& tOther) const {
        dMatrix Temp;
        std::transform(cbegin(), cend(),
                       tOther.cbegin(), Temp.begin(), std::minus<T>());
        return Temp;
    }

    constexpr
    dMatrix& operator-=(const dMatrix& tOther) {
        std::transform(cbegin(), cend(),
                       tOther.cbegin(), begin(), std::minus<T>());
        return *this;
    }

    constexpr
    dMatrix& operator-=(const T& tScalar) {
        std::transform(cbegin(), cend(),
                       begin(), RightUnaryMinus<T>(tScalar));
        return *this;
    }


    constexpr
    dMatrix operator*(const dMatrix& tOther) {
        dMatrix Temp;
        std::transform(cbegin(), cend(),
                       tOther.cbegin(), Temp.begin(), std::multiplies<T>());
        return Temp;
    }

    constexpr
    friend dMatrix operator*(const dMatrix &tMat, const T& tScalar) {
        dMatrix Temp;
        std::transform(tMat.cbegin(), tMat.cend(),
                       Temp.begin(), RightUnaryMultiplies<T>(tScalar));
        return Temp;
    }
    
    constexpr
    friend dMatrix operator*(const T& tScalar, const dMatrix &tMat) {
        dMatrix Temp;
        std::transform(tMat.cbegin(), tMat.cend(),
                       Temp.begin(), LeftUnaryMultiplies<T>(tScalar));
        return Temp;
    }

    constexpr
    dMatrix& operator*=(const dMatrix& tOther) {
        std::transform(cbegin(), cend(),
                       tOther.cbegin(), begin(), std::multiplies<T>());
        return *this;
    }

    constexpr
    dMatrix& operator*=(const T& tScalar) {
        std::transform(cbegin(), cend(),
                       begin(), RightUnaryMultiplies<T>(tScalar));
        return *this;
    }


    constexpr
    dMatrix operator/(const dMatrix& tOther) {
        dMatrix Temp;
        std::transform(cbegin(), cend(),
                       tOther.cbegin(), Temp.begin(), std::divides<T>());
        return Temp;
    }

    constexpr
    friend dMatrix operator/(const dMatrix &tMat, const T& tScalar) {
        dMatrix Temp;
        std::transform(tMat.cbegin(), tMat.cend(),
                       Temp.begin(), RightUnaryDivides<T>(tScalar));
        return Temp;
    }

    constexpr
    friend dMatrix operator/(const T& tScalar, const dMatrix &tMat) {
        dMatrix Temp;
        std::transform(tMat.cbegin(), tMat.cend(),
                       Temp.begin(), LeftUnaryDivides<T>(tScalar));
        return Temp;
    }

    constexpr
    dMatrix& operator/=(const dMatrix& tOther) {
        std::transform(cbegin(), cend(),
                       tOther.cbegin(), begin(), std::divides<T>());
        return *this;
    }

    constexpr
    dMatrix& operator/=(const T& tScalar) {
        std::transform(cbegin(), cend(),
                       begin(), RightUnaryDivides<T>(tScalar));
        return *this;
    }

    constexpr
    static dMatrix zero() {
        return dMatrix(T(0));
    }

    constexpr
    dMatrix <T, N, M> transpose() const {
        dMatrix <T, N, M> Temp;
        for(std::size_t i = 0; i < N; ++i) {
            for(std::size_t j = 0; j < M; ++j) {
                Temp[i][j] = *this[j][i];
            }
        }
        return Temp;
    }

    template<std::size_t K>
    constexpr
    dMatrix <T, M, K> dot(const dMatrix <T, N, K> &rhs) const {
        dMatrix <T, M, K> Temp(T(0));
//        TODO add fast algorithm
        for(std::size_t i = 0; i < M; ++i) {
            for(std::size_t j = 0; j < K; ++j) {
                for(std::size_t k = 0; k < N; ++k) {
                    Temp[i][j] += (*this)[i][k] * rhs[k][j];
                }
            }
        }
        return Temp;
    }

    template<std::size_t K>
    constexpr
    friend dMatrix <T, M, K> dot(const dMatrix &lhs, const dMatrix <T, N, K> &rhs) {;
        return lhs.dot(rhs);
    }

    #ifndef DMATH_ONLY_MATRIX
    constexpr
    friend dVector<T, M> dot(const dVector<T, M> &vec, const dMatrix &mat) {
        dVector<T, M> Temp(T(0));
//        TODO add fast algorithm
        for(std::size_t j = 0; j < N; ++j) {
            for(std::size_t k = 0; k < M; ++k) {
                Temp[j] += vec[k] * mat[k][j];
            }
        }

        return Temp;
    }

    #endif // DMATH_ONLY_MATRIX

    // only for square tMat
    template<typename Q = T>
    constexpr
    static is_square_t <dMatrix <Q, M, M>> identity() {
        dMatrix<Q, M, M> Temp(Q(0));
        auto iter = Temp.begin();
        for (std::size_t i = 0; i < N; ++i) {
            iter += i + i * N;
            *iter = Q(1);
        }
        return Temp;
    }


    constexpr
    bool operator==(const dMatrix& tOther) const {
        return mData == tOther.mData;
    }
    constexpr
    bool operator!=(const dMatrix& tOther) const {
        return mData != tOther.mData;
    }

    constexpr
    void fill(const T& value) {
        std::fill(begin(), end(), value);
    }

//    constexpr std::size_t size() const noexcept {
//        return mData.size();
//    }

    constexpr
    std::array<T, N>& operator[](std::size_t index) noexcept {
        return mData[index];
    }

    constexpr
    const std::array<T, N>& operator[](std::size_t index) const noexcept {
        return mData[index];
    }

    constexpr
    std::array<T, N>& at(std::size_t index) {
        return mData.at(index);
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
    const T* cbegin() const noexcept {
        return mData.cbegin()->cbegin();
    }

    constexpr
    const T* begin() const noexcept {
        return const_cast<dMatrix&>(*this).begin();
    }

    constexpr
    T* begin() noexcept {
        return mData.begin()->begin();
    }

    constexpr
    const T* cend() const noexcept {
        return (mData.cend() - 1)->cend();
    }

    constexpr
    const T* end() const noexcept {
        return const_cast<dMatrix&>(*this).end();
    }

    constexpr
    T* end() noexcept {
        return (mData.end() - 1)->end();
    }

    constexpr
    const T* crbegin() const noexcept {
        return mData.crbegin()->crbegin();
    }

    constexpr
    const T* rbegin() const noexcept {
        return const_cast<dMatrix&>(*this).rbegin();
    }

    constexpr
    T* rbegin() noexcept {
        return mData.rbegin()->rbegin();
    }

    constexpr
    const T* crend() const noexcept {
        return (mData.crend() - 1)->crend();
    }

    constexpr
    const T* rend() const noexcept {
        return const_cast<dMatrix&>(*this).rend();
    }

    constexpr
    T* rend() noexcept {
        return (mData.rend() - 1)->rend();
    }


    template<Iterate iter>
    constexpr
    auto begin() const {
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
    auto end() const {
        if constexpr (iter == Iterate::Row) {
            return mData.end();
        } else if constexpr (iter == Iterate::Col) {
            return columnView(mData.begin()->end());
        } else {
            return end();
        }
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
    void swap(dMatrix& tOther) {
        mData.swap(tOther.mData);
    }

    friend std::ostream &operator<<(std::ostream &tStream, const dMatrix &tMat) {
        tStream << "dMatrix <" << typeid(T).name() << ", " << M << ", " << N << ">({" << std::endl;
        for(std::size_t i = 0; i < tMat.rows(); ++i) {
            for(std::size_t j = 0; j < tMat.cols(); ++j) {
                tStream << tMat[i][j] << " ";
            }
            tStream << std::endl;
        }
        return tStream << "})";
    }

    template<typename U = T>
    is_vector_t < dMatrix <U, M, N> > operator+(const dVector <U, SizeOnlyForVector>& tOther) const {
        dMatrix <T, M, N> Temp;
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       tOther.mData.cbegin(), Temp.mData.begin()->begin(), std::plus<U>());
        return Temp;
    }

    template<typename U = T>
    is_vector_t <void> operator+=(const dVector <U, SizeOnlyForVector>& tOther) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       tOther.mData.cbegin(), mData.begin()->begin(), std::plus<U>());
    }

    template<typename U = T>
    is_vector_t <dMatrix <U, M, N> > operator-(const dVector <U, SizeOnlyForVector>& tOther) const {
        dMatrix <T, M, N> Temp;
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       tOther.mData.cbegin(), Temp.mData.begin()->begin(), std::minus<U>());
        return Temp;
    }

    template<typename U = T>
    is_vector_t <void> operator-=(const dVector <U, SizeOnlyForVector>& tOther) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       tOther.mData.cbegin(), mData.begin()->begin(), std::minus<U>());

    }


//    friend dVector<T, N> dot(const dVector<T, M> &vec, const dMatrix <T, M, N> &mat) {
//        dVector<T, N> Temp;
//        Temp.fill(0);
////        TODO add fast algorithm
//        for(std::size_t j = 0; j < N; ++j) {
//            for(std::size_t k = 0; k < M; ++k) {
//                Temp[j] += vec[k] * mat[k][j];
//            }
//        }
//        return Temp;
//    }

    operator is_vector_t <dVector <T, SizeOnlyForVector> > () const {
        dVector <T, SizeOnlyForVector> Temp;
        std::copy(mData.cbegin(), mData.cend(), Temp.mData.begin());
        return Temp;
    }


protected:
    std::array<std::array<T, N>, M> mData;
};

#endif //DMATH_DMATRIX_H
