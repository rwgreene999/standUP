#include "ConfigurationPanel.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

ConfigurationPanel::ConfigurationPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);

    auto *lblTitle = new QLabel("Settings", this);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(lblTitle);

    layout->addWidget(new QLabel("Notification Sound:", this));
    comboSound_ = new QComboBox(this);
    comboSound_->addItems({"Default Beep", "Silent", "Custom Audio File"});
    layout->addWidget(comboSound_);

    auto *fileLayout = new QHBoxLayout();
    txtSoundFile_ = new QLineEdit(this);
    txtSoundFile_->setPlaceholderText("Path to WAV / MP3");
    btnBrowseSound_ = new QPushButton("Browse", this);
    fileLayout->addWidget(txtSoundFile_);
    fileLayout->addWidget(btnBrowseSound_);
    layout->addLayout(fileLayout);

    connect(btnBrowseSound_, &QPushButton::clicked, this, &ConfigurationPanel::browseForSound);

    layout->addSpacing(10);
    layout->addWidget(new QLabel("Visual Notifications:", this));
    chkTopmost_ = new QCheckBox("Make window topmost on alert", this);
    chkFlash_ = new QCheckBox("Flash screen/window on alert", this);
    chkFlash_->setChecked(true);
    layout->addWidget(chkTopmost_);
    layout->addWidget(chkFlash_);

    layout->addSpacing(10);
    layout->addWidget(new QLabel("Color Theme:", this));
    comboTheme_ = new QComboBox(this);
    comboTheme_->addItems({"Dark", "Light", "Red", "Green"});
    layout->addWidget(comboTheme_);

    layout->addStretch();

    btnBackSettings_ = new QPushButton("Save & Back", this);
    layout->addWidget(btnBackSettings_);

    connect(btnBackSettings_, &QPushButton::clicked, this, &ConfigurationPanel::saveRequested);
}

AppSettings ConfigurationPanel::settings() const
{
    AppSettings settings;
    settings.soundType = comboSound_->currentText();
    settings.customSoundPath = txtSoundFile_->text();
    settings.topmost = chkTopmost_->isChecked();
    settings.flashScreen = chkFlash_->isChecked();
    settings.theme = comboTheme_->currentText();
    return settings;
}

void ConfigurationPanel::applySettings(const AppSettings &settings)
{
    comboSound_->setCurrentText(settings.soundType);
    txtSoundFile_->setText(settings.customSoundPath);
    chkTopmost_->setChecked(settings.topmost);
    chkFlash_->setChecked(settings.flashScreen);
    comboTheme_->setCurrentText(settings.theme);
}

void ConfigurationPanel::browseForSound()
{
    const QString fileName = QFileDialog::getOpenFileName(this, "Open Audio File", "", "Audio Files (*.wav *.mp3)");
    if (!fileName.isEmpty())
    {
        txtSoundFile_->setText(fileName);
        comboSound_->setCurrentText("Custom Audio File");
        emit soundPreviewRequested(fileName);
    }
}
