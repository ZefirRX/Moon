#include "aftwindow.h"
#include "ui_aftwindow.h"

aftwindow::aftwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::aftwindow)
{
    ui->setupUi(this);
}

aftwindow::~aftwindow()
{
    delete ui;
}