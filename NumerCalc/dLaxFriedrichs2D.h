#ifndef DLAXFRIEDRICHS2D_H
#define DLAXFRIEDRICHS2D_H
//-----------------------------//
#include <iostream>
//-----------------------------//
template <class T>
class dLaxFriedrichs2D {
public:
    dLaxFriedrichs2D() = default;
    ~dLaxFriedrichs2D() = default;

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

    T solve(const T& tOffsetMinusX, const T& tOffsetPlusX,
            const T& tOffsetMinusY, const T& tOffsetPlusY) {
        if (mStepTime <= 0 || mStepX <= 0 || mStepY <= 0) {
            std::cout << "ERROR! dLaxFriedrichs2D: Incorrect steps!" << std::endl;
        }

        return (tOffsetMinusX + tOffsetPlusX + tOffsetMinusY + tOffsetPlusY) / 4.0 -
                ((funcX(tOffsetPlusX) - funcX(tOffsetMinusX)) * mRatioX +
                (funcY(tOffsetPlusY) - funcY(tOffsetMinusY)) * mRatioY) / 2.0;
    }
protected:
    virtual T funcX(const T& tValue) {
        return tValue;
    }
    virtual T funcY(const T& tValue) {
        return tValue;
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
