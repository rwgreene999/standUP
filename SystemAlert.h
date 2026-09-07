#pragma once

#include <QObject>
#include <QTimer>
#include <QWidget>

#include "AppSettings.h"

class SystemAlert : public QObject
{
    Q_OBJECT
public:
    explicit SystemAlert(QWidget *targetWindow, QObject *parent = nullptr);

    void configure(const AppSettings &settings, const QString &normalStyleSheet);
    void start();
    void stop();

private slots:
    void toggleFlash();

private:
    QWidget *targetWindow_ = nullptr;
    QTimer *flashTimer_ = nullptr;
    bool flashState_ = false;
    bool flashEnabled_ = false;
    bool topmost_ = false;
    QString normalStyleSheet_;
};
