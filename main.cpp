#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <utility>
#include "Core/dVectors.h"
//-----------------------------//
#include "NumerCalc/dLaxFriedrichs1D.h"
#include "NumerCalc/dLeapFrog1D.h"
//-----------------------------//
int main() {
    dLaxFriedrichs1D <double> SolverLax;
    dLeapFrog1D <double> SolverLeap;

    SolverLax.setXStep(0.01);
    SolverLax.setTimeStep(0.005);

    SolverLeap.setXStep(0.01);
    SolverLeap.setTimeStep(0.005);

    //----------//

    double CurLayer[100];
    double NextLayer[100];

    for (int i = 0; i < 100; i++) {
        CurLayer[i] = sin(i / 100.0);
    }

    double TempVecMinus = 0;
    double TempVecPlus = 0;

    //----------//

    std::ofstream outputFile2;
    outputFile2.open("Test2.csv");

    for (int j = 0; j < 200; j++) {
        for (int i = 2; i < 98; i++) {
            TempVecMinus = SolverLax.solve(CurLayer[i - 2], CurLayer[i]);
            TempVecPlus = SolverLax.solve(CurLayer[i], CurLayer[i + 2]);

            NextLayer[i] = SolverLeap.solve(CurLayer[i], TempVecMinus, TempVecPlus);
//            NextLayer[i] = SolverLax.solve(CurLayer[i - 1], CurLayer[i + 1]);
        }

        NextLayer[0] = CurLayer[0] + 0.5 * (CurLayer[1] - CurLayer[0]);
        NextLayer[1] = CurLayer[1] + 0.5 * (CurLayer[2] - CurLayer[1]);
        NextLayer[98] = CurLayer[98] - 0.5 * (CurLayer[98] - CurLayer[97]);
        NextLayer[99] = CurLayer[99] - 0.5 * (CurLayer[99] - CurLayer[98]);

        std::swap(NextLayer, CurLayer);

        for (int i = 0; i < 100; i++) {
            if (i < 99) {
                outputFile2 << CurLayer[i] << ",";
            } else {
                outputFile2 << CurLayer[i];
            }
        }

        outputFile2 << '\n';
    }

    outputFile2.close();

    std::ofstream outputFile;

    outputFile.open("Test.csv");

    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 100; j++) {
            if (j < 99) {
                outputFile << sin(j / 100.0 - i / 200.0) << ",";
            } else {
                outputFile << sin(j / 100.0 - i / 200.0);
            }
        }

        outputFile << '\n';
    }

    outputFile.close();



//    dVectorND <double> Test = dVectorND <double>({0.1, 0.2, 0.3}) * 5 + dVectorND <double>({0.1, 0.2, 0.3}) * 3;
//
//    for (int i = 0; i < 3; i++) {
//        std::cout << Test[i] << std::endl;
//    }

//    auto Start = std::chrono::system_clock::now();
//
//    for (int i = 0; i < 150000000; i++) {
//        dVectorND <double>({0.1, 0.2, 0.3});
////        std::vector <double> Test = {0.1, 0.2, 0.3};
//    }
//
//    auto Stop = std::chrono::system_clock::now();
//
//    std::chrono::duration <double> Time = Stop - Start;
//    std::cout << Time.count() << std::endl;

    return 0;
}