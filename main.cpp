#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "deviceFinder.h"
#include "deviceHandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    DeviceHandler deviceHandler;

    DeviceFinder deviceFinder(&deviceHandler);

//    deviceFinder.startSearch();

    qmlRegisterSingletonType(QUrl("qrc:/Settings.qml"), "Settings", 1, 0, "Settings");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("deviceFinder", &deviceFinder);

    engine.load(url);

    return app.exec();
}
