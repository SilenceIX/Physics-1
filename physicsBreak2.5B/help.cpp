#include "help.h"
#include "ui_help.h"

help::help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::help)
{
    ui->setupUi(this);
}

help::~help()
{
    delete ui;
}

void help::updspr(int c)
{

    QVBoxLayout *l;
    switch (c)
    {
    case 0: l = sRect(); break;
    case 1: l = sRect1(); break;
    case 2: l = sRect2(); break;
    case 3: l = sRect3(); break;
    //case 4: l = sRect4(); break;
    case 5: l = sRect5(); break;
    //case 6: l = sRect6(); break;
    case 7: l = sRect7(); break;
    default: l = new QVBoxLayout();
    }


    l->addStretch();
    QWidget* widget = new QWidget;
    widget->setLayout(l);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    widget->setAutoFillBackground(true);
    widget->setPalette(Pal);

    ui->scrollArea->setWidget(widget);
}

void help::on_pushButton_clicked()
{
    this->close();

}


