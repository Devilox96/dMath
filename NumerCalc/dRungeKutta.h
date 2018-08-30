#ifndef DRUNGEKUTTA_H
#define DRUNGEKUTTA_H
//-----------------------------//
#include <iostream>
#include <vector>
//-----------------------------//
#include "dVectors.h" //---dMath/Core/dVectors.h---//
//-----------------------------//
struct dRungeKuttaParam {
    double Step = 0.1;

    double Arg = 0.0;
    dVector3D ZeroDerInit;
    dVector3D FirstDerInit;

    virtual dVector3D Func(double ArgP, dVector3D ZeroDerInitP) = 0;
    virtual dVector3D Func(double ArgP, dVector3D ZeroDerInitP, dVector3D FirstDerInitP) = 0;
};
//-----------------------------//
void dRungeKutta4th_1(dRungeKuttaParam& ParamP);
void dRungeKutta4th_2(dRungeKuttaParam& ParamP);
//-----------------------------//
#endif
