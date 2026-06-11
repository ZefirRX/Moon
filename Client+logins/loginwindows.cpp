#include "loginwindows.h"
#include "ui_loginwindows.h"
#include "aftwindow.h"
#include "homechats.h"

LoginWindows::LoginWindows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindows)
{
    ui->setupUi(this);
    setFixedSize(900, 500);
    ui->LabelGoToAfter->setText("Уже есть аккаунт? <a href=\"aftwin\" style=\"color:#7C6CFF;\">Войти</a>");
    connect(ui->LabelGoToAfter, &QLabel::linkActivated,
            this, &LoginWindows::openaftwindow);
}

LoginWindows::~LoginWindows()
{
    delete ui;
}

void LoginWindows::on_EditTag_cursorPositionChanged(int arg1, int arg2)
{

}

void LoginWindows::openaftwindow(const QString &link)
{
    aftwindow *window = new aftwindow();
    window->show();
    this->close();
}

void LoginWindows::on_ButtonRegister_clicked()
{
    HomeChats *window = new HomeChats();
    window -> show();
    this -> close();
}

