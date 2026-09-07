#include "standUP.h"

#include <QDateTime>
#include <QMessageBox>
#include <QStackedWidget>
#include <QTimer>
#include <QTime>

#include "AudioPlayer.h"
#include "ConfigurationPanel.h"
#include "MainScreen.h"
#include "SystemAlert.h"

standUP::standUP(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("standUP");
    resize(480, 520);

    stackedWidget_ = new QStackedWidget(this);

    mainScreen_ = new MainScreen(this);
    configPanel_ = new ConfigurationPanel(this);

    stackedWidget_->addWidget(mainScreen_);
    stackedWidget_->addWidget(configPanel_);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget_);
    setLayout(layout);

    clockTimer_ = new QTimer(this);
    connect(clockTimer_, &QTimer::timeout, this, &standUP::updateClock);
    clockTimer_->start(1000);

    countdownTimer_ = new QTimer(this);
    connect(countdownTimer_, &QTimer::timeout, this, &standUP::updateClock);

    audioPlayer_ = new AudioPlayer(this);
    systemAlert_ = new SystemAlert(this, this);

    connect(mainScreen_, &MainScreen::applyInputRequested, this, &standUP::onApplyInputTime);
    connect(mainScreen_, &MainScreen::goStopRequested, this, &standUP::onToggleGoStop);
    connect(mainScreen_, &MainScreen::pauseResumeRequested, this, &standUP::onTogglePauseResume);
    connect(mainScreen_, &MainScreen::settingsRequested, this, &standUP::showSettings);
    connect(mainScreen_, &MainScreen::addMinutesRequested, this, &standUP::onAddMinutes);
    connect(configPanel_, &ConfigurationPanel::saveRequested, this, &standUP::saveSettings);
    connect(configPanel_, &ConfigurationPanel::soundPreviewRequested, this, [this](const QString &filePath)
            { audioPlayer_->playPreview(filePath); });

    settings_.theme = "Dark";
    settings_.flashScreen = true;
    settings_.soundType = "Default Beep";

    audioPlayer_->configure(settings_);
    systemAlert_->configure(settings_, normalStyleSheet_);
    applyTheme();
    updateClock();
}

void standUP::updateClock()
{
    QDateTime now = QDateTime::currentDateTime();
    mainScreen_->currentTimeLabel()->setText("Current Time: " + now.toString("HH:mm:ss"));

    if (isRunning_ && !isPaused_)
    {
        qint64 secsLeft = now.secsTo(expireDateTime_);
        if (secsLeft <= 0)
        {
            triggerAlert();
        }
        else
        {
            QTime t = QTime(0, 0).addSecs(secsLeft);
            mainScreen_->statusLabel()->setText("Time Remaining: " + t.toString("HH:mm:ss"));
        }
    }
}

void standUP::onApplyInputTime()
{
    QString input = mainScreen_->timeInput()->text().trimmed();
    QDateTime now = QDateTime::currentDateTime();

    if (input.startsWith("+"))
    {
        bool ok = false;
        int mins = input.mid(1).toInt(&ok);
        if (ok && mins > 0)
        {
            expireDateTime_ = now.addSecs(mins * 60);
            startTimerExecution();
        }
        else
        {
            QMessageBox::warning(this, "Invalid Input", "Use format +[minutes], e.g., +15");
        }
    }
    else
    {
        QTime targetTime = QTime::fromString(input, "HH:mm:ss");
        if (targetTime.isValid())
        {
            QDateTime targetDateTime(now.date(), targetTime);
            if (targetDateTime <= now)
            {
                targetDateTime = targetDateTime.addDays(1);
            }
            expireDateTime_ = targetDateTime;
            startTimerExecution();
        }
        else
        {
            QMessageBox::warning(this, "Invalid Input", "Use format hh:mm:ss or +minutes");
        }
    }
}

void standUP::onToggleGoStop()
{
    if (isRunning_)
    {
        stopTimer();
    }
    else
    {
        expireDateTime_ = QDateTime::currentDateTime().addSecs(900);
        startTimerExecution();
    }
}

void standUP::onTogglePauseResume()
{
    if (!isRunning_)
    {
        return;
    }

    if (!isPaused_)
    {
        isPaused_ = true;
        remainingSeconds_ = QDateTime::currentDateTime().secsTo(expireDateTime_);
        mainScreen_->pauseResumeButton()->setText("Resume");
        mainScreen_->statusLabel()->setText("Status: Paused");
    }
    else
    {
        isPaused_ = false;
        expireDateTime_ = QDateTime::currentDateTime().addSecs(remainingSeconds_);
        mainScreen_->pauseResumeButton()->setText("Pause");
        mainScreen_->statusLabel()->setText("Status: Running");
    }
}

