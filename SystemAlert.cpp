#include "SystemAlert.h"

SystemAlert::SystemAlert(QWidget *targetWindow, QObject *parent)
    : QObject(parent), targetWindow_(targetWindow)
{
    flashTimer_ = new QTimer(this);
    connect(flashTimer_, &QTimer::timeout, this, &SystemAlert::toggleFlash);
}

void SystemAlert::configure(const AppSettings &settings, const QString &normalStyleSheet)
{
    topmost_ = settings.topmost;
    flashEnabled_ = settings.flashScreen;
    normalStyleSheet_ = normalStyleSheet;
}

void SystemAlert::start()
{
    if (topmost_)
    {
        targetWindow_->setWindowFlags(targetWindow_->windowFlags() | Qt::WindowStaysOnTopHint);
        targetWindow_->show();
    }

    if (flashEnabled_)
    {
        flashTimer_->start(500);
        toggleFlash();
    }
    else
    {
        targetWindow_->setStyleSheet(normalStyleSheet_);
    }
}

void SystemAlert::stop()
{
    flashTimer_->stop();
    flashState_ = false;
    targetWindow_->setWindowOpacity(1.0);
    targetWindow_->setStyleSheet(normalStyleSheet_);
}

void SystemAlert::toggleFlash()
{
    flashState_ = !flashState_;
    targetWindow_->setStyleSheet(flashState_ ? "background-color: yellow; color: black;"
                                             : normalStyleSheet_);
}
