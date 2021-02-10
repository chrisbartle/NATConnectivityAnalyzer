#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include "NATConnectivityAnalyzerLogic.h"
#include "NATConnectivityAnalyzerLogicRelay.h"

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
    QThread processingThread;
    CNATConnectivityAnalyzerLogic businessLogic;
    businessLogic.moveToThread(&processingThread);
    processingThread.start();
    //In an ideal world, we could just call our business logic object directly
    //Unfortunately, it's now located on a different thread and QML can't reference
    //it directly. Instead we'll use a relay object, that's on the main thread, to
    //pass the signals all around.
    CNATConnectivityAnalyzerLogicRelay businessLogicRelay;
    //Now wire the slots and signals together between the processing object and the relay
    QObject::connect(&businessLogicRelay, SIGNAL(startFullAnalysis()), &businessLogic, SLOT(DoFullAnalysis()));
    QObject::connect(&businessLogic, SIGNAL(fullAnalysisCompleted()), &businessLogicRelay, SIGNAL(fullAnalysisCompleted()));
    QObject::connect(&businessLogic, SIGNAL(updatedStatus(QString)), &businessLogicRelay, SIGNAL(updatedStatus(QString)));
    QObject::connect(&businessLogic, SIGNAL(updatedInternalIP(QString)), &businessLogicRelay, SIGNAL(updatedInternalIP(QString)));
    QObject::connect(&businessLogic, SIGNAL(updatedExternalIP(QString)), &businessLogicRelay, SIGNAL(updatedExternalIP(QString)));
    QObject::connect(&businessLogic, SIGNAL(updatedNATType(QString)), &businessLogicRelay, SIGNAL(updatedNATType(QString)));

    engine.rootContext()->setContextProperty("businessLogic", &businessLogicRelay);
     //Wire everything together
 //   QObject::connect(&businessLogic, SIGNAL(updatedInternalIP(QString)), ui.internalIP, SLOT(setText(QString)));
 //   QObject::connect(&businessLogic, SIGNAL(updatedExternalIP(QString)), ui.externalIP, SLOT(setText(QString)));
 //   QObject::connect(&businessLogic, SIGNAL(updatedNATType(QString)), ui.natType, SLOT(setText(QString)));

    int ret = app.exec();

    //Cleanup
    processingThread.quit();

    return ret;
}
