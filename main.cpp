#include <iostream>
#include <cmath>
#include <utility>
#include "Core/dVector.h"
#include "Core/dMatrix.h"

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
    dSMatrix<double, 3> m({1, 2, 3, 4, 5, 6, 7, 8, 9});
    for(auto row = m.begin<Iterate::Row>(); row != m.end<Iterate::Row>(); ++row) {
        for(auto i = row->begin(); i != row->end(); ++i) {
            std::cout << *i << "(" << std::distance(m.begin(), i) << ") ";
        }
        std::cout << std::endl;
    }

    std::cout << *m.begin<Iterate::Col>()->begin() << std::endl;

    for(auto col = m.begin<Iterate::Col>(); col != m.end<Iterate::Col>(); ++col) {
        for(auto i = col->begin(); i != col->end(); ++i) {
            std::cout << *i << "(" << std::distance(m.begin(), static_cast<double*>(i)) << ") ";
        }
        std::cout << std::endl;
    }

//    test([](){
//        dVector4D<double> d1(5);
//        dVector4D<double> d2(5);
//        auto d = d1 * d2;
//    });


    return EXIT_SUCCESS;
}
