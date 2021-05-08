#include <iostream>
#include <cmath>
#include <utility>
#include <ctime>
#include <chrono>
#include "Core/dVector.h"
//-----------------------------//
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
//-----------------------------//
#include "Renderer.h"
#include "NumerCalc/dWENO2D.h"
//-----------------------------//
using dGrid = std::vector <std::vector <dVector <double, 5>>>;
using dVec = dVector <double, 5>;

class SolverWENO : public dWENO2D <dVec> {
public:
    SolverWENO() {
        setTimeStep(60.0);

        setXStep(100.0e+03);
        setYStep(100.0e+03);
    }
public:
    dVec funcX(const dVec& tVal) override {
        return dVec(
                tVal[1],
                (tVal[1] * tVal[1] - tVal[3] * tVal[3]) / tVal[0] + 0.5 * mGrav * tVal[0] * tVal[0],
                (tVal[1] * tVal[2] - tVal[3] * tVal[4]) / tVal[0],
                0.0,
                -(tVal[1] * tVal[4] - tVal[2] * tVal[3]) / tVal[0]
        );
    }
    dVec funcY(const dVec& tVal) override {
        return dVec(
                tVal[2],
                (tVal[1] * tVal[2] - tVal[3] * tVal[4]) / tVal[0],
                (tVal[2] * tVal[2] - tVal[4] * tVal[4]) / tVal[0] + 0.5 * mGrav * tVal[0] * tVal[0],
                -(tVal[2] * tVal[3] - tVal[1] * tVal[4]) / tVal[0],
                0.0
        );
    }
private:
    static constexpr double mGrav  = 9.81;
};

class Solver {
    enum DisplayOutput {
        DISPLAY_ELEVATION,
        DISPLAY_FIELD,
        DISPLAY_FIELD_DIVERGENCE
    };
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

        mOutput.resize(mGridX);

        //---Divergence---//
        mDivergence.resize(mGridX * mGridY);
        B.resize(mGridX * mGridY);
        r.resize(mGridX * mGridY);
        p.resize(mGridX * mGridY);
        Ap.resize(mGridX * mGridY);
        ResOpt.resize(mGridX * mGridY);

        ResGrid.resize(mGridX + mOffsetXL + mOffsetXR);

        for (auto& iRow : ResGrid) {
            iRow.resize(mGridY + mOffsetYU + mOffsetYD);

            for (auto& iVal : iRow) {
                iVal = 0.0;
            }
        }

        for (size_t i = 0; i < mGridX * mGridY; i++) {
            mDivergence[i] = 0.0;
            B[i] = 0.0;
            r[i] = 0.0;
            p[i] = 0.0;
            Ap[i] = 0.0;
            ResOpt[i] = 0.0;
        }
        //---Divergence---//

        for (auto& iRow : mOutput) {
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
        findAlpha();

        TimePassed += mStepTime;

        if (int(TimePassed) % (60 * 15) == 0) {
            std::cout << "Time: " << TimePassed / 3600 / 24 << " Step - " << mStepTime << " Alpha - " << mMaxAlpha
                      << std::endl;
        }

#pragma omp parallel for default(none) firstprivate(mMaxAlpha) collapse(2) num_threads(4)
        for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
            for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                dVec Val = (*CurrentData)[i][j];

                dVec Val_xmin_1 = (*CurrentData)[i - 1][j];
                dVec Val_xmin_2 = (*CurrentData)[i - 2][j];
                dVec Val_xpl_1 = (*CurrentData)[i + 1][j];
                dVec Val_xpl_2 = (*CurrentData)[i + 2][j];

                dVec Val_ymin_1 = (*CurrentData)[i][j - 1];
                dVec Val_ymin_2 = (*CurrentData)[i][j - 2];
                dVec Val_ypl_1 = (*CurrentData)[i][j + 1];
                dVec Val_ypl_2 = (*CurrentData)[i][j + 2];

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
                }

                (*TempData)[i][j] =
                        (*CurrentData)[i][j] -
                        (mSpartialSolver.solve(Val, Val_xmin_2, Val_xmin_1, Val_xpl_1, Val_xpl_2, Val_ymin_2, Val_ymin_1, Val_ypl_1, Val_ypl_2, mMaxAlpha) -
                                mStepTime * source(i, j));
            }
        }

        updateBoundaries();
        std::swap(TempData2, TempData);

