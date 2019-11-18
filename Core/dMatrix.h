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
#include <random>

template<typename T, std::size_t SizeT>
class dVector;

template<typename T, std::size_t N, std::size_t M>
class dMatrix;


template<typename T, std::size_t M, std::size_t N>
class dMatrix {
protected:
// TODO make SFINAE

    static constexpr std::size_t SizeOnlyForVector = std::max(M, N);

    template<typename ReturnType = void>
    using is_square_t = std::enable_if_t<M == N, ReturnType>;

    template<typename ReturnType = void>
    using is_1x1_t = std::enable_if_t<M == 1 && N == 1, ReturnType>;

    template<typename ReturnType = void>
    using is_vector_t = std::enable_if_t<M == 1 || N == 1, ReturnType>;

public:
    constexpr dMatrix() = default;
    constexpr dMatrix(const dMatrix <T, M, N>& tCopy) = default;
    constexpr dMatrix(dMatrix <T, M, N>&& tMove) = default;

    ~dMatrix() = default;

    constexpr dMatrix <T, M, N> &operator=(const dMatrix <T, M, N>& tCopy) = default;
    constexpr dMatrix <T, M, N> &operator=(dMatrix <T, M, N> &&tMove) = default;

    dMatrix <T, M, N> operator+(const dMatrix <T, M, N>& tOther) const {
        dMatrix <T, M, N> Temp;
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       tOther.mData.cbegin()->cbegin(), Temp.begin()->begin(), std::plus<T>());
        return Temp;
    }

    template<typename NumberT>
    dMatrix <T, M, N> operator+(const NumberT& tNum) {
        dMatrix <T, M, N> Temp;
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       Temp.begin()->begin(), [&tNum](T it) { return it + toT(tNum); });
        return Temp;
    }

    template <typename NumberT>
    friend dMatrix <T, M, N> operator+(const dMatrix <T, M, N>& tMat, const NumberT& tNum) {
        dMatrix <T, M, N> Temp;
        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(),
                       Temp.begin()->begin(), [&tNum](T it) { return it + toT(tNum); });
        return Temp;
    }
    template <typename NumberT>
    friend dMatrix <T, M, N> operator+(const NumberT& tNum, const dVector <T, SizeT>& tVec) {
        return tVec + tNum;
    }


    void operator+=(const dMatrix <T, M, N>& tOther) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       tOther.mData.cbegin()->cbegin(), mData.begin()->begin(), std::plus<T>());
    }

    template<typename NumberT>
    void operator+=(const NumberT& tNum) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       mData.begin()->begin(), [&tNum](T it) { return it + toT(tNum); });
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

    template <typename NumberT>
    friend dMatrix <T, M, N> operator-(const dMatrix <T, M, N>& tMat, const NumberT& tNum) {
        dMatrix <T, M, N> Temp;
        std::transform(tMat.cbegin()->cbegin(), (tMat.cend() - 1)->cend(),
                       Temp.begin()->begin(), [&tNum](T it) { return it - toT(tNum); });
        return Temp;
    }

    void operator-=(const dMatrix <T, M, N>& tOther) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       tOther.mData.cbegin()->cbegin(), mData.begin()->begin(), std::minus<T>());

    }

    template<typename NumberT>
    void operator-=(const NumberT& tNum) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       mData.begin()->begin(), [&tNum](T it) { return it - toT(tNum); });
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
    void operator*=(const NumberT& tNum) {
        std::transform(mData.cbegin()->cbegin(), (mData.cend() - 1)->cend(),
                       mData.begin()->begin(), [&tNum](T it) { return it * toT(tNum); });
    }

    template<typename NumberT>
    friend dMatrix <T, M, N> operator/(const dMatrix <T, M, N> &matrix, const NumberT& tNum) {
        std::transform(matrix.mData.cbegin()->cbegin(), (matrix.mData.cend() - 1)->cend(),
                       matrix.begin()->begin(), [&tNum](T it) { return it / toT(tNum); });
    }

    template<typename NumberT>
    void operator/=(const NumberT& tNum) {
        std::transform(mData.cbegin(), mData.cend(), mData.begin(), [&tNum](T it) { return it / toT(tNum); });
    }

    static constexpr dMatrix <T, M, N> null() {
        dMatrix <T, M, N> Temp;
        Temp.fill(toT(0));
        return Temp;
    }

    static dMatrix <T, M, N> random() {
//        TODO !!!!!!
        std::random_device gen;
        std::uniform_real_distribution<double> rnd(-0.5, 0.5);

        dMatrix <T, M, N> Temp;
        std::generate(Temp.mData.begin()->begin(), (Temp.mData.end() - 1)->end(),
                      [&rnd, &gen]() { return rnd(gen); });;
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

    // only for square matrix
    template<typename Q = T>
    static is_square_t <dMatrix <Q, N, M>> identity() {
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
    is_square_t <Q> det() {
        Q Temp(0);
//        TODO
        return Temp;
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

    constexpr const std::array<T, N>* cbegin() const noexcept {
        return mData.cbegin();
    }

    constexpr std::array<T, N>* begin() noexcept {
        return mData.begin();
    }

    constexpr const std::array<T, N>* cend() const noexcept {
        return mData.cend();
    }

    constexpr std::array<T, N>* end() noexcept {
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


    friend dVector<T, N> dot(const dVector<T, M> &vec, const dMatrix <T, M, N> &mat) {
        dVector<T, N> Temp;
        Temp.fill(0);
//        TODO add fast algorithm
        for(std::size_t j = 0; j < N; ++j) {
            for(std::size_t k = 0; k < M; ++k) {
                Temp[j] += vec[k] * mat[k][j];
            }
        }
        return Temp;
    }

    operator is_vector_t <dVector <T, SizeOnlyForVector> > () const {
        dVector <T, SizeOnlyForVector> Temp;
        std::copy(mData.cbegin(), mData.cend(), Temp.mData.begin());
        return Temp;
    }


protected:
    std::array<std::array<T, N>, M> mData;

    template <typename NumberT>
    static constexpr T toT(const NumberT& tNum) {
        if constexpr (std::is_arithmetic_v <NumberT>) {
            return static_cast<T>(tNum);
        } else if (std::is_same_v <NumberT, T>) {
            return tNum;
        } else if (std::is_same_v <NumberT, dVector<T, 1> >) {
            return tNum[0];
        } else if (std::is_same_v <NumberT, dMatrix<T, 1, 1> >) {
            return tNum[0][0];
        }
    }

    template <typename, std::size_t>
    friend class dVector;
};

#endif //DMATH_DMATRIX_H
