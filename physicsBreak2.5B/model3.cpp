#include "models.h"
#include "plot.h"

void Model3::Transform()
{
    tr1->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), float(A * toGrad)));
    auto mat = tr2->rotateAround(QVector3D(0., 1.6f, 0.), float(A * toGrad), QVector3D(1.0, 0.0, 0.0));
    mat.translate(0.f, float(-0.0665 * sL + 1.1676), 0.f);
    if (sr > 0.)
        mat.scale(float(0.2 * sr));
    tr2->setMatrix(mat);
    //tr2->setTranslation(QVector3D(0., float(0.6), 0.));
    tr3->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), float(A * toGrad)));
    tr4->setRotationX(float(A * toGrad));
    //tr4->setMatrix(tr4->rotateAround(QVector3D(0., 0.65f, 0.), float(A * toGrad), QVector3D(1.0, 0.0, 0.0)));
}
void Model3::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");

    addObject(ent, ":/Stands/Math3/podstavka.obj", ":/Stands/Math3/stand.jpg");
    addObject(ent, ":/Stands/Math3/transportir.obj", ":/Stands/Math3/transp.jpg");
    Qt3DCore::QEntity *stick = addObject(ent, ":/Stands/Math3/stick.obj", ":/Stands/Math3/treeball.jpg");
    Qt3DCore::QEntity *ball = addObject(ent, ":/Stands/Math3/treeball.obj", ":/Stands/Math3/treeball.jpg");
    Qt3DCore::QEntity *st = addObject(ent, ":/Stands/Math3/stpen.obj", ":/Stands/Math3/treeball.jpg");
    Qt3DCore::QEntity *arr = addObject(ent, ":/Stands/Math3/arrow.obj", ":/Stands/Math3/text.jpg");

    tr1 = new Qt3DCore::QTransform();
    tr2 = new Qt3DCore::QTransform();
    tr3 = new Qt3DCore::QTransform();
    tr4 = new Qt3DCore::QTransform();


    st->addComponent(tr1);
    ball->addComponent(tr2);
    stick->addComponent(tr3);
    arr->addComponent(tr4);
    tr1->setTranslation(QVector3D(0.0, 1.6f, 0.f));
    tr2->setTranslation(QVector3D(0.0, 0.6f, 0.f));
    tr3->setTranslation(QVector3D(0.0, 1.6f, 0.0));
    tr4->setTranslation(QVector3D(0.0, 0.653f, 0.0));
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

    tr1->setScale3D(QVector3D(1., float(0.4 + 0.06 * sL), 1.));
    Transform();


    QLabel *lGraf = new QLabel(QString("Количетсво значений: %1").arg(500));
    sGraf = new QSlider(Qt::Horizontal); sGraf->setMinimum(50); sGraf->setMaximum(15000); sGraf->setValue(500);
    cGraf = new QCheckBox("Моментальное построение графиков");
    connect(sGraf, &QSlider::valueChanged, [=](int d){
        lGraf->setText(QString("Количечетсво значений: %1").arg(d));
    });
    cGraf->setCheckState(Qt::Checked);
    connect(cGraf, &QCheckBox::stateChanged, [=](int k){
        if (k == 0)
            sGraf->setEnabled(false);
        else
            sGraf->setEnabled(true);
    });
    inf->addWidget(cGraf);
    inf->addWidget(lGraf);
    inf->addWidget(sGraf);


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
    s2 = new QSlider(Qt::Horizontal); s2->setMinimum(100); s2->setMaximum(10000); s2->setValue(int(sL * 1000.));
    connect(s2, &QSlider::valueChanged, [=]()
    {
        this->sL = double(s2->value()) / 1000.;
        k2->setText(QString("Длина стержня: %1 м").arg(sL));
        tr1->setScale3D(QVector3D(1., float(0.4 + 0.06 * sL), 1.));
        Transform();

    });

    s3 = new QSlider(Qt::Horizontal); s3->setMinimum(1000); s3->setMaximum(10000); s3->setValue(int(sr * 1000.));
    connect(s3, &QSlider::valueChanged, [=]()
    {
        this->sr = double(s3->value()) / 1000.;
        k3->setText(QString("Радиус шара: %1 м").arg(sr));
        Transform();
    });
    s4 = new QSlider(Qt::Horizontal); s4->setMinimum(int(- PI * 10000.)); s4->setMaximum(int(PI * 10000.)); s4->setValue(int(sA * 10000.));
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

    QCheckBox *ch = new QCheckBox("Учитывать лобовое сопротивление воздуха");
    ch->setCheckState(Qt::Checked);
    connect(ch, &QCheckBox::stateChanged, [=](int k){
        if (k == 0)
        {
            sr = 0.;
            s3->setEnabled(false);
        } else
        {
            sr = double(s3->value()) / 1000.;
            s3->setEnabled(true);
        }
        Transform();
    });
    set->addWidget(ch);




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

    if (!cGraf->checkState() && (int64_t(time * 1000) % timesPrint == 0))
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
void Model3::Update_plot(double dt, int maxtime)
{
    double sa = this->GetA();
    double sw = this->GetW();
    Init();
    for (int i=0;i<maxtime;i++){
        for (int j=0;j<timesPrint;++j)
        {
            time += dt;
            Compute(dt);
        }
        for (auto plot : plots)
            plot->Update();
    }
    A = sa;
    W = sw;
}

