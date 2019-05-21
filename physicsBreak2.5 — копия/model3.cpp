#include "models.h"
#include "plot.h"

void Model3::Transform()
{
    tr1->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), float(A * 90. / PI)));
}

void Model3::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");

    addObject(ent, ":/Stands/Math3/podstavka.obj", ":/Stands/Math3/podstavka2.jpg");
    Qt3DCore::QEntity *pend = addObject(ent, ":/Stands/Math3/treeball.obj", ":/Stands/Math3/treeball.jpg");

    tr1 = new Qt3DCore::QTransform();

    //tr2 = new Qt3DCore::QTransform();

    pend->addComponent(tr1);
    tr1->setTranslation(QVector3D(0.0, 1.6f, 0.0));
}

Model3::Model3()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    LoadModel();

    sM = 0.5;
    sL = 0.1;
    sr = 3.5;
    sA = PI / 4.;
    A = sA;
    sW = 0.;
    Transform();

    i1 = new QLabel("Угол отклонения: 0.0 рад");
    i2 = new QLabel("Угловая скорость: 0.0 рад/с");
    i3 = new QLabel("Угловое ускорение 0.0 рад/c<sup>2</sup>");
    i4 = new QLabel("Энергия: 0.0 Дж");
    inf->addWidget(i1);
    inf->addWidget(i2);
    inf->addWidget(i3);
    inf->addWidget(i4);

    k1 = new QLabel(QString("Масса шара: %1 кг").arg(sM));
    k2 = new QLabel(QString("Длина стержня: %1 м").arg(sL));
    k3 = new QLabel(QString("Радиус шара: %1 м").arg(sr));
    k4 = new QLabel(QString("Начальный угол отклонения: %1 рад").arg(sA));
    k5 = new QLabel(QString("Начальная скорость маятника: %1 рад/c").arg(sW));

    s1 = new QSlider(Qt::Horizontal); s1->setMinimum(1); s1->setMaximum(10000); s1->setValue(int(sM * 100.));
    connect(s1, &QSlider::valueChanged, [=]()
    {
        this->sM = double(s1->value()) / 100.;
        k1->setText(QString("Масса шара: %1 кг").arg(sM));
    });
    s2 = new QSlider(Qt::Horizontal); s2->setMinimum(1); s2->setMaximum(10000); s2->setValue(int(sL * 1000.));
    connect(s2, &QSlider::valueChanged, [=]()
    {
        this->sL = double(s2->value()) / 1000.;
        k2->setText(QString("Длина стержня: %1 м").arg(sL));
    });

    s3 = new QSlider(Qt::Horizontal); s3->setMinimum(1); s3->setMaximum(10000); s3->setValue(int(sr * 1000.));
    connect(s3, &QSlider::valueChanged, [=]()
    {
        this->sr = double(s3->value()) / 1000.;
        k3->setText(QString("Радиус шара: %1 м").arg(sr));
    });
    s4 = new QSlider(Qt::Horizontal); s4->setMinimum(int(-2 * PI * 10000.)); s4->setMaximum(int(2 * PI * 10000.)); s4->setValue(int(sA * 10000.));
    connect(s4, &QSlider::valueChanged, [=]()
    {
        this->sA = double(s4->value()) / 10000.;
        k4->setText(QString("Начальный угол отклонения: %1 рад").arg(sA));
        A = sA;
        Transform();
    });
    s5 = new QSlider(Qt::Horizontal); s5->setMinimum(int(-2 * PI * 500.)); s5->setMaximum(int(2 * PI * 500.)); s5->setValue(int(sW * 500.));
    connect(s5, &QSlider::valueChanged, [=]()
    {
        this->sW = double(s5->value()) / 100.;
        k5->setText(QString("Начальная скорость маятника: %1 рад/c").arg(sW));
    });

    QPushButton *p1 = new QPushButton("Построить график перемещения");
    connect(p1, &QPushButton::clicked, [=]()
    {
        this->CreatePlot(0);
    });
    QPushButton *p2 = new QPushButton("Построить график скорости");
    connect(p2, &QPushButton::clicked, [=]()
    {
        this->CreatePlot(1);
    });
    QPushButton *p3 = new QPushButton("Построить график потенциальной энергии");
    connect(p3, &QPushButton::clicked, [=]()
    {
        this->CreatePlot(2);
    });
    QPushButton *p4 = new QPushButton("Построить график кинетической энергии");
    connect(p4, &QPushButton::clicked, [=]()
    {
        this->CreatePlot(3);
    });
    QPushButton *p5 = new QPushButton("Построить график энергии");
    connect(p5, &QPushButton::clicked, [=]()
    {
        this->CreatePlot(4);
    });
    set->addWidget(k1);
    set->addWidget(s1);
    set->addWidget(k2);
    set->addWidget(s2);
    set->addWidget(k3);
    set->addWidget(s3);
    set->addWidget(k4);
    set->addWidget(s4);
    set->addWidget(k5);
    set->addWidget(s5);
    set->addWidget(p1);
    set->addWidget(p2);
    set->addWidget(p3);
    set->addWidget(p4);
    set->addWidget(p5);
}



void Model3::Init()
{
    time = 0.;
    M = sM;
    L = sL;
    r = sr;
    A = sA;
    W = sW;
    S = PI * sr * sr;
}

double Model3::func(double axis, double speed)
{
    double r = -g * sin(axis) / L - (speed > 0 ? 1 : -1) * Cx * pl * speed * speed * S / (2 * M);
    return fabs(r) > 5e-2 ? r : 0.;
}

void Model3::Compute(double dt)
{
    double K[4], Q[4];
    E = func(A, W);
    K[0] = dt * E;
    Q[0] = dt * W;

    K[1] = dt * func(A + Q[0] / 2, W + K[0] / 2);
    Q[1] = dt * (W + K[0] / 2);

    K[2] = dt * func(A + Q[1] / 2, W + K[1] / 2) ;
    Q[2] = dt * (W + K[1] / 2);

    K[3] = dt * func(A + Q[2] , W + K[2]);
    Q[3] = dt * (W + K[2]);

    double r =  (K[0] + 2 * K[1] + 2 * K[2] + K[3]) / 6;
    W += r;
    A += (Q[0] + 2 * Q[1] + 2 * Q[2] + Q[3]) / 6;
    Ek = M * W * W * L * L / 2;
    Ep = M * g * L * (1 - cos(A));
}

void Model3::Update(double dt)
{
    time += dt;
    Compute(dt);
    Transform();
    for (auto plot : plots)
        if (plot->GetState() == Plot::State::Active)
            plot->Update();
        else
        {
            plot->Destroy();
            plots.removeOne(plot);
        }

    i1->setText(QString("Угол отклонения: %1 рад").arg(A));
    i2->setText(QString("Угловая скорость: %1 рад/с").arg(W));
    i3->setText(QString("Угловое ускорение %1 рад/c<sup>2</sup>").arg(E));
    i4->setText(QString("Энергия: %1 Дж").arg(GetEnergy()));

}

void Model3::CreatePlot(int plotID)
{
    Plot *plot = nullptr;

    switch (plotID)
    {
        case 0:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetA(); }, "Угловое смещение, рад");
        break;
        case 1:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetW(); }, "Угловая скорость, рад/c");
        break;
        case 2:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetEp(); }, "Потенциальная энергия, Дж");
        break;
        case 3:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetEk(); }, "Кинетическая энергия, Дж");
        break;
        case 4:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetEnergy(); }, "Полная энергия, Дж");
        break;
    }


    if (plot)
    {
        plot->show();
        plots.append(plot);
    }
}
