#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QThread>
#include <QAudioOutput>
#include <QUdpSocket>
#include <opus.h>
class Receiver : public QThread
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = nullptr);
    void setListeningPort(quint16 port);
    void setAudioFormat(int SampleRate, int Channel,
                        int SampleSize, const QString& Codec);
protected:
    void run()override;
private:
    QUdpSocket *udpSocket;
    OpusDecoder *opusDecoder;
    quint16 listeningPort;

    QAudioFormat format;

};

#endif // RECEIVER_H
