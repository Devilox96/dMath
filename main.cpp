#include <iostream>
#include <cmath>
#include <utility>
#include "Core/dVector.h"

#include <functional>
#include <ctime>

template<typename F, typename... Args>
void test(F func, Args&&... args){
    std::clock_t start = std::clock();
    for (int i = 0; i < 150'000'000; ++i) {
        func(std::forward<Args>(args)...);
    }
    std::clock_t end = std::clock();

    std::cout <<  double(end - start) << " ns\n";
}


int main() {
    test([](){
        dVector4D<double> d1(5);
        dVector4D<double> d2(5);
        auto d = d1 * d2;
    });


    return EXIT_SUCCESS;
}
