#include <iostream>
#include <cmath>
#include <utility>
#include <ctime>
#include <fstream>

#include "Core/dVector.h"
#include "Core/dMatrix.h"

//-----------------------------//
#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
//-----------------------------//
#include "Renderer.h"
//-----------------------------//

using dGrid = std::vector <std::vector <dVector <double, 6>>>;
using dVec = dVector <double, 6>;

class Solver {
public:
    Solver() {
        GridCurrentData.resize(mGridX + mOffsetXL + mOffsetXR);
        GridTempData.resize(mGridX + mOffsetXL + mOffsetXR);

        Bottom.resize(mGridX + mOffsetXL + mOffsetXR);

        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            GridCurrentData[i].resize(mGridY + mOffsetYU + mOffsetYD);
            GridTempData[i].resize(mGridY + mOffsetYU + mOffsetYD);

            Bottom[i].resize(mGridY + mOffsetYU + mOffsetYD);
        }

        AlphaX.resize(mGridY + mOffsetYU + mOffsetYD);
        AlphaY.resize(mGridX + mOffsetXL + mOffsetXR);

        Elevation.resize(mGridX);

        //---Divergence---//
        mDivergence.resize(mGridX * mGridY);
        B.resize(mGridX * mGridY);
        r.resize(mGridX * mGridY);
        p.resize(mGridX * mGridY);
        Ap.resize(mGridX * mGridY);
        ResOpt.resize(mGridX * mGridY);

        for (size_t i = 0; i < mGridX * mGridY; i++) {
            mDivergence[i] = 0.0;
            B[i] = 0.0;
            r[i] = 0.0;
            p[i] = 0.0;
            Ap[i] = 0.0;
            ResOpt[i] = 0.0;
        }
        //---Divergence---//

        for (auto& iRow : Elevation) {
            iRow.resize(mGridY);
        }

        initCoriolis();
        initFields();
        bottomFunc();
        initConditions();

        GridTempData = GridCurrentData;
        GridTempData2 = GridCurrentData;

        updateBoundaries();
        std::swap(CurrentData, TempData);

