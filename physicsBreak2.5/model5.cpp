#include "models.h"
#include "plot.h"

Model5::Model5()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    LoadModel();
}


void Model5::Transform()
{

}

void Model5::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");

    Qt3DCore::QEntity *p1 = addObject(ent, ":/Stands/Math5/ospol.obj", ":/Stands/Math5/wood.jpg");
    Qt3DCore::QEntity *p2 = addObject(ent, ":/Stands/Math5/prus.obj", ":/Stands/Math5/prus.jpg");
    Qt3DCore::QEntity *p3 = addObject(ent, ":/Stands/Math5/pstick.obj", ":/Stands/Math5/pstick.jpg");
    Qt3DCore::QEntity *p4 = addObject(ent, ":/Stands/Math5/st1.obj", ":/Stands/Math5/ster.png");
    Qt3DCore::QEntity *p5 = addObject(ent, ":/Stands/Math5/st2.obj", ":/Stands/Math5/ster.png");
    Qt3DCore::QEntity *p6 = addObject(ent, ":/Stands/Math5/wall.obj", ":/Stands/Math5/wall.jpg");

    tr1 = new Qt3DCore::QTransform();
    p1->addComponent(tr1);
    p6->addComponent(tr1);
    p5->addComponent(tr1);
    p4->addComponent(tr1);
    p3->addComponent(tr1);
    p2->addComponent(tr1);

    tr1->setTranslation(QVector3D(0.22, 0.46, 0.));

}


void Model5::Init()
{

}

void Model5::Compute(double)
{

}

void Model5::Update(double)
{

}

void Model5::CreatePlot(int)
{

}

