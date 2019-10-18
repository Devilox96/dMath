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
#include "Vector.h"
//-----------------------------//
template <typename T, size_t SizeT>
class dVectorND_variadic_array {
public:
    template <typename ... Args>
    explicit dVectorND_variadic_array <T, SizeT>(Args ... tArgs) : Data({tArgs ...}), Size(SizeT) {}
    ~dVectorND_variadic_array <T, SizeT>() = default;
private:
    std::array<T, SizeT> Data;
    unsigned long Size;
};
//-----------------------------//
int main() {
//    dLaxFriedrichs1D <double> SolverLax;
//    dLeapFrog1D <double> SolverLeap;
//
//    SolverLax.setXStep(M_PI / 50.0);
//    SolverLax.setTimeStep(1.0 / 50.0);
//
//    SolverLeap.setXStep(0.01);
//    SolverLeap.setTimeStep(0.005);
//
//    //----------//
//
//    double CurLayer[100];
//    double NextLayer[100];
//
//    for (int i = 0; i < 100; i++) {
//        CurLayer[i] = sin(-M_PI + i * M_PI / 50.0) + 2.0;
//    }
//
//    double TempVecMinus = 0;
//    double TempVecPlus = 0;
//
//    //----------//
//
//    std::ofstream outputFile2;
//    outputFile2.open("Test2.csv");
//
//    for (int j = 0; j < 100; j++) {
//        for (int i = 1; i < 99; i++) {
////            TempVecMinus = SolverLax.solve(CurLayer[i - 2], CurLayer[i]);
////            TempVecPlus = SolverLax.solve(CurLayer[i], CurLayer[i + 2]);
//
////            NextLayer[i] = SolverLeap.solve(CurLayer[i], TempVecMinus, TempVecPlus);
//            NextLayer[i] = SolverLax.solve(CurLayer[i - 1], CurLayer[i + 1]);
//        }
//
////        NextLayer[0] = CurLayer[0] + (1.0 / M_PI) * (CurLayer[1] - CurLayer[0]);
//        NextLayer[0] = SolverLax.solve(CurLayer[99], CurLayer[1]);
//        NextLayer[99] = SolverLax.solve(CurLayer[98], CurLayer[0]);
////        NextLayer[1] = CurLayer[1] + 0.5 * (CurLayer[2] - CurLayer[1]);
////        NextLayer[98] = CurLayer[98] - 0.5 * (CurLayer[98] - CurLayer[97]);
////        NextLayer[99] = CurLayer[99] - (1.0 / M_PI) * (CurLayer[99] - CurLayer[98]);
//
//        for (int i = 0; i < 100; i++) {
//            if (i < 99) {
//                outputFile2 << CurLayer[i] << ",";
//            } else {
//                outputFile2 << CurLayer[i];
//            }
//        }
//
//        std::swap(NextLayer, CurLayer);
//
//        outputFile2 << '\n';
//    }
//
//    outputFile2.close();
//
//    std::ofstream outputFile;
//
//    outputFile.open("Test.csv");
//
//    for (int i = 0; i < 100; i++) {
//        for (int j = 0; j < 100; j++) {
//            if (j < 99) {
//                outputFile << sin(-M_PI + j * M_PI / 50.0 + i / 50.0) + 2.0 << ",";
//            } else {
//                outputFile << sin(-M_PI + j * M_PI / 50.0 + i / 50.0) + 2.0;
//            }
//        }
//
//        outputFile << '\n';
//    }
//
//    outputFile.close();

//    dVectorND_variadic <double, 5> Source(0.1, 0.2, 0.3, 0.4, 0.5);

    dVectorND_variadic <double, 5> Temp1(0.1, 0.2, 0.3, 0.4, 0.5);
    Vector <double, 5> Temp2({0.1, 0.2, 0.3, 0.4, 0.5});
    dVectorND_variadic <double, 5> Add1(0.1, 0.1, 0.1, 0.1, 0.1);
    Vector <double, 5> Add2({0.1, 0.1, 0.1, 0.1, 0.1});

//    std::cout << "Result:" << Temp1 << std::endl;

//    std::cout << Temp1 * Add1 << std::endl;

    auto Start = std::chrono::system_clock::now();

//    for (int i = 0; i < 150000000; i++) {
//        Temp1 * Add1;
//    }

    auto Stop = std::chrono::system_clock::now();
//
    std::chrono::duration <double> Time = Stop - Start;
//    std::cout << "dVectorND_variadic multiplication *: " << Time.count() << "s" << std::endl;
//
//    std::cout << "Result:" << Temp1 << std::endl;

    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        Temp1 == Add1;
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "Vector comparison: " << Time.count() << "s" << std::endl;

    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        Temp2 == Add2;
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "dVectorND_variadic comparison: " << Time.count() << "s" << std::endl;

    //----------//

    return 0;
}