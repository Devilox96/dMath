#ifndef DRUNGEKUTTA_H
#define DRUNGEKUTTA_H
//-----------------------------//
#include <stdexcept>
//-----------------------------//
///---TODO: do smth with applications to approximated right part---///
///---TODO: get rid of callbacks, substitute for function overloading---///
template <class T>
class dRungeKutta1D {
public:
    dRungeKutta1D() : mStepTime(0.0) {}
    explicit dRungeKutta1D(double tStepTime) : mStepTime(tStepTime) {}

    T solve2nd(double tTime, const T& tInit, T (*tFunc)(double tTime, const T& tArg)) {
        if (!mStepTime) {
            throw std::runtime_error("dRungeKutta1D::solve2nd: time is not provided!");
        }

        T U1 = tInit + mStepTime / 2.0 * tFunc(tTime, tInit);
        T U2 = tInit + mStepTime * tFunc(tTime + mStepTime / 2.0, U1);

        return U2;
    }
    T solve2nd(const T& tInit, T (*tFunc)(const T& tArg)) {
        T U1 = tInit + mStepTime / 2.0 * tFunc(tInit);
        T U2 = tInit + mStepTime * tFunc(U1);

        return U2;
    }
    T solve4th(double tTime, const T& tInit, T (*tFunc)(double tTime, const T& tArg)) {
        if (!mStepTime) {
            throw std::runtime_error("dRungeKutta1D::solve4th: time is not provided!");
        }

        double HalfStep = mStepTime / 2.0;

        T U1 = tInit + HalfStep * tFunc(tTime, tInit);
        T U2 = tInit + HalfStep * tFunc(tTime + HalfStep, U1);
        T U3 = tInit + mStepTime * tFunc(tTime + HalfStep, U2);
        T U4 = (-tInit + U1 + 2.0 * U2 + U3 + HalfStep * tFunc(tTime + mStepTime, U3)) / 3.0;

        return U4;
    }
    T solve4th(const T& tInit, T (*tFunc)(const T& tArg)) {
        double HalfStep = mStepTime / 2.0;

        T U1 = tInit + HalfStep * tFunc(tInit);
        T U2 = tInit + HalfStep * tFunc(U1);
        T U3 = tInit + mStepTime * tFunc(U2);
        T U4 = (-tInit + U1 + 2.0 * U2 + U3 + HalfStep * tFunc(U3)) / 3.0;

        return U4;
    }
    T solve2ndTVD(double tTime, const T& tInit, T (*tFunc)(double tTime, const T& tArg)) {
        if (!mStepTime) {
            throw std::runtime_error("dRungeKutta1D::solve2ndTVD: time is not provided!");
        }

        T U1 = tInit + mStepTime * tFunc(tTime, tInit);
        T U2 = 0.5 * (tInit + U1 + mStepTime * tFunc(tTime + mStepTime, U1));

        return U2;
    }
    T solve2ndTVD(const T& tInit, T (*tFunc)(const T& tArg)) {
        T U1 = tInit + mStepTime * tFunc(tInit);
        T U2 = 0.5 * (tInit + U1 + mStepTime * tFunc(U1));

        return U2;
    }
    T solve3rdTVD(double tTime, const T& tInit, T (*tFunc)(double tTime, const T& tArg)) {
        if (!mStepTime) {
            throw std::runtime_error("dRungeKutta1D::solve3rdTVD: time is not provided!");
        }

        T U1 = tInit + mStepTime * tFunc(tTime, tInit);
        T U2 = tInit + 0.25 * (3.0 * tInit + U1 + mStepTime * tFunc(tTime + mStepTime, U1));
        T U3 = (tInit + 2.0 * U2 + 2.0 * mStepTime * tFunc(tTime + mStepTime / 2.0, U2)) / 3.0;

        return U3;
    }
    T solve3rdTVD(const T& tInit, T (*tFunc)(const T& tArg)) {
        T U1 = tInit + mStepTime * tFunc(tInit);
        T U2 = 0.25 * (3.0 * tInit + U1 + mStepTime * tFunc(U1));
        T U3 = (tInit + 2.0 * U2 + 2.0 * mStepTime * tFunc(U2)) / 3.0;

        return U3;
    }
private:
    double mStepTime;
};
//-----------------------------//
#endif
