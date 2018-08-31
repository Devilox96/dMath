#include <iostream>
#include "Core/dVectors.h"

int main() {
    dVector3D <double> Vec1;
    dVector3D <double> Vec2(1, 3, 5);

    Vec1 = Vec2;

    std::cout << (Vec1 == Vec2) << std::endl;
    std::cout << (Vec1 != Vec2) << std::endl;

    Vec1 += Vec2;

    std::cout << (Vec1 == Vec2) << std::endl;
    std::cout << (Vec1 != Vec2) << std::endl;

    dVector3D <double> Vec3 = (Vec1 + Vec2) * 5.0;

    return 0;
}