#include <iostream>
#include <cmath>
#include <utility>
#include <ctime>
#include <fstream>

#include "Core/dVector.h"
#include "Core/dMatrix.h"

using dGrid = std::vector <std::vector <dVector3D <double>>>;

class Solver {
public:
    Solver() {
        GridCurrentData.resize(mSizeX + mOffsetXL + mOffsetXR);
        GridTempData.resize(mSizeX + mOffsetXL + mOffsetXR);

        Bottom.resize(mSizeX + mOffsetXL + mOffsetXR);

        for (size_t i = 0; i < mSizeX + mOffsetXL + mOffsetXR; i++) {
            GridCurrentData[i].resize(mSizeY + mOffsetYU + mOffsetYD);
            GridTempData[i].resize(mSizeY + mOffsetYU + mOffsetYD);

            Bottom[i].resize(mSizeY + mOffsetYU + mOffsetYD);
        }

        initCoriolis();
        bottomFunc();
        initConditions();

        EOutput.open("Amplitude.dat");
    }
    ~Solver() {
        CurrentData = nullptr;
        TempData = nullptr;

        EOutput.close();
    }

    void solve() {
        std::vector <double> AlphaX(mSizeY + mOffsetYU + mOffsetYD);
        std::vector <double> AlphaY(mSizeX + mOffsetXL + mOffsetXR);

        for (int iTime = 0; iTime < 20; iTime++) {
            double MaxAlpha = 0.0;

            for (size_t iY = 0; iY < mSizeY + mOffsetYU + mOffsetYD; iY++) {
                for (size_t iX = 0; iX < mSizeX + mOffsetXL + mOffsetXR; iX++) {
                    AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaX[iY]);
                    AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaX[iY]);
                    AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0]), AlphaX[iY]);
                }
            }

            for (size_t iX = 0; iX < mSizeX + mOffsetXL + mOffsetXR; iX++) {
                for (size_t iY = 0; iY < mSizeY + mOffsetYU + mOffsetYD; iY++) {
                    AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaY[iX]);
                    AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaY[iX]);
                    AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0]), AlphaY[iX]);
                }
            }

            for (size_t iX = 0; iX < mSizeX + mOffsetXL + mOffsetXR; iX++) {
                for (size_t iY = 0; iY < mSizeY + mOffsetYU + mOffsetYD; iY++) {
                    MaxAlpha = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), MaxAlpha);
                    MaxAlpha = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), MaxAlpha);
                    MaxAlpha = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0]), MaxAlpha);

                    MaxAlpha = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), MaxAlpha);
                    MaxAlpha = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), MaxAlpha);
                    MaxAlpha = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0]), MaxAlpha);
                }
            }

            if (MaxAlpha > 500) {
                break;
            }

