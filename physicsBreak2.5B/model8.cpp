#include "models.h"
#include "plot.h"

Model8::Model8()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    LoadModel();


}



void Model8::Transform()
{

}

void Model8::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");

    /*addObject(ent, ":/Stands/Math7/base2.obj", ":/Stands/Math7/metal.jpg");
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
    q2->addComponent(g2);*/



   /* tr1->setTranslation(QVector3D(0., 0.681, 0.));
    g1->setTranslation(QVector3D(0., 1.443, 0.));
    g2->setTranslation(QVector3D(0., 1.443, 0.));
    gq->setTranslation(QVector3D(0., 1.443, 0.));*/

}


void Model8::Init()
{

}

void Model8::Compute(double dt)
{

}

void Model8::Update(double dt)
{
    Compute(dt);
    Transform();
}

void Model8::CreatePlot(int)
{

}

