#include "mainwindow.h"
#include <QDebug>
#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CreateWidgets();
    ModeChanged(0);
    Init();
    // TODO 连接信号槽，初始化机制再检查下是否正确，停止后再析构，线程的处理是否无误
//    connect()
}

MainWindow::~MainWindow()
{
    Stop();
}

void MainWindow::CreateWidgets()
{
    // TODO 布局设置完善
//    setWindowTitle("AudioRecord&Play");
    device_label_ = new QLabel(this);
    device_label_->setText("Device");

    device_box_ = new QComboBox(this);
//    device_box_->setFixedSize(device_label_->sizeHint());

//    start_button_ = new QPushButton(this);
//    start_button_->setEnabled(false);
//    start_button_->setToolTip(tr("Start"));
//    start_button_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    QHBoxLayout* h_layout = new QHBoxLayout;
    h_layout->addWidget(device_label_);
    h_layout->addWidget(device_box_);

//    controlLayout->addWidget(start_button_);
}

void MainWindow::Init()
{
    audio_send_ = new AudioSend(input_device_info_, 8000, 1, 16);
    transport_ = new Transport;

    audio_send_->moveToThread(&audio_send_thread_);
    transport_->moveToThread(&transport_thread_);

    connect(audio_send_, &AudioSend::AudioByteReady, transport_, &Transport::ReceiveAudioFrame);

    QList<QAudioDeviceInfo> play_devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for(int i = 0; i < play_devices.size(); ++i)
    {
        QAudioDeviceInfo play_device_info = play_devices.at(i);
        qDebug() << "AudioDevice" << i+1 << ": " << play_device_info.deviceName();
        AudioPlay* audio_play = new AudioPlay(play_device_info, 8000, 1, 16);
        audio_play_lists_.push_back(audio_play);
        QThread* audio_play_thread = new QThread(this);
        audio_play->moveToThread(audio_play_thread);
        connect(transport_, &Transport::ReceiveReady, audio_play, &AudioPlay::PlayoutAudio);
        audio_play_threads_.push_back(audio_play_thread);
    }
}
void MainWindow::Start()
{
    // 各线程启动
    for(int i =0; i < audio_play_threads_.size(); ++i)
    {
        audio_play_threads_[i]->start();
    }
    transport_thread_.start();
    audio_send_thread_.start();
}

void MainWindow::Stop()
{
    audio_send_thread_.quit();
    audio_send_thread_.wait();

    transport_thread_.quit();
    transport_thread_.wait();

    for(int i =0; i < audio_play_threads_.size(); ++i)
    {
        audio_play_threads_[i]->quit();
        audio_play_threads_[i]->wait();
    }
}

void MainWindow::ToggleStart()
{
    if(running_)
    {
        Stop();
        running_ = false;
    }else
    {
        Start();
        running_ = true;
    }
}

void MainWindow::ModeChanged(int id)
{
    device_box_->clear();
    const QAudio::Mode mode = id == 0 ? QAudio::AudioInput : QAudio::AudioOutput;
    for (auto &deviceInfo: QAudioDeviceInfo::availableDevices(mode))
        device_box_->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));

    device_box_->setCurrentIndex(0);
    DeviceChanged(0);
}

void MainWindow::DeviceChanged(int id)
{

    if (device_box_->count() == 0)
        return;

    // device has changed
    input_device_info_ = device_box_->itemData(id).value<QAudioDeviceInfo>();
    if(audio_send_ != nullptr)
    {
        delete audio_send_;
        audio_send_ = new AudioSend(input_device_info_, 8000, 1, 16);
        audio_send_->moveToThread(&audio_send_thread_);
    }

}


