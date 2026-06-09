#ifndef LOGINWINDOWS_H
#define LOGINWINDOWS_H

#include <QDialog>

namespace Ui {
class LoginWindows;
}

class LoginWindows : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindows(QWidget *parent = nullptr);
    ~LoginWindows();

private:
    Ui::LoginWindows *ui;
};

#endif // LOGINWINDOWS_H
