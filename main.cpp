#include "NATConnectivityAnalyzer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NATConnectivityAnalyzer w;
    w.show();
    return a.exec();
}
