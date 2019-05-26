#include "laboratory.h"

laboratory::laboratory()
{
    root       = new Qt3DCore::QEntity;
    lab        = new physics::object   (root);
    bullet     = new physics::bullet   (0.01f, root);
    pendulum   = new physics::pendulum (0.2f, *bullet, root);
    measurer   = new physics::measurer (0.01f, *pendulum, root);
    P          = new QPropertyAnimation(pendulum, "time");
    M          = new QPropertyAnimation(measurer, "time");
    group      = new QParallelAnimationGroup(root);
    w          = new Qt3DExtras::Qt3DWindow();
    cam        = new Camera(root);
    opt        = new QVBoxLayout;
    inf        = new QVBoxLayout;

    // init laboratry
    lab->setMesh(":/res/lab.obj");
    lab->setTexture(":/res/lab.png");
    lab->setPosition(QVector3D(0, 0, 0));

    // не работает. Почему?
    //bullet->setMesh(":/res/bullet.obj");
    //bullet->setTexture(":/res/bullet.png");
    //bullet->setPosition(QVector3D(1, 3.2f, 1));
    //bullet->set_r(bullet->transform()->translation());

    pendulum->setMesh(":/res/pendulum.obj");
    pendulum->setTexture(":/res/pendulum.png");
    pendulum->setPosition(QVector3D(-0.22f, 3.2f, 1.15f));
    pendulum->set_r(pendulum->transform()->translation());

    measurer->setMesh(":/res/measurer.obj");
    measurer->setTexture(":/res/measurer.png");
    measurer->setPosition(QVector3D(0.07f, 0.395f, 0.96f));
    measurer->set_r(measurer->transform()->translation());

    P->setDuration(physics::duration * 1000); // измеряется в милисекундах
    P->setLoopCount(-1);
    P->setStartValue(0);
    P->setEndValue(physics::duration);

    M->setDuration(physics::duration * 1000);
    M->setLoopCount(-1);
    M->setStartValue(0);
    M->setEndValue(physics::duration);

    group->addAnimation(P);
    group->addAnimation(M);

    w->setWindowState(Qt::WindowMaximized);
    w->setRootEntity(root);

    cam->setCamera(w->camera()); // прикрепить камеру к контроллеру
    cam->camera()->setPosition(QVector3D(0.5f, 1, 15)); // начальное положение камеры
    cam->camera()->setViewCenter(QVector3D(0.5f, 1, 0.96f)); // точка, вокруг которой камера будет вращаться
    cam->setLookSpeed(100); // скорость вращения
    cam->setLinearSpeed(10); // скорость зума


    // init options
    bullet_mass_label   = new QLabel("");
    pendulum_mass_label = new QLabel("");
    k_label             = new QLabel("");
    b_label             = new QLabel("");
    move_label          = new QLabel("Смещение маятника: 0.000 м");
    speed_label         = new QLabel("Скорость пули: 00.000 м/с");
    time_label          = new QLabel("Время: 00:00:00");

    QObject::connect(pendulum, &physics::MaterialPoint::timeChanged, [&]() {
        QString text;
        float x = (measurer->transform()->translation() - measurer->r()).x() / 4;
        text.sprintf("Смещение маятника: %0.3f м", double(x));
        move_label->setText(text);
        text.sprintf("Скорость пули: %02.3f м/с", double(bullet->v(0).x()));
        speed_label->setText(text);
        int s = int(pendulum->getTime());
        int ms = int(1e3f * pendulum->getTime()) % 1000;
        text.sprintf("Время: %02d:%02d:%02d.%03d", s / 3600, s / 60 % 60, s % 60, ms);
        time_label->setText(text);
    });

    bullet_mass_slider = new QSlider(Qt::Horizontal);
    bullet_mass_slider->setRange(10, 100);
    bullet_mass_slider->setSingleStep(1);
    QObject::connect(bullet_mass_slider, &QSlider::valueChanged, [&](int v) {
        bullet->set_m(float(v) / 1e3f);
        QString text;
        text.sprintf("Масса пули: %0.3f кг", double(bullet->m()));
        bullet_mass_label->setText(text);
    });
    bullet_mass_slider->setValue(55);
    bullet_mass_slider->setObjectName("bullet_mass_slider");
    pendulum_mass_slider = new QSlider(Qt::Horizontal);
    pendulum_mass_slider->setRange(100, 500);
    pendulum_mass_slider->setSingleStep(1);
    QObject::connect(pendulum_mass_slider, &QSlider::valueChanged, [&](int v) {
        pendulum->set_m(float(v) / 1e3f);
        QString text;
        text.sprintf("Масса подвеса маятника: %0.3f кг", double(pendulum->m()));
        pendulum_mass_label->setText(text);
    });
    pendulum_mass_slider->setValue(300);
    k_slider = new QSlider(Qt::Horizontal);
    k_slider->setRange(600, 1200);
    k_slider->setSingleStep(1);
    QObject::connect(k_slider, &QSlider::valueChanged, [&](int v) {
        bullet->set_k(float(v));
        QString text;
        text.sprintf("Жёсткость пружины: %04.0f", double(bullet->k()));
        k_label->setText(text);
    });
    k_slider->setValue(900);
    k_slider->setObjectName("k_slider");
    b_slider = new QSlider(Qt::Horizontal);
    b_slider->setRange(15, 25);
    b_slider->setSingleStep(1);
    QObject::connect(b_slider, &QSlider::valueChanged, [&](int v) {
        bullet->set_b(float(v) / 1e3f);
        QString text;
        text.sprintf("Деформация пружины: %01.3f м", double(bullet->b()));
        b_label->setText(text);
    });
    b_slider->setValue(20);
    b_slider->setObjectName("b_slider");

    start_button = new QPushButton("Старт");
    QObject::connect(start_button, &QPushButton::clicked, [&]() { start_or_stop(); });

    inf->addWidget(move_label);
    inf->addWidget(speed_label);
    inf->addWidget(time_label);

    opt->addWidget(bullet_mass_label);
    opt->addWidget(bullet_mass_slider);
    opt->addWidget(pendulum_mass_label);
    opt->addWidget(pendulum_mass_slider);
    opt->addWidget(k_label);
    opt->addWidget(k_slider);
    opt->addWidget(b_label);
    opt->addWidget(b_slider);
}

bool laboratory::start_or_stop() const
{
    if (group->state() == QParallelAnimationGroup::State::Running) {
        group->stop();
        pendulum->setTime(0);
        pendulum->move(0);
        measurer->move(0);
        return false;
    }
    else {
        start_button->setText("Стоп");
        group->start();
        return true;
    }
}

float laboratory::get_l() const
{
    return physics::l;
}

float laboratory::get_g() const
{
    return physics::g;
}

float laboratory::get_k() const
{
    return bullet->k();
}

float laboratory::get_b() const
{
    return bullet->b();
}

float laboratory::get_T() const
{
    return physics::T;
}

float laboratory::get_omega() const
{
    return physics::omega;
}

float laboratory::get_time() const
{
    return pendulum->getTime();
}

QVBoxLayout *laboratory::get_opt() const
{
    return opt;
}

QVBoxLayout *laboratory::get_inf() const
{
    return inf;
}

Qt3DExtras::Qt3DWindow *laboratory::get_window() const
{
    return w;
}

physics::bullet *laboratory::get_bullet() const
{
    return bullet;
}

physics::pendulum *laboratory::get_pendulum() const
{
    return pendulum;
}

physics::measurer *laboratory::get_measurer() const
{
    return measurer;
}
