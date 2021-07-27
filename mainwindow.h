#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "transport.h"
#include "audioplay.h"
#include "audiosend.h"
#include <QMainWindow>
#include <QThread>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QThread audioSendThread;
    QThread audioPlayThread;
    QThread transportThread;

    AudioSend* audioSend = nullptr;
    AudioPlay* audioPlay = nullptr;
    Transport* transport = nullptr;
};

#endif // MAINWINDOW_H
