#ifndef MEASURER_H
#define MEASURER_H

#include "materialpoint.h"
#include "pendulum.h"

namespace physics {

class measurer : public MaterialPoint
{
private:
    pendulum &P;
public:
    //bool finished;
    measurer(const float _m, pendulum &_P, Qt3DCore::QNode *parent = nullptr);
    void      move(const float t) override;
    QVector3D pos (const float t) override;
    QVector3D v   (const float t) override;
};

}
#endif // MEASURER_H
