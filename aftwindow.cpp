#include "aftwindow.h"
#include "ui_aftwindow.h"
#include "loginwindows.h"

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
void aftwindow::on_ButtonAftor_clicked()
{
    LoginWindows *window = new LoginWindows();
    window -> show();

    close();
}

