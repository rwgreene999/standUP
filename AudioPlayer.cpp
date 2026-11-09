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
#include <QRandomGenerator>
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

QByteArray synthWhoopWhoop()
{
    const int sampleRate = 44100;
    const double whoopDur = 0.18;
    const double pauseDur = 0.06;
    const double totalDur = whoopDur * 2 + pauseDur;
    const int numSamples = static_cast<int>(totalDur * sampleRate);
    std::vector<int16_t> samples(numSamples, 0);
    const double amp = 26000.0;

    for (int cycle = 0; cycle < 2; ++cycle)
    {
        const double startT = cycle * (whoopDur + pauseDur);
        const int startSample = static_cast<int>(startT * sampleRate);
        const int cycleSamples = static_cast<int>(whoopDur * sampleRate);
        double phase = 0.0;

        for (int i = 0; i < cycleSamples; ++i)
        {
            const int targetIdx = startSample + i;
            if (targetIdx >= numSamples)
                break;

            const double u = static_cast<double>(i) / cycleSamples;
            const double freq = 480.0 + 870.0 * (u * u);
            phase += 2.0 * M_PI * freq / sampleRate;
            if (phase > 2.0 * M_PI)
                phase -= 2.0 * M_PI;

            double env = 1.0;
            const int attack = static_cast<int>(cycleSamples * 0.08);
            const int decay = static_cast<int>(cycleSamples * 0.15);
            if (i < attack)
                env = static_cast<double>(i) / attack;
            else if (i > cycleSamples - decay)
                env = static_cast<double>(cycleSamples - i) / decay;

            const double wave = 0.85 * std::sin(phase) + 0.15 * std::sin(2.0 * phase);
            samples[targetIdx] = static_cast<int16_t>(amp * env * wave);
        }
    }
    return createWav(samples, sampleRate);
}

QByteArray synthTwinkle()
{
    const int sampleRate = 44100;
    const std::vector<double> notes = {1760.0, 2093.0, 2637.0, 3136.0, 3520.0, 2637.0, 3520.0};
    const double stepTime = 0.075;
    const double totalDur = stepTime * notes.size() + 0.20;
    const int numSamples = static_cast<int>(totalDur * sampleRate);
    std::vector<int16_t> samples(numSamples, 0);

    for (size_t n = 0; n < notes.size(); ++n)
    {
        const double freq = notes[n];
        const int startSample = static_cast<int>(n * stepTime * sampleRate);
        const int noteSamples = static_cast<int>(0.20 * sampleRate);
        const double noteAmp = 15000.0;

        for (int i = 0; i < noteSamples; ++i)
        {
            const int targetIdx = startSample + i;
            if (targetIdx >= numSamples)
                break;

            const double t = static_cast<double>(i) / sampleRate;
            double env = std::exp(-18.0 * t);
            if (i < sampleRate * 0.002)
            {
                env *= (static_cast<double>(i) / (sampleRate * 0.002));
            }
            const double shimmer = 1.0 + 0.15 * std::sin(2.0 * M_PI * 18.0 * t);
            const double wave = std::sin(2.0 * M_PI * freq * t) + 0.25 * std::sin(2.0 * M_PI * (freq * 1.5) * t);

            int32_t val = samples[targetIdx] + static_cast<int32_t>(noteAmp * env * shimmer * wave);
            if (val > 32767)
                val = 32767;
            if (val < -32768)
                val = -32768;
            samples[targetIdx] = static_cast<int16_t>(val);
        }
    }
    return createWav(samples, sampleRate);
}

QByteArray synthTripleLowBeep()
{
    const int sampleRate = 44100;
    const double beepDur = 0.16;
    const double gapDur = 0.08;
    const int numBeeps = 3;
    const double totalDur = numBeeps * beepDur + (numBeeps - 1) * gapDur + 0.05;
    const int numSamples = static_cast<int>(totalDur * sampleRate);
    std::vector<int16_t> samples(numSamples, 0);
    const double amp = 26000.0;

    for (int b = 0; b < numBeeps; ++b)
    {
        const int startSample = static_cast<int>(b * (beepDur + gapDur) * sampleRate);
        const int beepSamples = static_cast<int>(beepDur * sampleRate);
        const int attack = static_cast<int>(sampleRate * 0.015);
        const int decay = static_cast<int>(sampleRate * 0.035);

        for (int i = 0; i < beepSamples; ++i)
        {
            const int targetIdx = startSample + i;
            if (targetIdx >= numSamples)
                break;
            const double t = static_cast<double>(i) / sampleRate;

            double env = 1.0;
            if (i < attack)
                env = static_cast<double>(i) / attack;
            else if (i > beepSamples - decay)
                env = static_cast<double>(beepSamples - i) / decay;

            const double wave = 0.85 * std::sin(2.0 * M_PI * 180.0 * t) + 0.15 * std::sin(2.0 * M_PI * 360.0 * t);
            samples[targetIdx] = static_cast<int16_t>(amp * env * wave);
        }
    }
    return createWav(samples, sampleRate);
}

