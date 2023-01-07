#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>
#include <QByteArray>

// TODO 音频帧大小影响效果，时间越短效果越差，原因未知
const static int MAX_BUFFER_LEN = 800 * 100; //缓冲区最大容量
const static int FRAME_LEN = 800; //单个音频帧长度800字节，50ms音频数据

class Transport : public QObject
{
    Q_OBJECT

public:
    Transport();
    ~Transport();

private:
    QByteArray audio_frame_buffer_; // 存放缓存数据
    int current_buffer_index_ = 0; // 当前已读取到的缓存序号

public slots:
    // 接受并缓存音频数据,大小不定的音频数据输入
    void ReceiveAudioFrame(const QByteArray& audio_byte);

signals:
    // 缓存完毕后发送数据
    void ReceiveReady(const QByteArray& audio_frame);
};

#endif // TRANSPORT_H
