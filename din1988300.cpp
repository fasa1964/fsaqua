#include "din1988300.h"

#include <QtMath>

DIN1988300::DIN1988300()
{

}

// returns the reynolds-value
// di = inside diameter, v = speed flow rate, kinVis = kin. viscosity
double DIN1988300::reynolds(double di, double v, double kinVis)
{
    double re;

    re = v * di / kinVis;

    return re;
}

// returns the pressure loss in pipe per meter
// re = reynold, vm = speed flow rate
double DIN1988300::pressureLossPerMeter(double re, double kWert, double di, double vm, double dichte)
{
    double RM = 0.0;      // pressure loss per meter
    double hydr;          // value for hydraulically
    double la;            // lambda

    // turbulent flow
    if( re >= 2320 )
    {

        // status of hydraulically
        hydr = re * kWert / di;

        // hydraulically smooth
        if(hydr < 65)
        {
            // Gleichung 4 nach DIN 1988-300
            la = lamdaHydraulichGlatt_G4(re);
            RM = rohrreibung(la, di, vm, dichte);
        }

        // hydraulically rough
        if( hydr > 1300)
        {
            // Gleichung 6 nach DIN 1988-300
            la = lamdaHydraulichRau_G6(kWert, di);
            RM = rohrreibung(la, di, vm, dichte);
        }

        // transition area (Übergangsbereich)
        if( hydr > 65 && hydr < 1300)
        {
            // Gleichung 5 nach DIN 1988-300
            la = lamdaUebergangsbereich_G5(re, di, kWert);
            RM = rohrreibung(la, di, vm, dichte);
        }
    }

    // laminare Strömungen
    if(re < 2320)
    {
       la = 64/re;
       RM = rohrreibung(la, di, vm, dichte);
    }

    return RM;
}


double DIN1988300::rohrreibung(double la, double di, double vm, double dichte)
{
    double rm = 0.0;
    di = di / 1000; // in meter
    rm = la * (1/di) * (dichte/2) * qPow(vm , 2);
    return rm/100; // in hPa
}

// Formular Regulation DIN 1988-300 Gleichnug 4
// *********************************************
double DIN1988300::lamdaHydraulichGlatt_G4(double re)
{
    double la = 0.00002; // start value

    double la_left;
    double la_right;

    do{
        la_left = lamdaLeft_G4(la );
        la_right = lamdaRight_G4(la,re);
        la = la + 0.00001;

    }while (la_left > la_right+0.00001);

    return la;
}

double DIN1988300::lamdaUebergangsbereich_G5(double re, double di, double kWert)
{
    double la = 0.00002; // start value

    double la_left;
    double la_right;

    do{
        la_left = lamdaLeft_G5(la);
        la_right = lamdaRight_G5(re, di, kWert);
        la = la + 0.00001;

    }while (la_left > la_right+0.00001);

   return la;
}

double DIN1988300::lamdaHydraulichRau_G6(double kWert, double di)
{
    double lamda;

    double ze_1     = 3.71 * (kWert/di);
    double ze_log   = 2 *  ( qLn(ze_1) / qLn(10) );
    double ze_2     = qPow(ze_log, 2);

    lamda = 1 / ze_2;

    return lamda;
}

double DIN1988300::lamdaLeft_G4(double la)
{
    double lamda;
    lamda = 1 / qSqrt(la);
    return lamda;
}

double DIN1988300::lamdaLeft_G5(double la)
{
    double lamda;
    lamda = 1 / qSqrt(la);
    return lamda;
}

double DIN1988300::lamdaRight_G4(double la, double re)
{
    double lamda;

    double ze = re * qSqrt(la) / 2.51;

    lamda = 2 *  ( qLn(ze) / qLn(10) );

    return lamda;
}

double DIN1988300::lamdaRight_G5(double re, double di, double kWert)
{
    double lamda;
    qreal logRE = qLn(re) / qLn(10);
    qreal ze = 2.7 * qPow(logRE,1.2) / re + (kWert/di) / 3.71;

    lamda = -2 * (qLn(ze) / qLn(10));
    return lamda;

}
// *********************************************
// *********************************************

