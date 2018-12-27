#include "qfirebaseuser.h"
#include "qfirebaseroom.h"
#include "imageitem.h"

#include <qqmlcontext.h>
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

    bool is_ios = false;

    #ifdef Q_OS_IOS
        qmlRegisterType<IOSWebView>("IOSWebView", 1, 0, "IOSWebView");

        is_ios = true;
    #endif

    //qmlRegisterType<IOSCamera>("IOSCamera", 1, 0, "IOSCamera");

    qRegisterMetaType<QFirebaseUser*>("QFirebaseUser *");

    qmlRegisterType<QFirebaseUser>("FirebaseUser", 1, 0, "FirebaseUserBackend");
    qmlRegisterType<QFirebaseRoom>("FirebaseRoom", 1, 0, "FirebaseRoomBackend");
    qmlRegisterType<ImageItem>("FirebaseImage", 1, 0, "FirebaseImageItem");

    QQmlApplicationEngine engine;

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("_is_ios", is_ios);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
