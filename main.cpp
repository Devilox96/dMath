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
        GridCurrentData.resize(mSizeX + mOffset * 2);
        GridTempData.resize(mSizeX + mOffset * 2);

        Bottom.resize(mSizeX + mOffset * 2);

        for (size_t i = 0; i < mSizeX + mOffset * 2; i++) {
            GridCurrentData[i].resize(mSizeY + mOffset * 2);
            GridTempData[i].resize(mSizeY + mOffset * 2);

            Bottom[i].resize(mSizeY + mOffset * 2);
        }

        bottomFunc();

        initConditions();
        initConditions();

        EOutput.open("Amplitude.dat");
    }
    ~Solver() {
        CurrentData = nullptr;
        TempData = nullptr;

        EOutput.close();
    }

    void solve() {
        std::vector <double> AlphaX(mSizeY + mOffset * 2);
        std::vector <double> AlphaY(mSizeX + mOffset * 2);

        for (int iTime = 0; iTime < 2000; iTime++) {
            for (size_t iY = 0; iY < mSizeY + 2 * mOffset; iY++) {
                for (size_t iX = 0; iX < mSizeX + 2 * mOffset; iX++) {
                    AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaX[iY]);
                    AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaX[iY]);
                    AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0]), AlphaX[iY]);
                }
            }

            for (size_t iX = 0; iX < mSizeX + 2 * mOffset; iX++) {
                for (size_t iY = 0; iY < mSizeY + 2 * mOffset; iY++) {
                    AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaY[iX]);
                    AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaY[iX]);
                    AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0]), AlphaY[iX]);
                }
            }

            std::cout << iTime << ": " << (*CurrentData)[size_t(mSizeX / 2)][size_t(mSizeY / 2)] << std::endl;

            for (size_t i = mOffset; i < mSizeX + mOffset; i++) {
                for (size_t j = mOffset; j < mSizeY + mOffset; j++) {
                    auto PlusPlusX = WENO_2(
                            (funcX((*CurrentData)[i - 1][j]) + AlphaX[j] * (*CurrentData)[i - 1][j]) / 2.0,
                            (funcX((*CurrentData)[i][j]) + AlphaX[j] * (*CurrentData)[i][j]) / 2.0,
                            (funcX((*CurrentData)[i + 1][j]) + AlphaX[j] * (*CurrentData)[i + 1][j]) / 2.0,
                            false);
                    auto PlusMinusX = WENO_2(
                            (funcX((*CurrentData)[i][j]) - AlphaX[j] * (*CurrentData)[i][j]) / 2.0,
                            (funcX((*CurrentData)[i + 1][j]) - AlphaX[j] * (*CurrentData)[i + 1][j]) / 2.0,
                            (funcX((*CurrentData)[i + 2][j]) - AlphaX[j] * (*CurrentData)[i + 2][j]) / 2.0,
                            true);

                    auto MinusPlusX = WENO_2(
                            (funcX((*CurrentData)[i - 2][j]) + AlphaX[j] * (*CurrentData)[i - 2][j]) / 2.0,
                            (funcX((*CurrentData)[i - 1][j]) + AlphaX[j] * (*CurrentData)[i - 1][j]) / 2.0,
                            (funcX((*CurrentData)[i][j]) + AlphaX[j] * (*CurrentData)[i][j]) / 2.0,
                            false);
                    auto MinusMinusX = WENO_2(
                            (funcX((*CurrentData)[i - 1][j]) - AlphaX[j] * (*CurrentData)[i - 1][j]) / 2.0,
                            (funcX((*CurrentData)[i][j]) - AlphaX[j] * (*CurrentData)[i][j]) / 2.0,
                            (funcX((*CurrentData)[i + 1][j]) - AlphaX[j] * (*CurrentData)[i + 1][j]) / 2.0,
                            true);

                    //----------//

                    auto PlusPlusY = WENO_2(
                            (funcY((*CurrentData)[i][j - 1]) + AlphaY[i] * (*CurrentData)[i][j - 1]) / 2.0,
                            (funcY((*CurrentData)[i][j]) + AlphaY[i] * (*CurrentData)[i][j]) / 2.0,
                            (funcY((*CurrentData)[i][j + 1]) + AlphaY[i] * (*CurrentData)[i][j + 1]) / 2.0,
                            false);
                    auto PlusMinusY = WENO_2(
                            (funcY((*CurrentData)[i][j]) - AlphaY[i] * (*CurrentData)[i][j]) / 2.0,
                            (funcY((*CurrentData)[i][j + 1]) - AlphaY[i] * (*CurrentData)[i][j + 1]) / 2.0,
                            (funcY((*CurrentData)[i][j + 2]) - AlphaY[i] * (*CurrentData)[i][j + 2]) / 2.0,
                            true);

                    auto MinusPlusY = WENO_2(
                            (funcY((*CurrentData)[i][j - 2]) + AlphaY[i] * (*CurrentData)[i][j - 2]) / 2.0,
                            (funcY((*CurrentData)[i][j - 1]) + AlphaY[i] * (*CurrentData)[i][j - 1]) / 2.0,
                            (funcY((*CurrentData)[i][j]) + AlphaY[i] * (*CurrentData)[i][j]) / 2.0,
                            false);
                    auto MinusMinusY = WENO_2(
                            (funcY((*CurrentData)[i][j - 1]) - AlphaY[i] * (*CurrentData)[i][j - 1]) / 2.0,
                            (funcY((*CurrentData)[i][j]) - AlphaY[i] * (*CurrentData)[i][j]) / 2.0,
                            (funcY((*CurrentData)[i][j + 1]) - AlphaY[i] * (*CurrentData)[i][j + 1]) / 2.0,
                            true);

                    (*TempData)[i][j] =
                            (*CurrentData)[i][j] -
                            mStepTime / mStepX * (PlusPlusX + PlusMinusX - MinusPlusX - MinusMinusX) -
                            mStepTime / mStepY * (PlusPlusY + PlusMinusY - MinusPlusY - MinusMinusY) -
                            mStepTime *
                            dVector3D <double> (
                                    0.0,
                                    9.81 * (*CurrentData)[i][j][0] * (Bottom[i + 1][j] - Bottom[i - 1][j]),
                                    9.81 * (*CurrentData)[i][j][0] * (Bottom[i][j + 1] - Bottom[i][j - 1]));
                }
            }

            for (size_t i = 1; i < mSizeX + 2; i++) {
                (*TempData)[i][1][0] = 100.0;
                (*TempData)[i][0][0] = 100.0;
                (*TempData)[i][mSizeY + 2][0] = 2.0 * (*TempData)[i][mSizeY + 1][0] - (*TempData)[i][mSizeY][0];
                (*TempData)[i][mSizeY + 3][0] = (*TempData)[i][mSizeY + 2][0];

                (*TempData)[i][1][1] = 2.0 * (*TempData)[i][2][1] - (*TempData)[i][3][1];
                (*TempData)[i][0][1] = (*TempData)[i][1][1];
                (*TempData)[i][mSizeY + 2][1] = 2.0 * (*TempData)[i][mSizeY + 1][1] - (*TempData)[i][mSizeY][1];
                (*TempData)[i][mSizeY + 3][1] = (*TempData)[i][mSizeY + 2][1];

                (*TempData)[i][1][2] = 2.0 * (*TempData)[i][2][2] - (*TempData)[i][3][2];
                (*TempData)[i][0][2] = (*TempData)[i][1][2];
                (*TempData)[i][mSizeY + 2][2] = 2.0 * (*TempData)[i][mSizeY + 1][2] - (*TempData)[i][mSizeY][2];
                (*TempData)[i][mSizeY + 3][2] = (*TempData)[i][mSizeY + 2][2];
            }

            for (size_t j = 0; j < mSizeY + 2 * mOffset; j++) {
                for (int k = 0; k < 3; k++) {
                    (*TempData)[1][j][k] = (*TempData)[2][j][k];
                    (*TempData)[0][j][k] = (*TempData)[3][j][k];

                    (*TempData)[mSizeX + 2][j][k] = (*TempData)[mSizeX + 1][j][k];
                    (*TempData)[mSizeX + 3][j][k] = (*TempData)[mSizeX][j][k];
                }
            }

            std::swap(CurrentData, TempData);
        }
    }
    void save() {
        for (size_t j = mOffset; j < mSizeY + mOffset; j++) {
            for (size_t i = mOffset; i < mSizeX + mOffset; i++) {
                EOutput << (*CurrentData)[i][j][0] << " ";
            }

            EOutput << std::endl;
        }
    }
