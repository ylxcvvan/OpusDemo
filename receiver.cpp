#include "receiver.h"
#include <QDebug>
#include <QAudioOutput>

Receiver::Receiver(QObject *parent)
    : QThread(parent), opusDecoder(nullptr), udpSocket(nullptr)
{
    // 不在构造函数中创建 QAudioOutput 和 QUdpSocket
}

void Receiver::setListeningPort(quint16 port)
{
    listeningPort = port;
}

void Receiver::setAudioFormat(int SampleRate, int Channel, int SampleSize, const QString &Codec)
{
    format.setSampleRate(SampleRate);
    format.setChannelCount(Channel);
    format.setSampleSize(SampleSize);
    format.setCodec(Codec);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
}

void Receiver::run()
{
    int error;
    opusDecoder = opus_decoder_create(format.sampleRate(), format.channelCount(), &error);
    if (error != OPUS_OK) {

       qWarning() << "Failed to create Opus encoder. Error code:" << error;
        return;
    }

     QAudioOutput *audioOutput = new QAudioOutput(format);
    udpSocket = new QUdpSocket();
    udpSocket->bind(QHostAddress::Any, listeningPort);

    QIODevice *audioDevice = audioOutput->start();
    if (!audioDevice) {
        qWarning("Failed to start audio output");
        return;
    }

    QByteArray encodedData;
    QByteArray decodedData(format.bytesForFrames(960), 0);  // 960 frames of audio data

    while (!isInterruptionRequested()) {
        if (udpSocket->hasPendingDatagrams()) {
            encodedData.resize(int(udpSocket->pendingDatagramSize()));
            udpSocket->readDatagram(encodedData.data(), encodedData.size());

            int frameSize = opus_decode(opusDecoder,
                                        reinterpret_cast<const unsigned char*>(encodedData.constData()),
                                        encodedData.size(),
                                        reinterpret_cast<opus_int16*>(decodedData.data()),
                                        960,
                                        0);

            if (frameSize > 0) {
                audioDevice->write(decodedData.constData(), frameSize * format.bytesPerFrame());
            }
            qDebug()<<frameSize * format.bytesPerFrame();
        }
    }

    audioOutput->stop();
    opus_decoder_destroy(opusDecoder);
}