        EOutput.open("Amplitude.dat");
    }
    ~Solver() {
        CurrentData = nullptr;
        TempData = nullptr;

        EOutput.close();
    }

    void solve() {
//        std::vector <double> AlphaX(mGridY + mOffsetYU + mOffsetYD);
//        std::vector <double> AlphaY(mGridX + mOffsetXL + mOffsetXR);

//        double TimePassed = 0.0;

//        for (int iTime = 0; iTime < mTimeLimit; iTime++) {
            mMaxAlpha = 0.0;

            for (size_t iX = 0; iX < mGridX + mOffsetXL + mOffsetXR; iX++) {
                for (size_t iY = 0; iY < mGridY + mOffsetYU + mOffsetYD; iY++) {
                    double vx = (*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0];
                    double vy = (*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0];
                    double Bx = (*CurrentData)[iX][iY][3] / (*CurrentData)[iX][iY][0];
                    double By = (*CurrentData)[iX][iY][4] / (*CurrentData)[iX][iY][0];
                    double SqrX = sqrt(pow(Bx, 2.0) + 9.81 * (*CurrentData)[iX][iY][0]);
                    double SqrY = sqrt(pow(By, 2.0) + 9.81 * (*CurrentData)[iX][iY][0]);

                    mMaxAlpha = std::max(fabs(vx + Bx), mMaxAlpha);
                    mMaxAlpha = std::max(fabs(vx - Bx), mMaxAlpha);
                    mMaxAlpha = std::max(fabs(vx + SqrX), mMaxAlpha);
                    mMaxAlpha = std::max(fabs(vx - SqrX), mMaxAlpha);

                    mMaxAlpha = std::max(fabs(vy + By), mMaxAlpha);
                    mMaxAlpha = std::max(fabs(vy - By), mMaxAlpha);
                    mMaxAlpha = std::max(fabs(vy + SqrY), mMaxAlpha);
                    mMaxAlpha = std::max(fabs(vy - SqrY), mMaxAlpha);
                }
            }

//            if (mMaxAlpha > 500) {
//                break;
//            }

//            mStepTime = 0.2 * mStepX / mMaxAlpha;
            TimePassed += mStepTime;

            if (int(TimePassed) % (60 * 60) == 0) {
                std::cout << "Time: " << TimePassed / 3600 / 24 << " Step - " << mStepTime << " Alpha - " << mMaxAlpha
                          << std::endl;
            }

            for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
                for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                    dVec Val = (*CurrentData)[i][j];

                    dVec Val_xpl_2 = (*CurrentData)[i + 2][j];
                    dVec Val_xpl_1 = (*CurrentData)[i + 1][j];
                    dVec Val_xmin_1 = (*CurrentData)[i - 1][j];
                    dVec Val_xmin_2 = (*CurrentData)[i - 2][j];

                    dVec Val_ypl_2 = (*CurrentData)[i][j + 2];
                    dVec Val_ypl_1 = (*CurrentData)[i][j + 1];
                    dVec Val_ymin_1 = (*CurrentData)[i][j - 1];
                    dVec Val_ymin_2 = (*CurrentData)[i][j - 2];

                    if (i == mOffsetXL) {
                        Val_xpl_2[1] = Val[1] / Val[0] * Val_xpl_2[0];
                        Val_xpl_1[1] = Val[1] / Val[0] * Val_xpl_1[0];
                        Val_xmin_1[1] = Val[1] / Val[0] * Val_xmin_1[0];
                        Val_xmin_2[1] = Val[1] / Val[0] * Val_xmin_2[0];

                        Val_xpl_2[2] = Val[2] / Val[0] * Val_xpl_2[0];
                        Val_xpl_1[2] = Val[2] / Val[0] * Val_xpl_1[0];
                        Val_xmin_1[2] = Val[2] / Val[0] * Val_xmin_1[0];
                        Val_xmin_2[2] = Val[2] / Val[0] * Val_xmin_2[0];
                    }

                    if (j == mOffsetYU || j == mGridY + mOffsetYU - 1) {
                        Val_ypl_2[1] = Val[1] / Val[0] * Val_ypl_2[0];
                        Val_ypl_1[1] = Val[1] / Val[0] * Val_ypl_1[0];
                        Val_ymin_1[1] = Val[1] / Val[0] * Val_ymin_1[0];
                        Val_ymin_2[1] = Val[1] / Val[0] * Val_ymin_2[0];

                        Val_ypl_2[3] = Val[3] / Val[0] * Val_ypl_2[0];
                        Val_ypl_1[3] = Val[3] / Val[0] * Val_ypl_1[0];
                        Val_ymin_1[3] = Val[3] / Val[0] * Val_ymin_1[0];
                        Val_ymin_2[3] = Val[3] / Val[0] * Val_ymin_2[0];

                        Val_ypl_2[5] = Val[5] / Val[0] * Val_ypl_2[0];
                        Val_ypl_1[5] = Val[5] / Val[0] * Val_ypl_1[0];
                        Val_ymin_1[5] = Val[5] / Val[0] * Val_ymin_1[0];
                        Val_ymin_2[5] = Val[5] / Val[0] * Val_ymin_2[0];
                    }

                    auto PlusX = WENO(
                            (funcX(Val_xmin_1) + mMaxAlpha * Val_xmin_1) / 2.0,
                            (funcX(Val) + mMaxAlpha * Val) / 2.0,
                            (funcX(Val_xpl_1) + mMaxAlpha * Val_xpl_1) / 2.0,
                            (funcX(Val) - mMaxAlpha * Val) / 2.0,
                            (funcX(Val_xpl_1) - mMaxAlpha * Val_xpl_1) / 2.0,
                            (funcX(Val_xpl_2) - mMaxAlpha * Val_xpl_2) / 2.0
                    );
                    auto MinusX = WENO(
                            (funcX(Val_xmin_2) + mMaxAlpha * Val_xmin_2) / 2.0,
                            (funcX(Val_xmin_1) + mMaxAlpha * Val_xmin_1) / 2.0,
                            (funcX(Val) + mMaxAlpha * Val) / 2.0,
                            (funcX(Val_xmin_1) - mMaxAlpha * Val_xmin_1) / 2.0,
                            (funcX(Val) - mMaxAlpha * Val) / 2.0,
                            (funcX(Val_xpl_1) - mMaxAlpha * Val_xpl_1) / 2.0
                    );

                    auto PlusY = WENO(
                            (funcY(Val_ymin_1) + mMaxAlpha * Val_ymin_1) / 2.0,
                            (funcY(Val) + mMaxAlpha * Val) / 2.0,
                            (funcY(Val_ypl_1) + mMaxAlpha * Val_ypl_1) / 2.0,
                            (funcY(Val) - mMaxAlpha * Val) / 2.0,
                            (funcY(Val_ypl_1) - mMaxAlpha * Val_ypl_1) / 2.0,
                            (funcY(Val_ypl_2) - mMaxAlpha * Val_ypl_2) / 2.0
                    );
                    auto MinusY = WENO(
                            (funcY(Val_ymin_2) + mMaxAlpha * Val_ymin_2) / 2.0,
                            (funcY(Val_ymin_1) + mMaxAlpha * Val_ymin_1) / 2.0,
                            (funcY(Val) + mMaxAlpha * Val) / 2.0,
                            (funcY(Val_ymin_1) - mMaxAlpha * Val_ymin_1) / 2.0,
                            (funcY(Val) - mMaxAlpha * Val) / 2.0,
                            (funcY(Val_ypl_1) - mMaxAlpha * Val_ypl_1) / 2.0
                    );

                    (*TempData)[i][j] =
                            (*CurrentData)[i][j] -
                            mStepTime *
                            ((PlusX - MinusX) / mStepX + (PlusY - MinusY) / mStepY -
                            source(i, j));
                }
            }

            updateBoundaries();
            std::swap(TempData2, TempData);

            for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
                for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                    dVec Val = (*TempData2)[i][j];

                    dVec Val_xpl_2 = (*TempData2)[i + 2][j];
                    dVec Val_xpl_1 = (*TempData2)[i + 1][j];
                    dVec Val_xmin_1 = (*TempData2)[i - 1][j];
                    dVec Val_xmin_2 = (*TempData2)[i - 2][j];

                    dVec Val_ypl_2 = (*TempData2)[i][j + 2];
                    dVec Val_ypl_1 = (*TempData2)[i][j + 1];
                    dVec Val_ymin_1 = (*TempData2)[i][j - 1];
                    dVec Val_ymin_2 = (*TempData2)[i][j - 2];

                    if (i == mOffsetXL) {
                        Val_xpl_2[1] = Val[1] / Val[0] * Val_xpl_2[0];
                        Val_xpl_1[1] = Val[1] / Val[0] * Val_xpl_1[0];
                        Val_xmin_1[1] = Val[1] / Val[0] * Val_xmin_1[0];
                        Val_xmin_2[1] = Val[1] / Val[0] * Val_xmin_2[0];

                        Val_xpl_2[2] = Val[2] / Val[0] * Val_xpl_2[0];
                        Val_xpl_1[2] = Val[2] / Val[0] * Val_xpl_1[0];
                        Val_xmin_1[2] = Val[2] / Val[0] * Val_xmin_1[0];
                        Val_xmin_2[2] = Val[2] / Val[0] * Val_xmin_2[0];
                    }

                    if (j == mOffsetYU || j == mGridY + mOffsetYU - 1) {
                        Val_ypl_2[1] = Val[1] / Val[0] * Val_ypl_2[0];
                        Val_ypl_1[1] = Val[1] / Val[0] * Val_ypl_1[0];
                        Val_ymin_1[1] = Val[1] / Val[0] * Val_ymin_1[0];
                        Val_ymin_2[1] = Val[1] / Val[0] * Val_ymin_2[0];

                        Val_ypl_2[3] = Val[3] / Val[0] * Val_ypl_2[0];
                        Val_ypl_1[3] = Val[3] / Val[0] * Val_ypl_1[0];
                        Val_ymin_1[3] = Val[3] / Val[0] * Val_ymin_1[0];
                        Val_ymin_2[3] = Val[3] / Val[0] * Val_ymin_2[0];

                        Val_ypl_2[5] = Val[5] / Val[0] * Val_ypl_2[0];
                        Val_ypl_1[5] = Val[5] / Val[0] * Val_ypl_1[0];
                        Val_ymin_1[5] = Val[5] / Val[0] * Val_ymin_1[0];
                        Val_ymin_2[5] = Val[5] / Val[0] * Val_ymin_2[0];
                    }

                    auto PlusX = WENO(
                            (funcX(Val_xmin_1) + mMaxAlpha * Val_xmin_1) / 2.0,
                            (funcX(Val) + mMaxAlpha * Val) / 2.0,
                            (funcX(Val_xpl_1) + mMaxAlpha * Val_xpl_1) / 2.0,
                            (funcX(Val) - mMaxAlpha * Val) / 2.0,
                            (funcX(Val_xpl_1) - mMaxAlpha * Val_xpl_1) / 2.0,
                            (funcX(Val_xpl_2) - mMaxAlpha * Val_xpl_2) / 2.0
                    );
                    auto MinusX = WENO(
                            (funcX(Val_xmin_2) + mMaxAlpha * Val_xmin_2) / 2.0,
                            (funcX(Val_xmin_1) + mMaxAlpha * Val_xmin_1) / 2.0,
                            (funcX(Val) + mMaxAlpha * Val) / 2.0,
                            (funcX(Val_xmin_1) - mMaxAlpha * Val_xmin_1) / 2.0,
                            (funcX(Val) - mMaxAlpha * Val) / 2.0,
                            (funcX(Val_xpl_1) - mMaxAlpha * Val_xpl_1) / 2.0
                    );

                    auto PlusY = WENO(
                            (funcY(Val_ymin_1) + mMaxAlpha * Val_ymin_1) / 2.0,
                            (funcY(Val) + mMaxAlpha * Val) / 2.0,
                            (funcY(Val_ypl_1) + mMaxAlpha * Val_ypl_1) / 2.0,
                            (funcY(Val) - mMaxAlpha * Val) / 2.0,
                            (funcY(Val_ypl_1) - mMaxAlpha * Val_ypl_1) / 2.0,
                            (funcY(Val_ypl_2) - mMaxAlpha * Val_ypl_2) / 2.0
                    );
                    auto MinusY = WENO(
                            (funcY(Val_ymin_2) + mMaxAlpha * Val_ymin_2) / 2.0,
                            (funcY(Val_ymin_1) + mMaxAlpha * Val_ymin_1) / 2.0,
                            (funcY(Val) + mMaxAlpha * Val) / 2.0,
                            (funcY(Val_ymin_1) - mMaxAlpha * Val_ymin_1) / 2.0,
                            (funcY(Val) - mMaxAlpha * Val) / 2.0,
                            (funcY(Val_ypl_1) - mMaxAlpha * Val_ypl_1) / 2.0
                    );

                    (*TempData)[i][j] =
                            0.5 * (*CurrentData)[i][j] +
                            0.5 * (*TempData2)[i][j] -
                            0.5 * mStepTime *
                            ((PlusX - MinusX) / mStepX + (PlusY - MinusY) / mStepY -
                             source(i, j));
                }
            }

            updateBoundaries();

            std::swap(CurrentData, TempData);

            for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
