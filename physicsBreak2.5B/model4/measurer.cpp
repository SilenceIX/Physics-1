#include "measurer.h"

physics::measurer::measurer(const float _m, pendulum &_P, Qt3DCore::QNode *parent) :
    MaterialPoint(_m, parent), P(_P)
{
    finished = false;
}

void physics::measurer::move(const float t)
{
    setPosition(pos(t));
}

QVector3D physics::measurer::pos(const float t)
{
    // pend_len - поправка на длину подвеса маятника
    if (t >= T / 4) {
        finished = true;
    }
    float x = P.pos(finished ? T / 4 : t).x() + pend_len;
    return QVector3D(x, 0, 0) + r();
}

QVector3D physics::measurer::v(const float t)
{
    float x = P.pos(t).x();
    return r().x() < x ? QVector3D(P.v(t).x(), 0, 0) : QVector3D(0, 0, 0);
}