QByteArray synthFrog()
{
    const int sampleRate = 44100;
    const double croak1Dur = 0.16;
    const double gapDur = 0.07;
    const double croak2Dur = 0.32;
    const double totalDur = croak1Dur + gapDur + croak2Dur + 0.1;
    const int numSamples = static_cast<int>(totalDur * sampleRate);
    std::vector<int16_t> samples(numSamples, 0);

    struct CroakPart
    {
        double start;
        double dur;
        double baseFreq;
        double endFreq;
        double rate;
    };
    CroakPart parts[2] = {
        {0.0, croak1Dur, 130.0, 160.0, 45.0},
        {croak1Dur + gapDur, croak2Dur, 120.0, 95.0, 38.0}
    };

    for (int p = 0; p < 2; ++p)
    {
        const int startSample = static_cast<int>(parts[p].start * sampleRate);
        const int partSamples = static_cast<int>(parts[p].dur * sampleRate);
        double phase = 0.0;
        double pulsePhase = 0.0;

        for (int i = 0; i < partSamples; ++i)
        {
            const int targetIdx = startSample + i;
            if (targetIdx >= numSamples)
                break;
            const double progress = static_cast<double>(i) / partSamples;

            const double freq = parts[p].baseFreq + (parts[p].endFreq - parts[p].baseFreq) * progress;
            phase += 2.0 * M_PI * freq / sampleRate;
            if (phase > 2.0 * M_PI)
                phase -= 2.0 * M_PI;

            pulsePhase += 2.0 * M_PI * parts[p].rate / sampleRate;
            if (pulsePhase > 2.0 * M_PI)
                pulsePhase -= 2.0 * M_PI;

            double rasp = 0.5 + 0.5 * std::cos(pulsePhase);
            rasp = std::pow(rasp, 1.8);

            const double form1 = std::sin(phase);
            const double form2 = 0.4 * std::sin(phase * 2.3);
            const double form3 = 0.25 * std::sin(phase * 4.7);

            double env = 1.0;
            const int attack = static_cast<int>(partSamples * 0.12);
            const int decay = static_cast<int>(partSamples * 0.25);
            if (i < attack)
                env = static_cast<double>(i) / attack;
            else if (i > partSamples - decay)
                env = static_cast<double>(partSamples - i) / decay;

            const double wave = (form1 + form2 + form3) * rasp;
            double val = 25000.0 * env * wave;
            if (val > 32767.0)
                val = 32767.0;
            if (val < -32768.0)
                val = -32768.0;
            samples[targetIdx] = static_cast<int16_t>(val);
        }
    }
    return createWav(samples, sampleRate);
}

