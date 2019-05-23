#include "models.h"
#include "plot.h"

Model7::Model7()
{
    A0 = 1.0;   // Амплитуда
    h = 0.4;    //высота планки
    m = 0.5;    //масса груза
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    LoadModel();

    tr1->setTranslation(QVector3D(0., 0.1 + h * 1.4, 0.));


    {
        QLabel *k = new QLabel(QString("Начальный угол отклонения: %1 рад").arg(A0));
        QSlider *s = new QSlider(Qt::Horizontal); s->setMinimum(-PI * 100. / 6.); s->setMaximum(PI * 100./ 6.); s->setValue(int(A0 * 100.));
        connect(s, &QSlider::valueChanged, [=]()
        {
        A0 = double(s->value()) * 0.01;
        angle = A0;
        k->setText(QString("Начальный угол отклонения: %1 рад").arg(A0));
        Transform();
        });
        set->addWidget(k);
        set->addWidget(s);
    }
    {
        QLabel *k = new QLabel(QString("Высота планки: %1 м").arg(h));
        QSlider *s = new QSlider(Qt::Horizontal); s->setMinimum(20); s->setMaximum(80); s->setValue(int(h * 100.));
        connect(s, &QSlider::valueChanged, [=]()
        {
        h = double(s->value()) * 0.01;
        k->setText(QString("Высота планки: %1 м").arg(h));
        tr1->setTranslation(QVector3D(0., 0.1 + h * 1.4, 0.));
        g1->setScale3D(QVector3D(1., 1. - h, 1.));
        g2->setScale3D(QVector3D(1., 1. + h, 1.));
        });
        set->addWidget(k);
        set->addWidget(s);
    }
    {
        QLabel *k = new QLabel(QString("Масса груза: %1 кг").arg(m));
        QSlider *s = new QSlider(Qt::Horizontal); s->setMinimum(50); s->setMaximum(1000); s->setValue(int(m * 100.));
        connect(s, &QSlider::valueChanged, [=]()
        {
        m = double(s->value()) * 0.01;
        k->setText(QString("Масса груза: %1 кг").arg(m));
        });
        set->addWidget(k);
        set->addWidget(s);
    }

}

void Model7::Compute_left()
{
    angle = A0 * cos(W * t);
    x = -l * sin(angle);
    y = -l * cos(angle);
    velocity = -A0 * W * sin(W * t);
    Ek = m * pow(velocity, 2) / 2;
    Ep = m * g * pow(angle, 2) / (2 * l);
    E = Ek + Ep;
}

// Движение "справа", сжатый маятник
void Model7::Compute_right()
{
    k = l / h;
    W1 = sqrt(g * k / l);
    angle = A0 * sqrt(k) * cos(W1 * t);
    x = -l / k * sin(angle);
    y = -l / k * cos(angle) - l * (1 - 1 / k);
    velocity = -A0 * sqrt(k) * W1 * sin(W1 * t);
    Ek = m * pow(velocity, 2) / 2;
    Ep = m * g * pow(angle, 2) / (2 * h);
    E = Ek + Ep;
}


void Model7::Transform()
{
    if (angle < 0)
    {
        g1->setRotationX(angle * toGrad);
        g2->setMatrix(QMatrix4x4());
        g2->setTranslation(QVector3D(0., 1.443, 0.));
        g2->setRotationX(angle * toGrad);
        gq->setRotationX(angle * toGrad);
    }
    else
    {
        g1->setRotationX(0.);
        auto matrix = g2->matrix();
        matrix.setToIdentity();
        matrix.translate(QVector3D(0., 0.815, 0.));
        matrix.rotate(angle * toGrad, QVector3D(1., 0., 0.));
        matrix.translate(QVector3D(0., 0.628, 0.));
        g2->setMatrix(matrix);
        gq->setMatrix(matrix);
        //g2->setMatrix(g2->rotateAround(QVector3D(0., -9., 0.), angle * toGrad, QVector3D(1., 0., 0.)));
        //g2->rot
        //g2->setTranslation(QVector3D(0., 1.443, 0.));
    }
}

void Model7::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");

    addObject(ent, ":/Stands/Math7/base2.obj", ":/Stands/Math7/metal.jpg");
    Qt3DCore::QEntity *p1 = addObject(ent, ":/Stands/Math7/cube.obj", ":/Stands/Math7/metal.jpg");
    Qt3DCore::QEntity *p2 = addObject(ent, ":/Stands/Math7/planka.obj", ":/Stands/Math7/metal.jpg");
    Qt3DCore::QEntity *q = addObject(ent, ":/Stands/Math7/sphere.obj", ":/Stands/Math7/metal.jpg");
    Qt3DCore::QEntity *q1 = addObject(ent, ":/Stands/Math7/up_rope.obj", ":/Stands/Math7/rope.jpg");
    Qt3DCore::QEntity *q2 = addObject(ent, ":/Stands/Math7/down_rope.obj", ":/Stands/Math7/rope.jpg");


    tr1 = new Qt3DCore::QTransform();
    g1 = new Qt3DCore::QTransform();
    g2 = new Qt3DCore::QTransform();
    gq = new Qt3DCore::QTransform();
    p1->addComponent(tr1);
    p2->addComponent(tr1);
    q->addComponent(gq);
    q1->addComponent(g1);
    q2->addComponent(g2);



    tr1->setTranslation(QVector3D(0., 0.681, 0.));
    g1->setTranslation(QVector3D(0., 1.443, 0.));
    g2->setTranslation(QVector3D(0., 1.443, 0.));
    gq->setTranslation(QVector3D(0., 1.443, 0.));

}


void Model7::Init()
{
    t = 0.;
    W = sqrt(g / l);
    T_left = 2 * M_PI * sqrt(l / g);           //периоды при движении после столкновения
    T_right = 2 * M_PI * sqrt(h / g);          //вправо и влево
    t1 = T_right / 4;
    t2 = 3 * T_left / 4;
}

void Model7::Compute(double dt)
{
    t+=dt;
        if (x <= 0.0)
        {
             Compute_left();
        }
        else
        {
            if (t1 < 3 * T_right / 4)
            {
                Compute_right();
                t1 += dt;
            }
            else
            {
                if (t2 < T_left)
                {
                    Compute_left();
                    t2+=dt;
                }
                else {
                    t1 = T_right / 4;
                    t2 = 3 * T_left / 4;
                }
            }
        }
}

void Model7::Update(double dt)
{
    Compute(dt);
    Transform();
}

void Model7::CreatePlot(int)
{

}

