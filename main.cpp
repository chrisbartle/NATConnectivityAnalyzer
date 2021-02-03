#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext.h>
#include "NATConnectivityAnalyzerLogic.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    //Set everything up with the business logic
    CNATConnectivityAnalyzerLogic businessLogic;
    engine.rootContext()->setContextProperty("businessLogic", &businessLogic);
    engine.rootObjects();
    //Wire everything together
 //   QObject::connect(&businessLogic, SIGNAL(updatedInternalIP(QString)), ui.internalIP, SLOT(setText(QString)));
 //   QObject::connect(&businessLogic, SIGNAL(updatedExternalIP(QString)), ui.externalIP, SLOT(setText(QString)));
 //   QObject::connect(&businessLogic, SIGNAL(updatedNATType(QString)), ui.natType, SLOT(setText(QString)));

    return app.exec();
}
