#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QStackedWidget;
class QToolButton;

class MainScreen : public QWidget
{
    Q_OBJECT
public:
    explicit MainScreen(QWidget *parent = nullptr);

    QLineEdit *timeInput() const;
    QPushButton *goStopButton() const;
    QPushButton *pauseResumeButton() const;
    QToolButton *settingsButton() const;
    QLabel *currentTimeLabel() const;
    QLabel *expireTimeLabel() const;
    QLabel *statusLabel() const;
    QPushButton *addButton10() const;
    QPushButton *addButton30() const;
    QPushButton *addButton1Hour() const;
    QPushButton *addButton2Hours() const;
    QPushButton *stopwatchButton() const;
    QPushButton *beginStopwatchButton() const;

signals:
    void applyInputRequested();
    void goStopRequested();
    void pauseResumeRequested();
    void settingsRequested();
    void addMinutesRequested(int minutes);
    void stopwatchRequested();
    void beginStopwatchRequested();

private:
    QLineEdit *txtInputTime_ = nullptr;
    QPushButton *btnGoStop_ = nullptr;
    QPushButton *btnPauseResume_ = nullptr;
    QToolButton *btnSettings_ = nullptr;
    QPushButton *btnStopwatch_ = nullptr;
    QPushButton *btnBeginStopwatch_ = nullptr;
    QLabel *lblCurrentTime_ = nullptr;
    QLabel *lblExpireTime_ = nullptr;
    QLabel *lblStatus_ = nullptr;
    QPushButton *btnAdd10_ = nullptr;
    QPushButton *btnAdd30_ = nullptr;
    QPushButton *btnAdd1Hour_ = nullptr;
    QPushButton *btnAdd2Hours_ = nullptr;
};
