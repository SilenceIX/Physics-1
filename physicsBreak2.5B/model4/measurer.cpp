#include "measurer.h"

physics::measurer::measurer(const float _m, pendulum &_P, Qt3DCore::QNode *parent) :
    MaterialPoint(_m, parent), P(_P)
{
}

void physics::measurer::move(const float t)
{
    setPosition(pos(t));
}

QVector3D physics::measurer::pos(const float t)
{
    float x = t < T / 4.f ? sinf(PI * P.transform()->rotationX() / 180) * l : P.A;
    // 1 координата = 25 см = 0.25 м = 1/4 м
    return QVector3D(x * 4, 0, 0) + r();
}

QVector3D physics::measurer::v(const float t)
{
    return t < T / 4.f ? QVector3D(P.v(t).x(), 0, 0) : QVector3D(0, 0, 0);
}
