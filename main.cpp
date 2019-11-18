#include <iostream>
#include <cmath>
#include <utility>
#include <ctime>

#include "Core/dVector.h"
#include "Core/dMatrix.h"

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

    dVector<double, 9> d0{};
    dMatrix<double, 9, 1> d1{};
    dMatrix<double, 1, 9> d2{};
    test([&]{auto s = d0 + d0;});
    test([&]{auto s = d1 + d1;});
    test([&]{auto s = d2 + d2;});

//    test([]{dVector<double, 9> d{};});
//    test([]{dMatrix<double, 9, 1> d{};});
//    test([]{dMatrix<double, 1, 9> d{};});

    return EXIT_SUCCESS;
}