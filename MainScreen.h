#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QStackedWidget;

class MainScreen : public QWidget
{
    Q_OBJECT
public:
    explicit MainScreen(QWidget *parent = nullptr);

    QLineEdit *timeInput() const;
    QPushButton *goStopButton() const;
    QPushButton *pauseResumeButton() const;
    QPushButton *settingsButton() const;
    QLabel *currentTimeLabel() const;
    QLabel *expireTimeLabel() const;
    QLabel *statusLabel() const;
    QPushButton *addButton1() const;
    QPushButton *addButton5() const;
    QPushButton *addButton10() const;
    QPushButton *addButton30() const;

signals:
    void applyInputRequested();
    void goStopRequested();
    void pauseResumeRequested();
    void settingsRequested();
    void addMinutesRequested(int minutes);

private:
    QLineEdit *txtInputTime_ = nullptr;
    QPushButton *btnGoStop_ = nullptr;
    QPushButton *btnPauseResume_ = nullptr;
    QPushButton *btnSettings_ = nullptr;
    QLabel *lblCurrentTime_ = nullptr;
    QLabel *lblExpireTime_ = nullptr;
    QLabel *lblStatus_ = nullptr;
    QPushButton *btnAdd1_ = nullptr;
    QPushButton *btnAdd5_ = nullptr;
    QPushButton *btnAdd10_ = nullptr;
    QPushButton *btnAdd30_ = nullptr;
};
