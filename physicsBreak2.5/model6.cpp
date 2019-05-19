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

    Qt3DCore::QEntity *p1 = addObject(ent, ":/Stands/Math6/gruz1o.obj", ":/Stands/Math6/tex1.png");
    Qt3DCore::QEntity *p2 = addObject(ent, ":/Stands/Math6/gruz2o.obj", ":/Stands/Math6/tex1.png");
    Qt3DCore::QEntity *p5 = addObject(ent, ":/Stands/Math6/stand2o.obj", ":/Stands/Math6/tex1.png");
    Qt3DCore::QEntity *p6 = addObject(ent, ":/Stands/Math6/sterjeno.obj", ":/Stands/Math6/tex1.png");
    Qt3DCore::QEntity *p7 = addObject(ent, ":/Stands/Math6/stand.obj", ":/Stands/Math6/tex2.png");



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

void Model6::Compute(double)
{

}

void Model6::Update(double)
{

}

void Model6::CreatePlot(int)
{

}

