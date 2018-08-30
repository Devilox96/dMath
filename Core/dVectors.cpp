#include "dVectors.h"
//----------------------------------//
dVector3D::dVector3D(double xP, double yP, double zP) {
	x = xP;
	y = yP;
	z = zP;
}
dVector3D::dVector3D() {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}
//----------------------------------//
void dVector3D::operator+=(const dVector3D& VectP) {
	this -> x += VectP.x;
	this -> y += VectP.y;
	this -> z += VectP.z;
}
void dVector3D::operator-=(const dVector3D& VectP) {
	this -> x -= VectP.x;
	this -> y -= VectP.y;
	this -> z -= VectP.z;
}
//dVector3D& dVector3D::operator=(const dVector3D& VectP) {
//    this -> x = VectP.x;
//    this -> y = VectP.y;
//    this -> z = VectP.z;
//
//	return *this;
//}
bool dVector3D::operator==(const dVector3D& VectP) {
	return std::tie(x, y, z) == std::tie(VectP.x, VectP.y, VectP.z);
}
bool dVector3D::operator!=(const dVector3D& VectP) {
	return !(std::tie(x, y, z) == std::tie(VectP.x, VectP.y, VectP.z));
}
//----------------------------------//
dVector3D dVector3D::Norm() {
	dVector3D TempL;
	double AbsL;
	
	AbsL = sqrt(pow(this -> x, 2) + pow(this -> y, 2) + pow(this -> z, 2));
	
	TempL.x = (this -> x) / AbsL;
	TempL.y = (this -> y) / AbsL;
	TempL.z = (this -> z) / AbsL;
	
	return TempL;
}
double dVector3D::Greatest() {
    if (this -> x >= this -> y  &&
        this -> x >= this -> z) {
        return this -> x;
    } else if (this -> y >= this -> x  &&
               this -> y >= this -> z) {
        return this -> y;
    } else if (this -> z >= this -> x  &&
               this -> z >= this -> y) {
        return this -> z;
    }
}
double dVector3D::Least() {
    if (this -> x <= this -> y  &&
        this -> x <= this -> z) {
        return this -> x;
    } else if (this -> y <= this -> x  &&
               this -> y <= this -> z) {
        return this -> y;
    } else if (this -> z <= this -> x  &&
               this -> z <= this -> y) {
        return this -> z;
    }
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
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
	this -> x += VectP.x;
	this -> y += VectP.y;
}
void dVector2D::operator-=(const dVector2D& VectP) {
	this -> x -= VectP.x;
	this -> y -= VectP.y;
}
dVector2D& dVector2D::operator=(const dVector2D& VectP) {
	this -> x = VectP.x;
	this -> y = VectP.y;
	
	return *this;
}
bool dVector2D::operator==(const dVector2D& VectP) {
	if (this -> x == VectP.x &&
		this -> y == VectP.y) {
		return true;
	} else {
		return false;
	}
}
bool dVector2D::operator!=(const dVector2D& VectP) {
	if (this -> x == VectP.x &&
		this -> y == VectP.y) {
		return false;
	} else {
		return true;
	}
}
//----------------------------------//
dVector2D dVector2D::Norm() {
	dVector2D TempL;
	double AbsL;
	
	AbsL = sqrt(pow(this -> x, 2) + pow(this -> y, 2));
	
	TempL.x = (this -> x) / AbsL;
	TempL.y = (this -> y) / AbsL;
	
	return TempL;
}
double dVector2D::Greatest() {
    if (this -> x >= this -> y) {
        return this -> x;
    } else if (this -> y >= this -> x) {
        return this -> y;
    }
}
double dVector2D::Least() {
    if (this -> x <= this -> y) {
        return this -> x;
    } else if (this -> y <= this -> x) {
        return this -> y;
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
	this -> w += QuatP.w;
	this -> x += QuatP.x;
	this -> y += QuatP.y;
	this -> z += QuatP.z;
}
void dQuaternion::operator-=(const dQuaternion& QuatP) {
	this -> w -= QuatP.w;
	this -> x -= QuatP.x;
	this -> y -= QuatP.y;
	this -> z -= QuatP.z;
}
dQuaternion dQuaternion::operator*(const dQuaternion& QuatP) {
	dQuaternion QuatL(0.0, 0.0, 0.0, 0.0);
	
	QuatL.w = 	this -> w * QuatP.w -
				this -> x * QuatP.x -
				this -> y * QuatP.y -
				this -> z * QuatP.z;
	QuatL.x = 	this -> w * QuatP.x +
				this -> x * QuatP.w +
				this -> y * QuatP.z -
				this -> z * QuatP.y;
	QuatL.y = 	this -> w * QuatP.y +
				this -> y * QuatP.w +
				this -> z * QuatP.x -
				this -> x * QuatP.z;
	QuatL.z = 	this -> w * QuatP.z +
				this -> z * QuatP.w +
				this -> x * QuatP.y -
				this -> y * QuatP.x;
	
	return QuatL;
}
void dQuaternion::operator*=(const dQuaternion& QuatP) {
	dQuaternion QuatL(0.0, 0.0, 0.0, 0.0);
	
	QuatL.w = 	this -> w * QuatP.w -
				this -> x * QuatP.x -
				this -> y * QuatP.y -
				this -> z * QuatP.z;
	QuatL.x = 	this -> w * QuatP.x +
				this -> x * QuatP.w +
				this -> y * QuatP.z -
				this -> z * QuatP.y;
	QuatL.y = 	this -> w * QuatP.y +
				this -> y * QuatP.w +
				this -> z * QuatP.x -
				this -> x * QuatP.z;
	QuatL.z = 	this -> w * QuatP.z +
				this -> z * QuatP.w +
				this -> x * QuatP.y -
				this -> y * QuatP.x;
	
	this -> w = QuatL.w;
	this -> x = QuatL.x;
	this -> y = QuatL.y;
	this -> z = QuatL.z;
}
dQuaternion& dQuaternion::operator=(const dQuaternion& QuatP) {
	this -> w = QuatP.w;
	this -> x = QuatP.x;
	this -> y = QuatP.y;
	this -> z = QuatP.z;
}
bool dQuaternion::operator==(const dQuaternion& QuatP) {
	if (this -> w == QuatP.w &&
		this -> x == QuatP.x &&
		this -> y == QuatP.y &&
		this -> z == QuatP.z) {
		return true;
	} else {
		return false;
	}
}
bool dQuaternion::operator!=(const dQuaternion& QuatP) {
	if (this -> w == QuatP.w &&
		this -> x == QuatP.x &&
		this -> y == QuatP.y &&
		this -> z == QuatP.z) {
		return false;
	} else {
		return true;
	}
}
//----------------------------------//
void dQuaternion::Reciprocal() {
	dQuaternion QuatL(0.0, 0.0, 0.0, 0.0);
	int NormL;
	
	QuatL.w = this -> w;
	QuatL.x = -(this -> x);
	QuatL.y = -(this -> y);
	QuatL.z = -(this -> z);
	
	NormL = pow(this -> w, 2) + pow(this -> x, 2) +
			pow(this -> y, 2) + pow(this -> z, 2);
			
	QuatL.w /= NormL;
	QuatL.x /= NormL;
	QuatL.y /= NormL;
	QuatL.z /= NormL;
	
	*this = QuatL;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
dVector3D dVecCrossProd(const dVector3D& VectOneP, const dVector3D& VectTwoP) {
	dVector3D VectL(0.0, 0.0, 0.0);

	VectL.x = 	VectOneP.y * VectTwoP.z -
				VectOneP.z * VectTwoP.y;
	VectL.y = 	VectOneP.z * VectTwoP.x -
				VectOneP.x * VectTwoP.z;
	VectL.z = 	VectOneP.x * VectTwoP.y -
				VectOneP.y * VectTwoP.x;

	return VectL;
}
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
    if (this -> Num != VectP.Num) {
        return;
    }

    for (unsigned long i = 0; i < Num; i++) {
        this -> Vec[i] += VectP.Vec[i];
    }
}
void dVectorND::operator-=(const dVectorND& VectP) {
    if (this -> Num != VectP.Num) {
        return;
    }

    for (unsigned long i = 0; i < Num; i++) {
        this -> Vec[i] -= VectP.Vec[i];
    }
}
dVectorND& dVectorND::operator=(const dVectorND& VectP) {
    if (this -> Num != VectP.Num) {
        return *this;
    }

    for (unsigned long i = 0; i < Num; i++) {
        this -> Vec[i] = VectP.Vec[i];
    }

    return *this;
}
bool dVectorND::operator==(const dVectorND& VectP) {
    if (this -> Num != VectP.Num) {
        return false;
    }

    for (unsigned long i = 0; i < Num; i++) {
        if (this -> Vec[i] != VectP.Vec[i]) {
            return false;
        }
    }

    return true;
}
bool dVectorND::operator!=(const dVectorND& VectP) {
    if (this -> Num != VectP.Num) {
        return true;
    }

    for (unsigned long i = 0; i < Num; i++) {
        if (this -> Vec[i] != VectP.Vec[i]) {
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
        SumL += pow(this -> Vec[i], 2);
    }

    double AbsL = sqrt(SumL);

    for (unsigned long i = 0; i < Num; i++) {
        VecL.Vec[i] = this -> Vec[i] / AbsL;
    }

    return VecL;
}
double dVectorND::Greatest() {
    double GreatestL = this -> Vec[0];

    for (unsigned long i = 1; i < Num; i++) {
        if (this -> Vec[i] > GreatestL) {
            GreatestL = this -> Vec[i];
        }
    }

    return GreatestL;
}
double dVectorND::Least() {
    double LeastL = this -> Vec[0];

    for (unsigned long i = 1; i < Num; i++) {
        if (this -> Vec[i] < LeastL) {
            LeastL = this -> Vec[i];
        }
    }

    return LeastL;
}