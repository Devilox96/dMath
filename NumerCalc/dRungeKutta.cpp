#include "dRungeKutta.h"
//-----------------------------//
void dRungeKutta4th_1(dRungeKuttaParam& ParamP) {
    dVector3D <double> k1L;
    dVector3D <double> k2L;
    dVector3D <double> k3L;
    dVector3D <double> k4L;

    dVector3D <double> ZeroDerResL;

    k1L = ParamP.Func(ParamP.Arg, ParamP.ZeroDerInit);
    k2L = ParamP.Func(ParamP.Arg + ParamP.Step / 2, ParamP.ZeroDerInit + k1L * (ParamP.Step / 2));
    k3L = ParamP.Func(ParamP.Arg + ParamP.Step / 2, ParamP.ZeroDerInit + k2L * (ParamP.Step / 2));
    k4L = ParamP.Func(ParamP.Arg + ParamP.Step, ParamP.ZeroDerInit + k3L * ParamP.Step);

    ZeroDerResL = ParamP.ZeroDerInit + (k1L + k2L * 2 + k3L * 2 + k4L) * (ParamP.Step / 6);

    ParamP.Arg += ParamP.Step;
    ParamP.ZeroDerInit = ZeroDerResL;
}
void dRungeKutta4th_2(dRungeKuttaParam& ParamP) {
    dVector3D <double> k1L;
    dVector3D <double> k2L;
    dVector3D <double> k3L;
    dVector3D <double> k4L;

    dVector3D <double> l1L;
    dVector3D <double> l2L;
    dVector3D <double> l3L;
    dVector3D <double> l4L;

    dVector3D <double> ZeroDerResL;
    dVector3D <double> FirstDerResL;

    k1L = ParamP.FirstDerInit;
    l1L = ParamP.Func(ParamP.Arg, ParamP.ZeroDerInit, ParamP.FirstDerInit);

    k2L = ParamP.FirstDerInit + (l1L * (ParamP.Step / 2));
    l2L = ParamP.Func(ParamP.Arg + ParamP.Step / 2, ParamP.ZeroDerInit + k1L * (ParamP.Step / 2), ParamP.FirstDerInit + l1L * (ParamP.Step / 2));

    k3L = ParamP.FirstDerInit + l2L * (ParamP.Step / 2);
    l3L = ParamP.Func(ParamP.Arg + ParamP.Step / 2, ParamP.ZeroDerInit + k2L * (ParamP.Step / 2), ParamP.FirstDerInit + l2L * (ParamP.Step / 2));

    k4L = ParamP.FirstDerInit + l3L * ParamP.Step;
    l4L = ParamP.Func(ParamP.Arg + ParamP.Step, ParamP.ZeroDerInit + k3L * ParamP.Step, ParamP.FirstDerInit + l3L * ParamP.Step);

    ZeroDerResL = ParamP.ZeroDerInit + (k1L + k2L * 2 + k3L * 2 + k4L) * (ParamP.Step / 6);
    FirstDerResL = ParamP.FirstDerInit + (l1L + l2L * 2 + l3L * 2 + l4L) * (ParamP.Step / 6);

    ParamP.Arg += ParamP.Step;
    ParamP.ZeroDerInit = ZeroDerResL;
    ParamP.FirstDerInit = FirstDerResL;
}