private:
    size_t mSizeX = 254;
    size_t mSizeY = 51;

    size_t mOffset = 2;

    dGrid GridCurrentData;
    dGrid GridTempData;

    dGrid *CurrentData = &GridCurrentData;
    dGrid *TempData = &GridTempData;

    double mStepX = 0.1;
    double mStepY = 0.1;

    double mStepTime = 0.0001;

    std::vector <std::vector <double>> Bottom;

    std::ofstream EOutput;

    //----------//

    void bottomFunc() {
        for (size_t i = 0; i < mSizeX + 2 * mOffset; i++) {
            for (size_t j = 0; j < size_t(mSizeY / 3) + mOffset; j++) {
                Bottom[i][j] = 25.0;
            }

            for (size_t j = size_t(mSizeY / 3) + mOffset; j < size_t(mSizeY / 3 * 2) + mOffset; j++) {
                Bottom[i][j] = 25.0 - (double)(j - size_t(mSizeY / 3)) / 8;
            }

            for (size_t j = size_t(mSizeY / 3 * 2) + mOffset; j < mSizeY + 2 * mOffset; j++) {
                Bottom[i][j] = 0.0;
            }
        }
    }
    void initConditions() {
        for (size_t i = 0; i < mSizeX + 2 * mOffset; i++) {
            for (size_t j = 0; j < size_t(mSizeY / 3) + mOffset; j++) {
                GridCurrentData[i][j][0] = 100.0;
                GridCurrentData[i][j][1] = 0.0;

                GridTempData[i][j][0] = 100.0;
                GridTempData[i][j][1] = 0.0;
            }

            for (size_t j = size_t(mSizeY / 3) + mOffset; j < mSizeY + 2 * mOffset; j++) {
                GridCurrentData[i][j][0] = 50.0;
                GridCurrentData[i][j][1] = 0.0;

                GridTempData[i][j][0] = 50.0;
                GridTempData[i][j][1] = 0.0;
            }
        }
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

    //----------//

    dVector3D <double> WENO_2(const dVector3D <double>& tVal_minus, const dVector3D <double>& tVal, const dVector3D <double>& tVal_plus, bool tNegFlux) {
        auto V0 = 0.5 * tVal + 0.5 * tVal_plus;       //---r = 0---//
        auto V1 = -0.5 * tVal_minus + 1.5 * tVal;     //---r = 1---//

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