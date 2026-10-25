#include "ConfigurationPanel.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalBlocker>
#include <QVBoxLayout>

ConfigurationPanel::ConfigurationPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(12, 12, 12, 12);

    auto *lblTitle = new QLabel("Settings", this);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(lblTitle);

    layout->addWidget(new QLabel("Notification Sound:", this));
    comboSound_ = new QComboBox(this);
    comboSound_->addItems({"High Beep", "Low Beep", "Wabble", "Fairy Walk", "Whoop Whoop", "Twinkle", "Silent", "Custom Audio File"});
    layout->addWidget(comboSound_);

    auto *fileLayout = new QHBoxLayout();
    txtSoundFile_ = new QLineEdit(this);
    txtSoundFile_->setPlaceholderText("Path to WAV / MP3 / OGG / FLAC");
    btnBrowseSound_ = new QPushButton("Browse", this);
    fileLayout->addWidget(txtSoundFile_);
    fileLayout->addWidget(btnBrowseSound_);
    layout->addLayout(fileLayout);

    connect(comboSound_, &QComboBox::currentTextChanged, this, [this](const QString &soundType) {
        updateSoundControlsState();
        if (soundType != "Silent" && soundType != "Custom Audio File")
        {
            emit soundPreviewRequested(soundType);
        }
        else if (soundType == "Custom Audio File" && !txtSoundFile_->text().isEmpty())
        {
            emit soundPreviewRequested(txtSoundFile_->text());
        }
    });

    connect(btnBrowseSound_, &QPushButton::clicked, this, &ConfigurationPanel::browseForSound);
    updateSoundControlsState();

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
    comboTheme_->addItems({"Dark", "Light", "Red", "Green", "Sunset", "Sunrise"});
    layout->addWidget(comboTheme_);

    connect(comboTheme_, &QComboBox::currentTextChanged, this, &ConfigurationPanel::themePreviewRequested);

    layout->addStretch();

    auto *buttonLayout = new QHBoxLayout();
    btnSave_ = new QPushButton("Save", this);
    btnApply_ = new QPushButton("Apply", this);
    btnCancel_ = new QPushButton("Cancel", this);

    buttonLayout->addWidget(btnSave_);
    buttonLayout->addWidget(btnApply_);
    buttonLayout->addWidget(btnCancel_);
    layout->addLayout(buttonLayout);

    connect(btnSave_, &QPushButton::clicked, this, &ConfigurationPanel::saveRequested);
    connect(btnApply_, &QPushButton::clicked, this, &ConfigurationPanel::applyRequested);
    connect(btnCancel_, &QPushButton::clicked, this, &ConfigurationPanel::cancelRequested);
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
    const QSignalBlocker themeBlocker(comboTheme_);
    const QSignalBlocker soundBlocker(comboSound_);
    QString soundType = settings.soundType;
    if (soundType.compare("Default Beep", Qt::CaseInsensitive) == 0)
    {
        soundType = "High Beep";
    }
    else if (soundType.compare("whoop whoop", Qt::CaseInsensitive) == 0)
    {
        soundType = "Whoop Whoop";
    }
    else if (soundType.compare("twinkle", Qt::CaseInsensitive) == 0)
    {
        soundType = "Twinkle";
    }
    comboSound_->setCurrentText(soundType);
    txtSoundFile_->setText(settings.customSoundPath);
    chkTopmost_->setChecked(settings.topmost);
    chkFlash_->setChecked(settings.flashScreen);
    comboTheme_->setCurrentText(settings.theme);
    updateSoundControlsState();
}

void ConfigurationPanel::updateSoundControlsState()
{
    const bool isCustom = (comboSound_->currentText() == "Custom Audio File");
    txtSoundFile_->setEnabled(isCustom);
    btnBrowseSound_->setEnabled(isCustom);
}

void ConfigurationPanel::browseForSound()
{
    const QString fileName = QFileDialog::getOpenFileName(this, "Open Audio File", "", "Audio Files (*.wav *.mp3 *.ogg *.flac);;All Files (*)");
    if (!fileName.isEmpty())
    {
        txtSoundFile_->setText(fileName);
        comboSound_->setCurrentText("Custom Audio File");
        emit soundPreviewRequested(fileName);
    }
}
