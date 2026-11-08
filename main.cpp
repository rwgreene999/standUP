#include <QApplication>
#include <QIcon>

#include "standUP.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QIcon appIcon;
    for (int size : {16, 24, 32, 48, 64, 128, 256, 512})
    {
        appIcon.addFile(QStringLiteral(":/icons/standUP-%1.png").arg(size), QSize(size, size));
    }
    QApplication::setWindowIcon(appIcon);

    standUP window;
    window.show();
    return app.exec();
}