//                    Elevation[i - mOffsetXL][j - mOffsetYU] = (*CurrentData)[i][j][0] + Bottom[i][j];
                    double dBx_dx = (
                            (*CurrentData)[i + 1][j][3] / (*CurrentData)[i + 1][j][0] -
                            (*CurrentData)[i - 1][j][3] / (*CurrentData)[i - 1][j][0]
                            ) / (mStepX * 2.0);
                    double dBy_dy = (
                            (*CurrentData)[i][j + 1][4] / (*CurrentData)[i][j + 1][0] -
                            (*CurrentData)[i][j - 1][4] / (*CurrentData)[i][j - 1][0]
                            ) / (mStepY * 2.0);

                    Elevation[i - mOffsetXL][j - mOffsetYU] = dBx_dx + dBy_dy;
                }
            }

        for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
            for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                double dBx_dx = (
                        (*CurrentData)[i + 1][j][3] / (*CurrentData)[i + 1][j][0] -
                        (*CurrentData)[i - 1][j][3] / (*CurrentData)[i - 1][j][0]
                        ) / (mStepX * 2.0);
                double dBy_dy = (
                        (*CurrentData)[i][j + 1][4] / (*CurrentData)[i][j + 1][0] -
                        (*CurrentData)[i][j - 1][4] / (*CurrentData)[i][j - 1][0]
                        ) / (mStepY * 2.0);

                B[(i - mOffsetXL) * mGridY + j - mOffsetYU] = -(dBx_dx + dBy_dy) * mStepX * mStepX;
                mDivergence[(i - mOffsetXL) * mGridY + j - mOffsetYU] = 0.0;
            }
        }

        //---Poisson---//

        double rsold = 0.0;
        AxOpt(mDivergence, ResOpt, mGridY, mGridX);

        for (size_t i = 0; i < mGridX * mGridY; i++) {
            r[i] = B[i] - ResOpt[i];
            p[i] = r[i];

            rsold += r[i] * r[i];
        }

        for (size_t i = 0; i < mGridX * mGridY; i++) {
            AxOpt(p, Ap, mGridY, mGridX);
            double C = 0.0;

            for (size_t j = 0; j < Ap.size(); j++) {
                C += p[j] * Ap[j];

//                std::cout << p[j] << " " << Ap[j] << std::endl;
            }

            double Alpha = rsold / C;

            for (size_t j = 0; j < Ap.size(); j++) {
                mDivergence[j] += Alpha * p[j];
                r[j] -= Alpha * Ap[j];
            }

            double rsnew = 0.0;

            for (size_t j = 0; j < Ap.size(); j++) {
                rsnew += r[j] * r[j];
            }

            if (sqrt(rsnew) < 1.0e-10) {
                break;
            }

            double Ratio = rsnew / rsold;

            for (size_t j = 0; j < Ap.size(); j++) {
                p[j] = r[j] + Ratio * p[j];
            }

            rsold = rsnew;

//            std::cout << i << ": " << (mDivergence[6350] - mDivergence[6349]) / mStepX << " " << Ratio << std::endl;
        }

        for (size_t i = mOffsetXL + 1; i < mGridX + mOffsetXL - 1; i++) {
            for (size_t j = mOffsetYU + 1; j < mGridY + mOffsetYU - 1; j++) {
                (*CurrentData)[i][j][3] = (
                        (*CurrentData)[i][j][3] / (*CurrentData)[i][j][0] -
                        (mDivergence[(i - mOffsetXL + 1) * mGridY + j - mOffsetYU] - mDivergence[(i - mOffsetXL - 1) * mGridY + j - mOffsetYU]) / 2.0 / mStepX
                        ) * (*CurrentData)[i][j][0];
                (*CurrentData)[i][j][4] = (
                        (*CurrentData)[i][j][4] / (*CurrentData)[i][j][0] -
                        (mDivergence[(i - mOffsetXL) * mGridY + j - mOffsetYU + 1] - mDivergence[(i - mOffsetXL) * mGridY + j - mOffsetYU - 1]) / 2.0 / mStepX
                        ) * (*CurrentData)[i][j][0];
            }
        }

        for (size_t j = mOffsetYU + 1; j < mGridY + mOffsetYU - 1; j++) {
            (*CurrentData)[mOffsetXL][j][3] = (
                    (*CurrentData)[mOffsetXL][j][3] / (*CurrentData)[mOffsetXL][j][0] -
                    (mDivergence[mGridY + j - mOffsetYU] - mDivergence[(mGridX - 1) * mGridY + j - mOffsetYU]) / 2.0 / mStepX
                    ) * (*CurrentData)[mOffsetXL][j][0];
            (*CurrentData)[mGridX + mOffsetXL - 1][j][3] = (
                    (*CurrentData)[mGridX + mOffsetXL - 1][j][3] / (*CurrentData)[mGridX + mOffsetXL - 1][j][0] -
                    (mDivergence[mGridY + j - mOffsetYU] - mDivergence[(mGridX + mOffsetXL - 2) * mGridY + j - mOffsetYU]) / 2.0 / mStepX
                    ) * (*CurrentData)[mGridX + mOffsetXL - 1][j][0];
        }

        for (size_t i = mOffsetXL + 1; i < mGridX + mOffsetXL - 1; i++) {
            (*CurrentData)[i][mOffsetYU][4] = (
                    (*CurrentData)[i][mOffsetYU][4] / (*CurrentData)[i][mOffsetYU][0] -
                    (mDivergence[(i - mOffsetXL) * mGridY + 1] - mDivergence[(i - mOffsetXL) * mGridY]) / mStepX
                    ) * (*CurrentData)[i][mOffsetYU][0];
            (*CurrentData)[i][mGridY + mOffsetYU - 1][4] = (
                    (*CurrentData)[i][mGridY + mOffsetYU - 1][4] / (*CurrentData)[i][mGridY + mOffsetYU - 1][0] -
                    (mDivergence[(i - mOffsetXL) * mGridY + mGridY - 1] - mDivergence[(i - mOffsetXL) * mGridY + mGridY - 2]) / mStepX
                    ) * (*CurrentData)[i][mGridY + mOffsetYU - 1][0];
        }

