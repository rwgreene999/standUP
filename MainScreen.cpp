#include "MainScreen.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

MainScreen::MainScreen(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(12, 12, 12, 12);

    lblCurrentTime_ = new QLabel("Current Time: --:--:--", this);
    lblCurrentTime_->setAlignment(Qt::AlignCenter);
    lblCurrentTime_->setStyleSheet("font-size: 16px; font-weight: bold;");
    lblCurrentTime_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    lblExpireTime_ = new QLabel("Expire Time: --:--:--", this);
    lblExpireTime_->setAlignment(Qt::AlignCenter);
    lblExpireTime_->setStyleSheet("font-size: 16px; font-weight: bold;");
    lblExpireTime_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    lblStatus_ = new QLabel("Status: Idle", this);
    lblStatus_->setAlignment(Qt::AlignCenter);
    lblStatus_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    layout->addWidget(lblCurrentTime_);
    layout->addWidget(lblExpireTime_);
    layout->addWidget(lblStatus_);
    layout->addSpacing(4);

    auto *gridButtons = new QGridLayout();
    btnAdd10_ = new QPushButton("+10 Mins", this);
    btnAdd30_ = new QPushButton("+30 Mins", this);
    btnAdd1Hour_ = new QPushButton("+1 Hour", this);
    btnAdd2Hours_ = new QPushButton("+2 Hours", this);

    gridButtons->addWidget(btnAdd10_, 0, 0);
    gridButtons->addWidget(btnAdd30_, 0, 1);
    gridButtons->addWidget(btnAdd1Hour_, 1, 0);
    gridButtons->addWidget(btnAdd2Hours_, 1, 1);
    layout->addLayout(gridButtons);

    connect(btnAdd10_, &QPushButton::clicked, this, [this]()
            { emit addMinutesRequested(10); });
    connect(btnAdd30_, &QPushButton::clicked, this, [this]()
            { emit addMinutesRequested(30); });
    connect(btnAdd1Hour_, &QPushButton::clicked, this, [this]()
            { emit addMinutesRequested(60); });
    connect(btnAdd2Hours_, &QPushButton::clicked, this, [this]()
            { emit addMinutesRequested(120); });

    auto *inputLayout = new QHBoxLayout();
    txtInputTime_ = new QLineEdit(this);
    txtInputTime_->setPlaceholderText("hh:mm:ss or +mins");
    auto *btnApplyInput = new QPushButton("Set Time", this);
    inputLayout->addWidget(txtInputTime_);
    inputLayout->addWidget(btnApplyInput);
    layout->addLayout(inputLayout);

    connect(btnApplyInput, &QPushButton::clicked, this, &MainScreen::applyInputRequested);
    connect(txtInputTime_, &QLineEdit::returnPressed, this, &MainScreen::applyInputRequested);

    auto *controlLayout = new QHBoxLayout();
    btnGoStop_ = new QPushButton("Go", this);
    btnPauseResume_ = new QPushButton("Pause", this);
    btnSettings_ = new QPushButton("Settings", this);

    controlLayout->addWidget(btnGoStop_);
    controlLayout->addWidget(btnPauseResume_);
    controlLayout->addWidget(btnSettings_);
    layout->addLayout(controlLayout);

    connect(btnGoStop_, &QPushButton::clicked, this, &MainScreen::goStopRequested);
    connect(btnPauseResume_, &QPushButton::clicked, this, &MainScreen::pauseResumeRequested);
    connect(btnSettings_, &QPushButton::clicked, this, &MainScreen::settingsRequested);

    btnPauseResume_->setEnabled(false);

    auto *stopwatchLayout = new QHBoxLayout();
    btnStopwatch_ = new QPushButton("Stopwatch", this);
    btnBeginStopwatch_ = new QPushButton("Begin Stopwatch", this);

    stopwatchLayout->addWidget(btnStopwatch_);
    stopwatchLayout->addWidget(btnBeginStopwatch_);
    layout->addLayout(stopwatchLayout);

    connect(btnStopwatch_, &QPushButton::clicked, this, &MainScreen::stopwatchRequested);
    connect(btnBeginStopwatch_, &QPushButton::clicked, this, &MainScreen::beginStopwatchRequested);
}

QLineEdit *MainScreen::timeInput() const { return txtInputTime_; }
QPushButton *MainScreen::goStopButton() const { return btnGoStop_; }
QPushButton *MainScreen::pauseResumeButton() const { return btnPauseResume_; }
QPushButton *MainScreen::settingsButton() const { return btnSettings_; }
QPushButton *MainScreen::stopwatchButton() const { return btnStopwatch_; }
QPushButton *MainScreen::beginStopwatchButton() const { return btnBeginStopwatch_; }
QLabel *MainScreen::currentTimeLabel() const { return lblCurrentTime_; }
QLabel *MainScreen::expireTimeLabel() const { return lblExpireTime_; }
QLabel *MainScreen::statusLabel() const { return lblStatus_; }
QPushButton *MainScreen::addButton10() const { return btnAdd10_; }
QPushButton *MainScreen::addButton30() const { return btnAdd30_; }
QPushButton *MainScreen::addButton1Hour() const { return btnAdd1Hour_; }
QPushButton *MainScreen::addButton2Hours() const { return btnAdd2Hours_; }
