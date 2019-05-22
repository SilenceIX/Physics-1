#include "models.h"
#include "plot.h"

Model2::Model2()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();

    s_mass = 0.1;
    s_radius = 0.08;
    s_length = 0.2;
    s_psi_dot = 500;
    s_phi_dot = 0;
    s_theta = 1.57;

    phi = 0.0;
    psi = 0.0;
    theta_dot = 0.0;
    t = new QElapsedTimer();

    LoadModel();


    i1 = new QLabel("Угол отклонения: 0.0 рад");
    inf->addWidget(i1);

    {
        QLabel *k = new QLabel(QString("Длинна от вертикальной оси до диска: %1 м").arg(s_length));
        QSlider *s = new QSlider(Qt::Horizontal); s->setMinimum(15); s->setMaximum(20); s->setValue(int(s_length * 100.));
        connect(s, &QSlider::valueChanged, [=]()
        {
        s_length = double(s->value()) * 0.01;
        k->setText(QString("Длинна от вертикальной оси до диска: %1 м").arg(s_length));
        SetTransform();
        });
        set->addWidget(k);
        set->addWidget(s);

    }
    {
        QLabel *k = new QLabel(QString("Масса диска: %1 кг").arg(s_mass));
        QSlider *s = new QSlider(Qt::Horizontal); s->setMinimum(1); s->setMaximum(20); s->setValue(int(s_mass * 10.));
        connect(s, &QSlider::valueChanged, [=]()
        {
        s_mass = double(s->value()) * 0.1;
        k->setText(QString("Масса диска: %1кг").arg(s_mass));
        });
        set->addWidget(k);
        set->addWidget(s);
    }
    {
        QLabel *k = new QLabel(QString("Радиус диска: %1 м").arg(s_radius));
        QSlider *s = new QSlider(Qt::Horizontal); s->setMinimum(5); s->setMaximum(8); s->setValue(int(s_radius * 100.));
        connect(s, &QSlider::valueChanged, [=]()
        {
        s_radius = double(s->value()) * 0.01;
        k->setText(QString("Радиус диска: %1 м").arg(s_radius));
        SetTransform();
        });
        set->addWidget(k);
        set->addWidget(s);
    }
    {
        QLabel *k = new QLabel(QString("Скорость вращения диска: %1 рад/с").arg(s_psi_dot));
        QSlider *s = new QSlider(Qt::Horizontal); s->setMinimum(500); s->setMaximum(1000); s->setValue(int(s_psi_dot));
        connect(s, &QSlider::valueChanged, [=]()
        {
        s_psi_dot = s->value();
        k->setText(QString("Скорость вращения диска: %1 рад/с").arg(s_psi_dot));
        });
        set->addWidget(k);
        set->addWidget(s);
    }
    {
        QLabel *k = new QLabel(QString("Угол наклона от вертикальной оси: %1 рад").arg(s_phi_dot));
        QSlider *s = new QSlider(Qt::Horizontal); s->setMinimum(-30); s->setMaximum(30); s->setValue(int(s_phi_dot * 10.));
        connect(s, &QSlider::valueChanged, [=]()
        {
        s_phi_dot = double(s->value()) * 0.1;
        k->setText(QString("Начальня скорость прецессии диска: %1 рад/с").arg(s_phi_dot));
        });
        set->addWidget(k);
        set->addWidget(s);
    }
    {
        QLabel *k = new QLabel(QString("Начальня скорость прецессии диска: %1 рад/с").arg(s_theta));
        QSlider *s = new QSlider(Qt::Horizontal); s->setMinimum(785); s->setMaximum(1570); s->setValue(int(s_theta * 1000));
        connect(s, &QSlider::valueChanged, [=]()
        {
        s_theta = double(s->value()) * 0.001;
        k->setText(QString("Начальня скорость прецессии диска: %1 рад/с").arg(s_theta));
        SetTransform();
        });
        set->addWidget(k);
        set->addWidget(s);
    }

    Init();
    SetTransform();
}

void Model2::SetTransform()
{
    diskPos = QVector3D(cos(PI / 2 - s_theta)*sin(phi)*(2.5 * s_length - 0.45),
    0.95 + sin(PI / 2 - s_theta)*(2.5 * s_length - 0.45),
    cos(PI / 2 - s_theta)*cos(phi)*(2.5 * s_length - 0.45));

    nutation = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), s_theta * toGrad - 90);
    tr1->setRotation(precession * nutation * rotation);
    tr2->setRotation(precession * nutation);

    tr1->setScale3D(QVector3D(2.5 * s_radius, 2.5 * s_radius, 0.25));
    tr1->setTranslation((diskPos - QVector3D(0.0, 0.95, 0.0)) * s_length / 0.2 + QVector3D(0.0, 0.95, 0.0));
}

double Model2::dy1(double arg)
{
    double fun1 = -(L_phi - L_psi * cos(arg)) * L_psi;
    double fun2 = cos(arg) * (L_phi - L_psi * cos(arg)) * (L_phi - L_psi * cos(arg));
    double fun3 = mass * length * g * sin(arg);

    fun1 /= I0 * I0 * sin(arg);

    fun2 /= I0 * I0 * sin(arg) * sin(arg) * sin(arg);

    fun3 /= I0;

    return fun1 + fun2 + fun3;
}

double Model2::dy2(double arg)
{
    return arg;
}

double Model2::dy3(double arg)
{
    double fun1 = L_phi - L_psi * cos(arg);
    double fun2 = I0 * sin(arg) * sin(arg);

    return fun1 / fun2;
}

