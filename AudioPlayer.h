#pragma once

#include <QMap>
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
    void playPreview(const QString &soundTypeOrPath);
    void stopPlayback();
    bool isPlaying() const;

private:
    void playFile(const QString &filePath);
    void playBuiltInSound(const QString &soundName);
    QString ensureBuiltInSoundFile(const QString &soundName);

    QMediaPlayer *player_ = nullptr;
    QAudioOutput *audioOutput_ = nullptr;
    AppSettings settings_;
    QMap<QString, QString> builtInSoundPaths_;
};
