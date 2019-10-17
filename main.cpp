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


    //----------//
    std::cout << "Creating 150.000.000 3D vectors..." << std::endl;
    //----------//

    auto Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        dVector3D <double>(0.1, 0.2, 0.3);
    }

    auto Stop = std::chrono::system_clock::now();

    std::chrono::duration <double> Time = Stop - Start;
    std::cout << "dVector3D: " << Time.count() << "s" << std::endl;

    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        Vector3d <double>({0.1, 0.2, 0.3});
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "Vector3d: " << Time.count() << "s" << std::endl;

    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        dVectorND_variadic_array <double, 3>(0.1, 0.2, 0.3);
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "dVectorND_variadic_array: " << Time.count() << "s" << std::endl;

    std::cout << "------------------------------------------" << std::endl;

    //----------//
    std::cout << "Creating 150.000.000 5D vectors..." << std::endl;
    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        dVectorND <double>({0.1, 0.2, 0.3, 0.4, 0.5});
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "dVectorND: " << Time.count() << "s" << std::endl;

    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        Vector <double, 5>({0.1, 0.2, 0.3, 0.4, 0.5});
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "Vector: " << Time.count() << "s" << std::endl;

    //----------//

    std::cout << "------------------------------------------" << std::endl;

    //----------//
    std::cout << "Creating 150.000.000 7D vectors..." << std::endl;
    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        dVectorND <double>({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7});
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "dVectorND: " << Time.count() << "s" << std::endl;

    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        Vector <double, 7>({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7});
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "Vector: " << Time.count() << "s" << std::endl;

    //----------//

    std::cout << "------------------------------------------" << std::endl;

    //----------//
    std::cout << "Creating 150.000.000 9D vectors..." << std::endl;
    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        dVectorND <double>({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9});
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "dVectorND: " << Time.count() << "s" << std::endl;

    //----------//

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        Vector <double, 9>({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9});
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "Vector: " << Time.count() << "s" << std::endl;

    //----------//


    std::cout << "------------------------------------------" << std::endl;

    //----------//
    std::cout << "Calculating 300.000.000 dot products (7D)..." << std::endl;
    //----------//

    auto dTemp1 = dVectorND <double>({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7});
    auto dTemp2 = dVectorND <double>({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7});

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 300000000; i++) {
        dTemp1 * dTemp2;
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "dVectorND: " << Time.count() << "s" << std::endl;

    //----------//

    auto Temp1 = Vector <double, 7>({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7});
    auto Temp2 = Vector <double, 7>({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7});

    Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
        dot(Temp1, Temp2);
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "Vector: " << Time.count() << "s" << std::endl;

    //----------//

    std::cout << "------------------------------------------" << std::endl;

    //----------//

    return 0;
}