//            mStepTime = 0.1 * mStepX / MaxAlpha;
            std::cout << iTime << ": Step - " << mStepTime << " Alpha - " << MaxAlpha << std::endl;

            for (size_t i = mOffsetXL; i < mSizeX + mOffsetXL; i++) {
                for (size_t j = mOffsetYU; j < mSizeY + mOffsetYU; j++) {
                    auto PositiveX = WENO_PosFlux(
                            (funcX((*CurrentData)[i - 2][j]) + MaxAlpha * (*CurrentData)[i - 2][j]) / 2.0,
                            (funcX((*CurrentData)[i - 1][j]) + MaxAlpha * (*CurrentData)[i - 1][j]) / 2.0,
                            (funcX((*CurrentData)[i][j]) + MaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcX((*CurrentData)[i + 1][j]) + MaxAlpha * (*CurrentData)[i + 1][j]) / 2.0
                    );
                    auto NegativeX = WENO_NegFlux(
                            (funcX((*CurrentData)[i - 1][j]) - MaxAlpha * (*CurrentData)[i - 1][j]) / 2.0,
                            (funcX((*CurrentData)[i][j]) - MaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcX((*CurrentData)[i + 1][j]) - MaxAlpha * (*CurrentData)[i + 1][j]) / 2.0,
                            (funcX((*CurrentData)[i + 2][j]) - MaxAlpha * (*CurrentData)[i + 2][j]) / 2.0
                    );

                    auto PositiveY = WENO_PosFlux(
                            (funcY((*CurrentData)[i][j - 2]) + MaxAlpha * (*CurrentData)[i][j - 2]) / 2.0,
                            (funcY((*CurrentData)[i][j - 1]) + MaxAlpha * (*CurrentData)[i][j - 1]) / 2.0,
                            (funcY((*CurrentData)[i][j]) + MaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcY((*CurrentData)[i][j + 1]) + MaxAlpha * (*CurrentData)[i][j + 1]) / 2.0
                    );
                    auto NegativeY = WENO_NegFlux(
                            (funcY((*CurrentData)[i][j - 1]) - MaxAlpha * (*CurrentData)[i][j - 1]) / 2.0,
                            (funcY((*CurrentData)[i][j]) - MaxAlpha * (*CurrentData)[i][j]) / 2.0,
                            (funcY((*CurrentData)[i][j + 1]) - MaxAlpha * (*CurrentData)[i][j + 1]) / 2.0,
                            (funcY((*CurrentData)[i][j + 2]) - MaxAlpha * (*CurrentData)[i][j + 2]) / 2.0
                    );
                    
                    
//                    auto PlusPlusX = WENO_2(
//                            (funcX((*CurrentData)[i - 1][j]) + MaxAlpha * (*CurrentData)[i - 1][j]) / 2.0,
//                            (funcX((*CurrentData)[i][j]) + MaxAlpha * (*CurrentData)[i][j]) / 2.0,
//                            (funcX((*CurrentData)[i + 1][j]) + MaxAlpha * (*CurrentData)[i + 1][j]) / 2.0,
//                            false);
//                    auto PlusMinusX = WENO_2(
//                            (funcX((*CurrentData)[i - 1][j]) - MaxAlpha * (*CurrentData)[i - 1][j]) / 2.0,
//                            (funcX((*CurrentData)[i][j]) - MaxAlpha * (*CurrentData)[i][j]) / 2.0,
//                            (funcX((*CurrentData)[i + 1][j]) - MaxAlpha * (*CurrentData)[i + 1][j]) / 2.0,
//                            true);
//
//                    auto MinusPlusX = WENO_2(
//                            (funcX((*CurrentData)[i - 2][j]) + MaxAlpha * (*CurrentData)[i - 2][j]) / 2.0,
//                            (funcX((*CurrentData)[i - 1][j]) + MaxAlpha * (*CurrentData)[i - 1][j]) / 2.0,
//                            (funcX((*CurrentData)[i][j]) + MaxAlpha * (*CurrentData)[i][j]) / 2.0,
//                            false);
//                    auto MinusMinusX = WENO_2(
//                            (funcX((*CurrentData)[i - 2][j]) - MaxAlpha * (*CurrentData)[i - 2][j]) / 2.0,
//                            (funcX((*CurrentData)[i - 1][j]) - MaxAlpha * (*CurrentData)[i - 1][j]) / 2.0,
//                            (funcX((*CurrentData)[i][j]) - MaxAlpha * (*CurrentData)[i][j]) / 2.0,
//                            true);

                    //----------//

//                    auto PlusPlusY = WENO_2(
//                            (funcY((*CurrentData)[i][j - 1]) + MaxAlpha * (*CurrentData)[i][j - 1]) / 2.0,
//                            (funcY((*CurrentData)[i][j]) + MaxAlpha * (*CurrentData)[i][j]) / 2.0,
//                            (funcY((*CurrentData)[i][j + 1]) + MaxAlpha * (*CurrentData)[i][j + 1]) / 2.0,
//                            false);
//                    auto PlusMinusY = WENO_2(
//                            (funcY((*CurrentData)[i][j - 1]) - MaxAlpha * (*CurrentData)[i][j - 1]) / 2.0,
//                            (funcY((*CurrentData)[i][j]) - MaxAlpha * (*CurrentData)[i][j]) / 2.0,
//                            (funcY((*CurrentData)[i][j + 1]) - MaxAlpha * (*CurrentData)[i][j + 1]) / 2.0,
//                            true);
//
//                    auto MinusPlusY = WENO_2(
//                            (funcY((*CurrentData)[i][j - 2]) + MaxAlpha * (*CurrentData)[i][j - 2]) / 2.0,
//                            (funcY((*CurrentData)[i][j - 1]) + MaxAlpha * (*CurrentData)[i][j - 1]) / 2.0,
//                            (funcY((*CurrentData)[i][j]) + MaxAlpha * (*CurrentData)[i][j]) / 2.0,
//                            false);
//                    auto MinusMinusY = WENO_2(
//                            (funcY((*CurrentData)[i][j - 2]) - MaxAlpha * (*CurrentData)[i][j - 2]) / 2.0,
//                            (funcY((*CurrentData)[i][j - 1]) - MaxAlpha * (*CurrentData)[i][j - 1]) / 2.0,
//                            (funcY((*CurrentData)[i][j]) - MaxAlpha * (*CurrentData)[i][j]) / 2.0,
//                            true);

//                    (*TempData)[i][j] =
//                            ((*CurrentData)[i + 1][j] + (*CurrentData)[i - 1][j] + (*CurrentData)[i][j - 1] + (*CurrentData)[i][j + 1]) / 4.0 -
//                            mStepTime *
//                            ((PlusPlusX + PlusMinusX - MinusPlusX - MinusMinusX) / mStepX -
//                            (PlusPlusY + PlusMinusY - MinusPlusY - MinusMinusY) / mSizeY +
//                            viscosity(i, j));
                    (*TempData)[i][j] =
                            (*CurrentData)[i][j] -
                            mStepTime *
                            ((PositiveX + NegativeX) / mStepX +
                            (PositiveY + NegativeY) / mSizeY);
                }
            }

            updateBoundaries();

            std::swap(CurrentData, TempData);
        }
    }
    void save() {
        for (size_t j = mOffsetYU; j < mSizeY + mOffsetYU; j++) {
            for (size_t i = mOffsetXL; i < mSizeX + mOffsetXL; i++) {
                EOutput << (*CurrentData)[i][j][0] << " ";
            }

            EOutput << std::endl;
        }
    }
