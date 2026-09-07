#pragma once

#include <QWidget>

#include "AppSettings.h"

class QComboBox;
class QLineEdit;
class QPushButton;
class QCheckBox;

class ConfigurationPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurationPanel(QWidget *parent = nullptr);

    AppSettings settings() const;
    void applySettings(const AppSettings &settings);

signals:
    void saveRequested();
    void soundPreviewRequested(const QString &filePath);

private:
    QComboBox *comboSound_ = nullptr;
    QLineEdit *txtSoundFile_ = nullptr;
    QPushButton *btnBrowseSound_ = nullptr;
    QCheckBox *chkTopmost_ = nullptr;
    QCheckBox *chkFlash_ = nullptr;
    QComboBox *comboTheme_ = nullptr;
    QPushButton *btnBackSettings_ = nullptr;

    void browseForSound();
};
