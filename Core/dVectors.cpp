#include "dVectors.h"
//----------------------------------//
dVector2D::dVector2D(double xP, double yP) {
	x = xP;
	y = yP;
}
dVector2D::dVector2D() {
	x = 0.0;
	y = 0.0;
}
//----------------------------------//
void dVector2D::operator+=(const dVector2D& VectP) {
	x += VectP.x;
	y += VectP.y;
}
void dVector2D::operator-=(const dVector2D& VectP) {
	x -= VectP.x;
	y -= VectP.y;
}
bool dVector2D::operator==(const dVector2D& VectP) {
	return std::tie(x, y) == std::tie(VectP.x, VectP.y);
}
bool dVector2D::operator!=(const dVector2D& VectP) {
    return !(std::tie(x, y) == std::tie(VectP.x, VectP.y));
}
//----------------------------------//
dVector2D dVector2D::Norm() {
	dVector2D TempL;
	double AbsL;
	
	AbsL = sqrt(pow(x, 2) + pow(y, 2));
	
	TempL.x = x / AbsL;
	TempL.y = y / AbsL;
	
	return TempL;
}
double dVector2D::Greatest() {
    if (x >= y) {
        return x;
    } else {
        return y;
    }
}
double dVector2D::Least() {
    if (x <= y) {
        return x;
    } else {
        return y;
    }
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
dQuaternion::dQuaternion() {
	w = 0.0;
	x = 0.0;
	y = 0.0;
	z = 0.0;
}
dQuaternion::dQuaternion(double wP, double xP, double yP, double zP) {
	w = wP;
	x = xP;
	y = yP;
	z = zP;
}
//----------------------------------//
void dQuaternion::operator+=(const dQuaternion& QuatP) {
	w += QuatP.w;
	x += QuatP.x;
	y += QuatP.y;
	z += QuatP.z;
}
void dQuaternion::operator-=(const dQuaternion& QuatP) {
	w -= QuatP.w;
	x -= QuatP.x;
	y -= QuatP.y;
	z -= QuatP.z;
}
dQuaternion dQuaternion::operator*(const dQuaternion& QuatP) {
	dQuaternion QuatL(0.0, 0.0, 0.0, 0.0);
	
	QuatL.w = w * QuatP.w - x * QuatP.x - y * QuatP.y - z * QuatP.z;
	QuatL.x = w * QuatP.x + x * QuatP.w + y * QuatP.z - z * QuatP.y;
	QuatL.y = w * QuatP.y + y * QuatP.w + z * QuatP.x - x * QuatP.z;
	QuatL.z = w * QuatP.z + z * QuatP.w + x * QuatP.y - y * QuatP.x;
	
	return QuatL;
}
void dQuaternion::operator*=(const dQuaternion& QuatP) {
	dQuaternion QuatL(0.0, 0.0, 0.0, 0.0);
	
	QuatL.w = w * QuatP.w - x * QuatP.x - y * QuatP.y - z * QuatP.z;
	QuatL.x = w * QuatP.x + x * QuatP.w + y * QuatP.z - z * QuatP.y;
	QuatL.y = w * QuatP.y + y * QuatP.w + z * QuatP.x - x * QuatP.z;
	QuatL.z = w * QuatP.z + z * QuatP.w + x * QuatP.y - y * QuatP.x;
	
	w = QuatL.w;
	x = QuatL.x;
	y = QuatL.y;
	z = QuatL.z;
}
bool dQuaternion::operator==(const dQuaternion& QuatP) {
	return std::tie(w, x, y, z) == std::tie(QuatP.w, QuatP.x, QuatP.y, QuatP.z);
}
bool dQuaternion::operator!=(const dQuaternion& QuatP) {
    return !(std::tie(w, x, y, z) == std::tie(QuatP.w, QuatP.x, QuatP.y, QuatP.z));
}
//----------------------------------//
void dQuaternion::Reciprocal() {
	dQuaternion QuatL(0.0, 0.0, 0.0, 0.0);
	double NormL;
	
	QuatL.w = w;
	QuatL.x = -x;
	QuatL.y = -y;
	QuatL.z = -z;
	
	NormL = pow(w, 2) + pow(x, 2) + pow(y, 2) + pow(z, 2);
			
	QuatL.w /= NormL;
	QuatL.x /= NormL;
	QuatL.y /= NormL;
	QuatL.z /= NormL;
	
	*this = QuatL;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
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