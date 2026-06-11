#include "homechats.h"
#include "ui_homechats.h"

HomeChats::HomeChats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeChats)
{
    ui->setupUi(this);
}

HomeChats::~HomeChats()
{
    delete ui;
}
