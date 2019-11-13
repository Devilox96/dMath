#ifndef DRICHTMYER1D_H
#define DRICHTMYER1D_H
//-----------------------------//
#include <iostream>
//-----------------------------//
template <class T>
class dRichtmyer1D {
public:
    dRichtmyer1D() = default;
    ~dRichtmyer1D() = default;

    //----------//

    void setTimeStep(double tStep) {
        mTimeStep = tStep;
    }
    void setXStep(double tStep) {
        mXStep = tStep;
    }

    //----------//

    T solve(const T& tOffsetMinus, const T& tOffsetZero, const T& tOffsetPlus) {
        if (mTimeStep <= 0 || mXStep <= 0) {
            std::cout << "ERROR! dRichtmyer1D: Incorrect steps!" << std::endl;
        }

        T HalfMinus =   (tOffsetMinus + tOffsetZero -
                        (func(tOffsetZero) - func(tOffsetMinus)) * mTimeStep / mXStep) / 2;
        T HalfPlus =    (tOffsetPlus + tOffsetZero -
                        (func(tOffsetPlus) - func(tOffsetZero)) * mTimeStep / mXStep) / 2;

        return tOffsetZero -
               (func(HalfPlus) - func(HalfMinus)) * mTimeStep / mXStep;
    }
protected:
    virtual T func(const T& tVal) {
        return tVal;
    }
private:
    double mTimeStep = 0.0;
    double mXStep = 0.0;
};
//-----------------------------//
#endif
