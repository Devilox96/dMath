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

int main() {
    size_t Num = 200;

    double ul = 30;
    double ur = 10;
    double alpha = 1.0;
    double xl = 15.0;
//    double xl = 0.0;
    double xr = 35.0;
//    double xr = 20.0;

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

    std::ofstream Output;
    std::ofstream ROutput;
    std::ofstream EOutput;
    std::ofstream Exact;

    Output.open("Amplitude.dat");
    ROutput.open("RAmplitude.dat");
    EOutput.open("EAmplitude.dat");
    Exact.open("Exact.dat");

    //-----------------------------//

    LaxSolver Solver;

    Solver.setTimeStep(0.01);
    Solver.setXStep(1.0);

    for (int i = 0; i < Sec * 100; i++) {
        for (size_t j = 1; j < Num - 1; j++) {
            (*Second)[j] = Solver.solve((*First)[j - 1], (*First)[j + 1]);
        }

        (*Second)[0] = ul;
        (*Second)[Num - 1] = ur;

        std::swap(First, Second);
    }

    for (size_t i = 0; i < Num; i++) {
        Output << (*First)[i] << " ";
    }

    //---Lax---//
    //---Richtmyer---//

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

    RichtmyerSolver RSolver;

    RSolver.setTimeStep(0.02);
    RSolver.setXStep(1.0);

    for (int i = 0; i < Sec * 50; i++) {
        for (size_t j = 1; j < Num - 1; j++) {
            (*Second)[j] = RSolver.solve((*First)[j - 1], (*First)[j], (*First)[j + 1]);
        }

        (*Second)[0] = ul;
        (*Second)[Num - 1] = ur;

        std::swap(First, Second);
    }

    for (size_t i = 0; i < Num; i++) {
        ROutput << (*First)[i] << " ";
    }

    //---ENO---//

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

    for (int i = 0; i < 200; i++) {
        for (size_t j = 2; j < Num - 2; j++) {
            double Plus =
                    (-1.0 / (*First)[j - 1] * (*First)[j - 1] +
                    7.0 / (*First)[j] * (*First)[j] +
                    7.0 / (*First)[j + 1] * (*First)[j + 1] -
                    1.0 / (*First)[j + 2] * (*First)[j + 2]) / 24.0;
            double Minus =
                    (-1.0 / (*First)[j - 2] * (*First)[j - 2] +
                     7.0 / (*First)[j - 1] * (*First)[j - 1] +
                     7.0 / (*First)[j] * (*First)[j] -
                     1.0 / (*First)[j + 1] * (*First)[j + 1]) / 24.0;

            (*Second)[j] = (*First)[j] - 0.02 / 1.0 * (Plus - Minus);
        }

        (*Second)[0] = ul;
        (*Second)[1] = ul;
        (*Second)[Num - 1] = ur;
        (*Second)[Num - 2] = ur;
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

        std::cout << i << std::endl;
    }

    for (size_t i = 0; i < Num; i++) {
        Exact << GridExact[i] << " ";
    }

    return 0;
}