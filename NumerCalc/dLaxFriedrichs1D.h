#ifndef DLAXFRIEDRICHS1D_H
#define DLAXFRIEDRICHS1D_H
//-----------------------------//
#include <iostream>
//-----------------------------//
template <class T>
class dLaxFriedrichs1D {
public:
    dLaxFriedrichs1D() = default;
    ~dLaxFriedrichs1D() = default;

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

    T solve(const T& tOffsetMinus, const T& tOffsetPlus) {
        if (mStepTime <= 0 || mStepX <= 0) {
            std::cout << "ERROR! dLaxFriedrichs1D: Incorrect steps!" << std::endl;
        }

        return (tOffsetPlus + tOffsetMinus) / 2.0 +
               (func(tOffsetPlus) - func(tOffsetMinus)) * mRatioX / 2.0;
    }
protected:
    virtual T func(const T& tVal) {
        return tVal;
    }
private:
    double mStepTime = 0.0;
    double mStepX = 0.0;
    double mRatioX = 0.0; //---mStepTime / mStepX---//
};
//-----------------------------//
#endif
