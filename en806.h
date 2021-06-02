#ifndef EN806_H
#define EN806_H


class EN806
{
public:
    EN806();

    double getDA(int mindex, int lu, int luMax, double len);


    double daKupferEN806(int lu, int luMax, double len);
    double daEdelstahlEN806(int lu, int luMax, double len);
    double daVerzStahlEN806(int lu, int luMax, double len);
    double daPEX_EN806(int lu, int luMax, double len);
    double daPEXAL_EN806(int lu, int luMax, double len);
    double daPB_EN806(int lu, int luMax, double len);
    double daPP_EN806(int lu, int luMax, double len);
    double daPVC_EN806(int lu, int luMax, double len);

};

#endif // EN806_H
