#include "bullet.h"

physics::bullet::bullet(const float _m, Qt3DCore::QNode *parent) :
    MaterialPoint(_m, parent)
{
}

void physics::bullet::move(const float t)
{
    setPosition(pos(t));
}

QVector3D physics::bullet::pos(const float t)
{
    return v(0) * t + r();
}

QVector3D physics::bullet::v(const float t)
{
    // пуля движется с постоянной скоростью. Скорость определяется из
    // закона сохранения энергии: потенциальная энергия упругой
    // деформации пружины переходит в кинетическую энергию пули
    return { _b * sqrtf(_k / m()), 0, 0 };
}

void physics::bullet::set_k(const float __k)
{
    _k = __k;
}

void physics::bullet::set_b(const float __b)
{
    _b = __b;
}

float physics::bullet::b() const
{
    return _b;
}

float physics::bullet::k() const
{
    return _k;
}
