#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audioplay.h"
#include "audiosend.h"
#include "transport.h"

#include <QMainWindow>
#include <QThread>
#include <QVector>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void CreateWidgets();
    void Init();
    void Start();
    void Stop();


private:
    QThread audio_send_thread_;
    QThread transport_thread_;
    QVector<QThread*> audio_play_threads_;

    AudioSend* audio_send_ = nullptr;
    Transport* transport_ = nullptr;
    QVector<AudioPlay*> audio_play_lists_;

    QAudioDeviceInfo  input_device_info_;

    bool running_ = false;
private:
    QPushButton* start_button_;
    QLabel* device_label_;
    QComboBox* device_box_;

private slots:
    void ToggleStart();
    void ModeChanged(int id);
    void DeviceChanged(int id);

};

#endif // MAINWINDOW_H
