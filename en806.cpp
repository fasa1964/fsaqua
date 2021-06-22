#include "en806.h"



EN806::EN806()
{

}

//double EN806::getDA(int mindex, int lu, int luMax, double len)
//{
//    double da = 0;

//    if(mindex == 0) // copper
//        da = daKupferEN806(lu, luMax, len);

//    if(mindex == 1) // stainless steel
//        da = daEdelstahlEN806(lu, luMax, len);

//    if(mindex == 2) // pex-al (verbund)
//        da = daPEXAL_EN806(lu, luMax, len) ;

//    if(mindex == 3) // pe-x
//        da = daPEX_EN806(lu, luMax, len);

//    if(mindex == 4) // pb
//        da = daPB_EN806(lu, luMax, len);

//    if(mindex == 5) // pp
//        da = daPP_EN806(lu, luMax, len);

//    if(mindex == 6) // PVC
//        da = daPVC_EN806(lu, luMax, len);

//    if(mindex == 7) // verz. Stahl
//        da = daVerzStahlEN806(lu, luMax, len);

//    return da;
//}

double EN806::getDA(const QString &material, int lu, int luMax, double len)
{
    double da = 0;

    if(material == "Kupfer") // copper
        da = daKupferEN806(lu, luMax, len);

    if(material == "Edelstahl") // stainless steel
        da = daEdelstahlEN806(lu, luMax, len);

    if(material == "Edelstahl") // pex-al (verbund)
        da = daPEXAL_EN806(lu, luMax, len) ;

    if(material == "PE-X") // pe-x
        da = daPEX_EN806(lu, luMax, len);

    if(material == "PB") // pb
        da = daPB_EN806(lu, luMax, len);

    if(material == "PP") // pp
        da = daPP_EN806(lu, luMax, len);

    if(material == "PVC") // PVC
        da = daPVC_EN806(lu, luMax, len);

    if(material == "verz. Stahl") // verz. Stahl
        da = daVerzStahlEN806(lu, luMax, len);

    return da;
}

double EN806::daKupferEN806(int lu, int luMax, double len)
{
    double da = 0;

    if(lu == 1){
       if(len <= 20.0)
        return da = 12;
       else
        return da = 15;
    }

    if(lu == 2){
        if(len <= 7)
          return da = 12;
        else
          return da = 15;
    }

    if(lu == 3){
        if(len <= 5 && luMax <= 2)
            return da = 12;
        if(len <= 15)
            return da = 15;
        if(len > 15)
            return da = 18;
    }

    if(lu == 4){
        if(len <= 9)
            return da = 15;
        else
            return da = 18;
    }

    if(lu > 4 && lu <= 6){
        if(len <= 7 && luMax <= 4)
            return da = 15;
        else
            return da = 18;
    }

    if(lu > 6 && lu <= 10)
    {
        if(luMax <= 5)
            return da = 18;
        else
            return da = 22;
    }

    if(lu > 10 && lu <= 20)
    {
        if(luMax <= 8)
            return da = 22;
        else
            return da = 28;
    }


    if(lu > 20 && lu <= 50)
        return da = 28;

    if(lu > 50 && lu <= 165)
        return da = 35;

    if(lu > 165 && lu <= 430)
        return da = 42;

    if(lu > 430 && lu <= 1050)
        return da = 54;

    if(lu > 1050 && lu <= 2100)
        return da = 76.1;

    return da;
}

double EN806::daEdelstahlEN806(int lu, int luMax, double len)
{
    double da = 0;

    if(lu <= 3)
    {
       if(len <= 15)
           return da = 15;
       if(len > 15)
           return da = 18;
     }

     if(lu == 4 )
     {
         if(len <= 9)
             return da = 15;
         else
             return da = 18;
     }

     if(lu > 4 && lu <= 6)
     {
         if(len <= 7 && luMax <= 4)
             return da = 15;
         else
             return da = 18;
     }

     if(lu > 6 && lu <= 10)
     {
         if(luMax <= 5)
            return da = 18;
         else
            return da = 22;
     }

     if(lu > 10 && lu <= 20)
     {
         if(luMax <= 8)
            return da = 22;
         else
            return da = 28;
     }

     if(lu > 20 && lu <= 50)
         return da = 28;

     if(lu > 50 && lu <= 165)
         return da = 35;

     if(lu > 165 && lu <= 430)
         return da = 42;

     if(lu > 430 && lu <= 1050)
         return da = 54;

     if(lu > 1050 && lu <= 2100)
         return da = 76.1;

     return da;
}

double EN806::daVerzStahlEN806(int lu, int luMax, double len)
{
    double da = 0;

    if(lu <= 6)
    {
        if(len <= 10 && luMax <= 4)
            return da = 21.3;
        else
            return da = 26.9;
    }

    if(lu > 6 && lu <= 16)
    {
        if(len <= 6 && luMax <= 15)
            return da = 26.9;
        else
            return da = 33.7;
    }

    if(lu > 16 && lu <= 40)
        return da = 33.7;

    if(lu > 40 && lu <= 160)
        return da = 42.4;

    if(lu > 160 && lu <= 300)
        return da = 48.3;

    if(lu > 300 && lu <= 600)
        return da = 60.3;

    if(lu > 600 && lu <= 1600)
        return da = 76.1;

    return da;

}

