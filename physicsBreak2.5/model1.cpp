#include "models.h"
#include "plot.h"
Qt3DCore::QEntity *addObject(Qt3DCore::QEntity *entity, QString obj, QString texture)
{
      Qt3DCore::QEntity *mesh = new Qt3DCore::QEntity(entity);
      Qt3DRender::QMesh *objMesh = new Qt3DRender::QMesh(mesh);
      objMesh->setSource(QUrl::fromLocalFile(obj));

      Qt3DRender::QTextureLoader *tLoader = new Qt3DRender::QTextureLoader(mesh);
      tLoader->setSource(QUrl::fromLocalFile(texture));

      Qt3DExtras::QDiffuseSpecularMapMaterial *Mat = new Qt3DExtras::QDiffuseSpecularMapMaterial();
      Mat->setDiffuse(tLoader);
      Mat->setShininess(8.2f);
      Mat->setAmbient(QColor::fromRgb(255, 255, 255));
      Mat->setSpecular(tLoader);
      mesh->addComponent(objMesh);
      mesh->addComponent(Mat);
      return mesh;
}




Model1::Model1()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    R = 0.0025;
    r = 0.5;
    k = 0.01;
    c = 0;
    m = 1;
    angle = 0.;
    beta = 0.;
    omega = 0.;

    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");

    addObject(ent, ":/Stands/Math1/base.obj", ":/Stands/Math1/full.png");
    Qt3DCore::QEntity *obj1 = addObject(ent, ":/Stands/Math1/palka.obj", ":/Stands/Math1/palka.jpg");
    Qt3DCore::QEntity *obj2 = addObject(ent, ":/Stands/Math1/shar1.obj", ":/Stands/Math1/sharik.jpg");
    Qt3DCore::QEntity *obj3 = addObject(ent, ":/Stands/Math1/shar2.obj", ":/Stands/Math1/sharik.jpg");

    tr1 = new Qt3DCore::QTransform();
    tr2 = new Qt3DCore::QTransform();
    tr3 = new Qt3DCore::QTransform();
    obj1->addComponent(tr1);
    obj2->addComponent(tr2);
    obj3->addComponent(tr3);



    i1 = new QLabel("Угол отклонения: 0.0 рад");
    inf->addWidget(i1);

    k1 = new QLabel("Начальный угол отклонения: 0.0 рад");
    k2 = new QLabel("Коэффициент сопротивления: 0.0");
    k3 = new QLabel("Циклическая частота: 0.02 рад/c");
    k4 = new QLabel("Расстояние до шара: 0.1 м");
    k5 = new QLabel("Коэффициент жесткости: 0.5");
    k6 = new QLabel("Масса: 1 кг");


    s1 = new QSlider(Qt::Horizontal); s1->setMinimum(0); s1->setMaximum(int(PI * 500.));s1->setValue(0);
    connect(s1, &QSlider::valueChanged, [=]()
    {
        this->angle = double(s1->value()) / 1000.;
        A0 = angle;
        k1->setText(QString("Начальный угол отклонения: %1 рад").arg(angle));
        this->Transform();
    });
    s2 = new QSlider(Qt::Horizontal); s2->setMinimum(0); s2->setMaximum(500);s2->setValue(0);
    connect(s2, &QSlider::valueChanged, [=]()
    {
        this->c = double(s2->value())/100;
        k2->setText(QString("Коэффициент сопротивления: %1").arg(c));
    });
    s4 = new QSlider(Qt::Horizontal); s4->setMinimum(50); s4->setMaximum(500); s4->setValue(500);
    connect(s4, &QSlider::valueChanged, [=]()
    {
        this->r = double(s4->value()) / 1000.;
        k4->setText(QString("Расстояние до шара: %1 м").arg(r));
        this->Transform();
    });
    s5 = new QSlider(Qt::Horizontal); s5->setMinimum(1); s5->setMaximum(100);s5->setValue(1);
    connect(s5, &QSlider::valueChanged, [=]()
    {
        this->k = double(s5->value())/100;
        k5->setText(QString("Коэффициент жесткости: %1").arg(k));
        this->Transform();
    });
    s6 = new QSlider(Qt::Horizontal); s6->setMinimum(1); s6->setMaximum(2000);s6->setValue(1000);
    connect(s6, &QSlider::valueChanged, [=]()
    {
        this->m = double(s6->value()) / 1000.;
        k6->setText(QString("Масса: %1 кг").arg(m));
        this->Transform();
    });
    QPushButton *p1 = new QPushButton("Построить график перемещения");
    connect(p1, &QPushButton::clicked, [=]()
    {
        this->CreatePlot(0);
    });


    set->addWidget(k1);
    set->addWidget(s1);
    set->addWidget(k6);
    set->addWidget(s6);
    set->addWidget(k4);
    set->addWidget(s4);
    set->addWidget(k2);
    set->addWidget(s2);
    set->addWidget(k5);
    set->addWidget(s5);

    set->addWidget(p1);




}

void Model1::Init()
{
    t = 0.;
}

void Model1::Compute()
{
    J = 2*(2*m*R*R/5 + m*r*r);
    omega = sqrt(k/J);
    beta = c/(4*sqrt(m*k)*J);
    angle = A0 * pow(e, -beta * t) * cos(omega * t);
}

void Model1::Transform()
{
   tr1->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), float(angle * 180. / PI)));
   tr2->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), float(angle * 180. / PI)));
   tr3->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0, 1.0, 0.0), float(angle * 180. / PI)));
}

void Model1::Update(double dt)
{
    t+=dt;
    Compute();
    Transform();

    for (auto plot : plots)
        if (plot->GetState() == Plot::State::Active)
            plot->Update();
        else
        {
            plot->Destroy();
            plots.removeOne(plot);
        }

    i1->setText(QString("Угол отклонения: %1 рад/c").arg(angle));
}

void Model1::CreatePlot(int plotID)
{
    Plot *plot = nullptr;

    switch (plotID)
    {
        case 0:
            plot = new Plot([this]()->double{ return this->GetTime(); },
                            [this]()->double{ return this->GetAngle(); }, "Угловое смещение, рад");
        break;
    }

    if (plot)
    {
        plot->show();
        plots.append(plot);
    }
}

Qt3DCore::QEntity *Model1::GetEntity()
{
    return ent;
}

double Model1::GetA()
{
    return A0;
}

double Model1::GetBeta()
{
    return beta;
}

double Model1::GetOmega()
{
    return omega;
}

double Model1::GetAngle()
{
    return angle;
}

QVBoxLayout *Model1::GetSet()
{
    return set;
}

QVBoxLayout *Model1::GetInf()
{
    return inf;
}

double Model1::GetTime()
{
    return t;
}

