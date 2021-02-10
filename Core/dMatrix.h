//
// Created by skinderev on 24.10.2019.
//

#ifndef DMATH_DMATRIX_H
#define DMATH_DMATRIX_H


#include <cstddef>
#include <array>
#include <iostream>
#include <memory>
#include <numeric>
#include <algorithm>
#include "util.h"

//#define DMATH_ONLY_MATRIX

#ifndef DMATH_ONLY_MATRIX
    #include "dVector.h"
#endif // DMATH_ONLY_MATRIX

template<typename T, std::size_t M, std::size_t N>
class dMatrix;

template <typename T, std::size_t M>
using dSMatrix = dMatrix<T, M, M>;


template<typename T, std::size_t M, std::size_t N>
class dMatrix {
public:
    typedef T 	    			                    value_type;
    typedef value_type*			                    pointer;
    typedef const value_type*                       const_pointer;
    typedef value_type&                   	        reference;
    typedef const value_type&             	        const_reference;
//    typedef std::array<T, N>*          		        iterator;
//    typedef const value_type*			            const_iterator;
//    typedef std::size_t                    	        size_type;
//    typedef std::ptrdiff_t                   	    difference_type;
//    typedef std::reverse_iterator<iterator>	        reverse_iterator;
//    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

public:
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

    constexpr
    ~dMatrix() = default;

    dMatrix& operator=(const dMatrix& tCopy) = default;

    dMatrix& operator=(dMatrix &&tMove) noexcept = default;

    constexpr
    dMatrix operator+() const {
        return dMatrix(*this);
    }

    constexpr
    friend dMatrix operator+(const dMatrix& tMat, const T& tScalar) {
        dMatrix Temp;
//        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(), Temp.begin()->begin(), RightUnaryPlus<T>(tScalar));
        std::transform(tMat.cbegin<Iterate::Val>(), tMat.cend<Iterate::Val>(), Temp.begin<Iterate::Val>(), RightUnaryPlus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend dMatrix operator+(const T& tScalar, const dMatrix& tMat) {
        dMatrix Temp;
        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(), Temp.begin()->begin(), LeftUnaryPlus<T>(tScalar));
        return Temp;
    }

    constexpr
    dMatrix operator+(const dMatrix& tOther) const {
        dMatrix Temp;
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(), tOther.cbegin()->cbegin(), Temp.begin()->begin(), std::plus<T>());
        return Temp;
    }

