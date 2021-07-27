#include "transport.h"
#include <QDebug>
#include <QMutexLocker>

Transport::Transport()
{
}

Transport::~Transport()
{
    audio_data_buffer_.clear();
}

void Transport::receiveAudioData(const QByteArray &audioByte)
{
    audio_data_buffer_.append(audioByte.data(), audioByte.size());

    if(audio_data_buffer_.size() < current_buffer_index_ + FRAME_LEN) //缓冲区不足以播放一帧的音频
    {
        return;
    }else {
        QByteArray writeData;
        writeData.append(&audio_data_buffer_.data()[current_buffer_index_], FRAME_LEN);
        emit receiveReady(writeData);
        current_buffer_index_ += FRAME_LEN;

        if(current_buffer_index_ > MAX_BUFFER_LEN)
        {
            audio_data_buffer_ = audio_data_buffer_.right(audio_data_buffer_.size() - MAX_BUFFER_LEN);
            current_buffer_index_ -= MAX_BUFFER_LEN;
        }
    }
}

