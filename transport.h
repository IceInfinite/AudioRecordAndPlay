#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QWidget>
#include <QByteArray>

const int MAX_BUFFER_LEN = 800 * 100; //缓冲区最大容量
const int FRAME_LEN = 800; //单个音频帧长度800字节，50ms音频数据

class Transport : public QObject
{
    Q_OBJECT
public:
    Transport();
    ~Transport();

private:
    QByteArray audio_data_buffer_; // 存放缓存数据
    int current_buffer_index_ = 0; // 当前已读取到的缓存序号

public slots:
    // 接受并缓存音频数据，另一线程执行
    void receiveAudioData(const QByteArray &audioByte);

signals:
    // 缓存完毕后发送数据
    void receiveReady(const QByteArray &frameBuffer);
};

#endif // TRANSPORT_H
