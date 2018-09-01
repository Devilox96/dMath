#include "dVectors.h"
//----------------------------------//
template <typename T, typename VecT> dVector3D <VecT> operator*(const dVector3D <VecT>& VecP, const T NumP) {
    return dVector3D <VecT> (VecP.x * NumP, VecP.y * NumP, VecP.z * NumP);
}
template <typename T, typename VecT> dVector3D <VecT> operator/(const dVector3D <VecT>& VecP, const T NumP) {
    return dVector3D <VecT> (VecP.x / NumP, VecP.y / NumP, VecP.z / NumP);
}
//----------------------------------//
template <typename VecT> dVector3D <VecT> dVecCrossProd(const dVector3D <VecT>& VectOneP, const dVector3D <VecT>& VectTwoP) {
    dVector3D <VecT> VectL(0.0, 0.0, 0.0);

    VectL.x = VectOneP.y * VectTwoP.z - VectOneP.z * VectTwoP.y;
    VectL.y = VectOneP.z * VectTwoP.x - VectOneP.x * VectTwoP.z;
    VectL.z = VectOneP.x * VectTwoP.y - VectOneP.y * VectTwoP.x;

    return VectL;
}
//----------------------------------//
template <typename T, typename VecT> dVector2D <VecT> operator*(const dVector2D <VecT>& VecP, const T NumP) {
    return dVector2D <VecT> (VecP.x * NumP, VecP.y * NumP);
}
template <typename T, typename VecT> dVector2D <VecT> operator/(const dVector2D <VecT>& VecP, const T NumP) {
    return dVector2D <VecT> (VecP.x / NumP, VecP.y / NumP);
}
//----------------------------------//
template <typename T, typename QuatT> dQuaternion <QuatT> operator*(const dQuaternion <QuatT>& QuatP, const T NumP) {
    return dQuaternion <QuatT> (QuatP.w * NumP, QuatP.x * NumP, QuatP.y * NumP, QuatP.z * NumP);
}
template <typename T, typename QuatT> dQuaternion <QuatT> operator/(const dQuaternion <QuatT>& QuatP, const T NumP) {
    return dQuaternion <QuatT> (QuatP.w / NumP, QuatP.x / NumP, QuatP.y / NumP, QuatP.z / NumP);
}
//----------------------------------//























dVectorND::dVectorND(std::initializer_list <double> CoordsP) {
    for (double CoordI : CoordsP) {
        Vec.emplace_back(CoordI);
        Num++;
    }
}
dVectorND::dVectorND(unsigned long NumP) {
    for (unsigned long i = 0; i < NumP; i++) {
        Vec.emplace_back(0.0);
    }
    Num = NumP;
}
dVectorND ::~dVectorND() {
    Vec.clear();
}
//----------------------------------//
void dVectorND::Resize(unsigned long NumP) {
	Vec.resize(NumP);
}
//----------------------------------//
void dVectorND::operator+=(const dVectorND& VectP) {
    if (Num != VectP.Num) {
        return;
    }

    for (unsigned long i = 0; i < Num; i++) {
        Vec[i] += VectP.Vec[i];
    }
}
void dVectorND::operator-=(const dVectorND& VectP) {
    if (Num != VectP.Num) {
        return;
    }

    for (unsigned long i = 0; i < Num; i++) {
        Vec[i] -= VectP.Vec[i];
    }
}
dVectorND& dVectorND::operator=(const dVectorND& VectP) {
    if (Num != VectP.Num) {
        return *this;
    }

    for (unsigned long i = 0; i < Num; i++) {
        Vec[i] = VectP.Vec[i];
    }

    return *this;
}
bool dVectorND::operator==(const dVectorND& VectP) {
    if (Num != VectP.Num) {
        return false;
    }

    for (unsigned long i = 0; i < Num; i++) {
        if (Vec[i] != VectP.Vec[i]) {
            return false;
        }
    }

    return true;
}
bool dVectorND::operator!=(const dVectorND& VectP) {
    if (Num != VectP.Num) {
        return true;
    }

    for (unsigned long i = 0; i < Num; i++) {
        if (Vec[i] != VectP.Vec[i]) {
            return true;
        }
    }

    return false;
}
//----------------------------------//
dVectorND dVectorND::Norm() {
    double SumL = 0.0;
    dVectorND VecL(Num);

    for (unsigned long i = 0; i < Num; i++) {
        SumL += pow(Vec[i], 2);
    }

    double AbsL = sqrt(SumL);

    for (unsigned long i = 0; i < Num; i++) {
        VecL.Vec[i] = Vec[i] / AbsL;
    }

    return VecL;
}
double dVectorND::Greatest() {
    double GreatestL = Vec[0];

    for (unsigned long i = 1; i < Num; i++) {
        if (Vec[i] > GreatestL) {
            GreatestL = Vec[i];
        }
    }

    return GreatestL;
}
double dVectorND::Least() {
    double LeastL = Vec[0];

    for (unsigned long i = 1; i < Num; i++) {
        if (Vec[i] < LeastL) {
            LeastL = Vec[i];
        }
    }

    return LeastL;
}