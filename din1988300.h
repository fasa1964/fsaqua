#ifndef DIN1988300_H
#define DIN1988300_H


class DIN1988300
{
public:
    DIN1988300();


    double reynolds(double di, double v, double kinVis);

    double pressureLossPerMeter(double re, double kWert, double di, double vm, double dichte);
    double rohrreibung(double la, double di, double vm, double dichte);

    // return value of lamda
    double lamdaHydraulichGlatt_G4(double re);
    double lamdaUebergangsbereich_G5(double re, double di, double kWert);
    double lamdaHydraulichRau_G6(double kWert, double di);
    double lamdaLeft_G4(double la);
    double lamdaLeft_G5(double la);
    double lamdaRight_G4(double la, double re);
    double lamdaRight_G5(double re, double di, double kWert);
};

#endif // DIN1988300_H
