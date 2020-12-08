#include "NATConnectivityAnalyzer.h"

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
}
