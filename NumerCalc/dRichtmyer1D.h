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
        mStepTime = tStep;

        if (mStepX != 0) {
            mRatioX = mStepTime / mStepX;
        }
    }
    void setXStep(double tStep) {
        mStepX = tStep;

        if (mStepX != 0) {
            mRatioX = mStepTime / mStepX;
        }
    }

    //----------//

    T solve(const T& tOffsetMinus, const T& tOffsetZero, const T& tOffsetPlus) {
        if (mStepTime <= 0 || mStepX <= 0) {
            std::cout << "ERROR! dRichtmyer1D: Incorrect steps!" << std::endl;
        }

        T HalfMinus = (tOffsetMinus + tOffsetZero -
                       (func(tOffsetZero) - func(tOffsetMinus)) * mRatioX) / 2;
        T HalfPlus = (tOffsetPlus + tOffsetZero -
                      (func(tOffsetPlus) - func(tOffsetZero)) * mRatioX) / 2;

        return tOffsetZero -
               (func(HalfPlus) - func(HalfMinus)) * mRatioX;
    }
protected:
    virtual T func(const T& tValue) {
        return tValue;
    }
private:
    double mStepTime = 0.0;
    double mStepX = 0.0;
    double mRatioX = 0.0; //---mStepTime / mStepX---//
};
//-----------------------------//
#endif
