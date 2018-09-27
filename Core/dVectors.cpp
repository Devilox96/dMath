#include "dVectors.h"
//----------------------------------//
template <typename T, typename VecT> dVector3D <VecT> operator*(const dVector3D <VecT>& VecP, const T NumP) {
    return dVector3D <VecT>(VecP.x * NumP, VecP.y * NumP, VecP.z * NumP);
}
template <typename T, typename VecT> dVector3D <VecT> operator/(const dVector3D <VecT>& VecP, const T NumP) {
    return dVector3D <VecT>(VecP.x / NumP, VecP.y / NumP, VecP.z / NumP);
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
    return dVector2D <VecT>(VecP.x * NumP, VecP.y * NumP);
}
template <typename T, typename VecT> dVector2D <VecT> operator/(const dVector2D <VecT>& VecP, const T NumP) {
    return dVector2D <VecT>(VecP.x / NumP, VecP.y / NumP);
}
//----------------------------------//
template <typename T, typename QuatT> dQuaternion <QuatT> operator*(const dQuaternion <QuatT>& QuatP, const T NumP) {
    return dQuaternion <QuatT>(QuatP.w * NumP, QuatP.x * NumP, QuatP.y * NumP, QuatP.z * NumP);
}
template <typename T, typename QuatT> dQuaternion <QuatT> operator/(const dQuaternion <QuatT>& QuatP, const T NumP) {
    return dQuaternion <QuatT>(QuatP.w / NumP, QuatP.x / NumP, QuatP.y / NumP, QuatP.z / NumP);
}
//----------------------------------//
template <typename T, typename VecT> dVectorND <VecT> operator*(const dVectorND <VecT>& VectP, const T NumP) {
    dVectorND <VecT> TempVecL(VectP.Size);

    for (unsigned long i = 0; i < VectP.Size; i++) {
        TempVecL.Vec[i] = VectP.Vec[i] * NumP;
    }

    return dVectorND <VecT>(TempVecL);
}
template <typename T, typename VecT> dVectorND <VecT> operator/(const dVectorND <VecT>& VectP, const T NumP) {
    dVectorND <VecT> TempVecL(VectP.Size);

    for (unsigned long i = 0; i < VectP.Size; i++) {
        TempVecL.Vec[i] = VectP.Vec[i] / NumP;
    }

    return dVectorND <VecT>(TempVecL);
}