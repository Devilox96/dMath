#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <cstring>
#include <utility>
#include "Core/dVectors.h"

int main() {
    dVectorND <int> Test({1, 2, 3, 4, 5});
    dVectorND <int> Test2 = Test;

    Test2 *= 2;
    std::cout << 10 * dVectorND <int> ({1, 2, 3, 4, 5}) * dVectorND <int> ({1, 2, 3, 4, 5}) * 10;

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