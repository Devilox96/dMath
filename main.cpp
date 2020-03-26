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

double Func(double tVal) {
    return tVal * tVal / 2.0;
}

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
    size_t Num = 200;

    double ul = 30;
    double ur = 10;
    double alpha = 1.0;
//    double xl = 15.0;
    double xl = 0.0;
//    double xr = 35.0;
    double xr = 20.0;

    int Sec = 4;

    std::vector <double> GridFirst(Num);
    std::vector <double> GridSecond(Num);

    std::vector <double> GridExact(Num);

    std::vector <double> *First = &GridFirst;
    std::vector <double> *Second = &GridSecond;

    //---Lax---//

    for (size_t i = 0; i < xl; i++) {
        GridFirst[i] = ul;
        GridSecond[i] = ul;
    }

    for (size_t i = xl; i < xr; i++) {
        GridFirst[i] = ul - alpha * double(i - xl);
        GridSecond[i] = ul - alpha * double(i - xl);
    }

    for (size_t i = xr; i < Num; i++) {
        GridFirst[i] = ur;
        GridSecond[i] = ur;
    }

    std::ofstream EOutput;
    std::ofstream Exact;

    EOutput.open("EAmplitude.dat");
    Exact.open("Exact.dat");

    for (int i = 0; i < Sec * 1000; i++) {
        double Alpha = 0.0;

        for (const auto& iNum : (*First)) {
            Alpha = std::max(iNum, Alpha);
        }

        for (size_t j = 2; j < Num - 1; j++) {
            double PlusPlus = WENO_2(
                    (Func((*First)[j - 1]) + Alpha * (*First)[j - 1]) / 2.0,
                    (Func((*First)[j]) + Alpha * (*First)[j]) / 2.0,
                    (Func((*First)[j + 1]) + Alpha * (*First)[j + 1]) / 2.0, false);
            double PlusMinus = WENO_2(
                    (Func((*First)[j - 1]) - Alpha * (*First)[j - 1]) / 2.0,
                    (Func((*First)[j]) - Alpha * (*First)[j]) / 2.0,
                    (Func((*First)[j + 1]) - Alpha * (*First)[j + 1]) / 2.0, true);

            double MinusPlus = WENO_2(
                    (Func((*First)[j - 2]) + Alpha * (*First)[j - 2]) / 2.0,
                    (Func((*First)[j - 1]) + Alpha * (*First)[j - 1]) / 2.0,
                    (Func((*First)[j]) + Alpha * (*First)[j]) / 2.0, false);
            double MinusMinus = WENO_2(
                    (Func((*First)[j - 2]) - Alpha * (*First)[j - 2]) / 2.0,
                    (Func((*First)[j - 1]) - Alpha * (*First)[j - 1]) / 2.0,
                    (Func((*First)[j]) - Alpha * (*First)[j]) / 2.0, true);

            (*Second)[j] = (*First)[j] - 0.001 / 1.0 * (PlusPlus + PlusMinus - MinusPlus - MinusMinus);
        }

        (*Second)[0] = ul;
        (*Second)[1] = ul;
        (*Second)[Num - 1] = ur;
//        (*Second)[Num - 2] = ur;
//        (*Second)[Num - 3] = ur;

        std::swap(First, Second);
    }

    for (size_t i = 0; i < Num; i++) {
        EOutput << (*First)[i] << " ";
    }

    //-----------------------------//

    double Time = Sec;

    for (int i = 0; i < Num; i++) {
        if (i < Time * ul) {
            GridExact[i] = ul;
        } else if (i >= Time * ul && i <= xr + ur * Time) {
            GridExact[i] = (ul - alpha * i) / (1 - alpha * Time);
        } else {
            GridExact[i] = ur;
        }

//        std::cout << i << std::endl;
    }

    for (size_t i = 0; i < Num; i++) {
        Exact << GridExact[i] << " ";
    }

    return 0;
}