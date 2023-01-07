#ifndef AUDIOPLAY_H
#define AUDIOPLAY_H

#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
#include <QObject>

class AudioPlay : public QObject
{
    Q_OBJECT
public:
    AudioPlay(
        const QAudioDeviceInfo &info, int sample_rate, int channel_count,
        int sample_size);
    ~AudioPlay();
    void SetCurrentVolumn(qreal volumn);
    void SetAudioFormat(
        const QAudioDeviceInfo &info, int sample_rate, int channelCount,
        int sample_size);
    void OutputDeviceStart();

private:
    QAudioOutput *audio_output_ = nullptr;
    QIODevice *output_device_ = nullptr;
    QAudioFormat audio_format_;

public slots:
    void PlayoutAudio(const QByteArray &audio_frame);
};

#endif  // AUDIOPLAY_H
