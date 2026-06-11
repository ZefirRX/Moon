#ifndef HOMECHATS_H
#define HOMECHATS_H

#include <QWidget>

namespace Ui {
class HomeChats;
}

class HomeChats : public QWidget
{
    Q_OBJECT

public:
    explicit HomeChats(QWidget *parent = nullptr);
    ~HomeChats();

private:
    Ui::HomeChats *ui;
};

#endif // HOMECHATS_H
