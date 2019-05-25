#include "models.h"
#include "plot.h"

Model6::Model6()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    LoadModel();
}


void Model6::Transform()
{

}

void Model6::LoadModel()
{
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");
    addObject(ent, ":/Res/tablemetal.obj", ":/Res/tablemetal.png");

    addObject(ent, ":/Stands/Math6/stand.obj", ":/Stands/Math6/stand.png");
    Qt3DCore::QEntity *rod = addObject(ent, ":/Stands/Math6/rod.obj", ":/Stands/Math6/rod.png");
    Qt3DCore::QEntity *load = addObject(ent, ":/Stands/Math6/load.obj", ":/Stands/Math6/load.png");



    /*tr1 = new Qt3DCore::QTransform();
    p1->addComponent(tr1);
    p6->addComponent(tr1);
    p5->addComponent(tr1);
    p4->addComponent(tr1);
    p3->addComponent(tr1);
    p2->addComponent(tr1);

    tr1->setTranslation(QVector3D(0.22, 0.46, 0.));*/

}


void Model6::Init()
{

}




void Model6::Compute(double dt)
{

}

void Model6::Update(double dt)
{

}

void Model6::CreatePlot(int)
{

}

