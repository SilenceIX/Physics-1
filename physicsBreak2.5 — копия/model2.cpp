#include "models.h"
#include "plot.h"

Model2::Model2()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();

    mass = 0.1;
    radius = 0.05;
    length = 0.15;
    psi_dot = 500;
    phi_dot = 0;
    theta = 1.57;
    t = new QElapsedTimer();

    LoadModel();


    i1 = new QLabel("Угол отклонения: 0.0 рад");
    inf->addWidget(i1);

    k1 = new QLabel("Начальный угол отклонения: 0.0 рад");
    k2 = new QLabel("Коэффициент сопротивления: 0.0");
    k3 = new QLabel("Цикличиская частота: 0.02 рад/c");

    s1 = new QSlider(Qt::Horizontal); s1->setMinimum(0); s1->setMaximum(int(PI * 500.));
    connect(s1, &QSlider::valueChanged, [=]()
    {
        //this->psi_dot = double(s1->value()) / 1000.;
        k1->setText(QString("Начальный угол отклонения: %1 рад").arg(psi_dot));
    });
    s2 = new QSlider(Qt::Horizontal); s2->setMinimum(0); s2->setMaximum(1000);
    connect(s2, &QSlider::valueChanged, [=]()
    {
        this->phi_dot = double(s2->value()) / 500.;
        k2->setText(QString("Коэффициент сопротивления: %1").arg(phi_dot));
    });
    s3 = new QSlider(Qt::Horizontal); s3->setMinimum(1); s3->setMaximum(500);
    connect(s3, &QSlider::valueChanged, [=]()
    {
        this->theta = double(s3->value()) / 50.;
        k3->setText(QString("Цикличиская частота: %1 рад/c").arg(theta));
    });
    QPushButton *p1 = new QPushButton("Построить график перемещения");
    connect(p1, &QPushButton::clicked, [=]()
    {
        this->CreatePlot(0);
    });


    set->addWidget(k1);
    set->addWidget(s1);
    set->addWidget(k2);
    set->addWidget(s2);
    set->addWidget(k3);
    set->addWidget(s3);
    set->addWidget(p1);
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
    rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0),  57. * psi);
    precession = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), 57. * phi);
    nutation = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), 57. * theta - 90);

    tr1->setRotation(precession * nutation * rotation);
    tr2->setRotation(precession * nutation);
    tr3->setRotation(precession);}

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
    for (double i = 0; i * 1e-5 < delt; ++i)
        Compute(dt * 1e-5);
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




