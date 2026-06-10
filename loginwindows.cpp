#include "loginwindows.h"
#include "ui_loginwindows.h"
#include "mainwindow.h"

LoginWindows::LoginWindows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindows)
{
    ui->setupUi(this);
    setFixedSize(900, 500);
}

LoginWindows::~LoginWindows()
{
    delete ui;
}

void LoginWindows::on_EditTag_cursorPositionChanged(int arg1, int arg2)
{

}


void LoginWindows::on_pushButton_clicked()
{
    MainWindow *window = new MainWindow();
    window -> show();
    close();
}

