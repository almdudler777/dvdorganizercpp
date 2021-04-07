#include "actorwindow.h"
#include "ui_actorwindow.h"

ActorWindow::ActorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::actorWindow)
{
    ui->setupUi(this);
}

ActorWindow::~ActorWindow()
{
    delete ui;
}