private:
    size_t mSizeX = 254;
    size_t mSizeY = 50;

    size_t mOffsetXL = 2;
    size_t mOffsetXR = 2;
    size_t mOffsetYU = 2;
    size_t mOffsetYD = 2;

    dGrid GridCurrentData;
    dGrid GridTempData;

    dGrid *CurrentData = &GridCurrentData;
    dGrid *TempData = &GridTempData;

    double mStepX = 100.0e+03;
    double mStepY = 100.0e+03;

    double mStepTime = 0.05;

    std::vector <std::vector <double>> Bottom;
    std::vector <double> mCorParam;

    //----------//

    const double mGrav  = 9.81;
    const double mCorParam_0 = 1.0e-04;
    const double mBetaParam = 1.6e-11;

    //----------//

    std::ofstream EOutput;

    //----------//

    void initCoriolis() {
        double MeanY = int((mSizeY + mOffsetYU + mOffsetYD) / 2) * mStepY;

        mCorParam.resize(mSizeY + mOffsetYU + mOffsetYD);

        for (size_t i = 0; i < mSizeY + mOffsetYU + mOffsetYD; i++) {
            mCorParam[i] = mCorParam_0 + mBetaParam * (i * mStepY - MeanY);
        }
    }
    void bottomFunc() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution <> dis(0.0, 500.0);

        double StdX = 5.0 * mStepX;
        double StdY = 5.0 * mStepY;

        double MeanX = int((mSizeX + mOffsetXL + mOffsetXR) / 2) * mStepX;
        double MeanY = int((mSizeY + mOffsetYU + mOffsetYD) / 2) * mStepY;

        for (size_t i = 0; i < mSizeX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 0; j < mSizeY + mOffsetYU + mOffsetYD; j++) {
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
        for (size_t j = 0; j < mSizeY + mOffsetYU + mOffsetYD; j++) {
            (*TempData)[0][j][0] = (*TempData)[mSizeX + mOffsetXL - 2][j][0];
            (*TempData)[1][j][0] = (*TempData)[mSizeX + mOffsetXL - 1][j][0];

            (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 2][j][0] = (*TempData)[mOffsetXL][j][0];
            (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 1][j][0] = (*TempData)[mOffsetXL + 1][j][0];
        }
        //---h---//

        //---vx---//
        for (size_t j = 0; j < mSizeY + mOffsetYU + mOffsetYD; j++) {
            (*TempData)[0][j][1] =
                    (*TempData)[mSizeX + mOffsetXL - 1][j][1] / (*TempData)[mSizeX + mOffsetXL - 1][j][0] *
                    (*TempData)[0][j][0];
            (*TempData)[1][j][1] =
                    (*TempData)[mSizeX + mOffsetXL - 1][j][1] / (*TempData)[mSizeX + mOffsetXL - 1][j][0] *
                    (*TempData)[1][j][0];

            (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 2][j][1] =
                    (*TempData)[mOffsetXL][j][1] / (*TempData)[mOffsetXL][j][0] *
                    (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 2][j][0];
            (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 1][j][1] =
                    (*TempData)[mOffsetXL + 1][j][1] / (*TempData)[mOffsetXL + 1][j][0] *
                    (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 1][j][0];
        }

        for (size_t i = 0; i < mSizeX + mOffsetXL + mOffsetXR; i++) {
            (*TempData)[i][1][1] =
                    (*TempData)[i][2][1] / (*TempData)[i][2][0] / 2.0 *
                    (*TempData)[i][1][0];
            (*TempData)[i][0][1] = 0.0;

            (*TempData)[i][mSizeY + mOffsetYU + mOffsetYD - 2][1] =
                    (*TempData)[i][mSizeY + mOffsetYU - 1][1] / (*TempData)[i][mSizeY + mOffsetYU - 1][0] / 2.0 *
                    (*TempData)[i][mSizeY + mOffsetYU + mOffsetYD - 2][0];
            (*TempData)[i][mSizeY + mOffsetYU + mOffsetYD - 1][1] = 0.0;
        }
        //---vx---//

        //---vy---//
        for (size_t j = 0; j < mSizeY + mOffsetYU + mOffsetYD; j++) {
            (*TempData)[0][j][2] =
                    (*TempData)[mSizeX + mOffsetXL - 2][j][2] / (*TempData)[mSizeX + mOffsetXL - 1][j][0] *
                    (*TempData)[0][j][0];
            (*TempData)[1][j][2] =
                    (*TempData)[mSizeX + mOffsetXL - 1][j][2] / (*TempData)[mSizeX + mOffsetXL - 1][j][0] *
                    (*TempData)[1][j][0];

            (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 2][j][2] =
                    (*TempData)[mOffsetXL][j][2] / (*TempData)[mOffsetXL][j][0] *
                    (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 2][j][0];
            (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 1][j][2] =
                    (*TempData)[mOffsetXL + 1][j][2] / (*TempData)[mOffsetXL + 1][j][0] *
                    (*TempData)[mSizeX + mOffsetXL + mOffsetXR - 1][j][0];
        }

        for (size_t i = 0; i < mSizeX + mOffsetXL + mOffsetXR; i++) {
            (*TempData)[i][1][2] =
                    (*TempData)[i][2][2] / (*TempData)[i][2][0] / 2.0 *
                    (*TempData)[i][1][0];
            (*TempData)[i][0][2] = 0.0;

            (*TempData)[i][mSizeY + mOffsetYU + mOffsetYD - 2][2] =
                    (*TempData)[i][mSizeY + mOffsetYU - 1][2] / (*TempData)[i][mSizeY + mOffsetYU - 1][0] / 2.0 *
                    (*TempData)[i][mSizeY + mOffsetYU + mOffsetYD - 2][0];
            (*TempData)[i][mSizeY + mOffsetYU + mOffsetYD - 1][2] = 0.0;
        }
        //---vy---//
    }
    void initConditions() {
        double MeanWind = 20.0;
        double MeanY = int((mSizeY + mOffsetYU + mOffsetYD) / 2) * mStepY;

        for (size_t i = 0; i < mSizeX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 0; j < mSizeY + mOffsetYU + mOffsetYD; j++) {
                double TempHeight = 10000.0 - (MeanWind * mCorParam_0 / mGrav) * (j * mStepY - MeanY);

                GridCurrentData[i][j] = dVector3D <double>(TempHeight, 0.0, 0.0);
                GridTempData[i][j] = dVector3D <double>(TempHeight, 0.0, 0.0);
            }
        }

        for (size_t i = 0; i < mSizeX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 1; j < mSizeY + mOffsetYU + mOffsetYD - 1; j++) {
                GridCurrentData[i][j][1] =
                        GridCurrentData[i][j][0] *
                        (-0.5 * mGrav / (mCorParam[j] * mStepX) *
                        (GridCurrentData[i][j + 1][0] - GridCurrentData[i][j - 1][0]));
                GridTempData[i][j][1] =
                        GridTempData[i][j][0] *
                        (-0.5 * mGrav / (mCorParam[j] * mStepX) *
                        (GridTempData[i][j + 1][0] - GridTempData[i][j - 1][0]));
            }
        }

        for (size_t i = 1; i < mSizeX + mOffsetXL + mOffsetXR - 1; i++) {
            for (size_t j = 0; j < mSizeY + mOffsetYU + mOffsetYD; j++) {
                GridCurrentData[i][j][2] =
                        GridCurrentData[i][j][0] *
                        (0.5 * mGrav / (mCorParam[j] * mStepX) *
                        (GridCurrentData[i + 1][j][0] - GridCurrentData[i - 1][j][0]));
                GridTempData[i][j][2] =
                        GridTempData[i][j][0] *
                        (0.5 * mGrav / (mCorParam[j] * mStepX) *
                        (GridTempData[i + 1][j][0] - GridTempData[i - 1][j][0]));
            }
        }

        for (size_t i = 0; i < mSizeX + mOffsetXL + mOffsetXR; i++) {
            GridCurrentData[i][0][1] = 0.0;
            GridTempData[i][0][1] = 0.0;

            GridCurrentData[i][0][2] = 0.0;
            GridTempData[i][0][2] = 0.0;

            GridCurrentData[i][mSizeY + mOffsetYU + mOffsetYD - 1][1] = 0.0;
            GridTempData[i][mSizeY + mOffsetYU + mOffsetYD - 1][1] = 0.0;

            GridCurrentData[i][mSizeY + mOffsetYU + mOffsetYD - 1][2] = 0.0;
            GridTempData[i][mSizeY + mOffsetYU + mOffsetYD - 1][2] = 0.0;
        }

        for (size_t i = 0; i < mSizeX + mOffsetXL + mOffsetXR; i++) {
            for (size_t j = 0; j < mSizeY + mOffsetYU + mOffsetYD; j++) {
                GridCurrentData[i][j][0] -= Bottom[i][j];
                GridTempData[i][j][0] -= Bottom[i][j];
            }
        }

        updateBoundaries();
        std::swap(CurrentData, TempData);
    }

    dVector3D <double> funcX(const dVector3D <double>& tVal) {
        return dVector3D <double>(
                tVal[1],
                tVal[1] * tVal[1] / tVal[0] + 0.5 * 9.81 * tVal[0] * tVal[0],
                tVal[1] * tVal[2] / tVal[0]
        );
    }
    dVector3D <double> funcY(const dVector3D <double>& tVal) {
        return dVector3D <double>(
                tVal[2],
                tVal[1] * tVal[2] / tVal[0],
                tVal[2] * tVal[2] / tVal[0] + 0.5 * 9.81 * tVal[0] * tVal[0]
        );
    }
    dVector3D <double> source(int tPosX, int tPosY) {
        return dVector3D <double> (
                0.0,
                mCorParam[tPosY] * (*CurrentData)[tPosX][tPosY][2] -
                mGrav / (2.0 * mStepX) *
                (*CurrentData)[tPosX][tPosY][0] * (Bottom[tPosX + 1][tPosY] - Bottom[tPosX - 1][tPosY]),
                -mCorParam[tPosY] * (*CurrentData)[tPosX][tPosY][1] -
                mGrav / (2.0 * mStepY) *
                (*CurrentData)[tPosX][tPosY][0] * (Bottom[tPosX][tPosY + 1] - Bottom[tPosX][tPosY - 1]));
    }
    dVector3D <double> viscosity(int tPosX, int tPosY) {
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

        return dVector3D <double> (
                0.0,
                (*CurrentData)[tPosX][tPosY][0] * (v_x_xx + v_x_yy),
                (*CurrentData)[tPosX][tPosY][0] * (v_y_xx + v_y_yy));
    }

    //----------//

    dVector3D <double> WENO_NegFlux(
            const dVector3D <double>& tVal_minus_1,
            const dVector3D <double>& tVal,
            const dVector3D <double>& tVal_plus_1,
            const dVector3D <double>& tVal_plus_2) {
        int Dim = 3;

        dVector3D <double> V0_Plus;
        dVector3D <double> V1_Plus;

        dVector3D <double> V0_Minus;
        dVector3D <double> V1_Minus;

        V0_Plus = 0.5 * tVal + 0.5 * tVal_plus_1;
        V1_Plus = 1.5 * tVal_plus_1 - 0.5 * tVal_plus_2;

        V0_Minus = 0.5 * tVal_minus_1 + 0.5 * tVal;
        V1_Minus = 1.5 * tVal - 0.5 * tVal_plus_1;

        auto Beta0_Plus = tVal_plus_1 - tVal;
        auto Beta1_Plus = tVal_plus_2 - tVal_plus_1;

        auto Beta0_Minus = tVal - tVal_minus_1;
        auto Beta1_Minus = tVal_plus_1 - tVal;

        for (int i = 0; i < Dim; i++) {
            Beta0_Plus[i] *= Beta0_Plus[i];
            Beta1_Plus[i] *= Beta1_Plus[i];

            Beta0_Minus[i] *= Beta0_Minus[i];
            Beta1_Minus[i] *= Beta1_Minus[i];
        }

        dVector3D <double> Alpha0_Plus;
        dVector3D <double> Alpha1_Plus;

        dVector3D <double> Alpha0_Minus;
        dVector3D <double> Alpha1_Minus;

        for (int i = 0; i < Dim; i++) {
            Alpha0_Plus[i] = 2.0 / 3.0 / pow(1.0e-06 + Beta0_Plus[i], 2.0);
            Alpha1_Plus[i] = 1.0 / 3.0 / pow(1.0e-06 + Beta1_Plus[i], 2.0);

            Alpha0_Minus[i] = 2.0 / 3.0 / pow(1.0e-06 + Beta0_Minus[i], 2.0);
            Alpha1_Minus[i] = 1.0 / 3.0 / pow(1.0e-06 + Beta1_Minus[i], 2.0);
        }

        dVector3D <double> Omega0_Plus;
        dVector3D <double> Omega1_Plus;

        dVector3D <double> Omega0_Minus;
        dVector3D <double> Omega1_Minus;

        for (int i = 0; i < Dim; i++) {
            Omega0_Plus[i] = Alpha0_Plus[i] / (Alpha0_Plus[i] + Alpha1_Plus[i]);
            Omega1_Plus[i] = Alpha1_Plus[i] / (Alpha0_Plus[i] + Alpha1_Plus[i]);

            Omega0_Minus[i] = Alpha0_Minus[i] / (Alpha0_Minus[i] + Alpha1_Minus[i]);
            Omega1_Minus[i] = Alpha1_Minus[i] / (Alpha0_Minus[i] + Alpha1_Minus[i]);
        }

        dVector3D <double> Res;

        for (int i = 0; i < Dim; i++) {
            Res[i] =
                    (Omega0_Plus[i] * V0_Plus[i] + Omega1_Plus[i] * V1_Plus[i]) -
                    (Omega0_Minus[i] * V0_Minus[i] + Omega1_Minus[i] * V1_Minus[i]);
        }

        return Res;
    }
    dVector3D <double> WENO_PosFlux(
            const dVector3D <double>& tVal_minus_2,
            const dVector3D <double>& tVal_minus_1,
            const dVector3D <double>& tVal,
            const dVector3D <double>& tVal_plus_1) {
        int Dim = 3;

        dVector3D <double> V0_Plus;
        dVector3D <double> V1_Plus;

        dVector3D <double> V0_Minus;
        dVector3D <double> V1_Minus;

        V0_Plus = -0.5 * tVal_minus_1 + 1.5 * tVal;
        V1_Plus = 0.5 * tVal + 0.5 * tVal_plus_1;

        V0_Minus = -0.5 * tVal_minus_2 + 1.5 + tVal_minus_1;
        V1_Minus = 0.5 * tVal_minus_1 + 0.5 + tVal;

        auto Beta0_Plus = tVal - tVal_minus_1;
        auto Beta1_Plus = tVal_plus_1 - tVal;

        auto Beta0_Minus = tVal_minus_1 - tVal_minus_2;
        auto Beta1_Minus = tVal - tVal_minus_1;

        for (int i = 0; i < Dim; i++) {
            Beta0_Plus[i] *= Beta0_Plus[i];
            Beta1_Plus[i] *= Beta1_Plus[i];

            Beta0_Minus[i] *= Beta0_Minus[i];
            Beta1_Minus[i] *= Beta1_Minus[i];
        }

        dVector3D <double> Alpha0_Plus;
        dVector3D <double> Alpha1_Plus;

        dVector3D <double> Alpha0_Minus;
        dVector3D <double> Alpha1_Minus;

        for (int i = 0; i < Dim; i++) {
            Alpha0_Plus[i] = 1.0 / 3.0 / pow(1.0e-06 + Beta0_Plus[i], 2.0);
            Alpha1_Plus[i] = 2.0 / 3.0 / pow(1.0e-06 + Beta1_Plus[i], 2.0);

            Alpha0_Minus[i] = 1.0 / 3.0 / pow(1.0e-06 + Beta0_Minus[i], 2.0);
            Alpha1_Minus[i] = 2.0 / 3.0 / pow(1.0e-06 + Beta1_Minus[i], 2.0);
        }

        dVector3D <double> Omega0_Plus;
        dVector3D <double> Omega1_Plus;

        dVector3D <double> Omega0_Minus;
        dVector3D <double> Omega1_Minus;

        for (int i = 0; i < Dim; i++) {
            Omega0_Plus[i] = Alpha0_Plus[i] / (Alpha0_Plus[i] + Alpha1_Plus[i]);
            Omega1_Plus[i] = Alpha1_Plus[i] / (Alpha0_Plus[i] + Alpha1_Plus[i]);

            Omega0_Minus[i] = Alpha0_Minus[i] / (Alpha0_Minus[i] + Alpha1_Minus[i]);
            Omega1_Minus[i] = Alpha1_Minus[i] / (Alpha0_Minus[i] + Alpha1_Minus[i]);
        }

        dVector3D <double> Res;

        for (int i = 0; i < Dim; i++) {
            Res[i] =
                    (Omega0_Plus[i] * V0_Plus[i] + Omega1_Plus[i] * V1_Plus[i]) -
                    (Omega0_Minus[i] * V0_Minus[i] + Omega1_Minus[i] * V1_Minus[i]);
        }

        return Res;
    }

    dVector3D <double> WENO_2(const dVector3D <double>& tVal_minus, const dVector3D <double>& tVal, const dVector3D <double>& tVal_plus, bool tNegFlux) {
        dVector3D <double> V0;
        dVector3D <double> V1;

        if (tNegFlux) {
            V0 = 0.5 * tVal + 0.5 * tVal_minus;       //---r = 0---//
            V1 = -0.5 * tVal_plus + 1.5 * tVal;     //---r = 1---//
        } else {
            V0 = 0.5 * tVal + 0.5 * tVal_plus;       //---r = 0---//
            V1 = -0.5 * tVal_minus + 1.5 * tVal;     //---r = 1---//
        }

        auto Beta0 = tVal_plus - tVal;
        auto Beta1 = tVal - tVal_minus;

        int Dim = tVal.size();

        for (int i = 0; i < Dim; i++) {
            Beta0[i] *= Beta0[i];
            Beta1[i] *= Beta1[i];
        }

        dVector3D <double> Alpha0;
        dVector3D <double> Alpha1;

        if (tNegFlux) {
            for (int i = 0; i < Dim; i++) {
                Alpha0[i] = 1.0 / 3.0 / pow(1.0e-06 + Beta0[i], 2.0);
                Alpha1[i] = 2.0 / 3.0 / pow(1.0e-06 + Beta1[i], 2.0);
            }
        } else {
            for (int i = 0; i < Dim; i++) {
                Alpha0[i] = 2.0 / 3.0 / pow(1.0e-06 + Beta0[i], 2.0);
                Alpha1[i] = 1.0 / 3.0 / pow(1.0e-06 + Beta1[i], 2.0);
            }
        }

        dVector3D <double> Omega0;
        dVector3D <double> Omega1;

        for (int i = 0; i < Dim; i++) {
            Omega0[i] = Alpha0[i] / (Alpha0[i] + Alpha1[i]);
            Omega1[i] = Alpha1[i] / (Alpha0[i] + Alpha1[i]);
        }

        dVector3D <double> Res;

        for (int i = 0; i < Dim; i++) {
            Res[i] = Omega0[i] * V0[i] + Omega1[i] * V1[i];
        }

        return Res;
    }
};

int main() {
    Solver Test;
    Test.solve();
    Test.save();

    return 0;
}