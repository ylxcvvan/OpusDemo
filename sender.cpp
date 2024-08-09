#include "sender.h"
#include <QDebug>
#include <QAudioInput>

Sender::Sender(QObject *parent)
    : QThread(parent), opusEncoder(nullptr), udpSocket(nullptr)
{
    // 不在构造函数中创建 QAudioInput 和 QUdpSocket
}

void Sender::setDestination(const QString &host, quint16 port)
{
    destinationHost = host;
    destinationPort = port;
}

void Sender::setAudioFormat(int SampleRate, int Channel, int SampleSize, const QString &Codec)
{
    format.setSampleRate(SampleRate);
    format.setChannelCount(Channel);
    format.setSampleSize(SampleSize);
    format.setCodec(Codec);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
}

void Sender::run()
{
    // 在 run() 方法中创建 QAudioInput 和 QUdpSocket
    QAudioInput *audioInput = new QAudioInput(format);
    udpSocket = new QUdpSocket(this);

    int error;
    opusEncoder = opus_encoder_create(format.sampleRate(), format.channelCount(), OPUS_APPLICATION_VOIP, &error);
    if (error != OPUS_OK) {
        qDebug() << "Sample Rate:" << format.sampleRate();
        qDebug() << "Channels:" << format.channelCount();
        qDebug() << "Codec:" << format.codec();

        qWarning() << "Failed to create Opus encoder. Error code:" << error;
        return;
    }

    QIODevice *audioDevice = audioInput->start();
    if (!audioDevice) {
        qWarning("Failed to start audio input");
        return;
    }

    QByteArray audioData;
    QByteArray encodedData(4000, 0);  // Opus 编码输出缓冲区
    while (!isInterruptionRequested()) {
        // 读取音频数据
        audioData = audioDevice->read(960 * format.bytesPerFrame());  // 20 ms 的音频数据

        if (audioData.size() > 0) {
            // 编码为 Opus 格式
            int bytesEncoded = opus_encode(opusEncoder,
                                           reinterpret_cast<const opus_int16*>(audioData.constData()),
                                           960,
                                           reinterpret_cast<unsigned char*>(encodedData.data()),
                                           encodedData.size());

            if (bytesEncoded > 0) {
                // 通过 UDP 发送编码后的音频数据
                udpSocket->writeDatagram(encodedData.data(), bytesEncoded, QHostAddress(destinationHost), destinationPort);
            } else {
                qWarning("Opus encoding failed with error code %d", bytesEncoded);
            }
        }
    }

    // 停止音频输入
    audioInput->stop();

    // 清理资源
    opus_encoder_destroy(opusEncoder);
}
