#include "mathgalpend.h"

// Движение "слева", т.е. обычный математический маятник
void galpend::Compute_left_down()
{        
    angle = A0 * cos(W * t);
    x = -l * sin(angle);
    y = -l * cos(angle);
    velocity = -A0 * W * sin(W * t);
    Ek = m * pow(velocity, 2) / 2;
    Ep = m * g * pow(angle, 2) / (2 * l);
    E = Ek + Ep;
}

// Движение "справа", сжатый маятник
void galpend::Compute_right()
{
    k = l / h;
    W1 = sqrt(g * k / l);
    angle = A0 * sqrt(k) * cos(W1 * t);
    x = -l / k * sin(angle);
    y = -l / k * cos(angle) - l * (1 - 1 / k);
    velocity = -A0 * sqrt(k) * W1 * sin(W1 * t);
    Ek = m * pow(velocity, 2) / 2;
    Ep = m * g * pow(angle, 2) / (2 * h);
    E = Ek + Ep;
}

void galpend::Init()
{
    t = 0.;
}

void galpend::Compute(double dt)
{
    t+=dt;
    if (x <= 0.0)
    {
         Compute_left_down();
    }
    else
    {
        if (t1 < 3 * T_right / 4)
        {
            Compute_right();
            t1 += dt;
        }
        else
        {
            if (t2 < T_left)
            {
                Compute_left_down();
                t2+=dt;
            }
            else {
                t1 = T_right / 4;
                t2 = 3 * T_left / 4;
            }
        }
    }
}

void galpend::Transform()
{

}

void galpend::Update(double dt)
{
    Compute(dt);
    Transform();
}
