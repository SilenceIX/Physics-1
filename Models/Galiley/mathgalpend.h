#ifndef MATHGALPEND_H
#define MATHGALPEND_H

#include <qmath.h>

class galpend
{
private:
    double A0 = 1.0;   // Амплитуда
    double l = 1.0;    // Длина нити в метрах
    double h = 0.5;    //высота планки
    double m = 0.5;    //масса груза
    double t = 0.0;    // Время для счета координат
    double T = 0.0;    // Глобальное время системы
    const double g = 9.81;
    double dt = 0.005;
    double x;
    double y;          //координаты
    double angle;      //угол
    double velocity;   //скорость
    double Ek;         //кинетическая энергия
    double Ep;         //потенциальная энергия
    double E;          //полная энергия системы
    void Compute_left_down();
    void Compute_right();
public:
    void func();
};

#endif // MATHGALPEND_H
