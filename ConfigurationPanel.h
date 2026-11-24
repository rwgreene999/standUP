#pragma once

#include <QWidget>

#include "AppSettings.h"

class QCheckBox;
class QComboBox;
class QEvent;
class QLabel;
class QLineEdit;
class QObject;
class QPushButton;

class ConfigurationPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurationPanel(QWidget *parent = nullptr);

    AppSettings settings() const;
    void applySettings(const AppSettings &settings);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void saveRequested();
    void applyRequested();
    void cancelRequested();
    void soundPreviewRequested(const QString &filePath);
    void themePreviewRequested(const QString &theme);

private:
    QComboBox *comboSound_ = nullptr;
    QLineEdit *txtSoundFile_ = nullptr;
    QPushButton *btnBrowseSound_ = nullptr;
    QCheckBox *chkFlash_ = nullptr;
    QComboBox *comboTheme_ = nullptr;
    QPushButton *btnSave_ = nullptr;
    QPushButton *btnApply_ = nullptr;
    QPushButton *btnCancel_ = nullptr;
    QLabel *lblButtonHint_ = nullptr;

    void browseForSound();
    void updateSoundControlsState();
};
