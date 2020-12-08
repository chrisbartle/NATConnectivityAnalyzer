#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NATConnectivityAnalyzer.h"
#include <QtNetwork/qnetworkinterface.h>

class NATConnectivityAnalyzer : public QMainWindow
{
    Q_OBJECT

public:
    NATConnectivityAnalyzer(QWidget *parent = Q_NULLPTR);

private:
    Ui::NATConnectivityAnalyzerClass ui;
};
