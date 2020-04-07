#include <iostream>
#include <cmath>
#include <utility>
#include <ctime>
#include <fstream>

#include "Core/dVector.h"
#include "Core/dMatrix.h"

using dGrid = std::vector <std::vector <dVector <double, 6>>>;

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

        initCoriolis();
        initFields();
        bottomFunc();
        initConditions();

        GridTempData = GridCurrentData;

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
        std::vector <double> AlphaX(mGridY + mOffsetYU + mOffsetYD);
        std::vector <double> AlphaY(mGridX + mOffsetXL + mOffsetXR);

        double TimePassed = 0.0;

        for (int iTime = 0; iTime < mTimeLimit; iTime++) {
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

            if (mMaxAlpha > 500) {
                break;
            }

//            mStepTime = 0.2 * mStepX / mMaxAlpha;
            TimePassed += mStepTime;

            if (int(TimePassed) % (60 * 60) == 0) {
                std::cout << "Time: " << TimePassed / 3600 / 24 << " Step - " << mStepTime << " Alpha - " << mMaxAlpha
                          << std::endl;
            }

            for (size_t i = mOffsetXL + 1; i < mGridX + mOffsetXL; i++) {
                for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
                    auto PlusX = WENO(
                            (funcX((*CurrentData)[i - 1][j]) + mMaxAlpha * (*CurrentData)[i - 1][j]) / 2.0,
                            (funcX((*CurrentData)[i][j]) + mMaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcX((*CurrentData)[i + 1][j]) + mMaxAlpha * (*CurrentData)[i + 1][j]) / 2.0,
                            (funcX((*CurrentData)[i][j]) - mMaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcX((*CurrentData)[i + 1][j]) - mMaxAlpha * (*CurrentData)[i + 1][j]) / 2.0,
                            (funcX((*CurrentData)[i + 2][j]) - mMaxAlpha * (*CurrentData)[i + 2][j]) / 2.0
                    );
                    auto MinusX = WENO(
                            (funcX((*CurrentData)[i - 2][j]) + mMaxAlpha * (*CurrentData)[i - 2][j]) / 2.0,
                            (funcX((*CurrentData)[i - 1][j]) + mMaxAlpha * (*CurrentData)[i - 1][j]) / 2.0,
                            (funcX((*CurrentData)[i][j]) + mMaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcX((*CurrentData)[i - 1][j]) - mMaxAlpha * (*CurrentData)[i - 1][j]) / 2.0,
                            (funcX((*CurrentData)[i][j]) - mMaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcX((*CurrentData)[i + 1][j]) - mMaxAlpha * (*CurrentData)[i + 1][j]) / 2.0
                    );

                    auto PlusY = WENO(
                            (funcY((*CurrentData)[i][j - 1]) + mMaxAlpha * (*CurrentData)[i][j - 1]) / 2.0,
                            (funcY((*CurrentData)[i][j]) + mMaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcY((*CurrentData)[i][j + 1]) + mMaxAlpha * (*CurrentData)[i][j + 1]) / 2.0,
                            (funcY((*CurrentData)[i][j]) - mMaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcY((*CurrentData)[i][j + 1]) - mMaxAlpha * (*CurrentData)[i][j + 1]) / 2.0,
                            (funcY((*CurrentData)[i][j + 2]) - mMaxAlpha * (*CurrentData)[i][j + 2]) / 2.0
                    );
                    auto MinusY = WENO(
                            (funcY((*CurrentData)[i][j - 2]) + mMaxAlpha * (*CurrentData)[i][j - 2]) / 2.0,
                            (funcY((*CurrentData)[i][j - 1]) + mMaxAlpha * (*CurrentData)[i][j - 1]) / 2.0,
                            (funcY((*CurrentData)[i][j]) + mMaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcY((*CurrentData)[i][j - 1]) - mMaxAlpha * (*CurrentData)[i][j - 1]) / 2.0,
                            (funcY((*CurrentData)[i][j]) - mMaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcY((*CurrentData)[i][j + 1]) - mMaxAlpha * (*CurrentData)[i][j + 1]) / 2.0
                    );

                    (*TempData)[i][j] =
                            (*CurrentData)[i][j] -
                            mStepTime *
                            ((PlusX - MinusX) / mStepX + (PlusY - MinusY) / mStepY -
                            source(i, j));
                }
            }

            updateBoundaries();

            std::swap(CurrentData, TempData);
        }
    }
    void save() {
        for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
            for (size_t i = mOffsetXL; i < mGridX + mOffsetXL; i++) {
                EOutput << (*CurrentData)[i][j][0] + Bottom[i][j] << " ";
            }

            EOutput << std::endl;
        }
    }
