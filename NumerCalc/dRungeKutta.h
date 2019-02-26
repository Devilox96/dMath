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
    dVector2D <double> ZeroDerInit;
    dVector2D <double> FirstDerInit;

    virtual dVector2D <double> Func(double ArgP, dVector2D <double> ZeroDerInitP) = 0;
    virtual dVector2D <double> Func(double ArgP, dVector2D <double> ZeroDerInitP, dVector2D <double> FirstDerInitP) = 0;
};
//-----------------------------//
void dRungeKutta4th_1(dRungeKuttaParam& ParamP);
void dRungeKutta4th_2(dRungeKuttaParam& ParamP);
//-----------------------------//
#endif