#pragma omp parallel for default(none) firstprivate(mMaxAlpha) collapse(2) num_threads(4)
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
                }

                (*TempData)[i][j] = 0.5 * (
                        (*CurrentData)[i][j] +
                        (*TempData2)[i][j] -
                        (mSpartialSolver.solve(Val, Val_xmin_2, Val_xmin_1, Val_xpl_1, Val_xpl_2, Val_ymin_2, Val_ymin_1, Val_ypl_1, Val_ypl_2, mMaxAlpha) -
                                mStepTime * source(i, j)));
            }
        }

        updateBoundaries();

        std::swap(CurrentData, TempData);

//        fixFieldDivergence();
        fillOutput(DISPLAY_ELEVATION);
    }
    void save() {
        for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
            for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
                EOutput << (*CurrentData)[i][j][0] + Bottom[i][j] << " ";
            }

            EOutput << std::endl;
        }
    }
    void fillOutput(DisplayOutput tType) {
        switch (tType) {
            case DISPLAY_ELEVATION:
                for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                    for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
                        mOutput[i - mOffsetXL][j - mOffsetYU] = (*CurrentData)[i][j][0] + Bottom[i][j];
                    }
                }

                break;
            case DISPLAY_FIELD:
                for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                    for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
                        double Bx2 = pow((*CurrentData)[i][j][3] / (*CurrentData)[i][j][0], 2.0);
                        double By2 = pow((*CurrentData)[i][j][4] / (*CurrentData)[i][j][0], 2.0);

                        mOutput[i - mOffsetXL][j - mOffsetYU] = sqrt(By2);
                    }
                }

                break;
            case DISPLAY_FIELD_DIVERGENCE:
                for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                    for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
                        double dBx_dx = (
                                                (*CurrentData)[i + 1][j][3] / (*CurrentData)[i + 1][j][0] -
                                                (*CurrentData)[i - 1][j][3] / (*CurrentData)[i - 1][j][0]
                                        ) / (mStepX * 2.0);
                        double dBy_dy = (
                                                (*CurrentData)[i][j + 1][4] / (*CurrentData)[i][j + 1][0] -
                                                (*CurrentData)[i][j - 1][4] / (*CurrentData)[i][j - 1][0]
                                        ) / (mStepY * 2.0);

                        mOutput[i - mOffsetXL][j - mOffsetYU] = dBx_dx + dBy_dy;
                    }
                }

                break;
            default:
                break;
        }
    }
    std::vector <std::vector <double>> getElevation() const {
        return mOutput;
    }
private:
    size_t mGridX = 360;
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

    std::vector <std::vector <double>> mOutput;

    //---Divergence---//
    std::vector <double> mDivergence;
    std::vector <double> B;
    std::vector <double> r;
    std::vector <double> p;
    std::vector <double> Ap;
    std::vector <double> ResOpt;

    std::vector <std::vector <double>> ResGrid;
    //---Divergence---//

    //----------//

    const double mGrav  = 9.81;
    const double mCorParam_0 = 1.0e-04;
    const double mBetaParam = 1.6e-11;
