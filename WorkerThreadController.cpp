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

    //Get the STUN info
    setCurrentProcessingStatus("Running STUN test");
    QString natTypeString;
    CStunClientHelper clientHelper(m_stunServer.toStdString().c_str());
    NAT_TYPE natType = clientHelper.GetNatType();
    switch (natType)
    {
    case NAT_TYPE::ERROR_DETECTING_NAT:
        natTypeString = "There was an error detecting NAT.";
        break;

    case NAT_TYPE::FIREWALL_BLOCKS_UDP:
        natTypeString = "There is a firewall that blocks UDP.";
        break;

    case NAT_TYPE::FULL_CONE_NAT:
        natTypeString = "The NAT type is Full Cone NAT.";
        break;

    case NAT_TYPE::OPEN_INTERNET:
        natTypeString = "There is no NAT and directly on Open Internet.";
        break;

    case NAT_TYPE::RESTRICTED_CONE_NAT:
        natTypeString = "The NAT type is Restricted Cone NAT.";
        break;

    case NAT_TYPE::RESTRICTED_PORT_CONE_NAT:
        natTypeString = "The NAT type is Restricted Port Cone NAT.";
        break;

    case NAT_TYPE::SYMMETRIC_NAT:
        natTypeString = "The NAT type is Symmetric NAT.";
        break;

    case NAT_TYPE::SYMMETRIC_UDP_FIREWALL:
        natTypeString = "There is a symmetric UDP firewall.";
        break;
    }
    //emit updatedNATType(natTypeString);

    //Extract the external IP
    SOCKADDR_IN extIP;
    if (clientHelper.GetStunMappedAddress(&extIP))
    {
        QString extIPString = inet_ntoa(extIP.sin_addr);
        setExternalIP(extIPString);
    }

    setCurrentProcessingStatus("Testing Completed");

    setIsProcessingNow(false);

}
