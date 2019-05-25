#include "models.h"
#include "plot.h"

Model2::Model2()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();

    mass = 0.1;
    radius = 0.08;
    length = 0.2;
    psi_dot = 500;
    phi_dot = 0;
    theta = 1.57;

    phi = 0;



    t = new QElapsedTimer();

    LoadModel();


    i1 = new QLabel("Угол отклонения: 0.0 рад");
    inf->addWidget(i1);

    {
        QLabel *k = new QLabel(QString("Длина от вертикальной оси до диска: %1 м").arg(length));
        s1 = new QSlider(Qt::Horizontal); s1->setMinimum(15); s1->setMaximum(20); s1->setValue(int(length * 100.));
        connect(s1, &QSlider::valueChanged, [=]()
        {
        length = double(s1->value()) * 0.01;
        k->setText(QString("Длина от вертикальной оси до диска: %1 м").arg(length));
        SetTransform();
        });
        set->addWidget(k);
        set->addWidget(s1);

    }
    {
        QLabel *k = new QLabel(QString("Масса диска: %1 кг").arg(mass));
        s2 = new QSlider(Qt::Horizontal); s2->setMinimum(1); s2->setMaximum(20); s2->setValue(int(mass * 10.));
        connect(s2, &QSlider::valueChanged, [=]()
        {
        mass = double(s2->value()) * 0.1;
        k->setText(QString("Масса диска: %1кг").arg(mass));
        });
        set->addWidget(k);
        set->addWidget(s2);
    }
    {
        QLabel *k = new QLabel(QString("Радиус диска: %1 м").arg(radius));
        s3 = new QSlider(Qt::Horizontal); s3->setMinimum(5); s3->setMaximum(8); s3->setValue(int(radius * 100.));
        connect(s3, &QSlider::valueChanged, [=]()
        {
        radius = double(s3->value()) * 0.01;
        k->setText(QString("Радиус диска: %1 м").arg(radius));
        SetTransform();
        });
        set->addWidget(k);
        set->addWidget(s3);
    }
    {
        QLabel *k = new QLabel(QString("Скорость вращения диска: %1 рад/с").arg(psi_dot));
        s4 = new QSlider(Qt::Horizontal); s4->setMinimum(500); s4->setMaximum(1000); s4->setValue(int(psi_dot));
        connect(s4, &QSlider::valueChanged, [=]()
        {
        psi_dot = s4->value();
        k->setText(QString("Скорость вращения диска: %1 рад/с").arg(psi_dot));
        });
        set->addWidget(k);
        set->addWidget(s4);
    }
    {
        QLabel *k = new QLabel(QString("Угол наклона от вертикальной оси: %1 рад").arg(phi_dot));
        s5 = new QSlider(Qt::Horizontal); s5->setMinimum(-30); s5->setMaximum(30); s5->setValue(int(phi_dot * 10.));
        connect(s5, &QSlider::valueChanged, [=]()
        {
        phi_dot = double(s5->value()) * 0.1;
        k->setText(QString("Угол наклона от вертикальной оси: %1 рад").arg(phi_dot));
        });
        set->addWidget(k);
        set->addWidget(s5);
    }
    {
        QLabel *k = new QLabel(QString("Начальная скорость прецессии диска: %1 рад/с").arg(theta));
        s6 = new QSlider(Qt::Horizontal); s6->setMinimum(785); s6->setMaximum(1570); s6->setValue(int(theta * 1000));
        connect(s6, &QSlider::valueChanged, [=]()
        {
        theta = double(s6->value()) * 0.001;
        k->setText(QString("Начальная скорость прецессии диска: %1 рад/с").arg(theta));
        });
        set->addWidget(k);
        set->addWidget(s6);
    }




}


void Model2::SetTransform()
{
    QVector3D diskPos = QVector3D(cos(PI / 2 - theta)*sin(phi)*(0.56 + 10 * length),
    sin(PI / 2 - theta)*(0.56 + 10 * length),
    cos(PI / 2 - theta)*cos(phi)*(0.56 + 10 * length));

    nutation = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), theta * toGrad - 90);
    tr1->setRotation(precession * nutation * rotation);
    tr2->setRotation(precession * nutation);

    tr1->setScale3D(QVector3D(radius / 0.8, radius / 0.8, 1.0));
    tr1->setTranslation(diskPos * (length - 0.35) / 0.2);
}

void Model2::lock(bool b)
{
    s1->setEnabled(!b);
    s2->setEnabled(!b);
    s3->setEnabled(!b);
    s4->setEnabled(!b);
    s5->setEnabled(!b);
    s6->setEnabled(!b);
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
    rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0),  toGrad * psi);
    precession = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), toGrad * phi);
    nutation = QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), toGrad * theta - 90);
    tr1->setRotation(precession * nutation * rotation);
    tr2->setRotation(precession * nutation);
    tr3->setRotation(precession);
    SetTransform();

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


    tr1->setScale(0.25);
    tr2->setScale(0.25);
    tr3->setScale(0.25);
    tr4->setScale(0.25);
}

void Model2::Update(double dt)
{

    double delt = double(t->elapsed()) * 1e-3;
    for (double i = 0; i * 1e-5 < delt; ++i)
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
        /*case 0:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetPhi(); },  "Углол Phi");
        break;
        case 1:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetPsi(); }, "Углол Psi");
        break;*/
    }


    if (plot)
    {
        plot->show();
        plots.append(plot);
    }
}




