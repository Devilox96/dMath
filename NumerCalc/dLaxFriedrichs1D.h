#ifndef DLAXFRIEDRICHS1D_H
#define DLAXFRIEDRICHS1D_H
//-----------------------------//
#include "dVectors.h" //---dMath/Core/dVectors.h---//
//-----------------------------//
template <class T>
class dLaxFriedrichs1D {
public:
    dLaxFriedrichs1D() = default;
    ~dLaxFriedrichs1D() = default;

    //----------//

    void setTimeStep(double tStep) {
        mTimeStep = tStep;
    }
    void setXStep(double tStep) {
        mXStep = tStep;
    }

    //----------//

    T solve(const T& tVecMinus, const T& tVecPlus) {
        if (mTimeStep <= 0 || mXStep <= 0) {
            std::cout << "ERROR! dLaxFriedrichs1D: Incorrect steps!" << std::endl;
        }

        return  (tVecMinus + tVecPlus) / 2.0 -
                (func(tVecPlus) - func(tVecMinus)) * mTimeStep / mXStep / 2.0;
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
