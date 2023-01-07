#include "audioplay.h"

#include <QDebug>

AudioPlay::AudioPlay(
    const QAudioDeviceInfo &info, int sample_rate, int channel_count,
    int sample_size)
{
    SetAudioFormat(info, sample_rate, channel_count, sample_size);
    OutputDeviceStart();
}

AudioPlay::~AudioPlay()
{
    audio_output_->stop();
    output_device_->close();
}

void AudioPlay::SetCurrentVolumn(qreal volumn)
{
    audio_output_->setVolume(volumn);
}

void AudioPlay::SetAudioFormat(
    const QAudioDeviceInfo &info, int sample_rate, int channel_count,
    int sample_size)
{
    audio_format_.setSampleRate(sample_rate);
    audio_format_.setChannelCount(channel_count);
    audio_format_.setSampleSize(sample_size);
    audio_format_.setCodec("audio/pcm");
    audio_format_.setByteOrder(QAudioFormat::LittleEndian);
    audio_format_.setSampleType(QAudioFormat::UnSignedInt);

    if (!info.isFormatSupported(audio_format_))
    {
        qWarning()
            << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }
    audio_output_ = new QAudioOutput(info, audio_format_, this);
}

void AudioPlay::OutputDeviceStart()
{
    output_device_ = audio_output_->start();
}

void AudioPlay::PlayoutAudio(const QByteArray &audio_frame)
{
    output_device_->write(audio_frame);
}
