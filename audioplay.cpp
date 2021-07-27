#include "audioplay.h"
#include <QDebug>


AudioPlay::AudioPlay(int sampleRate, int channelCount, int sampleSize)
{
    setAudioFormat(sampleRate,channelCount,sampleSize);
    outputDeviceStart();
}

AudioPlay::~AudioPlay()
{
    audio_output_->stop();
    output_device_->close();
}
void AudioPlay::setCurrentVolumn(qreal volumn)
{
    audio_output_->setVolume(volumn);
}

void AudioPlay::setAudioFormat(int sampleRate, int channelCount, int sampleSize)
{
    audio_format_.setSampleRate(sampleRate);
    audio_format_.setChannelCount(channelCount);
    audio_format_.setSampleSize(sampleSize);
    audio_format_.setCodec("audio/pcm");
    audio_format_.setByteOrder(QAudioFormat::LittleEndian);
    audio_format_.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
    if(!info.isFormatSupported(audio_format_))
    {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }
    audio_output_ = new QAudioOutput(audio_format_, this);
}

void AudioPlay::outputDeviceStart()
{
    output_device_ = audio_output_->start();
}

void AudioPlay::PlayAudioData(const QByteArray &frameBuffer)
{
    output_device_->write(frameBuffer);
}
