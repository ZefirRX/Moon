#include "loginwindows.h"
#include "ui_loginwindows.h"

LoginWindows::LoginWindows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindows)
{
    ui->setupUi(this);
}

LoginWindows::~LoginWindows()
{
    delete ui;
}
