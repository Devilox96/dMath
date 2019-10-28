#include "dRungeKutta.h"
//-----------------------------//
void dRungeKutta4th_1(dRungeKuttaParam& ParamP) {
    dVector2D <double> k1L(0.0, 0.0);
    dVector2D <double> k2L(0.0, 0.0);
    dVector2D <double> k3L(0.0, 0.0);
    dVector2D <double> k4L(0.0, 0.0);

    dVector2D <double> ZeroDerResL(0.0, 0.0);

    k1L = ParamP.Func(ParamP.Arg, ParamP.ZeroDerInit);
    k2L = ParamP.Func(ParamP.Arg + ParamP.Step / 2, ParamP.ZeroDerInit + k1L * (ParamP.Step / 2));
    k3L = ParamP.Func(ParamP.Arg + ParamP.Step / 2, ParamP.ZeroDerInit + k2L * (ParamP.Step / 2));
    k4L = ParamP.Func(ParamP.Arg + ParamP.Step, ParamP.ZeroDerInit + k3L * ParamP.Step);

    ZeroDerResL = ParamP.ZeroDerInit + (k1L + k2L * 2 + k3L * 2 + k4L) * (ParamP.Step / 6);

    ParamP.Arg += ParamP.Step;
    ParamP.ZeroDerInit = ZeroDerResL;
}
void dRungeKutta4th_2(dRungeKuttaParam& ParamP) {
    dVector2D <double> k1L(0.0, 0.0);
    dVector2D <double> k2L(0.0, 0.0);
    dVector2D <double> k3L(0.0, 0.0);
    dVector2D <double> k4L(0.0, 0.0);

    dVector2D <double> l1L(0.0, 0.0);
    dVector2D <double> l2L(0.0, 0.0);
    dVector2D <double> l3L(0.0, 0.0);
    dVector2D <double> l4L(0.0, 0.0);

    dVector2D <double> ZeroDerResL(0.0, 0.0);
    dVector2D <double> FirstDerResL(0.0, 0.0);

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