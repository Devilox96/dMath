#ifndef DVECTORND_H
#define DVECTORND_H
//-----------------------------//
#include <iostream>
#include <utility>
#include <initializer_list>
#include <cmath>
#include <tuple>
//-----------------------------//
template <typename T>
class dVectorND {
public:
    explicit dVectorND <T>(unsigned long SizeP) : Data(new T[SizeP]), Size(SizeP) {}
    dVectorND <T>(const std::initializer_list <T>& ArgsP) : Data(new T[ArgsP.size()]), Size(ArgsP.size()) {
        std::copy(ArgsP.begin(), ArgsP.end(), Data);
    }
    dVectorND <T>(const dVectorND <T>& CopyVectorP) : Data(new T[CopyVectorP.Size]), Size(CopyVectorP.Size) {
        std::copy(CopyVectorP.Data, CopyVectorP.Data + Size, Data);
    }
    dVectorND <T>(dVectorND <T>&& MoveVectorP) noexcept : Data(std::exchange(MoveVectorP.Data, nullptr)), Size(MoveVectorP.Size) {}
    ~dVectorND <T>() {
        delete[] Data;
    }

    //----------//

    dVectorND <T>& operator=(const dVectorND <T>& CopyVectorP) {
        *this = dVectorND <T>(CopyVectorP);
        return *this;
    }
    dVectorND <T>& operator=(dVectorND <T>&& MoveVectorP) noexcept {
        std::swap(Data, MoveVectorP.Data);
        return *this;
    }

    //----------//

    void operator+=(const dVectorND <T>& VectorP) {
        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;

            return;
        }

