#include <QApplication>

#include "standUP.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    standUP window;
    window.show();
    return app.exec();
}