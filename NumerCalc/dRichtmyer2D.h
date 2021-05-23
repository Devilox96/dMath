#ifndef DRICHTMYER2D_H
#define DRICHTMYER2D_H
//-----------------------------//
#include <iostream>
//-----------------------------//
template <class T>
class dRichtmyer2D {
public:
    dRichtmyer2D() = default;
    ~dRichtmyer2D() = default;

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

    T solve(const T& tOffsetZero,
            const T& tX_min_2_Y,        const T& tX_pl_2_Y,
            const T& tX_Y_min_2,        const T& tX_Y_pl_2,
            const T& tX_pl_1_Y_pl_1,    const T& tX_min_1_Y_min_1,
            const T& tX_pl_1_Y_min_1,   const T& tX_min_1_Y_pl_1) {
        if (mStepTime <= 0 || mStepX <= 0 || mStepY <= 0) {
            std::cout << "ERROR! dRichtmyer2D: Incorrect steps!" << std::endl;
        }

        T HalfMinusX = firstStep(tX_min_2_Y, tOffsetZero, tX_min_1_Y_min_1, tX_min_1_Y_pl_1);
        T HalfPlusX = firstStep(tOffsetZero, tX_pl_2_Y, tX_pl_1_Y_min_1, tX_pl_1_Y_pl_1);
        T HalfMinusY = firstStep(tX_min_1_Y_min_1, tX_pl_1_Y_min_1, tX_Y_min_2, tOffsetZero);
        T HalfPlusY = firstStep(tX_min_1_Y_pl_1, tX_pl_1_Y_pl_1, tOffsetZero, tX_Y_pl_2);

        return  tOffsetZero -
                (funcX(HalfPlusX) - funcX(HalfMinusX)) * mRatioX -
                (funcY(HalfPlusY) - funcY(HalfMinusY)) * mRatioY;
    }
    T solveZwas(const T& tOffsetZero,
                const T& tX_min_1_Y,        const T& tX_pl_1_Y,
                const T& tX_Y_min_1,        const T& tX_Y_pl_1,
                const T& tX_pl_1_Y_pl_1,    const T& tX_min_1_Y_min_1,
                const T& tX_pl_1_Y_min_1,   const T& tX_min_1_Y_pl_1) {
        T Half_X_min_Y_min = firstStepZwas(tX_Y_min_1, tOffsetZero, tX_min_1_Y, tX_min_1_Y_min_1);
        T Half_X_pl_Y_min = firstStepZwas(tX_pl_1_Y_min_1, tX_pl_1_Y, tOffsetZero, tX_Y_min_1);
        T Half_X_min_Y_pl = firstStepZwas(tOffsetZero, tX_Y_pl_1, tX_min_1_Y_pl_1, tX_min_1_Y);
        T Half_X_pl_Y_pl = firstStepZwas(tX_pl_1_Y, tX_pl_1_Y_pl_1, tX_Y_pl_1, tOffsetZero);

        T GradX =   (funcX(Half_X_pl_Y_pl) - funcX(Half_X_min_Y_pl) +
                    funcX(Half_X_pl_Y_min) - funcX(Half_X_min_Y_min)) / 2;
        T GradY =   (funcY(Half_X_pl_Y_pl) - funcY(Half_X_pl_Y_min) +
                    funcY(Half_X_min_Y_pl) - funcY(Half_X_min_Y_min)) / 2;

        return tOffsetZero - GradX * mRatioX - GradY * mRatioY;
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

    //----------//

    T firstStep(const T& tOffsetMinusX, const T& tOffsetPlusX,
                const T& tOffsetMinusY, const T& tOffsetPlusY) {
        return  (tOffsetPlusX + tOffsetMinusX + tOffsetPlusY + tOffsetMinusY) / 4 -
                ((funcX(tOffsetPlusX) - funcX(tOffsetMinusX)) * mRatioX +
                (funcY(tOffsetPlusY) - funcY(tOffsetMinusY)) * mRatioY) / 2;
    }
    T firstStepZwas(const T& tBottomRight,  const T& tTopRight,
                    const T& tTopLeft,      const T& tBottomLeft) {
        return  (tBottomRight + tTopRight + tTopLeft + tBottomLeft) / 4 -
                ((funcX((tBottomRight + tTopRight) / 2) - funcX((tBottomLeft + tTopLeft) / 2)) * mRatioX +
                (funcY((tTopRight + tTopLeft) / 2) - funcY((tBottomRight + tBottomLeft) / 2)) * mRatioY) / 2;
    }
};

//-----------------------------//
#endif
