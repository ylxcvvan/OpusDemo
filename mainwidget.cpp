#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    //设置输入遮罩
    ui->lineEditChannel->setInputMask("00");  // 限制输入为最多2位整数
    ui->lineEditSampleSize->setInputMask("00");  // 限制输入为最多2位整数
    ui->lineEditPort->setInputMask("00000");  // 限制输入为最多5位数字

    receiver = new Receiver(this);
    sender = new Sender(this);
}

MainWidget::~MainWidget()
{

    // 停止并等待线程结束
    if (receiver) {
        receiver->requestInterruption(); // 请求停止线程
        receiver->wait(); // 等待线程退出
        delete receiver;
        receiver = nullptr; // 防止悬挂指针
    }
    if (sender) {
        sender->requestInterruption(); // 请求停止线程
        sender->wait(); // 等待线程退出
        delete sender;
        sender = nullptr; // 防止悬挂指针
    }


    delete ui;
}

void MainWidget::on_pushButtonBegin_clicked()
{
    int SampleRate=ui->lineEditSampleRate->currentText().toInt();
    int Channel=ui->lineEditChannel->text().toInt();
    int SampleSize=ui->lineEditSampleSize->text().toInt();
    QString Codec=ui->lineEditCodec->text();
    QString ip=ui->lineEditIpAddress->text();
    QHostAddress address(ip);
    if (address.isNull()) {
        QMessageBox::warning(this, "Invalid IP", "The IP address is not valid.");
        return;
    }

    int port=ui->lineEditPort->text().toInt();


    qDebug() << "Audio Format Settings:";
    qDebug() << "Sample Rate:" << SampleRate;
    qDebug() << "Channel Count:" << Channel;
    qDebug() << "Sample Size:" << SampleSize;
    qDebug() << "Codec:" << Codec;

    qDebug() << "Network Settings:";
    qDebug() << "IP Address:" << ip;
    qDebug() << "Port:" << port;


    receiver->setAudioFormat(SampleRate,Channel,SampleSize,Codec);
    sender->setAudioFormat(SampleRate,Channel,SampleSize,Codec);

    receiver->setListeningPort(port);
    sender->setDestination(ip,port);


    receiver->start();
    sender->start();

    ui->pushButtonBegin->setEnabled(false);
}


void MainWidget::on_pushButtonEnd_clicked()
{
    if (receiver) {
        receiver->requestInterruption(); // 请求停止线程
        receiver->wait(); // 等待线程退出
        delete receiver;
        receiver = nullptr; // 防止悬挂指针
    }
    if (sender) {
        sender->requestInterruption(); // 请求停止线程
        sender->wait(); // 等待线程退出
        delete sender;
        sender = nullptr; // 防止悬挂指针
    }
    this->close();
}

