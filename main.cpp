#include <iostream>
#include <cmath>
#include <utility>
#include "Core/dVector.h"

#include <functional>
#include <ctime>
#include <chrono>
#include <QVector3D>

#include "NumerCalc/dRungeKutta.h"

template<typename F, typename... Args>
void test(F func, Args&&... args){
    std::clock_t start = std::clock();
    for (int i = 0; i < 150'000'000; ++i) {
        func(std::forward<Args>(args)...);
    }
    std::clock_t end = std::clock();

    std::cout <<  double(end - start) / CLOCKS_PER_SEC << " s\n";
}

double Func(double tTime, const double& tArg) {
    return (tArg + tTime) * sin(tTime);
}

double Func2(const double& tArg) {
    return tArg * sin(tArg);
}

int main() {
    auto Start = std::chrono::system_clock::now();

    //----------//

    double Arg = 1;
    double TimeArg = 0;
    double TimeStep = 1.0;

    dRungeKutta1D <double> Solver(1.0);

    std::cout << Arg << std::endl;

    for (int i = 0; i < 10; i++) {
        Arg = Solver.solve4th(TimeArg, Arg, Func);
//        Arg = Solver.solve4th(Arg, Func2);
        TimeArg += TimeStep;
        std::cout << TimeArg << ": " << Arg << std::endl;
    }

    //----------//

    auto Stop = std::chrono::system_clock::now();

    std::chrono::duration <double> Time = Stop - Start;
//    std::cout << "Function: " << Time.count() << "s" << std::endl;
//
//    //----------//
//
//    Arg = 1;
//    TimeArg = 0;
//    TimeStep = 1.0;
//
//    Start = std::chrono::system_clock::now();
//
//    //----------//
//
//
//
//    //----------//
//
//    Stop = std::chrono::system_clock::now();
//
//    Time = Stop - Start;
//    std::cout << "Pointer: " << Time.count() << "s" << std::endl;

    //----------//

    return EXIT_SUCCESS;
}