#include "models.h"
#include "plot.h"

Model5::Model5()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();


    LoadModel();
    Transform();
}


void Model5::Transform()
{
    tr1->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), float(angle1 * 90. / PI)));
    tr2->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0, 0.0, 0.0), float(angle2 * 90. / PI)));

    double x1, x2;
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
    tr1->setTranslation(QVector3D(-0.09, 1.08, -0.22));
    tr2->setTranslation(QVector3D(-0.09, 1.08, 0.22));
    pruz->setTranslation(QVector3D(-0.09, 0.54, 0.0));


}


void Model5::Init()
{
    d = D;
    st_angle1 = Start_angle1;
    st_angle2 = Start_angle2;
    t =  0;
}

double Model5::FuncW2()
{
    double c=2*k*d*d;
    double z=m*l*l;
    double w2 = sqrt(g/l+c/z);
    return w2;
}

void Model5::Compute(double dt)
{
    double w2 = FuncW2();
    double F1 = (st_angle1 + st_angle2)*cos(w1*t);
    double F2 = (st_angle1 - st_angle2)*cos(w2*t);
    angle1 = (F1+F2)/2;
    angle2 = (F1-F2)/2;
    t += dt;
}

void Model5::Update(double dt)
{
    Compute(dt);
    Transform();
}

void Model5::CreatePlot(int)
{

}

