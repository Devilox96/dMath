//
// Created by devilox on 5/3/20.
//

#ifndef DMATH_TEMPMATRIX_H
#define DMATH_TEMPMATRIX_H

#include <array>

template <typename T, size_t M, size_t N>
class TempMatrix {
public:
    explicit TempMatrix(void (*tInitFunc)(std::array <std::array <T, N>, M>& tData)) {
        tInitFunc(mData);
    }
private:
    std::array <std::array <T, N>, M> mData;
};


#endif //DMATH_TEMPMATRIX_H
