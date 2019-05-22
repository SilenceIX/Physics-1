#include "mathgalpend.h"

// Движение "слева", т.е. обычный математический маятник
void galpend::Compute_left_down()
{
    double omega = 0.0;

    omega = sqrt(g / l);
    angle = A0 * cos(omega * t);
    x = -l * sin(angle);
    y = -l * cos(angle);
    velocity = -A0 * omega * sin(omega * t);
    Ek = m * pow(velocity, 2) / 2;
    Ep = m * g * pow(angle, 2) / (2 * l);
    E = Ek + Ep;
}

// Движение "справа", сжатый маятник
void galpend::Compute_right()
{
    double k = 0.0;
    double omega = 0.0;

    k = l / h; // Коэффициент сжатия
    omega = sqrt(g * k / l);
    angle = A0 * sqrt(k) * cos(omega * t);
    x = -l / k * sin(angle);
    y = -l / k * cos(angle) - l * (1 - 1 / k);
    velocity = -A0 * sqrt(k) * omega * sin(omega * t);
    Ek = m * pow(velocity, 2) / 2;
    Ep = m * g * pow(angle, 2) / (2 * h);
    E = Ek + Ep;
}

void galpend::func()
{
        double angle = 0.0;
        double omega = 0.0;

        // Вычисление начального положения, т.е. Xmin и Ymax, t = 0
        omega = sqrt(g / l);
        angle = A0 * cos(omega * t);
        x = -l * sin(angle);
        y = -l * cos(angle);
        velocity = -A0 * omega * sin(omega * t);
        Ek = m * pow(velocity, 2) / 2;
        Ep = m * g * pow(angle, 2) / (2 * l);
        E = Ek + Ep;
        t += dt;
        T += dt;

        // Соответствующие периоды колебаний
        double T_left = 2 * M_PI * sqrt(l / g);
        double T_right = 2 * M_PI * sqrt(h / g);

        while (T <= 5.0)
        {
            while (x <= 0.0)
            {
                Compute_left_down();
                /*fout << T << "\t" << P.E << std::endl;*/ // Тут должно быть оживление маятника
                t += dt;
                T += dt;
            }
            t = T_right / 4;
            while (t < 3 * T_right / 4)
            {
                Compute_right();
                /*fout << T << "\t" << P.E << std::endl;*/ // Тут должно быть оживление маятника
                t += dt;
                T += dt;
            }
            t = 3 * T_left / 4;
            while (t < T_left)
            {
                Compute_left_down();
                /*fout << T << "\t" << P.E << std::endl;*/ // Тут должно быть оживление маятника
                t += dt;
                T += dt;
            }
        }
}
