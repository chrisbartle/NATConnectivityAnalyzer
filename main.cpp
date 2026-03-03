#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "MainThreadController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //Instantiate the main thread controller that we'll be using
    //Singleton names must start with a capital letter
    MainThreadController controller;
    qmlRegisterSingletonInstance("App.Controller", 1, 0, "Controller", &controller);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("NATConnectivityAnalyzer", "Main");

    return app.exec();
}
