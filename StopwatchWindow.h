#pragma once

#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>

class QLabel;
class QPushButton;

class StopwatchWindow : public QWidget
{
    Q_OBJECT
public:
    explicit StopwatchWindow(QWidget *parent = nullptr);

public slots:
    void startStopwatch();
    void startCountdown();
    void toggleLap();
    void stopStopwatch();
    void resetStopwatch();

private slots:
    void updateTimerTick();
    void updateCountdownTick();

private:
    void updateDisplay();
    QString formatTime(qint64 totalMs) const;

    QLabel *lblTimeDisplay_ = nullptr;
    QLabel *lblStatus_ = nullptr;
    QPushButton *btnStart_ = nullptr;
    QPushButton *btnCountdown_ = nullptr;
    QPushButton *btnLap_ = nullptr;
    QPushButton *btnStop_ = nullptr;
    QPushButton *btnReset_ = nullptr;

    QTimer *tickTimer_ = nullptr;
    QTimer *countdownTimer_ = nullptr;
    QElapsedTimer elapsedTimer_;

    qint64 accumulatedMs_ = 0;
    bool isRunning_ = false;
    bool isLapFrozen_ = false;
    qint64 lapFrozenMs_ = 0;
    int countdownSecondsLeft_ = 10;
};