void Model3::lock(bool b)
{
    set->setEnabled(!b);
}


void Model3::CreatePlot(int plotID)
{
    this->Init();
    Plot *plot = nullptr;

    switch (plotID)
    {
        case 0:{
        YSize=this->A;
        plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetA(); }, "Угловое смещение, рад",abs(YSize));

        break;}
    case 1:{
        Compute(0.05);
        YSize=this->W;
        this->Init();
        plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetW(); }, "Угловая скорость, рад/c",abs(YSize));

            break;}
    case 2:{
        YSize=this->M * g * this->L * (1 - cos(this->A));
        plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetEp(); }, "Потенциальная энергия, Дж",abs(YSize));
            break;}
    case 3:{
            Compute(0.05);

            YSize = this->M * this->W * this->W * this->L * this->L / 2;
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetEk(); }, "Кинетическая энергия, Дж",abs(YSize));

            break;}
    case 4:
    Compute(0.05);
    YSize=this->M * g * this->L * (1 - cos(this->A)) + this->M * this->W * this->W * this->L * this->L / 2;
        plot = new Plot([this]()->double{ return this->GetTime(); },
                        [this]()->double{ return this->GetEnergy(); }, "Полная энергия, Дж",abs(YSize));

    break;
    case 5:
    Compute(0.05);
    YSize=this->M * g * this->L * (1 - cos(this->A)) + this->M * this->W * this->W * this->L * this->L / 2;
        plot = new Plot([this]()->double{ return this->GetTime(); },
                        [this]()->double{ return this->GetEnergy(); }, "Угловое ускорение, рад/с^2",abs(YSize));

    break;
    case 6:
    YSize=this->L * 5;
        plot = new Plot([this]()->double{ return sqrt(L / M); },
                        [this]()->double{ return this->GetW(); }, "Угловая скорость, рад/c",abs(YSize));

    break;
    }


    if (plot)
    {
        plot->show();
        plots.append(plot);
    }
}
void Model3::GetMenu(QMenu *m)
{

    QMenu *a1 = new QMenu("Графики энергии", m);
    QAction *a1_1 = new QAction("Потенциальная энергия", a1);
    QAction *a1_2 = new QAction("Кинетическая энергия", a1);
    QAction *a1_3 = new QAction("Полная энергия", a1);

    m->addMenu(a1);
    a1->addAction(a1_1);
    a1->addAction(a1_2);
    a1->addAction(a1_3);

    QMenu *a2 = new QMenu("Графики системы", m);
    QAction *a2_1 = new QAction("Угловое отклонение", a2);
    QAction *a2_2 = new QAction("Угловая скорость", a2);
    QAction *a2_3 = new QAction("Угловое ускорение", a2);
    QAction *a2_4 = new QAction("Отношение угловой скорости к квадрату длинны на массу", a2);

    m->addMenu(a2);
    a2->addAction(a2_1);
    a2->addAction(a2_2);
    a2->addAction(a2_3);
   // a2->addAction(a2_4);

    connect(a1_1, &QAction::triggered, [=](){
        this->CreatePlot(2);
        if (cGraf->checkState())
            this->Update_plot(0.001,sGraf->value());
    });
    connect(a1_2, &QAction::triggered, [=](){
        this->CreatePlot(3);
        if (cGraf->checkState())
            this->Update_plot(0.001,sGraf->value());
    });
    connect(a1_3, &QAction::triggered, [=](){
        this->CreatePlot(4);
        if (cGraf->checkState())
            this->Update_plot(0.001,sGraf->value());
    });

    connect(a2_1, &QAction::triggered, [=](){
        this->CreatePlot(0);
        if (cGraf->checkState())
            this->Update_plot(0.001,sGraf->value());
    });
    connect(a2_2, &QAction::triggered, [=](){
        this->CreatePlot(1);
        if (cGraf->checkState())
            this->Update_plot(0.001,sGraf->value());
    });
    connect(a2_3, &QAction::triggered, [=](){
        this->CreatePlot(5);
        if (cGraf->checkState())
            this->Update_plot(0.001,sGraf->value());
    });
    connect(a2_4, &QAction::triggered, [=](){
        this->CreatePlot(6);
        if (cGraf->checkState())
            this->Update_plot(0.001,sGraf->value());
    });
}

