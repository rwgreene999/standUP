#pragma once

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QString>
#include <QStringList>

struct AppSettings
{
    // Concrete choices (the "Random" entries are added by the settings panel).
    static const QStringList &themeNames()
    {
        static const QStringList names = {
            "Dark", "Light", "Red", "Green", "Sunset",
            "Sunrise", "Spring", "Ocean", "Brown", "Ice"};
        return names;
    }

    static QString randomThemeName() { return QStringLiteral("Random"); }

    static const QStringList &builtInSoundNames()
    {
        static const QStringList names = {
            "High Beep", "Low Beep", "Triple Low Beep", "Frog", "Hummingbird",
            "Sonar Ping", "Wind Chime", "Wabble", "Fairy Walk", "Whoop Whoop", "Twinkle"};
        return names;
    }

    static QString randomSoundName() { return QStringLiteral("Random Sound"); }

    QString soundType = "High Beep";
    QString customSoundPath;
    bool topmost = false;
    bool flashScreen = true;
    QString theme = "Dark";

    static QString configFilePath()
    {
        const QString dirPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/standUP";
        return dirPath + "/settings.json";
    }

    void load()
    {
        const QString path = configFilePath();
        QFile file(path);
        if (!file.exists() || !file.open(QIODevice::ReadOnly))
        {
            return;
        }

        const QByteArray data = file.readAll();
        file.close();

        QJsonParseError parseError;
        const QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject())
        {
            return;
        }

        const QJsonObject obj = doc.object();
        if (obj.contains("soundType") && obj["soundType"].isString())
        {
            soundType = obj["soundType"].toString();
            if (soundType == "Default Beep")
            {
                soundType = "High Beep";
            }
        }
        if (obj.contains("customSoundPath") && obj["customSoundPath"].isString())
        {
            customSoundPath = obj["customSoundPath"].toString();
        }
        if (obj.contains("topmost") && obj["topmost"].isBool())
        {
            topmost = obj["topmost"].toBool();
        }
        if (obj.contains("flashScreen") && obj["flashScreen"].isBool())
        {
            flashScreen = obj["flashScreen"].toBool();
        }
        if (obj.contains("theme") && obj["theme"].isString())
        {
            theme = obj["theme"].toString();
        }
    }

    void save() const
    {
        const QString path = configFilePath();
        const QFileInfo fileInfo(path);
        QDir dir = fileInfo.dir();
        if (!dir.exists())
        {
            dir.mkpath(".");
        }

        QJsonObject obj;
        obj["soundType"] = soundType;
        obj["customSoundPath"] = customSoundPath;
        obj["topmost"] = topmost;
        obj["flashScreen"] = flashScreen;
        obj["theme"] = theme;

        const QJsonDocument doc(obj);
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
        }
    }
};