private:
    size_t mGridX = 254;
    size_t mGridY = 50;

    size_t mOffsetXL = 1;
    size_t mOffsetXR = 2;
    size_t mOffsetYU = 2;
    size_t mOffsetYD = 2;

    dGrid GridCurrentData;
    dGrid GridTempData;

    dGrid *CurrentData = &GridCurrentData;
    dGrid *TempData = &GridTempData;

    double mStepX = 100.0e+03;
    double mStepY = 100.0e+03;

    double mStepTime = 60.0;

    double mTimeLimit = 90000;

    std::vector <std::vector <double>> Bottom;
    std::vector <double> mCorParam;
    std::vector <double> mHorizFieldY;
    std::vector <double> mVertField;
    
    double mMaxAlpha = 0.0;

    //----------//

    const double mGrav  = 9.81;
    const double mCorParam_0 = 1.0e-04;
    const double mBetaParam = 1.6e-11;

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
//            mHorizFieldY[i] = 3.5e-05 - 4.87e-07 * i;
//            mVertField[i] = 1.27e-05 + 1.46e-06 * i - 1.38e-08 * pow(i, 2.0);
            mHorizFieldY[i] = 0.0;
            mVertField[i] = 0.0;
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
        //---h---//
        for (size_t j = mOffsetYU; j < mGridY + mOffsetYU; j++) {
            (*TempData)[0][j][0] = (*TempData)[mGridX + mOffsetXL - 1][j][0];

            (*TempData)[mGridX + mOffsetXL + mOffsetXR - 2][j][0] = (*TempData)[mOffsetXL][j][0];
            (*TempData)[mGridX + mOffsetXL + mOffsetXR - 1][j][0] = (*TempData)[mOffsetXL + 1][j][0];
        }
        //---h---//

        //---West-East---//
        for (int iComp = 1; iComp < 3; iComp++) {
            for (size_t j = 0; j < mGridY + mOffsetYU + mOffsetYD; j++) {
                (*TempData)[0][j][iComp] =
                        (*TempData)[mOffsetXL][j][iComp] /
                        (*TempData)[mOffsetXL][j][0] *
                        (*TempData)[0][j][0];
                (*TempData)[1][j][iComp] =
                        (*TempData)[mOffsetXL][j][iComp] /
                        (*TempData)[mOffsetXL][j][0] *
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

        for (int iComp = 3; iComp < 6; iComp++) {
            for (size_t j = 0; j < mGridY + mOffsetYU + mOffsetYD; j++) {
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
        //---West-East---//

        //---vx---//
        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            (*TempData)[i][1][1] =
                    (*TempData)[i][2][1] /
                    (*TempData)[i][2][0] *
                    (*TempData)[i][1][0];
            (*TempData)[i][0][1] =
                    (*TempData)[i][2][1] /
                    (*TempData)[i][2][0] *
                    (*TempData)[i][0][0];

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][1] =
                    (*TempData)[i][mGridY + mOffsetYU - 1][1] /
                    (*TempData)[i][mGridY + mOffsetYU - 1][0] *
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][0];
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][1] =
                    (*TempData)[i][mGridY + mOffsetYU - 1][1] /
                    (*TempData)[i][mGridY + mOffsetYU - 1][0] *
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][0];
        }
        //---vx---//

        //---vy---//
        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            (*TempData)[i][1][2] = 0.0;
            (*TempData)[i][0][2] = 0.0;

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][2] = 0.0;
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][2] = 0.0;
        }
        //---vy---//

        //---Bx---//
        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            (*TempData)[i][1][3] =
                    (*TempData)[i][2][3] /
                    (*TempData)[i][2][0] *
                    (*TempData)[i][1][0];
            (*TempData)[i][0][3] =
                    (*TempData)[i][2][3] /
                    (*TempData)[i][2][0] *
                    (*TempData)[i][0][0];

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][3] =
                    (*TempData)[i][mGridY + mOffsetYU - 1][3] /
                    (*TempData)[i][mGridY + mOffsetYU - 1][0] *
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][0];
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][3] =
                    (*TempData)[i][mGridY + mOffsetYU - 1][3] /
                    (*TempData)[i][mGridY + mOffsetYU - 1][0] *
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][0];
        }
        //---Bx---//

        //---By---//
        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            (*TempData)[i][0][4] =
                    mHorizFieldY[0] *
                    (*TempData)[i][0][0];
            (*TempData)[i][1][4] =
                    ((*TempData)[i][2][4] / (*TempData)[i][2][0] +
                    (*TempData)[i][0][4] /
                    (*TempData)[i][0][0]) / 2.0 *
                    (*TempData)[i][1][0];

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][4] =
                    mHorizFieldY[mGridY + mOffsetYU + mOffsetYD - 1] *
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][4];
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][4] =
                    ((*TempData)[i][mGridY + mOffsetYU - 1][4] /
                    (*TempData)[i][mGridY + mOffsetYU - 1][0] +
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][4] /
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][0]) / 2.0 *
                    (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][0];
        }
        //---By---//

        //---Psi---//
        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            (*TempData)[i][1][5] = 0.0;
            (*TempData)[i][0][5] = 0.0;

            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 2][5] = 0.0;
            (*TempData)[i][mGridY + mOffsetYU + mOffsetYD - 1][5] = 0.0;
        }
        //---Psi---//
    }
    void initConditions() {
        double MeanWind = 20.0;
        double MeanY = int((mGridY + mOffsetYU + mOffsetYD) / 2) * mStepY;

        for (size_t i = 0; i < mGridX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 0; j < mGridY + mOffsetYU + mOffsetYD; j++) {
                double TempHeight = 10000.0 - (MeanWind * mCorParam_0 / mGrav) * (j * mStepY - MeanY);

                GridCurrentData[i][j] = dVector <double, 6>(
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

    dVector <double, 6> funcX(const dVector <double, 6>& tVal) {
        return dVector <double, 6>(
                tVal[1],
                (tVal[1] * tVal[1] - tVal[3] * tVal[3]) / tVal[0] + 0.5 * mGrav * tVal[0] * tVal[0],
                (tVal[1] * tVal[2] - tVal[3] * tVal[4]) / tVal[0],
                tVal[5],
                -(tVal[1] * tVal[4] - tVal[2] * tVal[3]) / tVal[0],
                mMaxAlpha * mMaxAlpha * tVal[3]
        );
    }
    dVector <double, 6> funcY(const dVector <double, 6>& tVal) {
        return dVector <double, 6>(
                tVal[2],
                (tVal[1] * tVal[2] - tVal[3] * tVal[4]) / tVal[0],
                (tVal[2] * tVal[2] - tVal[4] * tVal[4]) / tVal[0] + 0.5 * mGrav * tVal[0] * tVal[0],
                -(tVal[2] * tVal[3] - tVal[1] * tVal[4]) / tVal[0],
                tVal[5],
                mMaxAlpha * mMaxAlpha * tVal[4]
        );
    }
    dVector <double, 6> source(int tPosX, int tPosY) {
        double Bz = -mVertField[tPosY];

        return dVector <double, 6> (
                0.0,
                Bz * (*CurrentData)[tPosX][tPosY][3] / (*CurrentData)[tPosX][tPosY][0] +
                mCorParam[tPosY] * (*CurrentData)[tPosX][tPosY][2] -
                mGrav / (2.0 * mStepX) *
                (*CurrentData)[tPosX][tPosY][0] * (Bottom[tPosX + 1][tPosY] - Bottom[tPosX - 1][tPosY]),
                Bz * (*CurrentData)[tPosX][tPosY][4] / (*CurrentData)[tPosX][tPosY][0] +
                -mCorParam[tPosY] * (*CurrentData)[tPosX][tPosY][1] -
                mGrav / (2.0 * mStepY) *
                (*CurrentData)[tPosX][tPosY][0] * (Bottom[tPosX][tPosY + 1] - Bottom[tPosX][tPosY - 1]),
                Bz * (*CurrentData)[tPosX][tPosY][1] / (*CurrentData)[tPosX][tPosY][0],
                Bz * (*CurrentData)[tPosX][tPosY][2] / (*CurrentData)[tPosX][tPosY][0],
                0.0
                );
    }
    dVector <double, 6> viscosity(int tPosX, int tPosY) {
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

        return dVector <double, 6> (
                0.0,
                (*CurrentData)[tPosX][tPosY][0] * (v_x_xx + v_x_yy),
                (*CurrentData)[tPosX][tPosY][0] * (v_y_xx + v_y_yy),
                0.0,
                0.0,
                0.0);
    }

    //----------//

    dVector <double, 6> WENO(
            const dVector <double, 6>& tPosVal_minus_1,
            const dVector <double, 6>& tPosVal,
            const dVector <double, 6>& tPosVal_plus_1,
            const dVector <double, 6>& tNegVal,
            const dVector <double, 6>& tNegVal_plus_1,
            const dVector <double, 6>& tNegVal_plus_2) {
        dVector <double, 6> fPlus(0.0, 0.0, 0.0, 0.0, 0.0);
        dVector <double, 6> fMinus(0.0, 0.0, 0.0, 0.0, 0.0);

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
};

int main() {
    Solver Test;
    Test.solve();
    Test.save();

    return 0;
}