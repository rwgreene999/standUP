#include "AudioPlayer.h"

#include <QApplication>
#include <QProcess>
#include <QSoundEffect>
#include <QStandardPaths>
#include <QUrl>

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent)
{
    soundEffect_ = new QSoundEffect(this);
    playerProcess_ = nullptr;
}

AudioPlayer::~AudioPlayer()
{
    stopPlayback();
}

void AudioPlayer::configure(const AppSettings &settings)
{
    settings_ = settings;
}

void AudioPlayer::stopPlayback()
{
    if (playerProcess_)
    {
        playerProcess_->terminate();
        if (playerProcess_->state() != QProcess::NotRunning)
        {
            playerProcess_->kill();
        }
        playerProcess_->deleteLater();
        playerProcess_ = nullptr;
    }

    if (soundEffect_)
    {
        soundEffect_->stop();
    }
}

bool AudioPlayer::launchExternalPlayer(const QString &filePath)
{
    const QString playerName = chooseBestPlayer();
    if (playerName.isEmpty())
    {
        return false;
    }

    stopPlayback();

    playerProcess_ = new QProcess(this);
    playerProcess_->start(playerName, buildPlayerArgs(playerName, filePath));

    if (!playerProcess_->waitForStarted(2000))
    {
        playerProcess_->deleteLater();
        playerProcess_ = nullptr;
        return false;
    }

    return true;
}

QString AudioPlayer::chooseBestPlayer() const
{
    const QStringList candidates = {"ffplay", "vlc", "cvlc", "mpg123", "mpg321", "mplayer"};
    for (const QString &candidate : candidates)
    {
        if (!QStandardPaths::findExecutable(candidate).isEmpty())
        {
            return candidate;
        }
    }
    return {};
}

QStringList AudioPlayer::buildPlayerArgs(const QString &playerName, const QString &filePath) const
{
    const QString safePath = QUrl::fromLocalFile(filePath).toLocalFile();

    if (playerName == "ffplay")
    {
        return {"-nodisp", "-autoexit", "-loglevel", "error", safePath};
    }
    if (playerName == "mpg123" || playerName == "mpg321")
    {
        return {"-q", safePath};
    }
    if (playerName == "vlc" || playerName == "cvlc")
    {
        return {"--intf", "dummy", "--play-and-exit", safePath};
    }
    if (playerName == "mplayer")
    {
        return {"-really-quiet", "-novideo", safePath};
    }

    return {safePath};
}

void AudioPlayer::playAlert()
{
    if (settings_.soundType == "Silent")
    {
        return;
    }

    if (settings_.soundType == "Default Beep")
    {
        QApplication::beep();
        return;
    }

    if (settings_.soundType == "Custom Audio File" && !settings_.customSoundPath.isEmpty())
    {
        launchExternalPlayer(settings_.customSoundPath);
    }
}

void AudioPlayer::playPreview(const QString &filePath)
{
    if (filePath.isEmpty())
    {
        return;
    }

    if (launchExternalPlayer(filePath))
    {
        return;
    }

    soundEffect_->setSource(QUrl::fromLocalFile(filePath));
    soundEffect_->play();
}
