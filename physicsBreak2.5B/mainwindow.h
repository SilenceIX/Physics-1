#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DCore>
#include <functional>
#include "help.h"
#include "models.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initModels();
    void resizeEvent(QResizeEvent *) override;
    void mouseMove(QMouseEvent *);
    void mousePress(QMouseEvent *);
    void mouseWheel(QWheelEvent *);
    Qt3DRender::QCamera *getCamera();
    void CreateEntity();


private slots:
    void Update();

    void Repaint();

    void on_room1_clicked();

    void on_room2_clicked();

    void on_room3_clicked();

    void on_room4_clicked();

    void on_room5_clicked();

    void on_room6_clicked();

    void on_room7_clicked();

    void moveToRoom();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_startBut_clicked();

private:
    Ui::MainWindow *ui;
    QWidget *scene;

    Qt3DExtras::Qt3DWindow *sceneWindow;
    Qt3DCore::QEntity *sceneEntity;
    Qt3DCore::QEntity *sceneEntityRoom;
    Qt3DRender::QCamera *camera;
    Qt3DCore::QEntity *lightEntity;
    Qt3DRender::QDirectionalLight *light;
    QLabel *lTime;


    const double PI = 2 * asin(1.);
    int mouse_x, mouse_y;
    double alpha = 0.0, beta = 0.0, expand = 1.0;
    clock_t time, stime;
    float radius = 5.0f;
    float angle = 0;
    float angle1 = 0;
    int curC = -1, cur = 0;
    bool block = false;

    class MouseEv *ev;

    help *hwind;

    QTimer *timer, *uprend, *timer2;




    void cameraMoveTo();

    Model *m = nullptr;


};


class MouseEv : public QObject
{
    Q_OBJECT

public:
    MouseEv(MainWindow *window) : window(window)
    {}

private:
    MainWindow *window;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseMove)
        {
            window->mouseMove((QMouseEvent*)event);
            return true;
        }

        if (event->type() == QEvent::MouseButtonPress)
        {
            window->mousePress((QMouseEvent*)event);
            return true;
        }

        if (event->type() == QEvent::Wheel)
        {
            window->mouseWheel((QWheelEvent *)event);
        }
    }
};





#endif
