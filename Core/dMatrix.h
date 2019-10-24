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

//#define DMATH_ONLY_MATRIX

#ifndef DMATH_ONLY_MATRIX
    #include "dVector.h"
#endif // DMATH_ONLY_MATRIX

template<typename T, std::size_t N, std::size_t M>
class dMatrix;


template<typename T, std::size_t M, std::size_t N>
class dMatrix {
public:
    dMatrix() = default;
    dMatrix(const dMatrix <T, M, N>& tCopy) : mData(tCopy.mData) {}
    dMatrix(dMatrix <T, M, N>&& tMove) noexcept : mData(std::move(tMove.mData)) {}

    ~dMatrix() = default;

    dMatrix <T, M, N> &operator=(const dMatrix <T, M, N>& tCopy) {
        *this = tCopy;
        return *this;
    }

    dMatrix <T, M, N> &operator=(dMatrix <T, M, N> &&tMove) noexcept {
        *this = std::move(tMove);
        return *this;
    }

    dMatrix <T, M, N> operator+(const dMatrix <T, M, N>& tOther) const {
        dMatrix <T, M, N> Temp;
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(), tOther.mData.cbegin()->cbegin(), Temp.begin()->begin(), std::plus<T>());
        return Temp;
    }
    void operator+=(const dMatrix <T, M, N>& tOther) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(), tOther.mData.cbegin()->cbegin(), mData.begin()->begin(), std::plus<T>());
    }

    dMatrix <T, M, N> operator-() const {
        dMatrix <T, M, N> Temp();
        std::transform(Temp.mData.cbegin()->cbegin(), (Temp.mData.cend() - 1)->cend(), std::negate<T>());
        return Temp;

    }

    dMatrix <T, M, N> operator-(const dMatrix <T, M, N>& tOther) const {
        dMatrix <T, M, N> Temp;
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       tOther.mData.cbegin()->cbegin(), Temp.begin()->begin(), std::minus<T>());
        return Temp;
    }

    void operator-=(const dMatrix <T, M, N>& tOther) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       tOther.mData.cbegin()->cbegin(), mData.begin()->begin(), std::minus<T>());

    }

    template<typename NumberT>
    friend dMatrix <T, M, N> operator*(const dMatrix <T, M, N> &matrix, const NumberT& tNum) {
        dMatrix <T, M, N> Temp;
        std::transform(matrix.mData.cbegin()->cbegin(), (matrix.mData.cend() - 1)->cend(),
                       Temp.begin()->begin(), [&tNum](T it) { return it * T(tNum); });
        return Temp;
    }

    template<typename NumberT>
    friend dMatrix <T, M, N> operator*(const NumberT& tNum, const dMatrix <T, M, N> &matrix) {
        return matrix * tNum;
    }

    template<typename NumberT>
    void operator*=(const  NumberT&tNum) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       mData.begin()->begin(), [&tNum](T it) { return it * T(tNum); });
    }

    template<typename NumberT>
    friend dMatrix <T, M, N> operator/(const dMatrix <T, M, N> &matrix, const NumberT& tNum) {
        std::transform(matrix.mData.cbegin()->cbegin(), (matrix.mData.cend() - 1)->cend(),
                       matrix.begin()->begin(), [&tNum](T it) { return it / T(tNum); });
    }

    template<typename NumberT>
    void operator/=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), [&tNum](T it) { return it / T(tNum); });
    }

    static constexpr dMatrix <T, M, N> null() {
        dMatrix <T, M, N> Temp;
        Temp.fill(0);
        return Temp;
    }

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
    friend dMatrix <T, M, K> dot(const dMatrix <T, M, N> &mat1, const dMatrix <T, N, K> &mat2) {
        dMatrix <T, M, K> Temp;
        Temp.fill(0);
//        TODO add fast algorithm
        for(std::size_t i = 0; i < M; ++i) {
            for(std::size_t j = 0; j < K; ++j) {
                for(std::size_t k = 0; k < N; ++k) {
                    Temp[i][j] += mat1[i][k] * mat2[k][j];
                }
            }
        }
        return Temp;
    }

    #ifndef DMATH_ONLY_MATRIX
    friend dVector<T, M> dot(const dVector<T, M> &vec, const dMatrix <T, M, N> &mat) {
        dVector<T, M> Temp;
        Temp.fill(0);
//        TODO add fast algorithm
        for(std::size_t j = 0; j < N; ++j) {
            for(std::size_t k = 0; k < M; ++k) {
                Temp[j] += vec[k] * mat[k][j];
            }
        }

        return Temp;
    }

    friend dVector<T, M> dot(const dMatrix <T, M, N> &mat, const dVector<T, M> &vec) {
        return dot(vec, mat);
    }
    #endif // DMATH_ONLY_MATRIX

    // only for square matrix
    template<typename Q = T>
    static std::enable_if_t<N == M, dMatrix <Q, N, M>> identity() {
        dMatrix <Q, N, M> Temp;
        Temp.fill(0);
        auto iter = Temp.mData.begin()->begin();
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


    bool operator==(const dMatrix <T, M, N>& tOther) const {
        return mData == tOther.mData;
    }

    bool operator!=(const dMatrix <T, M, N>& tOther) const {
        return mData != tOther.mData;
    }


    void fill(const T& value) {
        std::fill(mData.begin()->begin(), (mData.end() - 1)->end(), value);
    }

//    constexpr std::size_t size() const noexcept {
//        return mData.size();
//    }

    constexpr std::array<T, N> &operator[](std::size_t index) noexcept {
        return mData.operator[](index);
    }

    constexpr const std::array<T, N> &operator[](std::size_t index) const noexcept {
        return mData.operator[](index);
    }

    constexpr std::array<T, N> &at(std::size_t index) {
        return mData.at(index);
    }

    constexpr const std::array<T, N> &at(std::size_t index) const {
        return mData.at(index);
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

    void swap(dMatrix <T, M, N>& tOther) {
        mData.swap(tOther.mData);
    }

    friend std::ostream &operator<<(std::ostream &stream, const dMatrix <T, M, N> &matrix) {
        stream << "dMatrix <" << M << ", " << N << ">(" << std::endl;
        for(const std::array<T, N>& row : matrix.mData) {
            for(const T& item : row) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
        return stream << ')';
    }

protected:
    std::array<std::array<T, N>, M> mData;
};

#endif //DMATH_DMATRIX_H
