#include "AudioPlayer.h"

#include <cmath>
#include <cstring>
#include <cstdint>
#include <vector>
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

namespace
{
QByteArray createWav(const std::vector<int16_t> &samples, int sampleRate = 44100)
{
    const int dataSize = static_cast<int>(samples.size() * sizeof(int16_t));
    const int fileSize = 36 + dataSize;
    QByteArray wav;
    wav.resize(44 + dataSize);
    char *ptr = wav.data();

    std::memcpy(ptr, "RIFF", 4);
    *reinterpret_cast<uint32_t *>(ptr + 4) = fileSize;
    std::memcpy(ptr + 8, "WAVE", 4);
    std::memcpy(ptr + 12, "fmt ", 4);
    *reinterpret_cast<uint32_t *>(ptr + 16) = 16;
    *reinterpret_cast<uint16_t *>(ptr + 20) = 1;
    *reinterpret_cast<uint16_t *>(ptr + 22) = 1;
    *reinterpret_cast<uint32_t *>(ptr + 24) = sampleRate;
    *reinterpret_cast<uint32_t *>(ptr + 28) = sampleRate * 2;
    *reinterpret_cast<uint16_t *>(ptr + 32) = 2;
    *reinterpret_cast<uint16_t *>(ptr + 34) = 16;
    std::memcpy(ptr + 36, "data", 4);
    *reinterpret_cast<uint32_t *>(ptr + 40) = dataSize;
    std::memcpy(ptr + 44, samples.data(), dataSize);
    return wav;
}

QByteArray synthHighBeep()
{
    const int sampleRate = 44100;
    const double duration = 0.35;
    const int numSamples = static_cast<int>(duration * sampleRate);
    std::vector<int16_t> samples(numSamples);
    const int attack = static_cast<int>(sampleRate * 0.01);
    const int decay = static_cast<int>(sampleRate * 0.05);
    const double amp = 26000.0;

    for (int i = 0; i < numSamples; ++i)
    {
        const double t = static_cast<double>(i) / sampleRate;
        double env = 1.0;
        if (i < attack)
            env = static_cast<double>(i) / attack;
        else if (i > numSamples - decay)
            env = static_cast<double>(numSamples - i) / decay;
        samples[i] = static_cast<int16_t>(amp * env * std::sin(2.0 * M_PI * 880.0 * t));
    }
    return createWav(samples, sampleRate);
}

QByteArray synthLowBeep()
{
    const int sampleRate = 44100;
    const double duration = 0.40;
    const int numSamples = static_cast<int>(duration * sampleRate);
    std::vector<int16_t> samples(numSamples);
    const int attack = static_cast<int>(sampleRate * 0.02);
    const int decay = static_cast<int>(sampleRate * 0.08);
    const double amp = 26000.0;

    for (int i = 0; i < numSamples; ++i)
    {
        const double t = static_cast<double>(i) / sampleRate;
        double env = 1.0;
        if (i < attack)
            env = static_cast<double>(i) / attack;
        else if (i > numSamples - decay)
            env = static_cast<double>(numSamples - i) / decay;
        const double wave = 0.85 * std::sin(2.0 * M_PI * 440.0 * t) + 0.15 * std::sin(2.0 * M_PI * 880.0 * t);
        samples[i] = static_cast<int16_t>(amp * env * wave);
    }
    return createWav(samples, sampleRate);
}

QByteArray synthWabble()
{
    const int sampleRate = 44100;
    const double cycleDuration = 0.28;
    const int totalCycles = 2;
    const double duration = cycleDuration * totalCycles;
    const int numSamples = static_cast<int>(duration * sampleRate);
    std::vector<int16_t> samples(numSamples);
    const double amp = 25000.0;
    double phase = 0.0;
    const int attack = static_cast<int>(sampleRate * 0.01);
    const int decay = static_cast<int>(sampleRate * 0.04);

    for (int i = 0; i < numSamples; ++i)
    {
        const double t = static_cast<double>(i) / sampleRate;
        const double cycleT = std::fmod(t, cycleDuration);
        const double cycleProgress = cycleT / cycleDuration;

        const double baseFreq = 400.0 + 450.0 * (0.5 - 0.5 * std::cos(M_PI * cycleProgress));
        const double wabbleMod = 70.0 * std::sin(2.0 * M_PI * 20.0 * t) * cycleProgress;
        const double currentFreq = baseFreq + wabbleMod;

        phase += 2.0 * M_PI * currentFreq / sampleRate;
        if (phase > 2.0 * M_PI)
            phase -= 2.0 * M_PI;

        double env = 1.0;
        if (i < attack)
            env = static_cast<double>(i) / attack;
        else if (i > numSamples - decay)
            env = static_cast<double>(numSamples - i) / decay;

        samples[i] = static_cast<int16_t>(amp * env * std::sin(phase));
    }
    return createWav(samples, sampleRate);
}

QByteArray synthFairyWalk()
{
    const int sampleRate = 44100;
    const std::vector<double> notes = {1046.50, 1174.66, 1318.51, 1567.98, 2093.00};
    const double noteDuration = 0.11;
    const double totalDuration = noteDuration * notes.size() + 0.15;
    const int numSamples = static_cast<int>(totalDuration * sampleRate);
    std::vector<int16_t> samples(numSamples, 0);

    for (size_t n = 0; n < notes.size(); ++n)
    {
        const double freq = notes[n];
        const int startSample = static_cast<int>(n * noteDuration * sampleRate);
        const int noteSamples = static_cast<int>(0.22 * sampleRate);
        const double noteAmp = 18000.0;

        for (int i = 0; i < noteSamples; ++i)
        {
            const int targetIdx = startSample + i;
            if (targetIdx >= numSamples)
                break;
            const double t = static_cast<double>(i) / sampleRate;
            double env = std::exp(-14.0 * t);
            if (i < sampleRate * 0.003)
                env *= (static_cast<double>(i) / (sampleRate * 0.003));
            const double wave = 0.8 * std::sin(2.0 * M_PI * freq * t) + 0.2 * std::sin(2.0 * M_PI * (freq * 2.0) * t);
            int32_t val = samples[targetIdx] + static_cast<int32_t>(noteAmp * env * wave);
            if (val > 32767)
                val = 32767;
            if (val < -32768)
                val = -32768;
            samples[targetIdx] = static_cast<int16_t>(val);
        }
    }
    return createWav(samples, sampleRate);
}
} // namespace

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

    if (settings_.soundType == "Custom Audio File")
    {
        if (!settings_.customSoundPath.isEmpty())
        {
            playFile(settings_.customSoundPath);
        }
        return;
    }

    playBuiltInSound(settings_.soundType);
}

