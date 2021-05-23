#ifndef DMATH_UTIL_H
#define DMATH_UTIL_H

#include <utility>

namespace dmath {
template<typename T>
struct RightUnaryPlus {
    T mValue;

    RightUnaryPlus(const T& tValue) : mValue(tValue) {}

    RightUnaryPlus(T&& tValue) : mValue(std::move(tValue)) {}

    constexpr T operator()(const T& tValue) const { return tValue + mValue; }
};

template<typename T>
struct LeftUnaryPlus {
    T mValue;

    LeftUnaryPlus(const T& tValue) : mValue(tValue) {}

    LeftUnaryPlus(T&& tValue) : mValue(std::move(tValue)) {}

    constexpr T operator()(const T& tValue) const { return mValue + tValue; }
};

template<typename T>
struct RightUnaryMinus {
    T mValue;

    RightUnaryMinus(const T& tValue) : mValue(tValue) {}

    RightUnaryMinus(T&& tValue) : mValue(std::move(tValue)) {}

    constexpr T operator()(const T& tValue) const { return tValue - mValue; }
};

template<typename T>
struct LeftUnaryMinus {
    T mValue;

    LeftUnaryMinus(const T& tValue) : mValue(tValue) {}

    LeftUnaryMinus(T&& tValue) : mValue(std::move(tValue)) {}

    constexpr T operator()(const T& tValue) const { return mValue - tValue; }
};

template<typename T>
struct RightUnaryMultiplies {
    T mValue;

    RightUnaryMultiplies(const T& tValue) : mValue(tValue) {}

    RightUnaryMultiplies(T&& tValue) : mValue(std::move(tValue)) {}

    constexpr T operator()(const T& tValue) const { return tValue * mValue; }
};

template<typename T>
struct LeftUnaryMultiplies {
    T mValue;

    LeftUnaryMultiplies(const T& tValue) : mValue(tValue) {}

    LeftUnaryMultiplies(T&& tValue) : mValue(std::move(tValue)) {}

    constexpr T operator()(const T& tValue) const { return mValue * tValue; }
};

template<typename T>
struct RightUnaryDivides {
    T mValue;

    RightUnaryDivides(const T& tValue) : mValue(tValue) {}

    RightUnaryDivides(T&& tValue) : mValue(std::move(tValue)) {}

    constexpr T operator()(const T& tValue) const { return tValue / mValue; }
};

template<typename T>
struct LeftUnaryDivides {
    T mValue;

    LeftUnaryDivides(const T& tValue) : mValue(tValue) {}

    LeftUnaryDivides(T&& tValue) : mValue(std::move(tValue)) {}

    constexpr T operator()(const T& tValue) const { return mValue / tValue; }
};

} // namespace dmath

#endif //DMATH_UTIL_H
