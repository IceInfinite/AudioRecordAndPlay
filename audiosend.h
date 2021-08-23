#ifndef AUDIOSEND_H
#define AUDIOSEND_H

#include "transport.h"
#include <QAudioInput>
#include <QAudioFormat>
#include <QIODevice>

class AudioSend : public QObject
{
    Q_OBJECT
public:
    AudioSend(int sampleRate, int channelCount, int sampleSize);
    ~AudioSend();
    void setAudioforamt(int sampleRate, int channelCount, int sampleSize);
    void inputDeviceStart();
private:
    QAudioInput* audio_input_ = nullptr;
    QIODevice* input_device_ = nullptr;
    QAudioFormat audio_format_;

private slots:
    // 读取输入音频流
    void handleReadyRead();

signals:
    void AudioDataReady(const QByteArray& audio_frame);
};

#endif // AUDIOSEND_H
