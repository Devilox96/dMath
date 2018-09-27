#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <cstring>
#include "Core/dVectors.h"

//template <typename VecT>
//struct dVectorND {
//private:
//    VecT* Vec;
//    unsigned long Size = 0;
//public:
//    dVectorND <VecT>() = default;
//    explicit dVectorND <VecT>(unsigned long NumP) : Vec(new VecT[NumP]), Size(NumP) {}
//    dVectorND <VecT>(const std::initializer_list <VecT>& CoordsP) : Vec(new VecT[CoordsP.size()]), Size(CoordsP.size()) {}
//    dVectorND <VecT>(const dVectorND <VecT>& CopyVecP) : Vec(new VecT(*CopyVecP.Vec)), Size(CopyVecP.Size) {}
//    ~dVectorND <VecT>() {
//        delete[] Vec;
//    }
//
//    //---Addition---//
//    void operator+=(const dVectorND <VecT>& VectP) {
//        if (Size != VectP.Size) {
//            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
//
//            return;
//        }
//
//        for (unsigned long i = 0; i < Size; i++) {
//            Vec[i] += VectP.Vec[i];
//        }
//    }
//    dVectorND <VecT> operator+(const dVectorND <VecT>& VectP) const {
//        dVectorND <VecT> TempVecL(Size);
//
//        if (Size != VectP.Size) {
//            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
//
//            return TempVecL;
//        }
//
//        for (unsigned long i = 0; i < Size; i++) {
//            TempVecL.Vec[i] = Vec[i] + VectP.Vec[i];
//        }
//
//        return dVectorND <VecT>(TempVecL);
//    }
//    //---Addition---//
//
//    //---Subtraction---//
//    dVectorND <VecT> operator-() {
//        dVectorND <VecT> TempVecL(Size);
//
//        for (unsigned long i = 0; i < Size; i++) {
//            TempVecL.Vec[i] = -Vec[i];
//        }
//
//        return dVectorND <VecT>(TempVecL);
//    }
//    void operator-=(const dVectorND <VecT>& VectP) {
//        if (Size != VectP.Size) {
//            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
//
//            return;
//        }
//
//        for (unsigned long i = 0; i < Size; i++) {
//            Vec[i] -= VectP.Vec[i];
//        }
//    }
//    dVectorND <VecT> operator-(const dVectorND <VecT>& VectP) const {
//        dVectorND <VecT> TempVecL(Size);
//
//        if (Size != VectP.Size) {
//            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
//
//            return TempVecL;
//        }
//
//        for (unsigned long i = 0; i < Size; i++) {
//            TempVecL.Vec[i] = Vec[i] - VectP.Vec[i];
//        }
//
//        return dVectorND <VecT>(TempVecL);
//    }
//    //---Subtraction---//
//
//    //---Multiplying---//
//    double operator*(const dVectorND <VecT>& VectP) {
//        double ResL = 0.0;
//
//        if (Size != VectP.Size) {
//            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
//
//            return ResL;
//        }
//
//        for (unsigned long i = 0; i < Size; i++) {
//            ResL += Vec[i] * VectP.Vec[i];
//        }
//
//        return ResL;
//    }
//    template <typename T> dVectorND <VecT> operator*(const T NumP) {
//        dVectorND <VecT> TempVecL(Size);
//
//        for (unsigned long i = 0; i < Size; i++) {
//            TempVecL.Vec[i] = Vec[i] * NumP;
//        }
//
//        return dVectorND <VecT>(TempVecL);
//    }
//    template <typename T> void operator*=(const T NumP) {
//        for (double& NumI : Vec) {
//            NumI *= NumP;
//        }
//    }
//    template <typename T> friend dVectorND <VecT> operator*(const dVectorND <VecT>& VectP, T NumP);
//    //---Multiplying---//
//
//    //---Division---//
//    template <typename T> dVectorND <VecT> operator/(const T NumP) {
//        dVectorND <VecT> TempVecL(Size);
//
//        for (unsigned long i = 0; i < Size; i++) {
//            TempVecL.Vec[i] = Vec[i] / NumP;
//        }
//
//        return dVectorND <VecT>(TempVecL);
//    }
//    template <typename T> void operator/=(const T NumP) {
//        for (double& NumI : Vec) {
//            NumI /= NumP;
//        }
//    }
//    template <typename T> friend dVectorND <VecT> operator/(const dVectorND <VecT>& VectP, T NumP);
//    //---Division---//
//
//    //---Equality---//
//    dVectorND <VecT>& operator=(const dVectorND <VecT>& VectP) {
//        if (Size != VectP.Size) {
//            return *this;
//        }
//
//        dVectorND <VecT> TempVecL(VectP);
//
//        std::swap(Vec, TempVecL.Vec);
//
//        return *this;
//    }
//    bool operator==(const dVectorND <VecT>& VectP) {
//        if (Size != VectP.Size) {
//            return false;
//        }
//
//        return std::equal(std::begin(Vec), std::end(Vec)), std::begin(VectP.Vec);
//    }
//    bool operator!=(const dVectorND <VecT>& VectP) {
//        if (Size != VectP.Size) {
//            return true;
//        }
//
//        return !std::equal(std::begin(Vec), std::end(Vec)), std::begin(VectP.Vec);
//    }
//    //---Equality---//
//
//    //---Absolute value---//
//    double Abs() {
//        double ResL = 0.0;
//
//        for (const VecT& NumI : Vec) {
//            ResL += pow(NumI, 2);
//        }
//
//        return sqrt(ResL);
//    }
//    double Abs_2() {
//        double ResL = 0.0;
//
//        for (const VecT& NumI : Vec) {
//            ResL += pow(NumI, 2);
//        }
//
//        return ResL;
//    }
//    //---Absolute value---//
//
//    dVectorND <double> Norm() {
//        double SumL = 0.0;
//        dVectorND <double> VecL(Size);
//
//        for (const VecT& NumI : Vec) {
//            SumL += pow(NumI, 2);
//        }
//
//        double AbsL = sqrt(SumL);
//
//        for (unsigned long i = 0; i < Size; i++) {
//            VecL.Vec[i] = Vec[i] / AbsL;
//        }
//
//        return dVectorND <double>(VecL);
//    }
//    VecT Greatest() {
//        VecT GreatestL = Vec[0];
//
//        for (unsigned long i = 1; i < Size; i++) {
//            if (Vec[i] > GreatestL) {
//                GreatestL = Vec[i];
//            }
//        }
//
//        return GreatestL;
//    }
//    VecT Least() {
//        VecT LeastL = Vec[0];
//
//        for (unsigned long i = 1; i < Size; i++) {
//            if (Vec[i] < LeastL) {
//                LeastL = Vec[i];
//            }
//        }
//
//        return LeastL;
//    }
//
//    //---Access---//
//    VecT& operator[](unsigned long NumP) {
//        if (NumP < Size) {
//            return Vec[NumP];
//        } else {
//            return Vec[0];
//        }
//    }
//    VecT operator[](unsigned long NumP) const {
//        if (NumP < Size) {
//            return Vec[NumP];
//        } else {
//            return Vec[0];
//        }
//    }
//    VecT ConstGet(unsigned long NumP) const {
//        return Vec[NumP];
//    }
//    unsigned long GetSize() const {
//        return Size;
//    }
////    //---Access---//
//};

int main() {
    auto Start = std::chrono::system_clock::now();

    for (int i = 0; i < 150000000; i++) {
//        dVectorND <double>({0.1, 0.2, 0.3}) + dVectorND <double>({0.1, 0.2, 0.3});
        std::vector <double> Test = {0.1, 0.2, 0.3};
    }

    auto Stop = std::chrono::system_clock::now();

    std::chrono::duration <double> Time = Stop - Start;
    std::cout << Time.count() << std::endl;

    return 0;
}