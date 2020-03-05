#include <iostream>
#include <cmath>
#include <utility>
#include <ctime>

#include "Core/dVector.h"
#include "Core/dMatrix.h"

#include "NumerCalc/dRungeKutta.h"

template<typename F, typename... Args>
void test(F func, Args&&... args){
    std::clock_t start = std::clock();
    for (int i = 0; i < 150'000'000; ++i) {
        func(std::forward<Args>(args)...);
    }
    std::clock_t end = std::clock();

    std::cout << double(end - start) / CLOCKS_PER_SEC << " s\n";
}

int main() {
    return 0;
}