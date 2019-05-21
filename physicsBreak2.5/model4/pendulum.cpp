#include "pendulum.h"
#include <algorithm>

physics::pendulum::pendulum(const float _m, bullet &_B, Qt3DCore::QNode *parent) :
    MaterialPoint(_m, parent), B(_B)
{
}

void physics::pendulum::move(const float t)
{
    // по идее перевод в градусы это 180 * rad / PI, но почему-то нам надо брать 1/4 от этого угла
    // честно? я ВАЩЕ хз, почему берётся 1/4 от угла. Но только так оно работает нормально
    // возможно, я чего-то не понимаю. Кто бы ты ни был, если ты поймёшь -- поделись плз
    rotate(45 * asinf((pos(t).x() - r().x()) / l) / PI);
    //setPosition(pos(t));
}

QVector3D physics::pendulum::pos(const float t)
{
    A = B.v(0).x() * m() / (B.m() + m()) / omega;
    float x = A * sinf(omega * t);
    return r() + QVector3D(x, l - sqrtf(l * l - x * x), 0);
}

QVector3D physics::pendulum::v(const float t)
{
    A = B.v(0).x() * m() / (B.m() + m()) / omega;
    float s = sinf(omega * t);
    float c = cosf(omega * t);
    return {
             A * omega * c,
             A * A * omega * s * c / sqrtf(l * l - A * A * s * s),
             0
           };
}
