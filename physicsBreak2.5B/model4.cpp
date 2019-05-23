#include "models.h"
Model4::Model4()
{
    root     = new Qt3DCore::QEntity;
    lab      = new physics::object   (root);
    bullet   = new physics::bullet   (0.01f, root);
    pendulum = new physics::pendulum (0.2f, *bullet, root);
    measurer = new physics::measurer (0.01f, *pendulum, root);
    P        = new QPropertyAnimation(pendulum, "time");
    M        = new QPropertyAnimation(measurer, "time");
    group    = new QParallelAnimationGroup(root);

    addObject(root, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(root, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");

    lab->setMesh(":/Stands/Math4/lab.obj");
    lab->setTexture(":/Stands/Math4/lab.png");
    lab->setPosition(QVector3D(0, 0, 0));

    pendulum->setMesh(":/Stands/Math4/pendulum.obj");
    pendulum->setTexture(":/Stands/Math4/pendulum.png");
    pendulum->setPosition(QVector3D(-0.22f, 3.2f, 1.15f)); // разместить в x = -0.22
    pendulum->set_r(pendulum->transform()->translation());

    measurer->setMesh(":/Stands/Math4/measurer.obj");
    measurer->setTexture(":/Stands/Math4/measurer.png");
    measurer->setPosition(QVector3D(0.07f, 0.395f, 0.96f));
    measurer->set_r(measurer->transform()->translation());


    P->setDuration(int(physics::T * 1e3f)); // измеряется в милисекундах
    P->setLoopCount(-1);
    P->setStartValue(0);
    P->setEndValue(physics::T);

    M->setDuration(int(physics::T * 1e3f));
    M->setLoopCount(-1);
    M->setStartValue(0);
    M->setEndValue(physics::T);

    group->addAnimation(P);
    group->addAnimation(M);



    box = new QVBoxLayout();
    bullet_mass_slider = new QSlider(Qt::Horizontal);
    bullet_mass_slider->setRange(4, 100);
    bullet_mass_slider->setSingleStep(1);
    bullet_mass_slider->setValue(10);
    QObject::connect(bullet_mass_slider, &QSlider::valueChanged, [&](int v) {
        bullet->set_m(float(v) / 1e3f);
    });
    pendulum_mass_slider = new QSlider(Qt::Horizontal);
    pendulum_mass_slider->setRange(100, 500);
    pendulum_mass_slider->setSingleStep(1);
    pendulum_mass_slider->setValue(200);
    QObject::connect(pendulum_mass_slider, &QSlider::valueChanged, [&](int v) {
        pendulum->set_m(float(v) / 1e3f);
    });
    k_slider = new QSlider(Qt::Horizontal);
    k_slider->setRange(1000, 15000);
    k_slider->setSingleStep(1);
    k_slider->setValue(10000);
    QObject::connect(k_slider, &QSlider::valueChanged, [&](int v) {
        bullet->set_k(float(v) / 1e3f);
    });
    b_slider = new QSlider(Qt::Horizontal);
    b_slider->setRange(1, 25);
    b_slider->setSingleStep(1);
    b_slider->setValue(10);
    QObject::connect(b_slider, &QSlider::valueChanged, [&](int v) {
        bullet->set_b(float(v) / 1e3f);
    });
    bullet_mass_Label = new QLabel("Масса пули, кг");
    pendulum_mass_Label = new QLabel("Масса подвеса маятника, кг");
    k_label = new QLabel("Жёсткость пружины");
    b_label = new QLabel("Деформация пружины, м");



    box->addWidget(bullet_mass_Label);
    box->addWidget(bullet_mass_slider);
    box->addWidget(pendulum_mass_Label);
    box->addWidget(pendulum_mass_slider);
    box->addWidget(k_label);
    box->addWidget(k_slider);
    box->addWidget(b_label);
    box->addWidget(b_slider);


}

void Model4::start() const
{
    group->start();
}

float Model4::get_l() const
{
    return physics::l;
}

float Model4::get_g() const
{
    return physics::g;
}

float Model4::get_k() const
{
    return bullet->k();
}

float Model4::get_b() const
{
    return bullet->b();
}

float Model4::get_T() const
{
    return physics::T;
}

float Model4::get_omega() const
{
    return physics::omega;
}

physics::bullet *Model4::get_bullet() const
{
    return bullet;
}

physics::pendulum *Model4::get_pendulum() const
{
    return pendulum;
}

physics::measurer *Model4::get_measurer() const
{
    return measurer;
}

void Model4::Init()
{
    start();
}