double Model2::dy4(double arg)
{
    double fun1 = L_psi / I_psi;
    double fun2 = phi_dot * cos(arg);

    return fun1 - fun2;
}

void Model2::Init()
{
    mass = s_mass;
    radius = s_radius;
    length = s_length;
    psi_dot = s_psi_dot;
    phi_dot = s_phi_dot;
    theta = s_theta;
    CalculateConstants();
    t->restart();
}

void Model2::Compute(double dt)
{
    time += dt;

    double K[4][4];

    K[0][0] = dt * dy1(theta);
    K[0][1] = dt * dy1(theta + K[0][0] / 2);
    K[0][2] = dt * dy1(theta + K[0][1] / 2);
    K[0][3] = dt * dy1(theta + K[0][2]);

    theta_dot = theta_dot + (K[0][0] + 2 * K[0][1] + 2 * K[0][2] + K[0][3]) / 6;

    K[1][0] = dt * dy2(theta_dot);
    K[1][1] = dt * dy2(theta_dot + K[1][0] / 2);
    K[1][2] = dt * dy2(theta_dot + K[1][1] / 2);
    K[1][3] = dt * dy2(theta_dot + K[1][2]);

    theta = theta + (K[1][0] + 2 * K[1][1] + 2 * K[1][2] + K[1][3]) / 6;

    phi_dot = L_phi - L_psi * cos(theta);
    phi_dot /= I0;
    phi_dot /= sin(theta);
    phi_dot /= sin(theta);

    psi_dot = L_psi / I_psi;
    psi_dot -= phi_dot * cos(theta);

    K[2][0] = dt * dy3(theta);
    K[2][1] = dt * dy3(theta + K[2][0] / 2);
    K[2][2] = dt * dy3(theta + K[2][1] / 2);
    K[2][3] = dt * dy3(theta + K[2][2]);

    phi = phi + (K[2][0] + 2 * K[2][1] + 2 * K[2][2] + K[2][3]) / 6;

    K[3][0] = dt * dy4(theta);
    K[3][1] = dt * dy4(theta + K[3][0] / 2);
    K[3][2] = dt * dy4(theta + K[3][1] / 2);
    K[3][3] = dt * dy4(theta + K[3][2]);

    psi = psi + (K[3][0] + 2 * K[3][1] + 2 * K[3][2] + K[3][3]) / 6;
}

void Model2::CalculateConstants()
{
    time = 0;
    I_psi = 0.5 * mass * radius * radius;
    I0 = mass * length * length + I_psi * 0.5;
    L_psi = I_psi * (phi_dot * cos(theta) + psi_dot);
    L_phi = I0 * phi_dot * sin(theta) * sin(theta) + L_psi * cos(theta);
}

void Model2::Transform()
{
    rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0),  toGrad * psi);
    precession = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), toGrad * phi);
    nutation = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), toGrad * theta - 90);

    tr1->setRotation(precession * nutation * rotation);
    tr2->setRotation(precession * nutation);
    tr3->setRotation(precession);

    diskPos = QVector3D(cos(PI / 2 - s_theta)*sin(phi)*(2.5 * s_length - 0.45),
                        0.95 + sin(PI / 2 - s_theta)*(2.5 * s_length - 0.45),
                        cos(PI / 2 - s_theta)*cos(phi)*(2.5 * s_length - 0.45));

    tr1->setTranslation(diskPos);

}

void Model2::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");

    Qt3DCore::QEntity *box = addObject(ent, ":/Stands/Math2/box.obj", ":/Stands/Math2/boxMat.png");
    Qt3DCore::QEntity *axis = addObject(ent, ":/Stands/Math2/axis.obj", ":/Stands/Math2/axisMat.png");
    Qt3DCore::QEntity *disk = addObject(ent, ":/Stands/Math2/disk.obj", ":/Stands/Math2/diskMat.png");
    Qt3DCore::QEntity *stand = addObject(ent, ":/Stands/Math2/stand.obj", ":/Stands/Math2/standMat.png");

    tr1 = new Qt3DCore::QTransform();
    tr2 = new Qt3DCore::QTransform();
    tr3 = new Qt3DCore::QTransform();
    tr4 = new Qt3DCore::QTransform();

    disk->addComponent(tr1);
    axis->addComponent(tr2);
    box->addComponent(tr3);
    stand->addComponent(tr4);

    tr1->setTranslation(QVector3D(0.0, 0.95, 0.0));
    tr2->setTranslation(QVector3D(0.0, 0.95, 0.0));
    tr3->setTranslation(QVector3D(0.0, 0.95, 0.0));
    tr4->setTranslation(QVector3D(0.0, 0.95, 0.0));

    tr1->setScale(0.25);
    tr2->setScale(0.25);
    tr3->setScale(0.25);
    tr4->setScale(0.25);
}

void Model2::Update(double dt)
{

    double delt = double(t->elapsed()) * 1e-3;
    for (double i = 0; i * 1e-5 < delt / 4.; ++i)
        Compute(1e-5);
    Transform();
    t->restart();
    for (auto plot : plots)
        if (plot->GetState() == Plot::State::Active)
            plot->Update();
        else
        {
            plot->Destroy();
            plots.removeOne(plot);
        }

    i1->setText(QString("Угол отклонения: %1 рад/c").arg(this->GetTheta()));
}

void Model2::CreatePlot(int plotID)
{
    Plot *plot = nullptr;

    switch (plotID)
    {
        case 0:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetPhi(); },  "Углол Phi");
        break;
        case 1:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetPsi(); }, "Углол Psi");
        break;
    }


    if (plot)
    {
        plot->show();
        plots.append(plot);
    }
}




