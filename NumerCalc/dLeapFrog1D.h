#ifndef DLEAPFROG1D_H
#define DLEAPFROG1D_H
//-----------------------------//
#include "dVectors.h" //---dMath/Core/dVectors.h---//
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

    T solve(const T& tVecTimeMinus, const T& tVecXMinus, const T& tVecXPlus) {
        if (mTimeStep <= 0 || mXStep <= 0) {
            std::cout << "ERROR! dLeapFrog1D: Incorrect steps!" << std::endl;
        }

        return tVecTimeMinus -
               (func(tVecXPlus) - func(tVecXMinus)) * mTimeStep / mXStep;
    }
protected:
    virtual T func(const T& tVec) {
        return tVec;
    }
private:
    double mTimeStep = 0.0;
    double mXStep = 0.0;
};
//-----------------------------//
#endif
