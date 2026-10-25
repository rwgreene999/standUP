#pragma once

#include <QObject>

#include "AppSettings.h"

class QAudioOutput;
class QMediaPlayer;

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer() override;

    void configure(const AppSettings &settings);
    void playAlert();
    void playPreview(const QString &filePath);
    void stopPlayback();
    bool isPlaying() const;

private:
    void playFile(const QString &filePath);
    void playDefaultBeep();
    QString ensureDefaultBeepFile();

    QMediaPlayer *player_ = nullptr;
    QAudioOutput *audioOutput_ = nullptr;
    AppSettings settings_;
    QString defaultBeepPath_;
};