    constexpr
    dMatrix& operator+=(const dMatrix& tOther) {
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(), tOther.cbegin()->cbegin(), begin()->begin(), std::plus<T>());
        return *this;
    }

    constexpr
    dMatrix& operator+=(const T& tScalar) {
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       begin()->begin(), RightUnaryPlus<T>(tScalar));
        return *this;
    }

    constexpr
    dMatrix operator-() const {
        dMatrix Temp;
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(), Temp.begin()->begin(), std::negate<T>());
        return Temp;
    }

    constexpr
    friend dMatrix operator-(const dMatrix& tMat, const T& tScalar) {
        dMatrix Temp;
        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(), Temp.begin()->begin(), RightUnaryMinus<T>(tScalar));
        return Temp;
    }

    constexpr
    friend dMatrix operator-(const T& tScalar, const dMatrix& tMat) {
        dMatrix Temp;
        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(), Temp.begin()->begin(), LeftUnaryMinus<T>(tScalar));
        return Temp;
    }

    constexpr
    dMatrix operator-(const dMatrix& tOther) const {
        dMatrix Temp;
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       tOther.cbegin()->cbegin(), Temp.begin()->begin(), std::minus<T>());
        return Temp;
    }

    constexpr
    dMatrix& operator-=(const dMatrix& tOther) {
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       tOther.cbegin()->cbegin(), begin()->begin(), std::minus<T>());
        return *this;
    }

    constexpr
    dMatrix& operator-=(const T& tScalar) {
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       begin()->begin(), RightUnaryMinus<T>(tScalar));
        return *this;
    }


    constexpr
    dMatrix operator*(const dMatrix& tOther) {
        dMatrix Temp;
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       tOther.cbegin()->cbegin(), Temp.begin()->begin(), std::multiplies<T>());
        return Temp;
    }

    constexpr
    friend dMatrix operator*(const dMatrix &tMat, const T& tScalar) {
        dMatrix Temp;
        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(),
                       Temp.begin()->begin(), RightUnaryMultiplies<T>(tScalar));
        return Temp;
    }
    
    constexpr
    friend dMatrix operator*(const T& tScalar, const dMatrix &tMat) {
        dMatrix Temp;
        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(),
                       Temp.begin()->begin(), LeftUnaryMultiplies<T>(tScalar));
        return Temp;
    }

    constexpr
    dMatrix& operator*=(const dMatrix& tOther) {
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       tOther.cbegin()->cbegin(), begin()->begin(), std::multiplies<T>());
        return *this;
    }

    constexpr
    dMatrix& operator*=(const T& tScalar) {
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       begin()->begin(), RightUnaryMultiplies<T>(tScalar));
        return *this;
    }


    constexpr
    dMatrix operator/(const dMatrix& tOther) {
        dMatrix Temp;
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       tOther.cbegin()->cbegin(), Temp.begin()->begin(), std::divides<T>());
        return Temp;
    }

    constexpr
    friend dMatrix operator/(const dMatrix &tMat, const T& tScalar) {
        dMatrix Temp;
        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(),
                       Temp.begin()->begin(), RightUnaryDivides<T>(tScalar));
        return Temp;
    }

    constexpr
    friend dMatrix operator/(const T& tScalar, const dMatrix &tMat) {
        dMatrix Temp;
        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(),
                       Temp.begin()->begin(), LeftUnaryDivides<T>(tScalar));
        return Temp;
    }

    constexpr
    dMatrix& operator/=(const dMatrix& tOther) {
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       tOther.cbegin()->cbegin(), begin()->begin(), std::divides<T>());
        return *this;
    }

    constexpr
    dMatrix& operator/=(const T& tScalar) {
        std::transform(cbegin()->cbegin(), (cend() - 1)->cend(),
                       begin()->begin(), RightUnaryDivides<T>(tScalar));
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
                Temp[i][j] = mData[j][i];
            }
        }
        return Temp;
    }

    template<std::size_t K>
    friend dMatrix <T, M, K> dot(const dMatrix &lhs, const dMatrix <T, N, K> &rhs) {
        dMatrix <T, M, K> Temp(0);
//        TODO add fast algorithm
        for(std::size_t i = 0; i < M; ++i) {
            for(std::size_t j = 0; j < K; ++j) {
                for(std::size_t k = 0; k < N; ++k) {
                    Temp[i][j] += lhs[i][k] * rhs[k][j];
                }
            }
        }
        return Temp;
    }

    #ifndef DMATH_ONLY_MATRIX
    friend dVector<T, M> dot(const dVector<T, M> &vec, const dMatrix &mat) {
        dVector<T, M> Temp(0);
//        TODO add fast algorithm
        for(std::size_t j = 0; j < N; ++j) {
            for(std::size_t k = 0; k < M; ++k) {
                Temp[j] += vec[k] * mat[k][j];
            }
        }

        return Temp;
    }

    friend dVector<T, M> dot(const dMatrix &mat, const dVector<T, M> &vec) {
        return dot(vec, mat);
    }
    #endif // DMATH_ONLY_MATRIX

    // only for square tMat
    template<typename Q = T>
    static std::enable_if_t<N == M, dMatrix <Q, M, M>> identity() {
        dMatrix<Q, M, M> Temp(Q(0));
        auto iter = Temp.begin()->begin();
        for (std::size_t i = 0; i < N; ++i) {
            iter += i + i * N;
            *iter = Q(1);
        }
        return Temp;
    }

    template<typename Q = T>
    std::enable_if_t<N == M, Q> det() {
//        TODO
        return 0;
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
        std::fill(begin()->begin(), (end() - 1)->end(), value);
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

    constexpr
    const std::array<T, N>& at(std::size_t index) const {
        return mData.at(index);
    }

    constexpr
    const std::array<T, N>* cbegin() const noexcept {
        return mData.cbegin();
    }

    constexpr
    const std::array<T, N>* begin() const noexcept {
        return mData.begin();
    }

    constexpr
    std::array<T, N>* begin() noexcept {
        return mData.begin();
    }

    constexpr
    const std::array<T, N>* cend() const noexcept {
        return mData.cend();
    }

    constexpr
    const std::array<T, N>* end() const noexcept {
        return mData.end();
    }

    constexpr
    std::array<T, N>* end() noexcept {
        return mData.end();
    }

    constexpr
    void swap(dMatrix& tOther) {
        mData.swap(tOther.mData);
    }

    friend std::ostream &operator<<(std::ostream &tStream, const dMatrix &tMat) {
        tStream << "dMatrix <" << M << ", " << N << ">(" << std::endl;
        for(const std::array<T, N>& row : tMat.mData) {
            for(const T& item : row) {
                tStream << item << " ";
            }
            tStream << std::endl;
        }
        return tStream << ')';
    }

protected:
    std::array<std::array<T, N>, M> mData;

    enum class Iterate { Row, Col, Val };

    template<Iterate iterate>
    constexpr
    const auto cbegin() const noexcept {
        if constexpr (iterate == Iterate::Row) {
            return cbegin();
//        } else if constexpr (iterate == Iterate::Col) {
//            return begin();
        } else if constexpr (iterate == Iterate::Val) {
            return cbegin()->cbegin();
        }
    }

    template<Iterate iterate>
    constexpr
    const auto begin() const noexcept {
        if constexpr (iterate == Iterate::Row) {
            return begin();
//        } else if constexpr (iterate == Iterate::Col) {
//            return begin();
        } else if constexpr (iterate == Iterate::Val) {
            return begin()->begin();
        }
    }

    template<Iterate iterate>
    constexpr
    auto begin() noexcept {
        if constexpr (iterate == Iterate::Row) {
            return begin();
//        } else if constexpr (iterate == Iterate::Col) {
//            return begin();
        } else if constexpr (iterate == Iterate::Val) {
            return begin()->begin();
        }
    }

    template<Iterate iterate>
    constexpr
    const auto cend() const noexcept {
        if constexpr (iterate == Iterate::Row) {
            return cend();
//        } else if constexpr (iterate == Iterate::Col) {
//            return begin();
        } else if constexpr (iterate == Iterate::Val) {
            return (cend()-1)->cend();
        }
    }

    template<Iterate iterate>
    constexpr
    const auto end() const noexcept {
        if constexpr (iterate == Iterate::Row) {
            return end();
//        } else if constexpr (iterate == Iterate::Col) {
//            return begin();
        } else if constexpr (iterate == Iterate::Val) {
            return (end()-1)->end();
        }
    }

    template<Iterate iterate>
    constexpr
    auto end() noexcept {
        if constexpr (iterate == Iterate::Row) {
            return end();
//        } else if constexpr (iterate == Iterate::Col) {
//            return begin();
        } else if constexpr (iterate == Iterate::Val) {
            return (end()-1)->end();
        }
    }

};

#include <optional>
#endif //DMATH_DMATRIX_H
