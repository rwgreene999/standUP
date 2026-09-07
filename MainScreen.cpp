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

    lblCurrentTime_ = new QLabel("Current Time: --:--:--", this);
    lblCurrentTime_->setAlignment(Qt::AlignCenter);
    lblCurrentTime_->setStyleSheet("font-size: 16px; font-weight: bold;");

    lblExpireTime_ = new QLabel("Expire Time: --:--:--", this);
    lblExpireTime_->setAlignment(Qt::AlignCenter);
    lblExpireTime_->setStyleSheet("font-size: 16px; font-weight: bold;");

    lblStatus_ = new QLabel("Status: Idle", this);
    lblStatus_->setAlignment(Qt::AlignCenter);

    layout->addWidget(lblCurrentTime_);
    layout->addWidget(lblExpireTime_);
    layout->addWidget(lblStatus_);
    layout->addSpacing(10);

    auto *inputLayout = new QHBoxLayout();
    txtInputTime_ = new QLineEdit(this);
    txtInputTime_->setPlaceholderText("hh:mm:ss or +mins");
    auto *btnApplyInput = new QPushButton("Set Time", this);
    inputLayout->addWidget(txtInputTime_);
    inputLayout->addWidget(btnApplyInput);
    layout->addLayout(inputLayout);

    connect(btnApplyInput, &QPushButton::clicked, this, &MainScreen::applyInputRequested);

    auto *gridButtons = new QGridLayout();
    btnAdd1_ = new QPushButton("+1 Min", this);
    btnAdd5_ = new QPushButton("+5 Mins", this);
    btnAdd10_ = new QPushButton("+10 Mins", this);
    btnAdd30_ = new QPushButton("+30 Mins", this);

    gridButtons->addWidget(btnAdd1_, 0, 0);
    gridButtons->addWidget(btnAdd5_, 0, 1);
    gridButtons->addWidget(btnAdd10_, 1, 0);
    gridButtons->addWidget(btnAdd30_, 1, 1);
    layout->addLayout(gridButtons);

    connect(btnAdd1_, &QPushButton::clicked, this, [this]()
            { emit addMinutesRequested(1); });
    connect(btnAdd5_, &QPushButton::clicked, this, [this]()
            { emit addMinutesRequested(5); });
    connect(btnAdd10_, &QPushButton::clicked, this, [this]()
            { emit addMinutesRequested(10); });
    connect(btnAdd30_, &QPushButton::clicked, this, [this]()
            { emit addMinutesRequested(30); });

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
}

QLineEdit *MainScreen::timeInput() const { return txtInputTime_; }
QPushButton *MainScreen::goStopButton() const { return btnGoStop_; }
QPushButton *MainScreen::pauseResumeButton() const { return btnPauseResume_; }
QPushButton *MainScreen::settingsButton() const { return btnSettings_; }
QLabel *MainScreen::currentTimeLabel() const { return lblCurrentTime_; }
QLabel *MainScreen::expireTimeLabel() const { return lblExpireTime_; }
QLabel *MainScreen::statusLabel() const { return lblStatus_; }
QPushButton *MainScreen::addButton1() const { return btnAdd1_; }
QPushButton *MainScreen::addButton5() const { return btnAdd5_; }
QPushButton *MainScreen::addButton10() const { return btnAdd10_; }
QPushButton *MainScreen::addButton30() const { return btnAdd30_; }
