#ifndef DLEAPFROG1D_H
#define DLEAPFROG1D_H
//-----------------------------//
#include <iostream>
//-----------------------------//
template <class T>
class dLeapFrog1D {
public:
    dLeapFrog1D() = default;
    ~dLeapFrog1D() = default;

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

    T solve(const T& tTimeMinus, const T& tOffsetMinus, const T& tOffsetPlus) {
        if (mStepTime <= 0 || mStepX <= 0) {
            std::cout << "ERROR! dLeapFrog1D: Incorrect steps!" << std::endl;
        }

        return tTimeMinus -
               (func(tOffsetPlus) - func(tOffsetMinus)) * mRatioX;
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
