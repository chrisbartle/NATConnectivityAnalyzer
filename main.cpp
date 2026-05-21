#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "MainThreadController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //The Windows style doesn't do padding quite right plus it makes sense to standardize
    //on a single style. This appears to be well in both Linux (it's the default there)
    //and Windows
    qputenv("QT_QUICK_CONTROLS_STYLE", "Fusion");

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
