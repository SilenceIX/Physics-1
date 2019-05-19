#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->w->setGeometry(0, 0, width() * 0.75, height());

    sceneWindow = new Qt3DExtras::Qt3DWindow();





    ev = new MouseEv(this);
    sceneWindow->defaultFrameGraph()->setClearColor(QColor(QRgb(0x888888)));
    sceneWindow->installEventFilter(ev);

    scene = QWidget::createWindowContainer(sceneWindow, ui->dvid);
    scene->setGeometry(ui->dvid->geometry());
    sceneEntity = new Qt3DCore::QEntity();
    sceneEntityRoom = new Qt3DCore::QEntity();
    sceneWindow->setRootEntity(sceneEntity);

    uprend = new QTimer();
    uprend->setInterval(100);
    connect(uprend, SIGNAL(timeout()), this, SLOT(Repaint()));


    connect(ui->action_3, &QAction::triggered, [=](){exit(0);});
    connect(ui->action, &QAction::triggered, [=](){hwind->updspr(0); hwind->show();});

    connect(ui->action_5, &QAction::triggered, [=](){curC = 0; on_pushButton_clicked();});
    connect(ui->action_6, &QAction::triggered, [=](){curC = 1; on_pushButton_clicked();});
    connect(ui->action_7, &QAction::triggered, [=](){curC = 2; on_pushButton_clicked();});
    connect(ui->action_8, &QAction::triggered, [=](){curC = 3; on_pushButton_clicked();});
    connect(ui->action_9, &QAction::triggered, [=](){curC = 4; on_pushButton_clicked();});
    connect(ui->action_10, &QAction::triggered, [=](){curC = 5; on_pushButton_clicked();});




    /*Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(sceneEntity);
    Qt3DRender::QSpotLight *light = new Qt3DRender::QSpotLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    light->setCutOffAngle(120);
    light->setLocalDirection(QVector3D(0.0, -1.0, 0.0));
    light->setLinearAttenuation(0.1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(0.0, 1.0, 0.0));
    lightEntity->addComponent(lightTransform);*/

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(sceneEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(0.6f);
    light->setLinearAttenuation(0.1f);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(10.0, 10.0, 0.0));
    lightEntity->addComponent(lightTransform);

    camera = sceneWindow->camera();
    camera->lens()->setPerspectiveProjection(100.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(20.0, 3.0, 0.0));
    camera->setUpVector(QVector3D(0, 1, 0));
    camera->setViewCenter(QVector3D(19.0, 3.0, 0.0));

    timer2 = new QTimer();
    timer2->setInterval(2);
    connect(timer2, SIGNAL(timeout()), this, SLOT(moveToRoom()));


    timer = new QTimer();
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));

    hwind = new help();
    hwind->setAttribute(Qt::WA_ShowModal);

    lTime = new QLabel();


    ui->startBut->setHidden(true);

    CreateEntity();
    initModels();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initModels()
{
}

Qt3DRender::QCamera * MainWindow::getCamera()
{
    return camera;
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    scene->setGeometry(ui->dvid->geometry());
}

void MainWindow::mousePress(QMouseEvent *me)
{
    mouse_x = me->x();
    mouse_y = me->y();
}

void MainWindow::mouseMove(QMouseEvent *me)
{
    Qt::MouseButtons b = me->buttons();
    if (b & Qt::LeftButton)
    {
        if (curC==-2)
        {
            beta += -(mouse_y - me->y()) * (PI / 360.);
            alpha += (mouse_x - me->x()) * (PI / 360.);
            mouse_x = me->x();
            mouse_y = me->y();


            if (beta > 75 * PI / 180) beta = 75 * PI / 180;
            if (beta < -10 * PI / 180) beta = -10 * PI / 180;
            QVector3D pos(-float(cos(beta)*cos(alpha))*radius,
                        float(sin(beta))*radius,
                        float(cos(beta)*sin(alpha))*radius);
            camera->setPosition(pos);
        }
        else
        {
            camera->tilt((mouse_y - me->y()) * 0.1f);
            camera->pan(-(mouse_x - me->x()) * 0.1f);
            camera->setUpVector(QVector3D(0, 1, 0));
            mouse_y = me->y();
            mouse_x = me->x();
        }
   }
}

void MainWindow::mouseWheel(QWheelEvent *ev)
{
    if (curC == -2)
    {
        if (ev->delta() > 0)
            radius -= 0.1f;
        else
            radius += 0.1f;

        if (radius < 2)
            radius = 2.f;

        if (radius > 5.5f)
            radius = 5.5f;

        QVector3D pos(-float(cos(beta)*cos(alpha))*radius,
                    float(sin(beta))*radius,
                    float(cos(beta)*sin(alpha))*radius);
        camera->setPosition(pos);
    }
}

