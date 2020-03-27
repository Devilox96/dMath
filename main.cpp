#include <iostream>
#include <cmath>
#include <utility>
#include <ctime>
#include <fstream>

#include "Core/dVector.h"
#include "Core/dMatrix.h"

#include "NumerCalc/dRungeKutta.h"
#include "NumerCalc/dENO.h"
#include "NumerCalc/dLaxFriedrichs1D.h"
#include "NumerCalc/dRichtmyer1D.h"

template<typename F, typename... Args>
void test(F func, Args&&... args) {
    std::clock_t start = std::clock();
    for (int i = 0; i < 150'000'000; ++i) {
        func(std::forward<Args>(args)...);
    }
    std::clock_t end = std::clock();

    std::cout << double(end - start) / CLOCKS_PER_SEC << " s\n";
}

class LaxSolver : public dLaxFriedrichs1D <double> {
private:
    double func(const double& tVal) override {
        return tVal * tVal / 2.0;
    }
};

class RichtmyerSolver : public dRichtmyer1D <double> {
private:
    double func(const double& tVal) override {
        return tVal * tVal / 2.0;
    }
};

double getEnergy(const std::vector <double>& tGrid) {
    double Energy = 0.0;

    for (const auto& iElem : tGrid) {
        Energy += iElem;
    }

    return Energy;
}

//---Problem---//

dVector2D <double> funcX(const dVector2D <double>& tVal) {
    return dVector2D <double>(
            tVal[1],
            tVal[1] * tVal[1] / tVal[0] + 0.5 * 9.81 * tVal[0] * tVal[0]
            );
}

void bottomFunc(std::vector <double>& tVec) {
    for (int i = 0; i < 200; i++) {
        tVec[i] = 25.0;
    }

    for (int i = 200; i < 400; i++) {
        tVec[i] = 25.0 - static_cast <double>(i - 200) / 8;
    }

    for (int i = 400; i < 600; i++) {
        tVec[i] = 0.0;
    }
}

void initConditions(std::vector <dVector2D <double>>& tGrid) {
    for (int i = 0; i < 200; i++) {
        tGrid[i][0] = 100.0;
        tGrid[i][1] = 0.0;
    }

    for (int i = 200; i < 600; i++) {
        tGrid[i][0] = 50.0;
        tGrid[i][1] = 0.0;
    }
}

//---Problem---//

double WENO_2(const double& tVal_minus, const double& tVal, const double& tVal_plus, bool tNegFlux) {
    double V0 = 0.5 * tVal + 0.5 * tVal_plus;       //---r = 0---//
    double V1 = -0.5 * tVal_minus + 1.5 * tVal;     //---r = 1---//

    double Beta0 = pow(tVal_plus - tVal, 2.0);
    double Beta1 = pow(tVal - tVal_minus, 2.0);

    double Alpha0;
    double Alpha1;

    if (tNegFlux) {
        Alpha0 = 1.0 / 3.0 / pow(1.0e-06 + Beta0, 2.0);
        Alpha1 = 2.0 / 3.0 / pow(1.0e-06 + Beta1, 2.0);
    } else {
        Alpha0 = 2.0 / 3.0 / pow(1.0e-06 + Beta0, 2.0);
        Alpha1 = 1.0 / 3.0 / pow(1.0e-06 + Beta1, 2.0);
    }

    double Omega0 = Alpha0 / (Alpha0 + Alpha1);
    double Omega1 = Alpha1 / (Alpha0 + Alpha1);

    return Omega0 * V0 + Omega1 * V1;
}

