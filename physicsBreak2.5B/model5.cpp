#include "models.h"
#include "plot.h"

Model5::Model5()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    angle1 = 0.;
    angle2 = 0.;
    E1 = 0.;
    E2 = 0.;
    Start_angle1 = 0.;
    Start_angle2 = 0.;
    D = 4.5;
    d = 4.5;
    k = 1.;

    LoadModel();
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

    i1 = new QLabel(QString("Угол откл. певрого маятника: %1 рад").arg(angle1));
    i2 = new QLabel(QString("Угол откл. второго маятника: %1 рад").arg(angle2));
    i3 = new QLabel(QString("Энергия первого маятника: %1 Дж").arg(E1));
    i4 = new QLabel(QString("Энергия второго маятника: %1 Дж").arg(E2));
    inf->addWidget(i1);
    inf->addWidget(i2);
    inf->addWidget(i3);
    inf->addWidget(i4);

    {
        QLabel *km = new QLabel(QString("Угол откл. первого маятника: %1 рад").arg(Start_angle1));
        s1 = new QSlider(Qt::Horizontal); s1->setMinimum(-120); s1->setMaximum(120); s1->setValue(int(Start_angle1 * toGrad * 10));
        connect(s1, &QSlider::valueChanged, [=]()
        {
        Start_angle1 = double(s1->value()) / (10 * toGrad);
        km->setText(QString("Угол откл. первого маятника: %1 рад").arg(Start_angle1));
        angle1 = Start_angle1;
        Transform();
        });
        set->addWidget(km);
        set->addWidget(s1);
    }
    {
        QLabel *km = new QLabel(QString("Угол откл. второго маятника: %1 рад").arg(Start_angle1));
        s2 = new QSlider(Qt::Horizontal); s2->setMinimum(-120); s2->setMaximum(120); s2->setValue(int(Start_angle2 * toGrad * 10));
        connect(s2, &QSlider::valueChanged, [=]()
        {
        Start_angle2 = double(s2->value()) / (10 * toGrad);
        km->setText(QString("Угол откл. второго маятника: %1 рад").arg(Start_angle2));
        angle2 = Start_angle2;
        Transform();
        });
        set->addWidget(km);
        set->addWidget(s2);
    }

    {
        QLabel *km = new QLabel(QString("Расстояние положения пружины от точек подвеса маятников: %1 м").arg(D));
        km->setWordWrap(true);
        s3 = new QSlider(Qt::Horizontal); s3->setMinimum(100); s3->setMaximum(4500); s3->setValue(int(D * 1000));
        connect(s3, &QSlider::valueChanged, [=]()
        {
        D = double(s3->value()) / 1000;
        km->setText(QString("Расстояние положения пружины от точек подвеса маятников: %1 м").arg(D));
        d = D;
        Transform();
        });
        set->addWidget(km);
        set->addWidget(s3);
    }

    {
        QLabel *l = new QLabel(QString("Жёсткость пружины: %1 Н/м").arg(k));
        s4 = new QSlider(Qt::Horizontal); s4->setMinimum(100); s4->setMaximum(5000); s4->setValue(int(k * 1000));
        connect(s4, &QSlider::valueChanged, [=]()
        {
        k = double(s4->value()) / 1000;
        l->setText(QString("Жёсткость пружины: %1 Н/м").arg(k));
        });
        set->addWidget(l);
        set->addWidget(s4);
    }



}


void Model5::Transform()
{
    tr1->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), float(angle1 * toGrad)));
    tr2->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), float(angle2 * toGrad)));

    double x1, x2;
    x1 = -0.22 - sin(angle1) * (0.54 - 0.3 * (4.5 - d) / 4.4);
    x2 = 0.22 - sin(angle2) * (0.54 - 0.3 * (4.5 - d) / 4.4);
    double m = (x1 + x2) / 2.;
    pruz->setScale3D(QVector3D(1.f, 1.f, float((x2 - x1) / 0.44)));
    pruz->setTranslation(QVector3D(-0.09f, float(0.54 + 0.3 * (4.5 - d) / 4.4), float(m)));
    //pruz->setScale3D(QVector3D())
}

void Model5::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");

    addObject(ent, ":/Stands/Math5/ospol.obj", ":/Stands/Math5/wood.jpg");
    addObject(ent, ":/Stands/Math5/pstick.obj", ":/Stands/Math5/pstick.jpg");
    addObject(ent, ":/Stands/Math5/wall.obj", ":/Stands/Math5/wall.jpg");
    Qt3DCore::QEntity *pr = addObject(ent, ":/Stands/Math5/prus.obj", ":/Stands/Math5/prus.jpg");
    Qt3DCore::QEntity *s1 = addObject(ent, ":/Stands/Math5/st1.obj", ":/Stands/Math5/ster.png");
    Qt3DCore::QEntity *s2 = addObject(ent, ":/Stands/Math5/st2.obj", ":/Stands/Math5/ster.png");



    tr1 = new Qt3DCore::QTransform();
    tr2 = new Qt3DCore::QTransform();
    pruz = new Qt3DCore::QTransform();
    s1->addComponent(tr1);
    s2->addComponent(tr2);
    pr->addComponent(pruz);
    tr1->setTranslation(QVector3D(-0.09f, 1.08f, -0.22f));
    tr2->setTranslation(QVector3D(-0.09f, 1.08f, 0.22f));
    pruz->setTranslation(QVector3D(-0.09f, 0.54f, 0.0f));


}


