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
    AudioSend(QAudioDeviceInfo& info, int sample_rate, int channel_count, int sample_size);
    ~AudioSend();
    void SetAudioforamt(QAudioDeviceInfo& info, int sample_rate, int channel_count, int sample_size);
    void InputDeviceStart();
private:
    QAudioInput* audio_input_ = nullptr;
    QIODevice* input_device_ = nullptr;
    QAudioFormat audio_format_;

private slots:
    // 读取输入音频流
    void HandleReadyRead();

signals:
    void AudioByteReady(const QByteArray& audio_byte);
};

#endif // AUDIOSEND_H
