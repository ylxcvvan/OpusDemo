#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include<QMessageBox>
#include"receiver.h"
#include"sender.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_pushButtonBegin_clicked();

    void on_pushButtonEnd_clicked();

private:
    Ui::MainWidget *ui;
    Receiver *receiver;
    Sender *sender;
};
#endif // MAINWIDGET_H
