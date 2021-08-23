#ifndef AUDIOPLAY_H
#define AUDIOPLAY_H

#include "transport.h"
#include <QAudioOutput>
#include <QAudioFormat>
#include <QIODevice>

class AudioPlay : public QObject
{
    Q_OBJECT
public:
    AudioPlay(const QAudioDeviceInfo& info, int sampleRate, int channelCount, int sampleSize);
    ~AudioPlay();
    void setCurrentVolumn(qreal volumn);
    void setAudioFormat(const QAudioDeviceInfo& info, int sampleRate, int channelCount, int sampleSize);
    void outputDeviceStart();

private:
    QAudioOutput* audio_output_ = nullptr;
    QIODevice* output_device_ = nullptr;
    QAudioFormat audio_format_;

public slots:
    void PlayAudioData(const QByteArray& audio_frame);
};

#endif // AUDIOPLAY_H
