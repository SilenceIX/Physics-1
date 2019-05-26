#ifndef laboratory_H
#define laboratory_H

#include <Qt3DExtras>
#include <QObject>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include "measurer.h"
#include "Camera.h"

class laboratory
{
private:
    Qt3DExtras::Qt3DWindow *w = nullptr;
    // камера (а точнее, контроллер камеры)
    Camera *cam = nullptr;

    Qt3DCore::QEntity       *root     = nullptr;
    physics ::object        *lab      = nullptr;
    physics ::bullet        *bullet   = nullptr;
    physics ::pendulum      *pendulum = nullptr;
    physics ::measurer      *measurer = nullptr;
    QPropertyAnimation      *P        = nullptr;
    QPropertyAnimation      *M        = nullptr;
    QParallelAnimationGroup *group    = nullptr;

    QVBoxLayout *opt                  = nullptr;
    QVBoxLayout *inf                  = nullptr;
    QSlider     *bullet_mass_slider   = nullptr;
    QSlider     *pendulum_mass_slider = nullptr;
    QSlider     *k_slider             = nullptr;
    QSlider     *b_slider             = nullptr;
    QLabel      *bullet_mass_label    = nullptr;
    QLabel      *pendulum_mass_label  = nullptr;
    QLabel      *k_label              = nullptr;
    QLabel      *b_label              = nullptr;
    QLabel      *move_label           = nullptr;
    QLabel      *speed_label          = nullptr;
    QLabel      *time_label           = nullptr;
    QPushButton *start_button         = nullptr;


public:
    laboratory();
    bool start_or_stop() const; // return true if started and return false if stopped

    float get_l() const;
    float get_g() const;
    float get_k() const;
    float get_b() const;
    float get_T() const;
    float get_omega() const;
    float get_time()  const;
    QVBoxLayout *get_opt() const;
    QVBoxLayout *get_inf() const;
    Qt3DExtras::Qt3DWindow *get_window () const;
    physics::bullet   *get_bullet() const;
    physics::pendulum *get_pendulum() const;
    physics::measurer *get_measurer() const;
};

#endif // laboratory_H
