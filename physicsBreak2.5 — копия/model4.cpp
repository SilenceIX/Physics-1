#include "models.h"
#include "plot.h"

void Model4::Transform()
{

}

void Model4::LoadModel()
{    
    addObject(ent, ":/Res/Room.obj", ":/Res/Room.png");
    addObject(ent, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");

    Qt3DCore::QEntity *p1 = addObject(ent, ":/Stands/Math4/doska.obj", ":/Stands/Math4/doska.png");
    Qt3DCore::QEntity *p2 = addObject(ent, ":/Stands/Math4/izmeritel.obj", ":/Stands/Math4/izmeritel.png");
    Qt3DCore::QEntity *p3 = addObject(ent, ":/Stands/Math4/knopka.obj", ":/Stands/Math4/knopka.png");
    Qt3DCore::QEntity *p4 = addObject(ent, ":/Stands/Math4/lineyka.obj", ":/Stands/Math4/lineyka.png");
    Qt3DCore::QEntity *p5 = addObject(ent, ":/Stands/Math4/mayatnik.obj", ":/Stands/Math4/mayatnik.png");
    Qt3DCore::QEntity *p6 = addObject(ent, ":/Stands/Math4/osi.obj", ":/Stands/Math4/osi.png");
    Qt3DCore::QEntity *p7 = addObject(ent, ":/Stands/Math4/pistol.obj", ":/Stands/Math4/pistol.png");
    Qt3DCore::QEntity *p8 = addObject(ent, ":/Stands/Math4/polka.obj", ":/Stands/Math4/polka.png");

    tr1 = new Qt3DCore::QTransform();
    p1->addComponent(tr1);
    p8->addComponent(tr1);
    p7->addComponent(tr1);
    p6->addComponent(tr1);
    p5->addComponent(tr1);
    p4->addComponent(tr1);
    p3->addComponent(tr1);
    p2->addComponent(tr1);

    tr1->setTranslation(QVector3D(4.9, -2.9, -1.0));

}

Model4::Model4()
{
    ent = new Qt3DCore::QEntity();
    inf =  new QVBoxLayout();
    set =  new QVBoxLayout();
    LoadModel();
}

void Model4::Init()
{

}

void Model4::Compute(double)
{

}

void Model4::Update(double)
{

}

void Model4::CreatePlot(int)
{

}


