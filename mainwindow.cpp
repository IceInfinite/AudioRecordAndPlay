#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(0, 0, 400, 400);
    setWindowTitle("AudioRecord&Play");

    audio_send_ = new AudioSend(8000, 1, 16);
    transport_ = new Transport;

    audio_send_->moveToThread(&audio_send_thread_);
    transport_->moveToThread(&transport_thread_);

//    connect(&audioSendThread, &QThread::finished, audioSend, &QObject::deleteLater);
//    connect(&audioPlayThread, &QThread::finished, audioPlay, &QObject::deleteLater);
//    connect(&transportThread, &QThread::finished, transport, &QObject::deleteLater);

    connect(audio_send_, &AudioSend::AudioDataReady, transport_, &Transport::receiveAudioData);
//    connect(transport_, &Transport::receiveReady, audio_play_lists, &AudioPlay::PlayAudioData);
    QList<QAudioDeviceInfo> play_devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for(int i = 0; i < play_devices.size()/2; ++i)
    {
        QAudioDeviceInfo play_device_info = play_devices.at(i);
        qDebug() << "AudioDevice" << i+1 << ": " << play_device_info.deviceName();
        AudioPlay* audio_play = new AudioPlay(play_device_info, 8000, 1, 16);
        audio_play_lists.push_back(audio_play);
        QThread* audio_play_thread = new QThread(this);
        audio_play->moveToThread(audio_play_thread);
        connect(transport_, &Transport::receiveReady, audio_play, &AudioPlay::PlayAudioData);
        audio_play_threads_.push_back(audio_play_thread);
    }

    for(int i =0; i < audio_play_threads_.size(); ++i)
    {
        audio_play_threads_[i]->start();
    }
    // 各线程启动
    transport_thread_.start();
    audio_send_thread_.start();

}

MainWindow::~MainWindow()
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
//    audio_play_threads_.quit();
//    audio_play_threads_.wait();
//    delete audioSend;
//    delete audioPlay;
//    delete transport;

}