int main() {
    size_t Num = 600;
    int TimeToCompute = 1;  //---Sec---//

    std::vector <dVector2D <double>> GridCurrentData(Num);
    std::vector <dVector2D <double>> GridTempData(Num);

    std::vector <dVector2D <double>> *CurrentData = &GridCurrentData;
    std::vector <dVector2D <double>> *TempData = &GridTempData;

    std::vector <double> Bottom(Num);
    bottomFunc(Bottom);

    std::ofstream EOutput;
    std::ofstream BottomOutput;

    EOutput.open("EAmplitude.dat");
    BottomOutput.open("BottomOutput.dat");

    initConditions((*CurrentData));
    initConditions((*TempData));

    for (int i = 0; i < 7090; i++) {
        double Alpha = 0.0;

        for (const auto& iNum : (*CurrentData)) {
            Alpha = std::max(fabs(iNum[1] / iNum[0] + sqrt(9.81 * iNum[0])), Alpha);
            Alpha = std::max(fabs(iNum[1] / iNum[0] - sqrt(9.81 * iNum[0])), Alpha);

            if (iNum[0] <= 0) {
                std::cout << iNum[0] << " " << i << std::endl;
            }
        }

        for (size_t j = 2; j < Num - 2; j++) {
            auto PlusPlus = dVector2D <double>(
                    WENO_2(
                            (funcX((*CurrentData)[j - 1])[0] + Alpha * (*CurrentData)[j - 1][0]) / 2.0,
                            (funcX((*CurrentData)[j])[0] + Alpha * (*CurrentData)[j][0]) / 2.0,
                            (funcX((*CurrentData)[j + 1])[0] + Alpha * (*CurrentData)[j + 1][0]) / 2.0,
                            false),
                    WENO_2(
                            (funcX((*CurrentData)[j - 1])[1] + Alpha * (*CurrentData)[j - 1][1]) / 2.0,
                            (funcX((*CurrentData)[j])[1] + Alpha * (*CurrentData)[j][1]) / 2.0,
                            (funcX((*CurrentData)[j + 1])[1] + Alpha * (*CurrentData)[j + 1][1]) / 2.0,
                            false));
            auto PlusMinus = dVector2D <double>(
                    WENO_2(
                            (funcX((*CurrentData)[j])[0] - Alpha * (*CurrentData)[j][0]) / 2.0,
                            (funcX((*CurrentData)[j + 1])[0] - Alpha * (*CurrentData)[j + 1][0]) / 2.0,
                            (funcX((*CurrentData)[j + 2])[0] - Alpha * (*CurrentData)[j + 2][0]) / 2.0,
                            true),
                    WENO_2(
                            (funcX((*CurrentData)[j])[1] - Alpha * (*CurrentData)[j][1]) / 2.0,
                            (funcX((*CurrentData)[j + 1])[1] - Alpha * (*CurrentData)[j + 1][1]) / 2.0,
                            (funcX((*CurrentData)[j + 2])[1] - Alpha * (*CurrentData)[j + 2][1]) / 2.0,
                            true));

            auto MinusPlus = dVector2D <double>(
                    WENO_2(
                            (funcX((*CurrentData)[j - 2])[0] + Alpha * (*CurrentData)[j - 2][0]) / 2.0,
                            (funcX((*CurrentData)[j - 1])[0] + Alpha * (*CurrentData)[j - 1][0]) / 2.0,
                            (funcX((*CurrentData)[j])[0] + Alpha * (*CurrentData)[j][0]) / 2.0,
                            false),
                    WENO_2(
                            (funcX((*CurrentData)[j - 2])[1] + Alpha * (*CurrentData)[j - 2][1]) / 2.0,
                            (funcX((*CurrentData)[j - 1])[1] + Alpha * (*CurrentData)[j - 1][1]) / 2.0,
                            (funcX((*CurrentData)[j])[1] + Alpha * (*CurrentData)[j][1]) / 2.0,
                            false));
            auto MinusMinus = dVector2D <double>(
                    WENO_2(
                            (funcX((*CurrentData)[j - 1])[0] - Alpha * (*CurrentData)[j - 1][0]) / 2.0,
                            (funcX((*CurrentData)[j])[0] - Alpha * (*CurrentData)[j][0]) / 2.0,
                            (funcX((*CurrentData)[j + 1])[0] - Alpha * (*CurrentData)[j + 1][0]) / 2.0,
                            true),
                    WENO_2(
                            (funcX((*CurrentData)[j - 1])[1] - Alpha * (*CurrentData)[j - 1][1]) / 2.0,
                            (funcX((*CurrentData)[j])[1] - Alpha * (*CurrentData)[j][1]) / 2.0,
                            (funcX((*CurrentData)[j + 1])[1] - Alpha * (*CurrentData)[j + 1][1]) / 2.0,
                            true));

            (*TempData)[j] =
                    (*CurrentData)[j] -
                    0.001 / 0.5 * (PlusPlus + PlusMinus - MinusPlus - MinusMinus) -
                    0.001 * dVector2D <double>(0.0, 9.81 * (*CurrentData)[j][0] * (Bottom[j + 1] - Bottom[j - 1]) / 0.5);
        }

        (*TempData)[1][0] = 100.0;
        (*TempData)[0][0] = 100.0;
        (*TempData)[Num - 2][0] = 2.0 * (*TempData)[Num - 3][0] - (*TempData)[Num - 4][0];
        (*TempData)[Num - 1][0] = (*TempData)[Num - 2][0];

        (*TempData)[1][1] = 2.0 * (*TempData)[2][1] - (*TempData)[3][1];
        (*TempData)[0][1] = (*TempData)[1][1];
        (*TempData)[Num - 2][1] = 2.0 * (*TempData)[Num - 3][0] - (*TempData)[Num - 4][0];
        (*TempData)[Num - 1][1] = (*TempData)[Num - 2][0];

        std::swap(CurrentData, TempData);
    }

    //-----------------------------//

    for (size_t i = 0; i < Num; i++) {
        EOutput << (*CurrentData)[i][0] << " ";
    }

    for (size_t i = 0; i < Num; i++) {
        BottomOutput << Bottom[i] << " ";
    }

    return 0;
}