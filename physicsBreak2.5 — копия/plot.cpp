#include "plot.h"
#include "ui_plot.h"
#include "mainwindow.h"

Plot::Plot(std::function<double()> getarg, std::function<double()> getvalue, QString s,QWidget *p) :
    QMainWindow(p),
    ui(new Ui::Plot),
    getarg(getarg),
    getvalue(getvalue)
{
    ui->setupUi(this);

    state = State::Active;

    plot = ui->PlotSurface;

    plot->xAxis->setLabel("Время, мс");
    plot->yAxis->setLabel(s);



    plot->addGraph();
    plot->xAxis->setRange(0, 20);
    plot->yAxis->setRange(-5, 5);

    plot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
}

void Plot::resizeEvent(QResizeEvent *re)
{
    ui->PlotSurface->setGeometry(0, 0, re->size().width(), re->size().height() * 0.75);
}

void Plot::closeEvent(QCloseEvent *ce)
{
    state = State::Closed;
}

void Plot::Destroy()
{
    delete plot;
    delete ui;
    this->destroy();
}

void Plot::Update()
{
    double arg = getarg();
    double value = getvalue();

    args.append(arg);
    values.append(value);

    if (args.size() >= 250000)
    {
        args.removeAt(0);
        values.removeAt(0);
    }

    Draw(args.toVector(), values.toVector());
}

void Plot::Draw(QVector<double> args, QVector<double> values)
{
    plot->graph(0)->setData(args, values, true);
    if (args.last() > 18)
        plot->xAxis->setRange(args.last() - 18, args.last() + 2);
    plot->replot();
}

void Plot::Restart()
{
    args.clear();
    values.clear();
    plot->xAxis->setRange(-18, 2);
    plot->replot();
}