void Model5::Init()
{
    d = D;
    w1 = sqrt(g/l);
    t =  0;
}

void Model5::Compute(double dt)
{
    Func(dt);
}

double Model5::FuncW2()
{
    double c=2*k*d*d;
    double z=m*l*l;
    double w2 = sqrt(g/l+c/z);
    return w2;
}

void Model5::Func(double dt)
{
    w2 = FuncW2();
    double F1 = (Start_angle1 + Start_angle2)*cos(w1*t);
    double F2 = (Start_angle1 - Start_angle2)*cos(w2*t);
    angle1 = (F1+F2)/2;
    angle2 = (F1-F2)/2;
    Get_W_Wm();
    E1 = (m*W*W*Start_angle1*Start_angle1*cos(Wm*t)*cos(Wm*t))/2;
    E2 = (m*W*W*Start_angle2*Start_angle2*cos(Wm*t)*cos(Wm*t))/2;
}



double Model5::Get_Angle1()
{
    return angle1;
}

double Model5::Get_Angle2()
{
    return angle2;
}

void Model5::Get_W_Wm()
{
    W= (w1+w2)/2;
    Wm= (w1-w2)/2;
}

double Model5::Get_E1()
{
    return E1;
}

double Model5::Get_E2()
{
    return E2;
}

void Model5::Update_plot(double dt, int maxtime)
{
    double st = t;
    Init();
    for (int i=0;i<maxtime;i++){
        for (int j=0;j<timesPrint;++j)
        {
            t += dt;
            Compute(dt);
        }
        for (auto plot : plots)
            plot->Update();
    }
    t = st;
}


void Model5::Update(double dt)
{
    t += dt;
    Compute(dt);
    Transform();

    if (!cGraf->checkState() && (int64_t(t * 1000) % timesPrint == 0))
        for (auto plot : plots)
            if (plot->GetState() == Plot::State::Active)
                plot->Update();
            else
            {
                plot->Destroy();
                plots.removeOne(plot);
            }

    if (int (t * 100) % 10 == 0)
    {
        i1->setText(QString("Угол отклонения певрого маятника: %1 рад").arg(angle1));
        i2->setText(QString("Угол отклонения второго маятника: %1 рад").arg(angle2));
        i3->setText(QString("Энергия первого маятника: %1 Дж").arg(E1));
        i4->setText(QString("Энергия второго маятника: %1 Дж").arg(E2));
    }
}

void Model5::CreatePlot(int plotID)
{
    Plot *plot = nullptr;
    double st = t;
    Init();
    switch (plotID)
    {
    case 0:{
    YSize= angle1;
    plot = new Plot([this]()->double{ return this->t; },
                        [this]()->double{ return this->angle1; }, "График отклонения первого маятника, рад",abs(YSize));

    break;}
    case 1:{
    YSize= angle2;
    plot = new Plot([this]()->double{ return this->t; },
                        [this]()->double{ return this->angle2; }, "График отклонения второго маятника, рад",abs(YSize));

    break;}
    case 2:{
    YSize= 1.;
    plot = new Plot([this]()->double{ return this->t; },
                        [this]()->double{ return this->E1; }, "Энергия первого маятника, Дж",abs(YSize));

    break;}
    case 3:{
    YSize= 1.;
    plot = new Plot([this]()->double{ return this->t; },
                        [this]()->double{ return this->E2; }, "Энергия второго маятника, Дж",abs(YSize));

    break;}
    case 4:{
    YSize= 1.;
    plot = new Plot([this]()->double{ return this->t; },
                        [this]()->double{ return this->E1 + this->E2; }, "Полная энергия, Дж",abs(YSize));

    break;}
    }
    t = st;
    if (plot)
    {
        plot->show();
        plots.append(plot);
    }
}

void Model5::GetMenu(QMenu *m)
{
    QMenu *a1 = new QMenu("Графики энергии", m);
    QAction *a1_1 = new QAction("Энергия первого маятника", a1);
    QAction *a1_2 = new QAction("Энергия второго маятника", a1);
    QAction *a1_3 = new QAction("Полная энергия", a1);

    m->addMenu(a1);
    a1->addAction(a1_1);
    a1->addAction(a1_2);
    a1->addAction(a1_3);

    QMenu *a2 = new QMenu("Графики системы", m);
    QAction *a2_1 = new QAction("График отклонения первого маятника", a2);
    QAction *a2_2 = new QAction("График отклонения второго маятника", a2);

    m->addMenu(a2);
    a2->addAction(a2_1);
    a2->addAction(a2_2);

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
}

