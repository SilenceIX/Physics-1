#include "models.h"
#include "plot.h"

Model5::Model5()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    Start_angle1 = 0.;
    Start_angle2 = 0.;
    D = 5.;



    LoadModel();
    Transform();

    {
        QLabel *k = new QLabel(QString("Угол отклонения первого маятника: %1 рад").arg(Start_angle1));
        s1 = new QSlider(Qt::Horizontal); s1->setMinimum(-200); s1->setMaximum(200); s1->setValue(int(Start_angle1 * toGrad * 10));
        connect(s1, &QSlider::valueChanged, [=]()
        {
        Start_angle1 = double(s1->value()) / (10 * toGrad);
        k->setText(QString("Угол отклонения первого маятника: %1 рад").arg(Start_angle1));
        angle1 = Start_angle1;
        Transform();
        });
        set->addWidget(k);
        set->addWidget(s1);
    }
    {
        QLabel *k = new QLabel(QString("Угол отклонения второго маятника: %1 рад").arg(Start_angle1));
        s2 = new QSlider(Qt::Horizontal); s2->setMinimum(-200); s2->setMaximum(200); s2->setValue(int(Start_angle2 * toGrad * 10));
        connect(s2, &QSlider::valueChanged, [=]()
        {
        Start_angle2 = double(s2->value()) / (10 * toGrad);
        k->setText(QString("Угол отклонения второго маятника: %1 рад").arg(Start_angle2));
        angle2 = Start_angle2;
        Transform();
        });
        set->addWidget(k);
        set->addWidget(s2);
    }

    {
        QLabel *k = new QLabel(QString("Расстояние положения пружины от точек подвеса маятников: %1 м").arg(D));
        s3 = new QSlider(Qt::Horizontal); s3->setMinimum(100); s3->setMaximum(1000); s3->setValue(int(D * 1000));
        connect(s3, &QSlider::valueChanged, [=]()
        {
        D = double(s3->value()) / 1000;
        k->setText(QString("Расстояние положения пружины от точек подвеса маятников: %1 м").arg(D));
        d = D;
        Transform();
        });
        set->addWidget(k);
        set->addWidget(s3);
    }

    {
        QLabel *l = new QLabel(QString("Жёсткость пружины: %1 Н/м").arg(k));
        s4 = new QSlider(Qt::Horizontal); s4->setMinimum(100); s4->setMaximum(1000); s4->setValue(int(k * 1000));
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
    x1 = -0.22 - sin(angle1) * 0.54;
    x2 = 0.22 - sin(angle2) * 0.54;
    double m = (x1 + x2) / 2.;
    pruz->setScale3D(QVector3D(1.f, 1.f, (x2 - x1) / 0.44));
    pruz->setTranslation(QVector3D(-0.09f, 0.54f, m));
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
    t += dt;
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
    E1 = (m*W*W*Start_angle1*Start_angle1*cos(Wm*t)*cos(Wm*t))/2;
    return E1;
}

double Model5::Get_E2()
{
    E2 = (m*W*W*Start_angle2*Start_angle2*cos(Wm*t)*cos(Wm*t))/2;
    return E2;
}


void Model5::Update(double dt)
{
    Compute(dt);
    Transform();
}

void Model5::CreatePlot(int)
{

}