void standUP::triggerAlert()
{
    isRunning_ = false;
    mainScreen_->statusLabel()->setText("Time to get out of your chair!");

    audioPlayer_->playAlert();
    systemAlert_->start();

    mainScreen_->goStopButton()->setText("Go");
    mainScreen_->pauseResumeButton()->setEnabled(false);
}

void standUP::onAddMinutes(int minutes)
{
    QDateTime now = QDateTime::currentDateTime();
    if (!isRunning_)
    {
        expireDateTime_ = now.addSecs(minutes * 60);
        startTimerExecution();
    }
    else
    {
        expireDateTime_ = expireDateTime_.addSecs(minutes * 60);
        mainScreen_->expireTimeLabel()->setText("Expire Time: " + expireDateTime_.toString("HH:mm:ss"));
    }
}

void standUP::saveSettings()
{
    settings_ = configPanel_->settings();
    audioPlayer_->configure(settings_);
    systemAlert_->configure(settings_, normalStyleSheet_);
    applyTheme();
    showMainScreen();
}

void standUP::showSettings()
{
    configPanel_->applySettings(settings_);
    stackedWidget_->setCurrentIndex(1);
}

void standUP::showMainScreen()
{
    stackedWidget_->setCurrentIndex(0);
}

void standUP::startTimerExecution()
{
    isRunning_ = true;
    isPaused_ = false;
    mainScreen_->goStopButton()->setText("Stop");
    mainScreen_->pauseResumeButton()->setEnabled(true);
    mainScreen_->pauseResumeButton()->setText("Pause");
    mainScreen_->expireTimeLabel()->setText("Expire Time: " + expireDateTime_.toString("HH:mm:ss"));
}

void standUP::stopTimer()
{
    isRunning_ = false;
    isPaused_ = false;
    systemAlert_->stop();
    mainScreen_->goStopButton()->setText("Go");
    mainScreen_->pauseResumeButton()->setText("Pause");
    mainScreen_->pauseResumeButton()->setEnabled(false);
    mainScreen_->statusLabel()->setText("Status: Idle");
    mainScreen_->expireTimeLabel()->setText("Expire Time: --:--:--");
}

void standUP::applyTheme()
{
    QString qss;
    if (settings_.theme == "Dark")
    {
        qss = "QWidget { background-color: #2b2b2b; color: #ffffff; }"
              "QPushButton { background-color: #3c3f41; color: #ffffff; border: 1px solid #555555; padding: 6px; border-radius: 4px; }"
              "QPushButton:hover { background-color: #484b4d; }"
              "QLineEdit, QComboBox { background-color: #313335; color: #ffffff; border: 1px solid #555555; padding: 4px; }"
              "QCheckBox { color: #ffffff; }";
    }
    else if (settings_.theme == "Light")
    {
        qss = "QWidget { background-color: #f0f0f0; color: #000000; }"
              "QPushButton { background-color: #e1e1e1; color: #000000; border: 1px solid #adadad; padding: 6px; border-radius: 4px; }"
              "QPushButton:hover { background-color: #e5f1fb; }"
              "QLineEdit, QComboBox { background-color: #ffffff; color: #000000; border: 1px solid #707070; padding: 4px; }"
              "QCheckBox { color: #000000; }";
    }
    else if (settings_.theme == "Red")
    {
        qss = "QWidget { background-color: #3a1c1c; color: #ffcccc; }"
              "QPushButton { background-color: #5c2c2c; color: #ffffff; border: 1px solid #8c3c3c; padding: 6px; border-radius: 4px; }"
              "QPushButton:hover { background-color: #7c3c3c; }"
              "QLineEdit, QComboBox { background-color: #4a2424; color: #ffffff; border: 1px solid #8c3c3c; padding: 4px; }"
              "QCheckBox { color: #ffcccc; }";
    }
    else if (settings_.theme == "Green")
    {
        qss = "QWidget { background-color: #1c3a1c; color: #ccffcc; }"
              "QPushButton { background-color: #2c5c2c; color: #ffffff; border: 1px solid #3c8c3c; padding: 6px; border-radius: 4px; }"
              "QPushButton:hover { background-color: #3c7c3c; }"
              "QLineEdit, QComboBox { background-color: #244a24; color: #ffffff; border: 1px solid #3c8c3c; padding: 4px; }"
              "QCheckBox { color: #ccffcc; }";
    }

    normalStyleSheet_ = qss;
    setStyleSheet(qss);
    systemAlert_->configure(settings_, normalStyleSheet_);
}
