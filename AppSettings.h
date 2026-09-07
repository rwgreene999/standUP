#pragma once

#include <QString>

struct AppSettings
{
    QString soundType = "Default Beep";
    QString customSoundPath;
    bool topmost = false;
    bool flashScreen = true;
    QString theme = "Dark";
};
