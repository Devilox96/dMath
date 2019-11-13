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
        mTimeStep = tStep;
    }
    void setXStep(double tStep) {
        mXStep = tStep;
    }

    //----------//

    T solve(const T& tTimeMinus, const T& tOffsetMinus, const T& tOffsetPlus) {
        if (mTimeStep <= 0 || mXStep <= 0) {
            std::cout << "ERROR! dLeapFrog1D: Incorrect steps!" << std::endl;
        }

        return tTimeMinus -
               (func(tOffsetPlus) - func(tOffsetMinus)) * mTimeStep / mXStep;
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
