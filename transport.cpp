#include "transport.h"
#include <QDebug>
#include <QMutexLocker>

Transport::Transport()
{
}

Transport::~Transport()
{
    audio_frame_buffer_.clear();
}

void Transport::ReceiveAudioFrame(const QByteArray& audio_byte)
{
    audio_frame_buffer_.append(audio_byte.data(), audio_byte.size());

    if(audio_frame_buffer_.size() < current_buffer_index_ + FRAME_LEN) //缓冲区不足以播放一帧的音频
    {
        return;
    }else {
        QByteArray audio_frame;
        audio_frame.append(&audio_frame_buffer_.data()[current_buffer_index_], FRAME_LEN);
        emit ReceiveReady(audio_frame);
        current_buffer_index_ += FRAME_LEN;

        if(current_buffer_index_ > MAX_BUFFER_LEN)
        {
            audio_frame_buffer_ = audio_frame_buffer_.right(audio_frame_buffer_.size() - MAX_BUFFER_LEN);
            current_buffer_index_ -= MAX_BUFFER_LEN;
        }
    }
}

