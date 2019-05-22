#include "models.h"
#include "plot.h"

Model7::Model7()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    LoadModel();
}


void Model7::Transform()
{

}

void Model7::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");

    Qt3DCore::QEntity *p1 = addObject(ent, ":/Stands/Math7/base2.obj", ":/Stands/Math7/metal.jpg");
    Qt3DCore::QEntity *p2 = addObject(ent, ":/Stands/Math7/cube.obj", ":/Stands/Math7/metal.jpg");
    Qt3DCore::QEntity *p3 = addObject(ent, ":/Stands/Math7/planka.obj", ":/Stands/Math7/metal.jpg");
    Qt3DCore::QEntity *p4 = addObject(ent, ":/Stands/Math7/sphere.obj", ":/Stands/Math7/metal.jpg");
    Qt3DCore::QEntity *p5 = addObject(ent, ":/Stands/Math7/up_rope.obj", ":/Stands/Math7/rope.jpg");
    Qt3DCore::QEntity *p6 = addObject(ent, ":/Stands/Math7/down_rope.obj", ":/Stands/Math7/rope.jpg");




    /*tr1 = new Qt3DCore::QTransform();
    p1->addComponent(tr1);
    p6->addComponent(tr1);
    p5->addComponent(tr1);
    p4->addComponent(tr1);
    p3->addComponent(tr1);
    p2->addComponent(tr1);

    tr1->setTranslation(QVector3D(0.22, 0.46, 0.));*/

}


void Model7::Init()
{

}

void Model7::Compute(double)
{

}

void Model7::Update(double)
{

}

void Model7::CreatePlot(int)
{

}

