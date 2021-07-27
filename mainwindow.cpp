#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(0, 0, 400, 400);
    setWindowTitle("AudioRecord&Play");

    audioSend = new AudioSend(8000, 1, 16);
    audioPlay = new AudioPlay(8000, 1, 16);
    transport = new Transport;
    audioPlay->setCurrentVolumn(100);

    audioSend->moveToThread(&audioSendThread);
    audioPlay->moveToThread(&audioPlayThread);
    transport->moveToThread(&transportThread);

//    connect(&audioSendThread, &QThread::finished, audioSend, &QObject::deleteLater);
//    connect(&audioPlayThread, &QThread::finished, audioPlay, &QObject::deleteLater);
//    connect(&transportThread, &QThread::finished, transport, &QObject::deleteLater);

    connect(audioSend, &AudioSend::AudioDataReady, transport, &Transport::receiveAudioData);
    connect(transport, &Transport::receiveReady, audioPlay, &AudioPlay::PlayAudioData);

    // 各线程启动
    audioPlayThread.start();
    transportThread.start();
    audioSendThread.start();

}

MainWindow::~MainWindow()
{
    audioSendThread.quit();
    audioSendThread.wait();

    transportThread.quit();
    transportThread.wait();

    audioPlayThread.quit();
    audioPlayThread.wait();
//    delete audioSend;
//    delete audioPlay;
//    delete transport;

}
