#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLoggingCategory>
#include <QQmlContext>

#include "deviceFinder.h"
#include "deviceHandler.h"
#include "optionsmodel.h"
#include "optionshandler.h"


int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));

    QGuiApplication app(argc, argv);


    OptionsHandler optionsHandler;

    DeviceHandler deviceHandler;

    DeviceFinder deviceFinder(&deviceHandler);

//    deviceFinder.startSearch();

    qmlRegisterSingletonType(QUrl("qrc:/Settings.qml"), "Settings", 1, 0, "Settings");

    qmlRegisterType<OptionsModel>("Options", 1, 0, "OptionsModel");
    qmlRegisterUncreatableType<OptionsHandler>("Options", 1, 0, "OptionsHandler", QStringLiteral("OptionsList should not be created in QML"));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("optionsHandler", &optionsHandler);
    engine.rootContext()->setContextProperty("deviceFinder", &deviceFinder);
    engine.rootContext()->setContextProperty("deviceHandler", &deviceHandler);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));



    return app.exec();
}
