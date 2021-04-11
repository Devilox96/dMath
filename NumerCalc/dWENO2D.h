//
// Created by devilox on 4/10/21.
//
//-----------------------------//
#ifndef DMATH_DWENO2D_H
#define DMATH_DWENO2D_H
//-----------------------------//
#include <cmath>
//-----------------------------//
/**
 * The class implements solver with Lax-Friedrichs flux splitting.
 * It is possible to add more variations using enum if needed.
 */
template <class T>
class dWENO2D {
protected:
    virtual T funcX(const T& tVal) {
        return tVal;
    }
    virtual T funcY(const T& tVal) {
        return tVal;
    }
private:
    ///---TODO: substitute pow for plain multiplication in case of single precision---//
    T approximate(
            const T& tPosVal_minus_1,
            const T& tPosVal,
            const T& tPosVal_plus_1,
            const T& tNegVal,
            const T& tNegVal_plus_1,
            const T& tNegVal_plus_2) {
        T fPlus, fMinus;
        T Beta0, Beta1;
        double d0, d1;
        T Alpha0, Alpha1;
        T Omega0, Omega1;

        //----------//

        Beta0 = pow(tPosVal_minus_1 - tPosVal, 2.0);
        Beta1 = pow(tPosVal - tPosVal_plus_1, 2.0);

        d0 = 1.0 / 3.0;
        d1 = 2.0 / 3.0;

        Alpha0 = d0 / pow(1.0e-06 + Beta0, 2.0);
        Alpha1 = d1 / pow(1.0e-06 + Beta1, 2.0);

        Omega0 = Alpha0 / (Alpha0 + Alpha1);
        Omega1 = Alpha1 / (Alpha0 + Alpha1);

        fPlus = Omega0 * (-0.5 * tPosVal_minus_1 + 1.5 * tPosVal) + Omega1 * (0.5 * tPosVal + 0.5 * tPosVal_plus_1);

        //----------//

        Beta0 = pow(tNegVal - tNegVal_plus_1, 2.0);
        Beta1 = pow(tNegVal_plus_1 - tNegVal_plus_2, 2.0);

        d0 = 2.0 / 3.0;
        d1 = 1.0 / 3.0;

        Alpha0 = d0 / pow(1.0e-06 + Beta0, 2.0);
        Alpha1 = d1 / pow(1.0e-06 + Beta1, 2.0);

        Omega0 = Alpha0 / (Alpha0 + Alpha1);
        Omega1 = Alpha1 / (Alpha0 + Alpha1);

        fMinus = Omega0 * (0.5 * tNegVal + 0.5 * tNegVal_plus_1) + Omega1 * (1.5 * tNegVal_plus_1 - 0.5 * tNegVal_plus_2);

        //----------//

        return fPlus + fMinus;
    }
};
//-----------------------------//
#endif
