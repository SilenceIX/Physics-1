#include "materialpoint.h"

float physics::MaterialPoint::time = 0;

physics::MaterialPoint::MaterialPoint(const float __m, Qt3DCore::QNode *parent) :
    object(parent), _m(__m), _r(transform()->translation())
{
}

void physics::MaterialPoint::setTime(const float t)
{
    time = t;
    move(t);
    emit timeChanged();
}

void physics::MaterialPoint::set_m(const float __m)
{
    _m = __m;
}

void physics::MaterialPoint::set_r(const QVector3D __r)
{
    _r = __r;
}

float physics::MaterialPoint::getTime() const
{
    return time;
}

float physics::MaterialPoint::m() const
{
    return _m;
}

QVector3D physics::MaterialPoint::r() const
{
    return _r;
}
