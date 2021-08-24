#include "audiosend.h"
#include <QDebug>

AudioSend::AudioSend(QAudioDeviceInfo& info, int sample_rate, int channel_count, int sample_size)
{
    SetAudioforamt(info sample_rate, channel_count,sample_size);
    InputDeviceStart();
}
AudioSend::~AudioSend()
{
    audio_input_->stop();
    input_device_->close();
}
void AudioSend::SetAudioforamt(QAudioDeviceInfo& info, int sample_rate, int channel_count, int sample_size)
{
    audio_format_.setSampleRate(sample_rate);
    audio_format_.setChannelCount(channel_count);
    audio_format_.setSampleSize(sample_size);
    audio_format_.setCodec("audio/pcm");
    audio_format_.setByteOrder(QAudioFormat::LittleEndian);
    audio_format_.setSampleType(QAudioFormat::UnSignedInt);

    if(!info.isFormatSupported(audio_format_))
    {
        qWarning() << "Default format not supported, trying to use the nearest.";
        audio_format_ = info.nearestFormat(audio_format_);
    }

    audio_input_ = new QAudioInput(info, audio_format_, this);
}

void AudioSend::InputDeviceStart()
{
    input_device_ = audio_input_->start();
    connect(input_device_, &QIODevice::readyRead, this, &AudioSend::HandleReadyRead);
}

void AudioSend::HandleReadyRead()
{
    QByteArray audio_byte;
    audio_byte = input_device_->readAll();
    emit AudioByteReady(audio_byte);
}