QByteArray synthHummingbird()
{
    const int sampleRate = 44100;
    const double totalDur = 5.0; // 5 seconds
    const int numSamples = static_cast<int>(totalDur * sampleRate);
    std::vector<int16_t> samples(numSamples, 0);

    struct Chirp
    {
        double time;
        double freq1;
        double freq2;
        double dur;
    };
    std::vector<Chirp> chirps = {
        {0.8, 3200.0, 4100.0, 0.08},
        {1.9, 3600.0, 4400.0, 0.07},
        {3.1, 3300.0, 4200.0, 0.09},
        {4.2, 3500.0, 4500.0, 0.08}
    };

    double wingPhase = 0.0;
    for (int i = 0; i < numSamples; ++i)
    {
        const double t = static_cast<double>(i) / sampleRate;

        const double flutterFreq = 68.0 + 3.0 * std::sin(2.0 * M_PI * 1.5 * t);
        wingPhase += 2.0 * M_PI * flutterFreq / sampleRate;
        if (wingPhase > 2.0 * M_PI)
            wingPhase -= 2.0 * M_PI;

        const double wingTone = 0.5 * std::sin(wingPhase)
                              + 0.3 * std::sin(2.0 * wingPhase)
                              + 0.15 * std::sin(3.0 * wingPhase)
                              + 0.08 * std::sin(4.0 * wingPhase);

        const double hoverWobble = 0.85 + 0.15 * std::sin(2.0 * M_PI * 11.0 * t);

        double overallEnv = 1.0;
        if (t < 0.2)
            overallEnv = t / 0.2;
        else if (t > 4.7)
            overallEnv = (5.0 - t) / 0.3;

        double sampleVal = 14000.0 * overallEnv * hoverWobble * wingTone;

        for (const auto &c : chirps)
        {
            if (t >= c.time && t <= c.time + c.dur)
            {
                const double dt = t - c.time;
                const double u = dt / c.dur;
                const double chirpFreq = c.freq1 + (c.freq2 - c.freq1) * u;
                const double chirpEnv = std::sin(M_PI * u);
                const double chirpWave = std::sin(2.0 * M_PI * chirpFreq * dt);
                sampleVal += 12000.0 * chirpEnv * chirpWave;
            }
        }

        if (sampleVal > 32767.0)
            sampleVal = 32767.0;
        if (sampleVal < -32768.0)
            sampleVal = -32768.0;
        samples[i] = static_cast<int16_t>(sampleVal);
    }
    return createWav(samples, sampleRate);
}

QByteArray synthSonarPing()
{
    const int sampleRate = 44100;
    const double totalDur = 1.8;
    const int numSamples = static_cast<int>(totalDur * sampleRate);
    std::vector<int16_t> samples(numSamples, 0);

    const double pingFreq = 920.0;
    for (int i = 0; i < numSamples; ++i)
    {
        const double t = static_cast<double>(i) / sampleRate;
        const double attack = (i < sampleRate * 0.008) ? (static_cast<double>(i) / (sampleRate * 0.008)) : 1.0;
        const double decay = std::exp(-2.8 * t);

        const double wave = 0.85 * std::sin(2.0 * M_PI * pingFreq * t)
                          + 0.10 * std::sin(2.0 * M_PI * (pingFreq * 0.5) * t)
                          + 0.05 * std::sin(2.0 * M_PI * (pingFreq * 2.0) * t);

        double val = 26000.0 * attack * decay * wave;
        if (val > 32767.0)
            val = 32767.0;
        if (val < -32768.0)
            val = -32768.0;
        samples[i] = static_cast<int16_t>(val);
    }
    return createWav(samples, sampleRate);
}

QByteArray synthWindChime()
{
    const int sampleRate = 44100;
    const double totalDur = 2.0;
    const int numSamples = static_cast<int>(totalDur * sampleRate);
    std::vector<int16_t> samples(numSamples, 0);

    struct ChimeStrike
    {
        double time;
        double freq;
        double amp;
    };
    std::vector<ChimeStrike> strikes = {
        {0.00, 1318.5, 9000.0},
        {0.12, 1975.5, 8000.0},
        {0.25, 2489.0, 7500.0},
        {0.42, 1661.2, 8500.0},
        {0.65, 2960.0, 7000.0},
        {0.85, 1975.5, 6500.0},
        {1.05, 1318.5, 6000.0}
    };

    for (const auto &s : strikes)
    {
        const int startSample = static_cast<int>(s.time * sampleRate);
        const int strikeSamples = static_cast<int>((totalDur - s.time) * sampleRate);

        for (int i = 0; i < strikeSamples; ++i)
        {
            const int targetIdx = startSample + i;
            if (targetIdx >= numSamples)
                break;
            const double t = static_cast<double>(i) / sampleRate;

            const double attack = (i < sampleRate * 0.003) ? (static_cast<double>(i) / (sampleRate * 0.003)) : 1.0;
            const double decay = std::exp(-4.2 * t);

            const double wave = 0.8 * std::sin(2.0 * M_PI * s.freq * t)
                              + 0.2 * std::sin(2.0 * M_PI * (s.freq * 2.76) * t);

            int32_t val = samples[targetIdx] + static_cast<int32_t>(s.amp * attack * decay * wave);
            if (val > 32767)
                val = 32767;
            if (val < -32768)
                val = -32768;
            samples[targetIdx] = static_cast<int16_t>(val);
        }
    }
    return createWav(samples, sampleRate);
}

