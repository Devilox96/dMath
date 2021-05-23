#ifndef DMATH_UTIL_H
#define DMATH_UTIL_H

#include <functional>
namespace dmath {
template<typename T>
struct RightUnaryPlus {
    T val_;
    RightUnaryPlus(const T& val): val_(val) {}
    RightUnaryPlus(T&& val): val_(std::move(val)) {}

    constexpr T operator()(const T& val) const { return val + val_; }
};

template<typename T>
struct LeftUnaryPlus {
    T val_;
    LeftUnaryPlus(const T& val): val_(val) {}
    LeftUnaryPlus(T&& val): val_(std::move(val)) {}

    constexpr T operator()(const T& val) const { return val_ + val; }
};

template<typename T>
struct RightUnaryMinus {
    T val_;
    RightUnaryMinus(const T& val): val_(val) {}
    RightUnaryMinus(T&& val): val_(std::move(val)) {}

    constexpr T operator()(const T& val) const { return val - val_; }
};

template<typename T>
struct LeftUnaryMinus {
    T val_;
    LeftUnaryMinus(const T& val): val_(val) {}
    LeftUnaryMinus(T&& val): val_(std::move(val)) {}

    constexpr T operator()(const T& val) const { return val_ - val; }
};

template<typename T>
struct RightUnaryMultiplies {
    T val_;
    RightUnaryMultiplies(const T& val): val_(val) {}
    RightUnaryMultiplies(T&& val): val_(std::move(val)) {}

    constexpr T operator()(const T& val) const { return val * val_; }
};

template<typename T>
struct LeftUnaryMultiplies {
    T val_;
    LeftUnaryMultiplies(const T& val): val_(val) {}
    LeftUnaryMultiplies(T&& val): val_(std::move(val)) {}

    constexpr T operator()(const T& val) const { return val_ * val; }
};

template<typename T>
struct RightUnaryDivides {
    T val_;
    RightUnaryDivides(const T& val): val_(val) {}
    RightUnaryDivides(T&& val): val_(std::move(val)) {}

    constexpr T operator()(const T& val) const { return val / val_; }
};

template<typename T>
struct LeftUnaryDivides {
    T val_;
    LeftUnaryDivides(const T& val): val_(val) {}
    LeftUnaryDivides(T&& val): val_(std::move(val)) {}

    constexpr T operator()(const T& val) const { return val_ / val; }
};

} // dmath

#endif //DMATH_UTIL_H
