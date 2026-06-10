#ifndef AFTWINDOW_H
#define AFTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class aftwindow;
}
QT_END_NAMESPACE

class aftwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit aftwindow(QWidget *parent = nullptr);
    ~aftwindow() override;

private:
    Ui::aftwindow *ui;
};
#endif // AFTWINDOW_H
