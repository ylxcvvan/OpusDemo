#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QThread>
#include<QIODevice>
#include<QAudioInput>
#include<QUdpSocket>
#include<opus.h>
class Sender : public QThread
{
    Q_OBJECT
public:
    explicit Sender(QObject *parent=nullptr);
    void setDestination(const QString &host, quint16 port);
    void setAudioFormat(int SampleRate, int Channel,
                        int SampleSize, const QString& Codec);
protected:
    void run()override;

private:

    QUdpSocket *udpSocket;
    QString destinationHost;
    quint16 destinationPort;

    QAudioFormat format;

    OpusEncoder *opusEncoder;

};

#endif // SENDER_H
