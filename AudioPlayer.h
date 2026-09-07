#pragma once

#include <QObject>

#include "AppSettings.h"

class QProcess;
class QSoundEffect;

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

private:
    bool launchExternalPlayer(const QString &filePath);
    QString chooseBestPlayer() const;
    QStringList buildPlayerArgs(const QString &playerName, const QString &filePath) const;

    QSoundEffect *soundEffect_ = nullptr;
    QProcess *playerProcess_ = nullptr;
    AppSettings settings_;
};
