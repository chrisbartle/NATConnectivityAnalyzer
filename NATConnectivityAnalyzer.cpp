#include "NATConnectivityAnalyzer.h"
#include "Stunner/StunClientHelper.h"

NATConnectivityAnalyzer::NATConnectivityAnalyzer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //Get internal IP
    QString internalIPString;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (int rut=0; rut<list.count(); rut++)
        if (!list[rut].isLoopback())
            if (list[rut].protocol() == QAbstractSocket::IPv4Protocol)
            {
                if (!internalIPString.isEmpty())
                    internalIPString.append(";");
                internalIPString.append(list[rut].toString());
            }
    ui.internalIP->setText(internalIPString);

    //Get the STUN info
	QString natTypeString;
    CStunClientHelper clientHelper("stun.xten.com");
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
	ui.natType->setText(natTypeString);
}
