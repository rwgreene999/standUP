#pragma once

#include <QDateTime>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "AppSettings.h"

class MainScreen;
class ConfigurationPanel;
class AudioPlayer;
class SystemAlert;

class standUP : public QWidget
{
    Q_OBJECT
public:
    explicit standUP(QWidget *parent = nullptr);

private slots:
    void updateClock();
    void onApplyInputTime();
    void onToggleGoStop();
    void onTogglePauseResume();
    void triggerAlert();
    void onAddMinutes(int minutes);
    void saveSettings();
    void showSettings();
    void showMainScreen();

private:
    void startTimerExecution();
    void stopTimer();
    void applyTheme();

    QStackedWidget *stackedWidget_ = nullptr;
    MainScreen *mainScreen_ = nullptr;
    ConfigurationPanel *configPanel_ = nullptr;
    QTimer *clockTimer_ = nullptr;
    QTimer *countdownTimer_ = nullptr;
    QDateTime expireDateTime_;
    bool isRunning_ = false;
    bool isPaused_ = false;
    qint64 remainingSeconds_ = 0;

    AppSettings settings_;
    AudioPlayer *audioPlayer_ = nullptr;
    SystemAlert *systemAlert_ = nullptr;
    QString normalStyleSheet_;
};
