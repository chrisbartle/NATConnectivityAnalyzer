#pragma once

#include <QtWidgets/QMainWindow>
#include <qthread.h>
#include "ui_NATConnectivityAnalyzer.h"
#include "NATConnectivityAnalyzerLogic.h"

class NATConnectivityAnalyzer : public QMainWindow
{
    Q_OBJECT

public:
    NATConnectivityAnalyzer(QWidget *parent = Q_NULLPTR);

private:
    Ui::NATConnectivityAnalyzerClass ui;
    CNATConnectivityAnalyzerLogic logic;
    QThread *processingThread;

private slots:
    void on_analyzeButton_clicked();
    void startAnalysis();
    void analysisDone();
};
