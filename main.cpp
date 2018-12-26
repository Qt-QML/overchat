#include "qfirebaseuser.h"
#include "qfirebaseroom.h"
#include "imageitem.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#ifdef Q_OS_IOS
    #include "ioswebview.h"
#endif

//#include "ioscamera.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    #ifdef Q_OS_IOS
        qmlRegisterType<IOSWebView>("IOSWebView", 1, 0, "IOSWebView");
    #endif

    //qmlRegisterType<IOSCamera>("IOSCamera", 1, 0, "IOSCamera");

    qRegisterMetaType<QFirebaseUser*>("QFirebaseUser *");

    qmlRegisterType<QFirebaseUser>("FirebaseUser", 1, 0, "FirebaseUserBackend");
    qmlRegisterType<QFirebaseRoom>("FirebaseRoom", 1, 0, "FirebaseRoomBackend");
    qmlRegisterType<ImageItem>("FirebaseImage", 1, 0, "FirebaseImageItem");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
