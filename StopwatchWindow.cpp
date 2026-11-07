#include "StopwatchWindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

StopwatchWindow::StopwatchWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Stopwatch");
    resize(360, 240);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    lblTimeDisplay_ = new QLabel("00:00:00.00", this);
    lblTimeDisplay_->setAlignment(Qt::AlignCenter);
    lblTimeDisplay_->setStyleSheet("font-size: 32px; font-weight: bold; font-family: monospace;");
    mainLayout->addWidget(lblTimeDisplay_);

    lblStatus_ = new QLabel("Status: Ready", this);
    lblStatus_->setAlignment(Qt::AlignCenter);
    lblStatus_->setStyleSheet("font-size: 14px; font-weight: bold;");
    mainLayout->addWidget(lblStatus_);

    auto *gridButtons = new QGridLayout();
    gridButtons->setSpacing(8);

    btnStart_ = new QPushButton("Start", this);
    btnCountdown_ = new QPushButton("Countdown to start", this);
    btnLap_ = new QPushButton("Lap", this);
    btnStop_ = new QPushButton("Stop", this);
    btnReset_ = new QPushButton("Reset", this);

    btnLap_->setEnabled(false);
    btnStop_->setEnabled(false);
    btnReset_->setEnabled(false);

    gridButtons->addWidget(btnStart_, 0, 0);
    gridButtons->addWidget(btnCountdown_, 0, 1);
    gridButtons->addWidget(btnLap_, 1, 0);
    gridButtons->addWidget(btnStop_, 1, 1);
    gridButtons->addWidget(btnReset_, 2, 0, 1, 2);

    mainLayout->addLayout(gridButtons);

    tickTimer_ = new QTimer(this);
    tickTimer_->setInterval(30);
    connect(tickTimer_, &QTimer::timeout, this, &StopwatchWindow::updateTimerTick);

    countdownTimer_ = new QTimer(this);
    countdownTimer_->setInterval(1000);
    connect(countdownTimer_, &QTimer::timeout, this, &StopwatchWindow::updateCountdownTick);

    connect(btnStart_, &QPushButton::clicked, this, &StopwatchWindow::startStopwatch);
    connect(btnCountdown_, &QPushButton::clicked, this, &StopwatchWindow::startCountdown);
    connect(btnLap_, &QPushButton::clicked, this, &StopwatchWindow::toggleLap);
    connect(btnStop_, &QPushButton::clicked, this, &StopwatchWindow::stopStopwatch);
    connect(btnReset_, &QPushButton::clicked, this, &StopwatchWindow::resetStopwatch);
}

void StopwatchWindow::startStopwatch()
{
    if (countdownTimer_->isActive())
    {
        countdownTimer_->stop();
    }

    if (!isRunning_)
    {
        isRunning_ = true;
        isLapFrozen_ = false;
        elapsedTimer_.start();
        tickTimer_->start();

        btnStart_->setEnabled(false);
        btnCountdown_->setEnabled(false);
        btnLap_->setEnabled(true);
        btnLap_->setText("Lap");
        btnStop_->setEnabled(true);
        btnReset_->setEnabled(false);
        lblStatus_->setText("Status: Running");
    }
}

void StopwatchWindow::startCountdown()
{
    if (isRunning_)
    {
        stopStopwatch();
    }

    accumulatedMs_ = 0;
    isLapFrozen_ = false;
    updateDisplay();

    countdownSecondsLeft_ = 10;
    lblStatus_->setText(QString("Starting in: %1s").arg(countdownSecondsLeft_));

    btnStart_->setEnabled(true);
    btnCountdown_->setEnabled(false);
    btnLap_->setEnabled(false);
    btnStop_->setEnabled(true);
    btnReset_->setEnabled(false);

    countdownTimer_->start();
}

void StopwatchWindow::updateCountdownTick()
{
    countdownSecondsLeft_--;
    if (countdownSecondsLeft_ > 0)
    {
        lblStatus_->setText(QString("Starting in: %1s").arg(countdownSecondsLeft_));
    }
    else
    {
        countdownTimer_->stop();
        lblStatus_->setText("Status: Running");
        startStopwatch();
    }
}

void StopwatchWindow::toggleLap()
{
    if (!isRunning_)
    {
        return;
    }

    if (!isLapFrozen_)
    {
        isLapFrozen_ = true;
        lapFrozenMs_ = accumulatedMs_ + elapsedTimer_.elapsed();
        btnLap_->setText("Resume Display");
        lblStatus_->setText(QString("Lap: %1 (Display frozen)").arg(formatTime(lapFrozenMs_)));
        lblTimeDisplay_->setText(formatTime(lapFrozenMs_));
    }
    else
    {
        isLapFrozen_ = false;
        btnLap_->setText("Lap");
        lblStatus_->setText("Status: Running");
        updateDisplay();
    }
}

void StopwatchWindow::stopStopwatch()
{
    if (countdownTimer_->isActive())
    {
        countdownTimer_->stop();
        lblStatus_->setText("Countdown canceled");
        btnStart_->setEnabled(true);
        btnCountdown_->setEnabled(true);
        btnStop_->setEnabled(false);
        btnReset_->setEnabled(accumulatedMs_ > 0);
        return;
    }

    if (isRunning_)
    {
        accumulatedMs_ += elapsedTimer_.elapsed();
        isRunning_ = false;
        tickTimer_->stop();

        isLapFrozen_ = false;
        btnLap_->setText("Lap");
        btnLap_->setEnabled(false);
        btnStart_->setEnabled(true);
        btnCountdown_->setEnabled(true);
        btnStop_->setEnabled(false);
        btnReset_->setEnabled(true);
        lblStatus_->setText("Status: Stopped");
        updateDisplay();
    }
}

void StopwatchWindow::resetStopwatch()
{
    if (countdownTimer_->isActive())
    {
        countdownTimer_->stop();
    }
    if (isRunning_)
    {
        tickTimer_->stop();
        isRunning_ = false;
    }

    accumulatedMs_ = 0;
    isLapFrozen_ = false;
    lblTimeDisplay_->setText("00:00:00.00");
    lblStatus_->setText("Status: Ready");

    btnStart_->setEnabled(true);
    btnCountdown_->setEnabled(true);
    btnLap_->setEnabled(false);
    btnLap_->setText("Lap");
    btnStop_->setEnabled(false);
    btnReset_->setEnabled(false);
}

void StopwatchWindow::updateTimerTick()
{
    if (!isLapFrozen_)
    {
        updateDisplay();
    }
}

void StopwatchWindow::updateDisplay()
{
    qint64 totalMs = accumulatedMs_;
    if (isRunning_)
    {
        totalMs += elapsedTimer_.elapsed();
    }
    lblTimeDisplay_->setText(formatTime(totalMs));
}

QString StopwatchWindow::formatTime(qint64 totalMs) const
{
    const int hours = static_cast<int>(totalMs / 3600000);
    const int minutes = static_cast<int>((totalMs % 3600000) / 60000);
    const int seconds = static_cast<int>((totalMs % 60000) / 1000);
    const int centis = static_cast<int>((totalMs % 1000) / 10);

    return QString("%1:%2:%3.%4")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(centis, 2, 10, QChar('0'));
}

