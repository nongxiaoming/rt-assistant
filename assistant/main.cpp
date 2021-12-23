#include "frmmain.h"
#include <QApplication>
#include "api/appinit.h"

#define APP_QT_VERSION_CHECK(major, minor, patch) (QT_VERSION >= QT_VERSION_CHECK(major, minor, patch))

void loadDXcbPlugin(int argc, char *argv[])
{
#if APP_QT_VERSION_CHECK(5,4,0)
    #if APP_QT_VERSION_CHECK(5,6,0)
      Q_UNUSED(argc);
      Q_UNUSED(argv);
      QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #else
      QApplication a(argc, argv);
      qputenv("QT_DEVICE_PIXEL_RATIO", "auto");
      QScreen *screen = QApplication::primaryScreen();
      qreal dpi = screen->logicalDotsPerInch()/96;
      qputenv("QT_SCALE_FACTOR", QByteArray::number(dpi));
      Q_UNUSED(a);
    #endif
#else
    Q_UNUSED(argc);
    Q_UNUSED(argv);
#endif
}

int main(int argc, char *argv[])
{
    loadDXcbPlugin(argc, argv);
    QApplication a(argc, argv);

    AppInit *init = new AppInit;
    init->Load();

    frmMain w;
    w.show();

    return a.exec();
}