void MainWindow::CreateEntity()
{
    addObject(sceneEntity, ":/Res/Corridor.obj", ":/Res/Corridor.png");
    addObject(sceneEntity, ":/Res/ceiling.obj", ":/Res/ceiling.jpg");
}

void MainWindow::Update()
{
    m->Update(0.005);
    time += 1;
    lTime->setText(QString("Время: %1:%2:%3").arg(time / 100 / 60, 2, 10,
                                                  QLatin1Char('0')).arg(time / 100 % 60, 2, 10,
                                                                        QLatin1Char('0')).arg(time % 100, 2, 10, QLatin1Char('0')));
}

void MainWindow::Repaint()
{
    camera->pan(1.0f);
    camera->pan(-1.0f);
    uprend->stop();
}

void MainWindow::on_room1_clicked()
{
    curC = 0;
    cameraMoveTo();
}

void MainWindow::on_room2_clicked()
{
    curC = 1;
    cameraMoveTo();
}

void MainWindow::on_room3_clicked()
{
    curC = 2;
    cameraMoveTo();
}

void MainWindow::on_room4_clicked()
{
    curC = 3;
    cameraMoveTo();
}

void MainWindow::on_room5_clicked()
{
    curC = 4;
    cameraMoveTo();
}

void MainWindow::on_room6_clicked()
{
    curC = 5;
    cameraMoveTo();
}

void MainWindow::on_room7_clicked()
{
    curC = 6;
    cameraMoveTo();
}

void MainWindow::on_pushButton_clicked()
{
    if (m != nullptr)
        m->~Model();
    if (curC == -1)
        qDebug()<< "Error!";
    else if (curC != -2)
    {
        block = true;

        beta = PI / 6.;
        alpha = 3 * PI / 2.;
        time = 0;
        lTime->setText("Время: 00:00:00");


        switch (curC)
        {
        case 0:
            m = new Model1();
            break;
        case 1:
            m = new Model2();
            break;
        case 2:
            m = new Model3();
            break;
        case 3:
            m = new Model4();
            break;
        case 4:
            m = new Model5();
            break;
        case 5:
            m = new Model6();
            break;
        default:
            m = new Model1();
        }
        sceneWindow->setRootEntity(m->GetEntity());
        QWidget *w1 = new QWidget(this), *w2 = new QWidget(this);
        QVBoxLayout *l = m->GetInf();
        l->addWidget(lTime);
        ui->status->addWidget(w1);
        ui->setup->addWidget(w2);
        ui->status->setCurrentWidget(w1);
        ui->setup->setCurrentWidget(w2);
        w1->setLayout(l);
        w2->setLayout(m->GetSet());
        ui->pushButton->setText("Выход из комнаты");
        ui->numbers->setHidden(true);
        ui->startBut->setVisible(true);
        ui->status->setVisible(true);
        ui->setup->setVisible(true);
        cur = curC + 1;
        curC = -2;
        this->setWindowTitle(m->GetName());

        camera->setFieldOfView(60.0f);
        QVector3D pos(-float(cos(beta)*cos(alpha))*radius,
                    float(sin(beta))*radius,
                    float(cos(beta)*sin(alpha))*radius);
        camera->setPosition(pos);
        camera->setViewCenter(QVector3D(0.0, 0.0, 0.0));
        uprend->start();
    }
    else
    {
        sceneWindow->setRootEntity(sceneEntity);
        camera->setFieldOfView(100.0f);
        camera->setPosition(QVector3D(20.0, 3.0, 0.0));
        camera->setViewCenter(QVector3D(19.0, 3.0, 0.0));

        curC = -1;
        cur = 0;
        ui->pushButton->setText("Перейти в комнату");
        ui->status->setHidden(true);
        ui->setup->setHidden(true);
        ui->startBut->setHidden(true);
        ui->numbers->setVisible(true);
        ui->setup->removeWidget(ui->setup->widget(0));
        ui->status->removeWidget(ui->setup->widget(0));
        this->setWindowTitle("Осцилляторы");
        timer->stop();        
        uprend->start();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    hwind->updspr(cur);
    hwind->show();
}

void MainWindow::on_startBut_clicked()
{
    //sections//
    if (!timer->isActive())
    {
        m->Init();
        time = 0;
        lTime->setText("Время: 00:00:00");
        ui->startBut->setText("Остановить");
        timer->start();
    }
    else
    {
        timer->stop();
        ui->startBut->setText("Запуск маятника");
    }
}

