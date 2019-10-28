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

    std::cout <<  double(end - start) / CLOCKS_PER_SEC << " s\n";
}


int main() {

//    dVector4D <double> d1;
    dVector <double, 9> d2;
    dVector <double, 9> d3;

    d2.fill(10);
    d3.fill(-3);
    //    test(d2.null);
//    std::cout << d2.norm_2() << std::endl;
//    std::cout << d2.norm() << std::endl;
//    test(std::bind(&dVector <double, 9>::operator+=, d2, d3));
//    test([&d2, &d3](){d2+=d3;});
    test([&d2](){d2.norm();});
    test([&d2](){d2.norm_2();});

    return EXIT_SUCCESS;
}