void AudioPlayer::playPreview(const QString &soundTypeOrPath)
{
    if (soundTypeOrPath.isEmpty() || soundTypeOrPath == "Silent")
    {
        return;
    }

    if (soundTypeOrPath == "High Beep" || soundTypeOrPath == "Default Beep" ||
        soundTypeOrPath == "Low Beep" || soundTypeOrPath == "Wabble" ||
        soundTypeOrPath == "Fairy Walk")
    {
        playBuiltInSound(soundTypeOrPath);
    }
    else
    {
        playFile(soundTypeOrPath);
    }
}

void AudioPlayer::playBuiltInSound(const QString &soundName)
{
    const QString name = (soundName == "Default Beep") ? "High Beep" : soundName;
    const QString path = ensureBuiltInSoundFile(name);
    if (!path.isEmpty())
    {
        playFile(path);
    }
}

QString AudioPlayer::ensureBuiltInSoundFile(const QString &soundName)
{
    QString name = soundName;
    if (name == "Default Beep")
    {
        name = "High Beep";
    }

    if (builtInSoundPaths_.contains(name) && QFile::exists(builtInSoundPaths_[name]))
    {
        return builtInSoundPaths_[name];
    }

    QString fileName;
    QByteArray wavData;

    if (name == "High Beep")
    {
        fileName = "standUP_high_beep.wav";
        wavData = synthHighBeep();
    }
    else if (name == "Low Beep")
    {
        fileName = "standUP_low_beep.wav";
        wavData = synthLowBeep();
    }
    else if (name == "Wabble")
    {
        fileName = "standUP_wabble.wav";
        wavData = synthWabble();
    }
    else if (name == "Fairy Walk")
    {
        fileName = "standUP_fairy_walk.wav";
        wavData = synthFairyWalk();
    }
    else
    {
        return {};
    }

    const QString fullPath = QDir::temp().filePath(fileName);
    if (!QFile::exists(fullPath))
    {
        QFile file(fullPath);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(wavData);
            file.close();
        }
    }

    builtInSoundPaths_[name] = fullPath;
    return fullPath;
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
