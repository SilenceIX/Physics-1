#ifndef MATHGALPEND_H
#define MATHGALPEND_H

#include <qmath.h>

class galpend
{
private:
    const double g = 9.81;

    double A0 = 1.0;   // Амплитуда
    double l = 1.0;    // Длина нити в метрах
    double h = 0.5;    //высота планки
    double m = 0.5;    //масса груза
    double t = 0.0;    // Время для счета координат
    double x;          //
    double y;          //координаты
    double angle;      //угол
    double W = sqrt(g / l);          //циклическая частота до столкновения
    double W1;         //циклическая частота после столкновения
    double k;          //коэффицент сжатия
    double velocity;   //скорость
    double Ek;         //кинетическая энергия
    double Ep;         //потенциальная энергия
    double E;          //полная энергия системы

    double T_left = 2 * M_PI * sqrt(l / g);           //периоды при движении после столкновения
    double T_right = 2 * M_PI * sqrt(h / g);          //вправо и влево
    double t1 = T_right / 4, t2 = 3 * T_left / 4;     //Время для счета координат в этих двух периодах

    void Compute_left_down();
    void Compute_right();
public:
    galpend();
    void Init();
    void Compute(double dt);
    void Transform();
    void Update(double dt);
    double GetA() {return A0;}
    double GetW() {return W;}
    double GetW1() {return W1;}
    double GetEk() {return Ek;}
    double GetEp() {return Ep;}
    double GetEnergy() {return E;}
    double GetTime() {return t;}
};

#endif // MATHGALPEND_H
