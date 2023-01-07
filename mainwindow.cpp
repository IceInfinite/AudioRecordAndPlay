#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow())
{
    ui_->setupUi(this);
    ModeChanged(0);
    Init();
}

MainWindow::~MainWindow()
{
    Stop();
    delete ui_;
    if (audio_send_)
    {
        delete audio_send_;
        audio_send_ = nullptr;
    }

    if (transport_)
    {
        delete transport_;
        transport_ = nullptr;
    }

    for (auto audio_play : audio_play_lists_)
    {
        if (audio_play)
        {
            delete audio_play;
            audio_play = nullptr;
        }
    }
    audio_play_lists_.clear();
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

void MainWindow::on_play_btn_clicked()
{
    if(running_)
    {
        Stop();
        ui_->play_btn->setText(tr("Play"));
    }else
    {
        Start();
        ui_->play_btn->setText(tr("Stop"));
    }
    running_ = !running_;
}

void MainWindow::ModeChanged(int id)
{
    ui_->device_comboBox->clear();
    const QAudio::Mode mode = id == 0 ? QAudio::AudioInput : QAudio::AudioOutput;
    for (auto &deviceInfo: QAudioDeviceInfo::availableDevices(mode))
        ui_->device_comboBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));

    on_device_comboBox_currentIndexChanged(0);
}

void MainWindow::on_device_comboBox_currentIndexChanged(int index)
{
    if (ui_->device_comboBox->count() == 0)
        return;

    // device has changed
    input_device_info_ = ui_->device_comboBox->itemData(index).value<QAudioDeviceInfo>();
    if(audio_send_ != nullptr)
    {
        delete audio_send_;
        audio_send_ = new AudioSend(input_device_info_, 8000, 1, 16);
        audio_send_->moveToThread(&audio_send_thread_);
        connect(audio_send_, &AudioSend::AudioByteReady, transport_, &Transport::ReceiveAudioFrame);
    }
}


