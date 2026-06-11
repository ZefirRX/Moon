#include "aftwindow.h"
#include "ui_aftwindow.h"
#include "loginwindows.h"
#include  "homechats.h".h

aftwindow::aftwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::aftwindow)
{
    ui->setupUi(this);
    ui->LabelGoToAfter->setText("Нет аккаунта? <a href=\"loginwin\" style=\"color:#7C6CFF;\">Cоздать</a>");
    connect(ui->LabelGoToAfter, &QLabel::linkActivated,
            this, &aftwindow::openloginwindow);
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
    HomeChats *window = new HomeChats();
    window -> show();

    close();
}

