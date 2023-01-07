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

namespace Ui
{
class MainWindow;
} // namespace Ui

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void Init();
    void Start();
    void Stop();

private:
    Ui::MainWindow *ui_;

    QThread audio_send_thread_;
    QThread transport_thread_;
    QVector<QThread*> audio_play_threads_;

    AudioSend* audio_send_ = nullptr;
    Transport* transport_ = nullptr;
    QVector<AudioPlay*> audio_play_lists_;

    QAudioDeviceInfo input_device_info_;

    bool running_ = false;

private slots:
    void on_play_btn_clicked();
    void on_device_comboBox_currentIndexChanged(int index);
    void ModeChanged(int id);

};

#endif // MAINWINDOW_H
