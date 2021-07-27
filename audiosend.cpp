#include "audiosend.h"
#include <QDebug>

AudioSend::AudioSend(int sampleRate, int channelCount, int sampleSize)
{
    setAudioforamt(sampleRate, channelCount,sampleSize);
    inputDeviceStart();
}
AudioSend::~AudioSend()
{
    audio_input_->stop();
    input_device_->close();
}
void AudioSend::setAudioforamt(int sampleRate, int channelCount, int sampleSize)
{
    audio_format_.setSampleRate(sampleRate);
    audio_format_.setChannelCount(channelCount);
    audio_format_.setSampleSize(sampleSize);
    audio_format_.setCodec("audio/pcm");
    audio_format_.setByteOrder(QAudioFormat::LittleEndian);
    audio_format_.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if(!info.isFormatSupported(audio_format_))
    {
        qWarning() << "Default format not supported, trying to use the nearest.";
        audio_format_ = info.nearestFormat(audio_format_);
    }

    audio_input_ = new QAudioInput(audio_format_, this);
}

void AudioSend::inputDeviceStart()
{
    input_device_ = audio_input_->start();
    connect(input_device_, &QIODevice::readyRead, this, &AudioSend::handleReadyRead);
}

void AudioSend::handleReadyRead()
{
    QByteArray audioByte;
    audioByte = input_device_->readAll();
    emit AudioDataReady(audioByte);
}
