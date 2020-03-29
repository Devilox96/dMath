#include <iostream>
#include <cmath>
#include <utility>
#include <ctime>
#include <fstream>

#include "Core/dVector.h"
#include "Core/dMatrix.h"

class Solver {
public:
    Solver() {
        GridCurrentData.resize(NumX + Offset * 2);
        GridTempData.resize(NumX + Offset * 2);

        Bottom.resize(NumX + Offset * 2);

        for (size_t i = 0; i < NumX + Offset * 2; i++) {
            GridCurrentData[i].resize(NumX + Offset * 2);
            GridTempData[i].resize(NumX + Offset * 2);

            Bottom[i].resize(NumX + Offset * 2);
        }

        bottomFunc(Bottom);

        initConditions((*CurrentData));
        initConditions((*TempData));

        EOutput.open("Amplitude.dat");
    }
    ~Solver() {
        CurrentData = nullptr;
        TempData = nullptr;

        EOutput.close();
    }

    void solve() {
        for (int iTime = 0; iTime < 2000; iTime++) {
            std::vector <double> AlphaX(NumX + Offset * 2);
            std::vector <double> AlphaY(NumX + Offset * 2);

            for (size_t iY = Offset; iY < NumX + Offset; iY++) {
                for (size_t iX = Offset; iX < NumX + Offset; iX++) {
                    AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaX[iY]);
                    AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaX[iY]);
                    AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0]), AlphaX[iY]);
                }
            }

            for (size_t iX = Offset; iX < NumX + Offset; iX++) {
                for (size_t iY = Offset; iY < NumX + Offset; iY++) {
                    AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaY[iX]);
                    AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaY[iX]);
                    AlphaX[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0]), AlphaX[iX]);
                }
            }

            std::cout << iTime << ": " << (*CurrentData)[302][302] << std::endl;

            for (size_t i = Offset; i < NumX + Offset; i++) {
                for (size_t j = Offset; j < NumX + Offset; j++) {
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
                            0.001 / 0.5 * (PlusPlusX + PlusMinusX - MinusPlusX - MinusMinusX) -
                            0.001 / 0.5 * (PlusPlusY + PlusMinusY - MinusPlusY - MinusMinusY) -
                            0.001 *
                            dVector3D <double> (
                                    0.0,
                                    9.81 * (*CurrentData)[i][j][0] * (Bottom[i + 1][j] - Bottom[i - 1][j]),
                                    9.81 * (*CurrentData)[i][j][0] * (Bottom[i][j + 1] - Bottom[i][j - 1]));
                }
            }

            for (size_t i = 1; i < NumX + 2; i++) {
                (*TempData)[i][1][0] = 100.0;
                (*TempData)[i][0][0] = 100.0;
                (*TempData)[i][NumX + 2][0] = 2.0 * (*TempData)[i][NumX + 1][0] - (*TempData)[i][NumX][0];
                (*TempData)[i][NumX + 3][0] = (*TempData)[i][NumX + 2][0];

                (*TempData)[i][1][1] = 2.0 * (*TempData)[i][2][1] - (*TempData)[i][3][1];
                (*TempData)[i][0][1] = (*TempData)[i][1][1];
                (*TempData)[i][NumX + 2][1] = 2.0 * (*TempData)[i][NumX + 1][1] - (*TempData)[i][NumX][1];
                (*TempData)[i][NumX + 3][1] = (*TempData)[i][NumX + 2][1];

                (*TempData)[i][1][2] = 2.0 * (*TempData)[i][2][2] - (*TempData)[i][3][2];
                (*TempData)[i][0][2] = (*TempData)[i][1][2];
                (*TempData)[i][NumX + 2][2] = 2.0 * (*TempData)[i][NumX + 1][2] - (*TempData)[i][NumX][2];
                (*TempData)[i][NumX + 3][2] = (*TempData)[i][NumX + 2][2];
            }

            for (size_t j = 0; j < NumX + 2 * Offset; j++) {
                for (int k = 0; k < 3; k++) {
                    (*TempData)[1][j][k] = (*TempData)[2][j][k];
                    (*TempData)[0][j][k] = (*TempData)[3][j][k];

                    (*TempData)[NumX + 2][j][k] = (*TempData)[NumX + 1][j][k];
                    (*TempData)[NumX + 3][j][k] = (*TempData)[NumX][j][k];
                }
            }

            std::swap(CurrentData, TempData);
        }
    }
    void save() {
        for (size_t i = Offset; i < NumX + Offset; i++) {
            for (size_t j = Offset; j < NumX + Offset; j++) {
                EOutput << (*CurrentData)[i][j][0] << " ";
            }

            EOutput << std::endl;
        }
    }
