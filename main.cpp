#include <QApplication>
#include <QIcon>
#include <QProxyStyle>

#include "standUP.h"

class ToolTipStyle : public QProxyStyle
{
public:
    using QProxyStyle::QProxyStyle;

    int styleHint(StyleHint hint,
                  const QStyleOption *option = nullptr,
                  const QWidget *widget = nullptr,
                  QStyleHintReturn *returnData = nullptr) const override
    {
        if (hint == QStyle::SH_ToolTip_WakeUpDelay)
            return 200; // milliseconds; 0 = instant, default is 700

        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QIcon appIcon;
    for (int size : {16, 24, 32, 48, 64, 128, 256, 512})
    {
        appIcon.addFile(QStringLiteral(":/icons/standUP-%1.png").arg(size), QSize(size, size));
    }
    QApplication::setWindowIcon(appIcon);
    app.setStyle(new ToolTipStyle(app.style())); // 200 ms tooltips

    standUP window;
    window.show();
    return app.exec();
}
