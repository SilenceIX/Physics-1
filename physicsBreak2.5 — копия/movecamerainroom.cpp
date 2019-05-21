#include "mainwindow.h"
#include "ui_mainwindow.h"

const float coordR[]={15.0, 8.0, 5.5, 0.0, -2.0, -8.0, -10.0};
const bool sideR[]={true, false, true, false, true, false, true};
float st, kx, kz;
int steps = 0;

void MainWindow::cameraMoveTo()
{
    block = true;
    ui->pushButton->setDisabled(true);
    float delta = camera->position().x() - coordR[curC];
    float fdeltz = camera->viewCenter().z() - 5.0f * (sideR[curC] ? 1.0f : -1.0f);
    float fdeltx = camera->viewCenter().x() - coordR[curC];
    if (delta < 0) st = 0.03f;
    else st = -0.03f;
    int stepR = int(delta / st);

    kz = fdeltz / float(stepR);

    kx = fdeltx / float(stepR);
    /*if (fdeltx < 0) kx = 0.05;
    else kx = -0.05;
    int stepR = int(fdeltx / kx);
    st = delta / float(stepR);
    kz = fdeltz / float(stepR);*/


    //camera->setViewCenter(QVector3D(coordR[s], 3.0, 5.0f * (sideR[c] ? 1.0f : -1.0f)));
    //k = delrot / stepR;
    block = false;
    timer2->start();
}

void MainWindow::moveToRoom()
{
    if (block)
    {
        timer2->stop();
        return;
    }
    ++steps;
    if (fabs(double(camera->position().x() - coordR[curC])) > 1e-1 && steps % 4 == 0)
    {
        camera->setPosition(QVector3D(camera->position().x() + st, camera->position().y(), camera->position().z()));
    }
    else if (fabs(double(camera->viewCenter().x() - coordR[curC])) > 1e-1)
    {
       camera->setViewCenter(QVector3D(camera->viewCenter().x() + kx, camera->position().y(), camera->viewCenter().z() + kz));
    }
    else if (fabs(double(camera->position().x() - coordR[curC])) <= 1e-1){
        timer2->stop();
        ui->pushButton->setDisabled(false);
        steps = 0;
    }


}
