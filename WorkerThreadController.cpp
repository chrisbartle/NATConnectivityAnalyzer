#include "WorkerThreadController.h"
#include <QtNetwork/qnetworkinterface.h>
#include <pcpnatpmp.h>
#include <miniupnpc.h>
#include <upnpcommands.h>

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
    clog << "\n*** " << inStatus.toStdString() << "\n";
}

void WorkerThreadController::DoNATAnalysis()
{
    char ntop_buff[INET6_ADDRSTRLEN];

    setIsProcessingNow(true);
    setInternalIP("");
    setNatType("");
    setPortForwardType("");
    setExternalIP("");
    setRouterReportedExternalIP("");
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
    UpdateStatus("Requesting external IP");
    CStunClientHelper clientHelper(m_stunServer.toStdString().c_str());
    //Extract the external IP
    sockaddr_in extIP;
    QString stunExternalIP;
    if (clientHelper.GetStunMappedAddress(&extIP))
    {
        stunExternalIP = inet_ntoa(extIP.sin_addr);
        setExternalIP(stunExternalIP);
        UpdateStatus("STUN identified external IP as " + stunExternalIP);
    }

    //Run the STUN test
    QString natTypeString;
    UpdateStatus("Running STUN test");
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

    //Now we'll try to set up port mapping on the NAT device
    //CStunClientHelper has code to run WSAStartup and WSACleanup. As long as it's in scope then internet connectivity should work fine.
    //Let's start with pcpnatpmp as it supports both PCP and PMP.
    UpdateStatus("Initializing PCP/PMP");
    QString portForwardingType;
    QString mappedExternalIP;
    int mappedHostPort = clientHelper.GetRandomPort();
    QString mappedHostPortStr = QString::number(mappedHostPort);
    int mappedExternalPort = clientHelper.GetRandomPort();
    QString mappedExternalPortStr = QString::number(mappedExternalPort);
    clog << "Picked host port " << mappedHostPortStr.toStdString() << std::endl;
    clog << "Picked external port " << mappedExternalPortStr.toStdString() << std::endl;
    //Setup logging
    pcp_set_loggerfn(libpcpnatpmplogger);
    pcp_log_level = PCP_LOGLVL_DEBUG;
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

    //Set up UPnP. Even if we don't use it, it can give useful information
    UpdateStatus("Initializing UPnP");
    int upnpError;
    UPNPDev* devlist = upnpDiscover(
        2000,     // delay in ms
        nullptr,  // multicast interface
        nullptr,  // minissdpd socket
        0,        // sameport
        0,        // ipv6
        2,        // ttl
        &upnpError
        );
    UPNPUrls upnpUrls;
    IGDdatas upnpData;
    char upnpLanaddr[64];
    char upnpWanaddr[64];
    int igdResult = UPNP_GetValidIGD(
        devlist,
        &upnpUrls,
        &upnpData,
        upnpLanaddr,
        sizeof(upnpLanaddr),
        upnpWanaddr,
        sizeof(upnpWanaddr)
        );
    if (igdResult == UPNP_CONNECTED_IGD)
    {
        //Connected to the router. Copy out the status info
        QString upnpStatus;
        char externalIPAddress[40];
        char connectionType[64];
        char status[64];
        char lastconnerr[64];
        unsigned int uptime = 0;
        unsigned int brUp, brDown;
        time_t timenow, timestarted;
        int r;
        if(UPNP_GetConnectionTypeInfo(upnpUrls.controlURL,
                                       upnpData.first.servicetype,
                                       connectionType) != UPNPCOMMAND_SUCCESS)
            upnpStatus += QString::asprintf("GetConnectionTypeInfo failed.\n");
        else
            upnpStatus += QString::asprintf("Connection Type : %s\n", connectionType);
        if(UPNP_GetStatusInfo(upnpUrls.controlURL, upnpData.first.servicetype,
                               status, &uptime, lastconnerr) != UPNPCOMMAND_SUCCESS)
            upnpStatus += QString::asprintf("GetStatusInfo failed.\n");
        else
            upnpStatus += QString::asprintf("Status : %s, uptime=%us, LastConnectionError : %s\n",
                   status, uptime, lastconnerr);
        if(uptime > 0) {
            timenow = time(NULL);
            timestarted = timenow - uptime;
            upnpStatus += QString::asprintf("  Time started : %s", ctime(&timestarted));
        }
        if(UPNP_GetLinkLayerMaxBitRates(upnpUrls.controlURL_CIF, upnpData.CIF.servicetype,
                                         &brDown, &brUp) != UPNPCOMMAND_SUCCESS) {
            upnpStatus += QString::asprintf("GetLinkLayerMaxBitRates failed.\n");
        } else {
            upnpStatus += QString::asprintf("MaxBitRateDown : %u bps", brDown);
            if(brDown >= 1000000) {
                upnpStatus += QString::asprintf(" (%u.%u Mbps)", brDown / 1000000, (brDown / 100000) % 10);
            } else if(brDown >= 1000) {
                upnpStatus += QString::asprintf(" (%u Kbps)", brDown / 1000);
            }
            upnpStatus += QString::asprintf("   MaxBitRateUp %u bps", brUp);
            if(brUp >= 1000000) {
                upnpStatus += QString::asprintf(" (%u.%u Mbps)", brUp / 1000000, (brUp / 100000) % 10);
            } else if(brUp >= 1000) {
                upnpStatus += QString::asprintf(" (%u Kbps)", brUp / 1000);
            }
            upnpStatus += QString::asprintf("\n");
        }
        r = UPNP_GetExternalIPAddress(upnpUrls.controlURL,
                                      upnpData.first.servicetype,
                                      externalIPAddress);
        if(r != UPNPCOMMAND_SUCCESS) {
            upnpStatus += QString::asprintf("GetExternalIPAddress failed. (errorcode=%d)\n", r);
        } else if(!externalIPAddress[0]) {
            upnpStatus += QString::asprintf("GetExternalIPAddress failed. (empty string)\n");
        } else {
            upnpStatus += QString::asprintf("ExternalIPAddress = %s\n", externalIPAddress);
        }
        //Add it to the log
        clog << "UPnP Router Status:\n" << upnpStatus.toStdString() << "\n";
    }

    //Attempt to open the connection
    bool mappedUsingPCP = false;
    bool mappedUsingUPnP = false;
    UpdateStatus("Attempting to forward a port with PCP/PMP");
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
            mappedUsingPCP = true;
        }
        else
            clog << "Ignoring IPv6 PCP map for " + pcpExtIP.toStdString();
        currentPCPExtMap++;
    }
    if (pcpExtMap)
        free(pcpExtMap);

    if (!mappedUsingPCP)
    {
        //We were unable to forward a port using PCP/PMP so we'll fall back to UPnP
        UpdateStatus("Attempting to forward a port with UPnP");
        int portMappingResult = UPNP_AddPortMapping(
            upnpUrls.controlURL,
            upnpData.first.servicetype,
            mappedExternalPortStr.toStdString().c_str(),
            mappedHostPortStr.toStdString().c_str(),
            upnpLanaddr,
            "NATConnectivityAnalyzer",
            "UDP",
            nullptr,
            "300"
            );
        if (portMappingResult == UPNPCOMMAND_SUCCESS)
        {
            UpdateStatus("UPnP library identified external IP as " + QString(upnpWanaddr));
            mappedExternalIP = upnpWanaddr;
            mappedUsingUPnP = true;
        }
    }

    //Confirm that the external IP that the router reported is the same as the one that STUN found previously.
    //We need to have this information in order to proceed further. No point in more diagnostics unless both the router
    //and STUN are responding
    if (!stunExternalIP.isEmpty() && !mappedExternalIP.isEmpty())
    {
        //The STUN server should see the same IP address that the router is reporting. If they're different
        //then a double NAT has been detected
        if (stunExternalIP != mappedExternalIP)
        {
            clog << "The STUN server is reporting IP " << stunExternalIP.toStdString() << " but the router is reporting that its port forwarding from "
                 << mappedExternalIP.toStdString() << ". This indicates the presence of a double NAT!" << std::endl;
            setRouterReportedExternalIP(mappedExternalIP);
            portForwardingType = "DOUBLE_NAT";
        }
        else
        {
            //Now that the port is forwarded, we'll send out a packet to the STUN server and see what port it sees
            CStunMessage *pResponseMessage = NULL;
            clog << "Running NAT test I" << std::endl;
            if (clientHelper.TestOne (*clientHelper.GetServerAddress(), hostAddr, &pResponseMessage))
            {
                sockaddr_in externalAddressSeenBySTUN;
                ((CStunBindingResponseMessage *)pResponseMessage)->GetMappedAddress (&externalAddressSeenBySTUN);
                if (externalAddressSeenBySTUN.sin_port == externalAddr.sin_port)
                {
                    clog << "The forwarded port matches the port that the STUN server sees. That means that this is a bidirectional mapping." << std::endl;
                    portForwardingType = "BIDIRECTIONAL";
                }
                else
                {
                    clog << "The forwarded port does not match the port that the STUN server sees. That means that this is a monodirectional mapping." << std::endl;
                    portForwardingType = "MONODIRECTIONAL";
                }
                //Now run NAT test II to verify that the port forwarding does indeed work
                if (!clientHelper.TestTwo (*clientHelper.GetServerAddress(), hostAddr, &pResponseMessage))
                {
                    clog << "The STUN server sent a packet to the forwarded port using a different IP and we didn't receive it. Port forwarding is apparently not working for 3rd parties." << std::endl;
                    portForwardingType = "BLOCKED";
                }
                else
                    clog << "The STUN server sent a packet to the forwarding port using a different IP and we received it! This confirms that port forwarding is working as expected!" << std::endl;
            }
            else
            {
                clog << "NAT test I returned failure" << std::endl;
                portForwardingType = "FAILED";
            }
        }
    }
    else if (!mappedExternalIP.isEmpty())
    {
        clog << "Port forwarding works but cant test without a STUN server" << std::endl;
        portForwardingType = "FAILED";
    }
    else
        portForwardingType = "NONE";
    setPortForwardType(portForwardingType);

    UpdateStatus("Closing forwarded ports");
    //Clean up PCP/PMP open ports
    if (ctx)
    {
        //In theory I should simply by able to call pcp_terminate with the flag set to 1 and it would
        //clean up everything and close the forwarded ports but there appears to be a bug in the library
        //so we'll close the flow seperately.
        pcp_close_flow(pcpFlow);
        pcp_delete_flow(pcpFlow);
        pcp_terminate(ctx, 0);
    }
    //Clean up UPnP
    if (mappedUsingUPnP)
    {
        clog << "Closing UPnP connection for external port " << mappedExternalPortStr.toStdString() << std::endl;
        UPNP_DeletePortMapping(
            upnpUrls.controlURL,
            upnpData.first.servicetype,
            mappedExternalPortStr.toStdString().c_str(),
            "UDP",
            nullptr
            );
    }
    FreeUPNPUrls(&upnpUrls);
    freeUPNPDevlist(devlist);

    //Get the log output and restore the original stream
    QString stunLog = QString::fromStdString(stunLogStream.str());
    setNatTestLog(stunLog);
    std::clog.rdbuf(old_clog);

    UpdateStatus("Testing Completed");

    setIsProcessingNow(false);

}