// EN 806-3 Table 3.4
double EN806::daPEX_EN806(int lu, int luMax, double len)
{
    double da = 0;

    if(lu == 1){
        if(len <= 13)
            return da = 12;
        else
            return da = 16;
    }

    if(lu == 2){
        if(len <= 4)
            return da = 12;
        else
            return da = 16;
    }

    if(lu == 3){
        if(len <= 9)
            return da = 16;
        else
            return da = 20;
    }

    if(lu == 4){
        if(len <= 5)
            return da = 16;
        else
            return da = 20;
    }

    if(lu == 5 && luMax <= 4){
        if(len <= 5)
            return da = 16;
        else
            return da = 20;
    }

    if(lu > 5 && lu <= 8){
        if(luMax <= 5)
            return da = 20;
        else
            return da = 25;
    }

    if(lu > 8 && lu <= 16)
        return da = 20;

    if(lu > 16 && lu <= 35)
        return da = 32;

    if(lu > 35 && lu <= 100)
        return da = 40;

    if(lu > 100 && lu <= 350)
        return da = 50;

    if(lu > 350 && lu <= 700)
        return da = 63;

    return da;

}

double EN806::daPEXAL_EN806(int lu, int luMax, double len)
{
    double da = 0;

    if(lu <= 3){
         if(len <= 9)
             return da = 16;
         if(len > 15)
             return da = 18;
     }

     if(lu == 4){
         if(len <= 5)
             return da = 16;
         else
             return da = 18;
     }

     if(lu == 5){
         if(len <= 4 && luMax <= 4)
             return da = 16;
         else
             return da = 18;
     }

     if(lu == 6 )
     {
         if(luMax <= 5)
            return da = 18;
         else
            return da = 20;
     }

     if(lu > 6 && lu <= 10 )
     {
         if(luMax <= 5)
             return da = 20;
         else
             return da = 26;
     }

     if(lu > 10 && lu <= 20 )
     {
         if(luMax <= 8)
             return da = 26;
         else
             return da = 32;
     }

     if(lu > 20 && lu <= 55)
         return da = 32;

     if(lu > 55 && lu <= 180)
         return da = 40;

     if(lu > 180 && lu <= 540)
         return da = 50;

     if(lu > 540 && lu <= 1300)
         return da = 63;

     return da;

}

double EN806::daPB_EN806(int lu, int luMax, double len)
{
    double da = 0;

    if(lu == 1)
    {
       if(len <= 20)
          return da = 12;
       else
          return da = 16;
    }

    if(lu == 2)
    {
        if(len <= 7)
            return da = 12;
        else
            return da = 16;
    }

    if(lu == 3)
    {
        if(len <= 5)
            return da = 12;
        if(len > 5 && len <= 15)
            return da = 16;
        else
            return da = 20;
    }

    if(lu == 4)
    {
        if(len <= 9)
            return da = 16;
        else
            return da = 20;
    }

    if(lu > 4 && lu <= 6)
    {
        if(len <= 7 && luMax <= 4)
            return da = 16;
        else
            return da = 20;
    }

    if(lu > 6 && lu <= 13)
        return da = 20;

    if(lu > 13 && lu <= 25)
        return da = 25;

    if(lu > 25 && lu <= 55)
        return da = 32;

    if(lu > 55 && lu <= 180)
        return da = 40;

    if(lu > 180 && lu <= 500)
        return da = 50;

    if(lu > 500 && lu <= 1100)
        return da = 63; // DN 50


    return da;
}

double EN806::daPP_EN806(int lu, int luMax, double len)
{
    double da = 0;
    if(lu == 1)
    {
       if(len <= 20)
          return da = 16;
       else
          return da = 20;
    }

    if(lu == 2)
    {
        if(len <= 12)
            return da = 16;
        else
            return da = 20;
    }

    if(lu == 3)
    {
        if(len <= 8)
            return da = 16;
        if(len > 8 && len <= 15)
            return da = 20;
        else
            return da = 25;
    }

    if(lu == 4)
    {
        if(len <= 9)
            return da = 20;
        else
            return da = 25;
    }

    if(lu > 4 && lu <= 6)
    {
        if(len <= 7 && luMax <= 4)
            return da = 20;
        else
            return da = 25;
    }

    if(lu > 6 && lu <= 13)
        return da = 25;

    if(lu > 13 && lu <= 30)
        return da = 32;

    if(lu > 30 && lu <= 70)
        return da = 40;

    if(lu > 70 && lu <= 200)
        return da = 50;

    if(lu > 200 && lu <= 540)
        return da = 63;

    if(lu > 540 && lu <= 970)
        return da = 75; // DN 50

    return da;
}

double EN806::daPVC_EN806(int lu, int luMax, double len)
{

    double da = 0;
    if(lu <= 3)
    {
       if(len <= 10)
          return da = 16;
       else
          return da = 20;
    }

    if(lu == 4)
    {
        if(len <= 6)
            return da = 16;
        else
            return da = 20;
    }

    if(lu == 5)
    {
        if(len <= 5 && luMax <= 4)
            return da = 16;
        else
            return da = 20;
    }

    if(lu > 5 && lu <= 10)
    {
        if(luMax <= 5)
            return da = 20;
        else
            return da = 25;
    }

    if(lu > 10 && lu <= 20)
    {
        if(luMax <= 8)
            return da = 25;
        else
            return da = 32;
    }

    if(lu > 20 && lu <= 45)
        return da = 32;

    if(lu > 45 && lu <= 160)
        return da = 40;

    if(lu > 160 && lu <= 420)
        return da = 50;

    if(lu > 420 && lu <= 900)
        return da = 63;

    return da;
}
