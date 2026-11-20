#include "SystemAlert.h"

SystemAlert::SystemAlert(QWidget *targetWindow, QObject *parent)
    : QObject(parent), targetWindow_(targetWindow)
{
    flashTimer_ = new QTimer(this);
    connect(flashTimer_, &QTimer::timeout, this, &SystemAlert::toggleFlash);
}

void SystemAlert::configure(const AppSettings &settings, const QString &normalStyleSheet)
{
    // An alert always takes over the screen by design; only the flash and
    // theme parts of the notification are configurable.
    flashEnabled_ = settings.flashScreen;
    normalStyleSheet_ = normalStyleSheet;
}

void SystemAlert::start()
{
    isAlerting_ = true;

    // Always make the alert window topmost. Changing flags on a visible
    // top-level widget hides/recreates it, so do this before showing again.
    const Qt::WindowFlags flags = targetWindow_->windowFlags();
    if (!(flags & Qt::WindowStaysOnTopHint))
    {
        targetWindow_->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    }

    // Restore from minimized and make sure it is visible after the flag change.
    if (targetWindow_->isMinimized())
    {
        targetWindow_->showNormal();
    }
    else
    {
        targetWindow_->show();
    }

    // These are the parts that actually put it in front of the user.
    targetWindow_->raise();
    targetWindow_->activateWindow();

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
    isAlerting_ = false;
    flashTimer_->stop();
    flashState_ = false;

    // Acknowledging/dismissing the alert releases the topmost state so the
    // window can drift back into the background.
    const Qt::WindowFlags flags = targetWindow_->windowFlags();
    if (flags & Qt::WindowStaysOnTopHint)
    {
        targetWindow_->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);
        targetWindow_->show();
    }

    targetWindow_->setWindowOpacity(1.0);
    targetWindow_->setStyleSheet(normalStyleSheet_);
}

void SystemAlert::toggleFlash()
{
    flashState_ = !flashState_;
    targetWindow_->setStyleSheet(flashState_ ? "background-color: yellow; color: black;"
                                             : normalStyleSheet_);
}
