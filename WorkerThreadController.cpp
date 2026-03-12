#include "WorkerThreadController.h"
#include <QtNetwork/qnetworkinterface.h>
#include "Stunner/StunClientHelper.h"

WorkerThreadController::WorkerThreadController(QObject *parent)
    : QObject{parent}
{}

void WorkerThreadController::DoNATAnalysis()
{
    setIsProcessingNow(true);

    //Get internal IP
    setCurrentProcessingStatus("Getting Internal IP");
    QString internalIPString;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (int rut = 0; rut < list.count(); rut++)
        if (!list[rut].isLoopback())
            if (list[rut].protocol() == QAbstractSocket::IPv4Protocol)
            {
                if (!internalIPString.isEmpty())
                    internalIPString.append(";");
                internalIPString.append(list[rut].toString());
            }
    setInternalIP(internalIPString);

    setIsProcessingNow(false);

}
