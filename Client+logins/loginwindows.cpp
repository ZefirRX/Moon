#include "loginwindows.h"
#include "ui_loginwindows.h"
#include "aftwindow.h"
#include "QMessageBox"
#include "utils.h"
#include "homechats.h"

LoginWindows::LoginWindows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindows)
{
    ui->setupUi(this);
    setFixedSize(900, 500);
    ui->LabelGoToAfter->setText("Уже есть аккаунт? <a href=\"aftwin\" style=\"color:#7C6CFF;\">Войти</a>");
    connect(ui->LabelGoToAfter, &QLabel::linkActivated, this, &LoginWindows::openaftwindow);
    connect(Connect::instance(), &Connect::registerResult, this, &LoginWindows::SlotRegisterResualt);
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
    if(ui->EditPassword->text() != ui -> EditRepeatPassword -> text())
    {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают");
        return;
    }
    else if (isBlank(ui->EditTag->text()) || isBlank(ui->EditPassword->text()))
    {
        QMessageBox::warning(this, "Ошибка", "Заполните тэг и пароль");
        return;
    }
    else
    {
        Connect::instance() ->  SendRegister(ui -> EditNick -> text(), ui -> EditTag -> text(), ui ->EditPassword->text());
    }
}

void LoginWindows::SlotRegisterResualt(bool ok)
{
    if (ok)
    {
        HomeChats *window = new HomeChats();
        window -> show();
        this ->close();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка регистрации", "Такой тэг уже занят");
    }
}