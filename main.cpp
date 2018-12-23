#include "qfirebaseroom.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

//#include "ioscamera.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //qmlRegisterType<IOSCamera>("IOSCamera", 1, 0, "IOSCamera");

    qRegisterMetaType<QFirebaseUser*>("QFirebaseUser *");

    qmlRegisterType<QFirebaseUser>("FirebaseUser", 1, 0, "FirebaseUserBackend");
    qmlRegisterType<QFirebaseRoom>("FirebaseRoom", 1, 0, "FirebaseRoomBackend");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