//            std::cout << dhBx_dx + dhBy_dy + mVertField[27] << std::endl;
//        }

//        for (size_t i = 0; i < mGridX; i++) {
////            Elevation[i][0] = mDivergence[i * mGridY] / 2.0 / mStepX;
//
//            for (size_t j = 0; j < mGridY; j++) {
////                Elevation[i][j] = (mDivergence[i * mGridY + j + 1] - mDivergence[i * mGridY + j - 1]) / 2.0 / mStepX;
//                Elevation[i][j] = mDivergence[i * mGridY + j];
//            }
//
////            Elevation[i][mGridY - 1] = mDivergence[i * mGridY + mGridY - 1] / 2.0 / mStepX;
//        }
    }
    void save() {
        for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
            for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
                EOutput << (*CurrentData)[i][j][0] + Bottom[i][j] << " ";
            }

            EOutput << std::endl;
        }
    }
    const std::vector <std::vector <double>>& getElevation() const {
        return Elevation;
    }
private:
    size_t mGridX = 254;
    size_t mGridY = 50;

    size_t mOffsetXL = 2;
    size_t mOffsetXR = 2;
    size_t mOffsetYU = 2;
    size_t mOffsetYD = 2;

    dGrid GridCurrentData;
    dGrid GridTempData;
    dGrid GridTempData2;

    dGrid *CurrentData = &GridCurrentData;
    dGrid *TempData = &GridTempData;
    dGrid *TempData2 = &GridTempData2;

    double mStepX = 100.0e+03;
    double mStepY = 100.0e+03;

    double mStepTime = 60.0;

    double mTimeLimit = 10000;

    std::vector <std::vector <double>> Bottom;
    std::vector <double> mCorParam;
    std::vector <double> mHorizFieldY;
    std::vector <double> mVertField;
    
    double mMaxAlpha = 0.0;

    std::vector <double> AlphaX;
    std::vector <double> AlphaY;

    double TimePassed = 0.0;

    std::vector <std::vector <double>> Elevation;

    //---Divergence---//
    std::vector <double> mDivergence;
    std::vector <double> B;
    std::vector <double> r;
    std::vector <double> p;
    std::vector <double> Ap;
    std::vector <double> ResOpt;
    //---Divergence---//

    //----------//

    const double mGrav  = 9.81;
    const double mCorParam_0 = 1.0e-04;
    const double mBetaParam = 1.6e-11;
