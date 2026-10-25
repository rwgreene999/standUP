#include "AudioPlayer.h"

#include <cmath>
#include <cstring>
#include <cstdint>
#include <QApplication>
#include <QAudioOutput>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QUrl>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent)
{
    player_ = new QMediaPlayer(this);
    audioOutput_ = new QAudioOutput(this);
    audioOutput_->setVolume(1.0f);
    player_->setAudioOutput(audioOutput_);
}

AudioPlayer::~AudioPlayer()
{
    stopPlayback();
}

void AudioPlayer::configure(const AppSettings &settings)
{
    settings_ = settings;
}

void AudioPlayer::playAlert()
{
    if (settings_.soundType == "Silent")
    {
        return;
    }

    if (settings_.soundType == "Default Beep")
    {
        playDefaultBeep();
        return;
    }

    if (settings_.soundType == "Custom Audio File" && !settings_.customSoundPath.isEmpty())
    {
        playFile(settings_.customSoundPath);
    }
}

void AudioPlayer::playPreview(const QString &filePath)
{
    if (filePath.isEmpty() || filePath == "Default Beep")
    {
        playDefaultBeep();
    }
    else
    {
        playFile(filePath);
    }
}

void AudioPlayer::playDefaultBeep()
{
    if (defaultBeepPath_.isEmpty() || !QFile::exists(defaultBeepPath_))
    {
        defaultBeepPath_ = ensureDefaultBeepFile();
    }
    playFile(defaultBeepPath_);
}

QString AudioPlayer::ensureDefaultBeepFile()
{
    const QString beepPath = QDir::temp().filePath("standUP_default_beep.wav");
    if (QFile::exists(beepPath))
    {
        return beepPath;
    }

    const int sampleRate = 44100;
    const double durationSec = 0.35;
    const int frequency = 880;

    const int numSamples = static_cast<int>(durationSec * sampleRate);
    const int dataSize = numSamples * 2; // 16-bit mono
    const int fileSize = 36 + dataSize;

    QByteArray wav;
    wav.resize(44 + dataSize);
    char *ptr = wav.data();

    // RIFF header
    std::memcpy(ptr, "RIFF", 4);
    *reinterpret_cast<uint32_t *>(ptr + 4) = fileSize;
    std::memcpy(ptr + 8, "WAVE", 4);

    // fmt subchunk
    std::memcpy(ptr + 12, "fmt ", 4);
    *reinterpret_cast<uint32_t *>(ptr + 16) = 16;
    *reinterpret_cast<uint16_t *>(ptr + 20) = 1; // PCM
    *reinterpret_cast<uint16_t *>(ptr + 22) = 1; // Mono
    *reinterpret_cast<uint32_t *>(ptr + 24) = sampleRate;
    *reinterpret_cast<uint32_t *>(ptr + 28) = sampleRate * 2; // Byte rate
    *reinterpret_cast<uint16_t *>(ptr + 32) = 2; // Block align
    *reinterpret_cast<uint16_t *>(ptr + 34) = 16; // Bits per sample

    // data subchunk
    std::memcpy(ptr + 36, "data", 4);
    *reinterpret_cast<uint32_t *>(ptr + 40) = dataSize;

    int16_t *samples = reinterpret_cast<int16_t *>(ptr + 44);
    const int attackSamples = static_cast<int>(sampleRate * 0.01); // 10ms attack
    const int decaySamples = static_cast<int>(sampleRate * 0.05);  // 50ms decay
    const double amplitude = 26000.0;

    for (int i = 0; i < numSamples; ++i)
    {
        const double t = static_cast<double>(i) / sampleRate;
        double env = 1.0;
        if (i < attackSamples)
        {
            env = static_cast<double>(i) / attackSamples;
        }
        else if (i > numSamples - decaySamples)
        {
            env = static_cast<double>(numSamples - i) / decaySamples;
        }
        samples[i] = static_cast<int16_t>(amplitude * env * std::sin(2.0 * M_PI * frequency * t));
    }

    QFile file(beepPath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(wav);
        file.close();
    }

    return beepPath;
}

void AudioPlayer::playFile(const QString &filePath)
{
    if (filePath.isEmpty())
    {
        return;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile())
    {
        return;
    }

    stopPlayback();
    player_->setSource(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
    player_->play();
}

void AudioPlayer::stopPlayback()
{
    if (player_)
    {
        player_->stop();
    }
}

bool AudioPlayer::isPlaying() const
{
    return player_ && player_->playbackState() == QMediaPlayer::PlayingState;
}
