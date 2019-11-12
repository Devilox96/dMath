#include <iostream>
#include <cmath>
#include <utility>
#include "Core/dVector.h"

#include <functional>
#include <ctime>
#include <chrono>
#include <QVector3D>

template<typename F, typename... Args>
void test(F func, Args&&... args){
    std::clock_t start = std::clock();
    for (int i = 0; i < 150'000'000; ++i) {
        func(std::forward<Args>(args)...);
    }
    std::clock_t end = std::clock();

    std::cout <<  double(end - start) / CLOCKS_PER_SEC << " s\n";
}


int main() {
//    dVectorND <double, 5> Temp1(0.1, 0.2, 0.3, 0.4, 0.5);
//    Vector <double, 5> Temp2({0.1, 0.2, 0.3, 0.4, 0.5});
//    dVectorND <double, 5> Add1(0.1, 0.1, 0.1, 0.1, 0.1);
//    Vector <double, 5> Add2({0.1, 0.1, 0.1, 0.1, 0.1});
//
////    std::cout << "Result:" << Temp1 << std::endl;
//
////    std::cout << Temp1 * Add1 << std::endl;
//
    auto Start = std::chrono::system_clock::now();

    for (long int i = 0; i < 100'000'000; i++) {
        (QVector3D (0.1, 0.2, 0.3) + QVector3D (0.1, 0.2, 0.3)) / i;
    }

    auto Stop = std::chrono::system_clock::now();

    std::chrono::duration <double> Time = Stop - Start;
    std::cout << "QVector3D: " << Time.count() << "s" << std::endl;

    //----------//

    Start = std::chrono::system_clock::now();

    for (long int i = 0; i < 100000000; i++) {
        (dVector <float, 3>(0.1, 0.2, 0.3) + dVector <float, 3>(0.1, 0.2, 0.3)) / i;
    }

    Stop = std::chrono::system_clock::now();

    Time = Stop - Start;
    std::cout << "dVector: " << Time.count() << "s" << std::endl;

    //----------//
//
//    Start = std::chrono::system_clock::now();
//
//    for (int i = 0; i < 150000000; i++) {
//        Temp2 == Add2;
//    }
//
//    Stop = std::chrono::system_clock::now();
//
//    Time = Stop - Start;
//    std::cout << "dVectorND comparison: " << Time.count() << "s" << std::endl;

    return EXIT_SUCCESS;
}