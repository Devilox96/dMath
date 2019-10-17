//
// Created by skinderev on 08.10.2019.
//

#ifndef PERSEPTRON_VECTOR_H
#define PERSEPTRON_VECTOR_H

#include <cstddef>
#include <array>
#include <vector>
#include <iostream>
#include <memory>
#include <numeric>
#include <algorithm>


template<typename Tp, std::size_t N>
class Vector;

template <typename Tp>
using Vector2d = Vector<Tp, 2>;

template <typename Tp>
using Vector3d = Vector<Tp, 3>;

template <typename Tp>
using Vector4d = Vector<Tp, 4>;


template<typename Tp, std::size_t N>
class Vector {
public:
    Vector() {};
    Vector(std::initializer_list<Tp> list) : data() {
        if (N > list.size()) throw std::runtime_error("sizes are not equal\n");
        std::copy_n(list.begin(), list.size(), data.begin());
    }

    // copy
    explicit Vector(const Vector<Tp, N> &other) : data(other.data) {}
    explicit Vector(const std::array<Tp, N> &other) : data(other) {}
    explicit Vector(const std::vector<Tp> &other){
        if (N != other.size()) throw std::runtime_error("sizes are not equal\n");
        std::copy_n(other.cbegin(), other.size(), data.begin());
    }

    // move
    explicit Vector(Vector<Tp, N> &&other) noexcept : data(std::move(other.data)) {}
    explicit Vector(std::array<Tp, N>&& other) noexcept : data(std::move(other)) {}

    Vector<Tp, N> &operator=(std::initializer_list<Tp> list) {
        if (N > list.size()) throw std::runtime_error("sizes are not equal\n");
//        data.reset(new std::array<Tp, N>());
        std::copy_n(list.begin(), list.size(), data.begin());
        return *this;
    }

    Vector<Tp, N> &operator=(const Vector<Tp, N> &other) {
        data = other.data;
        return *this;
    }

    Vector<Tp, N> &operator=(Vector<Tp, N> &&other) noexcept {
        data = std::move(other.data);
        return *this;
    }

    Vector<Tp, N> operator+(const Vector<Tp, N> &other) const {
        std::array<Tp, N> temp;
        std::transform(data.cbegin(), data.cend(), other.data.cbegin(), temp.begin(), std::plus<Tp>());
        return Vector<Tp, N>(temp);
    }

    void operator+=(const Vector<Tp, N> &other) {
        std::transform(data.cbegin(), data.cend(), other.data.cbegin(), data.begin(), std::plus<Tp>());
    }

    Vector<Tp, N> operator-() const {
        std::array<Tp, N> temp;
        std::transform(data.cbegin(), data.cend(), std::negate<Tp>());
        return Vector<Tp, N>(temp);

    }

    Vector<Tp, N> operator-(const Vector<Tp, N> &other) const {
        std::array<Tp, N> temp;
        std::transform(data.cbegin(), data.cend(), other.data.cbegin(), temp.begin(), std::minus<Tp>());
        return Vector<Tp, N>(temp);
    }

    void operator-=(const Vector<Tp, N> &other) {
        std::transform(data.cbegin(), data.cend(), other.data.cbegin(), data.begin(), std::minus<Tp>());
    }

    template<typename Tn>
    friend Vector<Tp, N> operator*(const Vector<Tp, N> &vector, Tn number) {
        std::array<Tp, N> temp;
        std::transform(vector.data.cbegin(), vector.data.cend(), temp.begin(),
                       [&number](Tp it) { return it * static_cast<Tp>(number); });
        return Vector<Tp, N>(temp);
    }

    template<typename Tn>
    friend inline Vector<Tp, N> operator*(Tn number, const Vector<Tp, N> &vector) {
        return vector * number;
    }

    template<typename Tn>
    void operator*=(const Tn number) {
        std::transform(data.cbegin(), data.cend(), data.begin(), [&number](Tp it) { return it * static_cast<Tp>(number); });
    }

    template<typename Tn>
    friend Vector<Tp, N> operator/(const Vector<Tp, N> &vector, Tn number) {
        std::transform(vector->data.cbegin(), vector->data.cend(), vector->data.begin(),
                       [&number](Tp it) { return it / static_cast<Tp>(number); });
    }

    template<typename Tn>
    void operator/=(const Tn number) {
        std::transform(data.cbegin(), data.cend(), data.begin(), [&number](Tp it) { return it / static_cast<Tp>(number); });
    }

    friend Tp dot(const Vector<Tp, N> &vec1, const Vector<Tp, N>  &vec2) {
        return std::inner_product(vec1.data.cbegin(), vec1.data.cend(), vec2.data.begin(), static_cast<Tp>(0));
    }

    friend Tp dot(const Vector<Tp, N> &vec1, const std::array<Tp, N> &vec2) {
        if (N != vec2.size()) throw std::runtime_error("sizes are not equal\n");
        return std::inner_product(vec1.data.cbegin(), vec1.data.cend(), vec2.begin(), 0);
    }
    inline friend Tp dot(const std::array<Tp, N> &vec1, const Vector<Tp, N> &vec2) {
        return dot(vec2, vec1);
    }

    friend Tp dot(const Vector<Tp, N> &vec1, const std::vector<Tp> &vec2) {
        if (N != vec2.size()) throw std::runtime_error("sizes are not equal\n");
        return std::inner_product(vec1.data.cbegin(), vec1.data.cend(), vec2.begin(), static_cast<Tp>(0));
    }
    inline friend Tp dot(const std::vector<Tp> &vec1, const Vector<Tp, N> &vec2) {
        return dot(vec2, vec1);
    }

    inline bool operator==(const Vector<Tp, N> &other) const {
        return *data == *other.data;
    }

    inline bool operator!=(const Vector<Tp, N> &other) const {
        return *data != *other.data;
    }

    inline constexpr Tp &operator[](std::size_t index) noexcept {
        return data.operator[](index);
    }

    inline constexpr const Tp &operator[](std::size_t index) const noexcept {
        return data.operator[](index);
    }

    inline constexpr Tp &at(std::size_t index) {
        return data.at(index);
    }

    inline constexpr const Tp &at(std::size_t index) const {
        return data.at(index);
    }

    inline constexpr std::size_t size() {
        return data.size();
    }

    inline constexpr const Tp* cbegin() {
        return data.cbegin();
    }

    inline Tp* begin() {
        return data.begin();
    }

    inline constexpr const Tp* cend() {
        return data.cend();
    }

    inline Tp* end() {
        return data.end();
    }

    friend std::ostream &operator<<(std::ostream &stream, const Vector<Tp, N> &vector) {
        auto i = vector.data.cbegin();
        stream << "Vector<" << N << ">(" << *i++;
        for (; i != vector.data.cend(); ++i) {
            stream << "; " << *i;
        }

        return stream << ')';
    }


private:
    std::array<Tp, N> data;
};


//}
#endif //PERSEPTRON_VECTOR_H