private:
    size_t NumX = 600;
//    size_t SizeY = 600;

    size_t Offset = 2;

    std::vector <std::vector <dVector3D <double>>> GridCurrentData;
    std::vector <std::vector <dVector3D <double>>> GridTempData;

    std::vector <std::vector <dVector3D <double>>> *CurrentData = &GridCurrentData;
    std::vector <std::vector <dVector3D <double>>> *TempData = &GridTempData;

    std::vector <std::vector <double>> Bottom;

    std::ofstream EOutput;

    //----------//

    void bottomFunc(std::vector <std::vector <double>>& tVec) {
        for (int i = 0; i < 604; i++) {
            for (int j = 0; j < 202; j++) {
                tVec[i][j] = 25.0;
            }

            for (int j = 202; j < 402; j++) {
                tVec[i][j] = 25.0 - static_cast <double>(j - 202) / 8;
            }

            for (int j = 402; j < 604; j++) {
                tVec[i][j] = 0.0;
            }
        }
    }
    void initConditions(std::vector <std::vector <dVector3D <double>>>& tGrid) {
        for (int i = 0; i < 604; i++) {
            for (int j = 0; j < 202; j++) {
                tGrid[i][j][0] = 100.0;
                tGrid[i][j][1] = 0.0;
            }

            for (int j = 202; j < 604; j++) {
                tGrid[i][j][0] = 50.0;
                tGrid[i][j][1] = 0.0;
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

////---Problem---//
//
//size_t NumX = 600;
//size_t Offset = 2;
//
//dVector3D <double> funcX(const dVector3D <double>& tVal) {
//    return dVector3D <double>(
//            tVal[1],
//            tVal[1] * tVal[1] / tVal[0] + 0.5 * 9.81 * tVal[0] * tVal[0],
//            tVal[1] * tVal[2] / tVal[0]
//    );
//}
//dVector3D <double> funcY(const dVector3D <double>& tVal) {
//    return dVector3D <double>(
//            tVal[2],
//            tVal[1] * tVal[2] / tVal[0],
//            tVal[2] * tVal[2] / tVal[0] + 0.5 * 9.81 * tVal[0] * tVal[0]
//    );
//}
//
//void bottomFunc(std::vector <std::vector <double>>& tVec) {
//    for (int i = 0; i < 604; i++) {
//        for (int j = 0; j < 202; j++) {
//            tVec[i][j] = 25.0;
//        }
//
//        for (int j = 202; j < 402; j++) {
//            tVec[i][j] = 25.0 - static_cast <double>(j - 202) / 8;
//        }
//
//        for (int j = 402; j < 604; j++) {
//            tVec[i][j] = 0.0;
//        }
//    }
//}
//void initConditions(std::vector <std::vector <dVector3D <double>>>& tGrid) {
//    for (int i = 0; i < 604; i++) {
//        for (int j = 0; j < 202; j++) {
//            tGrid[i][j][0] = 100.0;
//            tGrid[i][j][1] = 0.0;
//        }
//
//        for (int j = 202; j < 604; j++) {
//            tGrid[i][j][0] = 50.0;
//            tGrid[i][j][1] = 0.0;
//        }
//    }
//}
//
////---Problem---//
//
////---Method---//
//
//dVector3D <double> WENO_2(const dVector3D <double>& tVal_minus, const dVector3D <double>& tVal, const dVector3D <double>& tVal_plus, bool tNegFlux) {
//    auto V0 = 0.5 * tVal + 0.5 * tVal_plus;       //---r = 0---//
//    auto V1 = -0.5 * tVal_minus + 1.5 * tVal;     //---r = 1---//
//
//    auto Beta0 = tVal_plus - tVal;
//    auto Beta1 = tVal - tVal_minus;
//
//    int Dim = tVal.size();
//
//    for (int i = 0; i < Dim; i++) {
//        Beta0[i] *= Beta0[i];
//        Beta1[i] *= Beta1[i];
//    }
//
//    dVector3D <double> Alpha0;
//    dVector3D <double> Alpha1;
//
//    if (tNegFlux) {
//        for (int i = 0; i < Dim; i++) {
//            Alpha0[i] = 1.0 / 3.0 / pow(1.0e-06 + Beta0[i], 2.0);
//            Alpha1[i] = 2.0 / 3.0 / pow(1.0e-06 + Beta1[i], 2.0);
//        }
//    } else {
//        for (int i = 0; i < Dim; i++) {
//            Alpha0[i] = 2.0 / 3.0 / pow(1.0e-06 + Beta0[i], 2.0);
//            Alpha1[i] = 1.0 / 3.0 / pow(1.0e-06 + Beta1[i], 2.0);
//        }
//    }
//
//    dVector3D <double> Omega0;
//    dVector3D <double> Omega1;
//
//    for (int i = 0; i < Dim; i++) {
//        Omega0[i] = Alpha0[i] / (Alpha0[i] + Alpha1[i]);
//        Omega1[i] = Alpha1[i] / (Alpha0[i] + Alpha1[i]);
//    }
//
//    dVector3D <double> Res;
//
//    for (int i = 0; i < Dim; i++) {
//        Res[i] = Omega0[i] * V0[i] + Omega1[i] * V1[i];
//    }
//
//    return Res;
//}
//
////---Method---//

int main() {
    Solver Test;
    Test.solve();
    Test.save();

//    int TimeToCompute = 1;  //---Sec---//
//
//    std::vector <std::vector <dVector3D <double>>> GridCurrentData(NumX + Offset * 2);
//    std::vector <std::vector <dVector3D <double>>> GridTempData(NumX + Offset * 2);
//
//    std::vector <std::vector <dVector3D <double>>> *CurrentData = &GridCurrentData;
//    std::vector <std::vector <dVector3D <double>>> *TempData = &GridTempData;
//
//    std::vector <std::vector <double>> Bottom(NumX + Offset * 2);
//
//    for (size_t i = 0; i < NumX + Offset * 2; i++) {
//        GridCurrentData[i].resize(NumX + Offset * 2);
//        GridTempData[i].resize(NumX + Offset * 2);
//
//        Bottom[i].resize(NumX + Offset * 2);
//    }
//
//    //----------//
//
//    bottomFunc(Bottom);
//
//    std::ofstream EOutput;
//    std::ofstream BottomOutput;
//
//    EOutput.open("Amplitude.dat");
//    BottomOutput.open("BottomOutput.dat");
//
//    initConditions((*CurrentData));
//    initConditions((*TempData));
//
//    for (int iTime = 0; iTime < 2000; iTime++) {
//        std::vector <double> AlphaX(NumX + Offset * 2);
//        std::vector <double> AlphaY(NumX + Offset * 2);
//
//        for (size_t iY = Offset; iY < NumX + Offset; iY++) {
//            for (size_t iX = Offset; iX < NumX + Offset; iX++) {
//                AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaX[iY]);
//                AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaX[iY]);
//                AlphaX[iY] = std::max(fabs((*CurrentData)[iX][iY][1] / (*CurrentData)[iX][iY][0]), AlphaX[iY]);
//            }
//        }
//
//        for (size_t iX = Offset; iX < NumX + Offset; iX++) {
//            for (size_t iY = Offset; iY < NumX + Offset; iY++) {
//                AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] + sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaY[iX]);
//                AlphaY[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0] - sqrt(9.81 * (*CurrentData)[iX][iY][0])), AlphaY[iX]);
//                AlphaX[iX] = std::max(fabs((*CurrentData)[iX][iY][2] / (*CurrentData)[iX][iY][0]), AlphaX[iX]);
//            }
//        }
//
//        std::cout << iTime << ": " << (*CurrentData)[302][302] << std::endl;
//
//        for (size_t i = Offset; i < NumX + Offset; i++) {
//            for (size_t j = Offset; j < NumX + Offset; j++) {
//                auto PlusPlusX = WENO_2(
//                        (funcX((*CurrentData)[i - 1][j]) + AlphaX[j] * (*CurrentData)[i - 1][j]) / 2.0,
//                        (funcX((*CurrentData)[i][j]) + AlphaX[j] * (*CurrentData)[i][j]) / 2.0,
//                        (funcX((*CurrentData)[i + 1][j]) + AlphaX[j] * (*CurrentData)[i + 1][j]) / 2.0,
//                        false);
//                auto PlusMinusX = WENO_2(
//                        (funcX((*CurrentData)[i][j]) - AlphaX[j] * (*CurrentData)[i][j]) / 2.0,
//                        (funcX((*CurrentData)[i + 1][j]) - AlphaX[j] * (*CurrentData)[i + 1][j]) / 2.0,
//                        (funcX((*CurrentData)[i + 2][j]) - AlphaX[j] * (*CurrentData)[i + 2][j]) / 2.0,
//                        true);
//
//                auto MinusPlusX = WENO_2(
//                        (funcX((*CurrentData)[i - 2][j]) + AlphaX[j] * (*CurrentData)[i - 2][j]) / 2.0,
//                        (funcX((*CurrentData)[i - 1][j]) + AlphaX[j] * (*CurrentData)[i - 1][j]) / 2.0,
//                        (funcX((*CurrentData)[i][j]) + AlphaX[j] * (*CurrentData)[i][j]) / 2.0,
//                        false);
//                auto MinusMinusX = WENO_2(
//                        (funcX((*CurrentData)[i - 1][j]) - AlphaX[j] * (*CurrentData)[i - 1][j]) / 2.0,
//                        (funcX((*CurrentData)[i][j]) - AlphaX[j] * (*CurrentData)[i][j]) / 2.0,
//                        (funcX((*CurrentData)[i + 1][j]) - AlphaX[j] * (*CurrentData)[i + 1][j]) / 2.0,
//                        true);
//
//                //----------//
//
//                auto PlusPlusY = WENO_2(
//                        (funcY((*CurrentData)[i][j - 1]) + AlphaY[i] * (*CurrentData)[i][j - 1]) / 2.0,
//                        (funcY((*CurrentData)[i][j]) + AlphaY[i] * (*CurrentData)[i][j]) / 2.0,
//                        (funcY((*CurrentData)[i][j + 1]) + AlphaY[i] * (*CurrentData)[i][j + 1]) / 2.0,
//                        false);
//                auto PlusMinusY = WENO_2(
//                        (funcY((*CurrentData)[i][j]) - AlphaY[i] * (*CurrentData)[i][j]) / 2.0,
//                        (funcY((*CurrentData)[i][j + 1]) - AlphaY[i] * (*CurrentData)[i][j + 1]) / 2.0,
//                        (funcY((*CurrentData)[i][j + 2]) - AlphaY[i] * (*CurrentData)[i][j + 2]) / 2.0,
//                        true);
//
//                auto MinusPlusY = WENO_2(
//                        (funcY((*CurrentData)[i][j - 2]) + AlphaY[i] * (*CurrentData)[i][j - 2]) / 2.0,
//                        (funcY((*CurrentData)[i][j - 1]) + AlphaY[i] * (*CurrentData)[i][j - 1]) / 2.0,
//                        (funcY((*CurrentData)[i][j]) + AlphaY[i] * (*CurrentData)[i][j]) / 2.0,
//                        false);
//                auto MinusMinusY = WENO_2(
//                        (funcY((*CurrentData)[i][j - 1]) - AlphaY[i] * (*CurrentData)[i][j - 1]) / 2.0,
//                        (funcY((*CurrentData)[i][j]) - AlphaY[i] * (*CurrentData)[i][j]) / 2.0,
//                        (funcY((*CurrentData)[i][j + 1]) - AlphaY[i] * (*CurrentData)[i][j + 1]) / 2.0,
//                        true);
//
//                (*TempData)[i][j] =
//                        (*CurrentData)[i][j] -
//                        0.001 / 0.5 * (PlusPlusX + PlusMinusX - MinusPlusX - MinusMinusX) -
//                        0.001 / 0.5 * (PlusPlusY + PlusMinusY - MinusPlusY - MinusMinusY) -
//                        0.001 *
//                        dVector3D <double> (
//                                0.0,
//                                9.81 * (*CurrentData)[i][j][0] * (Bottom[i + 1][j] - Bottom[i - 1][j]),
//                                9.81 * (*CurrentData)[i][j][0] * (Bottom[i][j + 1] - Bottom[i][j - 1]));
//            }
//        }
//
//        for (size_t i = 1; i < NumX + 2; i++) {
//            (*TempData)[i][1][0] = 100.0;
//            (*TempData)[i][0][0] = 100.0;
//            (*TempData)[i][NumX + 2][0] = 2.0 * (*TempData)[i][NumX + 1][0] - (*TempData)[i][NumX][0];
//            (*TempData)[i][NumX + 3][0] = (*TempData)[i][NumX + 2][0];
//
//            (*TempData)[i][1][1] = 2.0 * (*TempData)[i][2][1] - (*TempData)[i][3][1];
//            (*TempData)[i][0][1] = (*TempData)[i][1][1];
//            (*TempData)[i][NumX + 2][1] = 2.0 * (*TempData)[i][NumX + 1][1] - (*TempData)[i][NumX][1];
//            (*TempData)[i][NumX + 3][1] = (*TempData)[i][NumX + 2][1];
//
//            (*TempData)[i][1][2] = 2.0 * (*TempData)[i][2][2] - (*TempData)[i][3][2];
//            (*TempData)[i][0][2] = (*TempData)[i][1][2];
//            (*TempData)[i][NumX + 2][2] = 2.0 * (*TempData)[i][NumX + 1][2] - (*TempData)[i][NumX][2];
//            (*TempData)[i][NumX + 3][2] = (*TempData)[i][NumX + 2][2];
//        }
//
//        for (size_t j = 0; j < NumX + 2 * Offset; j++) {
//            for (int k = 0; k < 3; k++) {
//                (*TempData)[1][j][k] = (*TempData)[2][j][k];
//                (*TempData)[0][j][k] = (*TempData)[3][j][k];
//
//                (*TempData)[NumX + 2][j][k] = (*TempData)[NumX + 1][j][k];
//                (*TempData)[NumX + 3][j][k] = (*TempData)[NumX][j][k];
//            }
//        }
//
//        std::swap(CurrentData, TempData);
//    }
//
//    //-----------------------------//
//
//    for (size_t i = Offset; i < NumX + Offset; i++) {
//        for (size_t j = Offset; j < NumX + Offset; j++) {
//            EOutput << (*CurrentData)[i][j][0] << " ";
//        }
//
//        EOutput << std::endl;
//    }
//
//    EOutput.close();
////
////    for (size_t i = 0; i < NumX; i++) {
////        BottomOutput << Bottom[i] << " ";
////    }

    return 0;
}