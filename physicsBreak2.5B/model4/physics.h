#ifndef PHYSICS_H
#define PHYSICS_H

#include <climits>
#include <Qt3DCore>
#include <QtMath>

namespace physics {

const float PI = float(M_PI); // число пи ~ 3.1415...
// параметры системы
const float pend_len = 0.23f; // длина подвеса
const float g = 9.810f; // ускорение свободного падения
const float l = 0.675f; // высота маятника
const float omega = sqrtf(g / l); // циклическая частота
const float T = 2.f * PI / omega; // период
const int   duration = INT_MAX / 1000;

}

#endif // PHYSICS_H
