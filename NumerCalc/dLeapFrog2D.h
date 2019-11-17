#ifndef DLEAPFROG2D_H
#define DLEAPFROG2D_H
//-----------------------------//
#include <iostream>
//-----------------------------//
template <class T>
class dLeapFrog2D {
public:
    dLeapFrog2D() = default;
    ~dLeapFrog2D() = default;

    //----------//

    void setTimeStep(double tStep) {
        mStepTime = tStep;

        if (mStepX != 0) {
            mRatioX = mStepTime / mStepX;
        }

        if (mStepY != 0) {
            mRatioY = mStepTime / mStepY;
        }
    }
    void setXStep(double tStep) {
        mStepX = tStep;

        if (mStepX != 0) {
            mRatioX = mStepTime / mStepX;
        }
    }
    void setYStep(double tStep) {
        mStepY = tStep;

        if (mStepY != 0) {
            mRatioY = mStepTime / mStepY;
        }
    }

    //----------//

    T solve(const T& tTimeMinus,
            const T& tOffsetMinusX, const T& tOffsetPlusX,
            const T& tOffsetMinusY, const T& tOffsetPlusY) {
        if (mStepTime <= 0 || mStepX <= 0 || mStepY <= 0) {
            std::cout << "ERROR! dLeapFrog2D: Incorrect steps!" << std::endl;
        }

        return  tTimeMinus -
                (funcX(tOffsetPlusX) - funcX(tOffsetMinusX)) * mRatioX -
                (funcY(tOffsetPlusY) - funcY(tOffsetMinusY)) * mRatioY;
    }
protected:
    virtual T funcX(const T& tVal) {
        return tVal;
    }
    virtual T funcY(const T& tVal) {
        return tVal;
    }
private:
    double mStepTime = 0.0;

    double mStepX = 0.0;
    double mStepY = 0.0;

    double mRatioX = 0.0; //---mStepTime / mStepX---//
    double mRatioY = 0.0; //---mStepTime / mStepY---//
};
//-----------------------------//
#endif
