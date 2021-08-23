#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audioplay.h"
#include "audiosend.h"
#include "transport.h"

#include <QMainWindow>
#include <QThread>
#include <QVector>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QThread audio_send_thread_;
    QVector<QThread*> audio_play_threads_;
    QThread transport_thread_;

    AudioSend* audio_send_ = nullptr;
    QVector<AudioPlay*> audio_play_lists;
    Transport* transport_ = nullptr;
};

#endif // MAINWINDOW_H
