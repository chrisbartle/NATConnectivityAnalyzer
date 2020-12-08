#include "NATConnectivityAnalyzer.h"

NATConnectivityAnalyzer::NATConnectivityAnalyzer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //Get internal IP
//    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    ui.internalIP->setText("blah");
}
