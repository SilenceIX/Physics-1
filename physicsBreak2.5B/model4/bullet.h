#ifndef BULLET_H
#define BULLET_H

#include "materialpoint.h"

namespace physics {

class bullet : public MaterialPoint
{
private:
    float _k = 10.00f; // жёсткость пружины
    float _b = 0.005f; // деформация пружины
public:
    bullet(const float _m, Qt3DCore::QNode *parent = nullptr);
    void      move(const float t) override;
    QVector3D pos (const float t) override;
    QVector3D v   (const float t) override;

    void set_k(const float __k);
    void set_b(const float __b);
    float b() const;
    float k() const;
};
}
#endif // BULLET_H
