#ifndef MATERIALPOINT_H
#define MATERIALPOINT_H

#include "object.h"

namespace physics {

class MaterialPoint : public object
{
    Q_OBJECT
    Q_PROPERTY(float time READ getTime WRITE setTime NOTIFY timeChanged)
private:
    static float time; // время, прошедшее с начал эксперимента
public:
    MaterialPoint(const float __m, Qt3DCore::QNode *parent = nullptr);
    float     _m; // масса
    QVector3D _r; // начальное положение, радиус-вектор
    virtual void      move(const float t) = 0; // выполнить перемещение
    virtual QVector3D pos (const float t) = 0; // текущее положение, радиус-вектор
    virtual QVector3D v   (const float t) = 0; // мгновенная скорость

    void      setTime(const float       t);
    void      set_m  (const float     __m);
    void      set_r  (const QVector3D __r);
    float     getTime() const;
    float     m() const;
    QVector3D r() const;

signals:
    void timeChanged() const;
};

}
#endif // MATERIALPOINT_H