//    const double mBetaParam = 0.0;



    SolverWENO mSpartialSolver;

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
//            mVertField[i] = 21.0;
        }
    }
    void bottomFunc() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution <> dis(0.0, 250.0);

        double StdX = 5 * mStepX;
        double StdY = 5 * mStepY;

        double MeanX = int((mGridX + mOffsetXL + mOffsetXR) / 2) * mStepX;
        double MeanY = int((mGridY + mOffsetYU + mOffsetYD) / 2) * mStepY;

        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 0; j < mGridY + mOffsetYU + mOffsetYD; j++) {
                Bottom[i][j] = 0.0;
//                Bottom[i][j] = dis(gen);
                Bottom[i][j] = 4000 * exp(
                        -0.5 * pow((i * mStepX - MeanX) / StdX, 2.0)
                        -0.5 * pow((j * mStepY - MeanY) / StdY, 2.0));
//                Bottom[i][j] = 4000 * pow(sin(i / 10.0), 2.0) + pow(sin(j / 10.0), 2.0);
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
        for (int iComp = 3; iComp < 5; iComp++) {
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

            (*TempData)[i][1][1] = 0.0;
            (*TempData)[i][0][1] = 0.0;

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][1] = 0.0;
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][1] = 0.0;
            //---vy---//

            //---Bx---//
            (*TempData)[i][0][3] = 0.0;
            (*TempData)[i][1][3] = 0.0;

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][3] = 0.0;
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][3] = 0.0;
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
//            (*TempData)[i][1][5] = extrapolate(
//                    (*TempData)[i][mOffsetYU][5] / (*TempData)[i][mOffsetYU][0],
//                    (*TempData)[i][mOffsetYU + 1][5] / (*TempData)[i][mOffsetYU + 1][0],
//                    (*TempData)[i][mOffsetYU + 2][5] / (*TempData)[i][mOffsetYU + 2][0],
//                    (*TempData)[i][mOffsetYU + 3][5] / (*TempData)[i][mOffsetYU + 3][0]
//                    ) * (*TempData)[i][1][0];
//
//            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][5] = extrapolate(
//                    (*TempData)[i][mGridY + mOffsetYU - 1][5] / (*TempData)[i][mGridY + mOffsetYU - 1][0],
//                    (*TempData)[i][mGridY + mOffsetYU - 2][5] / (*TempData)[i][mGridY + mOffsetYU - 2][0],
//                    (*TempData)[i][mGridY + mOffsetYU - 3][5] / (*TempData)[i][mGridY + mOffsetYU - 3][0],
//                    (*TempData)[i][mGridY + mOffsetYU - 4][5] / (*TempData)[i][mGridY + mOffsetYU - 4][0]
//                    ) * (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][0];
            //---Psi---//
        }

        //---Fixed North-South---//

        //-----------------------------//

        //---Extrapolation North-South---//

        //---vx---//
//        for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
//            (*TempData)[i][1][1] = extrapolate(
//                    (*TempData)[i][mOffsetYU][1] / (*TempData)[i][mOffsetYU][0],
//                    (*TempData)[i][mOffsetYU + 1][1] / (*TempData)[i][mOffsetYU + 1][0],
//                    (*TempData)[i][mOffsetYU + 2][1] / (*TempData)[i][mOffsetYU + 2][0],
//                    (*TempData)[i][mOffsetYU + 3][1] / (*TempData)[i][mOffsetYU + 3][0]
//                    ) * (*TempData)[i][1][0];
//
//            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][1] = extrapolate(
//                    (*TempData)[i][mGridY + mOffsetYU - 1][1] / (*TempData)[i][mGridY + mOffsetYU - 1][0],
//                    (*TempData)[i][mGridY + mOffsetYU - 2][1] / (*TempData)[i][mGridY + mOffsetYU - 2][0],
//                    (*TempData)[i][mGridY + mOffsetYU - 3][1] / (*TempData)[i][mGridY + mOffsetYU - 3][0],
//                    (*TempData)[i][mGridY + mOffsetYU - 4][1] / (*TempData)[i][mGridY + mOffsetYU - 4][0]
//                    ) * (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][0];
//        }
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
//                        TempHeight * mHorizFieldY[j]
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
                0.0,
//                tVal[5],
                -(tVal[1] * tVal[4] - tVal[2] * tVal[3]) / tVal[0]
