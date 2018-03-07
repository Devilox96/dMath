#ifndef DRUNGEKUTTA_H
#define DRUNGEKUTTA_H
//-----------------------------//
#include <iostream>
#include <vector>
//-----------------------------//
#include "dVectors.h"
//-----------------------------//
struct dRungeKuttaParam {
    double Step = 0.1;

    double Arg = 0.0;
    dVector3D ZeroDerInit;
    dVector3D FirstDerInit;

    virtual dVector3D Func(double ArgP, dVector3D ZeroDerInitP) {
        std :: cout << "dRungeKuttaParam: No function!" << std :: endl;

        return dVector3D();
    }
    virtual dVector3D Func(double ArgP, dVector3D ZeroDerInitP, dVector3D FirstDerInitP) {
        std :: cout << "dRungeKuttaParam: No function!" << std :: endl;

        return dVector3D();
    }
};
//-----------------------------//
void dRungeKutta4th_1(dRungeKuttaParam& ParamP);
void dRungeKutta4th_2(dRungeKuttaParam& ParamP);
//-----------------------------//
#endif