        for (unsigned long i = 0; i < Size; i++) {
            Data[i] += VectorP.Data[i];
        }
    }
    dVectorND <T> operator+(const dVectorND <T>& VectorP) const {
        dVectorND <T> TempVecL(Size);

        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
        } else {
            for (unsigned long i = 0; i < Size; i++) {
                TempVecL.Data[i] = Data[i] + VectorP.Data[i];
            }
        }

        return TempVecL;
    }

    //----------//

    dVectorND <T> operator-() const {
        dVectorND <T> TempVecL(Size);

        for (unsigned long i = 0; i < Size; i++) {
            TempVecL.Data[i] = -Data[i];
        }

        return TempVecL;
    }
    void operator-=(const dVectorND <T>& VectorP) {
        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;

            return;
        }

        for (unsigned long i = 0; i < Size; i++) {
            Data[i] -= VectorP.Data[i];
        }
    }
    dVectorND <T> operator-(const dVectorND <T>& VectorP) const {
        dVectorND <T> TempVecL(Size);

        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
        } else {
            for (unsigned long i = 0; i < Size; i++) {
                TempVecL.Data[i] = Data[i] - VectorP.Data[i];
            }
        }

        return TempVecL;
    }

    //----------//

    T operator*(const dVectorND <T>& VectorP) const {
        T ResultL = 0;

        if (Size != VectorP.Size) {
            std::cout << "Error (dVectorND): incompatible vectors sizes!" << std::endl;
        } else {
            for (unsigned long i = 0; i < Size; i++) {
                ResultL += Data[i] * VectorP.Data[i];
            }
        }

        return ResultL;
    }
    template <typename NumberT> void operator*=(const NumberT NumberP) {
        for (unsigned long i = 0; i < Size; i++) {
            Data[i] *= NumberP;
        }
    }
    template <typename NumberT> friend dVectorND <T> operator*(const dVectorND <T>& VectorP, NumberT NumberP) {
        dVectorND <T> TempVecL(VectorP.Size);

        for (unsigned long i = 0; i < VectorP.Size; i++) {
            TempVecL.Data[i] = VectorP.Data[i] * NumberP;
        }

        return TempVecL;
    }
    template <typename NumberT> friend dVectorND <T> operator*(NumberT NumberP, const dVectorND <T>& VectorP) {
        dVectorND <T> TempVecL(VectorP.Size);

        for (unsigned long i = 0; i < VectorP.Size; i++) {
            TempVecL.Data[i] = VectorP.Data[i] * NumberP;
        }

        return TempVecL;
    }

    //----------//

    template <typename NumberT> void operator/=(const NumberT NumberP) {
        for (unsigned long i = 0; i < Size; i++) {
            Data[i] /= NumberP;
        }
    }
    template <typename NumberT> friend dVectorND <T> operator/(const dVectorND <T>& VectorP, NumberT NumberP) {
        dVectorND <T> TempVecL(VectorP.Size);

        for (unsigned long i = 0; i < VectorP.Size; i++) {
            TempVecL.Data[i] = VectorP.Data[i] / NumberP;
        }

        return TempVecL;
    }

    //----------//

    bool operator==(const dVectorND <T>& VectorP) const {
        if (Size != VectorP.Size) {
            return false;
        }

        return std::equal(std::begin(Data), std::end(Data)), std::begin(VectorP.Data);
    }
    bool operator!=(const dVectorND <T>& VectorP) const {
        if (Size != VectorP.Size) {
            return true;
        }

        return !std::equal(std::begin(Data), std::end(Data)), std::begin(VectorP.Data);
    }

    //----------//

    T& operator[](unsigned long NumP) {
        if (NumP < Size && NumP >= 0) {
            return Data[NumP];
        } else {
            std::cout << "Error (dVectorND): out of range!" << std::endl;
            return Data[0];
        }
    }
    T operator[](unsigned long NumP) const {
        if (NumP < Size && NumP >= 0) {
            return Data[NumP];
        } else {
            std::cout << "Error (dVectorND): out of range!" << std::endl;
            return Data[0];
        }
    }

    //----------//

    friend std::ostream& operator<<(std::ostream& StreamP, const dVectorND <T>& VectorP) {
        for (unsigned long i = 0; i < VectorP.Size; i++) {
            StreamP << VectorP.Data[i] << " ";
        }

        return StreamP;
    }

    //----------//

    double Abs() const {
        double ResL = 0.0;

        for (unsigned long i = 0; i < Size; i++) {
            ResL += pow(Data[i], 2);
        }

        return sqrt(ResL);
    }
    float Absf() const {
        float ResL = 0.0;

        for (unsigned long i = 0; i < Size; i++) {
            ResL += powf(Data[i], 2);
        }

        return sqrtf(ResL);
    }
    T Abs2() const {
        T ResL = 0.0;

        for (unsigned long i = 0; i < Size; i++) {
            ResL += Data[i] * Data[i];
        }

        return ResL;
    }

    //----------//

    dVectorND <double> Norm() const {
        double SumL = 0.0;
        dVectorND <double> VecL(Size);

        for (unsigned long i = 0; i < Size; i++) {
            SumL += pow(Data[i], 2);
        }

        double AbsL = sqrt(SumL);

        for (unsigned long i = 0; i < Size; i++) {
            VecL.Data[i] = Data[i] / AbsL;
        }

        return VecL;
    }
    dVectorND <float> Normf() const {
        float SumL = 0.0;
        dVectorND <float> VecL(Size);

        for (unsigned long i = 0; i < Size; i++) {
            SumL += powf(Data[i], 2);
        }

        float AbsL = sqrtf(SumL);

        for (unsigned long i = 0; i < Size; i++) {
            VecL.Data[i] = Data[i] / AbsL;
        }

        return VecL;
    }

    //----------//

    T Greatest() const {
        T GreatestL = Data[0];

        for (unsigned long i = 1; i < Size; i++) {
            if (Data[i] > GreatestL) {
                GreatestL = Data[i];
            }
        }

        return GreatestL;
    }
    T Least() const {
        T LeastL = Data[0];

        for (unsigned long i = 1; i < Size; i++) {
            if (Data[i] < LeastL) {
                LeastL = Data[i];
            }
        }

        return LeastL;
    }

    //----------//

    unsigned long GetSize() const {
        return Size;
    }
private:
    T* Data;
    unsigned long Size;
};
//-----------------------------//
#endif
