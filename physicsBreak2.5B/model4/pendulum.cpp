#include "pendulum.h"
#include <algorithm>

physics::pendulum::pendulum(const float _m, bullet &_B, Qt3DCore::QNode *parent) :
    MaterialPoint(_m, parent), B(_B)
{
}

void physics::pendulum::move(const float t)
{
    rotate(180.f * asinf(pos(t).x() / l) / PI);
}

QVector3D physics::pendulum::pos(const float t)
{
    A = B.v(0).x() * B.m() / (B.m() + m()) / omega;
    float x = A * sinf(omega * t);
    //return r() + QVector3D(x, l - sqrtf(l * l - x * x), 0);
    return QVector3D(x, l - sqrtf(l * l - x * x), 0);
}

QVector3D physics::pendulum::v(const float t)
{
    A = B.v(0).x() * B.m() / (B.m() + m()) / omega;
    float s = sinf(omega * t);
    float c = cosf(omega * t);
    return {
             A * omega * c,
             A * A * omega * s * c / sqrtf(l * l - A * A * s * s),
             0
    };
}

float physics::pendulum::Ek(const float t)
{
    float v_len = v(t).length();
    return (m() + B.m()) * v_len * v_len / 2.f;
}

float physics::pendulum::Ep(const float t)
{
    return (m() + B.m()) * g * pos(t).y();
}
