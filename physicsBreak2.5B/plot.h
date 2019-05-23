#ifndef PLOT_H
#define PLOT_H

#include <QMainWindow>
#include <qcustomplot.h>

namespace Ui {
class Plot;
}

class Plot : public QMainWindow
{
    Q_OBJECT

public:
    enum class State
    {
        Active,
        Closed
    };

    explicit Plot(std::function<double()>, std::function<double()>, QString, QWidget *p = nullptr);

    void resizeEvent(QResizeEvent *) override;
    void closeEvent(QCloseEvent *) override;
    void Update();
    void Restart();
    void Destroy();

    State GetState() { return state; }

private:
    Ui::Plot *ui;

    State state;

    std::function<double()> getarg;
    std::function<double()> getvalue;

    QCustomPlot *plot;

    QList<double> args;
    QList<double> values;

    void Draw(QVector<double> args, QVector<double> values);
};

#endif
