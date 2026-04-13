#include "WorkerThreadController.h"
#include <QtNetwork/qnetworkinterface.h>

WorkerThreadController::WorkerThreadController(QObject *parent)
    : QObject{parent}
{}

void WorkerThreadController::DoNATAnalysis()
{
    setIsProcessingNow(true);
    setInternalIP("");
    setNatType("");
    setExternalIP("");
    setNatTestLog("");

    //Get internal IP
    setCurrentProcessingStatus("Getting Internal IP");
    QString internalIPString;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (int rut = 0; rut < list.count(); rut++)
        if (!list[rut].isLoopback())
            if (list[rut].protocol() == QAbstractSocket::IPv4Protocol)
            {
                if (!internalIPString.isEmpty())
                    internalIPString.append("\r\n");
                internalIPString.append(list[rut].toString());
            }
    setInternalIP(internalIPString);

    //Get the STUN info
    setCurrentProcessingStatus("Running STUN test");
    //Stunner outputs debug info into the clog stream. Redirect it into a buffer
    std::stringstream stunLogStream;
    std::streambuf* old_clog = std::clog.rdbuf(stunLogStream.rdbuf());
    QString natTypeString;
    CStunClientHelper clientHelper(m_stunServer.toStdString().c_str());
    NAT_TYPE natType = clientHelper.GetNatType();
    switch (natType)
    {
    case NAT_TYPE::ERROR_DETECTING_NAT:
        natTypeString = "ERROR_DETECTING_NAT";
        break;

    case NAT_TYPE::FIREWALL_BLOCKS_UDP:
        natTypeString = "FIREWALL_BLOCKS_UDP";
        break;

    case NAT_TYPE::FULL_CONE_NAT:
        natTypeString = "FULL_CONE_NAT";
        break;

    case NAT_TYPE::OPEN_INTERNET:
        natTypeString = "OPEN_INTERNET";
        break;

    case NAT_TYPE::RESTRICTED_CONE_NAT:
        natTypeString = "RESTRICTED_CONE_NAT";
        break;

    case NAT_TYPE::RESTRICTED_PORT_CONE_NAT:
        natTypeString = "RESTRICTED_PORT_CONE_NAT";
        break;

    case NAT_TYPE::SYMMETRIC_NAT:
        natTypeString = "SYMMETRIC_NAT";
        break;

    case NAT_TYPE::SYMMETRIC_UDP_FIREWALL:
        natTypeString = "SYMMETRIC_UDP_FIREWALL";
        break;
    }
    setNatType(natTypeString);
    //Get the log output and restore the original stream
    QString stunLog = QString::fromStdString(stunLogStream.str());
    setNatTestLog(stunLog);
    std::clog.rdbuf(old_clog);

    //Extract the external IP
    sockaddr_in extIP;
    if (clientHelper.GetStunMappedAddress(&extIP))
    {
        QString extIPString = inet_ntoa(extIP.sin_addr);
        setExternalIP(extIPString);
    }

    setCurrentProcessingStatus("Testing Completed");

    setIsProcessingNow(false);

}