QString canonicalBuiltInName(const QString &soundName)
{
    if (soundName.compare("Default Beep", Qt::CaseInsensitive) == 0 ||
        soundName.compare("High Beep", Qt::CaseInsensitive) == 0)
    {
        return "High Beep";
    }
    if (soundName.compare("Low Beep", Qt::CaseInsensitive) == 0)
    {
        return "Low Beep";
    }
    if (soundName.compare("Triple Low Beep", Qt::CaseInsensitive) == 0 ||
        soundName.compare("Low Beep 3x", Qt::CaseInsensitive) == 0)
    {
        return "Triple Low Beep";
    }
    if (soundName.compare("Frog", Qt::CaseInsensitive) == 0)
    {
        return "Frog";
    }
    if (soundName.compare("Hummingbird", Qt::CaseInsensitive) == 0)
    {
        return "Hummingbird";
    }
    if (soundName.compare("Sonar Ping", Qt::CaseInsensitive) == 0)
    {
        return "Sonar Ping";
    }
    if (soundName.compare("Wind Chime", Qt::CaseInsensitive) == 0)
    {
        return "Wind Chime";
    }
    if (soundName.compare("Wabble", Qt::CaseInsensitive) == 0)
    {
        return "Wabble";
    }
    if (soundName.compare("Fairy Walk", Qt::CaseInsensitive) == 0)
    {
        return "Fairy Walk";
    }
    if (soundName.compare("Whoop Whoop", Qt::CaseInsensitive) == 0)
    {
        return "Whoop Whoop";
    }
    if (soundName.compare("Twinkle", Qt::CaseInsensitive) == 0)
    {
        return "Twinkle";
    }
    return soundName;
}

bool isBuiltInSoundName(const QString &soundName)
{
    return AppSettings::builtInSoundNames().contains(canonicalBuiltInName(soundName));
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

    if (settings_.soundType == AppSettings::randomSoundName())
    {
        playRandomSound();
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
    if (soundTypeOrPath.isEmpty() || soundTypeOrPath.compare("Silent", Qt::CaseInsensitive) == 0)
    {
        return;
    }

    if (soundTypeOrPath == AppSettings::randomSoundName())
    {
        playRandomSound();
        return;
    }

    if (isBuiltInSoundName(soundTypeOrPath))
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
    const QString name = canonicalBuiltInName(soundName);
    const QString path = ensureBuiltInSoundFile(name);
    if (!path.isEmpty())
    {
        playFile(path);
    }
}

void AudioPlayer::playRandomSound()
{
    const QStringList pool = randomSoundPool();
    if (pool.isEmpty())
    {
        return;
    }

    QString pick;
    if (pool.size() == 1)
    {
        pick = pool.first();
    }
    else
    {
        do
        {
            pick = pool.at(QRandomGenerator::global()->bounded(static_cast<int>(pool.size())));
        } while (pick == lastRandomSound_);
    }

    lastRandomSound_ = pick;
    if (pick == "Custom Audio File")
    {
        playFile(settings_.customSoundPath);
    }
    else
    {
        playBuiltInSound(pick);
    }
}

QStringList AudioPlayer::randomSoundPool() const
{
    QStringList pool = AppSettings::builtInSoundNames();
    if (!settings_.customSoundPath.isEmpty())
    {
        const QFileInfo info(settings_.customSoundPath);
        if (info.exists() && info.isFile())
        {
            pool.append("Custom Audio File");
        }
    }
    return pool;
}

QString AudioPlayer::ensureBuiltInSoundFile(const QString &soundName)
{
    const QString name = canonicalBuiltInName(soundName);

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
    else if (name == "Triple Low Beep")
    {
        fileName = "standUP_triple_low_beep.wav";
        wavData = synthTripleLowBeep();
    }
    else if (name == "Frog")
    {
        fileName = "standUP_frog.wav";
        wavData = synthFrog();
    }
    else if (name == "Hummingbird")
    {
        fileName = "standUP_hummingbird.wav";
        wavData = synthHummingbird();
    }
    else if (name == "Sonar Ping")
    {
        fileName = "standUP_sonar_ping.wav";
        wavData = synthSonarPing();
    }
    else if (name == "Wind Chime")
    {
        fileName = "standUP_wind_chime.wav";
        wavData = synthWindChime();
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
    else if (name == "Whoop Whoop")
    {
        fileName = "standUP_whoop_whoop.wav";
        wavData = synthWhoopWhoop();
    }
    else if (name == "Twinkle")
    {
        fileName = "standUP_twinkle.wav";
        wavData = synthTwinkle();
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