//                mMaxAlpha * mMaxAlpha * tVal[3]
        );
    }
    dVec funcY(const dVec& tVal) {
        return dVec(
                tVal[2],
                (tVal[1] * tVal[2] - tVal[3] * tVal[4]) / tVal[0],
                (tVal[2] * tVal[2] - tVal[4] * tVal[4]) / tVal[0] + 0.5 * mGrav * tVal[0] * tVal[0],
                -(tVal[2] * tVal[3] - tVal[1] * tVal[4]) / tVal[0],
                0.0
//                tVal[5],
//                mMaxAlpha * mMaxAlpha * tVal[4]
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
                Bz * vy
//                Bz * vy,
//                -mMaxAlpha * mMaxAlpha * Bz
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
//                0.0,
                0.0);
    }

    //----------//

    double extrapolate(double tOffset_1, double tOffset_2, double tOffset_3, double tOffset_4) {
        return 4.0 * tOffset_1 - 6.0 * tOffset_2 + 4.0 * tOffset_3 - tOffset_4;
    }
    void AxOpt(const std::vector <double>& tMult, std::vector <double>& tRes, size_t tRows, size_t tColumns) {
        for (size_t i = 0; i < tRows * tColumns; i++) {
            tRes[i] = 4 * tMult[i];

            if (i >= 1 && i % tRows != 0) {
                tRes[i - 1] -= tMult[i];
                tRes[i] -= tMult[i - 1];
            }

            if (i >= tRows) {
                tRes[i - tRows] -= tMult[i];
                tRes[i] -= tMult[i - tRows];
            }
        }
    }
    void conjugateGradient() {
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
        }
    }
    void fixFieldDivergence() {
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

        conjugateGradient();

        for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
            for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                ResGrid[i][j] = mDivergence[(i - mOffsetXL) * mGridY + j - mOffsetYU];
            }
        }

        for (size_t i = mOffsetXL + 1; i < mGridX + mOffsetXL - 1; i++) {
            for (size_t j = mOffsetYU + 1; j < mGridY + mOffsetYU - 1; j++) {
                (*CurrentData)[i][j][3] =
                        (*CurrentData)[i][j][3] / (*CurrentData)[i][j][0] -
                        (ResGrid[i + 1][j] - ResGrid[i - 1][j]) / (2.0 * mStepX) *
                        (*CurrentData)[i][j][0];
                (*CurrentData)[i][j][4] =
                        (*CurrentData)[i][j][4] / (*CurrentData)[i][j][0] -
                        (ResGrid[i][j + 1] - ResGrid[i][j - 1]) / (2.0 * mStepX) *
                        (*CurrentData)[i][j][0];
            }
        }

        for (size_t j = mOffsetYU + 1; j < mGridY + mOffsetYU - 1; j++) {
            (*CurrentData)[mOffsetXL][j][3] =
                    (*CurrentData)[mOffsetXL][j][3] / (*CurrentData)[mOffsetXL][j][0] -
                    ResGrid[mOffsetXL + 1][j] / (2.0 * mStepX) *
                    (*CurrentData)[mOffsetXL][j][0];
            (*CurrentData)[mGridX + mOffsetXL - 1][j][3] =
                    (*CurrentData)[mGridX + mOffsetXL - 1][j][3] / (*CurrentData)[mGridX + mOffsetXL - 1][j][0] +
                    ResGrid[mGridX + mOffsetXL - 2][j] / (2.0 * mStepX) *
                    (*CurrentData)[mGridX + mOffsetXL - 1][j][0];

            (*CurrentData)[mOffsetXL][j][4] =
                    (*CurrentData)[mOffsetXL][j][4] / (*CurrentData)[mOffsetXL][j][0] -
                    (ResGrid[mOffsetXL][j + 1] - ResGrid[mOffsetXL][j - 1]) / (2.0 * mStepY) *
                    (*CurrentData)[mOffsetXL][j][0];
            (*CurrentData)[mGridX + mOffsetXL - 1][j][4] =
                    (*CurrentData)[mGridX + mOffsetXL - 1][j][4] / (*CurrentData)[mGridX + mOffsetXL - 1][j][0] -
                    (ResGrid[mGridX + mOffsetXL - 1][j + 1] - ResGrid[mGridX + mOffsetXL - 1][j - 1]) / (2.0 * mStepY) *
                    (*CurrentData)[mGridX + mOffsetXL - 1][j][0];
        }

        for (size_t i = mOffsetXL + 1; i < mGridX + mOffsetXL - 1; i++) {
            (*CurrentData)[i][mOffsetYU][3] =
                    (*CurrentData)[i][mOffsetYU][3] / (*CurrentData)[i][mOffsetYU][0] -
                    (ResGrid[i + 1][mOffsetYU] - ResGrid[i - 1][mOffsetYU]) / (2.0 * mStepX) *
                    (*CurrentData)[i][mOffsetYU][0];
            (*CurrentData)[i][mGridY + mOffsetYU - 1][3] =
                    (*CurrentData)[i][mGridY + mOffsetYU - 1][3] / (*CurrentData)[i][mGridY + mOffsetYU - 1][0] -
                    (ResGrid[i + 1][mGridY + mOffsetYU - 1] - ResGrid[i - 1][mGridY + mOffsetYU - 1]) / (2.0 * mStepX) *
                    (*CurrentData)[i][mGridY + mOffsetYU - 1][0];

            (*CurrentData)[i][mOffsetYU][4] =
                    (*CurrentData)[i][mOffsetYU][4] / (*CurrentData)[i][mOffsetYU][0] -
                    ResGrid[i][mOffsetYU + 1] / (2.0 * mStepY) *
                    (*CurrentData)[i][mOffsetYU][0];
            (*CurrentData)[i][mGridY + mOffsetYU - 1][4] =
                    (*CurrentData)[i][mGridY + mOffsetYU - 1][4] / (*CurrentData)[i][mGridY + mOffsetYU - 1][0] +
                    ResGrid[i][mGridY + mOffsetYU - 2] / (2.0 * mStepY) *
                    (*CurrentData)[i][mGridY + mOffsetYU - 1][0];
        }

        (*CurrentData)[mOffsetXL][mOffsetYU][3] =
                (*CurrentData)[mOffsetXL][mOffsetYU][3] / (*CurrentData)[mOffsetXL][mOffsetYU][0] -
                ResGrid[mOffsetXL + 1][mOffsetYU] / (2.0 * mStepX) *
                (*CurrentData)[mOffsetXL][mOffsetYU][0];
        (*CurrentData)[mOffsetXL][mOffsetYU][4] =
                (*CurrentData)[mOffsetXL][mOffsetYU][4] / (*CurrentData)[mOffsetXL][mOffsetYU][0] -
                ResGrid[mOffsetXL][mOffsetYU + 1] / (2.0 * mStepY) *
                (*CurrentData)[mOffsetXL][mOffsetYU][0];

        (*CurrentData)[mGridX + mOffsetXL - 1][mOffsetYU][3] =
                (*CurrentData)[mGridX + mOffsetXL - 1][mOffsetYU][3] / (*CurrentData)[mGridX + mOffsetXL - 1][mOffsetYU][0] +
                ResGrid[mGridX + mOffsetXL - 2][mOffsetYU] / (2.0 * mStepX) *
                (*CurrentData)[mGridX + mOffsetXL - 1][mOffsetYU][0];
        (*CurrentData)[mGridX + mOffsetXL - 1][mOffsetYU][4] =
                (*CurrentData)[mGridX + mOffsetXL - 1][mOffsetYU][4] / (*CurrentData)[mGridX + mOffsetXL - 1][mOffsetYU][0] -
                ResGrid[mGridX + mOffsetXL - 1][mOffsetYU + 1] / (2.0 * mStepY) *
                (*CurrentData)[mGridX + mOffsetXL - 1][mOffsetYU][0];

        (*CurrentData)[mOffsetXL][mGridY + mOffsetYU - 1][3] =
                (*CurrentData)[mOffsetXL][mGridY + mOffsetYU - 1][3] / (*CurrentData)[mOffsetXL][mGridY + mOffsetYU - 1][0] -
                ResGrid[mOffsetXL + 1][mGridY + mOffsetYU - 1] / (2.0 * mStepX) *
                (*CurrentData)[mOffsetXL][mGridY + mOffsetYU - 1][0];
        (*CurrentData)[mOffsetXL][mGridY + mOffsetYU - 1][4] =
                (*CurrentData)[mOffsetXL][mGridY + mOffsetYU - 1][4] / (*CurrentData)[mOffsetXL][mGridY + mOffsetYU - 1][0] +
                ResGrid[mOffsetXL][mGridY + mOffsetYU - 2] / (2.0 * mStepY) *
                (*CurrentData)[mOffsetXL][mGridY + mOffsetYU - 1][0];

        (*CurrentData)[mGridX + mOffsetXL - 1][mGridY + mOffsetYU - 1][3] =
                (*CurrentData)[mGridX + mOffsetXL - 1][mGridY + mOffsetYU - 1][3] / (*CurrentData)[mGridX + mOffsetXL - 1][mGridY + mOffsetYU - 1][0] +
                ResGrid[mGridX + mOffsetXL - 2][mGridY + mOffsetYU - 1] / (2.0 * mStepX) *
                (*CurrentData)[mGridX + mOffsetXL - 1][mGridY + mOffsetYU - 1][0];
        (*CurrentData)[mGridX + mOffsetXL - 1][mGridY + mOffsetYU - 1][4] =
                (*CurrentData)[mGridX + mOffsetXL - 1][mGridY + mOffsetYU - 1][4] / (*CurrentData)[mGridX + mOffsetXL - 1][mGridY + mOffsetYU - 1][0] +
                ResGrid[mGridX + mOffsetXL - 1][mGridY + mOffsetYU - 2] / (2.0 * mStepY) *
                (*CurrentData)[mGridX + mOffsetXL - 1][mGridY + mOffsetYU - 1][0];
    }
    void findAlpha() {
        mMaxAlpha = 0.0;

        for (size_t iX = 0; iX < mGridX + mOffsetXL + mOffsetXR; iX++) {
            for (size_t iY = 0; iY < mGridY + mOffsetYU + mOffsetYD; iY++) {
                double vx = (*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0];
                double vy = (*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0];
                double Bx = (*CurrentData)[iX][iY][3] / (*CurrentData)[iX][iY][0];
                double By = (*CurrentData)[iX][iY][4] / (*CurrentData)[iX][iY][0];
                double SqrX = sqrt(pow(Bx, 2.0) + 9.81 * (*CurrentData)[iX][iY][0]);
                double SqrY = sqrt(pow(By, 2.0) + 9.81 * (*CurrentData)[iX][iY][0]);

                mMaxAlpha = std::max(fabs(vx + SqrX), mMaxAlpha);
                mMaxAlpha = std::max(fabs(vy + SqrY), mMaxAlpha);
            }
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

    Renderer*   VulkanRenderer;
    SDL_Event   Event;
    bool        Run                 = true;

    try {
        int Steps = 0;
        int Limit = 1440 * 15;

        auto Start = std::chrono::system_clock::now();
        auto End = std::chrono::system_clock::now();

        std::vector <std::vector <float>> GraphData;
        std::vector <std::vector <double>> GraphDataDouble;

        GraphData.resize(360);
        GraphDataDouble.resize(360);

        for (int i = 0; i < 360; i++) {
            GraphData[i].resize(50);
            GraphDataDouble[i].resize(50);
        }

        std::chrono::duration <double> DurSeconds;

        VulkanRenderer = new Renderer(Window);

        VulkanRenderer -> setApplicationName("dPlot");
        VulkanRenderer -> setApplicationVersion(0, 1, 0);
        VulkanRenderer -> setDebugMode(true);

        VulkanRenderer -> init();

        while (Run) {
            while (SDL_PollEvent(&Event)) {
                if (Event.type == SDL_QUIT) {
                    Run = false;
                }
            }

            GraphDataDouble = Test.getElevation();

            for (int i = 0; i < 360; i++) {
                for (int j = 0; j < 50; j++) {
                    GraphData[i][j] = GraphDataDouble[i][j];
                }
            }

            VulkanRenderer -> draw(GraphData);

            if (Steps < Limit) {
                Test.solve();
                Steps++;

                if (Steps == Limit) {
                    std::cout << std::endl << "Calculations finished!" << std::endl;
                }

                End = std::chrono::system_clock::now();
                DurSeconds = End - Start;

//                std::cout << DurSeconds.count() << "s" << std::endl;
                Start = End;
            } else {
                break;
            }
        }
    } catch (const std::runtime_error& tExcept) {
        std::cout << tExcept.what() << std::endl;

        SDL_DestroyWindow(Window);
        SDL_Quit();
    }

    SDL_DestroyWindow(Window);
    SDL_Quit();

    return 0;
}

