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
        mTimeStep = tStep;
    }
    void setXStep(double tStep) {
        mXStep = tStep;
    }

    //----------//

    T solve(const T& tOffsetMinus, const T& tOffsetPlus) {
        if (mTimeStep <= 0 || mXStep <= 0) {
            std::cout << "ERROR! dLaxFriedrichs1D: Incorrect steps!" << std::endl;
        }

        return (tOffsetPlus + tOffsetMinus) / 2.0 +
               (func(tOffsetPlus) - func(tOffsetMinus)) * mTimeStep / mXStep / 2.0;
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
