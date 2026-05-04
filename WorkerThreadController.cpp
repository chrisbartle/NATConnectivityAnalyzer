#include "WorkerThreadController.h"
#include <QtNetwork/qnetworkinterface.h>
#include <pcpnatpmp.h>

//Logger callback function that is handed to libpcpnatpmp
void libpcpnatpmplogger(pcp_loglvl_e level, const char *message) {
    const char *level_str = "INFO";

    // Map the enum to strings for better readability
    switch (level) {
    case PCP_LOGLVL_ERR:   level_str = "ERROR"; break;
    case PCP_LOGLVL_WARN:  level_str = "WARN";  break;
    case PCP_LOGLVL_DEBUG: level_str = "DEBUG"; break;
    default:               level_str = "LOG";   break;
    }

    // Print to stderr or a file
    clog << "[" << level_str << "] " << message << "\r\n";
    //fprintf(stderr, "[%s] %s\n", level_str, message);
}



WorkerThreadController::WorkerThreadController(QObject *parent)
    : QObject{parent}
{}

//LogStatus updates the displayed status and the internal log
void WorkerThreadController::UpdateStatus(QString inStatus)
{
    setCurrentProcessingStatus(inStatus);
    clog << "*** " << inStatus.toStdString() << "\n";
}

void WorkerThreadController::DoNATAnalysis()
{
    char ntop_buff[INET6_ADDRSTRLEN];

    setIsProcessingNow(true);
    setInternalIP("");
    setNatType("");
    setExternalIP("");
    setNatTestLog("");

    //Stunner outputs debug info into the clog stream. Redirect it into a buffer
    //We'll use clog for all our stuff too
    std::stringstream stunLogStream;
    std::streambuf* old_clog = std::clog.rdbuf(stunLogStream.rdbuf());


    //Get internal IP
    UpdateStatus("Getting Internal IP");
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
    clog << internalIPString.toStdString() << "\n";

    //Get the STUN info
    QString natTypeString;
    UpdateStatus("Running STUN test");
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

    //Extract the external IP
    UpdateStatus("Requesting external IP");
    sockaddr_in extIP;
    QString stunExternalIP;
    if (clientHelper.GetStunMappedAddress(&extIP))
    {
        stunExternalIP = inet_ntoa(extIP.sin_addr);
        setExternalIP(stunExternalIP);
        UpdateStatus("STUN identified external IP as " + stunExternalIP);
    }

    //Now we'll try to set up port mapping on the NAT device
    //CStunClientHelper has code to run WSAStartup and WSACleanup. As long as it's in scope then internet connectivity should work fine.
    //Let's start with pcpnatpmp as it supports both PCP and PMP.
    UpdateStatus("Attempting to map a port with PCP/PMP");
    QString mappedExternalIP;
    int mappedHostPort = clientHelper.GetRandomPort();
    int mappedExternalPort = clientHelper.GetRandomPort();
    clog << "Picked host port " << mappedHostPort << std::endl;
    clog << "Picked external port " << mappedExternalPort << std::endl;
    //Setup logging
//    pcp_set_loggerfn(libpcpnatpmplogger);
//    pcp_log_level = PCP_LOGLVL_DEBUG;
    //Initialize the library
    pcp_ctx_t *ctx = pcp_init(ENABLE_AUTODISCOVERY, NULL);
    sockaddr_in hostAddr;
    hostAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    hostAddr.sin_family = AF_INET;
    hostAddr.sin_port = htons(mappedHostPort);
    sockaddr_in externalAddr;
    externalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    externalAddr.sin_family = AF_INET;
    externalAddr.sin_port = htons(mappedExternalPort);
    //Attempt to open the connection
    pcp_flow_t *pcpFlow  = pcp_new_flow(ctx, (struct sockaddr*)&hostAddr,
                                    NULL,
                                    (struct sockaddr*)&externalAddr,
                                    IPPROTO_UDP, 300, NULL);
    pcp_fstate_e pcpWaitStatus = pcp_wait(pcpFlow, 2000, 0);
    //Extract the mapping data
    size_t pcpMappingCnt = 0;
    pcp_flow_info_t *pcpExtMap = pcp_flow_get_info(pcpFlow, &pcpMappingCnt);
    pcp_flow_info_t *currentPCPExtMap = pcpExtMap;
    //pcpExtMap may actually contain multiple copies of pcp_flow_info_t arranged contiguously.
    //We need to iterate through them and check all of the results
    for (int pcpMapIterator = 0; (pcpMapIterator < pcpMappingCnt); pcpMapIterator++)
    {
        //Ignore any non successful results
        if (currentPCPExtMap->result != pcp_state_succeeded)
            continue;
        QString pcpExtIP = inet_ntop(AF_INET6, &pcpExtMap->ext_ip, ntop_buff, sizeof(ntop_buff));
        //Ignore any IPV6 addresses (IPv4 address are prepended with ::ffff:
        if (pcpExtIP.startsWith("::ffff:"))
        {
            //It's IPv4!
            pcpExtIP.remove("::ffff:");
            UpdateStatus("PCP library identified external IP as " + pcpExtIP);
            mappedExternalIP = pcpExtIP;
        }
        else
            clog << "Ignoring IPv6 PCP map for " + pcpExtIP.toStdString();
        currentPCPExtMap++;
    }
    if (pcpExtMap)
        free(pcpExtMap);

    //Confirm that the external IP that the router reported is the same as the one that STUN found previously.
    //We need to have this information in order to proceed further. No point in more diagnostics unless both the router
    //and STUN are responding
    if (!stunExternalIP.isEmpty() && !mappedExternalIP.isEmpty())
    {

    }

    //todo Clean up PCP/PMP open ports
    pcp_terminate(ctx, 0);

    //Get the log output and restore the original stream
    QString stunLog = QString::fromStdString(stunLogStream.str());
    setNatTestLog(stunLog);
    std::clog.rdbuf(old_clog);

    UpdateStatus("Testing Completed");

    setIsProcessingNow(false);

}
