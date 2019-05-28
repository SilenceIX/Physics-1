#include "models.h"
#include "plot.h"

Model6::Model6()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    LoadModel();

    Theta0 = 0;
    dtheta0 = PI/3.;
    m_st0 = 2; //масса стержня
    m_d0=5; //масса диска
    l_st0=5; //длина стержня
    r1_0 = 0.5; //расстояние от оси вращения до диска, который перемещается
    r2_0 = 4.8; //расстояние от оси вращения до неподвижного диска
    set->addWidget(new QLabel(QString("Масса стрежня: %1 кг").arg(m_st0)));
    set->addWidget(new QLabel(QString("Длинна стержня: %1 м").arg(l_st0)));
    QLabel *a1 = new QLabel(QString("Расстояние от оси вращения до неподвижного диска: %1 м").arg(r2_0));
    a1->setWordWrap(true);
    set->addWidget(a1);

    {
        QLabel *k = new QLabel(QString("Начальный угол отклонения: %1 рад").arg(Theta0));
        s1 = new QSlider(Qt::Horizontal); s1->setMinimum(-450); s1->setMaximum(450); s1->setValue(int(Theta0 * toGrad * 10));
        connect(s1, &QSlider::valueChanged, [=]()
        {
        Theta0 = double(s1->value()) / (10 * toGrad);
        k->setText(QString("Начальный угол отклонения: %1 рад").arg(Theta0));
        Theta = Theta0;
        Transform();
        });
        set->addWidget(k);
        set->addWidget(s1);
    }

    {
        QLabel *k = new QLabel(QString("Начальная угловая скорость: %1 рад/c").arg(dtheta0));
        s2 = new QSlider(Qt::Horizontal); s2->setMinimum(-450); s2->setMaximum(450); s2->setValue(int(dtheta0 * toGrad * 10));
        connect(s2, &QSlider::valueChanged, [=]()
        {
        dtheta0 = double(s2->value()) / (10 * toGrad);
        k->setText(QString("Начальная угловая скорость: %1 рад/c").arg(dtheta0));
        });
        set->addWidget(k);
        set->addWidget(s2);
    }

    {
        QLabel *k = new QLabel(QString("Масса дисков: %1 кг").arg(m_d0));
        s3 = new QSlider(Qt::Horizontal); s3->setMinimum(10); s3->setMaximum(100); s3->setValue(int(m_d0 * 100.));
        connect(s3, &QSlider::valueChanged, [=]()
        {
        m_d0 = double(s3->value()) / 100;
        k->setText(QString("Масса дисков: %1 кг").arg(m_d0));
        });
        set->addWidget(k);
        set->addWidget(s3);
    }

    {
        QLabel *k = new QLabel(QString("Расстояние от оси вращения до диска, который перемещается: %1 м").arg(r1_0));
        k->setWordWrap(true);
        s4 = new QSlider(Qt::Horizontal); s4->setMinimum(5); s4->setMaximum(35); s4->setValue(int(r1_0 * 10.));
        connect(s4, &QSlider::valueChanged, [=]()
        {
        r1_0 = double(s4->value()) / 10;
        k->setText(QString("Расстояние от оси вращения до диска, который перемещается: %1 м").arg(r1_0));
        Transform();
        });
        set->addWidget(k);
        set->addWidget(s4);
    }



}


void Model6::Transform()
{
    tr1->setRotationX(float(Theta * toGrad));
    //tr2->setRotationX(float(Theta * toGrad));
    auto m = tr2->rotateAround(QVector3D(0.34f, 1.535f, 0.), float(Theta * toGrad), QVector3D(1.f, 0., 0.));
    m.translate(QVector3D(0.34f, float(1.438 - 0.72 * (r1_0 - 0.5) / (3.5)), 0.));
    tr2->setMatrix(m);
}

void Model6::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");

    addObject(ent, ":/Stands/Math6/stand.obj", ":/Stands/Math6/stand_03.png");
    Qt3DCore::QEntity *rod = addObject(ent, ":/Stands/Math6/rod.obj", ":/Stands/Math6/rod.png");
    Qt3DCore::QEntity *load = addObject(ent, ":/Stands/Math6/load.obj", ":/Stands/Math6/load.png");

    tr1 = new Qt3DCore::QTransform();
    tr2 = new Qt3DCore::QTransform();
    rod->addComponent(tr1);
    load->addComponent(tr2);

    tr1->setTranslation(QVector3D(0.34f, 1.535f, 0.));
    tr2->setTranslation(QVector3D(0.34f, 1.438f, 0.));
}


void Model6::Init()
{
    Theta = Theta0;
    dtheta = dtheta0;
    m_st = m_st0; //масса стержня
    m_d = m_d0; //масса диска
    m = m_st + m_d; //масса маятника
    l_st = l_st0; //длина стержня
    r1 = r1_0; //расстояние от оси вращения до диска, который перемещается
    r2 = r2_0; //расстояние от оси вращения до неподвижного диска
    d = ( (r1+ r2)*m_d + 0.5*l_st*m_st ) / m; //расстояние от центра масс до точки подвеса (начало координат)
    //Подсчет момента инерции маятника
    Ist = m_st * (l_st*l_st / 12 + d * d); //стержня
    Id1 = 0.5*m_d * r1*r1; // дисков
    Id2 = 0.5*m_d * r2*r2;
    I = Ist + Id1 + Id2; //маятника
    C = -m * g*d*I; //просто константа для удобства
}




void Model6::Compute(double h)
{
    h /= 10;
    K[0] = h * (dtheta);
    L[0] = C * h * sin(Theta);
    K[1] = h * (dtheta + L[0] / 2);
    L[1] = C * h * sin(Theta + K[0] / 2);
    K[2] = h * (dtheta + L[1] / 2);
    L[2] = C * h * sin(Theta + K[1] / 2);
    K[3] = h * (dtheta + L[2]);
    L[3] = C * h * sin(Theta + K[2]);

    dtheta = dtheta + (L[0] + 2 * L[1] + 2 * L[2] + L[3]) / 6;
    Theta = Theta + (K[0] + 2 * K[1] + 2 * K[2] + K[3]) / 6;
}

void Model6::Update(double h)
{
    Compute(h);
    Transform();
}

void Model6::CreatePlot(int)
{

}