//    const double mBetaParam = 0.0;

    //----------//

    std::ofstream EOutput;

    //----------//

    void initCoriolis() {
        double MeanY = int((mGridY + mOffsetYU + mOffsetYD) / 2) * mStepY;

        mCorParam.resize(mGridY + mOffsetYU + mOffsetYD);

        for (size_t i = 0; i < mGridY + mOffsetYU + mOffsetYD; i++) {
            mCorParam[i] = mCorParam_0 + mBetaParam * (i * mStepY - MeanY);
        }
    }
    void initFields() {
        mHorizFieldY.resize(mGridY + mOffsetYU + mOffsetYD);
        mVertField.resize(mGridY + mOffsetYU + mOffsetYD);

        for (size_t i = 0; i < mGridY + mOffsetYU + mOffsetYD; i++) {
            mHorizFieldY[i] = 3.5e-05 - 4.87e-07 * i;
            mVertField[i] = 1.27e-05 + 1.46e-06 * i - 1.38e-08 * pow(i, 2.0);
//            mHorizFieldY[i] = 0.0;
//            mVertField[i] = 0.0;
        }
    }
    void bottomFunc() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution <> dis(0.0, 500.0);

        double StdX = 5.0 * mStepX;
        double StdY = 5.0 * mStepY;

        double MeanX = int((mGridX + mOffsetXL + mOffsetXR) / 2) * mStepX;
        double MeanY = int((mGridY + mOffsetYU + mOffsetYD) / 2) * mStepY;

        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 0; j < mGridY + mOffsetYU + mOffsetYD; j++) {
//                Bottom[i][j] = 0.0;
//                Bottom[i][j] = dis(gen);
                Bottom[i][j] = 4000 * exp(
                        -0.5 * pow((i * mStepX - MeanX) / StdX, 2.0)
                        -0.5 * pow((j * mStepY - MeanY) / StdY, 2.0));
            }
        }
    }
    void updateBoundaries() {
        //---Periodic East-West---//

        //---h---//
        for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
            (*TempData)[0][j][0] = (*TempData)[mGridX + mOffsetXL - 2][j][0];
            (*TempData)[1][j][0] = (*TempData)[mGridX + mOffsetXL - 1][j][0];

            (*TempData)[mGridX + mOffsetXL + mOffsetXR - 2][j][0] = (*TempData)[mOffsetXL][j][0];
            (*TempData)[mGridX + mOffsetXL + mOffsetXR - 1][j][0] = (*TempData)[mOffsetXL + 1][j][0];
        }
        //---h---//

        //---vx-vy---//
        for (int iComp = 1; iComp < 3; iComp++) {
            for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                (*TempData)[mGridX + mOffsetXL + mOffsetXR - 2][j][iComp] =
                        (*TempData)[mOffsetXL][j][iComp] /
                        (*TempData)[mOffsetXL][j][0] *
                        (*TempData)[mGridX + mOffsetXL + mOffsetXR - 2][j][0];
                (*TempData)[mGridX + mOffsetXL + mOffsetXR - 1][j][iComp] =
                        (*TempData)[mOffsetXL + 1][j][iComp] /
                        (*TempData)[mOffsetXL + 1][j][0] *
                        (*TempData)[mGridX + mOffsetXL + mOffsetXR - 1][j][0];
            }
        }
        //---vx-vy---//

        //---Bx-By-Psi---//
        for (int iComp = 3; iComp < 6; iComp++) {
            for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                (*TempData)[0][j][iComp] =
                        (*TempData)[mGridX + mOffsetXL - 2][j][iComp] /
                        (*TempData)[mGridX + mOffsetXL - 2][j][0] *
                        (*TempData)[0][j][0];
                (*TempData)[1][j][iComp] =
                        (*TempData)[mGridX + mOffsetXL - 1][j][iComp] /
                        (*TempData)[mGridX + mOffsetXL - 1][j][0] *
                        (*TempData)[1][j][0];

                (*TempData)[mGridX + mOffsetXL + mOffsetXR - 2][j][iComp] =
                        (*TempData)[mOffsetXL][j][iComp] /
                        (*TempData)[mOffsetXL][j][0] *
                        (*TempData)[mGridX + mOffsetXL + mOffsetXR - 2][j][0];
                (*TempData)[mGridX + mOffsetXL + mOffsetXR - 1][j][iComp] =
                        (*TempData)[mOffsetXL + 1][j][iComp] /
                        (*TempData)[mOffsetXL + 1][j][0] *
                        (*TempData)[mGridX + mOffsetXL + mOffsetXR - 1][j][0];
            }
        }
        //---Bx-By-Psi---//

        //---Periodic East-West---//
        
        //-----------------------------//

        //---Extrapolation East---//

        //---vx-vy---//
        for (int iComp = 1; iComp < 3; iComp++) {
            for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                (*TempData)[1][j][iComp] = extrapolate(
                        (*TempData)[mOffsetXL][j][iComp] / (*TempData)[mOffsetXL][j][0],
                        (*TempData)[mOffsetXL + 1][j][iComp] / (*TempData)[mOffsetXL + 1][j][0],
                        (*TempData)[mOffsetXL + 2][j][iComp] / (*TempData)[mOffsetXL + 2][j][0],
                        (*TempData)[mOffsetXL + 3][j][iComp] / (*TempData)[mOffsetXL + 3][j][0]
                        ) * (*TempData)[1][j][0];
            }
        }
        //---vx-vy---//

        //---Extrapolation East---//

        //-----------------------------//
        
        //---Fixed North-South---//

        for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
            //---vy---//
            (*TempData)[i][1][2] = 0.0;
            (*TempData)[i][0][2] = 0.0;

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][2] = 0.0;
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][2] = 0.0;
            //---vy---//

            //---Bx---//
            (*TempData)[i][1][3] = extrapolate(
                    (*TempData)[i][mOffsetYU][3] / (*TempData)[i][mOffsetYU][0],
                    (*TempData)[i][mOffsetYU + 1][3] / (*TempData)[i][mOffsetYU + 1][0],
                    (*TempData)[i][mOffsetYU + 2][3] / (*TempData)[i][mOffsetYU + 2][0],
                    (*TempData)[i][mOffsetYU + 3][3] / (*TempData)[i][mOffsetYU + 3][0]
                    ) * (*TempData)[i][1][0];

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][3] = extrapolate(
                    (*TempData)[i][mGridY + mOffsetYU - 1][3] / (*TempData)[i][mGridY + mOffsetYU - 1][0],
                    (*TempData)[i][mGridY + mOffsetYU - 2][3] / (*TempData)[i][mGridY + mOffsetYU - 2][0],
                    (*TempData)[i][mGridY + mOffsetYU - 3][3] / (*TempData)[i][mGridY + mOffsetYU - 3][0],
                    (*TempData)[i][mGridY + mOffsetYU - 4][3] / (*TempData)[i][mGridY + mOffsetYU - 4][0]
                    ) * (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][0];
            //---Bx---//
            
            //---By---//
            (*TempData)[i][0][4] = mHorizFieldY[0] * (*TempData)[i][0][0];
            (*TempData)[i][1][4] = mHorizFieldY[1] * (*TempData)[i][1][0];

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][4] =
                    mHorizFieldY[mGridY + mOffsetYU + mOffsetYD - 1] *
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][0];
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][4] =
                    mHorizFieldY[mGridY + mOffsetYU + mOffsetYD - 2] *
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][0];
            //---By---//
            
            //---Psi---//
            (*TempData)[i][1][5] = extrapolate(
                    (*TempData)[i][mOffsetYU][5] / (*TempData)[i][mOffsetYU][0],
                    (*TempData)[i][mOffsetYU + 1][5] / (*TempData)[i][mOffsetYU + 1][0],
                    (*TempData)[i][mOffsetYU + 2][5] / (*TempData)[i][mOffsetYU + 2][0],
                    (*TempData)[i][mOffsetYU + 3][5] / (*TempData)[i][mOffsetYU + 3][0]
                    ) * (*TempData)[i][1][0];

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][5] = extrapolate(
                    (*TempData)[i][mGridY + mOffsetYU - 1][5] / (*TempData)[i][mGridY + mOffsetYU - 1][0],
                    (*TempData)[i][mGridY + mOffsetYU - 2][5] / (*TempData)[i][mGridY + mOffsetYU - 2][0],
                    (*TempData)[i][mGridY + mOffsetYU - 3][5] / (*TempData)[i][mGridY + mOffsetYU - 3][0],
                    (*TempData)[i][mGridY + mOffsetYU - 4][5] / (*TempData)[i][mGridY + mOffsetYU - 4][0]
                    ) * (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][0];
            //---Psi---//
        }
        
        //---Fixed North-South---//
        
        //-----------------------------//

        //---Extrapolation North-South---//

        //---vx---//
        for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
            (*TempData)[i][1][1] = extrapolate(
                    (*TempData)[i][mOffsetYU][1] / (*TempData)[i][mOffsetYU][0],
                    (*TempData)[i][mOffsetYU + 1][1] / (*TempData)[i][mOffsetYU + 1][0],
                    (*TempData)[i][mOffsetYU + 2][1] / (*TempData)[i][mOffsetYU + 2][0],
                    (*TempData)[i][mOffsetYU + 3][1] / (*TempData)[i][mOffsetYU + 3][0]
                    ) * (*TempData)[i][1][0];
            
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][1] = extrapolate(
                    (*TempData)[i][mGridY + mOffsetYU - 1][1] / (*TempData)[i][mGridY + mOffsetYU - 1][0],
                    (*TempData)[i][mGridY + mOffsetYU - 2][1] / (*TempData)[i][mGridY + mOffsetYU - 2][0],
                    (*TempData)[i][mGridY + mOffsetYU - 3][1] / (*TempData)[i][mGridY + mOffsetYU - 3][0],
                    (*TempData)[i][mGridY + mOffsetYU - 4][1] / (*TempData)[i][mGridY + mOffsetYU - 4][0]
                    ) * (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][0];
        }
        //---vx---//

        //---Extrapolation North-South---//
    }
    void initConditions() {
        double MeanWind = 20.0;
        double MeanY = int((mGridY + mOffsetYU + mOffsetYD) / 2) * mStepY;

        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 0; j < mGridY + mOffsetYU + mOffsetYD; j++) {
                double TempHeight = 10000.0 - (MeanWind * mCorParam_0 / mGrav) * (j * mStepY - MeanY);

                GridCurrentData[i][j] = dVec(
                        TempHeight,
                        0.0,
                        0.0,
                        0.0,
                        TempHeight * mHorizFieldY[j],
                        0.0
                        );
            }
        }

        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 1; j < mGridY + mOffsetYU + mOffsetYD - 1; j++) {
                GridCurrentData[i][j][1] =
                        GridCurrentData[i][j][0] *
                        (-0.5 * mGrav / (mCorParam[j] * mStepX) *
                        (GridCurrentData[i][j + 1][0] - GridCurrentData[i][j - 1][0]));
            }
        }

        for (size_t i = 1; i < mGridX + mOffsetXL + mOffsetXR - 1; i++) {
            for (size_t j = 0; j < mGridY + mOffsetYU + mOffsetYD; j++) {
                GridCurrentData[i][j][2] =
                        GridCurrentData[i][j][0] *
                        (0.5 * mGrav / (mCorParam[j] * mStepX) *
                        (GridCurrentData[i + 1][j][0] - GridCurrentData[i - 1][j][0]));
            }
        }

        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            GridCurrentData[i][0][2] = 0.0;
            GridCurrentData[i][mGridY + mOffsetYU + mOffsetYD - 1][2] = 0.0;
        }

        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 0; j < mGridY + mOffsetYU + mOffsetYD; j++) {
                GridCurrentData[i][j][0] -= Bottom[i][j];
            }
        }
    }

    dVec funcX(const dVec& tVal) {
        return dVec(
                tVal[1],
                (tVal[1] * tVal[1] - tVal[3] * tVal[3]) / tVal[0] + 0.5 * mGrav * tVal[0] * tVal[0],
                (tVal[1] * tVal[2] - tVal[3] * tVal[4]) / tVal[0],
                tVal[5],
                -(tVal[1] * tVal[4] - tVal[2] * tVal[3]) / tVal[0],
                mMaxAlpha * mMaxAlpha * tVal[3]
        );
    }
    dVec funcY(const dVec& tVal) {
        return dVec(
                tVal[2],
                (tVal[1] * tVal[2] - tVal[3] * tVal[4]) / tVal[0],
                (tVal[2] * tVal[2] - tVal[4] * tVal[4]) / tVal[0] + 0.5 * mGrav * tVal[0] * tVal[0],
                -(tVal[2] * tVal[3] - tVal[1] * tVal[4]) / tVal[0],
                tVal[5],
                mMaxAlpha * mMaxAlpha * tVal[4]
        );
    }
    dVec source(int tPosX, int tPosY) {
        double Bz = -mVertField[tPosY];
//        double Bz = -(((*CurrentData)[tPosX + 1][tPosY][3] - (*CurrentData)[tPosX - 1][tPosY][3]) / mStepX / 2.0 +
//                 ((*CurrentData)[tPosX][tPosY + 1][4] - (*CurrentData)[tPosX][tPosY - 1][4]) / mStepY / 2.0);

//        double dhBx_dx = ((*CurrentData)[tPosX + 1][tPosY][3] - (*CurrentData)[tPosX - 1][tPosY][3]) / mStepX / 2.0;
//        double dhBy_dy = ((*CurrentData)[tPosX][tPosY + 1][4] - (*CurrentData)[tPosX][tPosY - 1][4]) / mStepY / 2.0;

//        double dvx_dx = (
//                (*CurrentData)[tPosX + 1][tPosY][1] / (*CurrentData)[tPosX + 1][tPosY][0] -
//                (*CurrentData)[tPosX - 1][tPosY][1] / (*CurrentData)[tPosX - 1][tPosY][0]
//                ) / mStepX / 2.0;
//        double dvy_dy = (
//                (*CurrentData)[tPosX][tPosY + 1][2] / (*CurrentData)[tPosX][tPosY + 1][0] -
//                (*CurrentData)[tPosX][tPosY - 1][2] / (*CurrentData)[tPosX][tPosY - 1][0]
//                ) / mStepY / 2.0;

        double vx = (*CurrentData)[tPosX][tPosY][1] / (*CurrentData)[tPosX][tPosY][0];
        double vy = (*CurrentData)[tPosX][tPosY][2] / (*CurrentData)[tPosX][tPosY][0];
        double Bx = (*CurrentData)[tPosX][tPosY][3] / (*CurrentData)[tPosX][tPosY][0];
        double By = (*CurrentData)[tPosX][tPosY][4] / (*CurrentData)[tPosX][tPosY][0];

        double dh_dx = (Bottom[tPosX + 1][tPosY] - Bottom[tPosX - 1][tPosY]) / (2.0 * mStepX);
        double dh_dy = (Bottom[tPosX][tPosY + 1] - Bottom[tPosX][tPosY - 1]) / (2.0 * mStepY);

        return dVec(
                0.0,
                Bz * Bx +
                mCorParam[tPosY] * (*CurrentData)[tPosX][tPosY][2] -
                mGrav * (*CurrentData)[tPosX][tPosY][0] * dh_dx,
                Bz * By +
                -mCorParam[tPosY] * (*CurrentData)[tPosX][tPosY][1] -
                mGrav * (*CurrentData)[tPosX][tPosY][0] * dh_dy,
                Bz * vx,
                Bz * vy,
                -mMaxAlpha * mMaxAlpha * Bz
                );
    }
    dVec viscosity(int tPosX, int tPosY) {
        double v_x_xx = (
                (*CurrentData)[tPosX - 1][tPosY][1] / (*CurrentData)[tPosX - 1][tPosY][0] +
                (*CurrentData)[tPosX][tPosY][1] / (*CurrentData)[tPosX][tPosY][0] * 2.0 +
                (*CurrentData)[tPosX + 1][tPosY][1] / (*CurrentData)[tPosX + 1][tPosY][0]) /
                pow(mStepX, 2.0);
        double v_x_yy = (
                (*CurrentData)[tPosX][tPosY - 1][1] / (*CurrentData)[tPosX][tPosY - 1][0] +
                (*CurrentData)[tPosX][tPosY][1] / (*CurrentData)[tPosX][tPosY][0] * 2.0 +
                (*CurrentData)[tPosX][tPosY + 1][1] / (*CurrentData)[tPosX][tPosY + 1][0]) /
                pow(mStepY, 2.0);
        double v_y_xx = (
                (*CurrentData)[tPosX - 1][tPosY][2] / (*CurrentData)[tPosX - 1][tPosY][0] +
                (*CurrentData)[tPosX][tPosY][2] / (*CurrentData)[tPosX][tPosY][0] * 2.0 +
                (*CurrentData)[tPosX + 1][tPosY][2] / (*CurrentData)[tPosX + 1][tPosY][0]) /
                pow(mStepX, 2.0);
        double v_y_yy = (
                (*CurrentData)[tPosX][tPosY - 1][2] / (*CurrentData)[tPosX][tPosY - 1][0] +
                (*CurrentData)[tPosX][tPosY][2] / (*CurrentData)[tPosX][tPosY][0] * 2.0 +
                (*CurrentData)[tPosX][tPosY + 1][2] / (*CurrentData)[tPosX][tPosY + 1][0]) /
                pow(mStepY, 2.0);

        return dVec (
                0.0,
                (*CurrentData)[tPosX][tPosY][0] * (v_x_xx + v_x_yy),
                (*CurrentData)[tPosX][tPosY][0] * (v_y_xx + v_y_yy),
                0.0,
                0.0,
                0.0);
    }

    //----------//

    dVec WENO(
            const dVec& tPosVal_minus_1,
            const dVec& tPosVal,
            const dVec& tPosVal_plus_1,
            const dVec& tNegVal,
            const dVec& tNegVal_plus_1,
            const dVec& tNegVal_plus_2) {
        dVec fPlus(0.0, 0.0, 0.0, 0.0, 0.0);
        dVec fMinus(0.0, 0.0, 0.0, 0.0, 0.0);

        for (int i = 0; i < 6; i++) {
            double Beta0 = pow(tPosVal_minus_1[i] - tPosVal[i], 2.0);
            double Beta1 = pow(tPosVal[i] - tPosVal_plus_1[i], 2.0);

            double d0 = 1.0 / 3.0;
            double d1 = 2.0 / 3.0;

            double Alpha0 = d0 / pow(1.0e-06 + Beta0, 2.0);
            double Alpha1 = d1 / pow(1.0e-06 + Beta1, 2.0);

            double Omega0 = Alpha0 / (Alpha0 + Alpha1);
            double Omega1 = Alpha1 / (Alpha0 + Alpha1);

            fPlus[i] =
                    Omega0 * (-0.5 * tPosVal_minus_1[i] + 1.5 * tPosVal[i]) +
                    Omega1 * (0.5 * tPosVal[i] + 0.5 * tPosVal_plus_1[i]);
        }

        for (int i = 0; i < 6; i++) {
            double Beta0 = pow(tNegVal[i] - tNegVal_plus_1[i], 2.0);
            double Beta1 = pow(tNegVal_plus_1[i] - tNegVal_plus_2[i], 2.0);

            double d0 = 2.0 / 3.0;
            double d1 = 1.0 / 3.0;

            double Alpha0 = d0 / pow(1.0e-06 + Beta0, 2.0);
            double Alpha1 = d1 / pow(1.0e-06 + Beta1, 2.0);

            double Omega0 = Alpha0 / (Alpha0 + Alpha1);
            double Omega1 = Alpha1 / (Alpha0 + Alpha1);

            fMinus[i] =
                    Omega0 * (0.5 * tNegVal[i] + 0.5 * tNegVal_plus_1[i]) +
                    Omega1 * (1.5 * tNegVal_plus_1[i] - 0.5 * tNegVal_plus_2[i]);
        }

        return fPlus + fMinus;
    }

    double extrapolate(double tOffset_1, double tOffset_2, double tOffset_3, double tOffset_4) {
        return 4.0 * tOffset_1 - 6.0 * tOffset_2 + 4.0 * tOffset_3 - tOffset_4;
    }
    void AxOpt(const std::vector <double>& tMult, std::vector <double>& tRes, size_t tRows, size_t tColumns) {
        for (size_t i = 0; i < tRows * tColumns; i++) {
            tRes[i] = 0.0;
        }

        for (size_t i = 0; i < tRows * tColumns; i++) {
            tRes[i] += 4 * tMult[i];
        }

//        for (size_t i = 0; i < tRows * tColumns; i++) {
//            if ((i + 1) % tRows == 0) {
//                tRes[i - tRows + 1] -= tMult[i];
//                tRes[i] -= tMult[i - tRows + 1];
//            }
//        }

        for (size_t i = 1; i < tRows * tColumns; i++) {
            if (i % tRows == 0) {
                continue;
            }

            tRes[i - 1] -= tMult[i];
            tRes[i] -= tMult[i - 1];
        }

        for (size_t i = tRows; i < tRows * tColumns; i++) {
            tRes[i - tRows] -= tMult[i];
            tRes[i] -= tMult[i - tRows];
        }
    }
};

int main(int argc, char** argv) {
    Solver Test;

    SDL_Init(SDL_INIT_VIDEO);

    auto Window = SDL_CreateWindow(
            "My App",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            1500, 300,
            SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    if (!Window) {
        std::cout << "Couldn't create the window: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    bool Run = true;
    SDL_Event Event;

    //----------//

    try {
        Renderer Rend(Window, true);

        while (Run) {
            while (SDL_PollEvent(&Event)) {
                if (Event.type == SDL_QUIT) {
                    Run = false;
                } else if (Event.type == SDL_WINDOWEVENT) {
                    if (Event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        Rend.setResized();
                    }
                }
            }

            Test.solve();

            Rend.drawFrame(Test.getElevation());
//            break;
        }

        //----------//

        vkDeviceWaitIdle(Rend.getDevice());

        Test.save();

        SDL_DestroyWindow(Window);
        SDL_Quit();
    } catch (const std::exception& tExcept) {
        std::cerr << tExcept.what() << std::endl;
        return -1;
    }

    return 0;
}

