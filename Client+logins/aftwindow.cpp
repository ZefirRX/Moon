#include "aftwindow.h"
#include "ui_aftwindow.h"
#include "loginwindows.h"
#include  "homechats.h"
#include "utils.h"
#include "QMessageBox"

aftwindow::aftwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::aftwindow)
{
    ui->setupUi(this);
    ui->LabelGoToAfter->setText("Нет аккаунта? <a href=\"loginwin\" style=\"color:#7C6CFF;\">Cоздать</a>");
    connect(ui->LabelGoToAfter, &QLabel::linkActivated,this, &aftwindow::openloginwindow);
    connect(Connect::instance(), &Connect::loginResult, this, &aftwindow::SlotLoginResult);
}

aftwindow::~aftwindow()
{
    delete ui;
}

void aftwindow::openloginwindow(const QString &link)
{
    LoginWindows *window = new LoginWindows();
    window->show();
    this->close();
}

void aftwindow::on_ButtonAftor_clicked()
{
    if(isBlank(ui->EditTag->text()) || isBlank(ui->EditPassword->text()))
    {
        QMessageBox::warning(this, "Ошибка", "Заполните тэг и пароль");
        return;
    }
    Connect::instance() -> SendLogin(ui->EditTag->text(), ui->EditPassword->text());
}

void aftwindow::SlotLoginResult(bool ok, QString info)
{
    if(ok)
    {
        HomeChats *window = new HomeChats();
        window -> show();
        close();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка входа", "Неверный тэг или пароль");
    }
}

