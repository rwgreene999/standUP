#include "MainScreen.h"

#include <QFontMetrics>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>

namespace
{
QIcon drawnGearIcon()
{
    const int size = 64;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(140, 140, 140));
    painter.translate(size / 2.0, size / 2.0);

    const qreal body = size * 0.30;
    const qreal tip = size * 0.44;
    const qreal hole = size * 0.12;
    const qreal toothWidth = size * 0.13;
    const int teeth = 8;

    for (int i = 0; i < teeth; ++i)
    {
        painter.save();
        painter.rotate(360.0 * i / teeth);
        painter.drawRoundedRect(
            QRectF(-toothWidth / 2.0, -tip, toothWidth, tip - body + size * 0.03),
            size * 0.02, size * 0.02);
        painter.restore();
    }

    QPainterPath path;
    path.setFillRule(Qt::OddEvenFill);
    path.addEllipse(QPointF(0.0, 0.0), body, body);
    path.addEllipse(QPointF(0.0, 0.0), hole, hole);
    painter.drawPath(path);

    return QIcon(pixmap);
}
} // namespace

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

    btnSettings_ = new QToolButton(this);
    btnSettings_->setAutoRaise(true);
    btnSettings_->setFixedSize(26, 26);
    btnSettings_->setToolTip("Settings");
    btnSettings_->setAccessibleName("Settings");
    btnSettings_->setCursor(Qt::PointingHandCursor);
    btnSettings_->setStyleSheet(
        "QToolButton { background: transparent; border: none; padding: 0; }"
        "QToolButton:hover { background: rgba(128, 128, 128, 0.25); border-radius: 4px; }");

    QFont gearFont = btnSettings_->font();
    if (gearFont.pointSizeF() > 0.0)
    {
        gearFont.setPointSizeF(gearFont.pointSizeF() * 1.7);
    }
    else
    {
        gearFont.setPixelSize(18);
    }

    const QChar gearChar(0x2699); // ⚙
    if (QFontMetrics(gearFont).inFont(gearChar))
    {
        btnSettings_->setToolButtonStyle(Qt::ToolButtonTextOnly);
        btnSettings_->setText(QString(gearChar));
        btnSettings_->setFont(gearFont);
    }
    else
    {
        btnSettings_->setToolButtonStyle(Qt::ToolButtonIconOnly);
        btnSettings_->setIcon(drawnGearIcon());
        btnSettings_->setIconSize(QSize(18, 18));
    }

    auto *headerLayout = new QHBoxLayout();
    headerLayout->addSpacing(26);
    headerLayout->addWidget(lblCurrentTime_, 1);
    headerLayout->addWidget(btnSettings_);
    layout->addLayout(headerLayout);

    lblExpireTime_ = new QLabel("Expire Time: --:--:--", this);
    lblExpireTime_->setAlignment(Qt::AlignCenter);
    lblExpireTime_->setStyleSheet("font-size: 16px; font-weight: bold;");
    lblExpireTime_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    lblStatus_ = new QLabel("Status: Idle", this);
    lblStatus_->setAlignment(Qt::AlignCenter);
    lblStatus_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

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

    controlLayout->addWidget(btnGoStop_);
    controlLayout->addWidget(btnPauseResume_);
    layout->addLayout(controlLayout);

    connect(btnGoStop_, &QPushButton::clicked, this, &MainScreen::goStopRequested);
    connect(btnPauseResume_, &QPushButton::clicked, this, &MainScreen::pauseResumeRequested);
    connect(btnSettings_, &QToolButton::clicked, this, &MainScreen::settingsRequested);

    btnPauseResume_->setEnabled(false);

    // Thin rule separating the timer controls from the stopwatch utilities.
    auto *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Plain);
    separator->setFixedHeight(1);
    separator->setStyleSheet("QFrame { background: rgba(128, 128, 128, 0.35); border: none; }");
    layout->addSpacing(4);
    layout->addWidget(separator);
    layout->addSpacing(2);

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
QToolButton *MainScreen::settingsButton() const { return btnSettings_; }
QPushButton *MainScreen::stopwatchButton() const { return btnStopwatch_; }
QPushButton *MainScreen::beginStopwatchButton() const { return btnBeginStopwatch_; }
QLabel *MainScreen::currentTimeLabel() const { return lblCurrentTime_; }
QLabel *MainScreen::expireTimeLabel() const { return lblExpireTime_; }
QLabel *MainScreen::statusLabel() const { return lblStatus_; }
QPushButton *MainScreen::addButton10() const { return btnAdd10_; }
QPushButton *MainScreen::addButton30() const { return btnAdd30_; }
QPushButton *MainScreen::addButton1Hour() const { return btnAdd1Hour_; }
QPushButton *MainScreen::addButton2Hours() const { return btnAdd2Hours_; }
