#ifndef PENDULUM_H
#define PENDULUM_H

#include "materialpoint.h"
#include "bullet.h"

namespace physics {

class pendulum : public MaterialPoint
{
public:
    float A    ; // амплитуда колебаний по оси x
    bullet &B  ; // пуля, с которой произошло столкновение
    pendulum(const float _m, bullet &_B, Qt3DCore::QNode *parent = nullptr);
    void      move(const float t) override;
    QVector3D pos (const float t) override;
    QVector3D v   (const float t) override;
    float Ek(const float t);
    float Ep(const float t);
};

}
#endif // PENDULUM_H
