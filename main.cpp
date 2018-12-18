#include "qfirebase.h"
#include "userlistobject.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<QFirebase>("Firebase", 1, 0, "FirebaseBackend");
    qmlRegisterType<UserListObject>("UserListObject", 1, 0, "UserListObject");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
