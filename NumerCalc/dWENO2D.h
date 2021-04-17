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
public:
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

    /**
     *
     * @param tOffsetZero Grid[i][j]
     * @param tX_min_1_Y Grid[i - 1][j]
     * @param tX_min_2_Y Grid[i - 2][j]
     * @param tX_pl_1_Y Grid[i + 1][j]
     * @param tX_pl_2_Y Grid[i + 2][j]
     * @param tX_Y_min_1 Grid[i][j - 1]
     * @param tX_Y_min_2 Grid[i][j - 2]
     * @param tX_Y_pl_1 Grid[i][j + 1]
     * @param tX_Y_pl_2 Grid[i][j + 2]
     * @param tMaxAlpha Maximum module of all eigen values of the matrix,
     * used for Lax-Friedrichs flux splitting
     */
    T solve(const T& tOffsetZero,
               const T& tX_min_1_Y,         const T& tX_min_2_Y,
               const T& tX_pl_1_Y,          const T& tX_pl_2_Y,
               const T& tX_Y_min_1,         const T& tX_Y_min_2,
               const T& tX_Y_pl_1,          const T& tX_Y_pl_2,
               const T& tMaxAlpha) {
        auto PlusX = approximate(
                (funcX(tX_min_1_Y) + tMaxAlpha * tX_min_1_Y) / 2.0,
                (funcX(tOffsetZero) + tMaxAlpha * tOffsetZero) / 2.0,
                (funcX(tX_pl_1_Y) + tMaxAlpha * tX_pl_1_Y) / 2.0,
                (funcX(tOffsetZero) - tMaxAlpha * tOffsetZero) / 2.0,
                (funcX(tX_pl_1_Y) - tMaxAlpha * tX_pl_1_Y) / 2.0,
                (funcX(tX_pl_2_Y) - tMaxAlpha * tX_pl_2_Y) / 2.0
        );
        auto MinusX = approximate(
                (funcX(tX_min_2_Y) + tMaxAlpha * tX_min_2_Y) / 2.0,
                (funcX(tX_min_1_Y) + tMaxAlpha * tX_min_1_Y) / 2.0,
                (funcX(tOffsetZero) + tMaxAlpha * tOffsetZero) / 2.0,
                (funcX(tX_min_1_Y) - tMaxAlpha * tX_min_1_Y) / 2.0,
                (funcX(tOffsetZero) - tMaxAlpha * tOffsetZero) / 2.0,
                (funcX(tX_pl_1_Y) - tMaxAlpha * tX_pl_1_Y) / 2.0
        );

        auto PlusY = approximate(
                (funcY(tX_Y_min_1) + tMaxAlpha * tX_Y_min_1) / 2.0,
                (funcY(tOffsetZero) + tMaxAlpha * tOffsetZero) / 2.0,
                (funcY(tX_Y_pl_1) + tMaxAlpha * tX_Y_pl_1) / 2.0,
                (funcY(tOffsetZero) - tMaxAlpha * tOffsetZero) / 2.0,
                (funcY(tX_Y_pl_1) - tMaxAlpha * tX_Y_pl_1) / 2.0,
                (funcY(tX_Y_pl_2) - tMaxAlpha * tX_Y_pl_2) / 2.0
        );
        auto MinusY = approximate(
                (funcY(tX_Y_min_2) + tMaxAlpha * tX_Y_min_2) / 2.0,
                (funcY(tX_Y_min_1) + tMaxAlpha * tX_Y_min_1) / 2.0,
                (funcY(tOffsetZero) + tMaxAlpha * tOffsetZero) / 2.0,
                (funcY(tX_Y_min_1) - tMaxAlpha * tX_Y_min_1) / 2.0,
                (funcY(tOffsetZero) - tMaxAlpha * tOffsetZero) / 2.0,
                (funcY(tX_Y_pl_1) - tMaxAlpha * tX_Y_pl_1) / 2.0
        );

        return tOffsetZero - ((PlusX - MinusX) * mRatioX + (PlusY - MinusY) * mRatioY);
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

    double mRatioX = 0.0;           //---mStepTime / mStepX---//
    double mRatioY = 0.0;           //---mStepTime / mStepY---//

    //----------//
    
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
